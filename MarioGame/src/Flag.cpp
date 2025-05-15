#include "Objects/Flag.h"

Flag::Flag(const std::string& type, sf::FloatRect bounds, int layer, sf::Texture* stick_tex, sf::Texture* flag_tex) : GameObject(type, bounds, layer),
stick_tex(stick_tex), flag_tex(flag_tex), is_finished(false)
{
	initSprites();
}

Flag::~Flag()
{

}

void Flag::initSprites()
{
	stick_sprite.setTexture(*stick_tex);
	stick_sprite.setScale(3.125f, 3.125f);
	stick_sprite.setPosition(getPosition().x, getPosition().y);
	flag_sprite.setTexture(*flag_tex);
	flag_sprite.setScale(3.125f, 3.125f);
	flag_sprite.setPosition(getPosition().x - 16.f * 3.125f, getPosition().y + 16.f * 3.125f);
}

void Flag::Touch(float deltaTime)
{
	if (flag_sprite.getPosition().y < stick_sprite.getGlobalBounds().height - flag_sprite.getGlobalBounds().height + 50.f)
	{
		flag_sprite.move(0.f, 200.f * deltaTime);
	}
	else
	{
		is_finished = true;
	}
}

void Flag::update(float deltaTime)
{
	GameObject::setPosition(stick_sprite.getPosition());
}

void Flag::render(sf::RenderTarget* target)
{
	target->draw(stick_sprite);
	target->draw(flag_sprite);
}
