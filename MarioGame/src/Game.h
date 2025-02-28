#pragma once

#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include <sstream>
#include <cmath>
#include <iomanip>

#include "Mario.h"
#include "AudioManager.h"
#include "Map.h"
#include "CollisionManager.h"
#include "GameObject.h"

#include "Math.h"

class Game
{
private:
	std::unique_ptr<sf::RenderWindow> window;
	std::unique_ptr<sf::View> view;

	sf::Vector2f last_camera_pos;

	//Audio
	std::unique_ptr<AudioManager> game_audio_manager;
	
	sf::Clock clock;
	float timer = 300.f;
	float ttimer = 0.f;

	//Mario
	std::unique_ptr<Mario> mario;
	bool is_colliding;

	//Collisions
	std::unique_ptr<CollisionManager> col_manager;

	//Level
	std::unique_ptr<Map> map;

	//Text
	sf::Font main_font;
	//Fps
	sf::Text fps_text;
	//Mario score
	sf::Text score_text;
	//Coins
	sf::Text coin_text;
	//World
	sf::Text world_text;
	//Time
	sf::Text time_text;

	//Score
	int score;
	
	//Frame rate counter
	sf::Clock fps_clock;
	float lastTime;


	void initVariables();
	void initWindow();
	void initMario();
	void initMap();
	void initCollisions();
	void initAudio();
	void initText();
public:
	Game();
	virtual ~Game();

	const bool running() const;

	void addScore(int score);

	void updateEvents();
	void updateView();
	void updateAudio();
	void updateText();
	void updateMap();
	void updateCollisions(float deltaTime);
	void update();
	void renderLevel();
	void renderText();
	void render();
};

