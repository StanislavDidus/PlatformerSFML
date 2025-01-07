#include "Animator.h"

//Con/Des
Animator::Animator(sf::Sprite& sprite, int frame_width, int frame_height) : sprite(sprite),
frame_width(frame_width), frame_height(frame_height)
{
	this->timer = 0.f;
}

Animator::~Animator()
{

}

//Functions
void Animator::addAnimation(int firstFrame, int lastFrame, float speed, const std::function<bool()>& condition, const std::function<int()>& get_direction, bool is_looped)
{
	std::pair<int, int> frames = {firstFrame, lastFrame};
	this->animations.emplace_back(this->sprite, this->frame_width, this->frame_height, frames, speed, condition, get_direction, is_looped);
}


void Animator::update(float deltaTime)
{
	this->timer += deltaTime;
	
	for (Animation& anim : this->animations)
	{
		if (anim.condition())
		{
			if (anim.last_play_time + anim.animation_speed < this->timer)
			{
				//Play anim
				//if(!anim.is_reversed)
					//anim.sprite.setTextureRect(sf::IntRect(anim.frame_width * anim.current_frame, 0, anim.frame_width, anim.frame_height));
				//else
				int dir = anim.get_direction();
				//this->sprite.setTextureRect(sf::IntRect((dir - 1) * (anim.frame_width / 2 * -1) + anim.current_frame * anim.frame_width, 0, dir * anim.frame_width, anim.frame_height));
				this->sprite.setTextureRect(sf::IntRect(dir == 1 ? anim.frame_width * anim.current_frame : anim.frame_width * anim.current_frame + anim.frame_width, 0, dir * anim.frame_width, anim.frame_height));
				anim.current_frame++;

				if (anim.current_frame > anim.animation_frames.second && anim.is_looped)
					anim.current_frame = anim.animation_frames.first;

				if (anim.current_frame > anim.animation_frames.second && !anim.is_looped)
					anim.current_frame = anim.animation_frames.second;

				anim.last_play_time = this->timer;
			}
		}
		else
			anim.current_frame = anim.animation_frames.first;
	}
}
