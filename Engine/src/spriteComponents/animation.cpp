#include "Animation.h"
#include <physicalComponents/sprite.h>

#include <engineComponents/input.h>

// Constructor / Destructor

s2d::Animation::Animation()
{
	this->loop = false;
	this->total_time_passed = 0.0f;
	this->total_frame_passed = 0.0f;
	this->current_frame = 0;
	this->is_playing = false;
	this->ptr_applied_sprite = nullptr;
	this->time_passed = 0.0f;
}

s2d::Animation::Animation(Sprite* ptr_appliedSprite, const std::string& name, const std::string fileLocation, const std::vector<s2d::KeyFrame>& frames)
{
	this->total_frame_passed = 0.0f;
	this->m_base_path = ptr_appliedSprite->sprite_renderer.path;
	this->m_path_to_file = fileLocation;
	this->time_passed = 2.0f;
	this->current_frame = -1;
	this->m_name = name;
	this->ptr_applied_sprite = ptr_appliedSprite;
	this->is_playing = false;
	this->total_time_passed = 0.0f;
	this->m_keyframes.resize(frames.size());
	this->loop = false;

	int currentPos = 0;
	for (int i = 0; i < frames.size(); i++)
	{
		this->m_keyframes[i].path = frames[i].path;
		this->m_keyframes[i].delay = frames[i].delay;
		currentPos += (int)this->m_keyframes[i].delay;
		this->m_keyframes[i].position = currentPos;
	}
	this->realoadTextures();
}

s2d::Animation::Animation(s2d::Sprite* ptr_applied_sprite, const s2d::Animation& animation)
{
	this->ptr_applied_sprite = ptr_applied_sprite;
	this->initCopyCtor(animation);
}

s2d::Animation::Animation(s2d::Sprite* ptr_applied_sprite, const s2d::Animation& animation, const std::string& name)
{
	this->ptr_applied_sprite = ptr_applied_sprite;
	this->initCopyCtor(animation);
	this->m_name = name;

	// renaming the path_to_file



	const std::string new_path = std::renamePartOnPath(this->m_path_to_file, this->m_name, "\\", EXTENSION_ANIMATION_FILE, 1);
	this->m_path_to_file = new_path;
}


void s2d::Animation::initCopyCtor(const s2d::Animation& animation)
{
	this->ptr_applied_sprite = ptr_applied_sprite;
	this->m_name = animation.m_name;
	this->m_base_path = this->ptr_applied_sprite->sprite_renderer.path;
	this->m_path_to_file = animation.getUserPathToFile();
	this->loop = animation.loop;

	const std::vector<s2d::KeyFrame>& keyframes = animation.getKeyFrames();
	for (size_t i = 0; i < keyframes.size(); i++)
	{
		s2d::KeyFrame copy = s2d::KeyFrame(keyframes[i]);
		this->m_keyframes.push_back(copy);
	}

	this->realoadTextures();
}

// Public methods

void s2d::Animation::deleteKeyFrame(const int pos)
{
	float delay = 0;
	for (int i = 0; i < this->m_keyframes.size(); i++)
	{
		delay += this->m_keyframes[i].delay;

		if (delay == pos)
		{
			float delayAdd = this->m_keyframes[i].delay;

			std::removeAtVec<s2d::KeyFrame>(this->m_keyframes, i);
			std::removeAtVec<sf::Texture>(this->m_textures, i);

			if (i + 1 > this->m_keyframes.size())
			{
				return;
			}
			this->m_keyframes[i].delay += delayAdd;

			return;
		}
	}
}

void s2d::Animation::play()
{
	this->m_base_path = this->ptr_applied_sprite->sprite_renderer.path;
	this->time_passed = 0.0f;
	this->total_frame_passed = 0;
	this->total_time_passed = 0.0f;
	this->current_frame = 0;
	this->is_playing = true;
}

void s2d::Animation::update()
{
	this->time_passed += Time::s_delta_time;
	this->total_time_passed += Time::s_delta_time;
	if (this->m_keyframes.size() == 0)
	{
		return;
	}
	const float condition = this->m_keyframes[current_frame].delay / 100;
	if (this->time_passed >= condition)
	{
		this->total_frame_passed++;
		this->time_passed = 0;
		this->ptr_applied_sprite->setSpriteTexture(this->m_textures[current_frame], this->m_keyframes[current_frame].path);
		this->current_frame++;
		if (this->current_frame == this->m_keyframes.size())
		{		
			if(this->loop)
			{
				// Hard coding the path bc skill issue
				this->ptr_applied_sprite->sprite_renderer.path = this->m_base_path;
				this->play();
			}
			else
			{
				this->stop();
			}
		}
	}
}

void s2d::Animation::stop()
{
	this->time_passed = 0.0f;
	this->total_frame_passed = 0;
	this->total_time_passed = 0.0f;
	this->current_frame = -1;
	this->is_playing = false;

	this->ptr_applied_sprite->setSpriteTexture(this->m_base_path);
}

s2d::KeyFrame& s2d::Animation::getKeyFrameAtMs(const float ms)
{
	float delay = 0;
	for (int i = 0; i < this->m_keyframes.size(); i++)
	{
		if (delay == ms)
		{
			return this->m_keyframes[i];
		}

		delay += this->m_keyframes[i].delay;
	}

	return this->m_keyframes[0];
}

const std::string s2d::Animation::getEnginePathToFile() const
{
	return s2d::EngineData::s_path_to_user_project + "\\" +this->m_path_to_file;
}

float s2d::Animation::getAnimationTime() const
{
	float time = 0.0f;
	for (size_t i = 0; i < this->m_keyframes.size(); i++)
	{
		time += this->m_keyframes[i].delay;
	}
	return time;
}

float s2d::Animation::getTimeTillFrame(size_t frame)
{
	if (frame == -1)
	{
		return -1;
	}
	float time = 0.0f;

	for (size_t i = 0; i < frame; i++)
	{
		time += this->m_keyframes[i].delay;
	}	
	return time;
}

void s2d::Animation::addKeyFrameAt(const int vecpos, const s2d::KeyFrame& frame)
{
	if (this->is_playing)
	{
		std::cout << "LOG: [WARNINING] Cant add keyframes while playing animation! " << frame.path << std::endl;
		return;
	}

	sf::Texture text;

	if (!text.loadFromFile(frame.path))
	{
		std::cout << "LOG: [ERROR] Cant read pixels from path " << frame.path << std::endl;
		return;
	}

	this->m_base_path = this->ptr_applied_sprite->sprite_renderer.path;
	this->m_keyframes.insert(this->m_keyframes.begin() + vecpos, frame);
	this->m_textures.insert(this->m_textures.begin() + vecpos, text);
}

void s2d::Animation::updateAllAnimations(s2d::SpriteRepository& repo)
{
	for (int i = 0; i < repo.amount(); i++)
	{
		s2d::Sprite* const sprite = repo.readAt(i);
		sprite->animator.update();
	}
}

void s2d::Animation::realoadTextures()
{
	this->m_textures.clear();
	this->m_textures = std::vector<sf::Texture>(this->m_keyframes.size());

	for (int i = 0; i < this->m_keyframes.size(); i++)
	{
		if (!this->m_textures[i].loadFromFile(this->m_keyframes[i].path))
		{
			std::cout << "LOG [ERROR] Couldnt load the animation file " + this->m_keyframes[i].path + " !";
		}
	}
}

