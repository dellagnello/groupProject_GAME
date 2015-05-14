#include "UI.h"


UI::UI() :
	mPlayerHealth(),
	mPlayerArmor(),
	mPlayerDamage(),
	mPortrait(),
	mMenuBackground()
{
	isMenuRequested = false;
	mHealthBar.setScale( 2.f, 1.f );
}

UI::~UI(void)
{

}

void UI::initText( Player & player ) {
	myFont.loadFromFile( "times.ttf" );
	mPlayerHealth.setFont( myFont );
	mPlayerDamage.setFont( myFont );
	mPlayerArmor.setFont( myFont );
	mPlayerLevel.setFont( myFont );
	mPlayerExp.setFont( myFont );
	mPlayerGold.setFont( myFont );

	mPlayerArmor.setCharacterSize( 12 );
	mPlayerDamage.setCharacterSize( 12 );
	mPlayerHealth.setCharacterSize( 12 );
	mPlayerLevel.setCharacterSize( 20 );
	mPlayerExp.setCharacterSize( 12 );
	mPlayerGold.setCharacterSize( 20 );
	mPlayerGold.setStyle( sf::Text::Bold );
	mPlayerGold.setColor( sf::Color::Yellow );

	mBox.setPosition( 1630, 0 );
	mHealthBar.setPosition( 1716, 134 );
	mPlayerHealth.setPosition( 1650, 25 );	
	mPlayerArmor.setPosition( 1650, 45 );
	mPlayerDamage.setPosition( 1650, 65 );
	mPlayerLevel.setPosition( 1790, 35 );
	mPlayerExp.setPosition( 1795, 65 );
	mPlayerGold.setPosition( 1800, 800 );

	setArmorText( player.mArmor );
	setHealthText( player.mHealth, player.mMaxHealth );
	setDamageText( player.mDamage );
}

void UI::initMain( TextureHolder & textures) {

	mInventoryBackground.setTexture( textures.get( Textures::Inventory ) );
	mInventoryBackground.setPosition( 1750, 900 );


	setBoxTexture( textures.get(Textures::UI) );
	setHealthBarTexture( textures.get(Textures::HealthBar) );

}

void UI::setBoxTexture( sf::Texture & texture )
{
	mBox.setTexture( texture );
}

void UI::setHealthBarTexture( sf::Texture & texture )
{
	mHealthBar.setTexture( texture );
}

void UI::setHealthText( int health, int maxhealth )
{
	mPlayerHealth.setString("Health");
	std::stringstream hss;
	hss <<  "Health:  " << std::to_string( health ) << " / " << maxhealth;
	std::string setstr = hss.str();
	mPlayerHealth.setString( setstr );
}

void UI::setDamageText( int damage )
{
	std::stringstream hss;
	hss <<  "Damage:  " << std::to_string( damage );
	std::string setstr = hss.str();
	mPlayerDamage.setString( setstr );
}

void UI::setArmorText( int armor )
{
	std::stringstream hss;
	hss <<  "Armor:  " << std::to_string( armor );
	std::string setstr = hss.str();
	mPlayerArmor.setString( setstr );
}

void UI::setLevelText( int lvl )
{
	std::stringstream hss;
	hss << "Level " << std::to_string( lvl );
	std::string setstr = hss.str();
	mPlayerLevel.setString( setstr );
}

void UI::setExpText( int exp )
{
	std::stringstream hss;
	hss << "XP: " << std::to_string( exp );
	std::string setstr = hss.str();
	mPlayerExp.setString( setstr );
}

void UI::setGoldText( int gold )
{
	std::stringstream hss;
	hss << "Gold: " << std::to_string( gold );
	std::string setstr = hss.str();
	mPlayerGold.setString( setstr );
}

void UI::updateText( Player & myPlayer )
{
	setArmorText( myPlayer.mArmor );
	setDamageText( myPlayer.mDamage );
	setHealthText( myPlayer.mHealth, myPlayer.mMaxHealth );
	setLevelText( myPlayer.mLevel );
	setExpText( myPlayer.mExp );
	setGoldText( myPlayer.mGold );
}


void UI::displayMenu( sf::RenderWindow & window, Player & player ) {

	// Set Up Initial Menu
	bool menuExited = false;
	mMenuBackground.setPosition( player.mPlayerSprite.getPosition().x - 150, player.mPlayerSprite.getPosition().y - 250 );
	window.draw(mMenuBackground);
	sf::Text myText("  Press Escape to Exit\n\nPress Space to Resume", myFont, 20);
	myText.setPosition( player.mPlayerSprite.getPosition().x - 84, player.mPlayerSprite.getPosition().y - 32 );
	window.draw(myText);
	window.display();

	// Clear the Input Stream ( wait for them to release escape.. )
	while( sf::Keyboard::isKeyPressed( sf::Keyboard::Escape ) )
	{
		sf::sleep( sf::milliseconds( 100 ) );
	}

	// Menu Loop
	while( !menuExited )
	{	
		if( sf::Keyboard::isKeyPressed( sf::Keyboard::Space ) ) // space to resume
		{
			menuExited = true;
			isMenuRequested = false;
		}
		if( sf::Keyboard::isKeyPressed( sf::Keyboard::Escape ) ) // escape to exit
		{
			window.close();
			exit(0);
		}
	}

}

void UI::display( sf::RenderWindow & window )
{
	window.draw( mBox );
	window.draw( mPlayerHealth );
	window.draw( mPlayerArmor );
	window.draw( mPlayerDamage );
	window.draw( mHealthBar );
	window.draw( mPlayerLevel );
	window.draw( mPlayerExp );
	window.draw( mPlayerGold );
	window.draw( mInventoryBackground );
}
