#pragma once

#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>


#include <iostream>
#include <vector>
#include <memory>

struct CollisionEvent;

class QuadTree
{
private:
	sf::FloatRect rect;

	std::vector<std::unique_ptr<QuadTree>> subTrees;

	std::vector<CollisionEvent> collisions;

	int MAXCOLLISIONS = 15;
	int MAXDIVISIONS = 100;

	bool is_divided;
public:
	QuadTree(const sf::FloatRect& rect, int maxCol, int maxDiv);
	virtual ~QuadTree();

	void insert(const CollisionEvent& rect);
	void erase(const CollisionEvent& rect);
	void clear();
	void subdivide();
	void checkCollisions(const sf::FloatRect& player, std::vector<CollisionEvent>& collision);
};

