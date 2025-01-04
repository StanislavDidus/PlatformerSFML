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

class IMarioState;
class IMarioIdle;
class IMarioWalk;
class IMarioJump;

class Mario
{
private:
	sf::RenderWindow* window;
	sf::Clock clock;

	sf::Texture texture;
	sf::Sprite sprite;

	Map* map;

	//Audio
	std::unique_ptr<AudioManager> mario_audio_manager;

	//States
	std::shared_ptr<IMarioState> current_state;
	std::shared_ptr<IMarioIdle> idle_state;
	std::shared_ptr<IMarioWalk> run_state;
	std::shared_ptr<IMarioJump> jump_state;

	sf::Vector2f velocity; // velocity.x , velocity.y
	sf::Vector2f max_velocity; // max velocity
	sf::Vector2f acceleration; // acceleration

	float jump_start_pos;
	float jump_start_max;

	int direction;

	

	//Jump
	bool is_jump_over;
	bool is_ground;
	
	float gravity;
	
	//Animation
	std::unique_ptr<Animator> animator;

	void initVariables();
	void initSprite();
	void initState();
	void initAnimator();
	void initAudio();
public:

	float deltaTime;
	friend class IMarioIdle;
	friend class IMarioWalk;
	friend class IMarioJump;

	Mario(sf::RenderWindow* window, Map* map);
	virtual ~Mario();

	//Accessors
	const sf::FloatRect getBounds() const;
	const sf::Vector2f getPosition() const;

	//Modifiers
	void setGround(bool state);
	void setPosition(const sf::Vector2f& pos);

	//Functions
	void move(float dirX, float dirY);
	void flip(int dir);
	void applyGravity(float deltaTime);
	void setState(const std::shared_ptr<IMarioState>& state);
	void update(float deltaTime);
	void render(sf::RenderTarget* target);
};
