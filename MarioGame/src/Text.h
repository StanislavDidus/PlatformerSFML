#pragma once

#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include "Animator.h"

#include <string>

class Text
{
private:
	sf::Sprite sprite;
	sf::Texture texture;

	std::shared_ptr<Animator> animator;

	sf::Vector2f pos;

	std::string path;

	int w, h;

	void initSprite()
	{
		sprite.setTexture(texture);
		sprite.setScale(3.125f, 3.125f);
		sprite.setTextureRect(sf::IntRect(0, 0, w, h));
		sprite.setPosition(pos.x, pos.y - 40.f);
	}

	void initAnimator()
	{
		animator = std::make_shared<Animator>();
		animator->addPosAnimation(sprite, sprite.getGlobalBounds().width / 2.f, sprite.getGlobalBounds().height / 1.5f, 90.f, [this]() {return false; }, false, 10, std::vector<sf::Vector2f>{{pos.x, pos.y - 50}, { pos.x, pos.y - 85 }}, "Score");
	}
public:
	Text(int w, int h, const sf::Vector2f& pos, sf::Texture* texture) : path(path), w(w), h(h), pos(pos), texture(*texture)
	{
		initSprite();
		initAnimator();
	}

	virtual ~Text() {}

	void update(float deltaTime)
	{
		animator->update(deltaTime);
	}

	void render(sf::RenderTarget* target)
	{
		target->draw(sprite);
	}

	const sf::Sprite getSprite() const { return sprite; }
	std::shared_ptr<Animation> getAnimation() const { return animator->getAnim("Score"); }
	std::shared_ptr<Animator> getAnimator() { return animator; }
};