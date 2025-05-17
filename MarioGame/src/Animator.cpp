#include "Managers/Animator.h"

//Con/Des
Animator::Animator()
{
	timer = 0.f;
	currentAnim = nullptr;
	deltaTime = 0.f;
	play_anim = "";
}

Animator::~Animator()
{

}

//Functions


void Animator::addFrameAnimation(sf::Sprite& sprite, float w, float h, const std::vector<int>& frames, float speed, const std::function<bool()>& condition, const std::function<int()>& get_direction, bool is_looped, int prior, const std::string& name)
{
	std::shared_ptr<FrameAnimation> anim = std::make_shared<FrameAnimation>(sprite, static_cast<int>(w), static_cast<int>(h), frames, speed, condition, get_direction, is_looped, prior, name);
	animations.emplace_back(anim);
	animations_map[name] = anim;
}

void Animator::addPosAnimation(sf::Sprite& sprite, float w, float h, float speed, std::function<bool()> condition, bool is_looped, int prior, std::vector<sf::Vector2f> positions, const std::string& name)
{
	std::shared_ptr<PosAnimation> anim = std::make_shared<PosAnimation>(sprite, static_cast<int>(w), static_cast<int>(h), speed, condition, is_looped, prior, positions, name);
	animations.emplace_back(anim);
	animations_map[name] = anim;
}

void Animator::addAnimationSequence(std::vector<std::shared_ptr<Animation>>&& animations, std::vector<float>& timing, std::function<bool()> condition, const std::string& name, int prior)
{
	std::shared_ptr<AnimationSequence> anim = std::make_shared<AnimationSequence>(std::move(animations), timing, condition, name, prior);
	animations.emplace_back(anim);
	animations_map[name] = anim;
}

std::shared_ptr<Animation> Animator::getAnim(const std::string& name)
{
	if (animations_map.find(name) != animations_map.end())
	{
		return animations_map[name];
	}

	return nullptr;
}

void Animator::playAnim(const std::string& name)
{
	if (animations_map.find(name) != animations_map.end())
	{
		play_anim = true;
		anim_name = name;
	}
}

const sf::FloatRect Animator::getCurrentFrame()
{
	Animation* anim = currentAnim;

	if (anim == nullptr)
		return sf::FloatRect();

	FrameAnimation* frameAnim = dynamic_cast<FrameAnimation*>(anim);

	if (frameAnim == nullptr)
	{
		std::cout << "frame\n";
		return sf::FloatRect();
	}

	int dir = frameAnim->get_direction();
	if (dir == 0) dir = 1;

	int columnCount = static_cast<int>(frameAnim->sprite.getTexture()->getSize().x / frameAnim->frame_width);
	int x = frameAnim->animation_frames[frameAnim->current_frame] % columnCount;
	int y = frameAnim->animation_frames[frameAnim->current_frame] / columnCount;

	float left = static_cast<float>(frameAnim->frame_width * x);
	float width = static_cast<float>(frameAnim->frame_width);

	return sf::FloatRect(
		left,
		static_cast<float>(frameAnim->frame_height * y),
		width,
		static_cast<float>(frameAnim->frame_height));

	/*return sf::FloatRect(
		left,
		frameAnim->frame_height * y,
		width,
		frameAnim->frame_height
	);*/
}

const bool Animator::isPlayed() const
{
	return play_anim;
}

void Animator::update(float deltaTime)
{
	timer += deltaTime;
	deltaTime = deltaTime;

	std::vector<Animation*> prior_anim;
	for (auto& anim : animations)
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
			anim_highest_prior->play(timer, deltaTime);
			currentAnim = anim_highest_prior;
		}
		else
		{
			prior_anim[0]->play(timer, deltaTime);
			currentAnim = prior_anim[0];
		}
	}

	//Play last animation
	if (play_anim)
	{
		if (animations_map.find(anim_name) != animations_map.end())
		{
			animations_map[anim_name]->play(timer, deltaTime, &play_anim);
			currentAnim = animations_map[anim_name].get();
			if (!play_anim)
				anim_name = "";
		}
	}
}
