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
	sf::FloatRect* collider_bounds;
	std::string collider_type;
	GameObject* object;

	CollisionEvent(sf::FloatRect fr, std::string col) : collider_bounds(fr), collider_type(col), object(nullptr) {}
	CollisionEvent(sf::FloatRect* fr, std::string col, GameObject* object) : collider_bounds(fr), collider_type(col), object(object) {}
};

class CollisionManager
{
private:
	
	std::vector<GameObject*> src_pos;

	float current_time = 0.f;

public:
	std::vector<CollisionEvent> collisions;
	std::vector<CollisionEvent> temp_collisions;
	void addCollision(const CollisionEvent& collision) { collisions.emplace_back(collision); }
	void addSourse(GameObject* pos) { src_pos.emplace_back(pos); }

	void update(float deltaTime)
	{
		this->current_time += deltaTime;
		
		if (this->current_time >= 1.f / 30.f)
		{
			this->temp_collisions.clear();

			for (const auto& col : this->collisions)
			{
				sf::FloatRect* collision = col.collider_bounds;

				bool founded = false;
				for (const auto& object : this->getSources())
				{
					sf::FloatRect obj = object->getBounds();

					sf::FloatRect col_bounds = { collision->left + collision.width, collision.top + collision.height, collision.width, collision.height };
					sf::FloatRect obj_bounds = { obj.left + obj.width, obj.top + obj.height, obj.width, obj.height };

					if (MathUtils::distance(col_bounds, obj_bounds) < obj_bounds.width * 3.f)
					{
						founded = true;
						break;
					}
				}

				if (founded)
					this->temp_collisions.emplace_back(col);
			}

			this->current_time = 0.f;
		}
	}

	std::vector<CollisionEvent> getCollisions() const { return collisions; }
	std::vector<GameObject*> getSources() const { return src_pos; }

	void clearCollision() { collisions.clear(); }

	bool checkSources(const CollisionEvent& col)
	{
		
	}

	bool checkCollision(const sf::FloatRect& player, const sf::Vector2f velocity, const std::string& type, const CollisionType& side)
	{
		for (const auto& col : temp_collisions)
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

	GameObject* getObject(const sf::FloatRect& player, const sf::Vector2f velocity, const std::string& type)
	{
		std::vector<GameObject*> gameObjects;
		GameObject* closestGO = nullptr;
		for (const auto& col : temp_collisions)
		{
			if (col.collider_type == type)
			{
				if (col.object != nullptr)
				{
					sf::FloatRect object = col.collider_bounds;

					if (player.intersects(object) && col.object->isActive())
					{
						gameObjects.push_back(col.object);
					}
					
				}
			}
		}

		for (GameObject* go : gameObjects)
		{
			if (!closestGO || MathUtils::distance(go->getBounds(), player) < MathUtils::distance(closestGO->getBounds(), player))
			{
				closestGO = go;
			}
		}

		return closestGO;
	}
	
	void callibrateCollision(const sf::FloatRect& player, float& x, float& y)
	{
		sf::FloatRect newBoundsX = { player.left + x, player.top, player.width, player.height };
		sf::FloatRect newBoundsY = { player.left , player.top + y, player.width, player.height };
		for (const auto& object : temp_collisions)
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

