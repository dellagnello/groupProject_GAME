#pragma once
#include <SFML/Graphics.hpp>

class Item
{
public:
	Item(void);
	~Item(void);

	bool Item::operator==(const Item &other) const;

	sf::Sprite mItemSprite;
	std::string mItemName;


	int mHealthRestore;
	int mManaRestore;
	int mStackSize;

	bool mInInventory;
};

