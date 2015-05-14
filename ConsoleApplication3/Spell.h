#pragma once

#include <string>
#include <SFML/Graphics.hpp>
#include "BoxedObj.h"

class Spell : public BoxedObj
{
public:
	Spell(void);
	~Spell(void);

	void updateBox();

	std::string mSpellName;
	std::string mSpellType;

	sf::Sprite mSpellSprite;

	int mSpellTimer;
	int mSpellTimerMax;

	sf::Vector2f position;

	sf::Vector2f mMouseOffset;

	float mSpellSpeed;

};

