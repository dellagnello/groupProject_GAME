#include "Gold.h"


Gold::Gold(void)
{
	mAmount = 0;
	sf::Font times;
	times.loadFromFile("times.ttf");
	mAmountIndicator.setFont(times);
	mAmountIndicator.setPosition(0,0);
	mAmountIndicator.setString( " " + mAmount );
	shouldDisplay = false;
}


Gold::~Gold(void)
{
}

void Gold::setPosition( float x, float y )
{
	mGoldIcon.setPosition( x, y );
}

void Gold::display( sf::RenderWindow & window )
{
	window.draw( mGoldIcon );
}