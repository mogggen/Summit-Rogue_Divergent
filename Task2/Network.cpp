#include "Network.h"
#include <cstring>
#include <iostream>
#include <cerrno>
#include <chrono>
#include <thread>

#ifdef _WIN32
#pragma comment(lib, "ws2_32.lib")
#endif

static const size_t BULLET_STATE_SIZE = sizeof(float) * 5 + sizeof(uint8_t);  // 21
static const size_t PLAYER_STATE_SIZE = sizeof(float) * 3 + sizeof(uint8_t) * 6 + BULLET_STATE_SIZE * BULLETS_PER_PLAYER;

size_t playerStatePacketSize() {
	return PLAYER_STATE_SIZE;
}

static void writeFloat(uint8_t*& p, float f) {
	memcpy(p, &f, sizeof(float));
	p += sizeof(float);
}
static void readFloat(const uint8_t*& p, float& f) {
	memcpy(&f, p, sizeof(float));
	p += sizeof(float);
}

bool serializePlayerState(const PlayerState& s, std::vector<uint8_t>& out) {
	out.resize(PLAYER_STATE_SIZE);
	uint8_t* p = out.data();
	writeFloat(p, s.worldX);
	writeFloat(p, s.worldY);
	writeFloat(p, s.aim);
	*p++ = s.facing;
	*p++ = s.walkAnim;
	*p++ = s.heartCount;
	*p++ = s.ammoCount;
	*p++ = s.isReloading;
	*p++ = s.playerId;
	for (int i = 0; i < BULLETS_PER_PLAYER; i++) {
		writeFloat(p, s.bullets[i].startX);
		writeFloat(p, s.bullets[i].startY);
		writeFloat(p, s.bullets[i].travelX);
		writeFloat(p, s.bullets[i].travelY);
		writeFloat(p, s.bullets[i].aim);
		*p++ = s.bullets[i].isFired;
	}
	return true;
}

bool deserializePlayerState(const uint8_t* data, size_t len, PlayerState& out) {
	if (len < PLAYER_STATE_SIZE) return false;
	const uint8_t* p = data;
	readFloat(p, out.worldX);
	readFloat(p, out.worldY);
	readFloat(p, out.aim);
	out.facing = *p++;
	out.walkAnim = *p++;
	out.heartCount = *p++;
	out.ammoCount = *p++;
	out.isReloading = *p++;
	out.playerId = *p++;
	for (int i = 0; i < BULLETS_PER_PLAYER; i++) {
		readFloat(p, out.bullets[i].startX);
		readFloat(p, out.bullets[i].startY);
		readFloat(p, out.bullets[i].travelX);
		readFloat(p, out.bullets[i].travelY);
		readFloat(p, out.bullets[i].aim);
		out.bullets[i].isFired = *p++;
	}
	return true;
}

void networkInit() {
#ifdef _WIN32
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
		std::cerr << "WSAStartup failed\n";
	}
#endif
}

void networkQuit() {
#ifdef _WIN32
	WSACleanup();
#endif
}

static void setNonBlocking(socket_t s) {
#ifdef _WIN32
	u_long mode = 1;
	ioctlsocket(s, FIONBIO, &mode);
#else
	int flags = fcntl(s, F_GETFL, 0);
	fcntl(s, F_SETFL, flags | O_NONBLOCK);
#endif
}

static void closeSocket(socket_t s) {
	if (s == INVALID_SOCKET) return;
#ifdef _WIN32
	closesocket(s);
#else
	close(s);
#endif
}

// --- Server ---
NetworkServer::NetworkServer() : listenSock(INVALID_SOCKET), valid(false) {}

NetworkServer::~NetworkServer() {
	for (socket_t c : clientSockets)
		closeSocket(c);
	clientSockets.clear();
	if (listenSock != INVALID_SOCKET) {
		closeSocket(listenSock);
		listenSock = INVALID_SOCKET;
	}
}

bool NetworkServer::start() {
	listenSock = socket(AF_INET, SOCK_STREAM, 0);
	if (listenSock == INVALID_SOCKET) {
		std::cerr << "Server: socket failed\n";
		return false;
	}
	int opt = 1;
#ifdef _WIN32
	setsockopt(listenSock, SOL_SOCKET, SO_REUSEADDR, (const char*)&opt, sizeof(opt));
#else
	setsockopt(listenSock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
#endif
	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons((uint16_t)NETWORK_PORT);
	if (bind(listenSock, (struct sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR) {
		std::cerr << "Server: bind failed\n";
		closeSocket(listenSock);
		listenSock = INVALID_SOCKET;
		return false;
	}
	if (listen(listenSock, MAX_PLAYERS) == SOCKET_ERROR) {
		std::cerr << "Server: listen failed\n";
		closeSocket(listenSock);
		listenSock = INVALID_SOCKET;
		return false;
	}
	setNonBlocking(listenSock);
	valid = true;
	std::cout << "Server lyssnar på port " << NETWORK_PORT << ". Väntar på klienter...\n";
	return true;
}

void NetworkServer::tick(const PlayerState& hostState) {
	// Accept new clients
	struct sockaddr_in clientAddr;
	socklen_t len = sizeof(clientAddr);
	socket_t client = accept(listenSock, (struct sockaddr*)&clientAddr, &len);
	if (client != INVALID_SOCKET) {
		setNonBlocking(client);
		uint8_t playerId = (uint8_t)(1 + clientSockets.size());
		clientSockets.push_back(client);
		clientStates.push_back(PlayerState());
		// Send CONNECT_ACK
		uint8_t ack[2] = { (uint8_t)CONNECT_ACK, playerId };
		send(client, (const char*)ack, 2, 0);
		std::cout << "Klient ansluten (spelare " << (int)playerId << ")\n";
	}

	// Receive from each client
	for (size_t i = 0; i < clientSockets.size(); i++) {
		uint8_t buf[512];
		int n = recv(clientSockets[i], (char*)buf, PLAYER_STATE_SIZE, 0);
		if (n == (int)PLAYER_STATE_SIZE) {
			PlayerState ps;
			if (deserializePlayerState(buf, n, ps)) {
				ps.playerId = (uint8_t)(i + 1);
				clientStates[i] = ps;
			}
		} else if (n <= 0) {
			bool disconnected = (n == 0);
#ifdef _WIN32
			if (n == SOCKET_ERROR && WSAGetLastError() != WSAEWOULDBLOCK) disconnected = true;
#else
			if (n == -1 && errno != EAGAIN && errno != EWOULDBLOCK) disconnected = true;
#endif
			if (disconnected) {
				closeSocket(clientSockets[i]);
				clientSockets.erase(clientSockets.begin() + i);
				clientStates.erase(clientStates.begin() + i);
				i--;
			}
		}
	}

	// Build full state: host (id 0) + clients
	std::vector<uint8_t> packet;
	packet.push_back((uint8_t)STATE_UPDATE);
	packet.push_back((uint8_t)(1 + clientStates.size()));
	std::vector<uint8_t> psBuf;
	serializePlayerState(hostState, psBuf);
	packet.insert(packet.end(), psBuf.begin(), psBuf.end());
	for (const auto& cs : clientStates) {
		psBuf.clear();
		serializePlayerState(cs, psBuf);
		packet.insert(packet.end(), psBuf.begin(), psBuf.end());
	}

	// Send to all clients
	for (socket_t c : clientSockets) {
		send(c, (const char*)packet.data(), (int)packet.size(), 0);
	}
}

void NetworkServer::getFullState(std::vector<PlayerState>& out) const {
	out.clear();
	// Host state is not stored here; caller adds it. We only have client states.
	out = clientStates;
}

// --- Client ---
NetworkClient::NetworkClient() : sock(INVALID_SOCKET), myPlayerId(0), valid(false) {}

NetworkClient::~NetworkClient() {
	if (sock != INVALID_SOCKET) {
		closeSocket(sock);
		sock = INVALID_SOCKET;
	}
}

bool NetworkClient::connectTo(const std::string& host, int port) {
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) {
		std::cerr << "Client: socket failed\n";
		return false;
	}
	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons((uint16_t)port);
	if (inet_pton(AF_INET, host.c_str(), &addr.sin_addr) <= 0) {
		std::cerr << "Client: ogiltig adress\n";
		closeSocket(sock);
		sock = INVALID_SOCKET;
		return false;
	}
	if (connect(sock, (struct sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR) {
		std::cerr << "Client: kunde inte ansluta till " << host << ":" << port << "\n";
		closeSocket(sock);
		sock = INVALID_SOCKET;
		return false;
	}
	setNonBlocking(sock);

	// Wait for CONNECT_ACK (block briefly)
	uint8_t ack[2];
	int total = 0;
	for (int tries = 0; tries < 50 && total < 2; tries++) {
		int n = recv(sock, (char*)ack + total, 2 - total, 0);
		if (n > 0) total += n;
		else if (n <= 0) {
#ifdef _WIN32
			if (n == SOCKET_ERROR && WSAGetLastError() != WSAEWOULDBLOCK) break;
#else
			if (n == -1 && errno != EAGAIN && errno != EWOULDBLOCK) break;
#endif
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}
	if (total < 2 || ack[0] != CONNECT_ACK) {
		std::cerr << "Client: fick inte CONNECT_ACK\n";
		closeSocket(sock);
		sock = INVALID_SOCKET;
		return false;
	}
	myPlayerId = ack[1];
	valid = true;
	std::cout << "Ansluten som spelare " << (int)myPlayerId << "\n";
	return true;
}

void NetworkClient::tick(const PlayerState& myState, std::vector<PlayerState>& outFullState) {
	outFullState.clear();
	// Send my state to host
	std::vector<uint8_t> psBuf;
	serializePlayerState(myState, psBuf);
	send(sock, (const char*)psBuf.data(), (int)psBuf.size(), 0);

	// Receive full state (STATE_UPDATE)
	uint8_t buf[4096];
	int n = recv(sock, (char*)buf, sizeof(buf), 0);
	if (n >= 2) {
		uint8_t type = buf[0];
		uint8_t numPlayers = buf[1];
		if (type == STATE_UPDATE && numPlayers > 0 && numPlayers <= MAX_PLAYERS) {
			size_t offset = 2;
			for (int i = 0; i < numPlayers && offset + PLAYER_STATE_SIZE <= (size_t)n; i++) {
				PlayerState ps;
				if (deserializePlayerState(buf + offset, PLAYER_STATE_SIZE, ps)) {
					ps.playerId = (uint8_t)i;
					outFullState.push_back(ps);
				}
				offset += PLAYER_STATE_SIZE;
			}
		}
	}
}
