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
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && mario.is_ground)
		{
			mario.setState(std::static_pointer_cast<IMarioState>(std::make_shared<IMarioJump>()));
		}
		else 
		{
			if (mario.velocity.x > 0.f)
			{
				mario.velocity.x -= 1000.f * deltaTime;
			}
			else if (mario.velocity.x < 0.f)
			{
				mario.velocity.x += 1000.f * deltaTime;
			}

			if (std::abs(mario.velocity.x) < 10.f)
			{	
				mario.velocity.x = 0.f;
			}
		}

		//Callibrate max jump height
		mario.jump_start_max = 0.3125f * abs(mario.velocity.x) + 25.f;

		
		float deltaX = mario.velocity.x * deltaTime;
		float deltaY = mario.velocity.y * deltaTime;
		mario.col->callibrateCollision(mario, deltaX, deltaY);
		mario.sprite.move(deltaX, deltaY);
		mario.is_ground = mario.col->checkCollision({ mario.sprite.getGlobalBounds().left + mario.velocity.x * deltaTime,
			mario.sprite.getGlobalBounds().top + mario.velocity.y * deltaTime,
			mario.sprite.getGlobalBounds().width,
			mario.sprite.getGlobalBounds().height }, mario.velocity, "All", CollisionType::DOWN);

		//mario.velocity.x *= 0.9f * mario.deltaTime;
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

		//Callibrate max jump height
		mario.jump_start_max = 0.3125f * abs(mario.velocity.x) + 25.f;
		
		float deltaX = mario.velocity.x * deltaTime;
		float deltaY = mario.velocity.y * deltaTime;
		//mario.setPosition(mario.getPosition());
		mario.col->callibrateCollision(mario, deltaX, deltaY);
		//std::cout << deltaX << ", " << deltaY << "\n"
		mario.sprite.move(deltaX, deltaY);
		mario.is_ground = mario.col->checkCollision({ mario.sprite.getGlobalBounds().left + mario.velocity.x * deltaTime,
			mario.sprite.getGlobalBounds().top + mario.velocity.y * deltaTime,
			mario.sprite.getGlobalBounds().width,
			mario.sprite.getGlobalBounds().height}, mario.velocity, "All", CollisionType::DOWN);

		//std::cout << mario.is_ground << "\n";

		//mario.velocity.x *= 0.9f * mario.deltaTime;

		mario.applyGravity(deltaTime);

		//Check slide
		mario.checkSlide();
	}

	void onExit(Mario& mario) override
	{
		mario.is_sliding = false;
		//--mario.jump_start_max = 25.f;
	}
};

class IMarioJump : public IMarioState
{
	void onEnter(Mario& mario) override
	{
		mario.jump_start_pos = mario.getPosition().y;

		mario.is_jump_over = false;

		mario.velocity.y = -50.f;

		mario.mario_audio_manager->playSound("Jump");
	}
	
	void onUpdate(Mario& mario, float deltaTime) override
	{
		//Inputs
		//Move mario
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		{
			mario.move(-1.f, 0.f);
			//mario.flip(-1.f);
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		{
			mario.move(1.f, 0.f);
			//mario.flip(1.f);
		}
		else
		{
			if (mario.velocity.x > 0.f)
			{
				mario.velocity.x -= 325.f * deltaTime;
			}
			else if (mario.velocity.x < 0.f)
			{
				mario.velocity.x += 325.f * deltaTime;
			}

			if (std::abs(mario.velocity.x) < 10.f)
			{
				mario.velocity.x = 0.f;
			}
		}

		//Callibrate max jump height
		//mario.jump_start_max = 0.3125f * abs(mario.velocity.x) + 25.f;

		//Callibrate max jump height
		mario.jump_start_max = 0.3125f * abs(mario.velocity.x) + 25.f;

		//Apply gravity and move 
		float deltaX = mario.velocity.x * deltaTime;
		float deltaY = mario.velocity.y * deltaTime;;
		mario.col->callibrateCollision(mario, deltaX, deltaY);
		mario.sprite.move(deltaX, deltaY);
		mario.is_ground = mario.col->checkCollision({ mario.sprite.getGlobalBounds().left,
			mario.sprite.getGlobalBounds().top + mario.velocity.y * deltaTime,
			mario.sprite.getGlobalBounds().width,
			mario.sprite.getGlobalBounds().height }, mario.velocity, "All", CollisionType::DOWN);

		mario.applyGravity(deltaTime);

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && mario.getPosition().y > mario.jump_start_pos - mario.jump_start_max && !mario.is_jump_over)
		{
			mario.velocity.y -= 10000.f * deltaTime;

			if (std::abs(mario.velocity.y) > mario.max_velocity.y)
				mario.velocity.y = mario.max_velocity.y * -1.f;
		}

		if (mario.getPosition().y < mario.jump_start_pos - mario.jump_start_max && mario.velocity.y >= 0.f)
		{
			mario.is_jump_over = true;
		}

		if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
		{
			mario.is_jump_over = true;
		}

		// Check lucky block collision
		// if hit lucky block
		if (!mario.is_jump_over)
		{
			GameObject* obj = mario.col->getObject({ mario.getBounds().left,
			mario.getBounds().top + mario.velocity.y * deltaTime,
			mario.getBounds().width,
			mario.getBounds().height }, mario.velocity, "Block");

			if (obj != nullptr)
			{
				obj->onHit();
			}
		}
		

		//Then check collision with roof
		if (mario.col->checkCollision({ mario.getBounds().left,
			mario.getBounds().top + mario.velocity.y * mario.deltaTime,
			mario.getBounds().width,
			mario.getBounds().height }, mario.velocity, "All", CollisionType::TOP) && !mario.is_jump_over)
		{
			mario.is_jump_over = true;
			mario.velocity.y += 350.f;
			//std::cout << "ROOF\n";
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

class IMarioCinematic: public IMarioState
{
	void onEnter(Mario& mario) override
	{
		mario.velocity = { 0,0 };
		mario.sprite.setTextureRect(sf::IntRect(0,0,16,16));
	}

	void onUpdate(Mario& mario, float deltaTime) override
	{
		float deltaX = mario.velocity.x * deltaTime;
		float deltaY = mario.velocity.y * deltaTime;
		//mario.col->callibrateCollision(mario, deltaX, deltaY);
		mario.sprite.move(deltaX, deltaY);

		//mario.applyGravity(deltaTime);

		std::cout << "Cinematic\n";
	}

	void onExit(Mario& mario) override
	{

	}
};