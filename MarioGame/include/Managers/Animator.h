#pragma once

#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include <functional>
#include <iostream>
#include <map>

#include "Algorythms/Math.h"

struct Animation
{
    const std::string name;
    
    int priority = 0;
    bool is_playing = false;
    const std::function<bool()> condition;
    bool is_finished = false;

    Animation(const std::string& name, int prio, const std::function<bool()>& condition) : name(name), priority(prio), condition(condition) {}
    virtual ~Animation() = default;

    virtual void play(float timer, float deltaTime, bool* end = nullptr) {}
};

struct AnimationSequence : public Animation
{
    std::vector<std::shared_ptr<Animation>> animations;
    std::vector<float> timing;
    float last_play_time;

    bool* bools = new bool[animations.size()];

    AnimationSequence(std::vector<std::shared_ptr<Animation>>&& animations, std::vector<float>& timing, std::function<bool()> condition, const std::string& name, const int prior)
        : Animation(name, prior, condition), timing(timing), last_play_time(0.f), animations(std::move(animations))
    {
        for (int i = 0; i < this->animations.size(); i++) 
        {
            bools[i] = true;
        }
    }

    virtual ~AnimationSequence()
    {
        delete[] bools;
    }

    void play(float timer, float deltaTime, bool* end = nullptr) override
    {
        is_finished = false;
        //Update timer
        for (auto& timer : timing)
        {
            timer -= deltaTime;
            if (timer < 0.f)
                timer = 0.f;
        }
        
        // Play each animation
        is_playing = true;
        for(int i = 0; i < animations.size(); i++)
        {
            if(bools[i] && timing[i] <= 0.f)
                animations[i]->play(timer, deltaTime, &bools[i]);
        }

        //Check if all animations are finished
        bool is_all_animations_finished = true;
        for (int i = 0; i < animations.size(); i++)
        {
            if (bools[i]) is_all_animations_finished = false;
        }

        if (is_all_animations_finished && end != nullptr)
        {
            *end = false;
            is_playing = false;
        }

        is_finished = false;
    }

    std::shared_ptr<Animation> getAnimation(int index)
    {
        return animations[index];
    }
};

struct FrameAnimation : public Animation
{
    sf::Sprite& sprite;
    int frame_width;
    int frame_height;

    bool is_looped; 

    std::vector<int> animation_frames;
    int current_frame;
    
    std::function<int()> get_direction;
    float animation_speed;

    float last_play_time;


    FrameAnimation(sf::Sprite& sprite, int w, int h, const std::vector<int>& frames, float speed, const std::function<bool()>& condition,
         const std::function<int()>& get_direction, bool is_looped, int prior , const std::string& name)
        : Animation(name, prior, condition), 
        sprite(sprite),
        frame_width(w),
        frame_height(h), 
        animation_frames(frames), get_direction(get_direction),
        animation_speed(speed), last_play_time(0.f), current_frame(0), is_looped(is_looped) {
    }

    void play(float timer, float deltaTime, bool* end = nullptr) override
    {
        //Check timer
        if (last_play_time + animation_speed < timer)
        {
            if (sprite.getTexture() == nullptr)
                return;
            
            //Play anim
            is_finished = false;
            is_playing = true;
            int dir = get_direction();
            
            //Get x any y coordinate of the frame
            int columnCount = static_cast<int>(sprite.getTexture()->getSize().x / frame_width);
            int x = animation_frames[current_frame] % columnCount;
            int y = animation_frames[current_frame] / columnCount;

            //Set textureRect according to the direction
            sprite.setTextureRect(sf::IntRect(
                dir == 1 ? frame_width * x : frame_width * x + frame_width,
                frame_height * y,
                dir * frame_width,
                frame_height));

            current_frame++;

            // Loop or end animation
            if (current_frame > animation_frames.size() - 1 && is_looped)
            {
                current_frame = 0;
            }
            if (current_frame > animation_frames.size() - 1 && !is_looped)
            {
                current_frame -= 1;
                //End animation
                if (end != nullptr)
                {
                    current_frame = 0;
                    is_finished = true;
                    *end = false;
                }
            }

            last_play_time = timer;
        }
        else
        {
            is_playing = false;
            
        }
    }
};

struct PosAnimation : public Animation
{
    sf::Sprite& sprite;
    int frame_width;
    int frame_height;

    bool is_looped;

    //std::pair<sf::Vector2f, sf::Vector2f> pos;
    std::vector<sf::Vector2f> positions;
    int currentPos = 0;

    float animation_speed;

    float last_play_time;

    //bool is_playing = false;
    bool is_moving_to_second = true;


    PosAnimation(sf::Sprite& sprite, int width, int height, float speed, std::function<bool()> condition,
        bool is_looped, int prior, std::vector<sf::Vector2f> positions, const std::string& name)
        : Animation(name,prior, condition), sprite(sprite), frame_width(width), frame_height(height),
        animation_speed(speed), last_play_time(0.f), is_looped(is_looped), positions(positions) {
    }

    void play(float timer, float deltaTime, bool* end = nullptr) override
    {
        is_playing = true;
        is_finished = false;
       
        // check direction to the point
        sf::Vector2f dir = positions[currentPos] - sprite.getPosition();
        // measure distance
        float distance = std::sqrt(dir.x * dir.x + dir.y * dir.y);

        // if we arrived to the point
        if (fabs(distance) <= 0.1f)
        {
            // callibrate position 
            sprite.setPosition(positions[currentPos]);
            currentPos++;

            //If it it the last point
            if (currentPos > positions.size() - 1)
            {
                is_playing = false;
                currentPos = 0;
                is_finished = true;
                if (end != nullptr)
                    *end = false;
            }
                    
        }
        else
        {
            //Move object towards the point
            // normalize direction (get unit vector)
            dir /= distance;
            // calculate step
            sf::Vector2f step = dir * std::min(distance, animation_speed * deltaTime);
            // set new position
            sprite.setPosition(sprite.getPosition() + step);
        }
    }
};

class Animator
{
private:
    int current_priority = -1;

    float timer;

    Animation* currentAnim;

    std::vector<std::shared_ptr<Animation>> animations;

    float deltaTime;

    bool play_anim;
    std::string anim_name;

    std::map<std::string, std::shared_ptr<Animation>> animations_map;

public:
    Animator();
	virtual ~Animator();

    void addFrameAnimation(sf::Sprite& sprite, float w, float h, const std::vector<int>& frames, float speed, const std::function<bool()>& condition, const std::function<int()> &get_direction, bool is_looped, int prior, const std::string& name);
    void addPosAnimation(sf::Sprite& sprite, float w, float h, float speed, std::function<bool()> condition, bool is_looped, int prior, std::vector<sf::Vector2f> positions, const std::string& name);
    void addAnimationSequence(std::vector<std::shared_ptr<Animation>>&& animations, std::vector<float>& timing, std::function<bool()> condition, const std::string& name, int prior);

    std::shared_ptr<Animation> getAnim(const std::string& name);
    void playAnim(const std::string& name);

    const sf::FloatRect getCurrentFrame();

    const bool isPlayed() const;

    void update(float deltaTime);
};

