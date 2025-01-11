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

					sprite.setPosition(window_pos_x , window_pos_y );
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

					
					if (lay->getName() == "Collisions")
					{
						this->collide_tiles.emplace_back(
							window_pos_x * 3.125f,
							window_pos_y * 3.125f,
							this->tile_sets[0]->getTileSize().x * 3.125f,
							this->tile_sets[0]->getTileSize().y * 3.125f
						);

					}
					else if (lay->getName() == "Interactions")
					{
						this->collide_tiles.emplace_back(
								window_pos_x * 3.125f,
								window_pos_y * 3.125f,
								this->tile_sets[0]->getTileSize().x * 3.125f,
							this->tile_sets[0]->getTileSize().y * 3.125f
							);
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
Map::Map(sf::RenderWindow* window) : window(window)
{
	this->initTiledMap();
	this->initSprites();
	this->initVerArray();
}

Map::~Map()
{
}

//Accessors
const void Map::calibrateCollision(const sf::FloatRect& player, float& x, float& y)
{
	sf::FloatRect newBoundsX = { player.left + x, player.top, player.width, player.height };
	sf::FloatRect newBoundsY = { player.left , player.top + y, player.width, player.height };


	for (auto& object : this->collide_tiles)
	{
		sf::FloatRect objectBounds = static_cast<sf::FloatRect>(object);
		if (y != 0)
		{
			bool topCollision = newBoundsY.top < objectBounds.top + objectBounds.height &&
				newBoundsY.top > objectBounds.top &&
				newBoundsY.left < objectBounds.left + objectBounds.width &&
				newBoundsY.left + newBoundsY.width > objectBounds.left;

			bool bottomCollision = newBoundsY.top + newBoundsY.height > objectBounds.top &&
				newBoundsY.top + newBoundsY.height < objectBounds.top + objectBounds.height &&
				newBoundsY.left < objectBounds.left + objectBounds.width &&
				newBoundsY.left + newBoundsY.width > objectBounds.left;

			if (y < 0)
			{
				if (topCollision)
				{
					y = objectBounds.top + objectBounds.height - player.top;
					continue;
					//std::cout << "top\n";
				}
			}

			else if (y > 0)
			{
				if (bottomCollision)
				{
					y = objectBounds.top - player.height - player.top;
					continue;
					//std::cout << "bottom\n";
				}
			}
		}

		if (x != 0)
		{
			bool leftCollision = newBoundsX.left < objectBounds.left + objectBounds.width &&
				newBoundsX.left + newBoundsX.width > objectBounds.left &&
				newBoundsX.top < objectBounds.top + objectBounds.height &&
				newBoundsX.top + newBoundsX.height > objectBounds.top;

			bool rightCollision = newBoundsX.left + newBoundsX.width > objectBounds.left &&
				newBoundsX.left < objectBounds.left + objectBounds.width &&
				newBoundsX.top < objectBounds.top + objectBounds.height &&
				newBoundsX.top + newBoundsX.height > objectBounds.top;

			if (x < 0)
			{
				if (leftCollision)
				{
					//std::cout << "left\n";
					x = objectBounds.left + objectBounds.width - player.left;
					continue;
				}

				if (newBoundsX.left < 0.f) // Left side of the screen
					x = 0.f;
			}
			else if (x > 0)
			{
				if (rightCollision)
				{
					//std::cout << "right\n";
					x = objectBounds.left - player.width - player.left;
					continue;
				}
			}
		}
	}
}

const bool Map::checkGround(const sf::FloatRect& player, float x, float y)
{
	sf::FloatRect newBounds = { player.left , player.top + y, player.width, player.height };

	for (auto& object : this->collide_tiles)
	{
		sf::FloatRect objectBounds = static_cast<sf::FloatRect>(object);

		bool bottomCollision = newBounds.top + newBounds.height > objectBounds.top &&
			newBounds.top + newBounds.height < objectBounds.top + objectBounds.height &&
			newBounds.left < objectBounds.left + objectBounds.width &&
			newBounds.left + newBounds.width > objectBounds.left;

		if (bottomCollision && y > 0)
		{
			return true;
		}
	}

	return false;
}

const bool Map::checkRoof(const sf::FloatRect& player, float x, float y)
{
	sf::FloatRect newBounds = { player.left , player.top + y, player.width, player.height };

	for (auto& object : this->collide_tiles)
	{
		sf::FloatRect objectBounds = static_cast<sf::FloatRect>(object);

		bool topCollision = newBounds.top < objectBounds.top + objectBounds.height &&
			newBounds.top > objectBounds.top &&
			newBounds.left < objectBounds.left + objectBounds.width &&
			newBounds.left + newBounds.width > objectBounds.left;

		if (topCollision && y < 0)
		{
			return true;
			std::cout << "ROOF\n";
		}
	}

	return false;
}

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

//Functions
void Map::update(float deltaTime)
{
	this->updateAnimations();


	timeSinceLastUpdate += deltaTime;

	if (timeSinceLastUpdate >= updateTime) 
	{
		this->initVerArray(); 
		timeSinceLastUpdate = 0.0f;
	}
}

void Map::render(sf::RenderTarget* target)
{
	//for(const auto& tile : this->animation_tiles)
		//target->draw(tile);
	//for (const auto& tile : this->other_tiles)
	//{
		//sf::Sprite sprite = sf::Sprite(tile);
		//target->draw(tile);
	//}
	//for (const auto& tile : this->collide_tiles)
		//target->draw(tile);

	sf::RenderStates rs;
	rs.texture = this->textures[0].get();
	
	target->draw(*this->v_array, rs);
	target->draw(*this->animated_v_array, rs);
}
	