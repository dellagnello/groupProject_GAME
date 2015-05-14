#include "Tile.h"
#include <iostream>


Tile::Tile(void)
{
	position.x = 0;
	position.y = 0;
	isCollidable = false;
}


Tile::~Tile(void)
{
}

void Tile::setPosition( int x, int y )
{
	position.x = x;
	position.y = y;
}

void Tile::setSprite( sf::Texture& myTexture, sf::IntRect rect )
{
	mSprite.setTexture( myTexture );
	mSprite.setTextureRect( rect );
	mSprite.setPosition( (float) position.x, (float) position.y );
	mSprite.setScale( 2.f, 2.f );
}

sf::Sprite & Tile::getSprite()
{
	return mSprite;
}