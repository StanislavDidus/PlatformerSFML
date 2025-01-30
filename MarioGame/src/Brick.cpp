#include "Brick.h"

void Brick::initSprite()
{

}

Brick::Brick(const sf::Sprite& sprite, const sf::FloatRect& rect, const std::string& type) : GameObject(type, rect), sprite(sprite)
{
	this->initSprite();
}

void Brick::setPosition(const sf::Vector2f& newPosition)
{
}

void Brick::update(float deltaTime)
{
}

void Brick::render(sf::RenderTarget* target)
{
	target->draw(this->sprite);
}
