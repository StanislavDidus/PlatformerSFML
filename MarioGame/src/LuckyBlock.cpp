#include "LuckyBlock.h"

void LuckyBlock::initSprite()
{
	this->texture.loadFromFile("assets/Textures/Levels/LuckyBlock.png");
	this->sprite.setTexture(this->texture);
	this->sprite.setTextureRect({0,0,16,16});
}

void LuckyBlock::initAnimations()
{
	this->animator = std::make_unique<Animator>(
		this->sprite,
		static_cast<int>(this->sprite.getLocalBounds().width),
		static_cast<int>(this->sprite.getLocalBounds().height)
	);

	//Add animations
	this->animator->addFrameAnimation(
		0, 2, 400.f / 1000.f, [this]() {return true; }, [this]() {return 1;  }, true, 5, "Idle");

	this->animator->addPosAnimation(
		150.f, [this]() {return false; }, false, 10, {{this->sprite.getPosition()},{this->sprite.getPosition().x, this->sprite.getPosition().y - 15.f}}, "Hit");
}

LuckyBlock::LuckyBlock(const sf::Sprite& sprite, const sf::FloatRect& rect, const std::string& type) : GameObject(type, rect) , sprite(sprite)
{
	this->initSprite();
	this->initAnimations();
}

void LuckyBlock::onHit()
{
	this->animator->playAnim("Hit");
}

void LuckyBlock::setPosition(const sf::Vector2f& newPosition)
{
}

void LuckyBlock::update(float deltaTime)
{
	this->animator->update(deltaTime);

	sf::Vector2f newPos = this->sprite.getPosition();
	setPosition(newPos);

	//if (sf::Keyboard::isKeyPressed(sf::Keyboard::G))
		//this->animator->playAnim("Hit");
}

void LuckyBlock::render(sf::RenderTarget* target)
{
	target->draw(this->sprite);
}
