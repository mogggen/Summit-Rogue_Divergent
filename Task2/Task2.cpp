#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include <cmath>
#include "Rectangle.h"
#include "Circle.h"
#include "Bullet.h"
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
}

int main(int argc, char* argv[])
{
	bool isInMenu = true;
	srand((unsigned)time(0));
	const char title[18] = "Enter the Gungeon";
	int enemies = 0;
	bool quit = false;
	int windowWidth = 1000, windowHeight = 800;
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Event event;
	std::cout << "----" << title << "----" << std::endl;
	std::cout << "Resolution: " << windowWidth << "x" << windowHeight << std::endl;
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

	int vv = 0;
	int a = 3;

	float aim = 0;

	// Game of Life: spawn cells when shooting (left click / hold)
	int golSpawnOffset = 0;
	Uint32 lastGolSpawnTime = 0;
	const Uint32 golSpawnInterval = 80;
	Uint32 lastGolStepTime = 0;
	const Uint32 golStepInterval = 200;
	int floorCount = 1;
	bool jumping = false;
	bool floating = false;
	float jumpStartWorldY = 0;
	int deltaTime;

	Rectangle* slider = new Rectangle();
	Rectangle* handle = new Rectangle();

	Rectangle* head = new Rectangle();

	Rectangle *torso = new Rectangle();

	Rectangle *leftLeg = new Rectangle();
	Rectangle *rightLeg = new Rectangle();

	Rectangle *leftArm = new Rectangle();
	Rectangle *rightArm = new Rectangle();

	Rectangle *rifle = new Rectangle();
	Bullet *liveRounds[10];

	Circle *shadow = new Circle();

	Rectangle *tables[4];
	Rectangle *shells[10];
	Rectangle *hearts[3];
	Circle *bullets[10];

	//
	//	definitons
	//
	for (int i = 0; i < sizeof(hearts) / sizeof(*hearts); i++)
	{
		hearts[i] = new Rectangle();
	}

	for (int i = 0; i < sizeof(bullets) / sizeof(*bullets); i++)
	{
		bullets[i] = new Circle();
		shells[i] = new Rectangle();
		liveRounds[i] = new Bullet();
	}

	for (int i = 0; i < sizeof(tables) / sizeof(*tables); i++)
	{
		tables[i] = new Rectangle();
	}

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
					if (!isReloading)
					{
						if (ammoCount > 0)
						{
							liveRounds[10 - ammoCount]->SetIsFired(true);
							ammoCount--;
						}
						else
							isReloading = true;
					}
				}
				else if (event.button.button == SDL_BUTTON_RIGHT)
				{
					// League-style: move player to click position (world coords)
					float clickWorldX = event.button.x + scrollX;
					float clickWorldY = event.button.y + scrollY;
					clickWorldX = (clickWorldX < 0) ? 0 : (clickWorldX > worldWidth - s * 2) ? (float)(worldWidth - s * 2) : clickWorldX;
					clickWorldY = (clickWorldY < 0) ? 0 : (clickWorldY > worldHeight - s * 3) ? (float)(worldHeight - s * 3) : clickWorldY;
					targetWorldX = clickWorldX;
					targetWorldY = clickWorldY;
					hasMoveTarget = true;
				}
				break;

			case SDL_KEYDOWN:
				switch (event.key.keysym.sym)
				{
				case SDLK_r:
					if (!isReloading && ammoCount < 10)
						isReloading = true;
					break;

				case SDLK_SPACE:
					if (vv == 0 && !jumping)
					{
						jumping = true;
						vv = -18;
						jumpStartWorldY = playerWorldY;
					}
					floating = true;
					break;
				}
				break;

			case SDL_KEYUP:
				if (event.key.keysym.sym == SDLK_SPACE)
					floating = false;
				break;
			}
		}

		// Jump mechanics (world Y: up = decrease Y)
		if (floating)
			a = 2;
		else
			a = 4;
		if (jumping)
		{
			playerWorldY += vv;
			vv += a;
			if (playerWorldY >= jumpStartWorldY)
			{
				playerWorldY = jumpStartWorldY;
				vv = 0;
				jumping = false;
			}
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

		//
		//	Reloading
		//
		if (isReloading && reloadingProgress < s * 4)
		{
			reloadingProgress += 2;
			slider->SetRectangle(Point2D(px - s * 2 + s / 2, py - s - s / 2), s * 4, 3);
			handle->SetRectangle(Point2D(px - s * 2 + s / 2 + reloadingProgress, py - s - s / 2 - 6), 3, 15);

			if (reloadingProgress >= s * 4)
			{
				ammoCount = 10;
				for (int k = 0; k < ammoCount; k++)
				{
					liveRounds[k]->SetIsFired(false);
				}
			}
		}
		else
		{
			slider->SetRectangle(Point2D(0, 0), 0, 0);
			handle->SetRectangle(Point2D(0, 0), 0, 0);
			reloadingProgress = 0;
			isReloading = false;
		}

		//hud
		for (int i = 0; i < sizeof(hearts) / sizeof(*hearts); i++)
		{
			hearts[i]->SetRectangle(Point2D(s / 2 + s * 2 * i / 3, s / 2), s / 2.5f, s);
		}

		for (int i = 0; i < sizeof(bullets) / sizeof(*bullets); i++)
		{
			bullets[i]->SetCircle(Point2D(s / 2 + s * 2 * i / 3, s + s / 4 + 1), s / 5);
			shells[i]->SetRectangle(Point2D(s / 2 + s * 2 * i / 3, s + s / 2), s / 2.5f, s);
		}

		int tableWidth = 120, tableHeight = 40;

		tables[0]->SetRectangle(Point2D(windowWidth / 4, windowHeight / 4), tableHeight, tableWidth);
		tables[1]->SetRectangle(Point2D(3 * windowWidth / 4, windowHeight / 4), tableHeight, tableWidth);
		tables[2]->SetRectangle(Point2D(2 * windowWidth / 3, windowHeight / 2), tableWidth, tableHeight);
		tables[3]->SetRectangle(Point2D(windowWidth / 3, windowHeight / 2), tableWidth, tableHeight);

		head->SetRectangle(Point2D(px + 2, py - s + 2), s - 4, s - 4);

		torso->SetRectangle(Point2D(px, py), s, s * 2);

		leftLeg->SetRectangle(Point2D(px, py + s * 2), s / 2 - 2, s * 2);
		rightLeg->SetRectangle(Point2D(px + s / 2 + 2, py + s * 2), s / 2 - 2, s * 2);

		leftArm->SetRectangle(Point2D(px - s / 2, py + s / 5), s / 2, s + s / 2);
		rightArm->SetRectangle(Point2D(px + s, py + s / 5), s / 2, s + s / 2);

		rifle->SetRectangle(Point2D(px + s + s / 2, py + s + s / 2), s + 2 * s / 3, s / 3);
		rifle->SetRotation(aim, Point2D(0, -6));

		if (!jumping)
			shadow->SetCircle(Point2D(px, py + s * 4 - s / 2), s / 2);
		else
		{
			float groundScreenY = jumpStartWorldY - scrollY;
			shadow->SetCircle(Point2D(px + (groundScreenY - py) / 4, groundScreenY), s / 2);
		}

		// Moving bullets (world space: Start+Travel in world, draw at world - scroll)
		for (int t = 0; t < sizeof(liveRounds) / sizeof(*liveRounds); t++)
		{
			if (liveRounds[t]->GetIsFired())
			{
				if (liveRounds[t]->GetBulletAim() == 0)
				{
					liveRounds[t]->SetBulletAim(aim + float(rand() % (314 / 8)) / 500.0f - (314 / 8) / 1600.0f);
				}
				if (liveRounds[t]->GetStart() == Point2D(0, 0))
					liveRounds[t]->SetStart(Point2D(playerWorldX + s + s / 2, playerWorldY + s + s / 2));
				liveRounds[t]->SetTravel(Point2D(
					liveRounds[t]->GetTravel().GetX() + (int)round(SDL_cosf(liveRounds[t]->GetBulletAim()) * 20),
					liveRounds[t]->GetTravel().GetY() + (int)round(SDL_sinf(liveRounds[t]->GetBulletAim()) * 20)));

				float bx = liveRounds[t]->GetStart().GetX() + liveRounds[t]->GetTravel().GetX() - scrollX;
				float by = liveRounds[t]->GetStart().GetY() + liveRounds[t]->GetTravel().GetY() - scrollY;
				liveRounds[t]->SetRectangle(Point2D(bx, by), 50, 12);
				liveRounds[t]->SetRotation(liveRounds[t]->GetBulletAim(), Point2D(0, -6));
			}
			else
			{
				liveRounds[t]->SetBulletAim(0.0f);
				liveRounds[t]->SetStart(Point2D(0, 0));
				liveRounds[t]->SetTravel(Point2D(0, 0));
				liveRounds[t]->SetRectangle(Point2D(0, 0), 0, 0);
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

		//liveRounds
		SDL_SetRenderDrawColor(renderer, 200, 200, 93, 0);
		for (int i = 0; i < sizeof(liveRounds) / sizeof(*liveRounds); i++)
		{
			liveRounds[i]->render(renderer);
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


		//hud
		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 0);
		for (int i = 0; i < heartCount; i++)
		{
			hearts[i]->render(renderer);
		}

		for (int i = 0; i < ammoCount; i++)
		{
			SDL_SetRenderDrawColor(renderer, 105, 105, 105, 0);
			bullets[i]->render(renderer);
			SDL_SetRenderDrawColor(renderer, 255, 191, 0, 0);
			shells[i]->render(renderer);
		}

		SDL_RenderPresent(renderer);



		deltaTime = SDL_GetTicks() - deltaTime;
		if (frameDelay > deltaTime)
			SDL_Delay(frameDelay - deltaTime);
	} //end of game-loop

	//
	//	deallocating
	//


	for (int i = 0; i < sizeof(shells) / sizeof(*shells); i++)
	{
		delete shells[i];
		delete bullets[i];
		delete liveRounds[i];
	}


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

	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	return 0;
}
