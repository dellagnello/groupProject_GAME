#pragma once
#include "BoxedObj.h"


class Tile : public BoxedObj
{
public:
	Tile(void);
	~Tile(void);
	void setPosition( int x, int y );
	void setSprite( sf::Texture& myTexture, sf::IntRect rect );
	sf::Sprite & getSprite();

	/* debug */
	bool isCollidable;

private:
	sf::Vector2i position;
	sf::Sprite mSprite;


};

