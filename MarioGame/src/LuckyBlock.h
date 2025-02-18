#pragma once

#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include <iostream>

#include "Gameobject.h"
#include "Block.h"
#include "Animator.h"

class LuckyBlock : public Block
{
private:
	
public:
	LuckyBlock(const sf::Sprite& sprite, const sf::FloatRect& rect, const std::string& type);

	void onHit() override;
};
