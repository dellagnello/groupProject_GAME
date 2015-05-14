#pragma once
#include "boxedobj.h"
class Gold :
	public BoxedObj
{
public:
	Gold(void);
	~Gold(void);

	void setPosition( float x, float y );
	void display( sf::RenderWindow & window );

	bool shouldDisplay;
	int mAmount;
	sf::Sprite mGoldIcon;
	sf::Text mAmountIndicator;

};

