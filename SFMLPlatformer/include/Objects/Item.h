#pragma once

#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include "GameObject.h"
#include "Managers/Animator.h"
#include "Managers/CollisionManager.h"
#include "Managers/TextureManager.h"

#include "string"

class Game;

class Item : public GameObject
{
private:
	

	float timer = 0.f;

	void initSprite()
	{
		sprite.setScale(3.125f, 3.125f);
		sprite.setPosition(getPosition());
	}

	void initAnimations()
	{
		animator = std::make_unique<Animator>();

		animator->addPosAnimation(
			sprite, 16, 16, 75.f, [this]() {return false; }, false, 10, { {sprite.getPosition()},{sprite.getPosition().x, sprite.getPosition().y - 16 * 3.125f} }, "Spawn"
		);
	}
public:
	std::shared_ptr<Game> game;

	int direction = 1;
	float gravity = 980.f;
	sf::Vector2f velocity = { 250.f, 0.f };

	std::shared_ptr<CollisionManager> col;
	std::shared_ptr<TextureManager> texture_manager;

	sf::Texture texture;
	sf::Sprite sprite;

	std::unique_ptr<Animator> animator;

	Item(const sf::FloatRect& rect, const std::string& name, std::shared_ptr<TextureManager> texture_manager, std::shared_ptr<CollisionManager> col, std::shared_ptr<Game> game, int layer) : GameObject(name, rect, layer), col(col), game(game), texture_manager(texture_manager)
	{
		initSprite();
		initAnimations();

		is_active = false;

		animator->playAnim("Spawn");
	}
	virtual ~Item() {  }

	//const bool isActive() const { return is_active; }

	void Jump()
	{
		direction *= -1;
		velocity.y = -400.f;
	}

	void move(sf::Vector2f pos, int dir, float deltaTime)
	{
		sf::Vector2f newPos = sprite.getPosition() + sf::Vector2f(dir * pos.x * deltaTime, 0.f) ;
		sprite.setPosition(newPos);
	}

	void checkCollision(float deltaTime)
	{
		if (col->checkCollision(sf::FloatRect(
				sprite.getGlobalBounds().left + velocity.x * deltaTime, sprite.getGlobalBounds().top,
				sprite.getGlobalBounds().width, sprite.getGlobalBounds().height
			), velocity, "All", CollisionType::RIGHT))
		{
			direction = -1;
		}
		else if (col->checkCollision(sf::FloatRect(
			sprite.getGlobalBounds().left + velocity.x * deltaTime * -1.f, sprite.getGlobalBounds().top,
			sprite.getGlobalBounds().width, sprite.getGlobalBounds().height
		), velocity, "All", CollisionType::LEFT))
		{
			direction = 1;
		}
	}

	void updateTimer(float deltaTime) 
	{
		if (timer < 1.f)
			timer += deltaTime;
		else
		{
			timer = 1.f;
			is_active = true;
		}
	}

	void update(float deltaTime) override;

	void render(sf::RenderTarget* target) override;
};