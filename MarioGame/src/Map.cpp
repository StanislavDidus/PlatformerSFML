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

					sprite.setPosition(window_pos_x, window_pos_y);
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


					if (lay->getName() == "Collisions")
					{
						this->tiles_type.emplace_back(col_rect, "Ground");
					}

					if (foundTile)
					{

						if (!foundTile->animation.frames.empty()) // if there is an animation
						{
							this->animation_tiles.emplace_back(sprite.getGlobalBounds(), sprite.getTextureRect(), foundTile->animation);

						}
						else
						{
							this->all_tiles.emplace_back(sprite.getGlobalBounds(), sprite.getTextureRect());
						}



						if (foundTile->properties[0].getStringValue() == "LuckyBlock")
						{
							this->tiles_type.emplace_back(col_rect, "LuckyBlock");
						}
						else if (foundTile->properties[0].getStringValue() == "Brick")
						{
							this->tiles_type.emplace_back(col_rect, "Brick");
						}

					}
					else
					{
						this->all_tiles.emplace_back(sprite.getGlobalBounds(), sprite.getTextureRect());

					}
				}
			}
		}
	}
}

void Map::initVerArray()
{
	this->v_array = std::make_unique<sf::VertexArray>(sf::Quads, this->all_tiles.size() * 4);
	this->animated_v_array = std::make_unique<sf::VertexArray>(sf::Quads, this->animation_tiles.size() * 4);

	sf::Vector2f scale = { 3.125f,3.125f };

	for (int i = 0; i < this->all_tiles.size(); i++)
	{
		sf::FloatRect posRect = this->all_tiles[i].first;

		sf::View view = this->window->getView();
		if ((posRect.left + posRect.width) * scale.x < view.getCenter().x - view.getSize().x / 2 ||
			posRect.left * scale.x >= view.getCenter().x + view.getSize().x / 2 ||
			(posRect.top + posRect.height) * scale.y < view.getCenter().y - view.getSize().y / 2 ||
			posRect.top * scale.y >= view.getCenter().y + view.getSize().y / 2)
		{
			//std::cout << "Pixel: " << i << ", posRect: " << posRect.left << ", " << posRect.top << " is cleared\n";
			continue;
		}

		sf::IntRect texRect = this->all_tiles[i].second;

		//std::cout << texRect.left << ", " << texRect.top << "\n";

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

	for (int i = 0; i < this->animation_tiles.size(); i++)
	{
		sf::FloatRect posRect = this->animation_tiles[i].posRect;

		sf::View view = this->window->getView();
		if ((posRect.left + posRect.width) * scale.x < view.getCenter().x - view.getSize().x / 2 ||
			posRect.left * scale.x >= view.getCenter().x + view.getSize().x / 2 ||
			(posRect.top + posRect.height) * scale.y < view.getCenter().y - view.getSize().y / 2 ||
			posRect.top * scale.y >= view.getCenter().y + view.getSize().y / 2)
		{
			//std::cout << "Pixel: " << i << ", posRect: " << posRect.left << ", " << posRect.top << " is cleared\n";
			continue;
		}


		sf::IntRect texRect = this->animation_tiles[i].texRect;

		float tile_x = posRect.left * scale.x;
		float tile_y = posRect.top * scale.y;
		float tile_width = (posRect.left + posRect.width) * scale.x;
		float tile_height = (posRect.top + posRect.height) * scale.y;

		float tex_width = texRect.left + texRect.width;
		float tex_height = texRect.top + texRect.height;

		this->animated_v_array->operator[](0 + i * 4).position = sf::Vector2f(tile_x, tile_y);
		this->animated_v_array->operator[](1 + i * 4).position = sf::Vector2f(tile_width, tile_y);
		this->animated_v_array->operator[](2 + i * 4).position = sf::Vector2f(tile_width, tile_height);
		this->animated_v_array->operator[](3 + i * 4).position = sf::Vector2f(tile_x, tile_height);

		this->animated_v_array->operator[](0 + i * 4).texCoords = sf::Vector2f(texRect.left, texRect.top);
		this->animated_v_array->operator[](1 + i * 4).texCoords = sf::Vector2f(tex_width, texRect.top);
		this->animated_v_array->operator[](2 + i * 4).texCoords = sf::Vector2f(tex_width, tex_height);
		this->animated_v_array->operator[](3 + i * 4).texCoords = sf::Vector2f(texRect.left, tex_height);
	}
}

//Con/Des
Map::Map(sf::RenderWindow* window, CollisionManager* col) : window(window), col_manager(col)
{
	this->initTiledMap();
	this->initSprites();
	this->initVerArray();
}

Map::~Map()
{
}

//Accessors
void Map::updateAnimations()
{
	for (auto& tile : this->animation_tiles)
	{
		//std::cout << tile.animation.frames[tile.current_frame].duration << "\n";
		if (tile.clock.getElapsedTime().asMilliseconds() > tile.animation.frames[tile.current_frame].duration)
		{
			int nextFrame = (tile.current_frame + 1 >= tile.animation.frames.size()) ? 0 : tile.current_frame + 1;
			std::vector<tmx::Tileset::Tile> tiles = this->tile_sets[0]->getTiles();
			tmx::Tileset::Tile* foundTile = nullptr;
			for (auto& tl : tiles) {
				if (tl.ID == tile.animation.frames[nextFrame].tileID) {
					foundTile = &tl;
					break;
				}
			}

			int sprite_pos_x = (tile.animation.frames[nextFrame].tileID % this->tile_sets[0]->getColumnCount()) * this->tile_sets[0]->getTileSize().x;
			int sprite_pos_y = (tile.animation.frames[nextFrame].tileID / this->tile_sets[0]->getColumnCount()) * this->tile_sets[0]->getTileSize().y;

			tile.texRect = sf::IntRect(sprite_pos_x, sprite_pos_y, this->tile_sets[0]->getTileSize().x, this->tile_sets[0]->getTileSize().y);

			tile.current_frame++;
			if (tile.current_frame >= tile.animation.frames.size())
				tile.current_frame = 0;



			tile.clock.restart();
		}
	}
}

void Map::updateCollisions()
{
	this->col_manager->clearCollision();
	sf::Vector2f scale = { 3.125f, 3.125f };
	for (const auto& col : this->tiles_type)
	{
		bool src_found = false;
		for (const auto& src : this->col_manager->getSources())
		{
			if (MathUtils::distance(col.first, src) < src.width * 2.5f)
			{
				src_found = true;
				break;
			}
			//std::cout << src.left << ", " << src.top << "\n";
		}
		
		if (src_found)
		{
			/*sf::View view = this->window->getView();
			sf::FloatRect posRect = { col.first.left / scale.x, col.first.top / scale.y, col.first.width / scale.x, col.first.height / scale.y };
			if ((posRect.left + posRect.width) * scale.x < view.getCenter().x - view.getSize().x / 2 ||
				posRect.left * scale.x >= view.getCenter().x + view.getSize().x / 2 ||
				(posRect.top + posRect.height) * scale.y < view.getCenter().y - view.getSize().y / 2 ||
				posRect.top * scale.y >= view.getCenter().y + view.getSize().y / 2)
			{
				continue;
			}*/
			this->col_manager->addCollision({ col.first, col.second });
		}
	}

}

//Functions
void Map::update(float deltaTime)
{
	this->updateAnimations();
	

	timeSinceLastUpdate += deltaTime;

	if (timeSinceLastUpdate >= updateTime)
	{
		this->initVerArray();
		this->updateCollisions();
		timeSinceLastUpdate = 0.0f;
	}
}

void Map::render(sf::RenderTarget* target)
{
	this->rs.texture = this->textures[0].get();

	target->draw(*this->v_array, this->rs);
	target->draw(*this->animated_v_array, this->rs);
}
