#pragma once

#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include <functional>
#include <iostream>
#include <map>

#include "Math.h"

struct Animation
{
    std::string name;
    
    int priority = 0;
    bool is_playing = false;
    std::function<bool()> condition*;

    Animation(const std::string& name, int prio, std::function<bool()>* condition = nullptr) : name(name), priority(prio), condition(condition) {}
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
        for (int i = 0; i < animations.size(); i++) 
        {
            bools[i] = true;
        }
    }

    void play(float timer, float deltaTime, bool* end = nullptr) override
    {
        //Create an array of boolians
        

        //Initialize array

        //Update timer
        for (auto& timer : this->timing)
        {
            timer -= deltaTime;
            if (timer < 0.f)
                timer = 0.f;
        }
        
        // Play each animation
        this->is_playing = true;
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
            this->is_playing = false;
        }

        //Clear memory
        
    }

    std::shared_ptr<Animation> getAnimation(int index)
    {
        return this->animations[index];
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
    
    std::function<int()>* get_direction;
    float animation_speed;

    float last_play_time;


    FrameAnimation(sf::Sprite& sprite, std::vector<int>& frames, float speed, std::function<bool()>* condition = nullptr,
         std::function<int()>* get_direction = nullptr, bool is_looped, int prior , const std::string& name)
        : Animation(name, prior, condition), sprite(sprite), animation_frames(frames), get_direction(get_direction),
        animation_speed(speed), last_play_time(0.f), current_frame(frames[0]), is_looped(is_looped) {
    }

    void play(float timer, float deltaTime, bool* end = nullptr) override
    {
        if (last_play_time + animation_speed < timer)
        {
            //Play anim
            //std::cout << "play pos\n";
            //if(!anim.is_reversed)
                //anim.sprite.setTextureRect(sf::IntRect(anim.frame_width * anim.current_frame, 0, anim.frame_width, anim.frame_height));
            //else
            this->is_playing = true;
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
        else
        {
            this->is_playing = false;
            //this->sprite.setTextureRect(sf::IntRect(0, 0, frame_width, frame_height));
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

    //bool is_playing = false;
    bool is_moving_to_second = true;


    PosAnimation(sf::Sprite& sprite, float width, float height, float speed, std::function<bool()> condition,
        bool is_looped, int prior, std::pair<sf::Vector2f, sf::Vector2f> pos, const std::string& name)
        : Animation(name,prior, condition), sprite(sprite), frame_width(width), frame_height(height),
        animation_speed(speed), last_play_time(0.f), is_looped(is_looped), pos(pos) {
    }

    void play(float timer, float deltaTime, bool* end = nullptr) override
    {
        if (true)
        {
            is_playing = true;
            // move to second point
            if (is_moving_to_second)
            {
                // check direction to the point
                sf::Vector2f dir = this->pos.second - this->sprite.getPosition();
                // measure distance
                float distance = std::sqrt(dir.x * dir.x + dir.y * dir.y);

                // if we arrived to second point
                if (fabs(distance) <= 0.1f)
                {
                    // callibrate position 
                    this->sprite.setPosition(this->pos.second);
                    // move backwards
                    is_moving_to_second = false;
                }
                else
                {
                    // normalize direction (get unit vector)
                    dir /= distance;
                    // step
                    sf::Vector2f step = dir * std::min(distance, this->animation_speed * deltaTime);
                    // set new position
                    this->sprite.setPosition(this->sprite.getPosition() + step);
                }
            }
            else
            {
                // move to the first point
                sf::Vector2f dir = this->pos.first - this->sprite.getPosition();
                // measure distance
                float distance = std::sqrt(dir.x * dir.x + dir.y * dir.y);

                // if arrived to the first point
                if (fabs(distance) <= 0.1f)
                {
                    // callibrate position
                    this->sprite.setPosition(this->pos.first);
                    // finish animation
                    is_playing = false;
                    this->is_moving_to_second = true;
                    if (end != nullptr)
                        *end = false;
              
                }
                else
                {
                    // normalize direction
                    dir /= distance;
                    // step
                    sf::Vector2f step = dir * std::min(distance, this->animation_speed * deltaTime);
                    // move by step
                    this->sprite.setPosition(this->sprite.getPosition() + step);
                }
            }
        }
    }
};

class Animator
{
private:
    int current_priority = -1;

    float timer;

    std::vector<std::shared_ptr<Animation>> animations;

    float deltaTime;

    bool play_anim;
    std::string anim_name;

    std::map<std::string, std::shared_ptr<Animation>> animations_map;

public:
    Animator();
	virtual ~Animator();

    void addFrameAnimation(sf::Sprite& sprite, int w, int h, int firstFrame, int lastFrame, float speed, const std::function<bool()>& condition, const std::function<int()> &get_direction, bool is_looped, int prior, const std::string& name);
    void addPosAnimation(sf::Sprite& sprite, int w, int h, float speed, std::function<bool()> condition, bool is_looped, int prior, std::pair<sf::Vector2f, sf::Vector2f> pos, const std::string& name);
    void addAnimationSequence(std::vector<std::shared_ptr<Animation>>&& animations, std::vector<float>& timing, std::function<bool()> condition, const std::string& name, int prior);

    std::shared_ptr<Animation> getAnim(const std::string& name);
    void playAnim(const std::string& name);

    const bool isPlayed() const;

    void update(float deltaTime);
};

