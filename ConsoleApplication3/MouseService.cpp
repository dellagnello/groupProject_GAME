#include "MouseService.h"


MouseService::MouseService(void)
{
}


MouseService::~MouseService(void)
{
}

sf::Vector2i MouseService::getMousePosition() {
	//sf::Vector2i mousePos = sf::Mouse::getPosition();

	/////message
	return sf::Mouse::getPosition();
}


sf::Vector2f MouseService::getMouseUnitVector( sf::Vector2f centerOfScreen ) {
	sf::Vector2i mousePos = sf::Mouse::getPosition();

	sf::Vector2f mouseOffset;

	mouseOffset = (sf::Vector2f)mousePos - centerOfScreen;

	float normalizer = sqrt( mouseOffset.x * mouseOffset.x + mouseOffset.y * mouseOffset.y );
	mouseOffset = mouseOffset / normalizer;

	return mouseOffset; // normalized
}
