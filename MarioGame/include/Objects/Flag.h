#pragma once

#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include "GameObject.h"

#include <iostream>
#include <vector>

class Flag : public GameObject
{
private:
	sf::Texture* stick_tex;
	sf::Texture* flag_tex;

	sf::Sprite stick_sprite;
	sf::Sprite flag_sprite;

	
public:
	bool is_finished;

	Flag(const std::string& type, sf::FloatRect bounds, int layer, sf::Texture* stick_tex, sf::Texture* flag_tex);
	virtual ~Flag();

	void initSprites();

	void Touch(float deltaTime);

	void update(float deltaTime);
	void render(sf::RenderTarget* target);
};

