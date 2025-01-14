#pragma once

#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

class GameObject
{
public:
	virtual ~GameObject() = default;

	virtual void update(float deltaTime) = 0;
	virtual void render(sf::RenderTarget* target) = 0;

	const virtual sf::Vector2f getPosition() const = 0;
	const virtual sf::FloatRect getBounds() const = 0;

	virtual void setPosition(const sf::Vector2f& pos) = 0;
};