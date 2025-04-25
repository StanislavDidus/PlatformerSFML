#pragma once

#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include "GameObject.h"
#include "Animator.h"
#include "CollisionManager.h"
#include "Manager/TextureManager.h"

#include "string"

class Game;

class Item : public GameObject
{
private:
	

	float timer = 0.f;

	void initSprite()
	{
		this->sprite.setScale(3.125f, 3.125f);
		this->sprite.setPosition(getPosition());
	}

	void initAnimations()
	{
		this->animator = std::make_unique<Animator>();

		this->animator->addPosAnimation(
			sprite, 16, 16, 75.f, [this]() {return false; }, false, 10, { {this->sprite.getPosition()},{this->sprite.getPosition().x, this->sprite.getPosition().y - 16 * 3.125f} }, "Spawn"
		);
	}
public:
	std::shared_ptr<Game> game;

	int direction = 1;
	sf::Vector2f velocity = { 250.f, 500.f };

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

		this->animator->playAnim("Spawn");
	}
	virtual ~Item() {  }

	//const bool isActive() const { return is_active; }

	void Jump()
	{
		std::cout << "Jump\n";
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