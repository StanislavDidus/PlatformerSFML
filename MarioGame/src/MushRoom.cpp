#include "MushRoom.h"
#include "Game.h"

void MushRoom::onHit()
{
	if (is_active)
	{
		if (game != nullptr)
		{
			game->showScore(this->sprite.getPosition(), "assets/Textures/Scores/1000.png");
		}
		is_active = false;
	}
}