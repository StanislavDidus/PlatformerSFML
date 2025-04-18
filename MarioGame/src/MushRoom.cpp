#include "MushRoom.h"
#include "Game.h"

void MushRoom::onHit()
{
	if (isActive())
	{
		if (game != nullptr)
		{
			texture = *texture_manager->get("1000S").get();
			game->showScore({sprite.getPosition().x, sprite.getPosition().y}, &texture);
		}
		is_active = false;
	}
}