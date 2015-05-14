#include "Item.h"


Item::Item(void)
{
	mInInventory = false;
	mHealthRestore = 0;
	mManaRestore = 0;
}


Item::~Item(void)
{
}

bool Item::operator==(const Item &other) const {
    if( other.mItemName == mItemName )
		return true;
	else
		return false;
  }


