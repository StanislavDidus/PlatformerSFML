#pragma once

#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include "Math.h"
#include "GameObject.h"

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

	CollisionEvent(sf::FloatRect fr, std::string col) : collider_bounds(fr), collider_type(col) {}
};

class CollisionManager
{
private:
	std::vector<CollisionEvent> collisions;
	std::vector<GameObject*> src_pos;

public:
	void addCollision(const CollisionEvent& collision) { collisions.emplace_back(collision); }
	void addSourse(GameObject* pos) { src_pos.emplace_back(pos); }

	std::vector<CollisionEvent> getCollisions() const { return collisions; }
	std::vector<GameObject*> getSources() const { return src_pos; }

	void clearCollision() { collisions.clear(); }

	bool checkCollision(const sf::FloatRect& player, const sf::Vector2f velocity, const std::string& type, const CollisionType& side)
	{
		for (const auto& col : collisions)
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
						if(velocity.y > 0)
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

	void callibrateCollision(const sf::FloatRect& player, float& x, float& y)
	{
		sf::FloatRect newBoundsX = { player.left + x, player.top, player.width, player.height };
		sf::FloatRect newBoundsY = { player.left , player.top + y, player.width, player.height };
		for (auto& object : collisions)
		{
			sf::FloatRect objectBounds = static_cast<sf::FloatRect>(object.collider_bounds);
			
			if (y != 0)
			{
				bool topCollision = newBoundsY.top < objectBounds.top + objectBounds.height &&
					newBoundsY.top > objectBounds.top &&
					newBoundsY.left < objectBounds.left + objectBounds.width &&
					newBoundsY.left + newBoundsY.width > objectBounds.left;

				bool bottomCollision = newBoundsY.top + newBoundsY.height > objectBounds.top &&
					newBoundsY.top + newBoundsY.height < objectBounds.top + objectBounds.height &&
					newBoundsY.left < objectBounds.left + objectBounds.width &&
					newBoundsY.left + newBoundsY.width > objectBounds.left;

				if (y < 0)
				{
					if (topCollision)
					{
						y = objectBounds.top + objectBounds.height - player.top;
						continue;
					}
				}

				else if (y > 0)
				{
					if (bottomCollision)
					{
						y = objectBounds.top - player.height - player.top;
						continue;
					}
				}
			}

			if (x != 0)
			{
				bool leftCollision = newBoundsX.left < objectBounds.left + objectBounds.width &&
					newBoundsX.left + newBoundsX.width > objectBounds.left &&
					newBoundsX.top < objectBounds.top + objectBounds.height &&
					newBoundsX.top + newBoundsX.height > objectBounds.top;

				bool rightCollision = newBoundsX.left + newBoundsX.width > objectBounds.left &&
					newBoundsX.left < objectBounds.left + objectBounds.width &&
					newBoundsX.top < objectBounds.top + objectBounds.height &&
					newBoundsX.top + newBoundsX.height > objectBounds.top;

				if (x < 0)
				{
					if (leftCollision)
					{
						x = objectBounds.left + objectBounds.width - player.left;
						continue;
					}

					
				}
				else if (x > 0)
				{
					if (rightCollision)
					{
						x = objectBounds.left - player.width - player.left;
						continue;
					}
				}

				if (newBoundsX.left < 0.f)
					x = 0.f;
			}
		}
	}
};

