#include <iostream>
#include <vector>
#include <ctime>
#include <cmath>
#include <getopt.h>
#include "SDL.h"

#define un unsigned

bool debugMode = true; // spawn resources

float detection_range = 10.f;
float travelSpeed = 0.01f;
int windowWidth = 1000, windowHeight = 800;
int entitySize = 30;

int mx = 0;
int my = 0;
int px = windowWidth / 2;
int py = windowHeight / 2;
int v = 5;

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

std::vector<int> oli_index;
std::vector<int> agr_index;
std::vector<int> mil_index;
std::vector<int> reb_index;

// corpses
std::vector<int> rotting;

enum Sanctions
{
	Oligarchs,
	Rebels,
	Military,
	Agriculture
};

void updateSanction(float &localX, float &localY, float &divisor, std::vector<int> sanction_index)
{
	for (size_t i = 0; i < sanction_index.size(); i++)
	{
		localX += posX[sanction_index[i]] * atraction[sanction_index[i]];
		localY += posY[sanction_index[i]] * atraction[sanction_index[i]];
		divisor += atraction[sanction_index[i]];
	}
	localX /= divisor;
	localY /= divisor;

	for (size_t i = 0; i < sanction_index.size(); i++)
	{
		size_t j = 0;
		for (; j < sanction_index.size(); j++)
		{
			if (posX[sanction_index[i]] + (localX - posX[sanction_index[i]]) * travelSpeed < posX[sanction_index[j]] + 2
			&& posX[sanction_index[i]] + (localX - posX[sanction_index[i]]) * travelSpeed > posX[sanction_index[j]] - 2
			&& posY[sanction_index[i]] + (localX - posY[sanction_index[i]]) * travelSpeed < posY[sanction_index[j]] + 2
			&& posY[sanction_index[i]] + (localX - posY[sanction_index[i]]) * travelSpeed > posY[sanction_index[j]] - 2)
			{
				j = 0;
				i++;
				break;
			}
		}
		if (j == sanction_index.size())
		{
			posX[sanction_index[i]] += (localX - posX[sanction_index[i]]) * travelSpeed;
			posY[sanction_index[i]] += (localY - posY[sanction_index[i]]) * travelSpeed;
		}
	}
}


void spawnEntities(size_t count)
{
	for (size_t i = 0; i < count; i++)
	{
		posX.push_back(rand() % windowWidth - entitySize);
		posY.push_back(rand() % windowHeight - entitySize);

		ageOfAdulthood.push_back(18 + rand() % 7);
		isMale.push_back(rand() % 2);

		const size_t last_index = isMale.size() - 1;

		agility.push_back(rand() % 50 + 50 * isMale[last_index]);
		stealth.push_back(rand() % 50 + 50 * !isMale[last_index]);
		hunger.push_back(rand() % 50 + 50);

		fear.push_back(rand() % 100);
		morale.push_back(rand() % 100);

		reputation.push_back(rand() % 100);
		popularity.push_back(rand() % 100);

		switch ((Sanctions)(rand() % 4))
		{
		case Military:
			mil_index.push_back(last_index);
			break;

		case Oligarchs:
			oli_index.push_back(last_index);
			break;

		case Agriculture:
			agr_index.push_back(last_index);
			break;

		case Rebels:
			reb_index.push_back(last_index);
			break;
		}

		atraction.push_back
		(
			agility[last_index] +
			stealth[last_index] +
			hunger[last_index] +
			fear[last_index] +
			morale[last_index] +
			reputation[last_index] +
			popularity[last_index]
		);
	}
}

void updateGameLogic()
{
	// movement for atraction and mating
	float localX = 0.f;
	float localY = 0.f;
	float divisor = 0.f;
	
	updateSanction(localX, localY, divisor, agr_index);

	localX = 0.f;
	localY = 0.f;
	divisor = 0.f;
	
	updateSanction(localX, localY, divisor, mil_index);

	localX = 0.f;
	localY = 0.f;
	divisor = 0.f;

	updateSanction(localX, localY, divisor, oli_index);

	localX = 0.f;
	localY = 0.f;
	divisor = 0.f;
	
	updateSanction(localX, localY, divisor, reb_index);

	// movement when undergoing survailance (state machine)
	
	for (size_t i = 0; i < agr_index.size(); i++)
	{
		if (sqrtf(powf(px - posX[agr_index[i]], 2) + powf(py - posY[agr_index[i]], 2)) < detection_range * stealth[agr_index[i]])
		{
			// generate food for everyone

			// gather at designated location
			// carry back
			// repeat

			// generate building materials for houses

			// smelt materials
			// travel to forest
			// chop down at forest
			// return to base
			// repeat
		}
	}
	
	/* code */
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
			// display help
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

	spawnEntities(1000);

	bool up = false, down = false, left = false, right = false;

	float aim = 0;
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

		for (size_t i = 0; i < mil_index.size(); i++)
		{
			SDL_SetRenderDrawColor(renderer, 0, 255, 0, 0);
			renderCircle(renderer, posX[mil_index[i]], posY[mil_index[i]], 4);
		}

		for (size_t i = 0; i < agr_index.size(); i++)
		{
			SDL_SetRenderDrawColor(renderer, 139, 69, 19, 0);
			renderCircle(renderer, posX[agr_index[i]], posY[agr_index[i]], 4);
		}

		for (size_t i = 0; i < reb_index.size(); i++)
		{
			SDL_SetRenderDrawColor(renderer, 255, 0, 0, 0);
			renderCircle(renderer, posX[reb_index[i]], posY[reb_index[i]], 4);
		}

		for (size_t i = 0; i < oli_index.size(); i++)
		{
			SDL_SetRenderDrawColor(renderer, 215, 215, 0, 0);
			renderCircle(renderer, posX[oli_index[i]], posY[oli_index[i]], 4);
		}
		
		
		
		updateGameLogic();

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
