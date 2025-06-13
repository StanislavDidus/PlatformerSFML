#include "PlayerState.cpp"

//Init
void Player::initVariables()
{
	this->velocity = { 0.f, 0.f };
	this->max_velocity = { 300.f, 400.f };
	this->acceleration = { 600.f, 1.f };

	this->gravity = 980.f;

	this->is_jump_over = false;
	this->is_ground = false;

	//Aninimation
	this->direction = 1;

	this->jump_start_pos = 0.f;
	this->jump_start_height = 150.f;
	this->jump_start_max = 25.f; // 25,50 - 150

	this->is_sliding = false;
	this->slide_time = 0.f;
	this->slide_time_max = 0.2f;

	this->speed = 0.f;

	current_Player_state = PlayerState::SMALL;

	shoot_timer = 0.1f;
	wasShootPressedLastFrame = false;

	is_touching_flag = false;

	play_anim = true;

	is_falling = false;

	dead_time = 3.f;
	dead_timer = 0.f;
	need_restart = false;

	t_clock = TClock();
}

void Player::initSprite()
{
	this->sprite.setTexture(this->texture);
	this->sprite.setScale(3.f, 3.f);
	this->sprite.setTextureRect(sf::IntRect(0, 0, 16, 16)); // Int rect (x, y, width, height)
}

void Player::initState()
{
	this->setState(std::make_shared<IPlayerIdle>());
}

void Player::initAnimator()
{
	//Create animator
	this->animator = std::make_unique<Animator>();

	//Add animations
	//Idle
	this->animator->addFrameAnimation(
		this->sprite, 16, 16, std::vector<int>{0}, 50.f / 1000.f, [this]() {return std::dynamic_pointer_cast<IPlayerIdle>(this->current_state) != nullptr && current_Player_state == PlayerState::SMALL; }, [this]() {return this->direction; }, true, 5, "Idle"
	);
	//Run
	this->animator->addFrameAnimation(
		this->sprite, 16, 16, std::vector<int>{ 1, 2, 3 }, 50.f / 1000.f, [this]() {return std::dynamic_pointer_cast<IPlayerWalk>(this->current_state) != nullptr && current_Player_state == PlayerState::SMALL;  }, [this]() {return this->direction; }, true, 5, "Run"
	);
	//Slide
	this->animator->addFrameAnimation(
		this->sprite, 16, 16, std::vector<int>{ 4 }, 10.f / 1000.f, [this]() {
			return this->is_sliding && std::dynamic_pointer_cast<IPlayerWalk>(this->current_state) != nullptr && current_Player_state == PlayerState::SMALL;
		}, [this]() {return this->direction; }, true, 10, "Slide"
			);
	//Jump
	this->animator->addFrameAnimation(
		this->sprite, 16, 16, std::vector<int>{ 5 }, 50.f / 1000.f, [this]() {return std::dynamic_pointer_cast<IPlayerJump>(this->current_state) != nullptr && current_Player_state == PlayerState::SMALL;  }, [this]() {return this->direction; }, false, 5, "Jump"
	);

	//Player growth animation
	this->animator->addFrameAnimation(
		this->sprite, 16, 32, std::vector<int>{0, 1, 0, 1, 0, 2, 0, 1, 2, 0, 1, 2}, 0.1f, [this]() {return false; }, [this]() {return this->direction; }, false, 32, "Grow"
	);

	//Big Player Animations
	//Idle
	this->animator->addFrameAnimation(
		this->sprite, 16, 32, std::vector<int>{7}, 100.f / 1000.f, [this]() {return std::dynamic_pointer_cast<IPlayerIdle>(this->current_state) != nullptr && current_Player_state == PlayerState::BIG; }, [this]() {return this->direction; }, true, 25, "BIdle"
	);
	//Run
	this->animator->addFrameAnimation(
		this->sprite, 16, 32, std::vector<int>{ 8, 9, 10 }, 100.f / 1000.f, [this]() {return std::dynamic_pointer_cast<IPlayerWalk>(this->current_state) != nullptr && current_Player_state == PlayerState::BIG;  }, [this]() {return this->direction; }, true, 25, "BRun"
	);
	//Slide
	this->animator->addFrameAnimation(
		this->sprite, 16, 32, std::vector<int>{ 11 }, 25.f / 1000.f, [this]() {
			return this->is_sliding && std::dynamic_pointer_cast<IPlayerWalk>(this->current_state) != nullptr && current_Player_state == PlayerState::BIG;
		}, [this]() {return this->direction; }, true, 30, "BSlide"
			);
	//Jump
	this->animator->addFrameAnimation(
		this->sprite, 16, 32, std::vector<int>{ 12 }, 100.f / 1000.f, [this]() {return std::dynamic_pointer_cast<IPlayerJump>(this->current_state) != nullptr && current_Player_state == PlayerState::BIG;  }, [this]() {return this->direction; }, false, 25, "BJump"
	);
	//Crouch
	this->animator->addFrameAnimation(
		this->sprite, 16, 32, std::vector<int>{ 13 }, 100.f / 1000.f, [this]() {return std::dynamic_pointer_cast<IPlayerCrouch>(this->current_state) != nullptr && current_Player_state == PlayerState::BIG;  }, [this]() {return this->direction; }, false, 30, "BCrouch"
	);
	//Transformat into fire Player
	this->animator->addFrameAnimation(
		this->sprite, 16, 32, std::vector<int>{ 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3 }, 0.1f, [this]() {return false;  }, [this]() {return this->direction; }, false, 30, "Fire"
	);

	//Fire Player Animations
	//Idle
	this->animator->addFrameAnimation(
		this->sprite, 16, 32, std::vector<int>{21}, 100.f / 1000.f, [this]() {return std::dynamic_pointer_cast<IPlayerIdle>(this->current_state) != nullptr && current_Player_state == PlayerState::FIRE; }, [this]() {return this->direction; }, true, 25, "FIdle"
	);
	//Run
	this->animator->addFrameAnimation(
		this->sprite, 16, 32, std::vector<int>{22, 23, 24 }, 100.f / 1000.f, [this]() {return std::dynamic_pointer_cast<IPlayerWalk>(this->current_state) != nullptr && current_Player_state == PlayerState::FIRE;  }, [this]() {return this->direction; }, true, 25, "FRun"
	);
	//Slide
	this->animator->addFrameAnimation(
		this->sprite, 16, 32, std::vector<int>{25 }, 25.f / 1000.f, [this]() {
			return this->is_sliding && std::dynamic_pointer_cast<IPlayerWalk>(this->current_state) != nullptr && current_Player_state == PlayerState::FIRE;
		}, [this]() {return this->direction; }, true, 30, "FSlide"
			);
	//Jump
	this->animator->addFrameAnimation(
		this->sprite, 16, 32, std::vector<int>{ 26 }, 100.f / 1000.f, [this]() {return std::dynamic_pointer_cast<IPlayerJump>(this->current_state) != nullptr && current_Player_state == PlayerState::FIRE;  }, [this]() {return this->direction; }, false, 25, "FJump"
	);
	//Crouch
	this->animator->addFrameAnimation(
		this->sprite, 16, 32, std::vector<int>{ 27 }, 100.f / 1000.f, [this]() {return std::dynamic_pointer_cast<IPlayerCrouch>(this->current_state) != nullptr && current_Player_state == PlayerState::FIRE;  }, [this]() {return this->direction; }, false, 30, "FCrouch"
	);
	//Shoot
	this->animator->addFrameAnimation(
		this->sprite, 16, 32, std::vector<int>{ 30 }, shoot_time, [this]() {return std::dynamic_pointer_cast<IPlayerShoot>(this->current_state) != nullptr;  }, [this]() {return this->direction; }, false, 50, "FShoot"
	);

	//Die
	this->animator->addFrameAnimation(
		this->sprite, 16, 16, std::vector<int>{ 6 }, 5.f, [this]() {return std::dynamic_pointer_cast<IPlayerDie>(this->current_state) != nullptr;  }, [this]() {return 1; }, false, 100, "Die"
	);
}

void Player::initAudio()
{
	this->Player_audio_manager = std::make_unique<AudioManager>();
	this->Player_audio_manager->addSound("Jump_Small", "assets/Sounds/Player/Jump_Small.wav", false);
	this->Player_audio_manager->addSound("Jump_Super", "assets/Sounds/Player/Jump_Super.wav", false);
	this->Player_audio_manager->addSound("FireBall", "assets/Sounds/Player/FireBall.wav", false);
	//this->Player_audio_manager->addSound("Bump", "assets/Sounds/Player/Bump.wav", false);
	//this->Player_audio_manager->addSound("Brick_Smash", "assets/Sounds/Player/Brick_Smash.wav", false);
	this->Player_audio_manager->addSound("PowerUp", "assets/Sounds/Player/PowerUp.wav", false);

}

//Con/Des
Player::Player(sf::RenderWindow* window, Map* map, std::shared_ptr<CollisionManager> col, std::shared_ptr<TextureManager> texture_manager, sf::View* view, const sf::FloatRect& rect, const std::string& type, int layer, int lifes) : window(window),
texture_manager(texture_manager), texture(*texture_manager->get("Player").get()), view(view),
texture1(*texture_manager->get("PlayerBig").get()), map(map), col(col), GameObject(type, rect, layer), lifes(lifes)
{
	this->initVariables();
	this->initSprite();
	this->initState();
	this->initAnimator();
	this->initAudio();

	//this->setPosition({ 10000.f, 400.f });
	this->sprite.setPosition(150.f, 500.f);

	//placeholder
	//this->sprite.setPosition(9451.f, 152.f);
}

Player::~Player()
{
}

//Accessors
const sf::FloatRect Player::getBounds() const
{
	return this->sprite.getGlobalBounds();
}

const sf::Vector2f Player::getPosition() const
{
	return this->sprite.getPosition();
}

//Modifiers
void Player::setGround(bool state)
{
	this->is_ground = state;
}

void Player::setPosition(const sf::Vector2f& newPosition)
{
	//GameObject::setPosition(newPosition);
	//sprite.setPosition(newPosition);
	//std::cout << newPosition.x << ", " << newPosition.y << "\n";
}

const int Player::getLifes() const
{
	return lifes;
}

void Player::Finish()
{
	if (std::dynamic_pointer_cast<IPlayerWaitToRun>(this->current_state) == nullptr && std::dynamic_pointer_cast<IPlayerRunToCastle>(this->current_state) == nullptr)
	{
		setState(std::static_pointer_cast<IPlayerState>(std::make_shared<IPlayerWaitToRun>()));
	}
}

//Functions
void Player::move(float dirX, float dirY)
{

	this->velocity.x += dirX * this->acceleration.x * deltaTime;

	if (std::abs(this->velocity.x) > this->max_velocity.x)
		this->velocity.x = this->max_velocity.x * (this->velocity.x > 0 ? 1.f : -1.f);
}

void Player::grow()
{
	//Set Player1 texture
	this->sprite.setTexture(this->texture1);
	this->sprite.setPosition(this->sprite.getPosition().x, this->sprite.getPosition().y - 48.f);
	this->sprite.setTextureRect(sf::IntRect(0, 0, 16, 32));
	//this->sprite.setScale(3.f, 3.f);
	GameObject::setBounds({ 16 * 3.f, 32 * 3.f });
	GameObject::setPosition({ sprite.getPosition().x, sprite.getPosition().y });
	current_Player_state = PlayerState::GROWING;
	tclock.addClock(1.1f, [this]() { current_Player_state = PlayerState::BIG; sprite.setTexture(texture1); }, "Grow");
	Player_audio_manager->playSound("PowerUp");
	this->animator->playAnim("Grow");
}

void Player::fire()
{
	const sf::Texture* tx = sprite.getTexture();
	sf::Image origin_img = tx->copyToImage();
	sf::FloatRect fragmentRect = animator->getCurrentFrame();

	//Basic Color
	sf::Image newImage;
	//Inverted colors
	std::vector<sf::Image> images(3);

	sf::IntRect intRect(
		static_cast<int>(fragmentRect.left),
		static_cast<int>(fragmentRect.top),
		static_cast<int>(fragmentRect.width),
		static_cast<int>(fragmentRect.height)
	);

	newImage.create(intRect.width, intRect.height);
	newImage.copy(origin_img, 0, 0, intRect, false);
	for (int i = 0; i < 3; i++)
	{
		images[i].create(intRect.width, intRect.height);
		images[i].copy(origin_img, 0, 0, intRect, false);
	}

	auto fromHex = [](uint32_t hex) {
		return sf::Color((hex >> 16) & 0xFF, (hex >> 8) & 0xFF, hex & 0xFF);
		};

	//Three filters for Player
	std::vector<uint3> colors = { {0x0c9300,0xfffeff,0xea9e22}, {0xb53120,0xfffeff,0xea9e22}, {0x000000,0xfeccc5,0x994e00} };

	for (int i = 0; i < 3; i++)
	{
		for (unsigned int x = 0; x < fragmentRect.width; ++x)
		{
			for (unsigned int y = 0; y < fragmentRect.height; ++y)
			{
				unsigned int globalX = x + fragmentRect.left;
				unsigned int globalY = y + fragmentRect.top;

				sf::Color col = origin_img.getPixel(globalX, globalY);

				if (col == fromHex(0xb53120))
					col = fromHex(colors[i].x);
				else if (col == fromHex(0xea9e22))
					col = fromHex(colors[i].y);
				else if (col == fromHex(0x6b6d00))
					col = fromHex(colors[i].z);

				images[i].setPixel(x, y, col);
			}
		}
	}
	sf::Image finalImage;
	unsigned int fullWidth = newImage.getSize().x + images[0].getSize().x + images[1].getSize().x + images[2].getSize().x;
	unsigned int fullHeight = newImage.getSize().y;

	finalImage.create(fullWidth, fullHeight);

	//Copy from images
	finalImage.copy(newImage, 0, 0);
	finalImage.copy(images[0], 16, 0);
	finalImage.copy(images[1], 32, 0);
	finalImage.copy(images[2], 48, 0);

	tx_glitch.loadFromImage(finalImage);
	sprite.setTexture(tx_glitch);

	sprite.setTextureRect(sf::IntRect(0, 0, 16, 32));
	current_Player_state = PlayerState::FIRING;
	tclock.addClock(1.3, [this]() { current_Player_state = PlayerState::FIRE; sprite.setTexture(texture1); }, "Fire");
	Player_audio_manager->playSound("PowerUp");
	animator->playAnim("Fire");
}

void Player::shoot()
{

}

void Player::die()
{
	if (!is_dead)
	{
		is_dead = true;
		EventBus::Get().Push("OnPlayerDeath");
	}
}

void Player::timeUp()
{
	if (std::dynamic_pointer_cast<IPlayerDie>(current_state) == nullptr && !is_dead)
	{
		setState(std::static_pointer_cast<IPlayerState>(std::make_shared<IPlayerDie>()));
		/////////////////////////////////////////////////////////////////////////
		//t_clock.addClock(3.f, [this]() {std::cout << "TimeUp\n"; }, "TimeUp");
	}
}

void Player::checkSlide()
{
	if (!this->is_sliding)
	{
		if (this->direction == -1 && this->velocity.x > 150.f)
		{
			this->is_sliding = true;

		}
		else if (this->direction == 1 && this->velocity.x < -150.f)
		{
			this->is_sliding = true;
		}
	}

	if (this->is_sliding)
	{
		this->slide_time += this->deltaTime;

		if (this->slide_time >= this->slide_time_max || this->velocity.x == 0.f)
		{
			this->is_sliding = false;
			this->slide_time = 0.f;

		}
	}
}

void Player::checkCollisions()
{


}

void Player::flip(int dir)
{
	this->direction = dir;
}

void Player::applyGravity(float deltaTime)
{
	this->velocity.y += this->gravity * deltaTime;

	if (this->velocity.y > this->max_velocity.y)
		this->velocity.y = this->max_velocity.y;
}

void Player::setState(const std::shared_ptr<IPlayerState>& state)
{
	if (current_state == state)
		return;

	if (this->current_state != nullptr) // OnExit
	{
		this->current_state->onExit(*this);

	}

	this->current_state = state; // set new state

	this->current_state->onEnter(*this); // OnEnter
}

void Player::setLastState()
{
	if (last_state == current_state)
		return;

	if (this->last_state != nullptr) // OnExit
	{
		this->last_state->onExit(*this);

	}

	this->current_state = last_state; // set new state
}

void Player::updateCollision()
{

}

void Player::updateFireBalls(float deltaTime)
{
	//FireBalls
	for (int i = fireBalls.size() - 1; i >= 0; i--)
	{
		fireBalls[i]->update(deltaTime);

		float distance = view->getCenter().x - fireBalls[i]->getPosition().x;
		if (fireBalls[i]->isDestroyed() || fireBalls[i]->getPosition().x < 0.f - fireBalls[i]->getBounds().width || abs(distance) > 400.f)
		{
			fireBalls.erase(fireBalls.begin() + i);
		}
	}
}


bool wasPressed = false;
void Player::update(float deltaTime)
{
	this->deltaTime = deltaTime;
	tclock.update(deltaTime);
	//std::cout << sprite.getPosition().x << ", " << sprite.getPosition().y << "\n";

	t_clock.update(deltaTime);

	//Check if Player is lower than 0
	if (this->sprite.getPosition().y - this->sprite.getGlobalBounds().height > 600 && std::dynamic_pointer_cast<IPlayerDie>(current_state) == nullptr)
	{
		//Dead
		this->die();
		//Start death timer

	}

	//Check fall
	


	if (this->current_state != nullptr) // Update current state
		this->current_state->onUpdate(*this, deltaTime);

	if (this->animator != nullptr && play_anim && !is_falling)
		this->animator->update(this->deltaTime);

	//Check Grow and not let Player move while growing
	
	if (tclock.getClock("Grow") != nullptr && tclock.getClock("Grow")->is_playing)
	{
		this->velocity = { 0.f,0.f };
	}

	//Check fire and not let Player move while transforms into fire form
	TClockEvent* fire_clock_event = tclock.getClock("Fire");
	if (fire_clock_event != nullptr)
	{
		velocity = { 0.f, 0.f };
	}
	//else if (current_Player_state == PlayerState::BIG)
		//sprite.setTexture(texture1);

	//Check collisions
	this->checkCollisions();

	// Limit Player x movement 
	this->sprite.setPosition(MathUtils::clamp(this->sprite.getPosition().x, this->window->getView().getCenter().x - this->window->getSize().x / 2.f, this->window->getView().getCenter().x + this->window->getSize().x / 2.f - this->sprite.getGlobalBounds().width), this->sprite.getPosition().y);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)) {
		if (!wasPressed) {
			if (current_Player_state == PlayerState::FIRE)
				shoot();
			wasPressed = true;
		}
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::F)) {
		if (!wasPressed) {
			fire();
			wasPressed = true;
		}
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::G)) {
		if (!wasPressed) {
			grow();
			wasPressed = true;
		}
	}
	else {
		wasPressed = false;
	}

	updateFireBalls(deltaTime);

	//Check finish state (if Player is not in cinematic state and touches a flag)
	is_touching_flag = col->checkCollision({ sprite.getGlobalBounds().left + velocity.x * deltaTime,
		sprite.getGlobalBounds().top,
		sprite.getGlobalBounds().width,
		sprite.getGlobalBounds().height
		}, velocity, "Flag", CollisionType::ALL);


	if (is_touching_flag && std::dynamic_pointer_cast<IPlayerCollectFlag>(this->current_state) == nullptr && std::dynamic_pointer_cast<IPlayerRunToCastle>(this->current_state) == nullptr)
	{
		setState(std::static_pointer_cast<IPlayerState>(std::make_shared<IPlayerCollectFlag>()));
	}
	//

	is_ground = col->checkCollision({ sprite.getGlobalBounds().left,
			sprite.getGlobalBounds().top + velocity.y * deltaTime,
			sprite.getGlobalBounds().width,
			sprite.getGlobalBounds().height }, velocity, "All", CollisionType::DOWN);

	sf::Vector2f newPosition = this->sprite.getPosition();
	GameObject::setPosition(newPosition);
}

void Player::render(sf::RenderTarget* target)
{
	for (auto fireBall : fireBalls)
	{
		fireBall->render(target);
	}

	if (!is_dead)
		target->draw(this->sprite);


	/*sf::RectangleShape debugRect;
	debugRect.setPosition(sprite.getGlobalBounds().left, sprite.getGlobalBounds().top);
	debugRect.setSize({ sprite.getGlobalBounds().width, sprite.getGlobalBounds().height });
	debugRect.setFillColor(sf::Color::Transparent);
	debugRect.setOutlineColor(sf::Color::Red);
	debugRect.setOutlineThickness(1.f);
	target->draw(debugRect);*/
}