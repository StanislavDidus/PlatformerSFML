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
	MushRoom(const sf::FloatRect& rect, const std::string& name, std::shared_ptr<CollisionManager> col) : Item(rect, name, col)
	{
		initSprite();

		col->addSourse(dynamic_cast<GameObject*>(this));

		col->addCollision({ rect, "Mushroom", this });
	}

	virtual ~MushRoom() {}

	void onHit() override { std::cout << "Collect mushroom\n"; }

	void update(float deltaTime) override
	{
		animator->update(deltaTime);

		if (isActive())
		{
			float deltaX = velocity.x * deltaTime * direction;
			float deltaY = velocity.y * deltaTime;
			col->callibrateCollision(sprite.getGlobalBounds(), deltaX, deltaY);
			checkCollision(deltaTime);
			sprite.move(deltaX, deltaY);
			//move({deltaX, deltaY}, direction, deltaTime);
		}

		updateTimer(deltaTime);
		
		sf::Vector2f newPos = sprite.getPosition();
		setPosition(newPos);
	}

	void render(sf::RenderTarget* target) override
	{
		target->draw(sprite);
		//std::cout << "render\n";
	}
};