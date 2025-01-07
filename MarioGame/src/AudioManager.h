#pragma once

#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>


class AudioManager
{
private:
	std::vector<std::unique_ptr<sf::SoundBuffer>> buffers;
	std::unordered_map<std::string, sf::Sound> sounds;
public:
	AudioManager();
	virtual ~AudioManager();

	void addSound(const std::string& name, const std::string& soundPath, bool loop);
	void playSound(const std::string& name);
	void update();
};

