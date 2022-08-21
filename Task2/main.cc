#include <iostream>
#include <vector>
#include <ctime>
#include <cmath>
#include <getopt.h>
#include "SDL.h"		// https://trenki2.github.io/blog/2017/06/02/using-sdl2-with-cmake/
#include "main.h"
#include "hardcodedRaster.h"

using namespace Game;

int main(int argc, char* argv[])
{
	int c;
	while((c = getopt(argc, argv, "dr")) != -1)
	{
		switch (std::tolower(c))
		{
		case 'd':
			debugMode = true;
			break;
		
		case 'r':
			debugMode = false;
			break;
		
		case 'h':
			printf("display help");
			break;
		}
	}

	// go to bed whithin a span of a day in in-game time, and receive a boost in stats as you're "visited by a sanction of your pick", that increments and decrements certain values
	// undersleeping grants respect, oversleeping does the opposite

	// TODO
	// render letters like in Virtual Beggar
	// get mark area for stats working
	// give sanctions stuff to do
	// 
	// generate resources when they are needed for performance: a resource is spawn at the location randomly picked by the entity/updateLogic (longer away => less likely to spawn)

	bool isInMenu = true;
	srand((unsigned)time(nullptr));
	bool quit = false;
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Event event;
	SDL_Window *window = SDL_CreateWindow("title", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowWidth, windowHeight, SDL_WINDOW_SHOWN);
	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);

	int frameDelay = 17;

	spawnEntities(10);

	bool up = false, down = false, left = false, right = false;

	float aim = 0;
	int deltaTime;

	bool mouseDown = false;

	while (!quit)
	{
		deltaTime = SDL_GetTicks();

		//Background
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_RenderClear(renderer);

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
					mouseDown = true;
					mStartX = event.motion.x;
					mStartY = event.motion.y;
				}
				break;

			case SDL_KEYDOWN:
				switch (event.key.keysym.sym)
				{
				case SDLK_w:
					up = true;
					break;

				case SDLK_s:
					down = true;
					break;

				case SDLK_a:
					left = true;
					break;

				case SDLK_d:
					right = true;
					break;
				}
				break;

			case SDL_KEYUP:
				switch (event.key.keysym.sym)
				{
				case SDLK_w:
					up = false;
					break;

				case SDLK_s:
					down = false;
					break;

				case SDLK_a:
					left = false;
					break;

				case SDLK_d:
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
		if (mouseDown)
		{
			if (0 < event.motion.x &&
				windowWidth > event.motion.x &&
				0 < event.motion.y &&
				windowHeight > event.motion.y &&
				event.motion.y != 1024)
			{
				mDownX = event.motion.x;
				mDownY = event.motion.y;
			}
		}

		for (size_t i = 0; i < mil_index.size(); i++)
		{
			SDL_SetRenderDrawColor(renderer, 0, 255, 0, 0);
			renderCircle(renderer, posX[mil_index[i]], posY[mil_index[i]], 4);
		}

		// for (size_t i = 0; i < agr_index.size(); i++)
		// {
		// 	SDL_SetRenderDrawColor(renderer, 139, 69, 19, 0);
		// 	renderCircle(renderer, posX[agr_index[i]], posY[agr_index[i]], 4);
		// }

		// for (size_t i = 0; i < reb_index.size(); i++)
		// {
		// 	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 0);
		// 	renderCircle(renderer, posX[reb_index[i]], posY[reb_index[i]], 4);
		// }

		// for (size_t i = 0; i < oli_index.size(); i++)
		// {
		// 	SDL_SetRenderDrawColor(renderer, 215, 215, 0, 0);
		// 	renderCircle(renderer, posX[oli_index[i]], posY[oli_index[i]], 4);
		// }
		
		// renderMarkedArea(renderer, event, mStartX, mStartY, mDownX, mDownY);

		updateGameLogic();

		CharacterRasterization(renderer, std::to_string(currentDay) + "ABCDEFGHIJ", windowWidth - 900, 50);

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
