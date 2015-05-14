#pragma once

#include <vector>
#include <iostream>
#include <fstream>
#include <random>
#include <SFML/Graphics.hpp>
#include "Tile.h"
#include "TextureHolder.h"
#include "Monster.h"
#include "Portal.h"

class LevelMaker
{
public:
	LevelMaker();
	~LevelMaker(void);

	sf::Vector2i getPlayerPosition();
	sf::Sprite & getPortalSprite();
	Portal & getPortal();

	void LevelMaker::makeLevel( std::mt19937 & mt, TextureHolder & textures );
	void printLevel();
	void initializeLevel();
	void makeRooms(std::mt19937 & mt);
	bool makeMaze(std::mt19937 & mt);
	void placePortal(std::mt19937 & mt);
	void placePlayer(std::mt19937 & mt);
	void placeKey(std::mt19937 & mt);

	void clearTileMap();
	void generateTileMap( TextureHolder& textures );
	std::vector< std::vector<Tile> > & getTileMap();
	Tile & LevelMaker::tileAt( int x, int y );
	void display( sf::RenderWindow& window );
	void displayBackground( sf::RenderWindow& window );
	void placeCreatures( std::mt19937 & mt, TextureHolder & textures );

	sf::Sprite mPortalSprite;


private:

	std::vector< std::vector<char> > mLevelMap;
	std::vector< std::vector<Tile> > mTileMap;
	Portal mPortal;

	sf::Sprite mBackgroundSprite;
	int xDim;
	int yDim;
	sf::Vector2i portalDim;

	sf::Vector2i playerPosition;
};

