#include "Game.h"


//Init
void Game::initVariables()
{
	start_game_timer = 3.f;
	is_game_started = false;

	//timers
	timer = 400.f;
	ttimer = 0.f;
	
	is_colliding = false;
	last_camera_pos = { 0.f,0.f };
	lastTime = 0.f;
	score = 0;
	coin_amount = 0;
}

void Game::initTextureManager()
{
	this->texture_manager = std::make_shared<TextureManager>();
	this->texture_manager->load("Mario", "assets/Textures/Mario/Mario0.png");
	this->texture_manager->load("MarioBig", "assets/Textures/Mario/Mario1.png");
	this->texture_manager->load("MarioFire", "assets/Textures/Mario/Mario2.png");
	this->texture_manager->load("LuckyBlock", "assets/Textures/Levels/LuckyBlock.png");
	this->texture_manager->load("Brick", "assets/Textures/Levels/Brick.png");
	this->texture_manager->load("Mushroom", "assets/Textures/Levels/Mushroom.png");
	this->texture_manager->load("FireFlower", "assets/Textures/Levels/FireFlower.png");
	this->texture_manager->load("Coin", "assets/Textures/Levels/Coin_Anim.png");
	this->texture_manager->load("Block", "assets/Textures/Levels/Block.png");
	this->texture_manager->load("FlagStick", "assets/Textures/Levels/Flag_Stick.png");
	this->texture_manager->load("BrokenBrick", "assets/Textures/Levels/BrokenBrick.png");
	this->texture_manager->load("Flag", "assets/Textures/Levels/Flag.png");
	this->texture_manager->load("200S", "assets/Textures/Scores/200.png");
	this->texture_manager->load("1000S", "assets/Textures/Scores/1000.png");	
}

void Game::initWindow()
{
	//Init Window
	this->window = std::make_unique<sf::RenderWindow>(sf::VideoMode(800,600), "Mario nintendo game", sf::Style::Default); // 4:3
	
	// 800 600
	//this->window->setFramerateLimit(60);
	this->window->setVerticalSyncEnabled(false);

	//Set View
	this->view = std::make_unique<sf::View>();
	this->view->setSize(800,600);
}

void Game::initMap()
{
	//Init TileMap
	this->map = std::make_unique<Map>(shared_from_this(), this->window.get(), this->col_manager, this->texture_manager, quadTree, gameObjects, CollisionEvent( flag->getBounds(), "Flag" ));
}

void Game::initCollisions()
{
	//Init QuadTree
	quadTree = std::make_shared<QuadTree>(sf::FloatRect(0.f, 0.f, 10550.f, 700.f), 4, 1000);
	//Init CollisionManager
	this->col_manager = std::make_shared<CollisionManager>(quadTree, window.get());
}

void Game::initAudio()
{
	//Init AudioManager
	this->game_audio_manager = std::make_unique<AudioManager>();

	//Main theme
	this->game_audio_manager->addSound("Main Theme", "assets/Sounds/Game/Ground Theme.wav", true);
	//this->game_audio_manager->playSound("Main Theme");
}

void Game::initText()
{
	//Init UI Text
	this->small_coin_texture.loadFromFile("assets/Textures/Levels/SmallCoin.png");
	this->small_coin_sprite.setTexture(this->small_coin_texture);
	this->small_coin_sprite.setScale(4.f, 4.f);
	this->small_coin_sprite.setTextureRect(sf::IntRect(0, 0, 8, 8));

	this->small_coin_anim = std::make_unique<Animator>();
	this->small_coin_anim->addFrameAnimation(
		this->small_coin_sprite, 8, 8, std::vector<int>{ 0, 1, 2 }, 400.f / 1000.f, [this]() {return true; }, [this]() {return 1; }, true, 5, "Small_Coin_Idle"
	);

	this->main_font.loadFromFile("assets/Fonts/SuperMario85.ttf");
	this->fps_text.setFont(this->main_font);
	this->fps_text.setCharacterSize(30);
	this->fps_text.setFillColor(sf::Color::White);

	this->score_text.setFont(this->main_font);
	this->score_text.setCharacterSize(30);
	this->score_text.setFillColor(sf::Color::White);
	this->score_text.setString("MARIO\n000000\n");

	this->coin_text.setFont(this->main_font);
	this->coin_text.setCharacterSize(30);
	this->coin_text.setFillColor(sf::Color::White);
	this->coin_text.setString(" x00\n");
	
	this->world_text.setFont(this->main_font);
	this->world_text.setCharacterSize(30);
	this->world_text.setFillColor(sf::Color::White);
	this->world_text.setString("WORLD\n 1-1\n");

	this->time_text.setFont(this->main_font);
	this->time_text.setCharacterSize(30);
	this->time_text.setFillColor(sf::Color::White);
	this->time_text.setString("TIME\n 300\n");
}

void Game::initMario()
{
	//Init Mario
	this->mario = std::make_unique<Mario>(this->window.get(), this->map.get(), this->col_manager.get(), texture_manager, sf::FloatRect(0, 0, 48, 48), "Mario", 25);
}

void Game::initFlag()
{
	//Init Flag
	flag = std::make_unique<Flag>("Flag", sf::FloatRect(198.f * 16.f * 3.125f, 1.f * 16.f * 3.125f, 16.f * 3.125f, 16.f * 10.f * 3.125f), 15, texture_manager->get("FlagStick").get(), texture_manager->get("Flag").get());
	quadTree->insert({flag->getBounds(), "Flag"});
	//std::cout << flag->getBounds().left << ", " << flag->getBounds().top << "\n";
}

//Con/Des
Game::Game()
{
	
}

Game::~Game()
{
}

//Accessors
const bool Game::running() const
{
	return this->window->isOpen();
}

void Game::addScore(int score)
{
	this->score += score;
}

void Game::addCoin()
{
	coin_amount++;
}

void Game::showScore(sf::Vector2f pos, sf::Texture* texture)
{
	//Display scores
	std::shared_ptr<Text> text = std::make_shared<Text>(16,8, pos, texture);
	text->getAnimator()->playAnim("Score");
	this->addScore(1000);
	scores_.push_back(text);
}

 // Uses for some particular situations
void Game::init()
{
	this->initVariables();
	this->initWindow();
	this->initCollisions();
	this->initTextureManager();
	this->initMario();
	this->initFlag();
	this->initMap();
	this->initAudio();
	this->initText();

	//placeholder
	//this->last_camera_pos = { 9451.f, 375.f };
}

//Functions
void Game::updateEvents()
{
	sf::Event event;
	this->window->pollEvent(event);

	if (event.type == sf::Event::EventType::Closed)
		this->window->close();
}

void Game::updateView()
{
	//Update view position regarding mario 
	this->last_camera_pos = MathUtils::lerp(this->last_camera_pos, { std::max(this->mario->getPosition().x, this->last_camera_pos.x) , 375.f }, 10 * mario->deltaTime);
	this->last_camera_pos.x = MathUtils::clamp(this->last_camera_pos.x, this->window->getSize().x / 2.f, 211.f * 16.f * 3.125f - this->window->getSize().x / 2.f);
	if(!(mario->getPosition().y - mario->getBounds().height > window->getSize().y))
		this->view->setCenter(this->last_camera_pos);
}

void Game::updateAudio()
{

}

void Game::updateText()
{
	//Update UI
	float currentTime = this->fps_clock.restart().asSeconds();
	float fps = 1.f / (currentTime - this->lastTime);

	std::stringstream ss;
	ss << "FPS: " << fps;
	
	this->fps_text.setString(ss.str());

	this->fps_text.setPosition(this->view->getCenter().x - this->window->getSize().x / 2.f + 50.f, this->view->getCenter().y - this->window->getSize().y / 2.f + 100.f);

	std::stringstream ss1;
	ss1 << "TIME\n " << this->timer << "\n";
	this->ttimer += this->mario->deltaTime;
	if (this->ttimer >= 1.f)
	{
		this->ttimer = 0.f;
		this->timer -= 1.f;
		if (this->timer < 0.f)
			this->timer = 0.f;
	}
	this->time_text.setString(ss1.str());

	std::ostringstream oss2;
	int size = MathUtils::getDigitCount(this->score);

	oss2 << "MARIO\n" << std::setw(size + (6 - size)) << std::setfill('0') << this->score << "\n";
	this->score_text.setString(oss2.str());

	std::ostringstream oss3;
	oss3 << " x" << std::setw(2) << std::setfill('0') << coin_amount << "\n";
	this->coin_text.setString(oss3.str());

	this->small_coin_sprite.setPosition(this->view->getCenter().x - this->window->getSize().x / 2.f + 280.f, this->view->getCenter().y - this->window->getSize().y / 2.f + 55.f);

	this->score_text.setPosition(this->view->getCenter().x - this->window->getSize().x / 2.f + 50.f, this->view->getCenter().y - this->window->getSize().y / 2.f + 20.f);
	this->coin_text.setPosition(this->view->getCenter().x - this->window->getSize().x / 2.f + 290.f, this->view->getCenter().y - this->window->getSize().y / 2.f + 55.f);
	this->world_text.setPosition(this->view->getCenter().x - this->window->getSize().x / 2.f + 450.f, this->view->getCenter().y - this->window->getSize().y / 2.f + 20.f);
	this->time_text.setPosition(this->view->getCenter().x - this->window->getSize().x / 2.f + 650.f, this->view->getCenter().y - this->window->getSize().y / 2.f + 20.f);
}

void Game::updateMap()
{
	this->map->update(this->mario->deltaTime);
	//quadTree->insert({ flag->getBounds(), "Flag" });
}

void Game::updateCollisions(float deltaTime)
{
	//this->map->updateCollisions();

	//Check collision with different gameobjects
	for (const auto& obj : gameObjects)
	{
		//this->col_manager->addCollision({ obj->getBounds(), obj->getType(), obj.get() });
		if (mario->getBounds().intersects(obj->getBounds()))
		{
			//std::cout << "Collect: " << obj->getType() << "\n";
			if (obj->isActive())
			{
				obj->onHit();

				if (obj->getType() == "Mushroom")
					this->mario->grow();
				else if (obj->getType() == "FireFlower")
					this->mario->fire();

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
	float deltaTime = this->clock.restart().asSeconds();
	deltaTime = std::min(deltaTime, 0.033f);

	//START MENU
	if (start_game_timer < 0.f)
		is_game_started = true;
	else
		this->start_game_timer -= deltaTime;

	this->updateEvents();

	//If game is started update all needed managers
	if (is_game_started)
	{
		this->updateCollisions(deltaTime);
		this->mario->update(deltaTime);

		for (const auto& object : gameObjects)
		{
			object->update(deltaTime);
		}

		this->small_coin_anim->update(deltaTime);

		//Update flag
		flag->update(deltaTime);
		//Final CutScene
		if (mario->is_touching_flag)
		{
			flag->Touch(deltaTime);
		}
		if (flag->is_finished)
		{
			mario->Finish(deltaTime);
		}
		//
		
		this->updateAudio();
		
		this->updateMap();

		for (const auto& score : scores_)
			score->getAnimator()->update(deltaTime);
	}
	

	this->updateView();
	this->updateText();
}

void Game::renderLevel()
{
	
}

void Game::renderText()
{
	this->window->draw(this->fps_text);
	this->window->draw(this->score_text);
	this->window->draw(this->coin_text);
	this->window->draw(this->world_text);
	this->window->draw(this->time_text);

	this->window->draw(this->small_coin_sprite);
}

void Game::DisplayStartMenu()
{

}

void Game::render()
{
	this->window->clear(); 
	renderQueue.clear();

	if (is_game_started)
	{

		//Render level
		for (const auto& object : gameObjects)
		{
			//object->render(window.get());
			renderQueue.emplace_back(object->layer, [this, object]() {object->render(this->window.get()); });
		}

		//this->map->render(this->window.get());
		this->map->render(this->renderQueue, this->window.get());

		renderQueue.emplace_back(flag->layer, [this]() {this->flag->render(window.get()); });

		//Render player
		renderQueue.emplace_back(mario->layer, [this]() {this->mario->render(this->window.get()); });

		//Render all the objects in queue
		std::sort(renderQueue.begin(), renderQueue.end());
		for (const auto& i : renderQueue)
		{
			i.renderFunc();
		}

		//Render UI
		for (const auto& score : scores_)
		{
			if (score->getAnimation() != nullptr)
				if (score->getAnimation()->is_playing)
				{
					score->render(this->window.get());
				}

		}
	}
	else
	{
		this->DisplayStartMenu();
	}
	this->renderText();
	this->window->setView(*this->view.get());
	this->window->display();
}