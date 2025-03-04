#pragma once

#include "Item.h"

class MushRoom : public Item
{
private:
	void initSprite()
	{
		texture.loadFromFile("assets/Textures/Levels/Mushroom.png");
		sprite.setTexture(texture);
		//this->sprite.setPosition(getPosition().x , getPosition().y - 50.f);
		//std::cout << sprite.getPosition().x << ", " << sprite.getPosition().y << "\n";
	}
public:
	MushRoom(const sf::FloatRect& rect, const std::string& name) : Item(rect, name)
	{
		initSprite();
	}

	virtual ~MushRoom() {}

	void update(float deltaTime) override
	{
		animator->update(deltaTime);
		
		sf::Vector2f newPos = sprite.getPosition();
		setPosition(newPos);
	}

	void render(sf::RenderTarget* target) override
	{
		target->draw(sprite);
		//std::cout << "render\n";
	}
};