#pragma once

#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <spriteComponents/spriteData.h>
#include <physicalComponents/gameObject.h>
#include <spriteComponents/boxCollider.h>
#include <SFML/Graphics.hpp>
#include <spriteComponents/physicsBody.h>
#include <stdExtension.h>

namespace s2d
{
	class Sprite
	{
	private:
		sf::Sprite m_sprite;
		sf::Texture m_texture;
		int m_vectorPosition;
		int m_id;
		int m_parentId;
		int m_childCount;

		/// <summary>
		/// This is only used if it is a child. It is the position in the vector
		/// from the parent sprite
		/// </summary>
		int m_childListPos;

		void initVariables(std::string name, s2d::Vector2 spawnPos, std::string path);
		void pushSetup();
		void pushSpriteFromCollider(s2d::BoxColliderPositionData::Position p, bool smaller, float& tXY, float& lXY, float& nXY);
		void setTextureSize();

	public:
		// General info
		std::string name;
		std::string path;
		int sortingLayerIndex;

		// Components
		s2d::BoxCollider collider;
		s2d::PhsysicsBody physicsBody;
		s2d::Transform transform;

		//Parent / child infos
		std::vector<s2d::Sprite*> childs;
		s2d::Sprite* parent;

		Sprite();
		Sprite(std::string name, s2d::Vector2 spawnPosition, std::string path);
		Sprite(std::string name, s2d::Vector2 spawnPosition, std::string path, bool addToWindowByConstruction);

		void setSpriteTexture(const std::string path);
		void setSpritePosition();

		/// <summary>
		/// Pushed the sprite to the sprites vector, gets rendered
		/// automaticly
		/// </summary>
		void addSpriteToScene();

		/// <summary>
		/// Gets the original position as a Cartesian coordinate system point (vector).
		/// </summary>
		/// <returns>The position as a vector</returns>
		s2d::Vector2 getOrigininalPosition();


		/// <summary>
		/// Renders the sprite instant to the screen when you create a 
		/// 'new' sprite, do not use this functions if you don't have a reason to 
		/// </summary>
		void renderInstant();
	public:

		void setId(const int id) { this->m_id = id; }
		void setVectorPosition(const int vec) { this->m_vectorPosition = vec; }

		int getId() const { return this->m_id; }
		int getParentId() const { return this->m_parentId; }
		int getChildCount() const { return this->m_childCount; }

		s2d::Sprite* getNode(); 

		int getVectorPosition() { return this->m_vectorPosition; }
		sf::Sprite& getSprite() { return this->m_sprite; }
		sf::Texture& getTexture() { return this->m_texture; }

	public:
		static void initActiveSprites();
		static int getMaxNumber(std::vector<s2d::Sprite*>& vec);
		static void updateHightestLayerIndex();
		static s2d::Sprite* getSpriteById(int id);
		static Sprite* getSpriteByName(std::string name);
		static void deleteSpriteByName(const std::string& name);

		static int highestLayerIndex;

		//Pointers getting deletet in gameWindow.cpp ~
		static std::vector<s2d::Sprite*> activeSprites;
	};
}
