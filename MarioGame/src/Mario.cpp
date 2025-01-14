#include "MarioState.h"

//Init
void Mario::initVariables()
{
	this->velocity = { 0.f, 0.f };
	this->max_velocity = { 1200.f, 400.f};
	this->acceleration = { 300.f, 1.f };

	this->gravity = 980.f;

	this->is_jump_over = false;
	this->is_ground = false;

	//Aninimation
	this->direction = 1;

	this->jump_start_pos = 0.f;
	this->jump_start_max = 150.f;
}

void Mario::initSprite()
{
	if (!this->texture.loadFromFile("assets/Textures/Mario/Mario0.png"))
		std::cout << "Could not load mario0 texture\n";

	this->sprite.setTexture(this->texture);
	this->sprite.setScale(3.125f, 3.125f);
	this->sprite.setPosition(16.f * 3.125f * 2.f, 0.f);

	this->sprite.setTextureRect(sf::IntRect(0, 0, 16, 16)); // Int rect (x, y, width, height)
	//this->sprite.setTextureRect(sf::IntRect(32, 0, -16, 16));
}

void Mario::initState()
{
	this->setState(std::make_shared<IMarioIdle>());
}

void Mario::initAnimator()
{
	//Create animator
	this->animator = std::make_unique<Animator>(
	this->sprite,
	static_cast<int>(this->sprite.getLocalBounds().width),
	static_cast<int>(this->sprite.getLocalBounds().height)
	);

	//Add animations
	//Idle
	this->animator->addAnimation(
		0, 0, 50.f / 1000.f, [this]() {return std::dynamic_pointer_cast<IMarioIdle>(this->current_state) != nullptr; }, [this]() {return this->direction; }, true
	);
	//Run
	this->animator->addAnimation(
		1, 3, 50.f / 1000.f, [this]() {return std::dynamic_pointer_cast<IMarioWalk>(this->current_state) != nullptr;  }, [this]() {return this->direction; }, true
	);
	//Jump
	this->animator->addAnimation(
		4, 5, 50.f / 1000.f, [this]() {return std::dynamic_pointer_cast<IMarioJump>(this->current_state) != nullptr;  }, [this]() {return this->direction; }, false
	);
}

void Mario::initAudio()
{
	this->mario_audio_manager = std::make_unique<AudioManager>();

	this->mario_audio_manager->addSound("Jump", "assets/Sounds/Mario/Jump.wav", false);
}

//Con/Des
Mario::Mario(sf::RenderWindow* window, Map* map, CollisionManager* col) : window(window), map(map), col(col)
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

void Mario::setPosition(const sf::Vector2f& pos)
{
	this->sprite.setPosition(pos);
}

//Functions
void Mario::move(float dirX, float dirY)
{	
	this->velocity.x += dirX * this->acceleration.x * deltaTime;

	if (this->velocity.y > this->max_velocity.y)
		this->velocity.y = this->max_velocity.y;
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
	if(this->current_state != nullptr) // OnExit
		this->current_state->onExit(*this);
	
	this->current_state = state; // set new state

	this->current_state->onEnter(*this); // OnEnter
}

void Mario::updateCollision()
{

}

void Mario::update(float deltaTime)
{
	this->deltaTime = deltaTime;
	
	if (this->current_state != nullptr) // Update current state
		this->current_state->onUpdate(*this, deltaTime);
	
	if (this->animator != nullptr)
		this->animator->update(this->deltaTime);
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
