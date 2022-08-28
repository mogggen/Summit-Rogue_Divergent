#include <iostream>
#include <conio.h>

//namespace Game
//{
	void gameLoopConsole()
	{
		bool isPaused = true;
		bool consoleRunning = false;
		printf("entered game loop console\r\n");
		while (true)
		{
			printf("> ");
			char keystroke = getch();
			printf("\r\n");
			switch (keystroke)
			{
			case 13: // Enter
				printf("stepped\r\n");
				break;
			case ' ':
				if (isPaused)
				{
					isPaused = false;
					printf("paused.\r\n");
				}
				else
				{
					isPaused = true;
					printf("resuming...\r\n");
				}
				break;
			case 27: // Esc
				//close loop
				printf("exiting...");
				exit(0);
				break;
			case 'h':
				printf("h: display help\r\n");
				printf("s: step the game loop one frame\r\n");
				printf("r: resume game loop\r\n");
				printf("p: pause the game loop\r\n");
				printf("c: terminate the game and discard progress\r\n");
				break;
			default:
				std::cerr << "invalid keystroke" << std::endl;
				break;
			}
		}
	}
//}