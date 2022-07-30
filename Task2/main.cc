#include <iostream>
#include <vector>
#include <ctime>
#include <cmath>
#include "Circle.h"
#include "SDL.h"

std::vector<bool> isMale;
// std::vector<int> ;

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

int main()
void main()
void main(void)
int main(void)
int main(int argc, char **argv)
int main(int argc, char* argv[])
{
	bool isInMenu = true;
	srand((unsigned)time(nullptr));
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

	int frameDelay = 17;

	int mx = 0;
	int my = 0;
	int px = windowWidth / 2;
	int py = windowHeight / 2;
	int v = 5;
	int s = 30;

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

	Circle* head = new Circle();
	Circle* torso = new Circle();
	Circle *shadow = new Circle();

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
		if (down && py < windowHeight - s * 3) py += v;
		if (left && px >= 0) px -= v;
		if (right && px < windowWidth - s) px += v;

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

		head->SetCircle(point(px + 2, py - s + 2), s);

		torso->SetCircle(point(px, py), s);

		//
		//	rendering
		//

		//Background
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_RenderClear(renderer);

		//head
		SDL_SetRenderDrawColor(renderer, 150, 50, 99, 0);
		head->render(renderer);

		//torso
		SDL_SetRenderDrawColor(renderer, 195, 50, 99, 0);
		torso->render(renderer);

		SDL_RenderPresent(renderer);



		deltaTime = SDL_GetTicks() - deltaTime;
		if (frameDelay > deltaTime)
			SDL_Delay(frameDelay - deltaTime);
	} //end of game-loop
	delete head;

	delete torso;

	delete shadow;

	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	return 0;
}
