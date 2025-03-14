#pragma once

#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include <sstream>
#include <cmath>
#include <iomanip>
#include <memory>

#include "Mario.h"
#include "AudioManager.h"
#include "Map.h"
#include "CollisionManager.h"
#include "GameObject.h"
#include "Text.h"

#include "Math.h"

class Game : public std::enable_shared_from_this<Game>
{
private:
	

	std::unique_ptr<sf::RenderWindow> window;
	std::unique_ptr<sf::View> view;

	sf::Vector2f last_camera_pos;

	//Audio
	std::unique_ptr<AudioManager> game_audio_manager;
	
	sf::Clock clock;
	float timer = 400.f;
	float ttimer = 0.f;

	//Items
	std::vector<std::shared_ptr<GameObject>> gameObjects; // All items (coins,mushroonms,stars)

	//Mario
	std::unique_ptr<Mario> mario;
	bool is_colliding;

	//Collisions
	std::shared_ptr<CollisionManager> col_manager;

	//Level
	std::unique_ptr<Map> map;

	//UI
	sf::Texture small_coin_texture;
	sf::Sprite small_coin_sprite;
	std::unique_ptr<Animator> small_coin_anim;

	std::vector<std::shared_ptr<Text>> scores_;

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
	void showScore(sf::Vector2f pos, const std::string& path);

	void init();
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

