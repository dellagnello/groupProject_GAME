#include "BoxedObj.h"



BoxedObj::BoxedObj(void) :
	boundingBox( 0, 0, 0, 0)
{
}

BoxedObj::~BoxedObj(void)
{
}

void BoxedObj::setBox( int left, int top, int width, int height )
{
	boundingBox.left = left;
	boundingBox.top = top;
	boundingBox.width = width;
	boundingBox.height = height;
}

sf::IntRect & BoxedObj::getBox()
{
	return boundingBox;
}

//void BoxedObj::draw(sf::RenderTarget& target, sf::RenderStates states) const
//{
	// Different Implementation for each one.
//}