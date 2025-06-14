#pragma once

#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include "Algorythms/Math.h"
#include "Objects/GameObject.h"
#include "Algorythms/QuadTree.h"

#include <iostream>
#include <vector>
#include <string>
#include <functional> 

enum CollisionType
{
	LEFT, RIGHT, TOP, DOWN, ALL
};

struct CollisionEvent
{
	sf::FloatRect collider_bounds;
	std::string collider_type;
	GameObject* object;

	CollisionEvent(sf::FloatRect fr, std::string col) : collider_bounds(fr), collider_type(col), object(nullptr) {}
	CollisionEvent(sf::FloatRect fr, std::string col, GameObject* object) : collider_bounds(fr), collider_type(col), object(object) {}
};

class CollisionManager
{
private:
	std::shared_ptr<QuadTree> quadTree;
	sf::RenderTarget* target;

public:
	CollisionManager(std::shared_ptr<QuadTree> quadTree, sf::RenderTarget* target) : quadTree(quadTree), target(target) {}

	bool checkCollision(const sf::FloatRect& player, const sf::Vector2f& velocity, const std::string& type, const CollisionType& side)
	{
		//Get all possible collition using QuadTree
		sf::FloatRect playerBounds = player;
		std::vector<CollisionEvent> possible_collisions;
		if (quadTree != nullptr)
			quadTree->checkCollisions(sf::FloatRect(playerBounds.left, playerBounds.top, playerBounds.width, playerBounds.height), possible_collisions);

		//Check collision with all possible game objects with chosen directions and types
		for (const auto& col : possible_collisions)
		{
			if (col.collider_type == type || type == "All")
			{

				sf::FloatRect object = col.collider_bounds;

				switch (side)
				{
				case CollisionType::LEFT:
					if (player.left < object.left + object.width &&
						player.left + player.width > object.left &&
						player.top < object.top + object.height &&
						player.top + player.height > object.top)
					{
						return true;
					}
					break;
				case CollisionType::RIGHT:
					if (player.left + player.width > object.left &&
						player.left < object.left + object.width &&
						player.top < object.top + object.height &&
						player.top + player.height > object.top)
					{
						return true;
					}
					break;
				case CollisionType::TOP:
					if (player.top < object.top + object.height &&
						player.top + player.height > object.top &&
						player.left < object.left + object.width &&
						player.left + player.width > object.left)
					{
						return true;
					}
					break;
				case CollisionType::DOWN:
					if (player.top + player.height > object.top &&
						player.top < object.top + object.height &&
						player.left < object.left + object.width &&
						player.left + player.width > object.left)
					{
						if (velocity.y > 0)
							return true;
					}
					break;
				case CollisionType::ALL:
					if (player.intersects(object))
						return true;

					break;
				}
			}
		}
		return false;
	}

	GameObject* getObject(const sf::FloatRect& player, const sf::Vector2f& velocity, const std::string& type)
	{
		//Get all possible collition using QuadTree
		sf::FloatRect playerBounds = player;
		std::vector<CollisionEvent> possible_collisions;
		if (quadTree != nullptr)
			quadTree->checkCollisions(sf::FloatRect(playerBounds.left, playerBounds.top, playerBounds.width, playerBounds.height), possible_collisions);

		std::vector<GameObject*> gameObjects;
		GameObject* closestGO = nullptr;
		for (const auto& col : possible_collisions)
		{
			if (col.collider_type == type)
			{
				if (col.object != nullptr)
				{
					sf::FloatRect object = col.collider_bounds;

					if (player.intersects(object) && col.object->is_collision)
					{
						
						gameObjects.push_back(col.object);
					}

				}
			}
		}

		//Return the closes gameObjects
		for (GameObject* go : gameObjects)
		{
			if (!closestGO || MathUtils::distance(go->getBounds(), player) < MathUtils::distance(closestGO->getBounds(), player))
			{
				closestGO = go;
			}
		}

		return closestGO;
	}

	void callibrateCollision(GameObject& player, float& x, float& y)
	{
		//Get all possible collition using QuadTree
		sf::FloatRect playerBounds = player.getBounds();

		std::vector<CollisionEvent> possible_collisions;
		if (quadTree != nullptr)
			quadTree->checkCollisions({ playerBounds.left + x, playerBounds.top + y, playerBounds.width, playerBounds.height }, possible_collisions);

		sf::FloatRect newBoundsY = playerBounds;
		newBoundsY.top += y;

		for (const auto& object : possible_collisions)
		{
			sf::FloatRect objectBounds = object.collider_bounds;

			if (newBoundsY.intersects(objectBounds))
			{
				float yOverlap = std::min(newBoundsY.top + newBoundsY.height, objectBounds.top + objectBounds.height) - std::max(newBoundsY.top, objectBounds.top);
				if (yOverlap > 0)
				{
					if (newBoundsY.top < objectBounds.top)
					{
						y = -yOverlap;
					}
					else
					{
						y = yOverlap;
					}
				}

			}
		}

		sf::FloatRect newBoundsX = playerBounds;
		newBoundsX.left += x;

		for (const auto& object : possible_collisions)
		{
			sf::FloatRect objectBounds = object.collider_bounds;
			if (newBoundsX.intersects(objectBounds))
			{
				float xOverlap = std::min(newBoundsX.left + newBoundsX.width, objectBounds.left + objectBounds.width) - std::max(newBoundsX.left, objectBounds.left);

				if (xOverlap > 0)
				{
					if (newBoundsX.left < objectBounds.left)
					{
						x = -xOverlap;
					}
					else
					{
						x = xOverlap;
					}
				}
			}
		}

		//Check collision with all possible game objects with chosen directions
		//And change x and y variables to the number that player does not go inside the collider
		//for (const auto& object : possible_collisions)
		//{
		//	//Check Y collisions
		//	sf::FloatRect objectBounds = object.collider_bounds;
		//	float playerBottom = playerBounds.top + playerBounds.height;

		//	if (y > 0)
		//	{
		//		if (newBoundsY.intersects(objectBounds))
		//		{
		//			y = objectBounds.top - playerBottom;
		//			newBoundsY.top = playerBounds.top + y;
		//		}
		//	}
		//	else if (y < 0) 
		//	{
		//		if (newBoundsY.intersects(objectBounds))
		//		{
		//			y = (objectBounds.top + objectBounds.height) - playerBounds.top;
		//			newBoundsY.top = playerBounds.top + y;
		//		}
		//	}
		//}

		//playerBounds.top += y;

		////Check X positions
		//sf::FloatRect newBoundsX = playerBounds;
		//newBoundsX.left += x;

		//for (const auto& object : possible_collisions)
		//{
		//	sf::FloatRect objectBounds = object.collider_bounds;

		//	if (x < 0) 
		//	{
		//		if (newBoundsX.intersects(objectBounds))
		//		{
		//			x = objectBounds.left + objectBounds.width - playerBounds.left;
		//		}
		//	}
		//	else if (x > 0) 
		//	{
		//		if (newBoundsX.intersects(objectBounds))
		//		{
		//			x = objectBounds.left - playerBounds.width - playerBounds.left;
		//		}
		//	}
		//}
	}
};

