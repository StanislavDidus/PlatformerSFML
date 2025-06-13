#pragma once

#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include <iostream>

#include "../Objects/Gameobject.h"
#include "Item.h"
#include "../Managers/CollisionManager.h"
#include "Block.h"
#include "../Managers/Animator.h"
#include "../Managers/AudioManager.h"

class Block : public GameObject
{
private:
	sf::Texture texture;
public:
	std::unique_ptr<Animator> animator;
	std::shared_ptr<CollisionManager> col;
	std::vector<std::shared_ptr<GameObject>>& gameObjects;
	std::unique_ptr<AudioManager> audio_manager;
	sf::Sprite sprite;
	Block(const sf::FloatRect& rect, const std::string& type, sf::Texture* texture, int layer, std::shared_ptr<CollisionManager> col, std::vector<std::shared_ptr<GameObject>>& gameObjects) : GameObject(type, rect, layer), col(col), gameObjects(gameObjects)
	{
		//Sprite
		
		is_destroyed = false;

		this->texture = *texture;
		sprite.setPosition(rect.left, rect.top);
		sprite.setTexture(this->texture);
		sprite.setTextureRect({ 0,0,16,16 });
		sprite.setScale(3.125f, 3.125f);

		audio_manager = std::make_unique<AudioManager>();
		audio_manager->addSound("Bump", "assets/Sounds/Player/Bump.wav", false);
		audio_manager->addSound("Brick_Smash", "assets/Sounds/Player/Brick_Smash.wav", false);

		
		//Animations
		animator = std::make_unique<Animator>();

		animator->addPosAnimation(
			sprite, 16 * 3.125f, 16 * 3.125f, 150.f, [this]() {return false; }, false, 25, std::vector<sf::Vector2f>{{sprite.getPosition().x, sprite.getPosition().y - 15.f}, { sprite.getPosition() }}, "Hit");
		
	
	}

	virtual ~Block() = default;

	void onHit() override
	{
		animator->playAnim("Hit");
		HitItem();
		audio_manager->playSound("Bump");
	}

	void onHitBig() override
	{
		HitItem();
	}

	virtual void HitItem()
	{
		//Check items above and hit them 
		sf::FloatRect bounds = { sprite.getGlobalBounds().left, sprite.getGlobalBounds().top - 50.f, sprite.getGlobalBounds().width, sprite.getGlobalBounds().height };
		for (const auto& obj : gameObjects)
		{
			if (obj->isActive())
			{
				if (bounds.intersects(obj->getBounds()))
				{
					Item* item = dynamic_cast<Item*>(obj.get());
					if (item != nullptr)
					{
						item->Jump();
					}
				}
			}
		}
	}

	void destroy()
	{
		is_destroyed = true;
		is_active = false;
	}

	sf::Sprite& getSprite()
	{
		return sprite;
	}

	sf::Texture& getTexture()
	{
		return texture;
	}

	void update(float deltaTime) override
	{
		animator->update(deltaTime);

		

		sf::Vector2f newPos = sprite.getPosition();
		setPosition(newPos);
	}
	
	void render(sf::RenderTarget* target) override
	{
		
		target->draw(sprite);
	}
};
