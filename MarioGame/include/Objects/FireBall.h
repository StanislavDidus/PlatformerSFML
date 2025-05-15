#pragma once

#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include <iostream>
#include <vector>

#include "Gameobject.h"
#include "Managers/Animator.h"
#include "Managers/CollisionManager.h"
#include "Managers/TextureManager.h"

class FireBall : public GameObject
{
private:
	sf::Texture texture;
	sf::Sprite sprite;

	sf::Texture explosion_texture;
	sf::Sprite explosion_sprite;

	std::shared_ptr<Animator> animator;

	std::shared_ptr<CollisionManager> collision_manager;
	std::shared_ptr<TextureManager> texture_manager;

	sf::Vector2f velocity = { 0.f,0.f };
	int dir = 1;
	float gravity = 1225.f;

	void initSprite();
	void initAnimator();
public:
	FireBall(const std::string& type, sf::FloatRect bounds, int layer, std::shared_ptr<TextureManager> texture_manager, std::shared_ptr<CollisionManager> collision_manager, int dir);
	virtual ~FireBall();

	void setPosition(sf::Vector2f pos);

	void update(float deltaTime);
	void updateCollisions(float deltaTime);
	void render(sf::RenderTarget* target);
};

