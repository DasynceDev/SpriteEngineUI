#pragma once

#include <SFML/Graphics.hpp>
#include <input.h>
#include <camera.h>
#include <stime.h>
#include <gameObject.h>
#include <fstream>
#include <stdExtension.h>

namespace s2d
{
	class UIRealTimeEditorNavigator
	{
	private:
		s2d::Transform m_cursor;
		s2d::Camera m_camera;

		// The white big as box
		sf::Texture m_windowRectangle_texture;
		sf::RectangleShape m_windowRectangle;
		short m_vecPos;

		float m_arrowSpeed;
		float m_scrollSpeed;

		bool* m_isAnyUIWindowHovered;
		sf::Event* m_ptr_event_engine;
		sf::RenderWindow* m_ptr_renderWindow;

		void navigateRightClick();
		void navigateScrollWheel();
		void navigateArrows();

		void calculateScrollWheelSpeed();

		void loadCameraSettingsFromFile();
		void setChangedPosition();
		void setWhiteBox();

	public:
		UIRealTimeEditorNavigator();
		UIRealTimeEditorNavigator(sf::RenderWindow& renderWindow, sf::Event* event, bool* isAnyUIWindowHovered);

		void update();
	};
}
