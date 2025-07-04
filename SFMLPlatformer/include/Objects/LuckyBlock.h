#pragma once

#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include <iostream>
#include <vector>

#include "Gameobject.h"
#include "Block.h"
#include "Managers/Animator.h"
#include "Items/MushRoom.h"
#include "Text.h"
#include "Items/FireFlower.h"
#include "Managers/TextureManager.h"

class Game;

enum LuckyBlockType
{
	None, MushRoomType, CoinType, UPType, FireFlowerType
};

class LuckyBlock : public Block
{
private:
	LuckyBlockType l_type;

	std::shared_ptr<Game> game;

	std::shared_ptr<TextureManager> texture_manager;

	bool ready_to_spawn;

	sf::Texture coin_texture;
	sf::Sprite coin_sprite;

	std::unique_ptr<Text> score_text;

	void initLuckyBlock();
	void initCoin();
	void initScore();
	void initAnimations();

	std::unique_ptr<Animator> coin_animator;

public:
	LuckyBlock(std::shared_ptr<Game> game, const sf::Sprite& sprite, std::shared_ptr<TextureManager> texture_manager, const sf::FloatRect& rect, const std::string& type, const LuckyBlockType& l_type, std::vector<std::shared_ptr<GameObject>>& gameObjects, std::shared_ptr<CollisionManager> col, int layer);
	virtual ~LuckyBlock() { }
	//std::shared_ptr<Animation> createPositionAnimation(sf::Sprite& sprite, float speed, std::string name, std::vector<sf::Vector2f> positions);

	void update(float deltaTime) override;
	void render(sf::RenderTarget* target) override;

	void onHit() override;
	void onHitBig() override;

	const void spawnMushroom();
	const void spawnFireFlower();
	const void giveCoin() const;
};
