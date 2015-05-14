#pragma once
//#include <SFML/Graphics.hpp>
#include "BoxedObj.h"
#include "Weapon.h"
#include "Potion.h"
#include "Spell.h"
#include "EquipmentHolder.h"
#include "TextureHolder.h"


class Player : public BoxedObj, public sf::Drawable
{
public:
	Player(void);
	~Player(void);

	sf::Sprite & getSprite();

	//  @OVERRIDE
	void Player::draw(sf::RenderTarget& target, sf::RenderStates states) const;

	void init(TextureHolder & textures);
	void updateBox();

	void updateData();
	void applyEquipment();
	void animateMovement(int & atkTimer, int & frameCounter, int & frameSpeed, int & switchFrame, sf::Clock & clock2);

	/* Public for ease of access from Game */
	sf::Vector2i pAnimationSource;
	bool mUsingClickMove;
	bool mIsMovingUp;
	bool mIsMovingDown;
	bool mIsMovingLeft;
	bool mIsMovingRight;
	bool mUsingSword;
	bool mChangeLevel;
	bool mUsingLamp;
	bool mToggleLamp;
	bool mUsingFlash;
	bool mUsingSpell;
	sf::Sprite mPlayerSprite;

	int mArmor;
	int mHealth;
	int mMaxHealth;
	int mDamage;
	int mLevel;
	int mExp;
	int mGold;

	std::vector<Item> mInventory;
	EquipmentHolder mEquipmentHolder;
	Weapon mWeapon;

	// In progress.
	Spell mSpell;

	// Click move
	sf::Vector2f mMouseNormal;
	sf::Vector2f mRequestedMovePos;
	float mDistanceMoved;
	float mNormalizer;



private:

	sf::Sprite mInventoryBackground;

	
};


