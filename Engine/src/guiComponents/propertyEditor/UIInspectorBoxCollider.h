#pragma once

#include <physicalComponents/sprite.h>
#include <ImGui.h>
#include <icons.h>
#include <UIAssetFolder.h>
#include <manager/fontManager.h>

namespace s2d
{
	class UIInspectorBoxCollider
	{
	private:	
		sf::Texture m_texture;

	public:
		UIInspectorBoxCollider();

		void init();

		void edit(float& x, float& y);
	    void solid(float& x, float& y, s2d::Sprite* sprite);
		void width(float x, float y, s2d::Sprite* sprite);
		void height(s2d::Sprite* sprite);
		void drawBoxCollider(s2d::Sprite* sprite);
	};
}