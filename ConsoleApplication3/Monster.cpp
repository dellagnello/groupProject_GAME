#include "Monster.h"
#include <iostream>

Monster::Monster(void) :
	mAnimationSource()
{
	frameCounter2 = 0;
	mHealth = 100;
	mMaxHealth = 100;
	isAlive = true;
	mLevel = 2;
	mMonsterSprite.setTextureRect( sf::IntRect( 32,0,32,32 ) );
	mMonsterHealthBar.setScale( 0.35f, 0.5f );
	mIsInRange = false;
}


Monster::~Monster(void)
{
}

void Monster::updateBox()
{
	setBox( mMonsterSprite.getPosition().x + 5, 
			mMonsterSprite.getPosition().y + 5,
			50, 50 );
}

void Monster::setGoldIcon( sf::Texture & t )
{
	mGold.mGoldIcon.setTexture(t);
	mGold.setPosition( 0.f, 0.f );
}

void Monster::move( sf::Time deltaTime, std::mt19937 & mt )
{
	sf::Vector2f movement( 0.f, 0.f );
	sf::Vector2f previousPosition = mMonsterSprite.getPosition();

	if( mIsMovingUp )
	{
		movement.y -= 100.f;
		mAnimationSource.y = 3;
	}
	if( mIsMovingDown )
	{
		movement.y += 100.f;
		mAnimationSource.y = 0;
	}
	if( mIsMovingLeft )
	{
		movement.x -= 100.f;
		mAnimationSource.y = 1;
	}
	if( mIsMovingRight )
	{
		movement.x += 100.f;
		mAnimationSource.y = 2;
	}

	mMonsterSprite.move( movement * deltaTime.asSeconds() );
	mMonsterHealthBar.move( movement * deltaTime.asSeconds() );
	updateBox();
}

sf::Vector2f & Monster::getAnimationSource()
{
	return mAnimationSource;
}

void Monster::chooseRandomDirection( std::mt19937 & mt )
{
	mIsMovingUp = false;
	mIsMovingDown = false;
	mIsMovingLeft = false;
	mIsMovingRight = false;

	int rand = 1 + ( mt() % 8 );
	//std::cout << rand << std::endl;
	if( rand == 1 )
		mIsMovingUp = true;
	if( rand == 2 )
		mIsMovingDown = true;
	if( rand == 3 )
		mIsMovingLeft = true;
	if( rand == 4 )
		mIsMovingRight = true;
	
	if( rand == 5 )
	{
		mIsMovingDown = true;
		mIsMovingLeft = true;
	}
	if( rand == 6 )
	{
		mIsMovingDown = true;
		mIsMovingRight = true;
	}
	if( rand == 7 )
	{
		mIsMovingUp = true;
		mIsMovingLeft = true;
	}
	if( rand == 8 )
	{
		mIsMovingUp = true;
		mIsMovingRight = true;
	}
}

void Monster::animate(int & frameSpeed, int & switchFrame )
{
	if( mIsMovingDown || mIsMovingLeft || mIsMovingRight || mIsMovingUp )
	{
		frameCounter2 += frameSpeed * mClock.restart().asSeconds();
		if( frameCounter2 >= switchFrame )
		{
			frameCounter2 = 0;
			mAnimationSource.x++;
			if( mAnimationSource.x > 2 ) { mAnimationSource.x = 0; }
		}
		mMonsterSprite.setTextureRect( sf::IntRect( mAnimationSource.x * 32, mAnimationSource.y * 32, 32, 32 ) );
	}
	else
	{
		mMonsterSprite.setTextureRect( sf::IntRect( 32, mAnimationSource.y * 32, 32, 32 ) );
	}
}