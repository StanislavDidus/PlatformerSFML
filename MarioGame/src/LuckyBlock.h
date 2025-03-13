#pragma once

#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include <iostream>
#include <vector>

#include "Gameobject.h"
#include "Block.h"
#include "Animator.h"
#include "MushRoom.h"
#include "Text.h"

class Game;

enum LuckyBlockType
{
	None, Mushroom, Coin, UP
};

class LuckyBlock : public Block
{
private:
	std::vector<std::shared_ptr<GameObject>>& gameObjects_;

	LuckyBlockType l_type;

	Game* game;

	std::shared_ptr<CollisionManager> col;

	sf::Texture coin_texture;
	sf::Sprite coin_sprite;

	std::unique_ptr<Text> score_text;

	void initLuckyBlock();
	void initCoin();
	void initScore();
	void initAnimations();

	std::unique_ptr<Animator> coin_animator;

public:
	LuckyBlock(Game* game, const sf::Sprite& sprite, const sf::FloatRect& rect, const std::string& type, const LuckyBlockType& l_type, std::vector<std::shared_ptr<GameObject>>& gameObjects, std::shared_ptr<CollisionManager> col);

	//std::shared_ptr<Animation> createPositionAnimation(sf::Sprite& sprite, float speed, std::string name, std::vector<sf::Vector2f> positions);

	void update(float deltaTime) override;
	void render(sf::RenderTarget* target) override;

	void onHit() override;

	const void spawnMushroom();
	const void giveCoin() const;
};
