#pragma once

#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include <iostream>

#include "Gameobject.h"
#include "Block.h"

class Brick : public Block
{
private:

public:
	Brick(const sf::Sprite& sprite, sf::Texture* texture, const sf::FloatRect& rect, const std::string& type, int layer);

	void onHit() override;
};
