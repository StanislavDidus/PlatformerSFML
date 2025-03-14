#include "Game.h"
#include <iostream>

int main()
{
	std::shared_ptr<Game> game = std::make_shared<Game>();
	game->init();

	while (game->running())
	{
		game->update();

		game->render();
	}

	return 0;
}