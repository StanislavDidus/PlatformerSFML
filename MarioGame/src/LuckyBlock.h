#pragma once

#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include <iostream>

#include "Gameobject.h"
#include "Animator.h"

class LuckyBlock : public GameObject
{
private:
	sf::Sprite sprite;
	sf::Texture texture;

	std::unique_ptr<Animator> animator;

	int x = 0;

	void initSprite();
	void initAnimations();
public:
	LuckyBlock(const sf::Sprite& sprite, const sf::FloatRect& rect, const std::string& type);

	void onHit() override;

	void setPosition(const sf::Vector2f& newPosition) override;

	void update(float deltaTime) override;
	void render(sf::RenderTarget* target) override;
};
