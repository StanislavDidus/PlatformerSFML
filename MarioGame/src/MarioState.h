#pragma once

#include "Mario.h"

class IMarioState
{
public:
	virtual ~IMarioState() = default;

	virtual void onEnter(Mario& mario) = 0;
	virtual void onUpdate(Mario& mario, float deltaTime) = 0;
	virtual void onExit(Mario& mario) = 0;
};

class IMarioIdle : public IMarioState
{
	void onEnter(Mario& mario) override
	{
		
	}

	void onUpdate(Mario& mario, float deltaTime) override
	{
		//Inputs
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		{
			mario.setState(std::static_pointer_cast<IMarioState>(std::make_shared<IMarioWalk>()));
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && mario.is_ground)
		{
			mario.setState(std::static_pointer_cast<IMarioState>(std::make_shared<IMarioJump>()));
		}

		
		float deltaX = mario.velocity.x;
		float deltaY = mario.velocity.y * deltaTime;
		mario.col->callibrateCollision(mario.getBounds(), deltaX, deltaY);
		mario.sprite.move(deltaX, deltaY);
		mario.is_ground = mario.col->checkCollision({ mario.getBounds().left,
			mario.getBounds().top + mario.velocity.y * deltaTime,
			mario.getBounds().width,
			mario.getBounds().height }, mario.velocity, "All", CollisionType::DOWN);

		mario.velocity.x *= 0.9f;
		mario.applyGravity(deltaTime);
		//Update idle anim
		//mario.sprite.setTextureRect(sf::IntRect(mario.dir == 1 ? 0 : 16, 0, 16 * mario.dir, 16));
	}

	void onExit(Mario& mario) override
	{

	}
};

class IMarioWalk : public IMarioState
{
	void onEnter(Mario& mario) override
	{

	}

	void onUpdate(Mario& mario, float deltaTime) override
	{
		//Inputs
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && mario.is_ground)
		{
			mario.setState(std::static_pointer_cast<IMarioState>(std::make_shared<IMarioJump>()));
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		{
			mario.move(-1.f, 0.f);
			mario.flip(-1.f);
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		{
			mario.move(1.f, 0.f);
			mario.flip(1.f);
		}
		else // If no button is pressed
		{
			mario.setState(std::static_pointer_cast<IMarioState>(std::make_shared<IMarioIdle>()));
		}
		
		float deltaX = mario.velocity.x;
		float deltaY = mario.velocity.y * deltaTime;
		mario.col->callibrateCollision(mario.sprite.getGlobalBounds(), deltaX, deltaY);
		mario.sprite.move(deltaX, deltaY);
		mario.is_ground = mario.col->checkCollision({mario.getBounds().left,
			mario.getBounds().top + mario.velocity.y * deltaTime,
			mario.getBounds().width,
			mario.getBounds().height}, mario.velocity, "All", CollisionType::DOWN);

		mario.velocity.x *= 0.9f;

		mario.applyGravity(deltaTime);
	}

	void onExit(Mario& mario) override
	{

	}
};

class IMarioJump : public IMarioState
{
	void onEnter(Mario& mario) override
	{
		mario.jump_start_pos = mario.getPosition().y;

		mario.is_jump_over = false;

		mario.velocity.y = -150.f;

		mario.mario_audio_manager->playSound("Jump");
	}
	
	void onUpdate(Mario& mario, float deltaTime) override
	{
		//Inputs
		//Move mario
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		{
			mario.move(-1.f, 0.f);
			mario.flip(-1.f);
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		{
			mario.move(1.f, 0.f);
			mario.flip(1.f);
		}

		//Apply gravity and move 
		float deltaX = mario.velocity.x;
		float deltaY = mario.velocity.y * deltaTime;;
		mario.col->callibrateCollision(mario.sprite.getGlobalBounds(), deltaX, deltaY);
		mario.sprite.move(deltaX, deltaY);
		mario.is_ground = mario.col->checkCollision({ mario.getBounds().left,
			mario.getBounds().top + mario.velocity.y * deltaTime,
			mario.getBounds().width,
			mario.getBounds().height }, mario.velocity, "All", CollisionType::DOWN);

		mario.velocity.x *= 0.9f;
		mario.applyGravity(deltaTime);

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && mario.getPosition().y > mario.jump_start_pos - mario.jump_start_max && !mario.is_jump_over)
		{
			mario.velocity.y -= 5000.f * deltaTime;

			if (std::abs(mario.velocity.y) > mario.max_velocity.y)
				mario.velocity.y = mario.max_velocity.y * -1.f;
		}

		if (mario.getPosition().y < mario.jump_start_pos - mario.jump_start_max)
		{
			mario.is_jump_over = true;
		}

		if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
		{
			mario.is_jump_over = true;
		}

		if (mario.col->checkCollision({ mario.getBounds().left,
			mario.getBounds().top + mario.velocity.y * deltaTime,
			mario.getBounds().width,
			mario.getBounds().height }, mario.velocity, "All", CollisionType::TOP) && !mario.is_jump_over)
		{
			mario.is_jump_over = true;
			mario.velocity.y += 400.f;
		}
		
		if (mario.is_ground)
		{
			mario.setState(std::static_pointer_cast<IMarioState>(std::make_shared<IMarioIdle>()));
			
		}
	}

	void onExit(Mario& mario) override
	{
		mario.is_jump_over = false;
	}
};
