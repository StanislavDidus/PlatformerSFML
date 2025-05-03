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
#include "Renderable.h"
#include "CollisionManager.h"
#include "GameObject.h"
#include "Text.h"
#include "Manager/TextureManager.h"
#include "Algorythms/QuadTree.h"
#include "Objects/Flag.h"

#include "Math.h"

class Game : public std::enable_shared_from_this<Game>
{
private:
	//Start game screen
	float start_game_timer;
	bool is_game_started;
	sf::Texture mario_icon_texture;
	sf::Sprite mario_icon;

	//Window
	std::unique_ptr<sf::RenderWindow> window;
	std::unique_ptr<sf::View> view;
	//view
	sf::Vector2f last_camera_pos;

	//Texture Manager
	std::shared_ptr<TextureManager> texture_manager;

	//Audio
	std::unique_ptr<AudioManager> game_audio_manager;
	
	//Timers
	sf::Clock clock;
	float timer;
	float ttimer;

	//Items
	std::vector<std::shared_ptr<GameObject>> gameObjects; // All items (coins,mushroonms,stars)

	//Mario
	std::unique_ptr<Mario> mario;
	bool is_colliding;

	//Collisions
	std::shared_ptr<CollisionManager> col_manager;
	std::shared_ptr<QuadTree> quadTree;

	//Render
	std::vector<Renderable> renderQueue;

	//Level
	std::unique_ptr<Map> map;

	//Flag
	std::unique_ptr<Flag> flag;

	//Sprites
	//sf::Texture flag_texture;
	//sf::Sprite flag_sprite;


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
	int coin_amount;
	
	//Frame rate counter
	sf::Clock fps_clock;
	float lastTime;


	void initVariables();
	void initTextureManager();
	void initWindow();
	void initMario();
	void initFlag();
	void initMap();
	void initCollisions();
	void initAudio();
	void initText();
public:
	

	Game();
	virtual ~Game();

	const bool running() const;

	void addScore(int score);
	void addCoin();
	void showScore(sf::Vector2f pos, sf::Texture* texture, int score);
	void DisplayStartMenu();

	bool init();
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

