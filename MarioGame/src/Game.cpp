#include "GameState.cpp"


//Init
void Game::initVariables()
{
	start_game_timer = 3.f;
	is_game_started = false;

	game_time = 10.f;
	
	last_camera_pos = { 0.f,0.f };
	lastTime = 0.f;

	is_game_over = false;
}

void Game::initTextureManager()
{
	//Init texture manager
	texture_manager = std::make_shared<TextureManager>();
	texture_manager->load("Mario", "assets/Textures/Mario/Mario.png"); //Small mario
	texture_manager->load("MarioBig", "assets/Textures/Mario/MarioBig.png"); //Big mario
	texture_manager->load("FireBall", "assets/Textures/FireBall.png"); //FireBall
	texture_manager->load("Explosion", "assets/Textures/Explosion.png"); //Explosion
	texture_manager->load("LuckyBlock", "assets/Textures/Levels/LuckyBlock.png"); //LuckyBlock
	texture_manager->load("Brick", "assets/Textures/Levels/Brick.png"); //Brick
	texture_manager->load("Mushroom", "assets/Textures/Levels/Mushroom.png"); //Mushroom
	texture_manager->load("FireFlower", "assets/Textures/Levels/FireFlower.png"); //FireFlower
	texture_manager->load("Coin", "assets/Textures/Levels/Coin_Anim.png"); //Coin
	texture_manager->load("Block", "assets/Textures/Levels/Block.png"); //Block
	texture_manager->load("FlagStick", "assets/Textures/Levels/Flag_Stick.png"); //FlagStick
	texture_manager->load("BrokenBrick", "assets/Textures/Levels/BrokenBrick.png"); //BrokenBrick
	texture_manager->load("Flag", "assets/Textures/Levels/Flag.png"); //Flag
	texture_manager->load("200S", "assets/Textures/Scores/200.png"); //200 score
	texture_manager->load("1000S", "assets/Textures/Scores/1000.png"); //1000 score
}

void Game::initWindow()
{
	//Init Window
	window = std::make_unique<sf::RenderWindow>(sf::VideoMode(800,600), "Mario nintendo game", sf::Style::Default); // 4:3
	
	// 800 600
	//window->setFramerateLimit(60);
	window->setVerticalSyncEnabled(false);

	sf::Image icon_image;
	icon_image.loadFromFile("assets/Textures/Game_Icon.png");
	window->setIcon(icon_image.getSize().x, icon_image.getSize().y, icon_image.getPixelsPtr());

	//Set View
	view = std::make_unique<sf::View>();
	view->setSize(800,600);
}

void Game::initMap()
{
	//Init TileMap
	map = std::make_unique<Map>(shared_from_this(), window.get(), col_manager, texture_manager, quadTree, gameObjects, CollisionEvent( flag->getBounds(), "Flag" ));
}

void Game::initCollisions()
{
	//Init QuadTree
	quadTree = std::make_shared<QuadTree>(sf::FloatRect(0.f, 0.f, 10550.f, 700.f), 4, 1000);
	//Init CollisionManager
	col_manager = std::make_shared<CollisionManager>(quadTree, window.get());
}

void Game::initAudio()
{
	//Init AudioManager
	game_audio_manager = std::make_unique<AudioManager>();

	//Main theme
	game_audio_manager->addSound("Main Theme", "assets/Sounds/Game/Ground Theme.wav", true);
	//game_audio_manager->playSound("Main Theme");
}

void Game::initText()
{
	//Init UI Text
	small_coin_texture.loadFromFile("assets/Textures/Levels/SmallCoin.png");
	small_coin_sprite.setTexture(small_coin_texture);
	small_coin_sprite.setScale(4.f, 4.f);
	small_coin_sprite.setTextureRect(sf::IntRect(0, 0, 8, 8));

	small_coin_anim = std::make_unique<Animator>();
	small_coin_anim->addFrameAnimation(
		small_coin_sprite, 8, 8, std::vector<int>{ 0, 1, 2 }, 400.f / 1000.f, [this]() {return true; }, [this]() {return 1; }, true, 5, "Small_Coin_Idle"
	);

	main_font.loadFromFile("assets/Fonts/SuperMario85.ttf");
	fps_text.setFont(main_font);
	fps_text.setCharacterSize(30);
	fps_text.setFillColor(sf::Color::White);

	score_text.setFont(main_font);
	score_text.setCharacterSize(30);
	score_text.setFillColor(sf::Color::White);
	score_text.setString("MARIO\n000000\n");

	coin_text.setFont(main_font);
	coin_text.setCharacterSize(30);
	coin_text.setFillColor(sf::Color::White);
	coin_text.setString(" x00\n");
	
	world_text.setFont(main_font);
	world_text.setCharacterSize(30);
	world_text.setFillColor(sf::Color::White);
	world_text.setString("WORLD\n 1-1\n");

	current_world_text.setFont(main_font);
	current_world_text.setCharacterSize(30);
	current_world_text.setFillColor(sf::Color::White);
	current_world_text.setString("WORLD 1-1\n");
	current_world_text.setPosition(window->getSize().x / 2.f - 150.f, window->getSize().y / 2.f / 30.f - 75.f);

	time_text.setFont(main_font);
	time_text.setCharacterSize(30);
	time_text.setFillColor(sf::Color::White);
	time_text.setString("TIME\n 300\n");

	lifes_amount_text.setFont(main_font);
	lifes_amount_text.setCharacterSize(30);
	lifes_amount_text.setFillColor(sf::Color::White);
	lifes_amount_text.setPosition(375.f, 25.f);

	game_over_text.setFont(main_font);
	game_over_text.setCharacterSize(30);
	game_over_text.setFillColor(sf::Color::White);
	game_over_text.setPosition(window->getSize().x / 2.f - 150, window->getSize().y / 2.f / 30.f - 50.f);
	game_over_text.setString("Game Over\n");

	time_up_text.setFont(main_font);
	time_up_text.setCharacterSize(30);
	time_up_text.setFillColor(sf::Color::White);
	time_up_text.setPosition(window->getSize().x / 2.f - 150, window->getSize().y / 2.f / 30.f - 50.f);
	time_up_text.setString("Time Up\n");

	mario_icon_texture = *texture_manager->get("Mario");
	mario_icon.setTexture(mario_icon_texture);
	mario_icon.setScale(3.3f,3.3f);
	mario_icon.setTextureRect(sf::IntRect(0, 0, 16, 16));
	mario_icon.setPosition(250.f,20.f);
}

void Game::initMario()
{
	//Init Mario
	mario = std::make_unique<Mario>(window.get(), map.get(), col_manager, texture_manager, view.get(), sf::FloatRect(0, 0, 48, 48), "Mario", 25, lifes);
}

void Game::initFlag()
{
	//Init Flag
	flag = std::make_unique<Flag>("Flag", sf::FloatRect((198.f * 16.f)  * 3.125f, 1.f * 16.f * 3.125f, 2.f * 3.125f, 16.f * 10.f * 3.125f), 15, texture_manager->get("FlagStick").get(), texture_manager->get("Flag").get());
	quadTree->insert({flag->getBounds(), "Flag"});
}

void Game::setState(const std::shared_ptr<IGameState>& state)
{
	if (current_state == state)
		return;

	if (current_state != nullptr) // OnExit
	{
		current_state->onExit(*this);
	}

	current_state = state; // set new state

	current_state->onEnter(*this); // OnEnter
}

//Con/Des
Game::Game() : lifes(3), score(0), coin_amount(0), lastTime(0.f), start_game_timer(0.f), is_game_over(false), is_game_started(false)
{
	
}

Game::~Game()
{
}

//Accessors
const bool Game::running() const
{
	return window->isOpen();
}

void Game::addScore(int score)
{
	this->score += score;
}

void Game::addCoin()
{
	coin_amount++;
}

void Game::showScore(sf::Vector2f pos, sf::Texture* texture, int score)
{
	//Display score
	std::shared_ptr<Text> text = std::make_shared<Text>(16,8, pos, texture);
	text->getAnimator()->playAnim("Score");
	addScore(score);
	scores_.push_back(text);
}

 // Uses for some particular situations
bool Game::init()
{
	initVariables();
	initCollisions();
	initTextureManager();
	initWindow();
	initMario();
	initFlag();
	initMap();
	initAudio();
	initText();

	setState(std::static_pointer_cast<IGameState>(std::make_shared<IGameShowInfo>()));

	return true;

	//placeholder
	//last_camera_pos = { 9451.f, 375.f };
}

//Functions
void Game::updateEvents()
{
	sf::Event event;
	window->pollEvent(event);

	if (event.type == sf::Event::EventType::Closed)
		window->close();
}

void Game::updateView()
{
	//Update view position regarding mario 
	last_camera_pos = MathUtils::lerp(last_camera_pos, { std::max(mario->getPosition().x, last_camera_pos.x) , 375.f }, 10 * mario->deltaTime);
	last_camera_pos.x = MathUtils::clamp(last_camera_pos.x, window->getSize().x / 2.f, 211.f * 16.f * 3.125f - window->getSize().x / 2.f);
	if(!(mario->getPosition().y - mario->getBounds().height > window->getSize().y))
		view->setCenter(last_camera_pos);
}

void Game::updateAudio()
{

}

void Game::updateText()
{
	//Update UI
	float currentTime = fps_clock.restart().asSeconds();
	float fps = 1.f / (currentTime - lastTime);

	std::stringstream ss;
	ss << "FPS: " << fps;
	
	fps_text.setString(ss.str());

	fps_text.setPosition(view->getCenter().x - window->getSize().x / 2.f + 50.f, view->getCenter().y - window->getSize().y / 2.f + 100.f);

	std::stringstream ss1;
	TClockEvent* clockEvent = tclock.getClock("GameLoop");
	if (clockEvent != nullptr)
	{
		ss1 << "TIME\n " << game_time - static_cast<int>(clockEvent->current_time) << "\n";
		time_text.setString(ss1.str());
	}

	std::ostringstream oss2;
	int size = static_cast<int>(MathUtils::getDigitCount(score));

	oss2 << "MARIO\n" << std::setw(size + (6 - size)) << std::setfill('0') << score << "\n";
	score_text.setString(oss2.str());

	std::ostringstream oss3;
	oss3 << " x" << std::setw(2) << std::setfill('0') << coin_amount << "\n";
	coin_text.setString(oss3.str());

	small_coin_sprite.setPosition(view->getCenter().x - window->getSize().x / 2.f + 280.f, view->getCenter().y - window->getSize().y / 2.f + 55.f);

	score_text.setPosition(view->getCenter().x - window->getSize().x / 2.f + 50.f, view->getCenter().y - window->getSize().y / 2.f + 20.f);
	coin_text.setPosition(view->getCenter().x - window->getSize().x / 2.f + 290.f, view->getCenter().y - window->getSize().y / 2.f + 55.f);
	world_text.setPosition(view->getCenter().x - window->getSize().x / 2.f + 450.f, view->getCenter().y - window->getSize().y / 2.f + 20.f);
	time_text.setPosition(view->getCenter().x - window->getSize().x / 2.f + 650.f, view->getCenter().y - window->getSize().y / 2.f + 20.f);

	std::ostringstream oss4;
	oss4 << "x   " << lifes << "\n";
	lifes_amount_text.setString(oss4.str());

	game_over_text.setPosition(view->getCenter().x - window->getSize().x / 2.f + 250.f, view->getCenter().y - window->getSize().y / 2.f + 275.f);
	time_up_text.setPosition(view->getCenter().x - window->getSize().x / 2.f + 300.f, view->getCenter().y - window->getSize().y / 2.f + 300.f);
}

void Game::updateMap()
{
	map->update(mario->deltaTime);
	//quadTree->insert({ flag->getBounds(), "Flag" });
}

void Game::updateCollisions(float deltaTime)
{
	//Check collision with different gameobjects
	for (const auto& obj : gameObjects)
	{
		if (mario->getBounds().intersects(obj->getBounds()))
		{
			if (obj->isActive())
			{
				obj->onHit();

				if (obj->getType() == "Mushroom")
					mario->grow();
				else if (obj->getType() == "FireFlower" && !mario->isFire() && mario->isBig())
					mario->fire();
				else if (obj->getType() == "FireFlower" && !mario->isFire() && !mario->isBig())
					mario->grow();

				//delete it
				auto it = std::find_if(gameObjects.begin(), gameObjects.end(),
					[obj](const std::shared_ptr<GameObject>& obj1) {
						return obj1 == obj;
					});

				if (it != gameObjects.end()) {
					gameObjects.erase(it);
				}
			}
		}
	}
}

void Game::update()
{
	float deltaTime = clock.restart().asSeconds();
	deltaTime = std::min(deltaTime, 0.033f);

	tclock.update(deltaTime);

	if (current_state != nullptr)
		current_state->onUpdate(*this, deltaTime);

	updateEvents();
	updateView();
	updateText();
}

void Game::renderLevel()
{
	
}

void Game::renderText()
{
	window->draw(fps_text);
	window->draw(score_text);
	window->draw(coin_text);
	window->draw(world_text);
	window->draw(time_text);

	window->draw(small_coin_sprite);
}

void Game::DisplayStartMenu()
{
	
}

void Game::restart()
{
	if (lifes > 1)
	{
		//Restart game
		initVariables();
		initCollisions();
		initTextureManager();
		initMario();
		initFlag();
		initMap();
		initAudio();
		initText();
		lifes--;

		setState(std::static_pointer_cast<IGameState>(std::make_shared<IGameShowInfo>()));
	}
	else
	{
		//Finish Game
		setState(std::static_pointer_cast<IGameState>(std::make_shared<IGameGameOver>()));
	}
}

void Game::render()
{
	window->clear(); 
	renderQueue.clear();

	current_state->onRender(*this);

	//Sort and render all the objects in the queue
	std::sort(renderQueue.begin(), renderQueue.end());
	for (const auto& i : renderQueue)
	{
		i.renderFunc();
	}

	renderText();
	window->setView(*view.get());
	window->display();
}