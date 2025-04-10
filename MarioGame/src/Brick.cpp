#include "Brick.h"

Brick::Brick(const sf::Sprite& sprite, const sf::FloatRect& rect, const std::string& type, int layer) : Block(rect, type, "assets/Textures/Levels/Brick.png", layer)
{
	//this->initSprite();
	//this->animator->addPosAnimation(
		//this->sprite, 150.f, [this]() {return false; }, false, 25, { {this->sprite.getPosition()},{this->sprite.getPosition().x, this->sprite.getPosition().y - 15.f} }, "Hit");
}

void Brick::onHit()
{
	this->animator->playAnim("Hit");
}

