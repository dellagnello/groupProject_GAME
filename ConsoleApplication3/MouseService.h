#include <SFML/Graphics.hpp>


#pragma once
class MouseService
{
public:
	MouseService(void);
	~MouseService(void);

	static sf::Vector2i getMousePosition();
	static sf::Vector2f getMouseUnitVector( sf::Vector2f centerOfScreen );


};

