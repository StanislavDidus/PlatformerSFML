#pragma once

#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include <iostream>

#include "Gameobject.h"
#include "Block.h"
#include "Animator.h"

class Block : public GameObject
{
private:
	
	sf::Texture texture;

	bool is_destroyed;
public:
	std::unique_ptr<Animator> animator;
	sf::Sprite sprite;
	Block(const sf::FloatRect& rect, const std::string& type, const std::string& sprite_path) : GameObject(type, rect)
	{
		//Sprite
		
		this->is_destroyed = false;

		if (!this->texture.loadFromFile(sprite_path)) {
			std::cerr << "Failed to load texture: " << sprite_path << std::endl;
		}
		this->sprite.setPosition(rect.left, rect.top);
		this->sprite.setTexture(this->texture);
		this->sprite.setTextureRect({ 0,0,16,16 });
		this->sprite.setScale(3.125f, 3.125f);

		
		//Animations
		animator = std::make_unique<Animator>();

		this->animator->addPosAnimation(
		this->sprite, 16 * 3.125f, 16 * 3.125f, 150.f, [this]() {return false; }, false, 25, { {this->sprite.getPosition()},{this->sprite.getPosition().x, this->sprite.getPosition().y - 15.f} }, "Hit");
		
	
	}

	virtual ~Block() = default;

	void onHit() override
	{
		animator->playAnim("Hit");
	}

	void destroy()
	{
		this->is_destroyed = true;
	}

	const bool isDesroyed() const
	{
		return this->is_destroyed;
	}

	sf::Sprite& getSprite()
	{
		return this->sprite;
	}

	sf::Texture& getTexture()
	{
		return this->texture;
	}

	void update(float deltaTime) override
	{
		animator->update(deltaTime);

		sf::Vector2f newPos = this->sprite.getPosition();
		setPosition(newPos);
	}
	
	void render(sf::RenderTarget* target) override
	{
		target->draw(this->sprite);
	}
};
