#pragma once
#include "item.h"

/* 
   Inherited Methods:   

   Inherited Members: 
*/

class Weapon :
	public Item
{
public:
	Weapon(void);
	~Weapon(void);

	int attackBonus;
};

