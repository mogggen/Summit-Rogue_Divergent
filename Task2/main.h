#pragma once
// sigh make your own raster or just hard-code every letter
//#include <SDL_ttf.h>	// https://www.youtube.com/watch?v=1tBzIhVgR7E
#include <vector>
namespace Game
{
	// ENTANGLEMENTS (person to person contact)
	// sex
	// assassination
	// membership
	// sleep

	// ITEMS
	// firearms
	// ammunition
	// food
	// currencies
	// axe
	// hoe
	// clothes
	// vehicles
	// buildings
	// wood
	// bricks
	// smelter
	// corpse

	// OPERATIONS
	// trade items
	// transfer of ownership
	// consumtion
	// write letters

	// messages
	// location of item relative to other item
	// person's stats relative to another person
	// conditional (a state is achived that is promised a response)

	int currentDay = 0;
	int dayDuration = 10000;
	int nextDay;

	bool debugMode = true; // spawn resources

	float detection_range = 10.f;
	float travelSpeed = 0.01f;
	int windowWidth = 1000, windowHeight = 800;
	int entitySize = 30;

	int mStartX = 0;
	int mStartY = 0;
	int mDownX = 0;
	int mDownY = 0;
	int px = windowWidth / 2;
	int py = windowHeight / 2;
	int v = 5;

	std::vector<int> grassMapX, grassMapY;
	std::vector<int> chunkId;

	std::vector<int> posX;
	std::vector<int> posY;
	std::vector<int> velX;
	std::vector<int> velY;
	std::vector<int> accX;
	std::vector<int> accY;

	std::vector<bool> isGay;
	std::vector<bool> isMale;

	std::vector<int> pregnancyDuration;
	std::vector<int> age;
	std::vector<int> ageOfAdulthood;

	std::vector<int> agility; // ?
	std::vector<int> stealth;
	std::vector<int> hunger;
	std::vector<int> atraction;

	std::vector<int> inteligence;
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

	// facts vs rumors
	std::vector<int> itemEdibility;
	std::vector<int> itemDurability;
	std::vector<int> itemUtility;
	std::vector<bool> itemCurrency;

	enum Sanctions
	{
		Oligarchs,
		Rebels,
		Military,
		Agriculture
	};

	void procreate(unsigned fatherIndex, unsigned motherIndex)
	{
		int timeUntilbaby = SDL_GetTicks() + 9000;
		posX.push_back(posX[motherIndex]);
		posY.push_back(posY[motherIndex]);
		age.push_back(0);
		ageOfAdulthood.push_back(rand() % 10 + 15);
		// agility.push_back();
	}

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

	void renderMarkedArea(SDL_Renderer *renderer, SDL_Event& event, const int mStartX, const int mStartY, int &mDownX, int &mDownY)
	{
		const float DASH_LEN = 8.f;
		SDL_SetRenderDrawColor(renderer, 189, 189, 189, 255);
		
		if (event.motion.x < windowWidth && event.motion.x >= 0 &&
		event.motion.y < windowHeight && event.motion.y >= 0 &&
		event.motion.y != 1024) // mouse has left the window
		{
			mDownX = event.motion.x;
			mDownY = event.motion.y;
		}

		// check X
		if (mStartX < mDownX)
		{
			size_t i = 0;
			for (; i * DASH_LEN < mDownX - mStartX; i += 2)
			{
				SDL_RenderDrawLine(renderer, mStartX + i * DASH_LEN, mStartY, mStartX + (i + 1) * DASH_LEN, mStartY);
				SDL_RenderDrawLine(renderer, mStartX + i * DASH_LEN, mDownY, mStartX + (i + 1) * DASH_LEN, mDownY);
			}
			if (i % 2 == 1)
			{
				SDL_RenderDrawLine(renderer, mStartX + i * DASH_LEN, mStartY, mDownX, mStartY);
				SDL_RenderDrawLine(renderer, mStartX + i * DASH_LEN, mDownY, mDownX, mDownY);
			}
		}
		else if (mStartX > mDownX)
		{
			size_t i = 0;
			for (; i * DASH_LEN < mStartX - mDownX; i += 2)
			{
				// printf("DownX to StartX");
				SDL_RenderDrawLine(renderer, mDownX + i * DASH_LEN, mStartY, mDownX + (i + 1) * DASH_LEN, mStartY);
				SDL_RenderDrawLine(renderer, mDownX + i * DASH_LEN, mDownY, mDownX + (i + 1) * DASH_LEN, mDownY);

			}
			if (i % 2 == 1)
			{
				SDL_RenderDrawLine(renderer, mDownX + i * DASH_LEN, mStartY, mStartX, mStartY);
				SDL_RenderDrawLine(renderer, mDownX + i * DASH_LEN, mDownY, mStartX, mDownY);
			}
		}
		// printf(" and ");
		// check Y
		if (mStartY < mDownY)
		{
			size_t i = 0;
			for (; i * DASH_LEN < mDownY - mStartY; i += 2)
			{
				SDL_RenderDrawLine(renderer, mStartX, mStartY + i * DASH_LEN, mStartX, mStartY + (i + 1) * DASH_LEN);
				SDL_RenderDrawLine(renderer, mDownX, mStartY + i * DASH_LEN, mDownX, mStartY + (i + 1) * DASH_LEN);
				// printf("StartY to DownY");
			}
			if (i % 2 == 1)
			{
				SDL_RenderDrawLine(renderer, mStartX, mStartY + i * DASH_LEN, mStartX, mDownY);
				SDL_RenderDrawLine(renderer, mDownX, mStartY + i * DASH_LEN, mDownX, mDownY);
			}
		}
		else if (mStartY > mDownY)
		{
			size_t i = 0;
			for (; i * DASH_LEN < mStartY - mDownY; i++)
			{
				SDL_RenderDrawLine(renderer, mStartX, mDownY + i * DASH_LEN, mStartX, mDownY + (++i) * DASH_LEN);
				SDL_RenderDrawLine(renderer, mDownX, mDownY + i * DASH_LEN, mDownX, mDownY + (i) * DASH_LEN);
			}
			if (i % 2 == 1)
			{
				SDL_RenderDrawLine(renderer, mStartX, mDownY + i * DASH_LEN, mStartX, mStartY);
				SDL_RenderDrawLine(renderer, mDownX, mDownY + i * DASH_LEN, mDownX, mStartY);
			}
		}
	}

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
			// size_t j = 0;
			// for (; j < sanction_index.size(); j++)
			// {
			// 	if (posX[sanction_index[i]] + (localX - posX[sanction_index[i]]) * travelSpeed < posX[sanction_index[j]] + 2
			// 	&& posX[sanction_index[i]] + (localX - posX[sanction_index[i]]) * travelSpeed > posX[sanction_index[j]] - 2
			// 	&& posY[sanction_index[i]] + (localX - posY[sanction_index[i]]) * travelSpeed < posY[sanction_index[j]] + 2
			// 	&& posY[sanction_index[i]] + (localX - posY[sanction_index[i]]) * travelSpeed > posY[sanction_index[j]] - 2)
			// 	{
			// 		j = 0;
			// 		i++;
			// 		break;
			// 	}
			// }
			// if (j == sanction_index.size())
			// {
			
			if (sqrtf(powf(posX[sanction_index[i] - px], 2) + powf(posY[sanction_index[i]] - py, 2)) > detection_range)
			{
				// work
				posX[sanction_index[i]] += (localX - posX[sanction_index[i]]) * travelSpeed;
				posY[sanction_index[i]] += (localY - posY[sanction_index[i]]) * travelSpeed;

			}
			else
			{
				// procreate
				// procreate(mom, dad);
				posX[sanction_index[i]] = rand() % windowWidth;
				posY[sanction_index[i]] = rand() % windowHeight;
			}
			// }
		}
	}

	void queueStateChange(const unsigned state)
	{

	}

	void spawnEntities(const size_t count)
	{
		for (size_t i = 0; i < count; i++)
		{
			posX.push_back(rand() % windowWidth - entitySize);
			posY.push_back(rand() % windowHeight - entitySize);

			ageOfAdulthood.push_back(18 + rand() % 7);
			isMale.push_back(rand() % 2);
			isGay.push_back(rand() % 10 == 1);

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
				ageOfAdulthood[last_index] +
				isMale[last_index] +

				agility[last_index] +
				stealth[last_index] +
				hunger[last_index] +
				fear[last_index] +
				morale[last_index] +
				reputation[last_index] +
				popularity[last_index]
			);
		}

		// their age affect how easily convinced a person is so their World View is shaped at a younger age
		// 

	}

	void updateGameLogic()
	{
		// movement for atraction and mating
		float localX = 0.f;
		float localY = 0.f;
		float divisor = 0.f;
		
		// updateSanction(localX, localY, divisor, agr_index);

		// localX = 0.f;
		// localY = 0.f;
		// divisor = 0.f;
		
		updateSanction(localX, localY, divisor, mil_index);

		// localX = 0.f;
		// localY = 0.f;
		// divisor = 0.f;

		// updateSanction(localX, localY, divisor, oli_index);

		// localX = 0.f;
		// localY = 0.f;
		// divisor = 0.f;
		
		// updateSanction(localX, localY, divisor, reb_index);

		// movement when undergoing survailance (state machine)
		
		if (nextDay < SDL_GetTicks())
		{
			nextDay = SDL_GetTicks() + dayDuration;
			currentDay++;
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
}