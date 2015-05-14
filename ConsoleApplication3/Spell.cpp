#include "Spell.h"


Spell::Spell(void)
{
	mSpellTimer = 0;
	mSpellTimerMax = 40;
	mSpellSpeed = 300.f;
	position.x = -100;
	position.y = -100;
	mMouseOffset.x = 0;
	mMouseOffset.y = 0;
}


Spell::~Spell(void)
{
}

void Spell::updateBox()
{
	setBox( mSpellSprite.getPosition().x, mSpellSprite.getPosition().y, mSpellSprite.getTextureRect().width, mSpellSprite.getTextureRect().height );
}
