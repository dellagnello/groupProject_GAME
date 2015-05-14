#include "BoxedObj.h"
#include "Gold.h"
#include <stdlib.h>
#include <random>
#include <vector>

#pragma once
class Monster : public BoxedObj
{
public:

	Monster(void);
	~Monster(void);
	
	void updateBox();
	void setGoldIcon( sf::Texture & t );

	void move( sf::Time deltaTime, std::mt19937 & mt );
	void chooseRandomDirection( std::mt19937 & mt );
	void animate(int & frameSpeed, int & switchFrame );
	sf::Vector2f & getAnimationSource();

	sf::Clock mClock;
	int frameCounter2;
	
	int mHealth;
	int mLevel;
	int mMaxHealth;
	bool isAlive;
	
	sf::Sprite mMonsterSprite;
	bool mIsMovingUp;
	bool mIsMovingDown;
	bool mIsMovingLeft;
	bool mIsMovingRight;
	bool mIsInRange;

	sf::Vector2f mAnimationSource;
	sf::Sprite mMonsterHealthBar;

	Gold mGold;

	sf::Vector2f previousMonsterPosition;
	sf::Vector2f previousMonstHealthBarPos;


};

