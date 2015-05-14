#pragma once

#include <SFML/Graphics.hpp>
#include "BoxedObj.h"

class Portal : public BoxedObj
{
public:
	Portal(void);
	~Portal(void);

	void updateBox();

	void setPosition( int x, int y );
	void setSprite( sf::Texture& myTexture, sf::IntRect rect );
	sf::Sprite & getSprite();

	/* debug */
	bool isCollidable;

private:
	sf::Vector2i position;
	sf::Sprite mSprite;
};

