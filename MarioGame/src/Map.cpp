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
						this->tiles.emplace_back(std::make_shared<Tile>(col_rect, sprite.getTextureRect(), "Ground", true, false, 10));
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
							this->game_objects.emplace_back(std::make_unique<Brick>(sprite, texture_manager->get("Brick").get(), col_rect, "Block", 15));
						}

						if (foundTile->properties[0].getStringValue() == "Flag")
						{
							//this->game_objects.emplace_back(std::make_unique<FlagStick>(sprite, texture_manager->get("Flag").get(), col_rect, "Flag", 15))
						}

					}
					else if (lay->getName() == "Sky")
					{
						this->tiles.emplace_back(std::make_shared<Tile>(col_rect, sprite.getTextureRect(), "Sky", false, false, 5));
					}
					else
						this->tiles.emplace_back(std::make_shared<Tile>(col_rect, sprite.getTextureRect(), "Decor", false, false, 6));
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
					this->game_objects.emplace_back(std::make_unique<LuckyBlock>(game, sprite, texture_manager, col_rect, "Block", LuckyBlockType::Mushroom, gameObjects_, col_manager, 15));
				}
				else if (object.getProperties()[0].getStringValue() == "UP")
				{
					this->game_objects.emplace_back(std::make_unique<LuckyBlock>(game, sprite, texture_manager, col_rect, "Block", LuckyBlockType::UP, gameObjects_, col_manager, 15));
				}
				else if (object.getProperties()[0].getStringValue() == "Coin")
				{
					this->game_objects.emplace_back(std::make_unique<LuckyBlock>(game, sprite, texture_manager, col_rect, "Block", LuckyBlockType::Coin, gameObjects_, col_manager, 15));
				}
				else
				{
					this->game_objects.emplace_back(std::make_unique<LuckyBlock>(game, sprite, texture_manager, col_rect, "Block", LuckyBlockType::None, gameObjects_, col_manager, 15));
				}
			}
		}
	}
}

void Map::initVerArray()
{
	std::set<int> unique_orders;
	for (const auto& tile : tiles)
	{
		unique_orders.insert(tile->getLayer());
	}
	std::vector<int> orders(unique_orders.begin(), unique_orders.end());

	
	vrtxs.clear();
	vrtxs.reserve(orders.size());
	
	
	std::unordered_map<int, std::vector<std::shared_ptr<Tile>>> groupedObjects;
	for (const auto& order : orders)
	{
		groupedObjects[order].reserve(tiles.size()); 
	}
	for (const auto& tile : tiles) {
		groupedObjects[tile->getLayer()].push_back(tile);
	}

	for (int i = 0; i < orders.size(); i++)
	{
		vrtxs.push_back({sf::VertexArray(sf::Quads, groupedObjects[orders[i]].size() * 4), orders[i]});
	}

	/*sf::View view = this->window->getView();
	sf::FloatRect viewRect(
		view.getCenter().x - view.getSize().x / 2,
		view.getCenter().y - view.getSize().y / 2,
		view.getSize().x,
		view.getSize().y
	);
	for (auto& pair : groupedObjects) {
		int layer = pair.first;
		auto& tiles = pair.second;
		tiles.erase(std::remove_if(tiles.begin(), tiles.end(), [&](const auto& tile) {
			sf::FloatRect posRect = tile->getPosition();
			return !viewRect.intersects(posRect);
			}), tiles.end());
	}*/
	
	//
	//this->v_array = std::make_unique<sf::VertexArray>(sf::Quads, this->tiles.size() * 4 );

	for (int j = 0; j < orders.size(); j++)
	{
		for (int i = 0; i < groupedObjects[orders[j]].size(); i++)
		{

			sf::FloatRect posRect = groupedObjects[orders[j]][i]->getPosition();

			sf::View view = this->window->getView();
			if ((posRect.left + posRect.width) < view.getCenter().x - view.getSize().x / 2 ||
				posRect.left >= view.getCenter().x + view.getSize().x / 2 + 50.f ||
				(posRect.top + posRect.height) < view.getCenter().y - view.getSize().y / 2 ||
				posRect.top >= view.getCenter().y + view.getSize().y / 2)
			{

				continue;
			}

			sf::IntRect texRect = groupedObjects[orders[j]][i]->getTextureRect();

			float tile_x = posRect.left;
			float tile_y = posRect.top;
			float tile_width = (posRect.left + posRect.width);
			float tile_height = (posRect.top + posRect.height);

			float tex_width = texRect.left + texRect.width;
			float tex_height = texRect.top + texRect.height;

			vrtxs[j].v_array[i * 4 + 0].position = sf::Vector2f(tile_x, tile_y);
			vrtxs[j].v_array[i * 4 + 1].position = sf::Vector2f(tile_width, tile_y);
			vrtxs[j].v_array[i * 4 + 2].position = sf::Vector2f(tile_width, tile_height);
			vrtxs[j].v_array[i * 4 + 3].position = sf::Vector2f(tile_x, tile_height);

			vrtxs[j].v_array[i * 4 + 0].texCoords = sf::Vector2f(texRect.left, texRect.top);
			vrtxs[j].v_array[i * 4 + 1].texCoords = sf::Vector2f(tex_width, texRect.top);
			vrtxs[j].v_array[i * 4 + 2].texCoords = sf::Vector2f(tex_width, tex_height);
			vrtxs[j].v_array[i * 4 + 3].texCoords = sf::Vector2f(texRect.left, tex_height);

		}
	}
}

void Map::initCollisions()
{
	
	
	for (const auto& tile: this->tiles)
	{
		if (tile->isCollision())
		{
			//this->col_manager->addCollision({ tile->getPosition(), tile->getType() });
			quadTree->insert({ sf::FloatRect(tile->getPosition().left, tile->getPosition().top, tile->getPosition().width, tile->getPosition().height), "Tiles"});
		}
	}

	for (const auto& obj : game_objects)
	{
		quadTree->insert({ sf::FloatRect(obj->getBounds().left, obj->getBounds().top, obj->getBounds().width, obj->getBounds().height), obj->getType(), obj.get() });
	}
		
	for (const auto& object : this->game_objects)
	{
		//this->col_manager->addCollision({ object->getBounds(), object->getType(), object.get()});
	}
}

//Con/Des
Map::Map(std::shared_ptr<Game> game, sf::RenderWindow* window, std::shared_ptr<CollisionManager> col, std::shared_ptr<TextureManager> texture_manager, std::shared_ptr<QuadTree> quadTree, std::vector<std::shared_ptr<GameObject>>& gameObjects_) : game(game), window(window), col_manager(col), texture_manager(texture_manager),
gameObjects_(gameObjects_), quadTree(quadTree)
{
	this->initTiledMap();
	this->initSprites();
	this->initVerArray();
	this->initCollisions();
}

Map::~Map()
{
	
}

//Accessors
void Map::updateAnimations()
{
	/*for (auto& tile : this->tiles)
	{
		if (!tile->isAnimation())
			continue;

		if (tile->anim_timer->getElapsedTime().asMilliseconds() > tile->getAnimation()->frames[tile->current_frame].duration)
		{
			int nextFrame = (tile->current_frame + 1 >= tile->getAnimation()->frames.size()) ? 0 : tile->current_frame + 1;
			std::vector<tmx::Tileset::Tile> tiles = this->tile_sets[0]->getTiles();
			tmx::Tileset::Tile* foundTile = nullptr;
			for (auto& tl : tiles) {
				if (tl.ID == tile->getAnimation()->frames[nextFrame].tileID) {
					foundTile = &tl;
					break;
				}
			}

			int sprite_pos_x = (tile->getAnimation()->frames[nextFrame].tileID % this->tile_sets[0]->getColumnCount()) * this->tile_sets[0]->getTileSize().x;
			int sprite_pos_y = (tile->getAnimation()->frames[nextFrame].tileID / this->tile_sets[0]->getColumnCount()) * this->tile_sets[0]->getTileSize().y;

			tile->setTextureRect(sf::IntRect(sprite_pos_x, sprite_pos_y, this->tile_sets[0]->getTileSize().x, this->tile_sets[0]->getTileSize().y));

			tile->current_frame += 1;
			if (tile->current_frame >= tile->getAnimation()->frames.size())
				tile->current_frame = 0;

			tile->anim_timer->restart();
		}
	}*/
}

void Map::updateCollisions()
{	
}

//Functions

void Map::update(float deltaTime)
{
	
	//this->initVerArray();
	
	//this->updateAnimations();
	
	timeSinceLastUpdate += deltaTime;

	if (timeSinceLastUpdate >= updateTime)
	{
		this->initVerArray();
		//this->initCollisions();
		timeSinceLastUpdate = 0.0f;
	}

	for (const auto& object : this->game_objects)
	{
		object->update(deltaTime);
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

void Map::render(std::vector<Renderable>& queue, sf::RenderTarget* target)
{
	this->rs.texture = this->textures[0].get();

	for (const auto& vrtx : vrtxs)
	{
		//target->draw(vrtx.v_array, this->rs);
		queue.emplace_back(vrtx.layer, [vrtx, target, this]() {target->draw(vrtx.v_array, this->rs); });
	}

	for (auto& object : this->game_objects)
	{
		queue.emplace_back(object->layer, [&object, this, target]() {object->render(target); });
	}
}
