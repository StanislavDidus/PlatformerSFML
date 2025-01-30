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
void Animator::addFrameAnimation(int firstFrame, int lastFrame, float speed, const std::function<bool()>& condition, const std::function<int()>& get_direction, bool is_looped, int prior, const std::string& name)
{
	std::pair<int, int> frames = {firstFrame, lastFrame};
	this->animations.emplace_back(std::make_unique<FrameAnimation>(this->sprite, this->frame_width, this->frame_height, frames, speed, condition, get_direction, is_looped, prior, name));
}

void Animator::addPosAnimation(float speed, std::function<bool()> condition, bool is_looped, int prior, std::pair<sf::Vector2f, sf::Vector2f> pos, const std::string& name)
{
	this->animations.emplace_back(std::make_unique<PosAnimation>(this->sprite, this->frame_width, this->frame_height, speed, condition, is_looped, prior, pos, name));
}

void Animator::playAnim(const std::string& name)
{
	for (const auto& anim : this->animations)
	{
		if (anim->name == name)
		{
			this->play_anim = true;
			this->anim_name = name;
		}
			//anim->play(0.f, deltaTime);
	}
}

void Animator::update(float deltaTime)
{
	this->timer += deltaTime;
	this->deltaTime = deltaTime;

	std::vector<Animation*> prior_anim;
	for (auto& anim : this->animations)
	{
		if (anim->condition())
		{
			prior_anim.emplace_back(anim.get()); 
		}
	}

	if (!prior_anim.empty())
	{
		if (prior_anim.size() > 1)
		{
			auto* anim_highest_prior = prior_anim[0];
			for (auto& anim1 : prior_anim)
			{
				if (anim1->priority > anim_highest_prior->priority)
				{
					anim_highest_prior = anim1;
				}
			}
			anim_highest_prior->play(this->timer, this->deltaTime);
		}
		else
		{
			prior_anim[0]->play(this->timer, this->deltaTime);
		}
	}

	//Play last animation
	if (this->play_anim)
	{
		for (const auto& anim : this->animations)
		{
			if (anim->name == this->anim_name)
			{
				anim->play(this->timer, this->deltaTime, &this->play_anim);
				if (!this->play_anim)
					this->anim_name = "";
			}
		}
	}
}
