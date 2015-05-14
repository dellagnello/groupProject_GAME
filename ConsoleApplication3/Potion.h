#pragma once
#include "item.h"
class Potion :
	public Item
{
public:
	Potion(void);
	~Potion(void);

	int mHealthRestore;
	int mManaRestore;
};

