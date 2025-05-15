#pragma once

#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include <iostream>
#include <vector>

#include "Gameobject.h"
#include "Managers/Animator.h"
#include "Block.h"

class Brick : public Block
{
private:
	sf::Texture broken_bricks_texture;
	std::vector<sf::Sprite> broken_bricks_sprites;

	std::vector<std::unique_ptr<Animator>> animators_bb; // broken brick animator

	sf::Vector2f velocity = { -150.f, -450.f };

	bool is_broken = false;
public:
	Brick(const sf::Sprite& sprite, sf::Texture* texture, sf::Texture* texture1, const sf::FloatRect& rect, const std::string& type, int layer, std::shared_ptr<CollisionManager> col, std::vector<std::shared_ptr<GameObject>>& gameObjects);

	void initSprites();
	void initAnimator();
	
	void onHit() override;
	void onHitBig() override;

	void update(float deltaTime) override;
	void render(sf::RenderTarget* target) override;

	void Destroy();
};
