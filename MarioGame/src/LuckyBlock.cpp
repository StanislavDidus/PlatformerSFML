#include "LuckyBlock.h"
#include "Game.h"

void LuckyBlock::initLuckyBlock()
{
	this->animator->addFrameAnimation(
		this->sprite, 16 * 3.125f, 16 * 3.125f, 0, 2, 400.f / 1000.f, [this]() { return this->is_active;  }, [this]() {return 1; }, true, 5, "LIdle"
	);
}

void LuckyBlock::initCoin()
{
	this->coin_texture.loadFromFile("assets/Textures/Levels/Coin_Anim.png");
	this->coin_sprite.setTexture(this->coin_texture);
	this->coin_sprite.setScale(3.125f, 3.125f);
	this->coin_sprite.setTextureRect(sf::IntRect(0, 0 , 8, 16));
	this->coin_sprite.setPosition(this->getSprite().getPosition().x + 8 * 3.125f - 4 * 3.125f, this->getSprite().getPosition().y);

	//this->animator->addPosAnimation(
		//this->coin_sprite, 450.f, [this]() {return false; }, false, 10, { {this->coin_sprite.getPosition().x , this->coin_sprite.getPosition().y - 85.f}  ,{this->coin_sprite.getPosition().x, this->coin_sprite.getPosition().y - 155.f} }, "Coin");

	this->animator->addFrameAnimation(
		this->coin_sprite, 8 * 3.125f, 16 * 3.125f, 0, 3, 100.f / 1000.f, [this]() {return true; }, [this]() {return 1; }, true, 5, "CIdle"
	);

	
}

void LuckyBlock::initScore()
{
	this->score_texture.loadFromFile("assets/Textures/Scores/200.png");
	this->score_sprite.setTexture(this->score_texture);
	this->score_sprite.setScale(2.f, 2.f);
	this->score_sprite.setTextureRect(sf::IntRect(0, 0, 23, 7));
	this->score_sprite.setPosition(this->coin_sprite.getPosition().x, this->coin_sprite.getPosition().y - 85.f);

	PosAnimation ps3 = { score_sprite, score_sprite.getGlobalBounds().width / 2.f, score_sprite.getGlobalBounds().height / 1.5f, 60.f,[this]() {return false; }, false, 10, { {this->coin_sprite.getPosition().x, this->coin_sprite.getPosition().y - 120.f}  ,{this->coin_sprite.getPosition().x, this->coin_sprite.getPosition().y - 100.f} }, "Score" };
	PosAnimation ps = { coin_sprite, coin_sprite.getGlobalBounds().width / 3.125f, coin_sprite.getGlobalBounds().height / 3.125f, 450.f,[this]() {return false; }, false, 10, { {this->coin_sprite.getPosition().x , this->coin_sprite.getPosition().y - 85.f}  ,{this->coin_sprite.getPosition().x, this->coin_sprite.getPosition().y - 155.f} }, "Coin" };
	PosAnimation ps1 = { this->sprite, this->sprite.getGlobalBounds().width / 3.125f, this->sprite.getGlobalBounds().height / 3.125f, 150.f, [this]() {return false; }, false, 25, {{this->sprite.getPosition()},{this->sprite.getPosition().x, this->sprite.getPosition().y - 15.f}}, "Hit" };
	std::vector<std::shared_ptr<Animation>> ps_a;
	ps_a.emplace_back(std::make_unique<PosAnimation>(ps));
	ps_a.emplace_back(std::make_unique<PosAnimation>(ps1));
	ps_a.emplace_back(std::make_unique<PosAnimation>(ps3));
	std::vector<float> timing;
	timing.emplace_back(0.f);
	timing.emplace_back(0.f);
	timing.emplace_back(0.5f);
	this->animator->addAnimationSequence(std::move(ps_a), timing, [this]() {return false; }, "Get", 25);
}

LuckyBlock::LuckyBlock(Game* game, const sf::Sprite& sprite, const sf::FloatRect& rect, const std::string& type, const LuckyBlockType& l_type) : Block(rect, type, "assets/Textures/Levels/LuckyBlock.png"),  game(game), l_type(l_type)
{
	this->initLuckyBlock();
	if (l_type == LuckyBlockType::Coin)
	{
		this->initCoin();
		this->initScore();
	}
}

void LuckyBlock::update(float deltaTime)
{
	animator->update(deltaTime);

	sf::Vector2f newPos = this->getSprite().getPosition();
	setPosition(newPos);
}

void LuckyBlock::render(sf::RenderTarget* target)
{
	if (this->animator != nullptr)
	{
		std::shared_ptr<Animation> anim = this->animator->getAnim("Get");
		if (anim != nullptr)
		{
			AnimationSequence* sequence = dynamic_cast<AnimationSequence*>(anim.get());
			
			if (sequence != nullptr)
			{
				if(sequence->getAnimation(0)->is_playing)
				{ 
					target->draw(this->coin_sprite);
					
				}
				if (sequence->getAnimation(2)->is_playing)
				{
					target->draw(this->score_sprite);
				}
			}
				
		}
	}

	target->draw(this->getSprite());
}

void LuckyBlock::onHit()
{	
	//Animation
	//this->animator->playAnim("Hit");

	//Bonus
	if (this->is_active)
	{
		switch (this->l_type)
		{
		case LuckyBlockType::None:
			std::cout << "None\n";
			break;
		case LuckyBlockType::Mushroom:
			std::cout << "Mushroom\n";
			break;
		case LuckyBlockType::Coin:
			std::cout << "Coin\n";
			this->giveCoin();
			break;
		case LuckyBlockType::UP:
			std::cout << "UP\n";
			break;
		}
	}

	//Disable
	this->getTexture().loadFromFile("assets/Textures/Levels/Block.png");
	this->sprite.setTextureRect(sf::IntRect(0,0,16,16));
	this->is_active = false;
}

const void LuckyBlock::giveCoin() const
{
	this->animator->playAnim("Get");
	this->game->addScore(200);
}
