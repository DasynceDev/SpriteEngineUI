#include "Animator.h"
#include <physicalComponents/sprite.h>

// Constructor / Destructor

s2d::Animator::Animator()
{
	this->init();
}

s2d::Animator::Animator(Sprite* ptr_attachedSprite)
{
	this->ptr_attachedSprite = ptr_attachedSprite;
	this->init();
}

void s2d::Animator::init()
{
	this->exist = false;
	this->ptr_attachedSprite = nullptr;
	this->animationPlaying.name = "<Unknown>";
}

// Public functions

void s2d::Animator::createAnimation(const std::string& name, const std::string& fileLocation, const std::vector<s2d::KeyFrame>& textures)
{
	EXIST_COMPONENT;
	animations.insert({ name, Animation(ptr_attachedSprite, name, fileLocation, textures) });
}

void s2d::Animator::removeAnimation(const std::string& name)
{
	EXIST_COMPONENT;
	animations.erase(name);
}

void s2d::Animator::play(const std::string& name)
{
	EXIST_COMPONENT;
	auto it = this->animations.find(name);
	if (it != this->animations.end())
	{
		this->animationPlaying.name = name;
		this->animationPlaying.isAAnimationPlaying = true;
		it->second.play();
	}
}

void s2d::Animator::stop(const std::string& name)
{
	EXIST_COMPONENT;
	auto it = this->animations.find(name);
	if (it != this->animations.end())
	{
		this->animationPlaying.name = name;
		this->animationPlaying.isAAnimationPlaying = true;
		it->second.stop();
	}
}

void s2d::Animator::update()
{
	EXIST_COMPONENT;
	for (auto& anim : this->animations)
	{
		if (anim.second.isPlaying)
			anim.second.update();
	}
}

void s2d::Animator::reset()
{
	this->animationPlaying.name = "<Unknown>";;
	this->animationPlaying.isAAnimationPlaying = false;
	this->exist = false;
	this->animations.clear();
}

//Static functions

void s2d::Animator::stopAllAnimations(s2d::SpriteRepository& toUpdate)
{
	for (int i = 0; i < toUpdate.amount(); i++)
	{
		s2d::Sprite* const sprite = toUpdate.readAt(i);
		sprite->animator.stop(sprite->animator.animationPlaying.name);
	}
}
