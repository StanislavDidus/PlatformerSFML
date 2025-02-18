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
	
	GameObject(std::string type, sf::FloatRect bounds) : type(type), bounds(bounds) {}

	virtual ~GameObject() = default;

	virtual void onHit() {}; 

	virtual void update(float deltaTime) {}
	virtual void render(sf::RenderTarget* target) = 0;

	const sf::FloatRect getBounds() const { return bounds; };	
	const std::string getType() const { return type; };

	virtual void setPosition(const sf::Vector2f& newPosition) {
		bounds.left = newPosition.x;
		bounds.top = newPosition.y;
	}
};