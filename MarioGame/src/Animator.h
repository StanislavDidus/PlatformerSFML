#pragma once

#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include <functional>

struct Animation
{
    int priority = 0;
    std::function<bool()> condition;

    Animation(int prio, std::function<bool()> condition) : priority(prio), condition(condition) {}
    virtual ~Animation() = default;

    virtual void play(float timer, float deltaTime) {}
};

struct FrameAnimation : public Animation
{
    sf::Sprite& sprite;
    int frame_width;
    int frame_height;

    bool is_looped; 

    std::pair<int, int> animation_frames;
    int current_frame;
    
    std::function<int()> get_direction;
    float animation_speed;

    float last_play_time;


    FrameAnimation(sf::Sprite& sprite, float width, float height, std::pair<int, int>& frames, float speed, std::function<bool()> condition,
         std::function<int()> get_direction, bool is_looped, int prior)
        : Animation(prior, condition), sprite(sprite), frame_width(width), frame_height(height), animation_frames(frames), get_direction(get_direction),
        animation_speed(speed), last_play_time(0.f), current_frame(frames.first), is_looped(is_looped) {
    }

    void play(float timer, float deltaTime) override
    {
        if (last_play_time + animation_speed < timer)
        {
            //Play anim
            //if(!anim.is_reversed)
                //anim.sprite.setTextureRect(sf::IntRect(anim.frame_width * anim.current_frame, 0, anim.frame_width, anim.frame_height));
            //else

            int dir = get_direction();
            //this->sprite.setTextureRect(sf::IntRect((dir - 1) * (anim.frame_width / 2 * -1) + anim.current_frame * anim.frame_width, 0, dir * anim.frame_width, anim.frame_height));
            this->sprite.setTextureRect(sf::IntRect(dir == 1 ? frame_width * current_frame : frame_width * current_frame + frame_width, 0, dir * frame_width, frame_height));
            current_frame++;

            if (current_frame > animation_frames.second && is_looped)
                current_frame = animation_frames.first;

            if (current_frame > animation_frames.second && !is_looped)
                current_frame = animation_frames.second;

            last_play_time = timer;
        }
    }
};

struct PosAnimation : public Animation
{
    sf::Sprite& sprite;
    int frame_width;
    int frame_height;

    bool is_looped;

    std::pair<sf::Vector2f, sf::Vector2f> pos;

    float animation_speed;

    float last_play_time;


    PosAnimation(sf::Sprite& sprite, float width, float height, float speed, std::function<bool()> condition,
        bool is_looped, int prior, std::pair<sf::Vector2f, sf::Vector2f> pos)
        : Animation(prior, condition), sprite(sprite), frame_width(width), frame_height(height),
        animation_speed(speed), last_play_time(0.f), is_looped(is_looped), pos(pos) {
    }

    void play(float timer, float deltaTime) override
    {
        if (last_play_time + animation_speed < timer)
        {
            //Play anim
            //if(!anim.is_reversed)
                //anim.sprite.setTextureRect(sf::IntRect(anim.frame_width * anim.current_frame, 0, anim.frame_width, anim.frame_height));
            //else
            //this->sprite.setTextureRect(sf::IntRect((dir - 1) * (anim.frame_width / 2 * -1) + anim.current_frame * anim.frame_width, 0, dir * anim.frame_width, anim.frame_height));
            //this->sprite.setTextureRect(sf::IntRect(dir == 1 ? frame_width * current_frame : frame_width * current_frame + frame_width, 0, dir * frame_width, frame_height));
            //current_frame++;

            
            last_play_time = timer;
        }
    }
};

class Animator
{
private:
    sf::Sprite& sprite;
    int frame_width;
    int frame_height;

    int current_priority = -1;

    float timer;

    std::vector<std::unique_ptr<Animation>> animations;

    float deltaTime;

public:
    Animator(sf::Sprite& sprite, int frame_width, int frame_height);
	virtual ~Animator();

    void addFrameAnimation(int firstFrame, int lastFrame, float speed, const std::function<bool()>& condition, const std::function<int()> &get_direction, bool is_looped, int prior);
   
    void update(float deltaTime);
};

