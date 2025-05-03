#pragma once

#include "..\..\Item.h"
#include "..\..\Text.h"
#include "..\..\Manager/TextureManager.h"

class FireFlower : public Item
{
private:
	void initSprite()
	{
		texture = *texture_manager->get("FireFlower").get();
		sprite.setTexture(texture);
		sprite.setTextureRect(sf::IntRect(0, 0, 16, 16));
	}

	void initAnimations()
	{
		animator->addFrameAnimation(sprite, 16, 16, std::vector<int>{0, 1, 2, 3}, 50.f / 1000.f, []() {return true; }, []() {return 1; }, true, 15, "Idle");
	}
public:
	FireFlower(const sf::FloatRect& rect, const std::string& name, std::shared_ptr<TextureManager> texture_manager, std::shared_ptr<CollisionManager> col, std::shared_ptr<Game> game, int layer) : Item(rect, name, texture_manager, col, game, layer)
	{
		initSprite();
		initAnimations();
	}

	virtual ~FireFlower() {}

	void onHit() override;

	void update(float deltaTime) override
	{
		animator->update(deltaTime);

		if (isActive())
		{
			
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

