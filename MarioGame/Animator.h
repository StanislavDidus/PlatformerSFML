#pragma once

#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include <functional>

struct Animation
{
    sf::Sprite& sprite;
    int frame_width;
    int frame_height;

    bool is_looped; 
    
    std::pair<int, int> animation_frames;
    int current_frame;
    std::function<bool()> condition;
    std::function<int()> get_direction;
    float animation_speed;

    float last_play_time;


    Animation(sf::Sprite& sprite, float width, float height, std::pair<int, int>& frames, float speed, std::function<bool()> condition,
         std::function<int()> get_direction, bool is_looped)
        : sprite(sprite), frame_width(width), frame_height(height), animation_frames(frames), get_direction(get_direction),
        animation_speed(speed), condition(condition), last_play_time(0.f), current_frame(frames.first), is_looped(is_looped) {
    }
};

class Animator
{
private:
    sf::Sprite& sprite;
    int frame_width;
    int frame_height;

    float timer;

    std::vector<Animation> animations;

public:
    Animator(sf::Sprite& sprite, int frame_width, int frame_height);
	virtual ~Animator();

    void addAnimation(int firstFrame, int lastFrame, float speed, const std::function<bool()>& condition, const std::function<int()> &get_direction, bool is_looped);
    
    void update(float deltaTime);
};

