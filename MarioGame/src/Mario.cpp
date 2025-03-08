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
}

void Mario::initSprite()
{
	if (!this->texture.loadFromFile("assets/Textures/Mario/Mario0.png"))
		std::cout << "Could not load mario0 texture\n";
	if (!this->texture1.loadFromFile("assets/Textures/Mario/Mario1.png"))
		std::cout << "Could not load mario1 texture\n";

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
		this->sprite, 16, 16, std::vector<int>{0}, 50.f / 1000.f, [this]() {return std::dynamic_pointer_cast<IMarioIdle>(this->current_state) != nullptr && !this->is_grown; }, [this]() {return this->direction; }, true, 5, "Idle"
	);
	//Run
	this->animator->addFrameAnimation(
		this->sprite, 16, 16, std::vector<int>{ 1, 2, 3 }, 50.f / 1000.f, [this]() {return std::dynamic_pointer_cast<IMarioWalk>(this->current_state) != nullptr && !this->is_grown;  }, [this]() {return this->direction; }, true, 5, "Run"
	);
	//Slide
	this->animator->addFrameAnimation(
		this->sprite, 16, 16, std::vector<int>{ 4 }, 10.f / 1000.f, [this]() {
			return this->is_sliding && std::dynamic_pointer_cast<IMarioWalk>(this->current_state) != nullptr && !this->is_grown;
		}, [this]() {return this->direction; }, true, 10, "Slide"
			);
	//Jump
	this->animator->addFrameAnimation(
		this->sprite, 16, 16, std::vector<int>{ 5 }, 50.f / 1000.f, [this]() {return std::dynamic_pointer_cast<IMarioJump>(this->current_state) != nullptr && !this->is_grown;  }, [this]() {return this->direction; }, false, 5, "Jump"
	);

	//Mario growth animation
	this->animator->addFrameAnimation(
		this->sprite, 16, 32, std::vector<int>{0,1,0,1,0,2,0,1,2,0,1,2}, 0.1f, [this]() {return false; }, [this]() {return this->direction; }, false, 30, "Grow"
	);

	//Big Mario Animations
	//Idle
	this->animator->addFrameAnimation(
		this->sprite, 16, 32, std::vector<int>{7}, 100.f / 1000.f, [this]() {return std::dynamic_pointer_cast<IMarioIdle>(this->current_state) != nullptr && this->is_grown; }, [this]() {return this->direction; }, true, 25, "BIdle"
	);
	//Run
	this->animator->addFrameAnimation(
		this->sprite, 16, 32, std::vector<int>{ 8, 9, 10 }, 100.f / 1000.f, [this]() {return std::dynamic_pointer_cast<IMarioWalk>(this->current_state) != nullptr && this->is_grown;  }, [this]() {return this->direction; }, true, 25, "BRun"
	);
	//Slide
	this->animator->addFrameAnimation(
		this->sprite, 16, 32, std::vector<int>{ 11 }, 25.f / 1000.f, [this]() {
			return this->is_sliding && std::dynamic_pointer_cast<IMarioWalk>(this->current_state) != nullptr && this->is_grown;
		}, [this]() {return this->direction; }, true, 30, "BSlide"
			);
	//Jump
	this->animator->addFrameAnimation(
		this->sprite, 16, 32, std::vector<int>{ 12 }, 100.f / 1000.f, [this]() {return std::dynamic_pointer_cast<IMarioJump>(this->current_state) != nullptr && this->is_grown;  }, [this]() {return this->direction; }, false, 25, "BJump"
	);

}

void Mario::initAudio()
{
	this->mario_audio_manager = std::make_unique<AudioManager>();

	//this->mario_audio_manager->addSound("Jump", "assets/Sounds/Mario/Jump.wav", false);
}

//Con/Des
Mario::Mario(sf::RenderWindow* window, Map* map, CollisionManager* col, const sf::FloatRect& rect, const std::string& type) : window(window),
map(map), col(col), GameObject(type, rect)
{
	this->initVariables();
	this->initSprite();
	this->initState();
	this->initAnimator();
	this->initAudio();
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
	sprite.setPosition(newPosition);
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
	this->sprite.setPosition(this->sprite.getPosition().x, this->sprite.getPosition().y - 16.f * 3.125f);
	this->sprite.setTextureRect(sf::IntRect(0, 0, 16, 32));
	this->is_grown = true;
	this->animator->playAnim("Grow");
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

void Mario::chechCollisions()
{
	if (col->checkCollision({ getBounds().left,
			getBounds().top,
			getBounds().width,
			getBounds().height }, velocity, "Mushroom", CollisionType::ALL))
	{
		GameObject* obj = col->getObject({ getBounds().left,
		getBounds().top,
		getBounds().width,
		getBounds().height }, velocity, "Mushroom");

		std::cout << "mushroom1\n";

		if (obj != nullptr)
		{
			obj->onHit();
			std::cout << "mushroom2\n";
		}
	}
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

	if (this->current_state != nullptr) // Update current state
		this->current_state->onUpdate(*this, deltaTime);

	if (this->animator != nullptr)
		this->animator->update(this->deltaTime);

	//Check Grow
	if (this->is_grown && this->grow_timer < this->grow_time)
	{
		this->grow_timer += deltaTime;
		this->velocity = { 0.f,0.f };
	}
	else if (!this->is_grown && this->grow_timer >= this->grow_time)
		this->grow_timer = 0.f;

	//Check collisions
	this->chechCollisions();

	// Limit mario x movement 
	this->sprite.setPosition(MathUtils::clamp(this->sprite.getPosition().x, this->window->getView().getCenter().x - this->window->getSize().x / 2.f, this->window->getView().getCenter().x + this->window->getSize().x / 2.f - this->sprite.getGlobalBounds().width), this->sprite.getPosition().y);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::H)) {
		if (!wasPressed) {
			this->grow();
			std::cout << "Grow\n";
			wasPressed = true; 
		}
	}
	else {
		wasPressed = false; 
	}

	sf::Vector2f newPosition = this->sprite.getPosition();
	setPosition(newPosition);
}

void Mario::render(sf::RenderTarget* target)
{
	target->draw(this->sprite);

	/*sf::RectangleShape debugRect;
	debugRect.setPosition(sprite.getGlobalBounds().left, sprite.getGlobalBounds().top);
	debugRect.setSize({ sprite.getGlobalBounds().width, sprite.getGlobalBounds().height });
	debugRect.setFillColor(sf::Color::Transparent);
	debugRect.setOutlineColor(sf::Color::Red);
	debugRect.setOutlineThickness(1.f);
	target->draw(debugRect);*/
}
