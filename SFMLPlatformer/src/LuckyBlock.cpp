#include "Objects/LuckyBlock.h"
#include "Game.h"

void LuckyBlock::initLuckyBlock()
{
	animator->addFrameAnimation(
		sprite, 16, 16, std::vector<int>{ 0,1,2 }, 400.f / 1000.f, [this]() { return is_active;  }, [this]() {return 1; }, true, 5, "LIdle"
	);
}

void LuckyBlock::initCoin()
{
	coin_texture = *texture_manager->get("Coin").get();
	coin_sprite.setTexture(coin_texture);
	coin_sprite.setScale(3.125f, 3.125f);
	coin_sprite.setTextureRect(sf::IntRect(0, 0, 8, 16));
	coin_sprite.setPosition(getSprite().getPosition().x + 8 * 3.125f - 4 * 3.125f, getSprite().getPosition().y);
	animator->addFrameAnimation(
		coin_sprite, 8, 16, std::vector<int>{ 0,1,2,3 }, 100.f / 1000.f, [this]() {return true; }, [this]() {return 1; }, true, 5, "CIdle"
	);
}

void LuckyBlock::initScore()
{
	//Initialize hit animation with a moving coin and score indicator
	score_text = std::make_unique<Text>(16,7, coin_sprite.getPosition(), texture_manager->get("200S").get());
	PosAnimation ps = { coin_sprite, static_cast<int>(coin_sprite.getGlobalBounds().width / 3.125f), static_cast<int>(coin_sprite.getGlobalBounds().height / 3.125f), 450.f,[this]() {return false; }, false, 10, std::vector<sf::Vector2f>{{sprite.getPosition().x + coin_sprite.getGlobalBounds().width / 2, sprite.getPosition().y - 155},{sprite.getPosition().x + coin_sprite.getGlobalBounds().width / 2, sprite.getPosition().y - 85}}, "Coin"};
	PosAnimation ps1 = { sprite, static_cast<int>(sprite.getGlobalBounds().width / 3.125f), static_cast<int>(sprite.getGlobalBounds().height / 3.125f), 150.f, [this]() {return false; }, false, 25, std::vector<sf::Vector2f>{{sprite.getPosition().x, sprite.getPosition().y - 15}, {sprite.getPosition()}}, "Hit"};
	std::vector<std::shared_ptr<Animation>> ps_a;
	ps_a.emplace_back(std::make_unique<PosAnimation>(ps));
	ps_a.emplace_back(std::make_unique<PosAnimation>(ps1));
	ps_a.emplace_back(score_text->getAnimation());
	std::vector<float> timing;
	timing.emplace_back(0.f);
	timing.emplace_back(0.f);
	timing.emplace_back(0.5f);
	animator->addAnimationSequence(std::move(ps_a), timing, [this]() {return false; }, "Get", 25);
}

LuckyBlock::LuckyBlock(std::shared_ptr<Game> game, const sf::Sprite& sprite, std::shared_ptr<TextureManager> texture_manager, const sf::FloatRect& rect, const std::string& type, const LuckyBlockType& l_type, std::vector<std::shared_ptr<GameObject>>& gameObjects, std::shared_ptr<CollisionManager> col, int layer) : Block(rect, type, texture_manager->get("LuckyBlock").get(), layer, col, gameObjects), game(game), l_type(l_type), texture_manager(texture_manager)
{
	audio_manager->addSound("PowerUp_Appear", "assets/Sounds/Game/PowerUp_Appear.wav", false);
	initLuckyBlock();
	if (l_type == LuckyBlockType::CoinType)
	{
		initCoin();
		initScore();
		audio_manager->addSound("Coin", "assets/Sounds/Game/Coin.wav", false);
	}
}

void LuckyBlock::update(float deltaTime)
{
	animator->update(deltaTime);

	if (score_text != nullptr)
		score_text->update(deltaTime);

	if (ready_to_spawn)
	{
		if (!animator->getAnim("Hit")->is_playing)
		{
			ready_to_spawn = false;
			if (is_active)
			{
				switch (l_type)
				{
				case LuckyBlockType::None:

					break;
				case LuckyBlockType::MushRoomType:
					audio_manager->playSound("PowerUp_Appear");
					spawnMushroom();
					break;
				case LuckyBlockType::FireFlowerType:
					audio_manager->playSound("PowerUp_Appear");
					spawnFireFlower();
					break;
				case LuckyBlockType::CoinType:
					giveCoin();
					break;
				case LuckyBlockType::UPType:

					break;
				}

			}
			getTexture() = *texture_manager->get("Block").get();
			sprite.setTextureRect(sf::IntRect(0, 0, 16, 16));
			is_active = false;
		}

		
	}

	sf::Vector2f newPos = getSprite().getPosition();
	setPosition(newPos);
}

void LuckyBlock::render(sf::RenderTarget* target)
{
	if (animator != nullptr)
	{
		std::shared_ptr<Animation> anim = animator->getAnim("Get");
		if (anim != nullptr)
		{
			AnimationSequence* sequence = dynamic_cast<AnimationSequence*>(anim.get());

			if (sequence != nullptr)
			{
				if (sequence->getAnimation(0)->is_playing)
				{
					target->draw(coin_sprite);

				}
				if (sequence->getAnimation(2)->is_playing)
				{
					if (score_text != nullptr)
						score_text->render(target);
				}
			}

		}
	}
	target->draw(getSprite());
}

void LuckyBlock::onHit()
{
	//Animation
	if (is_active)
	{
		HitItem();
		animator->playAnim("Hit");
		//audio_manager->playSound("Bump");

		switch (l_type)
		{
		case LuckyBlockType::None:

			break;
		case LuckyBlockType::MushRoomType:

			ready_to_spawn = true;
			break;
		case LuckyBlockType::FireFlowerType:
			ready_to_spawn = true;
			break;
		case LuckyBlockType::CoinType:

			giveCoin();
			getTexture() = *texture_manager->get("Block").get();
			sprite.setTextureRect(sf::IntRect(0, 0, 16, 16));
			is_active = false;
			break;

			//Bonus
			//Disable
		}
	}
}

void LuckyBlock::onHitBig()
{
	if (l_type == LuckyBlockType::MushRoomType)
	{
		l_type = LuckyBlockType::FireFlowerType;
	}
	onHit();
}

const void LuckyBlock::spawnMushroom()
{
	sf::FloatRect rect = { sprite.getPosition().x, sprite.getPosition().y, 50.f, 50.f };
	std::shared_ptr<MushRoom> obj = std::make_shared<MushRoom>(rect, "Mushroom", texture_manager, col, game, 10);
	gameObjects.emplace_back(obj);
}

const void LuckyBlock::spawnFireFlower()
{
	sf::FloatRect rect = { sprite.getPosition().x, sprite.getPosition().y, 50.f, 50.f };
	std::shared_ptr<FireFlower> obj = std::make_shared<FireFlower>(rect, "FireFlower", texture_manager, col, game, 10);
	gameObjects.emplace_back(obj);
}

const void LuckyBlock::giveCoin() const
{
	audio_manager->playSound("Coin");
	animator->playAnim("Get");
	game->addScore(200);
	game->addCoin();
}
