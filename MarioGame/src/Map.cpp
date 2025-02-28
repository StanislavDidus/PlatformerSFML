#include "Map.h"

//Init
void Map::initTiledMap()
{
	if (!this->tiled_map.load("assets/TiledMaps/Level1.tmx"))
		std::cout << "ERROR::MAP::INITTILEDMAP::Could not load map: assets/TiledMaps/Level1.tmx\n";

	for (const auto& currentTileSet : this->tiled_map.getTilesets())
	{
		this->tile_sets.emplace_back(std::make_unique<tmx::Tileset>(currentTileSet));
		std::unique_ptr<sf::Texture> texture = std::make_unique<sf::Texture>();
		if (!texture->loadFromFile(currentTileSet.getImagePath()))
		{
			std::cout << "ERROR::MAP::INITTILEDMAP::Could not load tile set: " << currentTileSet.getImagePath() << "\n";
		}
		else
		{
			this->textures.emplace_back(std::move(texture));
		}
	}
}

void Map::initSprites()
{
	for (const auto& lay : this->tiled_map.getLayers())
	{
		if (const auto tileLayer = dynamic_cast<tmx::TileLayer*>(lay.get()))
		{
			for (int x = 0; x < this->tiled_map.getBounds().width / this->tile_sets[0]->getTileSize().x; x++)
			{
				for (int y = 0; y < this->tiled_map.getBounds().height / this->tile_sets[0]->getTileSize().y; y++)
				{
					auto& tile = tileLayer->getTiles()[y * this->tiled_map.getBounds().width / this->tile_sets[0]->getTileSize().x + x];

					if (tile.ID == 0)
						continue;

					int GID = tile.ID - this->tile_sets[0]->getFirstGID();

					int sprite_pos_x = (GID % this->tile_sets[0]->getColumnCount()) * this->tile_sets[0]->getTileSize().x;
					int sprite_pos_y = (GID / this->tile_sets[0]->getColumnCount()) * this->tile_sets[0]->getTileSize().y;

					int window_pos_x = x * this->tile_sets[0]->getTileSize().x;
					int window_pos_y = y * this->tile_sets[0]->getTileSize().y;

					sf::Sprite sprite;
					sprite.setTexture(*this->textures[0]);
					sprite.setTextureRect(sf::IntRect(
						sprite_pos_x,
						sprite_pos_y,
						this->tile_sets[0]->getTileSize().x,
						this->tile_sets[0]->getTileSize().y));

					//sprite.setPosition(window_pos_x, window_pos_y);
					//sprite.setScale(3.125f, 3.125f);

					// Look for an tile by ID
					std::vector<tmx::Tileset::Tile> tiles = this->tile_sets[0]->getTiles();
					tmx::Tileset::Tile* foundTile = nullptr;
					for (auto& tl : tiles) {
						if (tl.ID == GID) {
							foundTile = &tl;
							break;
						}
					}

					sf::FloatRect col_rect = { window_pos_x * 3.125f,
							window_pos_y * 3.125f,
							this->tile_sets[0]->getTileSize().x * 3.125f,
							this->tile_sets[0]->getTileSize().y * 3.125f };

					sprite.setPosition(col_rect.left, col_rect.top);
					sprite.setScale(3.125f, 3.125f);

					if (lay->getName() == "Collisions")
					{
						//this->tiles_type.emplace_back(col_rect, "Ground");
						this->tiles.emplace_back(col_rect, sprite.getTextureRect(), "Ground", true, false);
					}
					else if (foundTile)
					{

						if (!foundTile->animation.frames.empty())
						{
							if (foundTile->properties[0].getStringValue() == "LuckyBlock")
							{
								//std::cout << "lb\n";
								//Tile tile = { col_rect, sprite.getTextureRect(), "LuckyBlock", true, true };
								//tile.setAnimation(std::make_shared<tmx::Tileset::Tile::Animation>(foundTile->animation));
								//this->tiles.emplace_back(std::move(tile));
								//this->game_objects.emplace_back(std::make_unique<LuckyBlock>(sprite, col_rect, "Block"));
							}
						}

						if (foundTile->properties[0].getStringValue() == "Brick")
						{
							//this->tiles.emplace_back(col_rect, sprite.getTextureRect(), "Brick", true, false);
							this->game_objects.emplace_back(std::make_unique<Brick>(sprite, col_rect, "Block"));
						}

					}
					else
					{
						this->tiles.emplace_back(col_rect, sprite.getTextureRect(), "Decor", false, false);
					}

				}

				
			}
		}
		if (const auto objectLayer = dynamic_cast<tmx::ObjectGroup*>(lay.get()))
		{
			for (const auto& object : objectLayer->getObjects())
			{
				int GID = object.getTileID() - this->tile_sets[0]->getFirstGID();

				int sprite_pos_x = (GID % this->tile_sets[0]->getColumnCount()) * this->tile_sets[0]->getTileSize().x;
				int sprite_pos_y = (GID / this->tile_sets[0]->getColumnCount()) * this->tile_sets[0]->getTileSize().y;

				sf::Sprite sprite;
				sprite.setTexture(*this->textures[0]);
				sprite.setTextureRect(sf::IntRect(
					sprite_pos_x,
					sprite_pos_y,
					this->tile_sets[0]->getTileSize().x,
					this->tile_sets[0]->getTileSize().y));

				sf::FloatRect col_rect = { object.getPosition().x * 3.125f,
							(object.getPosition().y * 3.125f ) - 16.f * 3.125f,
							this->tile_sets[0]->getTileSize().x * 3.125f,
							this->tile_sets[0]->getTileSize().y * 3.125f };

				sprite.setPosition(col_rect.left, col_rect.top);
				sprite.setScale(3.125f, 3.125f);
				
				if (object.getProperties()[0].getStringValue() == "Mushroom")
				{
					this->game_objects.emplace_back(std::make_unique<LuckyBlock>(game, sprite, col_rect, "Block", LuckyBlockType::Mushroom));
				}
				else if (object.getProperties()[0].getStringValue() == "UP")
				{
					this->game_objects.emplace_back(std::make_unique<LuckyBlock>(game, sprite, col_rect, "Block", LuckyBlockType::UP));
				}
				else if (object.getProperties()[0].getStringValue() == "Coin")
				{
					this->game_objects.emplace_back(std::make_unique<LuckyBlock>(game, sprite, col_rect, "Block", LuckyBlockType::Coin));
				}
				else
				{
					this->game_objects.emplace_back(std::make_unique<LuckyBlock>(game, sprite, col_rect, "Block", LuckyBlockType::None));
				}
			}
		}
	}
}

void Map::initVerArray()
{
	this->v_array = std::make_unique<sf::VertexArray>(sf::Quads, this->tiles.size() * 4 );

	sf::Vector2f scale = { 1.f,1.f };

	for (int i = 0; i < this->tiles.size(); i++)
	{
		
		sf::FloatRect posRect = this->tiles[i].getPosition();
		
		sf::View view = this->window->getView();
		if ((posRect.left + posRect.width) * scale.x < view.getCenter().x - view.getSize().x / 2 ||
			posRect.left * scale.x >= view.getCenter().x + view.getSize().x / 2 ||
			(posRect.top + posRect.height) * scale.y < view.getCenter().y - view.getSize().y / 2 ||
			posRect.top * scale.y >= view.getCenter().y + view.getSize().y / 2)
		{
			
			continue;
		}

		sf::IntRect texRect = this->tiles[i].getTextureRect();

		float tile_x = posRect.left * scale.x;
		float tile_y = posRect.top * scale.y;
		float tile_width = (posRect.left + posRect.width) * scale.x;
		float tile_height = (posRect.top + posRect.height) * scale.y;

		float tex_width = texRect.left + texRect.width;
		float tex_height = texRect.top + texRect.height;

		this->v_array->operator[](0 + i * 4).position = sf::Vector2f(tile_x, tile_y);
		this->v_array->operator[](1 + i * 4).position = sf::Vector2f(tile_width, tile_y);
		this->v_array->operator[](2 + i * 4).position = sf::Vector2f(tile_width, tile_height);
		this->v_array->operator[](3 + i * 4).position = sf::Vector2f(tile_x, tile_height);

		this->v_array->operator[](0 + i * 4).texCoords = sf::Vector2f(texRect.left, texRect.top);
		this->v_array->operator[](1 + i * 4).texCoords = sf::Vector2f(tex_width, texRect.top);
		this->v_array->operator[](2 + i * 4).texCoords = sf::Vector2f(tex_width, tex_height);
		this->v_array->operator[](3 + i * 4).texCoords = sf::Vector2f(texRect.left, tex_height);
	}
}

void Map::initCollisions()
{
	for (const auto& tile: this->tiles)
	{
		if(tile.isCollision())
			this->col_manager->addCollision({tile.getPosition(), tile.getType()});
	}

	for (const auto& object : this->game_objects)
	{
		this->col_manager->addCollision({ object->getBounds(), object->getType(), object.get()});
	}
}

//Con/Des
Map::Map(Game* game, sf::RenderWindow* window, CollisionManager* col) : game(game), window(window), col_manager(col)
{
	this->initTiledMap();
	this->initSprites();
	this->initVerArray();
	this->initCollisions();
}

Map::~Map()
{
	delete this->col_manager;
}

//Accessors
void Map::updateAnimations()
{
	for (auto& tile : this->tiles)
	{
		if (!tile.isAnimation())
			continue;

		if (tile.anim_timer->getElapsedTime().asMilliseconds() > tile.getAnimation()->frames[tile.current_frame].duration)
		{
			int nextFrame = (tile.current_frame + 1 >= tile.getAnimation()->frames.size()) ? 0 : tile.current_frame + 1;
			std::vector<tmx::Tileset::Tile> tiles = this->tile_sets[0]->getTiles();
			tmx::Tileset::Tile* foundTile = nullptr;
			for (auto& tl : tiles) {
				if (tl.ID == tile.getAnimation()->frames[nextFrame].tileID) {
					foundTile = &tl;
					break;
				}
			}

			int sprite_pos_x = (tile.getAnimation()->frames[nextFrame].tileID % this->tile_sets[0]->getColumnCount()) * this->tile_sets[0]->getTileSize().x;
			int sprite_pos_y = (tile.getAnimation()->frames[nextFrame].tileID / this->tile_sets[0]->getColumnCount()) * this->tile_sets[0]->getTileSize().y;

			tile.setTextureRect(sf::IntRect(sprite_pos_x, sprite_pos_y, this->tile_sets[0]->getTileSize().x, this->tile_sets[0]->getTileSize().y));

			tile.current_frame += 1;
			if (tile.current_frame >= tile.getAnimation()->frames.size())
				tile.current_frame = 0;

			tile.anim_timer->restart();
		}
	}
}

void Map::updateCollisions()
{
	this->col_manager->clearCollision();
	
	for (const auto& tile : this->tiles)
	{
		if (tile.isCollision())
			this->col_manager->addCollision({ tile.getPosition(), tile.getType() });
	}

	for (const auto& object : this->game_objects)
	{
		this->col_manager->addCollision({ object->getBounds(), object->getType(), object.get() });
	}
}

//Functions
void Map::update(float deltaTime)
{
	
	//this->initVerArray();
	
	this->updateAnimations();
	

	timeSinceLastUpdate += deltaTime;

	if (timeSinceLastUpdate >= updateTime)
	{
		this->initVerArray();
		this->updateCollisions();
		timeSinceLastUpdate = 0.0f;
	}

	for (const auto& object : this->game_objects)
	{
		object->update(deltaTime);

		Block* block = dynamic_cast<Block*>(object.get());
		if (block != nullptr && block->isDesroyed())
		{
			//this->game_objects.erase(i);
		}
	}

	this->game_objects.erase(std::remove_if(this->game_objects.begin(), this->game_objects.end(),
		[](const std::unique_ptr<GameObject>& obj) { 
			Block* block = dynamic_cast<Block*>(obj.get());
			if(block != nullptr && block->isDesroyed())
				return true; 
			return false;
		}),
		this->game_objects.end());
}

void Map::render(sf::RenderTarget* target)
{
	this->rs.texture = this->textures[0].get();

	target->draw(*this->v_array, this->rs);
	//target->draw(*this->animated_v_array, this->rs);

	//Draw collisions
	for (auto& t : this->col_manager->temp_collisions)
	{
		/*sf::Sprite sp;
		sf::Texture tx;
		tx.loadFromFile("assets/Textures/col.png");
		sp.setTexture(tx);
		sp.setScale(3.125f, 3.125f);
		sp.setPosition({ t.collider_bounds.left, t.collider_bounds.top });
		target->draw(sp);*/
	}

	for (auto& object : this->game_objects)
	{
		object->render(target);
	}
}
