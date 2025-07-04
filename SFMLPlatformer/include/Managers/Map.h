﻿#pragma once

#include <tmxlite/Map.hpp>
#include <tmxlite/TileLayer.hpp>
#include <tmxlite/Tileset.hpp>
#include <tmxlite/Layer.hpp>
#include <tmxlite/ObjectGroup.hpp>

#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include "CollisionManager.h"
#include "Objects/GameObject.h"
#include "Objects/LuckyBlock.h"
#include "Objects/Brick.h"
#include "Renderable.h"
#include "Managers/TextureManager.h"
#include "Algorythms/QuadTree.h"

#include <iostream>
#include <set>
#include <algorithm>
#include <unordered_map>

class Game;

struct Vrtx
{
	Vrtx(sf::VertexArray v_array, int layer) : v_array(v_array), layer(layer) {}
	
	sf::VertexArray v_array;
	int layer;
};

class Tile
{
private:
	sf::FloatRect posRect;
	sf::IntRect texRect;
	int layer;

	std::string type;

	int tile_set_index;
	
	bool is_collision;
	bool is_animation;

	std::shared_ptr<tmx::Tileset::Tile::Animation> animation;
	
	
public:
	int current_frame;
	std::unique_ptr<sf::Clock> anim_timer;

	Tile(const sf::FloatRect& p, const sf::IntRect& t, const std::string& ty, const bool col, const bool anim, int layer) : posRect(p), texRect(t), type(ty), is_collision(col),
		is_animation(anim), layer(layer), current_frame(0), tile_set_index(0)
	{
		
		//Init clock
		if (is_animation)
		{
			anim_timer = std::make_unique<sf::Clock>();
			current_frame = 0;
		}
	}

	const sf::FloatRect getPosition() const
	{
		return posRect;
	}
	const sf::IntRect getTextureRect() const
	{
		return texRect;
	}
	const std::string& getType() const
	{
		return type;
	}
	const int getLayer() const
	{
		return layer;
	}
	sf::Clock* getClock() const
	{
		return anim_timer.get();
	}
	std::shared_ptr<tmx::Tileset::Tile::Animation> getAnimation() const
	{
		if (!animation)
		{
			throw std::runtime_error("Animation is not set for this tile.");
		}
		return animation;
	}
	const int getFrame() const
	{
		return current_frame;
	}
	const bool isCollision() const
	{
		return is_collision;
	}
	const bool isAnimation() const
	{
		return is_animation;
	}
	void setTextureRect(const sf::IntRect& rect)
	{
		texRect = rect;
	}
	

	void setAnimation(std::shared_ptr<tmx::Tileset::Tile::Animation> anim)
	{
		animation = anim;
	}
};

class Map
{
private:
	std::vector<std::shared_ptr<GameObject>>& gameObjects;

	//Flag
	CollisionEvent flag;

	std::vector<std::unique_ptr<tmx::Tileset>> tile_sets; // all tilesets
	std::vector<std::unique_ptr<sf::Texture>> textures; // all textures for tile sets
	tmx::Map tiled_map; // level tiled map

	std::unique_ptr<sf::VertexArray> v_array; // vertexArray for level
	std::vector<std::unique_ptr<sf::VertexArray>> v_arrays;
	std::vector<Vrtx> vrtxs;

	//Sprites
	std::vector<std::shared_ptr<Tile>> tiles; // ALL tiles
	std::vector<std::shared_ptr<GameObject>> blocks; // LuckyBlocks, Bricks, Walls

	std::unordered_map<int, tmx::Tileset::Tile> index_tile;
	std::unordered_map<int, sf::IntRect> index_texture;

	//Collision
	std::shared_ptr<CollisionManager> col_manager;

	//Window
	sf::RenderWindow* window;
	sf::RenderStates rs;

	//Game
	std::shared_ptr<TextureManager> texture_manager;
	std::shared_ptr<Game> game;
	std::shared_ptr<QuadTree> quadTree;
	bool quadTreeNeedsUpdate = false;

	float updateTime = 1.0f / 30.0f; 
	float timeSinceLastUpdate = 0.0f;
	
	void initTiledMap();
	void initSprites();
	void initVerArray();
	void initCollisions();
public:
	Map(std::shared_ptr<Game> game, sf::RenderWindow* window, std::shared_ptr<CollisionManager> col, std::shared_ptr<TextureManager> texture_manager, std::shared_ptr<QuadTree> quadTree, std::vector<std::shared_ptr<GameObject>>& gameObjects, const CollisionEvent& flag);
	virtual ~Map();

	//const std::vector<sf::// FloatRect> getLuckyBlocks() const;

	void updateAnimations();
	void updateCollisions();
	void update(float deltaTime);
	void render(std::vector<Renderable>& queue, sf::RenderTarget* target);
};

