#include "Game.h"


//Init
void Game::initVariables()
{
	this->is_colliding = false;

	this->last_camera_pos = { 0.f,0.f };

	this->lastTime = 0.f;
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
	this->map = std::make_unique<Map>(this->window.get(), this->col_manager.get());
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
	this->fps_text.setCharacterSize(40);
	this->fps_text.setFillColor(sf::Color::White);
}

void Game::initMario()
{
	this->mario = std::make_unique<Mario>(this->window.get(), this->map.get(), this->col_manager.get());

	this->col_manager->addSourse(this->mario->getBounds());
}

//Con/Des
Game::Game()
{
	this->initVariables();
	this->initWindow();
	this->initCollisions();
	this->initMap();
	this->initMario();
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
	sf::Vector2f newPos = sf::Vector2f(MathUtils::clamp(mario->getPosition().x, this->window->getSize().x / 2.f , 5000.f), 325.f);
	this->last_camera_pos = MathUtils::lerp(this->last_camera_pos, newPos, 10 * mario->deltaTime);
	//this->view->setCenter({this->mario->getPosition().x, 325.f});
	this->view->setCenter(this->last_camera_pos);
	
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

	this->fps_text.setPosition(this->view->getCenter().x - this->window->getSize().x / 2.f, this->view->getCenter().y - this->window->getSize().y / 2.f);
}

void Game::updateMap()
{
	this->map->update(this->mario->deltaTime);
}

void Game::update()
{
	float deltaTime = this->clock.restart().asSeconds();
	deltaTime = std::min(deltaTime, 0.033f);
	
	this->updateEvents();
	this->mario->update(deltaTime);
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
}

void Game::render()
{
	this->window->clear();

	//Render level
	this->renderLevel();

	//Render player
	this->mario->render(this->window.get());

	this->renderText();

	this->window->setView(*this->view.get());
	
	this->window->display();
}