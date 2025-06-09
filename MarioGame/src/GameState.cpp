#include "Game.h"

class IGameState
{
public:
	virtual ~IGameState() = default;

	virtual void onEnter(Game& game) = 0;
	virtual void onUpdate(Game& game, float deltaTime) = 0;
	virtual void onRender(Game& game) = 0;
	virtual void onExit(Game& game) = 0;
};

class IGameShowInfo : public IGameState
{
	void onEnter(Game& game) override
	{
		game.tclock.clear();
		game.tclock.addClock(3.f, [this, &game]() { game.setState(std::static_pointer_cast<IGameState>(std::make_shared<IGamePlay>())); }, "Start");
	}

	void onUpdate(Game& game, float deltaTime) override
	{

	}

	void onRender(Game& game) override
	{
		game.window->draw(game.current_world_text);
		game.window->draw(game.mario_icon);
		game.window->draw(game.lifes_amount_text);
	}

	void onExit(Game& game) override
	{

	}
};

class IGameStartMenu : public IGameState
{
	void onEnter(Game& game) override
	{

	}

	void onUpdate(Game& game, float deltaTime) override
	{

	}

	void onRender(Game& game) override
	{

	}

	void onExit(Game& game) override
	{

	}
};

class IGameGameOver : public IGameState
{
	void onEnter(Game& game) override
	{
		game.tclock.addClock(5.f, [this, &game]() {game.window->close(); }, "Close");
		game.game_audio_manager->stop();
		game.game_audio_manager->playSound("GameOver");
	}

	void onUpdate(Game& game, float deltaTime) override
	{

	}

	void onRender(Game& game) override
	{
		game.window->draw(game.game_over_text);
	}

	void onExit(Game& game) override
	{

	}
};

class IGameTimeUp : public IGameState
{
	void onEnter(Game& game) override
	{
		game.tclock.addClock(3.f, [&game]() {game.restart1 = true; }, "Restart");
	}

	void onUpdate(Game& game, float deltaTime) override
	{
		
	}

	void onRender(Game& game) override
	{
		game.window->draw(game.time_up_text);
	}

	void onExit(Game& game) override
	{

	}
};

class IGameFinish : public IGameState
{
	void onEnter(Game& game) override
	{
		//game.tclock.clear();
		game.flagStar_sprite.setTexture(*game.texture_manager->get("FlagStar").get());
		game.flagStar_sprite.setScale(3.125f, 3.125f);
		game.flagStar_sprite.setPosition(10200.f, 400.f); // 204, 8

		game.flagStar_animator = std::make_unique<Animator>();
		game.flagStar_animator->addPosAnimation(game.flagStar_sprite, 16, 16, 250.f, []() {return false; }, false, 10, std::vector<sf::Vector2f>{{10200.f, 300.f}}, "Flag");
	
	}

	void onUpdate(Game& game, float deltaTime) override
	{
		TClockEvent* clockEvent = game.tclock.getClock("GameLoop");
		if (clockEvent != nullptr)
		{
			float time = clockEvent->current_time;
			if (clockEvent->current_time < clockEvent->target_time)
			{
				clockEvent->current_time += 1.f * deltaTime * 50.f;
				game.addScore(1.f * deltaTime * 50.f * 50.f);
			}
			else if (clockEvent->current_time >= clockEvent->target_time)
			{
				if (!game.flagStar_animator->getAnim("Flag")->is_playing && !game.flagStar_animator->getAnim("Flag")->is_finished)
					game.flagStar_animator->playAnim("Flag");
				else if (game.flagStar_animator->getAnim("Flag")->is_finished)
					game.tclock.addClock(3.f, [this, &game]() {game.window->close(); }, "CloseApplication");
			}
		}

		game.flagStar_animator->update(deltaTime);

		game.small_coin_anim->update(deltaTime);

		game.updateMap();
	}

	void onRender(Game& game) override
	{
		//Add all renderable objects to the renderQueue
		for (const auto& object : game.gameObjects)
		{
			game.renderQueue.emplace_back(object->layer, [this, &game, object]() {object->render(game.window.get()); });
		}

		game.map->render(game.renderQueue, game.window.get());
		game.renderQueue.emplace_back(game.flag->layer, [this, &game]() {game.flag->render(game.window.get()); });
		game.renderQueue.emplace_back(5, [this, &game]() {game.window->draw(game.flagStar_sprite); });

		//Sort and render all the objects in the queue
		std::sort(game.renderQueue.begin(), game.renderQueue.end());
		for (const auto& i : game.renderQueue)
		{
			i.renderFunc();
		}

		//Render UI
		for (const auto& score : game.scores_)
		{
			if (score->getAnimation() != nullptr)
				if (score->getAnimation()->is_playing)
				{
					score->render(game.window.get());
				}
		}
	}

	void onExit(Game& game) override
	{

	}
};

class IGamePlay : public IGameState
{
	void onEnter(Game& game) override
	{
		game.game_audio_manager->playSound("Main Theme");
		
		game.tclock.addClock(game.game_time, [this, &game]()
			{
				std::cout << "Timer\n";
				game.mario->timeUp();
				game.game_audio_manager->stop();
				game.game_audio_manager->playSound("Warning");
				game.tclock.addClock(3.f, [this, &game]() { game.setState(std::static_pointer_cast<IGameState>(std::make_shared<IGameTimeUp>())); }, "TimeUp");
			}
		, "GameLoop");
	}

	void onUpdate(Game& game, float deltaTime) override
	{
		game.updateCollisions(deltaTime);
		game.mario->update(deltaTime);

		for (const auto& object : game.gameObjects)
		{
			object->update(deltaTime);
		}

		game.small_coin_anim->update(deltaTime);

		//Update flag
		game.flag->update(deltaTime);
		//Final CutScene
		if (game.mario->is_touching_flag)
		{
			game.flag->Touch(deltaTime);
		}


		game.updateMap();

		for (const auto& score : game.scores_)
			score->getAnimator()->update(deltaTime);
	}

	void onRender(Game& game) override
	{
		//Add all renderable objects to the renderQueue
		for (const auto& object : game.gameObjects)
		{
			game.renderQueue.emplace_back(object->layer, [this, &game, object]() {object->render(game.window.get()); });
		}

		game.map->render(game.renderQueue, game.window.get());
		game.renderQueue.emplace_back(game.flag->layer, [this, &game]() {game.flag->render(game.window.get()); });
		game.renderQueue.emplace_back(game.mario->layer, [this, &game]() {game.mario->render(game.window.get()); });

		//Sort and render all the objects in the queue
		std::sort(game.renderQueue.begin(), game.renderQueue.end());
		for (const auto& i : game.renderQueue)
		{
			i.renderFunc();
		}

		//Render UI
		for (const auto& score : game.scores_)
		{
			if (score->getAnimation() != nullptr)
				if (score->getAnimation()->is_playing)
				{
					score->render(game.window.get());
				}
		}
	}

	void onExit(Game& game) override
	{
		TClockEvent* it = game.tclock.getClock("GameLoop");
		if (it != nullptr)
			it->is_active = false;
	}
};