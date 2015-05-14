#include "Player.h"
#include <iostream>

Player::Player(void) :
	mInventory(),
	mInventoryBackground(),
	mMouseNormal(0.f,0.f),
	mRequestedMovePos(0.f,0.f),
	pAnimationSource(1,0)
{
	mIsMovingDown = false;
	mIsMovingUp = false;
	mIsMovingLeft = false;
	mIsMovingRight = false;
	mUsingSword = false;
	mChangeLevel = false;
	mUsingLamp = false;
	mToggleLamp = false;
	mUsingFlash = false;
	mUsingSpell = false;
	mUsingClickMove = false;

	mDistanceMoved = 0;
	mNormalizer = 0;
	mLevel = 1;
	mExp = 0;
	mGold = 0;
	mArmor = 0;
	mDamage = 5;
	mHealth = 20;
	mMaxHealth = 20;


	updateBox();
	mInventoryBackground.setPosition( 1750, 900 );
	



	//Weapon myWeapon;
	//myWeapon.attackBonus = 10;
	//mWeapon = myWeapon;
}

Player::~Player(void)
{
}

sf::Sprite & Player::getSprite()
{
	return mPlayerSprite;
}

void Player::updateBox()
{
	setBox( mPlayerSprite.getPosition().x - (mPlayerSprite.getTextureRect().width/2) + 5, 
			mPlayerSprite.getPosition().y - (mPlayerSprite.getTextureRect().height/2),
			24, 32 );
}

void Player::updateData()
{
	if( (mLevel * 100) <= mExp )
	{
		mLevel++;
	}

	//mDamage = mWeapon.attackBonus;
}

void Player::init( TextureHolder & textures )
{
	mPlayerSprite.setTexture( textures.get(Textures::Player) );
	mPlayerSprite.setTextureRect( sf::IntRect(32,0,32,32) );	
	mPlayerSprite.setOrigin( mPlayerSprite.getTextureRect().width/2,
							 mPlayerSprite.getTextureRect().height/2 );
}

void Player::animateMovement(int & atkTimer, int & frameCounter, int & frameSpeed, int & switchFrame, sf::Clock & clock2)
{
	if( mIsMovingDown || mIsMovingLeft || mIsMovingRight || mIsMovingUp || (mUsingSword && atkTimer < 20) ||
			mUsingClickMove )
	{
		
		frameCounter += frameSpeed * clock2.restart().asSeconds();
		if( frameCounter >= switchFrame )
		{
			frameCounter = 0;
			pAnimationSource.x++;
			if( pAnimationSource.x > 2 ) { pAnimationSource.x = 0; }
		}
	}
	else
	{
		pAnimationSource.x = 1;
	}

	mPlayerSprite.setTextureRect( sf::IntRect( pAnimationSource.x * 32, pAnimationSource.y * 32, 32, 32 ) );
}

void Player::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw( mPlayerSprite, states );
}