#include <iostream>
#include <conio.h>

void lore(const char keystroke, int storyKey, int enviormentToken, int conflictNumber, int StoneSequence)
{
	std::vector<std::string> Characters = {
	"You",
	"He",
	"She",
	"They",
	};
	std::vector<std::string> subjects = {
	"walked",
	"talked",
	"ran",
	"interacted",
	"discovered",
	"overcame",
	"interrupted",
	"hid",
	};
	std::vector<std::string> positional = {
	"from",
	"to",
	"ahead",
	"behind",
	"under",
	"over",
	};

	std::vector<std::string> objects = {
	"the nuke",
	"Alcatraz",
	"the device",
	"the machine",
	"the creature",
	"the kangaroo",
	"the bottely fluids",
	//"Egypt",
	"Arsenal codes",
	"the organic books",
	"the melting puppy",
	"the girl",
	"the boy",
	"the father",
	"the mother",
	"the daughter",
	"the grandson",
	"the sdaughter",
	"the son",
	};

	switch (keystroke)
	{
	case 'f':
		storyKey++;
		break;
	case 'j':
		enviormentToken++;
		break;
	case 'g':
		conflictNumber++;
		break;
	case 'h':
		StoneSequence++;
		break;
	}

	int outputResult = StoneSequence % objects.size() + storyKey % subjects.size() + enviormentToken % positional.size() + conflictNumber % Characters.size();

	std::cout << Characters[(rand() + conflictNumber + outputResult) % Characters.size()] << " "
		<< subjects[(rand() + storyKey + outputResult) % subjects.size()] << " "
		<< positional[(rand() + enviormentToken + outputResult) % positional.size()] << " "
		<< objects[(rand() + StoneSequence + outputResult) % objects.size()] << "." << "\r\n\r\n\r\n";
}

//namespace Game
//{
	void gameLoopConsole()
	{
		int step = 0;
		printf("you enter a dark room...\r\n\r\n");
		int valueF = 0;
		int valueG = 0;
		int valueH = 0;
		int valueJ = 0;
		while (true)
		{
			char keystroke = getch();
			lore(keystroke, valueF, valueJ, valueG, valueH);
			printf("\r\n");
			printf("\r\ncontinue...\r\n");
			switch (keystroke)
			{
			case 13: // Enter
				break;
			case ' ':
				break;
			case 27: // Esc
				//close loop
				printf("exiting...");
				exit(0);
				break;
			case 'f':

				break;
			case 'q':
				printf("q: display help\r\n");
				printf("Enter: continue...\r\n");
				printf("Esc: terminate the game and discard progress\r\n");
				break;
			default:
				std::cerr << "invalid keystroke" << std::endl;
				break;
			}
			step++;
		}
	}
//}