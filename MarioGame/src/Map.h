#pragma once

#include <tmxlite/Map.hpp>
#include <tmxlite/TileLayer.hpp>
#include <tmxlite/Tileset.hpp>

#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include "CollisionManager.h"

#include <iostream>
#include <unordered_map>

struct AnimationTile
{
	sf::FloatRect posRect;
	sf::IntRect texRect;
	tmx::Tileset::Tile::Animation animation;
	sf::Clock clock;
	int current_frame = 0;

	AnimationTile(const sf::FloatRect& posRect, const sf::IntRect& rect, const tmx::Tileset::Tile::Animation& animation)
		: posRect(posRect), texRect(rect), animation(animation) {
	}
};

class Map
{
private:
	std::vector<std::unique_ptr<tmx::Tileset>> tile_sets;
	std::vector<std::unique_ptr<sf::Texture>> textures;
	tmx::Map tiled_map;
	//tmx::Tileset* tile_set;

	std::unique_ptr<sf::VertexArray> v_array;
	std::unique_ptr<sf::VertexArray> animated_v_array;

	//Sprites
	std::vector<AnimationTile> animation_tiles;
	std::vector<sf::IntRect> collide_tiles;
	std::vector<std::pair<sf::FloatRect, sf::IntRect>> all_tiles;
	std::vector<std::pair<sf::FloatRect, std::string>> tiles_type;

	std::vector<sf::FloatRect> lucky_blocks;

	//Collision
	CollisionManager* col_manager;

	//Window
	sf::RenderWindow* window;
	sf::RenderStates rs;

	float updateTime = 1.0f / 30.0f; 
	float timeSinceLastUpdate = 0.0f;
	
	void initTiledMap();
	void initSprites();
	void initVerArray();
public:
	Map(sf::RenderWindow* window, CollisionManager* col);
	virtual ~Map();

	const void calibrateCollision(const sf::FloatRect& player, float& x, float& y);
	const bool checkGround(const sf::FloatRect& player, float x, float y);
	const bool checkRoof(const sf::FloatRect& player, float x, float y);

	const std::vector<sf::FloatRect> getLuckyBlocks() const;

	void updateAnimations();
	void updateCollisions();
	void update(float deltaTime);
	void render(sf::RenderTarget* target);
};

