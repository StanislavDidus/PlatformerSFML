#include "Objects/Items/FireFlower.h"
#include "Game.h"

void FireFlower::onHit()
{
	if (isActive())
	{
		if (game != nullptr)
		{
			texture = *texture_manager->get("1000S").get();
			game->showScore({ sprite.getPosition().x, sprite.getPosition().y }, &texture, 1000);
			//std::cout << "scroe\n";
		}
		is_active = false;
	}
}
