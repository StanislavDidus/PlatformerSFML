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
	int layer;
	bool is_active; //Is object active
	bool is_destroyed;
	bool is_collision = true; //Collision work

	GameObject(std::string type, sf::FloatRect bounds, int layer) : type(type), bounds(bounds), layer(layer), is_active(true) {}

	virtual ~GameObject() = default;

	virtual void onHit() {}; // When hit by small mario
	virtual void onHitBig() {}; // When hit by big mario

	virtual void update(float deltaTime) {}
	virtual void render(sf::RenderTarget* target) = 0;

	const sf::FloatRect getBounds() const { return bounds; };	
	const std::string getType() const { return type; };

	const bool isActive() const { return is_active; };
	virtual const bool isDestroyed() const { return is_destroyed; };

	const sf::Vector2f getPosition() { return { bounds.left, bounds.top }; };
	virtual void setPosition(const sf::Vector2f& newPosition) {
		bounds.left = newPosition.x;
		bounds.top = newPosition.y;
		
	}
	
	virtual void setBounds(const sf::Vector2f& newBounds)
	{
		bounds.width = newBounds.x;
		bounds.height = newBounds.y;
	}

	bool operator==(const GameObject& other) const
	{
		return this->type == other.type && this->bounds == other.bounds;
	}
};