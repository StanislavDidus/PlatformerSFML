#pragma once

#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include <iostream>

#include "Gameobject.h"

class Brick : public GameObject
{
private:
	sf::Sprite sprite;
	sf::Texture texture;

	void initSprite();
public:
	Brick(const sf::Sprite& sprite, const sf::FloatRect& rect, const std::string& type);

	void setPosition(const sf::Vector2f& newPosition) override;

	void update(float deltaTime) override;
	void render(sf::RenderTarget* target) override;
};
