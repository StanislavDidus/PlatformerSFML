#include "Game.h"
#include <iostream>

/*
*  MY SUPER Player BROS 85 CLONE
*/

int main()
{
	std::shared_ptr<Game> game = std::make_shared<Game>();
	game->init();

	std::cout << "Game initialized, running: " << game->running() << std::endl;

	while (game->running())
	{
		game->update();

		game->render();
	}

	return 0;
}