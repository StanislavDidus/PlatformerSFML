#pragma once

#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include <iostream>
#include <memory>
#include <cmath>
#include <ctime>

#include "Managers/Animator.h"
#include "Algorythms/TClock.h"
#include "Managers/Map.h"
#include "Managers/AudioManager.h"
#include "GameObject.h"
#include "Algorythms/QuadTree.h"
#include "Objects/FireBall.h"
#include "Managers/EventBus.h"
//#include "Manager/TextureManager.h"

class IPlayerState;
class IPlayerIdle;
class IPlayerWalk;
class IPlayerJump;
class IPlayerCrouch;
class IPlayerShoot;
class IPlayerDie;
class IPlayerCollectFlag;
class IPlayerWaitToRun;
class IPlayerRunToCastle;

struct uint3
{
	sf::Uint32 x, y, z;
	uint3(sf::Uint32 x, sf::Uint32 y, sf::Uint32 z) : x(x), y(y), z(z) {}
};

enum PlayerState
{
	SMALL, GROWING, BIG, FIRING, FIRE
};

class Player : public GameObject
{
private:
	sf::RenderWindow* window;
	sf::View* view;
	sf::Clock clock;

	TClock t_clock;

	std::shared_ptr<TextureManager> texture_manager;
	//Player
	sf::Texture texture;
	//Big Player
	sf::Texture texture1;
	//Fire Player

	sf::Sprite sprite;
	sf::Texture tx_glitch;

	std::vector<std::shared_ptr<FireBall>> fireBalls;

	std::shared_ptr<QuadTree> quadTree;

	Map* map;

	std::shared_ptr<CollisionManager> col;

	//Audio
	std::unique_ptr<AudioManager> Player_audio_manager;

	//States
	std::shared_ptr<IPlayerState> last_state;
	std::shared_ptr<IPlayerState> current_state;

	sf::Vector2f velocity; // velocity.x , velocity.y
	sf::Vector2f max_velocity; // max velocity
	float speed;
	sf::Vector2f acceleration; // acceleration

	float jump_start_pos;
	float jump_start_max;
	float jump_start_height;

	int direction;

	TClock tclock;

	PlayerState current_Player_state;

	//Shoot
	float shoot_time;
	float shoot_timer;
	bool wasShootPressedLastFrame;

	//Slide
	float slide_time;
	float slide_time_max;
	bool is_sliding;

	bool is_falling;

	//Jump
	bool is_jump_over;
	bool is_ground;
	
	float gravity;

	//Lifes
	int lifes;
	float dead_timer;
	float dead_time;
	
	//Animation
	std::unique_ptr<Animator> animator;
	bool play_anim;

	void initVariables();
	void initSprite();
	void initState();
	void initAnimator();
	void initAudio();
public:
	bool is_touching_flag;
	bool is_dead;
	bool need_restart;

	float deltaTime;
	friend class IPlayerIdle;
	friend class IPlayerWalk;
	friend class IPlayerJump;
	friend class IPlayerCrouch;
	friend class IPlayerShoot;
	friend class IPlayerDie;
	friend class IPlayerCollectFlag;
	friend class IPlayerWaitToRun;
	friend class IPlayerRunToCastle;

	Player(sf::RenderWindow* window, Map* map, std::shared_ptr<CollisionManager> col, std::shared_ptr<TextureManager> texture_manager, sf::View* view, const sf::FloatRect& rect, const std::string& type, int layer, int lifes);
	virtual ~Player();

	//Accessors
	const sf::FloatRect getBounds() const;
	const sf::Vector2f getPosition() const;

	//Modifiers
	void setGround(bool state);
	void setPosition(const sf::Vector2f& newPosition) override;

	const bool isBig() const { return current_Player_state == PlayerState::BIG; }
	const bool isFire() const { return current_Player_state == PlayerState::FIRE; }

	Animator* getAnimator() { return animator.get(); }

	const int getLifes() const; //Return the amount of Player's lifes

	void Finish();

	//Functions
	void move(float dirX, float dirY);
	void grow();
	void fire();
	void shoot();
	void die();
	void timeUp();
	void checkSlide();
	void checkCollisions();
	void flip(int dir);
	void applyGravity(float deltaTime);
	void setState(const std::shared_ptr<IPlayerState>& state);
	void setLastState();
	void updateCollision();
	void updateFireBalls(float deltaTime);
	void update(float deltaTime) override;
	void render(sf::RenderTarget* target) override;
};
