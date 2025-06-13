#include "Objects/Item.h"
#include "Game.h" 

void Item::update(float deltaTime)
{
	move(velocity, direction, deltaTime);
	updateTimer(deltaTime);
}

void Item::render(sf::RenderTarget* target)
{
	target->draw(sprite);
}