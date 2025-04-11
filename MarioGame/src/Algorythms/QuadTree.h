#pragma once

#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include <iostream>
#include <vector>
#include <memory>

class QuadTree
{
private:
	sf::FloatRect rect;

	std::vector<std::unique_ptr<QuadTree>> subTrees;

	std::vector<sf::FloatRect> collisions;

	int MAXCOLLISIONS = 4;
	int MAXDIVISIONS = 1000;

	bool is_divided;
public:
	QuadTree(const sf::FloatRect& rect);
	virtual ~QuadTree();

	void insert(const sf::FloatRect& rect);
	void subdivide();
	void checkCollisions(const sf::FloatRect& player, std::vector<sf::FloatRect>& collision);
};

