#pragma once

#include <cstdint>
#include <vector>
#include <string>

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
typedef SOCKET socket_t;
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
typedef int socket_t;
#define INVALID_SOCKET (-1)
#define SOCKET_ERROR (-1)
#endif

const int NETWORK_PORT = 5555;
const int MAX_PLAYERS = 8;
const int BULLETS_PER_PLAYER = 10;

struct BulletState {
	float startX, startY;
	float travelX, travelY;
	float aim;
	uint8_t isFired;
};

struct PlayerState {
	float worldX, worldY;
	float aim;
	uint8_t facing;
	uint8_t walkAnim;
	uint8_t heartCount;
	uint8_t ammoCount;
	uint8_t isReloading;
	BulletState bullets[BULLETS_PER_PLAYER];
	uint8_t playerId;
};

enum MessageType : uint8_t {
	CONNECT_ACK = 1,
	STATE_UPDATE = 2
};

class NetworkServer {
	socket_t listenSock;
	std::vector<socket_t> clientSockets;
	std::vector<PlayerState> clientStates;
	bool valid;
public:
	NetworkServer();
	~NetworkServer();
	bool start();
	void tick(const PlayerState& hostState);
	void getFullState(std::vector<PlayerState>& out) const;
	int getClientCount() const { return (int)clientSockets.size(); }
	bool isValid() const { return valid; }
};

class NetworkClient {
	socket_t sock;
	uint8_t myPlayerId;
	bool valid;
public:
	NetworkClient();
	~NetworkClient();
	bool connectTo(const std::string& host, int port = NETWORK_PORT);
	void tick(const PlayerState& myState, std::vector<PlayerState>& outFullState);
	uint8_t getMyPlayerId() const { return myPlayerId; }
	bool isValid() const { return valid; }
};

void networkInit();
void networkQuit();
bool serializePlayerState(const PlayerState& s, std::vector<uint8_t>& out);
bool deserializePlayerState(const uint8_t* data, size_t len, PlayerState& out);
size_t playerStatePacketSize();
