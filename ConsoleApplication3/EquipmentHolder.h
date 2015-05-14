#pragma once

#include "Potion.h"

/*
	To be used for items that are actually equipped.
	Slots available in Equipment are:
			-- Chest
			-- Legs
			-- Gloves
			-- Boots
			-- Helmet
			-- Weapon

	Have all of the EquipmentHolder updating functions here.
	Handling of player stat updates is done in Player
*/

class EquipmentHolder
{
public:
	EquipmentHolder(void);
	~EquipmentHolder(void);



	Item slot_Chest;
	Item slot_Legs;
	Item slot_Gloves;
	Item slot_Boots;
	Item slot_Helmet;
	Item slot_Weapon;
};

