#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include <cmath>
#include "Rectangle.h"
#include "Circle.h"
#include "Bullet.h"
#include "Network.h"
#include "SDL.h"
#include "SDL_image.h"

float aiming(float px, float py, float mx, float my)
{
	if (mx <= px && my <= py)
		return atan(float(py - my) / float(px - mx)) - M_PI;
	else if (mx <= px && my >= py)
		return atan(float(py - my) / float(px - mx)) - M_PI;
	else if (mx >= px && my >= py)
		return atan(float(my - py) / float(mx - px));
	else if (mx >= px && my <= py)
		return atan(float(py - my) / float(px - mx));
	return 0.f;
}

static void fillPlayerState(PlayerState& ps, float worldX, float worldY, int facing) {
	ps.worldX = worldX;
	ps.worldY = worldY;
	ps.facing = (uint8_t)facing;
}

int main(int argc, char* argv[])
{
	bool isInMenu = true;
	srand((unsigned)time(0));
	const char title[18] = "Enter the Gungeon";
	int enemies = 0;
	bool quit = false;
	int windowWidth = 1920, windowHeight = 1200;

	// Nätverk: --host (värd) eller --join [ip] (klient, default 127.0.0.1)
	bool isHost = false;
	bool isClient = false;
	std::string joinHost = "127.0.0.1";
	for (int i = 1; i < argc; i++) {
		std::string arg = argv[i];
		if (arg == "--host") isHost = true;
		else if (arg == "--join") {
			isClient = true;
			if (i + 1 < argc && argv[i + 1][0] != '-') { joinHost = argv[i + 1]; i++; }
		}
	}
	bool isNetworked = isHost || isClient;
	uint8_t myPlayerId = 0;
	NetworkServer* server = nullptr;
	NetworkClient* client = nullptr;
	std::vector<PlayerState> allPlayers;

	SDL_Init(SDL_INIT_EVERYTHING);
	if (isNetworked) networkInit();
	SDL_Event event;
	std::cout << "----" << title << "----" << std::endl;
	std::cout << "Resolution: " << windowWidth << "x" << windowHeight << std::endl;
	if (isNetworked) {
		if (isHost) std::cout << "Värd (localhost:" << NETWORK_PORT << ")" << std::endl;
		else std::cout << "Klient -> " << joinHost << std::endl;
	}
	std::cout << "Right-click to move | Left-click to shoot | Edge-scroll with mouse | SPACE to jump";
	SDL_Window *window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowWidth, windowHeight, SDL_WINDOW_SHOWN);
	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);

	IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);
	std::string resourceDir = "resources/";
	SDL_Texture* mapTexture = IMG_LoadTexture(renderer, (resourceDir + "map.jpg").c_str());
	if (!mapTexture)
	{
		resourceDir = "../resources/";
		mapTexture = IMG_LoadTexture(renderer, (resourceDir + "map.jpg").c_str());
	}
	int mapWidth = 0, mapHeight = 0;
	if (mapTexture)
		SDL_QueryTexture(mapTexture, nullptr, nullptr, &mapWidth, &mapHeight);
	const int mapZoom = 5;

	// Game of Life grid over the map
	const int cellSize = 32;
	int worldWidth = (mapWidth > 0 && mapHeight > 0) ? (mapWidth * mapZoom) : windowWidth * 2;
	int worldHeight = (mapWidth > 0 && mapHeight > 0) ? (mapHeight * mapZoom) : windowHeight * 2;
	int gridCols = (worldWidth + cellSize - 1) / cellSize;
	int gridRows = (worldHeight + cellSize - 1) / cellSize;
	std::vector<std::vector<bool>> grid(gridRows, std::vector<bool>(gridCols, false));
	std::vector<std::vector<bool>> gridNext(gridRows, std::vector<bool>(gridCols, false));

	SDL_Texture* texAliveCell = IMG_LoadTexture(renderer, (resourceDir + "sprites/alive_cell_yellow.png").c_str());
	SDL_Texture* texDeadCell  = IMG_LoadTexture(renderer, (resourceDir + "sprites/dead_cell.png").c_str());
	if (!texAliveCell) texAliveCell = IMG_LoadTexture(renderer, (std::string("../") + resourceDir + "sprites/alive_cell_yellow.png").c_str());
	if (!texDeadCell)  texDeadCell  = IMG_LoadTexture(renderer, (std::string("../") + resourceDir + "sprites/dead_cell.png").c_str());

	std::string spriteBase = resourceDir + "sprites/green_boy/";
	SDL_Texture* walkDown[2] = {
		IMG_LoadTexture(renderer, (spriteBase + "walk-down-0.png").c_str()),
		IMG_LoadTexture(renderer, (spriteBase + "walk-down-1.png").c_str())
	};
	SDL_Texture* walkUp[2] = {
		IMG_LoadTexture(renderer, (std::string(spriteBase) + "walk-up-0.png").c_str()),
		IMG_LoadTexture(renderer, (std::string(spriteBase) + "walk-up-1.png").c_str())
	};
	SDL_Texture* walkLeft[2] = {
		IMG_LoadTexture(renderer, (std::string(spriteBase) + "walk-left-0.png").c_str()),
		IMG_LoadTexture(renderer, (std::string(spriteBase) + "walk-left-1.png").c_str())
	};
	SDL_Texture* walkRight[2] = {
		IMG_LoadTexture(renderer, (std::string(spriteBase) + "walk-right-0.png").c_str()),
		IMG_LoadTexture(renderer, (std::string(spriteBase) + "walk-right-1.png").c_str())
	};

	if (isHost) {
		server = new NetworkServer();
		if (!server->start()) {
			delete server;
			server = nullptr;
			isHost = false;
			isNetworked = false;
		}
	}
	if (isClient) {
		client = new NetworkClient();
		if (!client->connectTo(joinHost)) {
			delete client;
			client = nullptr;
			isClient = false;
			isNetworked = false;
			std::cerr << "Kunde inte ansluta till " << joinHost << ". Spelar ensam.\n";
		} else {
			myPlayerId = client->getMyPlayerId();
		}
	}

	int frameDelay = 17;

	int mx = 0;
	int my = 0;
	int px = 0, py = 0;  // screen position (computed each frame: playerWorld - scroll)
	int v = 5;
	int s = 30;
	// Spawn point (change these to move where the player starts)
	float spawnWorldX = (float)(s * 2);
	float spawnWorldY = (float)(worldHeight - s * 3);
	float playerWorldX = spawnWorldX;
	float playerWorldY = spawnWorldY;

	// League-style: click to move (right-click), edge scroll
	float scrollX = 0, scrollY = 0;
	float targetWorldX = -1, targetWorldY = -1;
	bool hasMoveTarget = false;
	bool isRightMouseHeld = false;
	const int edgeScrollMargin = 50;
	const float edgeScrollSpeed = 12.f;
	int maxScrollX = 0, maxScrollY = 0;
	if (mapWidth > 0 && mapHeight > 0)
	{
		maxScrollX = mapWidth * mapZoom - windowWidth;
		maxScrollY = mapHeight * mapZoom - windowHeight;
		if (maxScrollX < 0) maxScrollX = 0;
		if (maxScrollY < 0) maxScrollY = 0;
	}

	int heartCount = 3;
	int ammoCount = 10;
	bool isReloading = false;
	int reloadingProgress = 0;

	int fireDelay = 0;


	// Walk animation: 0=down, 1=up, 2=left, 3=right
	int lastFacing = 0;
	int walkAnimFrame = 0;
	Uint32 lastWalkAnimTime = 0;
	const Uint32 walkAnimInterval = 120;

	float aim = 0;

	// Game of Life: spawn cells when shooting (left click / hold)
	int golSpawnOffset = 0;
	Uint32 lastGolSpawnTime = 0;
	const Uint32 golSpawnInterval = 80;
	Uint32 lastGolStepTime = 0;
	const Uint32 golStepInterval = 200;
	int deltaTime;

	while (!quit)
	{
		deltaTime = SDL_GetTicks();

		//Read trough remaing events
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_QUIT:
				quit = true;
				break;

			case SDL_MOUSEBUTTONDOWN:
				if (event.button.button == SDL_BUTTON_LEFT)
				{
					// Game of Life: spawn one cell immediately in aim direction
					float pwX = playerWorldX, pwY = playerWorldY;
					float wx = pwX + cellSize * SDL_cosf(aim);
					float wy = pwY + cellSize * SDL_sinf(aim);
					int gx = (int)(wx / cellSize), gy = (int)(wy / cellSize);
					if (gx >= 0 && gx < gridCols && gy >= 0 && gy < gridRows)
						grid[gy][gx] = true;
					lastGolSpawnTime = SDL_GetTicks();
					golSpawnOffset = 1;
				}
				else if (event.button.button == SDL_BUTTON_RIGHT)
				{
					// League-style: move player to click position (world coords)
					isRightMouseHeld = true;
					float clickWorldX = event.button.x + scrollX;
					float clickWorldY = event.button.y + scrollY;
					clickWorldX = (clickWorldX < 0) ? 0 : (clickWorldX > worldWidth - s * 2) ? (float)(worldWidth - s * 2) : clickWorldX;
					clickWorldY = (clickWorldY < 0) ? 0 : (clickWorldY > worldHeight - s * 3) ? (float)(worldHeight - s * 3) : clickWorldY;
					targetWorldX = clickWorldX;
					targetWorldY = clickWorldY;
					hasMoveTarget = true;
				}
				break;

			case SDL_MOUSEBUTTONUP:
				if (event.button.button == SDL_BUTTON_RIGHT)
				{
					isRightMouseHeld = false;
				}
				break;

			case SDL_KEYDOWN:
				switch (event.key.keysym.sym)
				{
				case SDLK_ESCAPE:
					quit = true;
					break;
				}
				break;
			}
		}

		// Continuously update target position while right mouse is held
		if (isRightMouseHeld)
		{
			int mouseX, mouseY;
			SDL_GetMouseState(&mouseX, &mouseY);
			float clickWorldX = mouseX + scrollX;
			float clickWorldY = mouseY + scrollY;
			clickWorldX = (clickWorldX < 0) ? 0 : (clickWorldX > worldWidth - s * 2) ? (float)(worldWidth - s * 2) : clickWorldX;
			clickWorldY = (clickWorldY < 0) ? 0 : (clickWorldY > worldHeight - s * 3) ? (float)(worldHeight - s * 3) : clickWorldY;
			targetWorldX = clickWorldX;
			targetWorldY = clickWorldY;
			hasMoveTarget = true;
		}

		// League-style: move player toward click target
		bool isWalking = false;
		if (hasMoveTarget)
		{
			float dx = targetWorldX - playerWorldX;
			float dy = targetWorldY - playerWorldY;
			float dist = sqrtf(dx * dx + dy * dy);
			const float arriveRadius = 6.f;
			if (dist <= arriveRadius)
			{
				hasMoveTarget = false;
			}
			else
			{
				isWalking = true;
				float step = (v < dist) ? (float)v : dist;
				playerWorldX += (dx / dist) * step;
				playerWorldY += (dy / dist) * step;
				playerWorldX = (playerWorldX < 0) ? 0 : (playerWorldX > worldWidth - s * 2) ? (float)(worldWidth - s * 2) : playerWorldX;
				playerWorldY = (playerWorldY < 0) ? 0 : (playerWorldY > worldHeight - s * 3) ? (float)(worldHeight - s * 3) : playerWorldY;
				if (SDL_fabsf(dx) >= SDL_fabsf(dy))
					lastFacing = (dx > 0) ? 3 : 2;
			else
				lastFacing = (dy > 0) ? 0 : 1;
			}
		}
		if (isWalking)
			std::cout << "Player position: (" << playerWorldX << ", " << playerWorldY << ")" << std::endl;
		Uint32 now = SDL_GetTicks();
		if (isWalking && now - lastWalkAnimTime >= walkAnimInterval)
		{
			lastWalkAnimTime = now;
			walkAnimFrame = 1 - walkAnimFrame;
		}
		if (!isWalking)
			walkAnimFrame = 0;

		// League-style camera: edge scroll when mouse near screen edge, else follow player
		int mouseX, mouseY;
		SDL_GetMouseState(&mouseX, &mouseY);
		bool edgeScrolling = false;
		if (mouseX < edgeScrollMargin && scrollX > 0)
		{
			scrollX -= edgeScrollSpeed;
			if (scrollX < 0) scrollX = 0;
			edgeScrolling = true;
		}
		if (mouseX > windowWidth - edgeScrollMargin && scrollX < maxScrollX)
		{
			scrollX += edgeScrollSpeed;
			if (scrollX > maxScrollX) scrollX = (float)maxScrollX;
			edgeScrolling = true;
		}
		if (mouseY < edgeScrollMargin && scrollY > 0)
		{
			scrollY -= edgeScrollSpeed;
			if (scrollY < 0) scrollY = 0;
			edgeScrolling = true;
		}
		if (mouseY > windowHeight - edgeScrollMargin && scrollY < maxScrollY)
		{
			scrollY += edgeScrollSpeed;
			if (scrollY > maxScrollY) scrollY = (float)maxScrollY;
			edgeScrolling = true;
		}
		if (!edgeScrolling)
		{
			scrollX = playerWorldX - windowWidth / 2.f + (s * 2) / 2.f;
			scrollY = playerWorldY - windowHeight / 2.f + (s * 3) / 2.f;
			if (scrollX < 0) scrollX = 0;
			if (scrollY < 0) scrollY = 0;
			if (scrollX > maxScrollX) scrollX = (float)maxScrollX;
			if (scrollY > maxScrollY) scrollY = (float)maxScrollY;
		}

		// Screen position from world and scroll (for drawing and aim)
		px = (int)(playerWorldX - scrollX);
		py = (int)(playerWorldY - scrollY);

		// Cursor for aim (mx, my already set from edge-scroll block above)
		if (0 < mouseX && mouseX < windowWidth && 0 < mouseY && mouseY < windowHeight)
		{
			mx = mouseX;
			my = mouseY;
		}
		aim = aiming(px + s + s / 2, py + s, mx, my);

		// Game of Life: spawn cells when left mouse held (stream in aim direction)
		Uint32 mouseButtons = SDL_GetMouseState(nullptr, nullptr);
		if (mouseButtons & SDL_BUTTON_LMASK)
		{
			Uint32 now = SDL_GetTicks();
			if (now - lastGolSpawnTime >= golSpawnInterval)
			{
				lastGolSpawnTime = now;
				float wx = playerWorldX + (golSpawnOffset + 1) * cellSize * SDL_cosf(aim);
				float wy = playerWorldY + (golSpawnOffset + 1) * cellSize * SDL_sinf(aim);
				int gx = (int)(wx / cellSize);
				int gy = (int)(wy / cellSize);
				if (gx >= 0 && gx < gridCols && gy >= 0 && gy < gridRows)
					grid[gy][gx] = true;
				golSpawnOffset++;
			}
		}
		else
			golSpawnOffset = 0;

		// Game of Life: next generation (double buffer)
		Uint32 nowGol = SDL_GetTicks();
		if (nowGol - lastGolStepTime >= golStepInterval)
		{
			lastGolStepTime = nowGol;
			for (int gy = 0; gy < gridRows; gy++)
				for (int gx = 0; gx < gridCols; gx++)
				{
					int alive = 0;
					for (int dy = -1; dy <= 1; dy++)
						for (int dx = -1; dx <= 1; dx++)
							if (dx != 0 || dy != 0)
							{
								int ny = gy + dy, nx = gx + dx;
								if (ny >= 0 && ny < gridRows && nx >= 0 && nx < gridCols && grid[ny][nx])
									alive++;
							}
					bool cur = grid[gy][gx];
					gridNext[gy][gx] = (cur && (alive == 2 || alive == 3)) || (!cur && alive == 3);
				}
			grid.swap(gridNext);
		}

		// Nätverk: skicka lokal state, hämta alla spelare
		if (isNetworked) {
			PlayerState localState;
			fillPlayerState(localState, playerWorldX, playerWorldY, lastFacing);
			if (isHost && server) {
				localState.playerId = 0;
				server->tick(localState);
				allPlayers.clear();
				allPlayers.push_back(localState);
				std::vector<PlayerState> clientStates;
				server->getFullState(clientStates);
				for (const auto& cs : clientStates) allPlayers.push_back(cs);
			} else if (isClient && client) {
				localState.playerId = myPlayerId;
				client->tick(localState, allPlayers);
			}
		}

		//
		//	rendering
		//

		// Background: rolling map (20x zoom, scrolls with player)
		SDL_SetRenderDrawColor(renderer, 195, 176, 145, 0);
		SDL_RenderClear(renderer);
		if (mapTexture && mapWidth > 0 && mapHeight > 0)
		{
			// Map drawn 20x size; scrolls only when player reaches screen edge
			SDL_Rect mapDest = {
				(int)(-scrollX),
				(int)(-scrollY),
				mapWidth * mapZoom,
				mapHeight * mapZoom
			};
			SDL_RenderCopy(renderer, mapTexture, nullptr, &mapDest);
		}

		// Game of Life grid: draw cells over the map (alive = yellow, dead = dead_cell)
		SDL_Texture* cellTex = nullptr;
		for (int gy = 0; gy < gridRows; gy++)
		{
			for (int gx = 0; gx < gridCols; gx++)
			{
				float sx = gx * cellSize - scrollX;
				float sy = gy * cellSize - scrollY;
				if (sx + cellSize < 0 || sx > windowWidth || sy + cellSize < 0 || sy > windowHeight)
					continue;
				cellTex = grid[gy][gx] ? texAliveCell : texDeadCell;
				if (cellTex)
				{
					SDL_Rect dst = { (int)sx, (int)sy, cellSize, cellSize };
					SDL_RenderCopy(renderer, cellTex, nullptr, &dst);
				}
			}
		}

		// Andra spelare (nätverk): rita deras kulor och figur
		for (size_t i = 0; i < allPlayers.size(); i++) {
			if (i == myPlayerId) continue;
			const PlayerState& op = allPlayers[i];
			float ox = op.worldX - scrollX;
			float oy = op.worldY - scrollY;
			SDL_Texture* owalkTex = nullptr;
			switch (op.facing) {
			case 0: owalkTex = walkDown[op.walkAnim & 1]; break;
			case 1: owalkTex = walkUp[op.walkAnim & 1]; break;
			case 2: owalkTex = walkLeft[op.walkAnim & 1]; break;
			case 3: owalkTex = walkRight[op.walkAnim & 1]; break;
			}
			if (owalkTex) {
				int tw = 0, th = 0;
				SDL_QueryTexture(owalkTex, nullptr, nullptr, &tw, &th);
				SDL_Rect dst = { (int)ox, (int)(oy - s), (int)(s * 2), (int)(s * 3) };
				if (tw > 0 && th > 0) SDL_RenderCopy(renderer, owalkTex, nullptr, &dst);
			}
		}

		// Player: walk animation sprite (green_boy) or fallback to rectangles
		SDL_Texture* walkTex = nullptr;
		switch (lastFacing)
		{
		case 0: walkTex = walkDown[walkAnimFrame]; break;
		case 1: walkTex = walkUp[walkAnimFrame]; break;
		case 2: walkTex = walkLeft[walkAnimFrame]; break;
		case 3: walkTex = walkRight[walkAnimFrame]; break;
		}
		if (walkTex)
		{
			int tw = 0, th = 0;
			SDL_QueryTexture(walkTex, nullptr, nullptr, &tw, &th);
			SDL_Rect dst = { (int)px, (int)(py - s), (int)(s * 2), (int)(s * 3) };
			if (tw > 0 && th > 0)
				SDL_RenderCopy(renderer, walkTex, nullptr, &dst);
		}

		SDL_RenderPresent(renderer);



		deltaTime = SDL_GetTicks() - deltaTime;
		if (frameDelay > deltaTime)
			SDL_Delay(frameDelay - deltaTime);
	} //end of game-loop

	//
	//	deallocating
	//
	if (mapTexture)
		SDL_DestroyTexture(mapTexture);
	if (texAliveCell)
		SDL_DestroyTexture(texAliveCell);
	if (texDeadCell)
		SDL_DestroyTexture(texDeadCell);
	for (int i = 0; i < 2; i++)
	{
		if (walkDown[i]) SDL_DestroyTexture(walkDown[i]);
		if (walkUp[i]) SDL_DestroyTexture(walkUp[i]);
		if (walkLeft[i]) SDL_DestroyTexture(walkLeft[i]);
		if (walkRight[i]) SDL_DestroyTexture(walkRight[i]);
	}
	IMG_Quit();

	if (server) { delete server; server = nullptr; }
	if (client) { delete client; client = nullptr; }
	if (isNetworked) networkQuit();

	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	return 0;
}
