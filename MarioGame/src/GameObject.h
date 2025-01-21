#pragma once

#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include <iostream>
#include <string>

class GameObject
{
private:
	std::string type;
	sf::FloatRect bounds;
public:
	GameObject(std::string type, sf::FloatRect bounds) : type(type), bounds(bounds)
	{

	}

	GameObject() : type("None"), bounds(0,0,50,50)
	{

	}

	virtual ~GameObject() = default;

	virtual void update(float deltaTime) = 0;
	virtual void render(sf::RenderTarget* target) = 0;

	const virtual sf::Vector2f getPosition() const = 0;
	const virtual sf::FloatRect getBounds() const = 0;

	virtual void setPosition(const sf::Vector2f& pos) = 0;
};