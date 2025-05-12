#pragma once

#include "Item.h"
#include "Text.h"
#include "Manager/TextureManager.h"

class MushRoom : public Item
{
private:
	void initSprite()
	{
		texture = *texture_manager->get("Mushroom").get();
		sprite.setTexture(texture);
	}
public:
	MushRoom(const sf::FloatRect& rect, const std::string& name, std::shared_ptr<TextureManager> texture_manager, std::shared_ptr<CollisionManager> col, std::shared_ptr<Game> game, int layer) : Item(rect, name, texture_manager, col, game, layer)
	{
		initSprite();
	}

	virtual ~MushRoom() {  }

	void onHit() override;

	void update(float deltaTime) override
	{
		animator->update(deltaTime);

		if (isActive())
		{
			float deltaX = velocity.x * deltaTime * direction;
			float deltaY = velocity.y * deltaTime;
			col->callibrateCollision(*this, deltaX, deltaY);
			checkCollision(deltaTime);
			sprite.move(deltaX, deltaY);

			velocity.y += gravity * deltaTime;
			if (velocity.y > 500.f)
				velocity.y = 500.f;
			//move({deltaX, deltaY}, direction, deltaTime);
		}

		updateTimer(deltaTime);
		
		sf::Vector2f newPos = sprite.getPosition();
		setPosition(newPos);
	}

	void render(sf::RenderTarget* target) override
	{
		target->draw(sprite);
	}
};