#pragma once

#include "Item.h"
#include "Text.h"

class MushRoom : public Item
{
private:
	std::unique_ptr<Text> score_text;

	Game* game;

	bool is_active = true;

	void initSprite()
	{
		texture.loadFromFile("assets/Textures/Levels/Mushroom.png");
		sprite.setTexture(texture);
		//this->sprite.setPosition(getPosition().x , getPosition().y - 50.f);
		//std::cout << sprite.getPosition().x << ", " << sprite.getPosition().y << "\n";
	}

	void initText()
	{
		//score_text = std::make_unique<Text>(16, 8, sprite.getPosition(), "assets/Textures/Scores/1000.png");
	}
public:
	MushRoom(const sf::FloatRect& rect, const std::string& name, std::shared_ptr<CollisionManager> col, Game* game) : Item(rect, name, col, game)
	{
		initSprite();
		//initText();

		//col->addSourse(dynamic_cast<GameObject*>(this));

		//col->addCollision({ getBounds(), "Mushroom", this});
	}

	virtual ~MushRoom() { }

	void onHit() override;

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
	}
};