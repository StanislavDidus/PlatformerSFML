#include "LuckyBlock.h"

LuckyBlock::LuckyBlock(const sf::Sprite& sprite, const sf::FloatRect& rect, const std::string& type) : Block(rect, type, "assets/Textures/Levels/LuckyBlock.png")
{
	
}

void LuckyBlock::onHit()
{
	//When lucky block is hit

	
	this->animator->playAnim("Hit");
}



