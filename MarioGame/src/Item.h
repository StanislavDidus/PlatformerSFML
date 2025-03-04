#pragma once

#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include "GameObject.h"
#include "Animator.h"

#include "string"

class Item : public GameObject
{
private:
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
	sf::Texture texture;
	sf::Sprite sprite;

	std::unique_ptr<Animator> animator;

	Item(const sf::FloatRect& rect, const std::string& name) : GameObject(name, rect)
	{
		initSprite();
		initAnimations();

		this->animator->playAnim("Spawn");
		std::cout << "Spawn\n";
	}
	virtual ~Item() {}

	//void setTexture(const sf::Texture& texture) { this->texture = texture; }
	//void setSprite(const sf::Sprite& sprite) { this->sprite = sprite; }

	//const sf::Sprite getSprite() const { return sprite; }
	//const sf::Texture getTexture() const { return texture; }

	void update(float deltaTime) override
	{

	}

	void render(sf::RenderTarget* target) override
	{
		target->draw(sprite);
	}
};