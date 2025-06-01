#pragma once

#include "Objects/Mario.h"

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
		bool isEPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::E);
		if (isEPressed && !mario.wasShootPressedLastFrame && mario.fireBalls.size() < 2 && mario.current_mario_state == MarioState::FIRE && mario.shoot_time == 0.f)
		{
			mario.last_state = mario.current_state;
			mario.setState(std::static_pointer_cast<IMarioState>(std::make_shared<IMarioShoot>()));
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		{
			mario.setState(std::static_pointer_cast<IMarioState>(std::make_shared<IMarioWalk>()));
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && mario.is_ground)
		{
			mario.setState(std::static_pointer_cast<IMarioState>(std::make_shared<IMarioJump>()));
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) && (mario.current_mario_state == MarioState::BIG || mario.current_mario_state == MarioState::FIRE))
		{
			mario.setState(std::static_pointer_cast<IMarioState>(std::make_shared<IMarioCrouch>()));
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
		mario.wasShootPressedLastFrame = isEPressed;

		//Callibrate max jump height
		mario.jump_start_max = 0.3125f * abs(mario.velocity.x) + 25.f;

		
		float deltaX = mario.velocity.x * deltaTime;
		float deltaY = mario.velocity.y * deltaTime;
		mario.col->callibrateCollision(mario, deltaX, deltaY);
		mario.sprite.move(deltaX, deltaY);
		mario.applyGravity(deltaTime);


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
		bool isEPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::E);
		if (isEPressed && !mario.wasShootPressedLastFrame && mario.fireBalls.size() < 2 && mario.current_mario_state == MarioState::FIRE && mario.shoot_time == 0.f)
		{
			mario.last_state = mario.current_state;
			mario.setState(std::static_pointer_cast<IMarioState>(std::make_shared<IMarioShoot>()));
		}
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
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) && (mario.current_mario_state == MarioState::BIG || mario.current_mario_state == MarioState::FIRE))
		{
			mario.setState(std::static_pointer_cast<IMarioState>(std::make_shared<IMarioCrouch>()));
		}
		else // If no button is pressed
		{
			mario.setState(std::static_pointer_cast<IMarioState>(std::make_shared<IMarioIdle>()));
		}
		mario.wasShootPressedLastFrame = isEPressed;

		//Callibrate max jump height
		mario.jump_start_max = 0.3125f * abs(mario.velocity.x) + 25.f;
		
		float deltaX = mario.velocity.x * deltaTime;
		float deltaY = mario.velocity.y * deltaTime;
		mario.col->callibrateCollision(mario, deltaX, deltaY);
		mario.sprite.move(deltaX, deltaY);

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

		if (mario.current_mario_state == MarioState::SMALL)
		{
			mario.mario_audio_manager->playSound("Jump_Small");
		}
		else if (mario.current_mario_state == MarioState::BIG || mario.current_mario_state == MarioState::FIRE)
		{
			mario.mario_audio_manager->playSound("Jump_Super");
		}
	}
	
	void onUpdate(Mario& mario, float deltaTime) override
	{
		//Inputs
		//Move mario
		bool isEPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::E);
		if (isEPressed && !mario.wasShootPressedLastFrame && mario.fireBalls.size() < 2 && mario.current_mario_state == MarioState::FIRE && mario.shoot_time == 0.f)
		{
			mario.last_state = mario.current_state;
			mario.setState(std::static_pointer_cast<IMarioState>(std::make_shared<IMarioShoot>()));
		}
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
		mario.wasShootPressedLastFrame = isEPressed;

		//Callibrate max jump height
		mario.jump_start_max = 0.3125f * abs(mario.velocity.x) + 25.f;

		//Apply gravity and move 
		float deltaX = mario.velocity.x * deltaTime;
		float deltaY = mario.velocity.y * deltaTime;;
		mario.col->callibrateCollision(mario, deltaX, deltaY);
		mario.sprite.move(deltaX, deltaY);

		mario.applyGravity(deltaTime);

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && mario.getPosition().y > mario.jump_start_pos - mario.jump_start_max && !mario.is_jump_over)
		{
			mario.velocity.y -= 10000.f * deltaTime;

			if (std::abs(mario.velocity.y) > mario.max_velocity.y)
				mario.velocity.y = mario.max_velocity.y * -1.f;
		}

	if ((mario.getPosition().y < mario.jump_start_pos - mario.jump_start_max && mario.velocity.y >= 0.f) ||
		(mario.getBounds().top > mario.sprite.getPosition().y))
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
				if (mario.current_mario_state == MarioState::SMALL)
				{
					obj->onHit();
				}
				else if (mario.current_mario_state == MarioState::BIG || mario.current_mario_state == MarioState::FIRE)
				{
					obj->onHitBig();
				}

				mario.is_jump_over = true;
				mario.velocity.y += 350.f;
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

	}
};

class IMarioCrouch : public IMarioState
{
	void onEnter(Mario& mario) override
	{
		
	}

	void onUpdate(Mario& mario, float deltaTime) override
	{
		if (!sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		{
			mario.setState(std::static_pointer_cast<IMarioState>(std::make_shared<IMarioIdle>()));
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
		
		mario.jump_start_max = 0.3125f * abs(mario.velocity.x) + 25.f;

		float deltaX = mario.velocity.x * deltaTime;
		float deltaY = mario.velocity.y * deltaTime;
		mario.col->callibrateCollision(mario, deltaX, deltaY);
		mario.sprite.move(deltaX, deltaY);

		mario.applyGravity(deltaTime);
	}

	void onExit(Mario& mario) override
	{

	}
};

class IMarioShoot : public IMarioState
{
	void onEnter(Mario& mario)
	{
		mario.is_jump_over = true;
		mario.fireBalls.push_back(std::make_shared<FireBall>("FireBall", sf::FloatRect(mario.sprite.getPosition().x + 8 * mario.direction, mario.sprite.getPosition().y + 8, 8, 8), 20, mario.texture_manager, mario.col, mario.direction));
		mario.mario_audio_manager->playSound("FireBall");
	}

	void onUpdate(Mario& mario, float deltaTime)
	{
		
		mario.jump_start_max = 0.3125f * abs(mario.velocity.x) + 25.f;

		//Apply gravity and move 
		float deltaX = mario.velocity.x * deltaTime;
		float deltaY = mario.velocity.y * deltaTime;;
		mario.col->callibrateCollision(mario, deltaX, deltaY);
		mario.sprite.move(deltaX, deltaY);

		mario.applyGravity(deltaTime);
		
		if (mario.shoot_time < mario.shoot_timer)
		{
			mario.shoot_time += deltaTime;
		}
		else
		{
			mario.shoot_time = 0.f;
			mario.setLastState();
		}
		

	}

	void onExit(Mario& mario)
	{

	}
};

class IMarioDie : public IMarioState
{
	void onEnter(Mario& mario)
	{
		mario.sprite.setTexture(mario.texture);
		//mario.sprite.setTextureRect(sf::IntRect(0,0,16,16));
		mario.velocity.y = -500.f;
		mario.current_mario_state == MarioState::SMALL;
	}

	void onUpdate(Mario& mario, float deltaTime)
	{

		mario.jump_start_max = 0.3125f * abs(mario.velocity.x) + 25.f;

		//Apply gravity and move 
		mario.sprite.move(0.f, mario.velocity.y * deltaTime);
		mario.applyGravity(deltaTime);
	}

	void onExit(Mario& mario)
	{

	}
};

class IMarioCollectFlag: public IMarioState
{
	void onEnter(Mario& mario) override
	{
		if (mario.current_mario_state == MarioState::SMALL)
			mario.sprite.setTextureRect(sf::IntRect(112, 0, 16, 16));
		else if (mario.current_mario_state == MarioState::BIG)
			mario.sprite.setTextureRect(sf::IntRect(0, 64, 16, 32));
		else if(mario.current_mario_state == MarioState::FIRE)
			mario.sprite.setTextureRect(sf::IntRect(0, 128, 16, 32));
		mario.sprite.move(20.f, 0.f);

		mario.sprite.setPosition(MathUtils::clamp(mario.sprite.getPosition().x, 9900.f - 48.f, 9900.f), MathUtils::clamp(mario.sprite.getPosition().y, 50.f, 550.f - 48.f));
	}

	void onUpdate(Mario& mario, float deltaTime) override
	{
		if (mario.is_touching_flag && mario.sprite.getPosition().y < 550 - mario.sprite.getGlobalBounds().height)
		{
			mario.sprite.move(0.f, 200.f * deltaTime);
		}
	}

	void onExit(Mario& mario) override
	{

	}
};

class IMarioWaitToRun: public IMarioState
{
	void onEnter(Mario& mario) override
	{
		mario.sprite.move(mario.sprite.getGlobalBounds().width, 0.f);
		if (mario.current_mario_state == MarioState::SMALL)
			mario.sprite.setTextureRect(sf::IntRect(128, 0, -16, 16));
		else if (mario.current_mario_state == MarioState::BIG)
			mario.sprite.setTextureRect(sf::IntRect(16, 64, -16, 32));
		else if (mario.current_mario_state == MarioState::FIRE)
			mario.sprite.setTextureRect(sf::IntRect(16, 128, -16, 32));
		mario.tclock.addClock(1.f, [this, &mario]() {mario.setState(std::static_pointer_cast<IMarioState>(std::make_shared<IMarioRunToCastle>())); }, "RunToCastle");
	}

	void onUpdate(Mario& mario, float deltaTime) override
	{
		
	}

	void onExit(Mario& mario) override
	{

	}
};

class IMarioRunToCastle : public IMarioState
{
	void onEnter(Mario& mario) override
	{
		
		//mario.is_finishing = true;
	}

	void onUpdate(Mario& mario, float deltaTime) override
	{
		if (mario.current_mario_state == MarioState::SMALL)
			mario.animator->playAnim("Run");
		else if (mario.current_mario_state == MarioState::BIG)
			mario.animator->playAnim("BRun");
		else if(mario.current_mario_state == MarioState::FIRE)
			mario.animator->playAnim("FRun");
		//velocity.x = 100.f;
		if (mario.sprite.getPosition().y < 600 - mario.sprite.getGlobalBounds().height)
			mario.velocity.y = 70.f;
		else
			mario.velocity.y = 0;

		if (mario.sprite.getPosition().x > 10200)
		{
			mario.need_quit = true;
		}

		float deltaX = 175.f * deltaTime;
		float deltaY = mario.velocity.y * deltaTime;
		mario.sprite.move(deltaX, deltaY);
	}

	void onExit(Mario& mario) override
	{

	}
};