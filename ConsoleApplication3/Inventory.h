#pragma once

#include <vector>
#include <SFML/Graphics.hpp>
//#include "Item.h"
#include "Potion.h"
#include "Player.h"
#include <sstream>

class Inventory
{
public:
	Inventory(void);
	~Inventory(void);

	void setNumSlots( int n );
	void initializeSlots();
	void drawSlots( sf::RenderWindow & window );
	void drawItems( sf::RenderWindow & window );
	int checkMouseCollision();
	bool add( Item x );

	int mNumSlots;
	std::vector<sf::RectangleShape> mSlots;

	std::map<int, Item> mItems;
	//std::vector<Item> mItems; // can use pointers

	sf::Vector2f previousPos;
	bool moveItem;
	bool useItem;
	/* DEBUG */
	sf::Texture mTexture;
	Item emptyItem;

	/*       */

};

