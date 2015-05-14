#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <sstream>
#include <iostream>
#include "Player.h"

class UI
{
public:
	UI();
	~UI(void);

	void UI::initText( Player & player );
	void UI::initMain( TextureHolder & textures );

	void setHealthText( int health, int maxhealth );
	void setDamageText( int damage );
	void setArmorText( int armor );
	void setLevelText( int lvl );
	void setExpText( int exp );
	void setGoldText( int gold );
	void setBoxTexture( sf::Texture & texture );
	void setHealthBarTexture( sf::Texture & texture );
	void updateText( Player & myPlayer );
	void setInventoryBackground( sf::Texture & texture );


	void displayMenu( sf::RenderWindow & window, Player & player );
	void display( sf::RenderWindow & window );

	sf::Sprite mBox;
	sf::Sprite mHealthBar;
	sf::Sprite mPortrait;
	sf::RectangleShape mMenuBackground;
	sf::Text mPlayerHealth;
	sf::Text mPlayerDamage;
	sf::Text mPlayerArmor;
	sf::Text mPlayerLevel;
	sf::Text mPlayerExp;
	sf::Text mPlayerGold;

	bool isMenuRequested;

private:
	sf::Sprite mInventoryBackground;
	sf::Font myFont;
	

};

