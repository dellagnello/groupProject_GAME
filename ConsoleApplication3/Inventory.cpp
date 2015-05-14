#include "Inventory.h"
#include <iostream>

Inventory::Inventory(void) :
	previousPos(100,100),
	mItems(),
	emptyItem()
{
	emptyItem.mItemName = "Empty";
	setNumSlots( 16 );
	initializeSlots();
	moveItem = false;
	useItem = false;


	/* Debug */

	/*       */

}


Inventory::~Inventory(void)
{
}


void Inventory::setNumSlots( int n )
{
	mNumSlots = n;
	for( int i = 0; i < n; i++ )
	{
		mItems.insert( std::pair<int,Item> (i, emptyItem) );
	}
}

void Inventory::initializeSlots()
{
	int x = 0;
	int y = 0;
	int posx = 0;
	int posy = 0;
	for( int i = 0; i < mNumSlots; i++ )
	{
		sf::RectangleShape slot;
		mSlots.push_back( slot );
		mSlots[i].setFillColor( sf::Color(255,255,255,15) );
		mSlots[i].setSize( sf::Vector2f( 32,32 ) );
		posx = 1755 + 36 * x;			
		posy = 911 + 38 * y;
		x++;
		if( (i > 0) && ((i+1) % 4 == 0) )
		{
			y++;
			x = 0;
		}
		mSlots[i].setPosition( (float) posx, (float) posy );
	}
}

void Inventory::drawSlots( sf::RenderWindow & window )
{
	for( int i = 0; i < mSlots.size(); i++ )
	{
		window.draw(mSlots[i]);
	}
}

void Inventory::drawItems( sf::RenderWindow & window )
{
	for( int i = 0; i < mItems.size(); i++ )
	{
		window.draw( mItems[i].mItemSprite );
	}
}

int Inventory::checkMouseCollision()
{
	int returnval = 0;
	Item & itemToMove = mItems[0];
	Item & itemToUse = mItems[0];
	sf::Vector2i mousePos = sf::Mouse::getPosition();
	for( int i = 0; i < mItems.size(); i++ )
	{
		sf::Vector2f itemPos = mItems[i].mItemSprite.getPosition();
		if( mousePos.x > itemPos.x && mousePos.x < itemPos.x + mItems[i].mItemSprite.getTextureRect().width &&
			mousePos.y > itemPos.y && mousePos.y < itemPos.y + mItems[i].mItemSprite.getTextureRect().height )
		{
			if( sf::Mouse::isButtonPressed( sf::Mouse::Left ) )
			{
				itemToMove = mItems[i];
				moveItem = true;
			}
			if( sf::Mouse::isButtonPressed( sf::Mouse::Right ) )
			{
				returnval = i;
				itemToUse = mItems[i];
				useItem = true;
			}
		}
	}
	
	if( moveItem )
	{
		itemToMove.mItemSprite.setPosition( mousePos.x - 10, mousePos.y - 10 );
		if( !sf::Mouse::isButtonPressed( sf::Mouse::Left ) )
		{
			moveItem = false;
		}
	}

	if( useItem )
	{

		if( !sf::Mouse::isButtonPressed( sf::Mouse::Right ) )
		{
			useItem = false;
		}
	}

	if( !moveItem )
	{
		bool doSet = false;
		sf::Vector2f temp(100,100);
		for( int j = 0; j < mSlots.size(); j++ )
		{
			if( itemToMove.mItemSprite.getPosition().x < mSlots[j].getPosition().x + mSlots[j].getSize().x &&
				itemToMove.mItemSprite.getPosition().x + 20 > mSlots[j].getPosition().x &&
				itemToMove.mItemSprite.getPosition().y < mSlots[j].getPosition().y + mSlots[j].getSize().y &&
				itemToMove.mItemSprite.getPosition().y + 20 > mSlots[j].getPosition().y &&
				moveItem == false )
			{
				temp = mSlots[j].getPosition();
				mSlots[j].setFillColor( sf::Color(35,35,140,30) );
				doSet = true;
				
			}
			else
			{
				mSlots[j].setFillColor( sf::Color(255,255,255,30) );
			}
		}
		if( doSet )
		{
			itemToMove.mItemSprite.setPosition( temp );
			previousPos = itemToMove.mItemSprite.getPosition();
		}
		else
		{
			itemToMove.mItemSprite.setPosition( previousPos );
		}
	}


	return returnval;
}

bool Inventory::add( Item x )
{
	for( int i = 0; i < mItems.size(); i++ )
	{
		if( mItems.at(i) == emptyItem )
		{
			mItems.at(i) = x;
			break;							
		}
		if( i == mItems.size() - 1 )
			return false;
	}
	return true;
}