#include "Game.h"


//Init
void Game::initVariables()
{
	this->is_colliding = false;

	this->last_camera_pos = { 0.f,0.f };

	this->lastTime = 0.f;

	this->score = 0;
}

void Game::initWindow()
{
	this->window = std::make_unique<sf::RenderWindow>(sf::VideoMode(800,600), "Mario nintendo game", sf::Style::Default); // 4:3
	
	// 800 600
	//this->window->setFramerateLimit(60);
	this->window->setVerticalSyncEnabled(false);

	this->view = std::make_unique<sf::View>();
	this->view->setSize(800,600);
}

void Game::initMap()
{
	this->map = std::make_unique<Map>(this, this->window.get(), this->col_manager.get(), gameObjects);
}

void Game::initCollisions()
{
	this->col_manager = std::make_unique<CollisionManager>();
}

void Game::initAudio()
{
	this->game_audio_manager = std::make_unique<AudioManager>();

	//Main theme
	this->game_audio_manager->addSound("Main Theme", "assets/Sounds/Game/Ground Theme.wav", true);
	//this->game_audio_manager->playSound("Main Theme");
}

void Game::initText()
{
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
	this->mario = std::make_unique<Mario>(this->window.get(), this->map.get(), this->col_manager.get(), sf::FloatRect(0, 0, 50, 50), "Mario");

	this->col_manager->addSourse(dynamic_cast<GameObject*>(this->mario.get()));
}

//Con/Des
Game::Game()
{
	this->initVariables();
	this->initWindow();
	this->initCollisions();
	this->initMario();
	this->initMap();
	this->initAudio();
	this->initText();
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
	//sf::Vector2f newPos = sf::Vector2f(MathUtils::clamp(mario->getPosition().x, this->window->getSize().x / 2.f , 10150.f), 375.f);

	
	this->last_camera_pos = MathUtils::lerp(this->last_camera_pos, { std::max(this->mario->getPosition().x, this->last_camera_pos.x) , 375.f }, 10 * mario->deltaTime);
	//this->last_camera_pos.x = std::max(this->window->getSize().x / 2.f, this->last_camera_pos.x);
	//this->last_camera_pos.x = std::min(211 * 16 * 3.125f - this->window->getSize().x / 2.f, this->last_camera_pos.x);
	this->last_camera_pos.x = MathUtils::clamp(this->last_camera_pos.x, this->window->getSize().x / 2.f, 211.f * 16.f * 3.125f - this->window->getSize().x / 2.f);
	this->view->setCenter(this->last_camera_pos);
	
	//this->view->setCenter({this->mario->getPosition().x , 375.f});
	
	//std::cout << this->view->getCenter().x << ", " << this->view->getCenter().y << "\n";
	//this->view->setSize(800, 600);
	//this->view->setCenter(this->mario->getPosition());

	//
}

void Game::updateAudio()
{

}

void Game::updateText()
{
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

	this->score_text.setPosition(this->view->getCenter().x - this->window->getSize().x / 2.f + 50.f, this->view->getCenter().y - this->window->getSize().y / 2.f + 20.f);
	this->coin_text.setPosition(this->view->getCenter().x - this->window->getSize().x / 2.f + 250.f, this->view->getCenter().y - this->window->getSize().y / 2.f + 20.f);
	this->world_text.setPosition(this->view->getCenter().x - this->window->getSize().x / 2.f + 450.f, this->view->getCenter().y - this->window->getSize().y / 2.f + 20.f);
	this->time_text.setPosition(this->view->getCenter().x - this->window->getSize().x / 2.f + 650.f, this->view->getCenter().y - this->window->getSize().y / 2.f + 20.f);
}

void Game::updateMap()
{
	this->map->update(this->mario->deltaTime);
}

void Game::updateCollisions(float deltaTime)
{
	this->col_manager->update(deltaTime);
}

void Game::update()
{
	float deltaTime = this->clock.restart().asSeconds();
	deltaTime = std::min(deltaTime, 0.033f);
	
	this->updateEvents();
	this->updateCollisions(deltaTime);
	this->mario->update(deltaTime);

	for (const auto& object : gameObjects)
	{
		object->update(deltaTime);
	}

	this->updateView();
	this->updateAudio();
	this->updateText();
	this->updateMap();
}

void Game::renderLevel()
{
	this->map->render(this->window.get());
}

void Game::renderText()
{
	this->window->draw(this->fps_text);
	this->window->draw(this->score_text);
	this->window->draw(this->coin_text);
	this->window->draw(this->world_text);
	this->window->draw(this->time_text);
}

void Game::render()
{
	this->window->clear();

	//Render level
	for (const auto& object : gameObjects)
	{
		object->render(window.get());
	}

	this->renderLevel();

	//Render player
	this->mario->render(this->window.get());

	this->renderText();

	this->window->setView(*this->view.get());
	
	this->window->display();
}