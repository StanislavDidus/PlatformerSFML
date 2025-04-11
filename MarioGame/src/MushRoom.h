#pragma once

#include "Item.h"
#include "Text.h"
#include "Manager/TextureManager.h"

class MushRoom : public Item
{
private:
	//std::unique_ptr<Text> score_text;
	

	//std::shared_ptr<Game> game;

	bool is_active = true;

	void initSprite()
	{
		texture = *texture_manager->get("Mushroom").get();
		sprite.setTexture(texture);
		//this->sprite.setPosition(getPosition().x , getPosition().y - 50.f);
		//std::cout << sprite.getPosition().x << ", " << sprite.getPosition().y << "\n";
	}

	void initText()
	{
		//score_text = std::make_unique<Text>(16, 8, sprite.getPosition(), "assets/Textures/Scores/1000.png");
	}
public:
	MushRoom(const sf::FloatRect& rect, const std::string& name, std::shared_ptr<TextureManager> texture_manager, std::shared_ptr<CollisionManager> col, std::shared_ptr<Game> game, int layer) : Item(rect, name, texture_manager, col, game, layer)
	{
		initSprite();
		//initText();

		//col->addSourse(dynamic_cast<GameObject*>(this));

		//col->addCollision({ getBounds(), "Mushroom", this});
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