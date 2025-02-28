#pragma once

#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include <iostream>

#include "Gameobject.h"
#include "Block.h"
#include "Animator.h"

class Game;

enum LuckyBlockType
{
	None, Mushroom, Coin, UP
};

class LuckyBlock : public Block
{
private:
	LuckyBlockType l_type;

	Game* game;

	bool is_active = true;

	sf::Texture coin_texture;
	sf::Sprite coin_sprite;

	sf::Texture score_texture;
	sf::Sprite score_sprite;

	void initLuckyBlock();
	void initCoin();
	void initScore();
	void initAnimations();

	std::unique_ptr<Animator> coin_animator;

public:
	LuckyBlock(Game* game, const sf::Sprite& sprite, const sf::FloatRect& rect, const std::string& type, const LuckyBlockType& l_type);

	std::shared_ptr<Animation> createPositionAnimation(sf::Sprite& sprite, float speed, std::string name, std::pair<sf::Vector2f, sf::Vector2f> points);

	void update(float deltaTime) override;
	void render(sf::RenderTarget* target) override;

	void onHit() override;

	const void giveCoin() const;
};
