#include <SFML/Graphics.hpp>
#include "TextureHolder.h"
#include "LevelMaker.h"
#include "Player.h"
#include "Monster.h"
#include "UI.h"
#include "Inventory.h"
#include "Gold.h"
#include "MouseService.h"

#pragma once

class Game
{
public:
	Game(void);
	~Game(void);

	void run();

private:
	/*-------------------------
	 Initializers and Loaders  */
	void loadTextures();
	void initializeUI();
	void initializeWindowAttributes();
	/*-------------------------*/



	void restartGame();

	void processEvents();
	void update( sf::Time deltaTime );
	void render();
	void handlePlayerInput( sf::Keyboard::Key key, bool isPressed );
	void updateMousePosition();
	void movePlayer( sf::Time deltaTime, std::vector< std::vector<Tile> > & myTileMap );
	void moveMonsters( sf::Time deltaTime, std::vector< std::vector<Tile> > & myTileMap );
	void monsterAttack( Monster & monster, int monsterTimer );
	void playerAttack( Monster & monster );
	void animateEnvironment();
	void toggleLamp();
	void updateSwordStatus();
	void monsterDeath( Monster & myMonster );
	void animatePortal();

	void displayGold();
	bool goldCollision( Gold & gold );

	// abilities
	void playerFlash( std::vector< std::vector<Tile> > & myTileMap );
	void playerFireball( std::vector< std::vector<Tile> > & myTileMap, sf::Time deltaTime, std::mt19937 & mt );
	void clickMove( sf::Time deltaTime, std::vector< std::vector<Tile> > & myTileMap );

	void checkMonstersInRange();  // only move monsters if they are within a certain range of the player

	// Collision   ****** NEED TO REFACTOR TO COLLISION SERVICE *********
	bool collision( BoxedObj x, BoxedObj y );
	bool attackCollision( BoxedObj monster ); // returns true if the player's sword collides with this monster.
	bool environmentCollision( std::vector< std::vector<Tile> > & myTileMap, BoxedObj & myMover );
	bool mapBoundsCollision( Monster & monster );
	//
	void displayMenu();
	void checkForDeath();
	void placeCreatures();
	bool attemptCreaturePlacement( sf::Vector2f & pos );

private:

	sf::RenderWindow mWindow;
	sf::View mView;
	sf::Font mFontTimes;
	sf::Vector2i mMousePosition;

	sf::Texture mTexture;
	sf::Sprite mFog;
	sf::Clock clock;
	sf::Clock clock2;
	sf::Clock monstAnimationClock;
	std::mt19937 mt;

	TextureHolder textures;
	LevelMaker mLevelMaker;
	Player mPlayerObj;
	Inventory mInventoryObj;
	sf::Sprite mPlayerSword;
	UI mUI;
	std::vector<Monster> mMonsters;

	int portalAniTimer;

	sf::RectangleShape mMenu;
	bool mMenuRequested;



	static const sf::Time TIMEPERFRAME;
	sf::RectangleShape alphaChannel;
	sf::Texture myTexture;  // for potion debug
};

