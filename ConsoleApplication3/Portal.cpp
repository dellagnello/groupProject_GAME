#include "Portal.h"


Portal::Portal(void)
{
}


Portal::~Portal(void)
{
}

void Portal::updateBox()
{
	setBox( mSprite.getPosition().x + 5, 
			mSprite.getPosition().y + 5,
			58, 58 );
}

void Portal::setPosition( int x, int y )
{
	position.x = x;
	position.y = y;
}

void Portal::setSprite( sf::Texture& myTexture, sf::IntRect rect )
{
	mSprite.setTexture( myTexture );
	mSprite.setTextureRect( rect );
	mSprite.setPosition( (float) position.x, (float) position.y );
	//mSprite.setScale( 2.f, 2.f );
}

sf::Sprite & Portal::getSprite()
{
	return mSprite;
}