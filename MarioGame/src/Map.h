#pragma once

#include <tmxlite/Map.hpp>
#include <tmxlite/TileLayer.hpp>
#include <tmxlite/Tileset.hpp>
#include <tmxlite/Layer.hpp>
#include <tmxlite/ObjectGroup.hpp>

#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include "CollisionManager.h"
#include "GameObject.h"
#include "LuckyBlock.h"
#include "Brick.h"

#include <iostream>
#include <unordered_map>

class Game;

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

class Tile
{
private:
	sf::FloatRect posRect;
	sf::IntRect texRect;

	std::string type;

	int tile_set_index;
	
	bool is_collision;
	bool is_animation;

	std::shared_ptr<tmx::Tileset::Tile::Animation> animation;
	
	
public:
	int current_frame;
	std::unique_ptr<sf::Clock> anim_timer;

	Tile(const sf::FloatRect& p, const sf::IntRect& t, const std::string& ty, const bool col, const bool anim) : posRect(p), texRect(t), type(ty), is_collision(col), is_animation(anim)
	{
		
		//Init clock
		if (this->is_animation)
		{
			this->anim_timer = std::make_unique<sf::Clock>();
			this->current_frame = 0;
		}
	}

	const sf::FloatRect getPosition() const
	{
		return this->posRect;
	}
	const sf::IntRect getTextureRect() const
	{
		return this->texRect;
	}
	const std::string& getType() const
	{
		return this->type;
	}
	sf::Clock* getClock() const
	{
		return this->anim_timer.get();
	}
	std::shared_ptr<tmx::Tileset::Tile::Animation> getAnimation() const
	{
		if (!this->animation)
		{
			throw std::runtime_error("Animation is not set for this tile.");
		}
		return this->animation;
	}
	const int getFrame() const
	{
		return this->current_frame;
	}
	const bool isCollision() const
	{
		return this->is_collision;
	}
	const bool isAnimation() const
	{
		return this->is_animation;
	}
	void setTextureRect(const sf::IntRect& rect)
	{
		this->texRect = rect;
	}
	

	void setAnimation(std::shared_ptr<tmx::Tileset::Tile::Animation> anim)
	{
		this->animation = anim;
	}
};

class Map
{
private:
	std::vector<std::shared_ptr<GameObject>>& gameObjects_;

	std::vector<std::unique_ptr<tmx::Tileset>> tile_sets; // all tilesets
	std::vector<std::unique_ptr<sf::Texture>> textures; // all textures for tile sets
	tmx::Map tiled_map; // level tiled map

	std::unique_ptr<sf::VertexArray> v_array; // vertexArray for level

	//Sprites
	std::vector<Tile> tiles; // ALL tiles
	std::vector<std::unique_ptr<GameObject>> game_objects;

	std::unordered_map<int, tmx::Tileset::Tile> index_tile;
	std::unordered_map<int, sf::IntRect> index_texture;

	//Collision
	std::shared_ptr<CollisionManager> col_manager;

	//Window
	sf::RenderWindow* window;
	sf::RenderStates rs;

	//Game
	std::shared_ptr<Game> game;

	float updateTime = 1.0f / 30.0f; 
	float timeSinceLastUpdate = 0.0f;
	
	void initTiledMap();
	void initSprites();
	void initVerArray();
	void initCollisions();
public:
	Map(std::shared_ptr<Game> game, sf::RenderWindow* window, std::shared_ptr<CollisionManager> col, std::vector<std::shared_ptr<GameObject>>& gameObjects_);
	virtual ~Map();

	//const std::vector<sf::FloatRect> getLuckyBlocks() const;

	void updateAnimations();
	void updateCollisions();
	void update(float deltaTime);
	void render(sf::RenderTarget* target);
};

