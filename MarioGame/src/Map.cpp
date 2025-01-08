#include "Map.h"

//Init
void Map::initTiledMap()
{
	if (!this->tiled_map.load("assets/TiledMaps/Level1.tmx"))
		std::cout << "ERROR::MAP::INITTILEDMAP::Could not load map: assets/TiledMaps/Level1.tmx\n";

	for (const auto& currentTileSet : this->tiled_map.getTilesets())
	{
		this->tile_sets.push_back(std::make_unique<tmx::Tileset>(currentTileSet));
		std::unique_ptr<sf::Texture> texture = std::make_unique<sf::Texture>();
		if (!texture->loadFromFile(currentTileSet.getImagePath()))
		{
			std::cout << "ERROR::MAP::INITTILEDMAP::Could not load tile set: " << currentTileSet.getImagePath() << "\n";
		}
		else
		{
			this->textures.push_back(std::move(texture));
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

					int window_pos_x = x * this->tile_sets[0]->getTileSize().x * 3.125f;
					int window_pos_y = y * this->tile_sets[0]->getTileSize().y * 3.125f;

					sf::Sprite sprite;
					sprite.setTexture(*this->textures[0]);
					sprite.setTextureRect(sf::IntRect(
						sprite_pos_x,
						sprite_pos_y,
						this->tile_sets[0]->getTileSize().x,
						this->tile_sets[0]->getTileSize().y));

					sprite.setPosition(window_pos_x, window_pos_y);
					sprite.setScale(3.125f, 3.125f);

					// Look for an tile by ID
					std::vector<tmx::Tileset::Tile> tiles = this->tile_sets[0]->getTiles();
					tmx::Tileset::Tile* foundTile = nullptr;
					for (auto& tl : tiles) {
						if (tl.ID == GID) {
							foundTile = &tl;
							break;
						}
					}

					if (foundTile)
						if (!foundTile->animation.frames.empty()) // if there is an animation
						{
							//std::unordered_map<sf::Sprite, tmx::Tileset::Tile::Animation> newMap;
							//newMap[sprite] = foundTile->animation;
							//this->animation_tiles.push_back(newMap);
						}
						if (lay->getName() == "Collisions")
						{
							this->collide_tiles.push_back(sprite);
							
						}
						else if (lay->getName() == "Interactions")
						{
							this->collide_tiles.push_back(sprite);
						}
						else
						{
							this->other_tiles.push_back(sprite);
						}
				}
			}
		}
	}
}

void Map::initImage()
{
	this->image = std::make_unique<sf::VertexArray>(sf::Quads, this->other_tiles.size());

	for (int i = 0; i < this->other_tiles.size(); i++)
	{
		sf::IntRect texRect = this->other_tiles[i].getTextureRect();
		this->image->operator[](i) = sf::Vertex(
			this->other_tiles[i].getPosition(),
			sf::Vector2f(texRect.left, texRect.top)
		);
	}
}

//Con/Des
Map::Map(sf::RenderWindow* window) : window(window)
{
	this->initTiledMap();
	this->initSprites();
	this->initImage();
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
		sf::FloatRect objectBounds = object.getGlobalBounds();
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
		sf::FloatRect objectBounds = object.getGlobalBounds();

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
		sf::FloatRect objectBounds = object.getGlobalBounds();

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

sf::Vector2f Map::getBlock(float x, float y, float mult)
{
	sf::Vector2f pos = { x * 16.f * mult,
		this->window->getSize().y - 16.f * mult * y };

	return pos;
}

//Modifiers
void Map::addObject(const std::string& texturePath, const sf::Vector2f& position, const sf::Vector2f& scale, bool is_collision)
{
	auto texture = std::make_unique<sf::Texture>();
	if (!texture->loadFromFile(texturePath))
	{
		std::cout << "ERROR::MAP::ADDOBJECT::Could not load texture: " << texturePath << "\n";
		return;
	}

	this->textures.push_back(std::move(texture));

	sf::Sprite sprite;
	sprite.setTexture(*this->textures.back());
	sprite.setPosition(position);
	sprite.setScale(scale);

	if (is_collision)
		this->all_objects.push_back(sprite);
	else
		this->not_col_objects.push_back(sprite);
}

//Functions
void Map::update()
{

}

void Map::render(sf::RenderTarget* target)
{
	//for(const auto& tile : this->animation_tiles)
		//target->draw(tile);
	for (const auto& tile : this->other_tiles)
		target->draw(tile);
	for (const auto& tile : this->collide_tiles)
		target->draw(tile);
}
	