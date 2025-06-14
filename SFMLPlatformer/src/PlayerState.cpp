#pragma once

#include "Objects/Player.h"

class IPlayerState
{
public:
	virtual ~IPlayerState() = default;

	virtual void onEnter(Player& Player) = 0;
	virtual void onUpdate(Player& Player, float deltaTime) = 0;
	virtual void onExit(Player& Player) = 0;
};

class IPlayerIdle : public IPlayerState
{
	void onEnter(Player& Player) override
	{
		
	}

	void onUpdate(Player& Player, float deltaTime) override
	{
		//Inputs
		bool isEPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::E);
		if (isEPressed && !Player.wasShootPressedLastFrame && Player.fireBalls.size() < 2 && Player.current_Player_state == PlayerState::FIRE && Player.shoot_time == 0.f)
		{
			Player.last_state = Player.current_state;
			Player.setState(std::static_pointer_cast<IPlayerState>(std::make_shared<IPlayerShoot>()));
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		{
			Player.setState(std::static_pointer_cast<IPlayerState>(std::make_shared<IPlayerWalk>()));
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && Player.is_ground)
		{
			Player.setState(std::static_pointer_cast<IPlayerState>(std::make_shared<IPlayerJump>()));
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) && (Player.current_Player_state == PlayerState::BIG || Player.current_Player_state == PlayerState::FIRE))
		{
			Player.setState(std::static_pointer_cast<IPlayerState>(std::make_shared<IPlayerCrouch>()));
		}
		else 
		{
			if (Player.velocity.x > 0.f)
			{
				Player.velocity.x -= 1000.f * deltaTime;
			}
			else if (Player.velocity.x < 0.f)
			{
				Player.velocity.x += 1000.f * deltaTime;
			}

			if (std::abs(Player.velocity.x) < 10.f)
			{	
				Player.velocity.x = 0.f;
			}
		}
		Player.wasShootPressedLastFrame = isEPressed;

		//Callibrate max jump height
		Player.jump_start_max = 0.3125f * abs(Player.velocity.x) + 25.f;

		
		float deltaX = Player.velocity.x * deltaTime;
		float deltaY = Player.velocity.y * deltaTime;
		Player.col->callibrateCollision(Player, deltaX, deltaY);
		Player.sprite.move(deltaX, deltaY);
		Player.applyGravity(deltaTime);


	}

	void onExit(Player& Player) override
	{

	}
};

class IPlayerWalk : public IPlayerState
{
	void onEnter(Player& Player) override
	{

	}

	void onUpdate(Player& Player, float deltaTime) override
	{
		//Inputs
		bool isEPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::E);
		if (isEPressed && !Player.wasShootPressedLastFrame && Player.fireBalls.size() < 2 && Player.current_Player_state == PlayerState::FIRE && Player.shoot_time == 0.f)
		{
			Player.last_state = Player.current_state;
			Player.setState(std::static_pointer_cast<IPlayerState>(std::make_shared<IPlayerShoot>()));
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && Player.is_ground)
		{
			Player.setState(std::static_pointer_cast<IPlayerState>(std::make_shared<IPlayerJump>()));
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		{
			Player.move(-1.f, 0.f);
			Player.flip(-1);
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		{
			Player.move(1.f, 0.f);
			Player.flip(1);
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) && (Player.current_Player_state == PlayerState::BIG || Player.current_Player_state == PlayerState::FIRE))
		{
			Player.setState(std::static_pointer_cast<IPlayerState>(std::make_shared<IPlayerCrouch>()));
		}
		else // If no button is pressed
		{
			Player.setState(std::static_pointer_cast<IPlayerState>(std::make_shared<IPlayerIdle>()));
		}
		Player.wasShootPressedLastFrame = isEPressed;

		//Callibrate max jump height
		Player.jump_start_max = 0.3125f * abs(Player.velocity.x) + 25.f;
		
		float deltaX = Player.velocity.x * deltaTime;
		float deltaY = Player.velocity.y * deltaTime;
		Player.col->callibrateCollision(Player, deltaX, deltaY);
		Player.sprite.move(deltaX, deltaY);

		Player.applyGravity(deltaTime);

		//Check slide
		Player.checkSlide();
	}

	void onExit(Player& Player) override
	{
		Player.is_sliding = false;
		//--Player.jump_start_max = 25.f;
	}
};

class IPlayerJump : public IPlayerState
{
	void onEnter(Player& Player) override
	{
		Player.jump_start_pos = Player.getPosition().y;

		Player.is_jump_over = false;

		Player.velocity.y = -50.f;

		if (Player.current_Player_state == PlayerState::SMALL)
		{
			Player.Player_audio_manager->playSound("Jump_Small");
		}
		else if (Player.current_Player_state == PlayerState::BIG || Player.current_Player_state == PlayerState::FIRE)
		{
			Player.Player_audio_manager->playSound("Jump_Super");
		}
	}
	
	void onUpdate(Player& Player, float deltaTime) override
	{
		//Inputs
		//Move Player
		bool isEPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::E);
		if (isEPressed && !Player.wasShootPressedLastFrame && Player.fireBalls.size() < 2 && Player.current_Player_state == PlayerState::FIRE && Player.shoot_time == 0.f)
		{
			Player.last_state = Player.current_state;
			Player.setState(std::static_pointer_cast<IPlayerState>(std::make_shared<IPlayerShoot>()));
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		{
			Player.move(-1.f, 0.f);
			//Player.flip(-1.f);
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		{
			Player.move(1.f, 0.f);
			//Player.flip(1.f);
		}
		else
		{
			if (Player.velocity.x > 0.f)
			{
				Player.velocity.x -= 325.f * deltaTime;
			}
			else if (Player.velocity.x < 0.f)
			{
				Player.velocity.x += 325.f * deltaTime;
			}

			if (std::abs(Player.velocity.x) < 10.f)
			{
				Player.velocity.x = 0.f;
			}
		}
		Player.wasShootPressedLastFrame = isEPressed;

		//Callibrate max jump height
		Player.jump_start_max = 0.3125f * abs(Player.velocity.x) + 25.f;

		//Apply gravity and move 
		float deltaX = Player.velocity.x * deltaTime;
		float deltaY = Player.velocity.y * deltaTime;;
		Player.col->callibrateCollision(Player, deltaX, deltaY);
		Player.sprite.move(deltaX, deltaY);

		Player.applyGravity(deltaTime);

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && Player.getPosition().y > Player.jump_start_pos - Player.jump_start_max && !Player.is_jump_over)
		{
			Player.velocity.y -= 10000.f * deltaTime;

			if (std::abs(Player.velocity.y) > Player.max_velocity.y)
				Player.velocity.y = Player.max_velocity.y * -1.f;
		}

	if ((Player.getPosition().y < Player.jump_start_pos - Player.jump_start_max && Player.velocity.y >= 0.f) ||
		(Player.getBounds().top > Player.sprite.getPosition().y))
		{
			Player.is_jump_over = true;
		}

		// Check lucky block collision
		// if hit lucky block
		if (!Player.is_jump_over)
		{
			GameObject* obj = Player.col->getObject({ Player.getBounds().left,
			Player.getBounds().top + Player.velocity.y * deltaTime,
			Player.getBounds().width,
			Player.getBounds().height }, Player.velocity, "Block");

			if (obj != nullptr)
			{
				if (Player.current_Player_state == PlayerState::SMALL)
				{
					obj->onHit();
				}
				else if (Player.current_Player_state == PlayerState::BIG || Player.current_Player_state == PlayerState::FIRE)
				{
					obj->onHitBig();
				}

				Player.is_jump_over = true;
				Player.velocity.y += 350.f;
			}
		}
		

		//Then check collision with roof
		if (Player.col->checkCollision({ Player.getBounds().left,
			Player.getBounds().top + Player.velocity.y * Player.deltaTime,
			Player.getBounds().width,
			Player.getBounds().height }, Player.velocity, "All", CollisionType::TOP) && !Player.is_jump_over)
		{
			Player.is_jump_over = true;
			Player.velocity.y += 350.f;
			//std::cout << "ROOF\n";
		}

		if (Player.is_ground)
		{
			Player.setState(std::static_pointer_cast<IPlayerState>(std::make_shared<IPlayerIdle>()));

		}
	}

	void onExit(Player& Player) override
	{

	}
};

class IPlayerCrouch : public IPlayerState
{
	void onEnter(Player& Player) override
	{
		
	}

	void onUpdate(Player& Player, float deltaTime) override
	{
		if (!sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		{
			Player.setState(std::static_pointer_cast<IPlayerState>(std::make_shared<IPlayerIdle>()));
		}
		else
		{
			if (Player.velocity.x > 0.f)
			{
				Player.velocity.x -= 1000.f * deltaTime;
			}
			else if (Player.velocity.x < 0.f)
			{
				Player.velocity.x += 1000.f * deltaTime;
			}

			if (std::abs(Player.velocity.x) < 10.f)
			{
				Player.velocity.x = 0.f;
			}
		}
		
		Player.jump_start_max = 0.3125f * abs(Player.velocity.x) + 25.f;

		float deltaX = Player.velocity.x * deltaTime;
		float deltaY = Player.velocity.y * deltaTime;
		Player.col->callibrateCollision(Player, deltaX, deltaY);
		Player.sprite.move(deltaX, deltaY);

		Player.applyGravity(deltaTime);
	}

	void onExit(Player& Player) override
	{

	}
};

class IPlayerShoot : public IPlayerState
{
	void onEnter(Player& Player)
	{
		Player.is_jump_over = true;
		Player.fireBalls.push_back(std::make_shared<FireBall>("FireBall", sf::FloatRect(Player.sprite.getPosition().x + 8 * Player.direction, Player.sprite.getPosition().y + 8, 8, 8), 20, Player.texture_manager, Player.col, Player.direction));
		Player.Player_audio_manager->playSound("FireBall");
	}

	void onUpdate(Player& Player, float deltaTime)
	{
		
		Player.jump_start_max = 0.3125f * abs(Player.velocity.x) + 25.f;

		//Apply gravity and move 
		float deltaX = Player.velocity.x * deltaTime;
		float deltaY = Player.velocity.y * deltaTime;;
		Player.col->callibrateCollision(Player, deltaX, deltaY);
		Player.sprite.move(deltaX, deltaY);

		Player.applyGravity(deltaTime);
		
		if (Player.shoot_time < Player.shoot_timer)
		{
			Player.shoot_time += deltaTime;
		}
		else
		{
			Player.shoot_time = 0.f;
			Player.setLastState();
		}
		

	}

	void onExit(Player& Player)
	{

	}
};

class IPlayerDie : public IPlayerState
{
	void onEnter(Player& Player)
	{
		Player.sprite.setTexture(Player.texture);
		//Player.sprite.setTextureRect(sf::IntRect(0,0,16,16));
		Player.velocity.y = -500.f;
		Player.current_Player_state = PlayerState::SMALL;
	}

	void onUpdate(Player& Player, float deltaTime)
	{

		Player.jump_start_max = 0.3125f * abs(Player.velocity.x) + 25.f;

		//Apply gravity and move 
		Player.sprite.move(0.f, Player.velocity.y * deltaTime);
		Player.applyGravity(deltaTime);
	}

	void onExit(Player& Player)
	{

	}
};

class IPlayerCollectFlag: public IPlayerState
{
	void onEnter(Player& Player) override
	{
		if (Player.current_Player_state == PlayerState::SMALL)
			Player.sprite.setTextureRect(sf::IntRect(112, 0, 16, 16));
		else if (Player.current_Player_state == PlayerState::BIG)
			Player.sprite.setTextureRect(sf::IntRect(0, 64, 16, 32));
		else if(Player.current_Player_state == PlayerState::FIRE)
			Player.sprite.setTextureRect(sf::IntRect(0, 128, 16, 32));
		Player.sprite.move(20.f, 0.f);

		Player.sprite.setPosition(MathUtils::clamp(Player.sprite.getPosition().x, 9900.f - Player.sprite.getGlobalBounds().width, 9900.f), MathUtils::clamp(Player.sprite.getPosition().y, 50.f, 550.f + Player.sprite.getGlobalBounds().height));
	
		EventBus::Get().Push("OnPlayerTouchFlag");
	}

	void onUpdate(Player& Player, float deltaTime) override
	{
		if (Player.is_touching_flag && Player.sprite.getPosition().y < 550 - Player.sprite.getGlobalBounds().height)
		{
			Player.sprite.move(0.f, 200.f * deltaTime);
		}
	}

	void onExit(Player& Player) override
	{

	}
};

class IPlayerWaitToRun: public IPlayerState
{
	void onEnter(Player& Player) override
	{
		Player.sprite.move(Player.sprite.getGlobalBounds().width, 0.f);
		if (Player.current_Player_state == PlayerState::SMALL)
			Player.sprite.setTextureRect(sf::IntRect(128, 0, -16, 16));
		else if (Player.current_Player_state == PlayerState::BIG)
			Player.sprite.setTextureRect(sf::IntRect(16, 64, -16, 32));
		else if (Player.current_Player_state == PlayerState::FIRE)
			Player.sprite.setTextureRect(sf::IntRect(16, 128, -16, 32));
		Player.tclock.addClock(1.f, [this, &Player]() {Player.setState(std::static_pointer_cast<IPlayerState>(std::make_shared<IPlayerRunToCastle>())); }, "RunToCastle");
	}

	void onUpdate(Player& Player, float deltaTime) override
	{
		
	}

	void onExit(Player& Player) override
	{

	}
};

class IPlayerRunToCastle : public IPlayerState
{
	void onEnter(Player& Player) override
	{
		
		//Player.is_finishing = true;
	}

	void onUpdate(Player& Player, float deltaTime) override
	{
		if (Player.current_Player_state == PlayerState::SMALL)
			Player.animator->playAnim("Run");
		else if (Player.current_Player_state == PlayerState::BIG)
			Player.animator->playAnim("BRun");
		else if(Player.current_Player_state == PlayerState::FIRE)
			Player.animator->playAnim("FRun");
		//velocity.x = 100.f;
		if (Player.sprite.getPosition().y < 600 - Player.sprite.getGlobalBounds().height)
			Player.velocity.y = 70.f;
		else
			Player.velocity.y = 0;

		if (Player.sprite.getPosition().x > 10200)
		{
			EventBus::Get().Push("OnQuit");
		}

		float deltaX = 175.f * deltaTime;
		float deltaY = Player.velocity.y * deltaTime;
		Player.sprite.move(deltaX, deltaY);
	}

	void onExit(Player& Player) override
	{

	}
};