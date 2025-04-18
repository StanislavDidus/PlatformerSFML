#include "Animator.h"

//Con/Des
Animator::Animator()
{
	this->timer = 0.f;
}

Animator::~Animator()
{

}

//Functions
void Animator::addFrameAnimation(sf::Sprite& sprite, int w, int h, const std::vector<int>& frames, float speed, const std::function<bool()>& condition, const std::function<int()>& get_direction, bool is_looped, int prior, const std::string& name)
{
	//std::pair<int, int> frames = {firstFrame, lastFrame};
	std::shared_ptr<FrameAnimation> anim = std::make_shared<FrameAnimation>(sprite, w, h, frames, speed, condition, get_direction, is_looped, prior, name);
	this->animations.emplace_back(anim);
	this->animations_map[name] = anim;
}

void Animator::addPosAnimation(sf::Sprite& sprite, int w, int h, float speed, std::function<bool()> condition, bool is_looped, int prior, std::vector<sf::Vector2f> positions, const std::string& name)
{
	std::shared_ptr<PosAnimation> anim = std::make_shared<PosAnimation>(sprite, sprite.getGlobalBounds().width / 3.125f, sprite.getGlobalBounds().height / 3.125f, speed, condition, is_looped, prior, positions, name);
	this->animations.emplace_back(anim);
	this->animations_map[name] = anim;
}

void Animator::addAnimationSequence(std::vector<std::shared_ptr<Animation>>&& animations, std::vector<float>& timing, std::function<bool()> condition, const std::string& name, int prior)
{
	std::shared_ptr<AnimationSequence> anim = std::make_shared<AnimationSequence>(std::move(animations), timing, condition, name, prior);
	this->animations.emplace_back(anim);
	this->animations_map[name] = anim;
}

std::shared_ptr<Animation> Animator::getAnim(const std::string& name)
{
	if (this->animations_map.find(name) != this->animations_map.end())
	{
		return this->animations_map[name];
	}

	return nullptr;
}

void Animator::playAnim(const std::string& name)
{
	if (this->animations_map.find(name) != this->animations_map.end())
	{
		this->play_anim = true;
		this->anim_name = name;
	}
}

const bool Animator::isPlayed() const
{
	return this->play_anim;
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
		if (this->animations_map.find(this->anim_name) != this->animations_map.end())
		{
			this->animations_map[this->anim_name]->play(this->timer, this->deltaTime, &this->play_anim);
			if (!this->play_anim)
				this->anim_name = "";
		}
	}
}
