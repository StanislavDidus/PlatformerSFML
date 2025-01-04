#pragma once

//#include "Animator.h"

#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include <iostream>

class Map
{
private:
	std::vector<sf::Sprite> all_objects;
	std::vector<sf::Sprite> not_col_objects;
	std::vector<std::unique_ptr<sf::Texture>> textures;

	//Window
	sf::RenderWindow* window;
	
	void initSprites();
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

