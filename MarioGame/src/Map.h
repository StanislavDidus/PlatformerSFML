#pragma once

#include <tmxlite/Map.hpp>
#include <tmxlite/TileLayer.hpp>
#include <tmxlite/Tileset.hpp>

#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include "CollisionManager.h"
#include "GameObject.h"

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
	std::vector<std::unique_ptr<tmx::Tileset>> tile_sets; // all tilesets
	std::vector<std::unique_ptr<sf::Texture>> textures; // all textures for tile sets
	tmx::Map tiled_map; // level tiled map

	std::unique_ptr<sf::VertexArray> v_array; // vertexArray for level
	std::unique_ptr<sf::VertexArray> animated_v_array; // vertexArray for animated tiles

	//Sprites
	std::vector<AnimationTile> animation_tiles; // All animated tiles
	std::vector<std::pair<sf::FloatRect, sf::IntRect>> all_tiles; // All tiles except animated
	std::vector<std::pair<sf::FloatRect, std::string>> tiles_type; // All tiles with collisions

	std::vector<GameObject> gameObjects; // gameObject (LuckyBlocks, Bricks)

	std::vector<sf::FloatRect> lucky_blocks; // All lucky blocks

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
	void initCollisions();
public:
	Map(sf::RenderWindow* window, CollisionManager* col);
	virtual ~Map();

	//const std::vector<sf::FloatRect> getLuckyBlocks() const;

	void updateAnimations();
	void updateCollisions();
	void update(float deltaTime);
	void render(sf::RenderTarget* target);
};

