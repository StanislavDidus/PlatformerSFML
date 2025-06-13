#pragma once

#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <unordered_map>

class TextureManager
{
private:
	std::unordered_map<std::string, std::shared_ptr<sf::Texture>> textures;
public:
	TextureManager() = default;
	virtual ~TextureManager() = default;

	void load(const std::string& name, const std::string& path)
	{
		std::shared_ptr<sf::Texture> texture = std::make_shared<sf::Texture>();
		if (!texture->loadFromFile(path))
		{
			std::cout << "Could not load a texture: " << path << "\n";
			return;
		}
		textures[name] = texture;
	}

	std::shared_ptr<sf::Texture> get(const std::string& name)
	{
		return textures.at(name);
	}
};

