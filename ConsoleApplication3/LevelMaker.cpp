#include "LevelMaker.h"

/* IMPLEMENT A QUAD TREE FOR TILE MAP 
	-Performance Gain is HUGE
		-> Only render visible sprites
		-> Only check local tiles for collision

*/

using namespace std;

LevelMaker::LevelMaker() :
	mBackgroundSprite(),
	mPortal()
{
	xDim = 60;
	yDim = 30;
	playerPosition.x = 0;
	playerPosition.y = 0;
	mBackgroundSprite.setPosition( 0.f, 0.f );
	mBackgroundSprite.setTextureRect( sf::IntRect( 0, 0, 64*xDim, 64*yDim ) );
}

LevelMaker::~LevelMaker(void)
{
}

sf::Vector2i LevelMaker::getPlayerPosition()
{
	return playerPosition;
}

void LevelMaker::makeLevel( std::mt19937 & mt, TextureHolder & textures )
{
	initializeLevel();
	mPortal.setSprite( textures.get( Textures::PortalForest ), sf::IntRect( 0, 0, 64, 64 ) );
	//makeRooms(mt);
	bool mazeMade = false;
	while( !mazeMade )
	{
		mazeMade = makeMaze(mt);
		if( !mazeMade )
		{
			initializeLevel();
		}
	}
	sf::Texture & myTexture = textures.get( Textures::BackgroundGrass );
	myTexture.setRepeated(true);
	mBackgroundSprite.setTexture( myTexture );

	placePortal(mt);
	placePlayer(mt);
	placeKey(mt);
	clearTileMap();
	generateTileMap( textures );
}

/* Function:  populate mLevelMap with '#', with dimensions xDim,yDim. */
void LevelMaker::initializeLevel()
{
	mLevelMap.clear();
	for( int i = 0; i < yDim; i++ )
	{
		vector<char> row;
		for( int j = 0; j < xDim; j++ )
		{
			row.push_back( '#' );
		}
		mLevelMap.push_back( row );
	}
}

/* Function:  print mLevelMap to console */
void LevelMaker::printLevel()
{
	for( int i = 0; i < mLevelMap.size(); i++ )
	{
		for( int j = 0; j < mLevelMap[i].size(); j++ )
		{
			cout << mLevelMap[i][j];
		}
		cout << endl;
	}
	ofstream outfile;
	outfile.open( "map90.txt");
	for( int i = 0; i < mLevelMap.size(); i++ )
	{
		for( int j = 0; j < mLevelMap[i].size(); j++ )
		{
			outfile << mLevelMap[i][j];
		}
		outfile << endl;
	}


}

/* Function:  make a randomized organic, open level */
void LevelMaker::makeRooms(mt19937 & mt)
{
	int numrooms = 200 + (mt() % 30);
	int randomX, randomY, randomWidth, randomHeight;
	cout << numrooms << endl;
	int roomsMade = 0;
	int iterations = 0;

	while( roomsMade < numrooms )
	{
		randomX = 1 + (mt() % (xDim - 1));
		randomY = 1 + (mt() % (yDim - 1));
		randomWidth = 2 + (mt() % 3);
		randomHeight = 2 + (mt() % 3);

		bool canMake = true;
		// i === column
		// j === row
		for( int i = randomY - 1; i < randomY + randomHeight + 1; i++ )
		{
			for( int j = randomX - 1; j < randomX + randomWidth + 1; j++ )
			{
				if( i < yDim && j < xDim )
				{
					if( mLevelMap[i][j] == '.' )
					{
						canMake = false;
					}
				} else {
					canMake = false;
				}
			}
		}

		if( canMake )
		{
			for( int i = randomY; i < randomY + randomHeight; i++ )
			{
				for( int j = randomX; j < randomX + randomWidth; j++ )
				{
					mLevelMap[i][j] = ' ';
				}
			}
			roomsMade++;
		}
		iterations++;
		if( iterations > 900 )
		{
			break;
		}
	} // end while
}

/* Function:  make a randomized maze puzzle level */
bool LevelMaker::makeMaze( mt19937 & mt )
{
	vector<sf::Vector2i> currentPossible;
	int numTiles = 800;
	int randomX, randomY, randomWidth, randomHeight;
	int tilesMade = 0;
	int randIndex = 0;
	int neighbors = 0;
	bool canMake = true;

	// MAKE FIRST ROOM TILE
	randomX = 5 + (mt() % (xDim / 2));
	randomY = 5 + (mt() % (yDim / 2));
    mLevelMap[randomY][randomX] = ' ';
	if( randomY + 1 < yDim - 1 )
	{
		if( mLevelMap[randomY+1][randomX] == '#' )
			currentPossible.push_back( sf::Vector2i(randomX,randomY + 1) );
	}
	if( randomY - 1 > 1 )
	{
		if( mLevelMap[randomY-1][randomX] == '#' )
			currentPossible.push_back( sf::Vector2i(randomX,randomY - 1) );
	}
	if( randomX + 1 < xDim - 1 )
	{
		if( mLevelMap[randomY][randomX + 1] == '#' )
			currentPossible.push_back( sf::Vector2i(randomX + 1,randomY) );
	}
	if( randomX - 1 > 1 )
	{
		if( mLevelMap[randomY][randomX - 1] == '#' )
			currentPossible.push_back( sf::Vector2i(randomX - 1,randomY) );
	}

	// BUILD MAP FROM FIRST ROOM
	while( tilesMade < numTiles )
	{
		canMake = true;
		neighbors = 0;
		if( currentPossible.size() == 0 )
		{
			//cout << "this is the problem" << endl;
			return false;
		}
		else
		{		
			randIndex = mt() % ( currentPossible.size() );
			sf::Vector2i toAdd = currentPossible[randIndex];
			if( mLevelMap[toAdd.y][toAdd.x] != ' ' )
			{
				currentPossible.erase(currentPossible.begin()+randIndex);
				//currentPossible.clear();
				if( toAdd.y + 1 < yDim - 1 )
				{
					if( mLevelMap[toAdd.y+1][toAdd.x] == ' ' )
						neighbors++;
				}
				if( toAdd.y - 1 > 1 )
				{
					if( mLevelMap[toAdd.y-1][toAdd.x] == ' ' )
						neighbors++;
				}
				if( toAdd.x + 1 < xDim - 1 )
				{
					if( mLevelMap[toAdd.y][toAdd.x+1] == ' ' )
						neighbors++;
				}
				if( toAdd.x - 1 > 1 )
				{
					if( mLevelMap[toAdd.y][toAdd.x-1] == ' ' )
						neighbors++;
				}

				if( neighbors >= 2 )
				{
					canMake = false;
				}
				int randomizer = mt() % 5;
				if( randomizer == 1 )
				{
					canMake = false;
				}

				if( canMake == true )
				{
					mLevelMap[toAdd.y][toAdd.x] = ' ';
					tilesMade++;
					if( toAdd.y + 1 < yDim - 1 )
					{
						if( mLevelMap[toAdd.y+1][toAdd.x] == '#' )
							currentPossible.push_back( sf::Vector2i(toAdd.x,toAdd.y + 1) );
					}
					if( toAdd.y - 1 > 1 )
					{
						if( mLevelMap[toAdd.y-1][toAdd.x] == '#' )
							currentPossible.push_back( sf::Vector2i(toAdd.x,toAdd.y - 1) );
					}
					if( toAdd.x + 1 < xDim - 1 )
					{
						if( mLevelMap[toAdd.y][toAdd.x + 1] == '#' )
							currentPossible.push_back( sf::Vector2i(toAdd.x + 1,toAdd.y) );
					}
					if( toAdd.x - 1 > 1 )
					{
						if( mLevelMap[toAdd.y][toAdd.x - 1] == '#' )
							currentPossible.push_back( sf::Vector2i(toAdd.x - 1,toAdd.y) );
					}
				}
			}
		}
	} // end while
	
	int numcleared = 0;

	// clear out some of the tiles to make it easier
	while( numcleared < 100 )
	{
		bool canRemove = false;
		int myX = 1 + mt() % (xDim - 2);
		int myY = 1 + mt() % (yDim - 2);
		if( mLevelMap[myY][myX] == '#' )
		{
			if( myX + 1 < xDim - 1 )
			{
				if( mLevelMap[myY][myX+1] == ' ' )
				{
					canRemove = true;
				}
			}
			if( myX - 1 > 1 )
			{
				if( mLevelMap[myY][myX-1] == ' ' )
				{
					canRemove = true;
				}
			}
			if( myY + 1 < yDim - 1 )
			{
				if( mLevelMap[myY+1][myX] == ' ' )
				{
					canRemove = true;
				}
			}
			if( myY - 1 > 1 )
			{
				if( mLevelMap[myY-1][myX] == ' ' )
				{
					canRemove = true;
				}
			}

		}
		if( canRemove )
		{
			numcleared++;
			mLevelMap[myY][myX] = ' ';
		}
	}
	
	return true;
}

/* Function:  place the portal to the next level ('>' in mLevelMap) */
void LevelMaker::placePortal(mt19937 & mt)
{
	int randx, randy;
	bool portalPlaced = false;
	while( !portalPlaced )
	{
		randx = 1 + mt() % (xDim - 2);
		randy = 1 + mt() % (yDim - 2);
		if( mLevelMap[randy][randx] == ' ' )
		{
			mLevelMap[randy][randx] = '>';
			portalPlaced = true;
			portalDim.x = randx;
			portalDim.y = randy;
		}
	}
}

/* Function:  place the player on the map ('P' in mLevelMap) */
void LevelMaker::placePlayer(mt19937 & mt)
{
	bool playerPlaced = false;
	bool couldntPlace = false;
	int randx, randy;
	int iterations = 0;
	// if the portal is on the right half of the map...
	if( portalDim.x > (xDim / 2) )
	{
		// then place the player on the left half of the map.
		while( !playerPlaced )
		{
			iterations++;
			randx = mt() % (xDim / 2);
			randy = mt() % yDim;
			if( mLevelMap[randy][randx] == ' ' )
			{
				if( randx < xDim - 1 && randx > 1 && randy < yDim - 1 && randy > 1 )
				{
					if( mLevelMap[randy][randx+1] != '#' &&
						mLevelMap[randy][randx-1] != '#' &&
						mLevelMap[randy+1][randx] != '#' &&
						mLevelMap[randy-1][randx] != '#' )
					{
						mLevelMap[randy][randx] = 'P';
						playerPlaced = true;
					}
				}
			}
		}
	}  // else place the player on the right half of the map.
	else {
		
		// then place the player on the left half of the map.
		while( !playerPlaced )
		{
			iterations++;
			randx = (xDim/2) + mt() % (xDim / 2);
			randy = mt() % yDim;
			if( mLevelMap[randy][randx] == ' ' )
			{
				if( randx < xDim - 1 && randx > 1 && randy < yDim - 1 && randy > 1 )
				{
					if( mLevelMap[randy][randx+1] != '#' &&
						mLevelMap[randy][randx-1] != '#' &&
						mLevelMap[randy+1][randx] != '#' &&
						mLevelMap[randy-1][randx] != '#' )
					{
						mLevelMap[randy][randx] = 'P';
						playerPlaced = true;
					}
				}
			}
		}
	}

}

/* Function: place the key on the map ('K' in mLevelMap) */
void LevelMaker::placeKey(mt19937 & mt)
{
	int randx, randy;
	bool keyPlaced = false;
	while( !keyPlaced )
	{
		randx = mt() % xDim;
		randy = mt() % yDim;
		if( mLevelMap[randy][randx] == ' ' )
		{
			mLevelMap[randy][randx] = 'K';
			keyPlaced = true;
		}
	}
}

/* Function: erase the tile map (before you make a new one) */
void LevelMaker::clearTileMap()
{
	mTileMap.clear();
}

/* Function: make a map of tiles out of mLevelMap */
void LevelMaker::generateTileMap( TextureHolder& textures )
{
	//printLevel();
	int x = 0;
	int y = 0;
	for( int i = 0; i < mLevelMap.size(); i++ )
	{
		vector<Tile> row;
		for( int j = 0; j < mLevelMap[i].size(); j++ )
		{
			Tile myTile;
			myTile.setBox( x + 2, y + 20, 60, 40 );
			myTile.setPosition(x,y);
			if( mLevelMap[i][j] == 'P' )
			{
				playerPosition.x = x;
				playerPosition.y = y;
			}
			x += 64;
			if( mLevelMap[i][j] == '#' )
			{
				/* WHYYY DOES 160 cause overflow in picture? */
				myTile.setSprite( textures.get( Textures::Landscape ), sf::IntRect(0, 32, 32, 32 ) );
				myTile.isCollidable = true;
			}
			if( mLevelMap[i][j] == ' ' )
			{
				/* Do Not Display */
				//myTile.setSprite( textures.get( Textures::Landscape ), sf::IntRect( 128, 32, 32, 32 ) );
			}
			if( mLevelMap[i][j] == '>' )
			{
				mPortal.getSprite().setPosition(x-64,y); // center the portal in the 64x64 block
				mPortal.updateBox();
			}
			row.push_back( myTile );
		}
		y += 64;
		x = 0;
		mTileMap.push_back( row );
	}
}


std::vector< std::vector<Tile> > & LevelMaker::getTileMap()
{
	return mTileMap;
}

Tile & LevelMaker::tileAt( int x, int y )
{
	return mTileMap[x][y];
}


/* Function: draw the tile map to the window */
void LevelMaker::display( sf::RenderWindow& window )
{
	for( int i = 0; i < mTileMap.size(); i++ ) {
		for( int j = 0; j < mTileMap[i].size(); j++ )
		{
			//window.draw(mTileMap[i][j].getSprite());
			//sf::IntRect myRect((int)mTileMap[i][j].getBox().left, (int)mTileMap[i][j].getBox().top, (int)mTileMap[i][j].getBox().width, (int)mTileMap[i][j].getBox().height);
			if( mTileMap[i][j].isCollidable ){
				sf::RectangleShape hitbox(sf::Vector2f(mTileMap[i][j].getBox().width,mTileMap[i][j].getBox().height));
				hitbox.setPosition(mTileMap[i][j].getBox().left,mTileMap[i][j].getBox().top);
				hitbox.setFillColor(sf::Color(100,30,55));
				window.draw(hitbox);
			}

			window.draw(mTileMap[i][j].getSprite());
			
		}
	}

}

void LevelMaker::displayBackground( sf::RenderWindow& window )
{
	window.draw( mBackgroundSprite );
	window.draw( mPortal.getSprite() );
}

sf::Sprite & LevelMaker::getPortalSprite()
{
	return mPortalSprite;
}

Portal & LevelMaker::getPortal()
{
	return mPortal;
}

