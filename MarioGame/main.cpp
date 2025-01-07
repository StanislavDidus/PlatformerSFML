//#include <SFML/Network.hpp>
//#include <SFML/Audio.hpp>

#include "Game.h"

int main()
{
	Game game;

	while (game.running())
	{
		game.update();

		game.render();
	}

	return 0;
}