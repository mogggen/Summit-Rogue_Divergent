#include <iostream>
#include <vector>
#include <ctime>
#include <cmath>
#include "SDL.h"

#define un unsigned

int windowWidth = 1000, windowHeight = 800;
int entitySize = 30;

void renderCircle(SDL_Renderer *renderer, float cx, float cy, float radius)
{
	for (int h = 0; h < radius * 2; h++)
	{
		int x1, x2;
		int dy = radius - h;
		int dx = SDL_sqrt(radius * radius - dy * dy);
		x1 = dx;
		x2 = -dx;
		SDL_RenderDrawLine(renderer, cx + radius + x1, cy + radius + dy, cx + radius + x2, cy + radius + dy);
	}
}

std::vector<int> posX;
std::vector<int> posY;

std::vector<bool> isGay;
std::vector<bool> ageOfAdulthood;
std::vector<bool> isMale;

std::vector<int> agility;
std::vector<int> stealth;
std::vector<int> hunger;
std::vector<int> atraction;

std::vector<int> fear;
std::vector<int> morale;

std::vector<int> reputation;
std::vector<int> popularity;

// corpses
std::vector<int> rotting;

void spawnEntities(size_t count=10)
{
	for (size_t i = 0; i < count; i++)
	{
		posX.push_back(rand() % windowWidth - entitySize);
		posY.push_back(rand() % windowHeight - entitySize);

		ageOfAdulthood.push_back(18 + rand() % 7);
		
	}
	
}

// float aiming(float px, float py, float mx, float my)
// {
// 	if (mx <= px && my <= py)
// 		return SDL_atanf(float(py - my) / float(px - mx)) - M_PI;
// 	else if (mx <= px && my >= py)
// 		return SDL_atanf(float(py - my) / float(px - mx)) - M_PI;
// 	else if (mx >= px && my >= py)
// 		return SDL_atanf(float(my - py) / float(mx - px));
// 	else if (mx >= px && my <= py)
// 		return SDL_atanf(float(py - my) / float(px - mx));
// }

int main(int argc, char* argv[])
{
	bool isInMenu = true;
	srand((unsigned)time(nullptr));
	bool quit = false;
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Event event;
	SDL_Window *window = SDL_CreateWindow("title", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowWidth, windowHeight, SDL_WINDOW_SHOWN);
	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);

	int frameDelay = 17;

	sp

	int mx = 0;
	int my = 0;
	int px = windowWidth / 2;
	int py = windowHeight / 2;
	int v = 5;

	int heartCount = 3;
	int ammoCount = 10;
	bool isReloading = false;
	int reloadingProgress = 0;

	int fireDelay = 0;

	bool up = false, down = false, left = false, right = false;

	int vv = 0;
	int a = 3;

	float aim = 0;
	int floorCount = 1;
	bool jumping = false;
	bool floating = false;
	int temp = py;
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
				// mark with rectangle
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
				}
				break;
			}
		}

		//movement
		if (up && py >= 0) py -= v;
		if (down && py < windowHeight - entitySize * 3) py += v;
		if (left && px >= 0) px -= v;
		if (right && px < windowWidth - entitySize) px += v;

		//checks cursor position
		if (0 < event.motion.x &&
			windowWidth > event.motion.x &&
			0 < event.motion.y &&
			windowHeight > event.motion.y &&
			event.motion.y != 1024)
		{
			mx = event.motion.x;
			my = event.motion.y;
		}

		//
		//	rendering
		//

		//Background
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_RenderClear(renderer);

		//head
		SDL_SetRenderDrawColor(renderer, 150, 50, 99, 0);
		renderCircle(renderer, px, py, entitySize);

		SDL_RenderPresent(renderer);

		deltaTime = SDL_GetTicks() - deltaTime;
		if (frameDelay > deltaTime)
			SDL_Delay(frameDelay - deltaTime);
	} //end of game-loop

	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	return 0;
}
