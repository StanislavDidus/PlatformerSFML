#pragma once

#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include <sstream>
#include <cmath>
#include <iomanip>
#include <memory>

#include "Objects/Player.h"
#include "Managers/AudioManager.h"
#include "Managers/Animator.h"
#include "Managers/Map.h"
#include "Renderable.h"
#include "Managers/CollisionManager.h"
#include "Objects/GameObject.h"
#include "Objects/Text.h"
#include "Managers/TextureManager.h"
#include "Algorythms/QuadTree.h"
#include "Objects/Flag.h"
#include "Managers/EventBus.h"

#include "Math.h"

class IGameState;
class IGameShowInfo;
class IGameStartMenu;
class IGameGameOver;
class IGameFinish;
class IGameTimeUp;
class IGamePlay;

class Game : public std::enable_shared_from_this<Game>
{
private:
	std::shared_ptr<IGameState> current_state;

	//Start game screen
	float start_game_timer;
	bool is_game_started;
	sf::Texture Player_icon_texture;
	sf::Sprite Player_icon;
	sf::Text current_world_text;
	sf::Text lifes_amount_text;

	//Window and view
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

	TClock tclock;
	float game_time;

	bool restart_next_frame;

	//Items
	std::vector<std::shared_ptr<GameObject>> gameObjects; // All items (coins,mushroonms,stars)

	//Player
	std::unique_ptr<Player> player;
	int lifes;

	bool is_game_over;

	//Collisions
	std::shared_ptr<CollisionManager> col_manager;
	std::shared_ptr<QuadTree> quadTree;

	//Render
	std::vector<Renderable> renderQueue;

	//Level
	std::unique_ptr<Map> map;

	//Flag
	std::unique_ptr<Flag> flag;

	sf::Sprite flagStar_sprite;
	std::unique_ptr<Animator> flagStar_animator;

	//UI
	sf::Texture small_coin_texture;
	sf::Sprite small_coin_sprite;
	std::unique_ptr<Animator> small_coin_anim;

	std::vector<std::shared_ptr<Text>> scores_;

	//Text
	sf::Font main_font;
	//Fps
	sf::Text fps_text;
	//Player score
	sf::Text score_text;
	//Coins
	sf::Text coin_text;
	//World
	sf::Text world_text;
	//Time
	sf::Text time_text;
	//Game Over
	sf::Text game_over_text;
	//Time Up
	sf::Text time_up_text;

	//Score
	float score;
	int coin_amount;
	
	//Frame rate counter
	sf::Clock fps_clock;
	float lastTime;

	bool is_level_clear;


	void initVariables();
	void initTextureManager();
	void initWindow();
	void initPlayer();
	void initFlag();
	void initMap();
	void initCollisions();
	void initAudio();
	void initText();
	void initEventBus();
public:
	friend class IGameShowInfo;
	friend class IGameStartMenu;
	friend class IGameGameOver;
	friend class IGameTimeUp;
	friend class IGameFinish;
	friend class IGamePlay;

	Game();
	virtual ~Game();

	const bool running() const;

	void addScore(float score);
	void addCoin();
	void showScore(sf::Vector2f pos, sf::Texture* texture, int score); //Display the number of points you gained with an animation
	void DisplayStartMenu();
	void restart();

	void OnPlayerDeath();
	void OnPlayerTouchFlag();

	void setState(const std::shared_ptr<IGameState>& state);

	bool init();
	void updateEvents();
	void updateView();
	void updateText();
	void updateMap(); //Update tile maps and gameObjects related to it
	void updateCollisions(float deltaTime);
	void update();
	void renderLevel();
	void renderText();
	void render();
};

