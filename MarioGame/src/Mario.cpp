#include "MarioState.cpp"

//Init
void Mario::initVariables()
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

	this->grow_time = 0.9f;
	this->grow_timer = 0.f;
	this->is_grown = false;

	this->fire_time = 1.3f;
	this->fire_timer = 0.f;
	this->fire_transform = false;
	this->is_fire = false;
}

void Mario::initSprite()
{
	this->sprite.setTexture(this->texture);
	this->sprite.setScale(3.f, 3.f);
	this->sprite.setTextureRect(sf::IntRect(0, 0, 16, 16)); // Int rect (x, y, width, height)
}

void Mario::initState()
{
	this->setState(std::make_shared<IMarioIdle>());
}

void Mario::initAnimator()
{
	//Create animator
	this->animator = std::make_unique<Animator>();

	//Add animations
	//Idle
	this->animator->addFrameAnimation(
		this->sprite, 16, 16, std::vector<int>{0}, 50.f / 1000.f, [this]() {return std::dynamic_pointer_cast<IMarioIdle>(this->current_state) != nullptr && !this->is_grown && !is_fire && !fire_transform; }, [this]() {return this->direction; }, true, 5, "Idle"
	);
	//Run
	this->animator->addFrameAnimation(
		this->sprite, 16, 16, std::vector<int>{ 1, 2, 3 }, 50.f / 1000.f, [this]() {return std::dynamic_pointer_cast<IMarioWalk>(this->current_state) != nullptr && !this->is_grown && !is_fire && !fire_transform;  }, [this]() {return this->direction; }, true, 5, "Run"
	);
	//Slide
	this->animator->addFrameAnimation(
		this->sprite, 16, 16, std::vector<int>{ 4 }, 10.f / 1000.f, [this]() {
			return this->is_sliding && std::dynamic_pointer_cast<IMarioWalk>(this->current_state) != nullptr && !this->is_grown && !is_fire && !fire_transform;
		}, [this]() {return this->direction; }, true, 10, "Slide"
			);
	//Jump
	this->animator->addFrameAnimation(
		this->sprite, 16, 16, std::vector<int>{ 5 }, 50.f / 1000.f, [this]() {return std::dynamic_pointer_cast<IMarioJump>(this->current_state) != nullptr && !this->is_grown && !is_fire && !fire_transform;  }, [this]() {return this->direction; }, false, 5, "Jump"
	);

	//Mario growth animation
	this->animator->addFrameAnimation(
		this->sprite, 16, 32, std::vector<int>{0, 1, 0, 1, 0, 2, 0, 1, 2, 0, 1, 2}, 0.1f, [this]() {return false; }, [this]() {return this->direction; }, false, 30, "Grow"
	);

	//Big Mario Animations
	//Idle
	this->animator->addFrameAnimation(
		this->sprite, 16, 32, std::vector<int>{7}, 100.f / 1000.f, [this]() {return std::dynamic_pointer_cast<IMarioIdle>(this->current_state) != nullptr && this->is_grown && !is_fire && !fire_transform; }, [this]() {return this->direction; }, true, 25, "BIdle"
	);
	//Run
	this->animator->addFrameAnimation(
		this->sprite, 16, 32, std::vector<int>{ 8, 9, 10 }, 100.f / 1000.f, [this]() {return std::dynamic_pointer_cast<IMarioWalk>(this->current_state) != nullptr && this->is_grown && !is_fire && !fire_transform;  }, [this]() {return this->direction; }, true, 25, "BRun"
	);
	//Slide
	this->animator->addFrameAnimation(
		this->sprite, 16, 32, std::vector<int>{ 11 }, 25.f / 1000.f, [this]() {
			return this->is_sliding && std::dynamic_pointer_cast<IMarioWalk>(this->current_state) != nullptr && this->is_grown && !is_fire && !fire_transform;
		}, [this]() {return this->direction; }, true, 30, "BSlide"
			);
	//Jump
	this->animator->addFrameAnimation(
		this->sprite, 16, 32, std::vector<int>{ 12 }, 100.f / 1000.f, [this]() {return std::dynamic_pointer_cast<IMarioJump>(this->current_state) != nullptr && this->is_grown && !is_fire && !fire_transform;  }, [this]() {return this->direction; }, false, 25, "BJump"
	);
	//Crouch
	this->animator->addFrameAnimation(
		this->sprite, 16, 32, std::vector<int>{ 13 }, 100.f / 1000.f, [this]() {return std::dynamic_pointer_cast<IMarioCrouch>(this->current_state) != nullptr && !is_fire && !fire_transform;  }, [this]() {return this->direction; }, false, 30, "BCrouch"
	);
	//Transformat into fire mario
	this->animator->addFrameAnimation(
		this->sprite, 16, 32, std::vector<int>{ 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3 }, 0.1f, [this]() {return false;  }, [this]() {return this->direction; }, false, 30, "Fire"
	);

	//Fire Mario Animations
	//Idle
	this->animator->addFrameAnimation(
		this->sprite, 16, 32, std::vector<int>{0}, 100.f / 1000.f, [this]() {return std::dynamic_pointer_cast<IMarioIdle>(this->current_state) != nullptr && this->is_grown && is_fire; }, [this]() {return this->direction; }, true, 25, "FIdle"
	);
	//Run
	this->animator->addFrameAnimation(
		this->sprite, 16, 32, std::vector<int>{1, 2, 3 }, 100.f / 1000.f, [this]() {return std::dynamic_pointer_cast<IMarioWalk>(this->current_state) != nullptr && this->is_grown && is_fire;  }, [this]() {return this->direction; }, true, 25, "FRun"
	);
	//Slide
	this->animator->addFrameAnimation(
		this->sprite, 16, 32, std::vector<int>{4 }, 25.f / 1000.f, [this]() {
			return this->is_sliding && std::dynamic_pointer_cast<IMarioWalk>(this->current_state) != nullptr && this->is_grown && is_fire;
		}, [this]() {return this->direction; }, true, 30, "FSlide"
			);
	//Jump
	this->animator->addFrameAnimation(
		this->sprite, 16, 32, std::vector<int>{ 5 }, 100.f / 1000.f, [this]() {return std::dynamic_pointer_cast<IMarioJump>(this->current_state) != nullptr && this->is_grown && is_fire;  }, [this]() {return this->direction; }, false, 25, "FJump"
	);
	//Crouch
	this->animator->addFrameAnimation(
		this->sprite, 16, 32, std::vector<int>{ 6 }, 100.f / 1000.f, [this]() {return std::dynamic_pointer_cast<IMarioCrouch>(this->current_state) != nullptr && is_fire;  }, [this]() {return this->direction; }, false, 30, "FCrouch"
	);
}

void Mario::initAudio()
{
	this->mario_audio_manager = std::make_unique<AudioManager>();

	//this->mario_audio_manager->addSound("Jump", "assets/Sounds/Mario/Jump.wav", false);
}

//Con/Des
Mario::Mario(sf::RenderWindow* window, Map* map, CollisionManager* col, std::shared_ptr<TextureManager> texture_manager, const sf::FloatRect& rect, const std::string& type, int layer) : window(window), texture(*texture_manager->get("Mario").get()),
texture1(*texture_manager->get("MarioBig").get()), texture2(*texture_manager->get("MarioFire").get()), map(map), col(col), GameObject(type, rect, layer)
{
	this->initVariables();
	this->initSprite();
	this->initState();
	this->initAnimator();
	this->initAudio();

	//this->setPosition({ 10000.f, 400.f });
	//this->sprite.setPosition(150.f,400.f);

	//placeholder
	//this->sprite.setPosition(9451.f, 152.f);
}

Mario::~Mario()
{
}

//Accessors
const sf::FloatRect Mario::getBounds() const
{
	return this->sprite.getGlobalBounds();
}

const sf::Vector2f Mario::getPosition() const
{
	return this->sprite.getPosition();
}

//Modifiers
void Mario::setGround(bool state)
{
	this->is_ground = state;
}

void Mario::setPosition(const sf::Vector2f& newPosition)
{
	GameObject::setPosition(newPosition);
	//sprite.setPosition(newPosition);
	//std::cout << newPosition.x << ", " << newPosition.y << "\n";
}

void Mario::Finish(float deltaTime)
{
	if (std::dynamic_pointer_cast<IMarioRunToCastle>(this->current_state) == nullptr)
	{
		setState(std::static_pointer_cast<IMarioState>(std::make_shared<IMarioRunToCastle>()));
	}
}

//Functions
void Mario::move(float dirX, float dirY)
{

	this->velocity.x += dirX * this->acceleration.x * deltaTime;

	if (std::abs(this->velocity.x) > this->max_velocity.x)
		this->velocity.x = this->max_velocity.x * (this->velocity.x > 0 ? 1.f : -1.f);
}

void Mario::grow()
{
	//Set mario1 texture
	this->sprite.setTexture(this->texture1);
	this->sprite.setPosition(this->sprite.getPosition().x, this->sprite.getPosition().y - 48.f);
	this->sprite.setTextureRect(sf::IntRect(0, 0, 16, 32));
	//this->sprite.setScale(3.f, 3.f);
	GameObject::setBounds({ 16 * 3.f, 32 * 3.f });
	GameObject::setPosition({ sprite.getPosition().x, sprite.getPosition().y });
	this->is_grown = true;
	this->animator->playAnim("Grow");
}

void Mario::fire()
{
	play_anim = false;

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

	//Three filters for mario
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

	fire_transform = true;
	animator->playAnim("Fire");
}

void Mario::die()
{

}

void Mario::checkSlide()
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

void Mario::checkCollisions()
{


}

void Mario::flip(int dir)
{
	this->direction = dir;
}

void Mario::applyGravity(float deltaTime)
{
	this->velocity.y += this->gravity * deltaTime;

	if (this->velocity.y > this->max_velocity.y)
		this->velocity.y = this->max_velocity.y;
}

void Mario::setState(const std::shared_ptr<IMarioState>& state)
{
	if (this->current_state != nullptr) // OnExit
		this->current_state->onExit(*this);

	this->current_state = state; // set new state

	this->current_state->onEnter(*this); // OnEnter
}

void Mario::updateCollision()
{

}


bool wasPressed = false;
void Mario::update(float deltaTime)
{
	this->deltaTime = deltaTime;

	//std::cout << sprite.getPosition().x << ", " << sprite.getPosition().y << "\n";

	//Check if mario is lower than 0
	if (this->sprite.getPosition().y - this->sprite.getGlobalBounds().height > 600)
	{
		//Dead
		this->die();
		//Start death timer

	}

	if (this->current_state != nullptr) // Update current state
		this->current_state->onUpdate(*this, deltaTime);

	if (this->animator != nullptr)
		this->animator->update(this->deltaTime);

	//Check Grow and not let mario move while growing
	if (this->is_grown && this->grow_timer < this->grow_time)
	{
		this->grow_timer += deltaTime;
		this->velocity = { 0.f,0.f };
	}
	else if (!this->is_grown && this->grow_timer >= this->grow_time)
		this->grow_timer = 0.f;

	//Check fire and not let mario move while transforms into fire form
	if (fire_transform)
	{
		if (fire_timer < fire_time)
		{
			fire_timer += deltaTime;
			velocity = { 0.f, 0.f };
		}
		else
		{
			fire_timer = 0.f;
			fire_transform = false;
			is_fire = true;
			sprite.setTexture(texture2);
		}
	}

	//Check collisions
	this->checkCollisions();

	// Limit mario x movement 
	this->sprite.setPosition(MathUtils::clamp(this->sprite.getPosition().x, this->window->getView().getCenter().x - this->window->getSize().x / 2.f, this->window->getView().getCenter().x + this->window->getSize().x / 2.f - this->sprite.getGlobalBounds().width), this->sprite.getPosition().y);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::F)) {
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

	//Check finish state (if mario is not in cinematic state and touches a flag)
	is_touching_flag = col->checkCollision({ sprite.getGlobalBounds().left + velocity.x * deltaTime,
		sprite.getGlobalBounds().top,
		sprite.getGlobalBounds().width,
		sprite.getGlobalBounds().height
		}, velocity, "Flag", CollisionType::ALL);


	if (is_touching_flag && std::dynamic_pointer_cast<IMarioCollectFlag>(this->current_state) == nullptr && std::dynamic_pointer_cast<IMarioRunToCastle>(this->current_state) == nullptr)
	{
		setState(std::static_pointer_cast<IMarioState>(std::make_shared<IMarioCollectFlag>()));
	}
	//

	is_ground = col->checkCollision({ sprite.getGlobalBounds().left,
			sprite.getGlobalBounds().top + velocity.y * deltaTime,
			sprite.getGlobalBounds().width,
			sprite.getGlobalBounds().height }, velocity, "All", CollisionType::DOWN);

	sf::Vector2f newPosition = this->sprite.getPosition();
	setPosition(newPosition);
}

void Mario::render(sf::RenderTarget* target)
{
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
