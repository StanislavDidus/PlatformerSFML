#include "MushRoom.h"
#include "Game.h"

void MushRoom::onHit()
{
	if (is_active)
	{
		if (game != nullptr)
		{
			texture = *texture_manager->get("1000S").get();
			game->showScore({sprite.getPosition().x, sprite.getPosition().y}, &texture);
		}
		is_active = false;
	}
}