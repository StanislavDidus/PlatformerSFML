#include "QuadTree.h"

QuadTree::QuadTree(const sf::FloatRect& rect) : rect(rect), is_divided(false)
{

}

QuadTree::~QuadTree()
{

}

void QuadTree::insert(const sf::FloatRect& rect)
{
	if (!this->rect.intersects(rect))
		return;

	//Check if reachead a collisions limit
	if (collisions.size() - 1 < MAXCOLLISIONS && !is_divided)
	{
		collisions.push_back(rect);
	}
	else
	{
		//Divide
		subdivide();
		
		for (const auto& quadTree : subTrees)
		{
			quadTree->insert(rect);
		}
	}
}

void QuadTree::subdivide()
{
	is_divided = true;

	subTrees.emplace_back(std::make_unique<QuadTree>(sf::FloatRect(rect.left, rect.top, rect.width / 2, rect.height / 2))); //Left-Up
	subTrees.emplace_back(std::make_unique<QuadTree>(sf::FloatRect(rect.left + (rect.width / 2), rect.top, rect.width / 2, rect.height / 2))); //Right-Up
	subTrees.emplace_back(std::make_unique<QuadTree>(sf::FloatRect(rect.left, rect.top + (rect.height / 2), rect.width / 2, rect.height / 2))); //Left-Down
	subTrees.emplace_back(std::make_unique<QuadTree>(sf::FloatRect(rect.left + (rect.width / 2), rect.top + (rect.height / 2), rect.width / 2, rect.height / 2))); //Right-Down

	for (const auto& col : collisions)
	{
		for (auto& tree : subTrees)
		{
			tree->insert(col);
		}
	}
	collisions.clear();
}

void QuadTree::checkCollisions(const sf::FloatRect& player, std::vector<sf::FloatRect>& results)
{
	if(is_divided)
		for (const auto& tree : subTrees)
		{
			if (player.intersects(tree->rect))
			{
				tree->checkCollisions(player, results);
			}
		}
	else if(!is_divided)
	{
		for (const auto& col : collisions)
		{
			if(player.intersects(col))
				results.push_back(col);
		}
	}
}
