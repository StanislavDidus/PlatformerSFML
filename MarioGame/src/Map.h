#pragma once

#include <tmxlite/Map.hpp>
#include <tmxlite/TileLayer.hpp>
#include <tmxlite/Tileset.hpp>

#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include <iostream>
#include <unordered_map>

class Map
{
private:
	std::vector<sf::Sprite> all_objects;
	std::vector<sf::Sprite> not_col_objects;
	
	std::vector<std::unique_ptr<tmx::Tileset>> tile_sets;
	std::vector<std::unique_ptr<sf::Texture>> textures;
	tmx::Map tiled_map;
	tmx::Tileset* tile_set;

	std::unique_ptr<sf::VertexArray> v_array;
	std::unique_ptr<sf::Image> image;

	//Sprites
	std::vector<std::unordered_map<sf::IntRect, tmx::Tileset::Tile::Animation>> animation_tiles;
	std::vector<sf::IntRect> collide_tiles;
	std::vector <std::pair<sf::FloatRect, sf::IntRect>> all_tiles;

	//Window
	sf::RenderWindow* window;
	
	void initTiledMap();
	void initSprites();
	void initVerArray();
	void initImage();
public:
	Map(sf::RenderWindow* window);
	virtual ~Map();

	const void calibrateCollision(const sf::FloatRect& player, float& x, float& y);
	const bool checkGround(const sf::FloatRect& player, float x, float y);
	const bool checkRoof(const sf::FloatRect& player, float x, float y);

	sf::Vector2f getBlock(float x, float y, float mult);

	void addObject(const std::string& texturePath, const sf::Vector2f& position, const sf::Vector2f& scale, bool is_collision);

	void update();
	void render(sf::RenderTarget* target);
};

