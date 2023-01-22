#include "Testing.h"

//User classes

void Testing::start()
{
	this->m_spriteSpeed = 800;
	this->m_score = 0;
	thisSprite = s2d::Sprite::getSpriteByName("Plane");

	up = s2d::Sprite::getSpriteByName("Up");
	down = s2d::Sprite::getSpriteByName("Down");
	camera = &s2d::GameObject::camera;
	this->m_collided = false;

}

void Testing::update()
{
	std::string fps = "fps " + std::to_string(s2d::Time::fps);
	ImGui::Text(fps.c_str());

	std::string score = "score " + std::to_string(m_score);
	ImGui::Text(score.c_str());

	
	if (s2d::Input::onKeyHold(s2d::KeyBoardCode::A))
	{
		thisSprite->transform.position.x -= this->m_spriteSpeed * s2d::Time::deltaTime;
	}
	if (s2d::Input::onKeyHold(s2d::KeyBoardCode::W))
	{
		thisSprite->transform.position.y += this->m_spriteSpeed * s2d::Time::deltaTime;
	}
	if (s2d::Input::onKeyHold(s2d::KeyBoardCode::S))
	{
		thisSprite->transform.position.y -= this->m_spriteSpeed * s2d::Time::deltaTime;
	}
	if (s2d::Input::onKeyHold(s2d::KeyBoardCode::D))
	{
		thisSprite->transform.position.x += this->m_spriteSpeed * s2d::Time::deltaTime;
	}
}