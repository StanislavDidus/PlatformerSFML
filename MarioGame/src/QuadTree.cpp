#include "Algorythms/QuadTree.h"

#include "Managers\CollisionManager.h"

QuadTree::QuadTree(const sf::FloatRect& rect, int maxCol, int maxDiv) : rect(rect), is_divided(false), MAXCOLLISIONS(maxCol), MAXDIVISIONS(maxDiv)
{

}

QuadTree::~QuadTree()
{

}

void QuadTree::insert(const CollisionEvent& bound)
{
	if (!rect.intersects(bound.collider_bounds))
		return;

	//Check if reachead a collisions limit
	if (is_divided)
	{
		for (const auto& quadTree : subTrees)
		{
			quadTree->insert(bound);
		}
	}
	else
	{
		collisions.reserve(MAXCOLLISIONS);
		collisions.push_back(bound);

		if (collisions.size() > MAXCOLLISIONS)
		{
			subdivide();
		}
	}
}

void QuadTree::clear()
{
	if (is_divided)
	{
		subTrees.clear();
		is_divided = false;
		MAXDIVISIONS = 50;
	}
	collisions.clear();
}

void QuadTree::subdivide()
{
	is_divided = true;
	
	subTrees.reserve(4);

	subTrees.emplace_back(std::make_unique<QuadTree>(sf::FloatRect(rect.left, rect.top, rect.width / 2, rect.height / 2), MAXCOLLISIONS, MAXDIVISIONS)); //Left-Up
	subTrees.emplace_back(std::make_unique<QuadTree>(sf::FloatRect(rect.left + (rect.width / 2), rect.top, rect.width / 2, rect.height / 2), MAXCOLLISIONS, MAXDIVISIONS)); //Right-Up
	subTrees.emplace_back(std::make_unique<QuadTree>(sf::FloatRect(rect.left, rect.top + (rect.height / 2), rect.width / 2, rect.height / 2), MAXCOLLISIONS, MAXDIVISIONS)); //Left-Down
	subTrees.emplace_back(std::make_unique<QuadTree>(sf::FloatRect(rect.left + (rect.width / 2), rect.top + (rect.height / 2), rect.width / 2, rect.height / 2), MAXCOLLISIONS, MAXDIVISIONS)); //Right-Down

	for (const auto& col : collisions)
	{
		for (auto& tree : subTrees)
		{
			if (tree->rect.intersects(col.collider_bounds))
			{
				tree->insert(col);
			}
		}
	}
	collisions.clear();
}

void QuadTree::checkCollisions(const sf::FloatRect& player, std::vector<CollisionEvent>& results)
{
	if (!player.intersects(rect))
		return;
	
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
			if ((col.object != nullptr && col.object->is_collision) || col.object == nullptr)
				results.push_back(col);
		}
	}
}
