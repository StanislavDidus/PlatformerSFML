#pragma once

#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include <iostream>
#include <memory>
#include <cmath>
#include <ctime>

#include "Animator.h"
#include "Map.h"
#include "AudioManager.h"
#include "GameObject.h"
#include "Algorythms/QuadTree.h"
//#include "Manager/TextureManager.h"

class IMarioState;
class IMarioIdle;
class IMarioWalk;
class IMarioJump;
class IMarioCrouch;
class IMarioCollectFlag;
class IMarioRunToCastle;

struct uint3
{
	sf::Uint32 x, y, z;
	uint3(sf::Uint32 x, sf::Uint32 y, sf::Uint32 z) : x(x), y(y), z(z) {}
};

class Mario : public GameObject
{
private:
	sf::RenderWindow* window;
	sf::Clock clock;

	//Mario
	sf::Texture texture;
	//Big Mario
	sf::Texture texture1;
	//Fire Mario
	sf::Texture texture2;

	sf::Sprite sprite;
	sf::Texture tx_glitch;

	std::shared_ptr<QuadTree> quadTree;

	Map* map;

	CollisionManager* col;

	//Audio
	std::unique_ptr<AudioManager> mario_audio_manager;

	//States
	std::shared_ptr<IMarioState> current_state;
	std::shared_ptr<IMarioIdle> idle_state;
	std::shared_ptr<IMarioWalk> run_state;
	std::shared_ptr<IMarioJump> jump_state;

	sf::Vector2f velocity; // velocity.x , velocity.y
	sf::Vector2f max_velocity; // max velocity
	float speed;
	sf::Vector2f acceleration; // acceleration

	float jump_start_pos;
	float jump_start_max;
	float jump_start_height;

	int direction;

	//Grow
	float grow_time;
	float grow_timer;
	bool is_grown;

	//Fire
	float fire_time;
	float fire_timer;
	bool is_fire;
	bool fire_transform;

	//Slide
	float slide_time;
	float slide_time_max;
	bool is_sliding;

	//Jump
	bool is_jump_over;
	bool is_ground;
	
	float gravity;
	
	//Animation
	std::unique_ptr<Animator> animator;
	bool play_anim = true;

	void initVariables();
	void initSprite();
	void initState();
	void initAnimator();
	void initAudio();
public:
	bool is_touching_flag = false;
	bool is_dead;

	float deltaTime;
	friend class IMarioIdle;
	friend class IMarioWalk;
	friend class IMarioJump;
	friend class IMarioCrouch;
	friend class IMarioCollectFlag;
	friend class IMarioRunToCastle;

	Mario(sf::RenderWindow* window, Map* map, CollisionManager* col, std::shared_ptr<TextureManager> texture_manager, const sf::FloatRect& rect, const std::string& type, int layer);
	virtual ~Mario();

	//Accessors
	const sf::FloatRect getBounds() const;
	const sf::Vector2f getPosition() const;

	//Modifiers
	void setGround(bool state);
	void setPosition(const sf::Vector2f& newPosition) override;

	void Finish(float deltaTime);

	//Functions
	void move(float dirX, float dirY);
	void grow();
	void fire();
	void die();
	void checkSlide();
	void checkCollisions();
	void flip(int dir);
	void applyGravity(float deltaTime);
	void setState(const std::shared_ptr<IMarioState>& state);
	void updateCollision();
	void update(float deltaTime) override;
	void render(sf::RenderTarget* target) override;
};
