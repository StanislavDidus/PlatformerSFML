#include "Brick.h"

Brick::Brick(const sf::Sprite& sprite, sf::Texture* texture, sf::Texture* texture1, const sf::FloatRect& rect, const std::string& type, int layer, std::shared_ptr<CollisionManager> col, std::vector<std::shared_ptr<GameObject>>& gameObjects) : Block(rect, type, texture, layer, col, gameObjects), broken_bricks_texture(*texture1)
{
	//initSprite();
	//animator->addPosAnimation(
		//sprite, 150.f, [this]() {return false; }, false, 25, { {sprite.getPosition()},{sprite.getPosition().x, sprite.getPosition().y - 15.f} }, "Hit");
	
}

void Brick::initSprites()
{
	for (int i = 0; i < 4; i++)
	{
		broken_bricks_sprites.emplace_back();
		sf::Sprite& sprite = broken_bricks_sprites.back();
		sprite.setTexture(broken_bricks_texture);
		sprite.setScale(3.125f, 3.125f);
		sprite.setTextureRect(sf::IntRect(0, 0, 8, 8));

		
	}
	broken_bricks_sprites[0].setPosition(getPosition().x, getPosition().y); // LEFT-UP
	broken_bricks_sprites[1].setPosition(getPosition().x + (getBounds().width / 2), getPosition().y); // RIGHT-UP
	broken_bricks_sprites[2].setPosition(getPosition().x, getPosition().y + (getBounds().height / 2)); // LEFT-DOWN
	broken_bricks_sprites[3].setPosition(getPosition().x + (getBounds().width / 2), getPosition().y + (getBounds().height / 2)); // RIGHT-DOWN
}

void Brick::initAnimator()
{
	for (int i = 0; i < 4; i++)
	{
		animators_bb.push_back(std::make_unique<Animator>());
		animators_bb[i]->addFrameAnimation(
			broken_bricks_sprites[i], 8, 8, std::vector<int>{0, 1, 2, 3}, .4f, [this]() {return true;  }, [this]() {return 1; }, true, 10, "Idle"
		);
	}
}

void Brick::onHit()
{
	animator->playAnim("Hit");
	HitItem();
}

void Brick::onHitBig()
{
	if (!is_broken)
	{
		HitItem();
		Destroy();
	}
}

void Brick::update(float deltaTime)
{
	animator->update(deltaTime);


	if(animators_bb.size() > 0)
		for (auto& animator : animators_bb)
		{
			animator->update(deltaTime);
		}

	//Move broken pieces
	if (is_broken)
	{
		broken_bricks_sprites[0].move(velocity.x * deltaTime * -1, velocity.y * deltaTime);
		broken_bricks_sprites[2].move(velocity.x * deltaTime * -1, velocity.y * deltaTime);
		broken_bricks_sprites[1].move(velocity.x * deltaTime * 1, velocity.y * deltaTime);
		broken_bricks_sprites[3].move(velocity.x * deltaTime * 1, velocity.y * deltaTime);

		velocity += {125.f * deltaTime, 1000.f * deltaTime};
		if (velocity.x < 0)
			velocity.x = 0;

		if (broken_bricks_sprites[0].getPosition().y > 600)
		{
			destroy();
		}
	}

	sf::Vector2f newPos = sprite.getPosition();
	setPosition(newPos);
}

void Brick::render(sf::RenderTarget* target)
{
	if (!is_broken)
		target->draw(sprite);
	else
	{
		for (int i = 0; i < 4; i++)
		{
			target->draw(broken_bricks_sprites[i]);
		}
	}
}

void Brick::Destroy()
{
	//Destroy
	initSprites();
	initAnimator();

	is_broken = true;
	is_collision = false;

	
}


