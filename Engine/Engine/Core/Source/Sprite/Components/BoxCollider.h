#pragma once

#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <math.h>

#include "Math/Vector2.h"
#include "Sprite/Components/Component.h"
#include "Sprite/Components/Light/LightRepository.h"
#include "Camera/Camera.h"

namespace spe
{
	class SpriteRepository;
	class Sprite;
	class BoxCollider : public spe::Component
	{
	private:
		int m_Start = -5;
		int m_end = 5;

		bool m_GotLeft;
		bool m_GotRight;
		bool m_GotUp;
		bool m_GotDown;

		void Init() override;
		bool CheckCollision(spe::BoxCollider& other);
		void CheckCollisionPosition(spe::BoxCollider& other) noexcept;
		void ResetPosition() noexcept;

	public:
		bool IsSolid;
		bool CanCollide;
		bool CollidedInFrame;
		std::unordered_map<uint32_t, spe::Sprite*> CollidedSprites;

		static spe::Sprite* s_ptr_CameraCollider;

		bool Left;
		bool Right;
		bool Up;
		bool Down;
		bool Collided;

		//Deleting the pointer in gameEngine.cpp!
		spe::Sprite* ptr_Sprite;

		Vector2 Width;
		Vector2 Height;
		bool Controller;

		BoxCollider();
		BoxCollider(spe::Sprite* sprite, spe::BoxCollider& rhs);

		//Giving it a pointer, so we dont have to update it consistently
		BoxCollider(spe::Sprite* ptr_sprite);

		void Reset() override;
		void Update(spe::SpriteRepository& tocheck);

		// User utility

		/// <summary>
		/// Iterates over the collided sprites map to find if it collided with a ceratain tag (kinda inefficent)
		/// </summary>
		/// <param name="tag">To search for</param>
		/// <returns>True if the sprite collides with the provided tag</returns>
		spe::Sprite* CollidedWithTag(const std::string& tag);

		spe::Sprite* CollidedWithName(const std::string& name);

		static bool ProcessSprite(spe::Sprite* sprite, const spe::Camera& camera);
		static void InitCameraCollider(spe::LightRepository& repo);
		static void DeleteCameraCollider();
	};
}

