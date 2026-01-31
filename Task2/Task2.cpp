#include <iostream>
#include <string>
#include <ctime>
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
	std::cout << "WASD to move\nleft_click to shoot\nSPACE to jump";
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
	int px = windowWidth / 2;
	int py = windowHeight / 2;
	int v = 5;
	int s = 30;

	// Scroll only when player leaves center zone (free area = width/4 and height/4 from center)
	float scrollX = 0, scrollY = 0;
	const int centerZoneLeft   = windowWidth / 4;
	const int centerZoneRight  = (3 * windowWidth) / 4;
	const int centerZoneTop    = windowHeight / 4;
	const int centerZoneBottom = (3 * windowHeight) / 4;
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

	bool up = false, down = false, left = false, right = false;

	// Walk animation: 0=down, 1=up, 2=left, 3=right
	int lastFacing = 0;
	int walkAnimFrame = 0;
	Uint32 lastWalkAnimTime = 0;
	const Uint32 walkAnimInterval = 120;

	int vv = 0;
	int a = 3;

	float aim = 0;
	int floorCount = 1;
	bool jumping = false;
	bool floating = false;
	int temp = py;
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
				if (event.button.button == SDL_BUTTON_LEFT && !isReloading)
				{
					if (ammoCount > 0)
					{
						liveRounds[10 - ammoCount]->SetIsFired(true);
						ammoCount--;
					}
					else
						isReloading = true;
				}
				break;

			case SDL_KEYDOWN:
				switch (event.key.keysym.sym)
				{
				case SDLK_w:
				case SDLK_UP:
					up = true;
					break;

				case SDLK_s:
				case SDLK_DOWN:
					down = true;
					break;

				case SDLK_a:
				case SDLK_LEFT:
					left = true;
					break;

				case SDLK_d:
				case SDLK_RIGHT:
					right = true;
					break;

				case SDLK_r:
					if (!isReloading && ammoCount < 10)
						isReloading = true;
					break;

				case SDLK_SPACE:
					if (vv == 0 && !jumping)
					{
						jumping = true;
						vv = -18;
						temp = py;
					}
					floating = true;
					break;
				}
				break;

			case SDL_KEYUP:
				switch (event.key.keysym.sym)
				{
				case SDLK_w:
				case SDLK_UP:
					up = false;
					break;

				case SDLK_s:
				case SDLK_DOWN:
					down = false;
					break;

				case SDLK_a:
				case SDLK_LEFT:
					left = false;
					break;

				case SDLK_d:
				case SDLK_RIGHT:
					right = false;
					break;

				case SDLK_SPACE:
					floating = false;
					break;
				}
				break;
			}
		}

		//jumping mechanics (prototype)
		if (floating)
			a = 2;
		else a = 4;
		if (py <= temp && jumping)
		{
			py += vv;
			vv += a;
		}
		else
		{
			vv = 0;
			jumping = false;
		}

		// Movement: free in center zone (width/4, height/4 from center); scroll only outside it.
		// When moving back towards center, always reduce scroll so camera follows and scroll turns off.
		if (up)
		{
			if (py > centerZoneTop)
			{
				py -= v;
				if (scrollY > 0)
					scrollY = (scrollY - v > 0) ? scrollY - v : 0;
			}
			else if (scrollY > 0)
				scrollY = (scrollY - v > 0) ? scrollY - v : 0;
			else
				py = centerZoneTop;
		}
		if (down)
		{
			if (scrollY > 0)
				scrollY = (scrollY - v > 0) ? scrollY - v : 0;
			else if (py < centerZoneBottom - s * 3)
				py += v;
			else if (scrollY < maxScrollY)
				scrollY = (scrollY + v < maxScrollY) ? scrollY + v : (float)maxScrollY;
			else
				py = centerZoneBottom - s * 3;
		}
		if (left)
		{
			if (scrollX > 0)
				scrollX = (scrollX - v > 0) ? scrollX - v : 0;
			else if (px > centerZoneLeft)
				px -= v;
			else
				px = centerZoneLeft;
		}
		if (right)
		{
			if (px < centerZoneRight - s)
				px += v;
			else if (scrollX < maxScrollX)
				scrollX = (scrollX + v < maxScrollX) ? scrollX + v : (float)maxScrollX;
			else
				px = centerZoneRight - s;
		}

		// Update facing and walk animation frame
		bool isWalking = up || down || left || right;
		if (isWalking)
		{
			if (down) lastFacing = 0;
			else if (up) lastFacing = 1;
			else if (left) lastFacing = 2;
			else if (right) lastFacing = 3;
			Uint32 now = SDL_GetTicks();
			if (now - lastWalkAnimTime >= walkAnimInterval)
			{
				lastWalkAnimTime = now;
				walkAnimFrame = 1 - walkAnimFrame;
			}
		}
		else
			walkAnimFrame = 0;

		//checks cursor position (SDL_GetMouseState works on both Windows and Linux)
		int mouseX, mouseY;
		SDL_GetMouseState(&mouseX, &mouseY);
		if (0 < mouseX && mouseX < windowWidth && 0 < mouseY && mouseY < windowHeight)
		{
			mx = mouseX;
			my = mouseY;
		}
		aim = aiming(px + s + s / 2, py + s, mx, my);

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
			shadow->SetCircle(Point2D(px + (temp - py) / 4, temp), s / py - temp);

		//moving bullets
		for (int t = 0; t < sizeof(liveRounds) / sizeof(*liveRounds); t++)
		{
			if (liveRounds[t]->GetIsFired())
			{
				if (liveRounds[t]->GetBulletAim() == 0)
				{
					liveRounds[t]->SetBulletAim(aim + float(rand() % (314 / 8)) / 500.0f - (314 / 8) / 1600.0f);
				}
				if (liveRounds[t]->GetStart() == Point2D(0, 0)) liveRounds[t]->SetStart(Point2D(px + s + s / 2, py + s + s / 2));
				liveRounds[t]->SetTravel(Point2D(
					liveRounds[t]->GetTravel().GetX() + (int)round(SDL_cosf(liveRounds[t]->GetBulletAim()) * 20), 
					liveRounds[t]->GetTravel().GetY() + (int)round(SDL_sinf(liveRounds[t]->GetBulletAim()) * 20)));

				liveRounds[t]->SetRectangle(liveRounds[t]->GetStart() + liveRounds[t]->GetTravel(), 50, 12);
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
