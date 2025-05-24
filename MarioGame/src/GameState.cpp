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
		game.tclock.addClock(3.f, [this, &game]() {game.window->close(); }, "Close");
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
		game.tclock.addClock(3.f, [&game]() {game.restart(); }, "Restart");
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

class IGamePlay : public IGameState
{
	void onEnter(Game& game) override
	{
		game.tclock.addClock(game.game_time, [this, &game]()
			{
				game.mario->timeUp();
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
		if (game.flag->is_finished)
		{
			game.mario->Finish(deltaTime);
		}
		//

		if (game.mario->need_restart)
		{
			game.restart();
		}
		if (game.mario->need_quit)
		{
			game.window->close();
		}

		game.updateAudio();

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