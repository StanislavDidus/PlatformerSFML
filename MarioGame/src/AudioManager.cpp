#include "Managers/AudioManager.h"

//Con/Des
AudioManager::AudioManager()
{

}

AudioManager::~AudioManager()
{

}

//Functions
void AudioManager::addSound(const std::string& name, const std::string& soundPath, bool loop)
{
	std::unique_ptr<sf::SoundBuffer> buffer = std::make_unique<sf::SoundBuffer>();
	if (!buffer->loadFromFile(soundPath))
		std::cout << "ERROR::MAP::ADDOBJECT::Could not load sound: " << soundPath << "\n";

	sf::Sound sound;
	sound.setBuffer(*buffer);
	sound.setLoop(loop);

	buffers.push_back(std::move(buffer));
	sounds[name] = sound;
}

void AudioManager::playSound(const std::string& name)
{
	sounds[name].play();
}

void AudioManager::update()
{

}
