#include "Game.h"

class IGameState
{
public:
	virtual ~IGameState() = default;

	virtual void onEnter(Game& game) = 0;
	virtual void onUpdate(Game& game, float deltaTime) = 0;
	virtual void onExit(Game& game) = 0;
};

class IGameShowInfo : public IGameState
{
	void onEnter(Game& game) override
	{

	}

	void onUpdate(Game& game, float deltaTime) override
	{

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

	void onExit(Game& game) override
	{

	}
};

class IGameGameOver : public IGameState
{
	void onEnter(Game& game) override
	{

	}

	void onUpdate(Game& game, float deltaTime) override
	{

	}

	void onExit(Game& game) override
	{

	}
};

class IGameTimeUp : public IGameState
{
	void onEnter(Game& game) override
	{

	}

	void onUpdate(Game& game, float deltaTime) override
	{

	}

	void onExit(Game& game) override
	{

	}
};

class IGamePlay : public IGameState
{
	void onEnter(Game& game) override
	{

	}

	void onUpdate(Game& game, float deltaTime) override
	{

	}

	void onExit(Game& game) override
	{

	}
};