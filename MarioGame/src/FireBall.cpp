#include "Objects/FireBall.h"

void FireBall::initSprite()
{
	texture = *texture_manager->get("FireBall").get();
	sprite.setTexture(texture);
	sprite.setPosition(getPosition());
	sprite.setScale(3.125f, 3.125f);
	sprite.setTextureRect(sf::IntRect(0, 0, 8, 8));

	explosion_texture = *texture_manager->get("Explosion").get();
	explosion_sprite.setTexture(explosion_texture);
	explosion_sprite.setScale(3.125f, 3.125f);
	explosion_sprite.setTextureRect(sf::IntRect(0,0,16,16));

	velocity = { 550.f * dir, 550.f };
}

void FireBall::initAnimator()
{
	animator = std::make_shared<Animator>();

	animator->addFrameAnimation(sprite, 8, 8, std::vector<int>{0, 1, 2, 3}, 0.1f, []() {return true; }, []() {return 1; }, true, 5, "Idle");
	animator->addFrameAnimation(explosion_sprite, 16, 16, std::vector<int>{0, 1, 2}, 0.075f, []() {return false; }, []() {return 1; }, false, 5, "Explosion");
}

FireBall::FireBall(const std::string& type, sf::FloatRect bounds, int layer, std::shared_ptr<TextureManager> texture_manager, std::shared_ptr<CollisionManager> collision_manager, int dir) :
	GameObject(type, bounds, layer), texture_manager(texture_manager), collision_manager(collision_manager), dir(dir)
{
	initSprite();
	initAnimator();
}

FireBall::~FireBall()
{
}

void FireBall::setPosition(sf::Vector2f pos)
{
	sprite.setPosition(pos);
}

void FireBall::update(float deltaTime)
{
	if (is_active)
	{
		

		velocity.y += gravity * deltaTime;
		if (velocity.y > 400.f) velocity.y = 400.f;

		updateCollisions(deltaTime);

		float deltaX = velocity.x * deltaTime;
		float deltaY = velocity.y * deltaTime;
		collision_manager->callibrateCollision(*this, deltaX, deltaY);
		sprite.move(deltaX, deltaY);
	}

	animator->update(deltaTime);

	if (animator->getAnim("Explosion")->is_finished && !is_active)
		is_destroyed = true;

	GameObject::setPosition(sprite.getPosition());
}

void FireBall::updateCollisions(float deltaTime)
{
	//Check Bottom collision
	if (collision_manager->checkCollision(sf::FloatRect(sprite.getGlobalBounds().left,
		sprite.getGlobalBounds().top + velocity.y * deltaTime,
		sprite.getGlobalBounds().width,
		sprite.getGlobalBounds().height), velocity, "All", CollisionType::DOWN))
	{
		velocity.y = -300.f;
	}
	//Check Left and Right collisions
	if (dir == -1 && collision_manager->checkCollision(sf::FloatRect(sprite.getGlobalBounds().left + velocity.x * deltaTime,
		sprite.getGlobalBounds().top,
		sprite.getGlobalBounds().width,
		sprite.getGlobalBounds().height), velocity, "All", CollisionType::LEFT))
	{
		explosion_sprite.setPosition(sprite.getPosition());
		animator->playAnim("Explosion");
		is_active = false;
	}
	else if (dir==1 && collision_manager->checkCollision(sf::FloatRect(sprite.getGlobalBounds().left + velocity.x * deltaTime,
		sprite.getGlobalBounds().top,
		sprite.getGlobalBounds().width,
		sprite.getGlobalBounds().height), velocity, "All", CollisionType::RIGHT))
	{
		explosion_sprite.setPosition(sprite.getPosition());
		animator->playAnim("Explosion");
		is_active = false;
	}
}

void FireBall::render(sf::RenderTarget* target)
{
	if (!is_active)
		target->draw(explosion_sprite);
	else
		target->draw(sprite);
}
