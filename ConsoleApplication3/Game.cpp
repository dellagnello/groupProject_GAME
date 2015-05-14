#include "Game.h"

/*


	NOTES:   

		--  mMousePosition is deprecated.  Use the MouseService::getMousePosition() and MouseService::getMouseUnitVector() functions instead.
			Try to phase out mMousePosition wherever found.

		--  The global variables should be refactored.

		--  The functions in "Other Functions" Section should probably be refactored into other classes.










*/






//-------------GLOBALS-------------//
const float SCREENX = 1920;
const float SCREENY = 1080;
const sf::Time Game::TIMEPERFRAME = sf::seconds(1.f/70.f);
sf::Vector2i swordAniSrc(1,0);
int frameCounter = 0;
int frameSpeed = 600; int switchFrame = 100;

int atkTimer = 0;  // DEBUG MELEE ATTACK GLOBAL

int countComparisons = 0;
int envAnimTimer = 0;
int maxAniTime = 6;
int monsterTimer = 0;
int maxTime = 30;
bool attackOccurred = false;

bool global_canUsePotion = false;


int alphaLevel = 5;
int alphaCount = 0;
//---------------------------------//

//----------GAME CONSTRUCTOR-------------//
//-----------AND DESTRUCTOR--------------//
Game::Game(void) : mWindow( sf::VideoMode(SCREENX,SCREENY), "Logan's Game", sf::Style::Fullscreen),
	mPlayerObj(),
	mMenu(),
	mLevelMaker(),
	mView(sf::FloatRect(0,0,SCREENX,SCREENY)),
	mUI(),
	mMonsters(),
	mMousePosition(),
	mPlayerSword()
{
	
	mt.seed( time(NULL) );		// seed rng


	// LOADERS AND INITIALIZERS
	initializeWindowAttributes();			// set vertical sync, framerate, and zoom of mWindow
	loadTextures();							// populate the TextureHolder
	initializeUI();							// set up the 'mUI' 
	mPlayerObj.init(textures);
	portalAniTimer = 0;

	
	// MAKE THE FIRST LEVEL
	mLevelMaker.makeLevel(mt,textures);
	placeCreatures();

	// 
	mPlayerObj.mPlayerSprite.setOrigin( mPlayerObj.mPlayerSprite.getTextureRect().width/2,
										mPlayerObj.mPlayerSprite.getTextureRect().height/2 );

	// PLACE THE PLAYER
	/* Debug Player Positioning 
	*
	*   -- places the player on the portal
	*
	*/
	mPlayerObj.mPlayerSprite.setPosition( mLevelMaker.getPortal().getSprite().getPosition().x + (mPlayerObj.mPlayerSprite.getTextureRect().width/2),
										  mLevelMaker.getPortal().getSprite().getPosition().y + (mPlayerObj.mPlayerSprite.getTextureRect().height/2) );
	/* Real Player Positioning
	*
	*      -- places the player on the 'P' in the map 
	*
	*/
	//mPlayerObj.mPlayerSprite.setPosition( (float) mLevelMaker.getPlayerPosition().x, (float) mLevelMaker.getPlayerPosition().y );  

	// CENTER VIEW
	mView.setCenter( mPlayerObj.mPlayerSprite.getPosition() );



	/*
	*                 #
	*  MOVE THIS!!    |
    *                 V
	*/

	//DEBUG POTION  

	
	Potion myPotion;
	myTexture.loadFromFile("Images/potion.png");
	myPotion.mItemSprite.setTexture( myTexture ); 
	myPotion.mHealthRestore = 5;
	myPotion.mItemName = "Health Potion";
	mInventoryObj.add(myPotion);
	mInventoryObj.mItems[0].mItemSprite.setPosition( mInventoryObj.mSlots[0].getPosition() );
	mInventoryObj.mItems[0].mHealthRestore = 3;

	// DEBUG SPELL Initialization (Fireball)
	mPlayerObj.mSpell.mSpellSprite.setTexture( textures.get( Textures::Fireball ) );
	mPlayerObj.mSpell.mSpellName = "Fireball";
	mPlayerObj.mSpell.mSpellType = "Offense";
	mPlayerObj.mSpell.mSpellSprite.setOrigin( mPlayerObj.mSpell.mSpellSprite.getPosition().x + ( mPlayerObj.mSpell.mSpellSprite.getTextureRect().width / 2 ), 
													mPlayerObj.mSpell.mSpellSprite.getPosition().y + ( mPlayerObj.mSpell.mSpellSprite.getTextureRect().height / 2 ) );
	// END SPELL DEBUG Init.

	/*
	*
	*
	*
	*
	*/

	
}
Game::~Game(void)
{
}
//---------------------------------------//

//---------INIT-------------//
void Game::loadTextures()
{
	textures.load(Textures::BackgroundGrass, "Images/grass.png");
	textures.load(Textures::Landscape, "Images/environment11.png");
	textures.load(Textures::Player, "Images/player.png" );
	textures.load(Textures::Fog, "Images/fog3.png");
	textures.load(Textures::Lamp, "Images/lamp2.png");
	textures.load(Textures::Monster, "Images/Skeleton.png");
	textures.load(Textures::UI, "Images/GUI.png");
	textures.load(Textures::HealthBar, "Images/healthbar.png");
	textures.load(Textures::Inventory, "Images/inventory.png");
	textures.load(Textures::Sword, "Images/sword.png");
	textures.load(Textures::Gold, "Images/gold.png");
	textures.load(Textures::Flash, "Images/flash.png");
	textures.load(Textures::PortalForest, "Images/portal4.png");
	textures.load(Textures::Fireball, "Images/fireball.png");
}
void Game::initializeUI()
{
	mUI.initText( mPlayerObj );
	mUI.initMain( textures );
	
	mFog.setTexture( textures.get(Textures::Fog) );
	mFog.setPosition( -150.f, 0.f );
	mFog.setScale( 1.3f,1.f );

	mUI.isMenuRequested = false;
	mUI.mMenuBackground.setSize( sf::Vector2f( 300.f, 500.f ) );
	mUI.mMenuBackground.setFillColor( sf::Color::Black );

}
void Game::initializeWindowAttributes()
{
	mWindow.setVerticalSyncEnabled(true);
	mWindow.setFramerateLimit( 60 );
	mView.zoom( 0.5f );
}
//--------------------------//


//--------MAIN SFML FUNCTIONS--------//

void Game::run()
{
	sf::Time timeSinceLastUpdate = sf::Time::Zero;
	while( mWindow.isOpen() )
	{
		timeSinceLastUpdate += clock.restart();
		while( timeSinceLastUpdate > TIMEPERFRAME )
		{
			
			timeSinceLastUpdate -= TIMEPERFRAME;
			processEvents();
			update(TIMEPERFRAME);
			render();
		}
		// deprecated updateMousePosition()... remove this.
		updateMousePosition();
		
		
	}
}

void Game::processEvents()
{
	sf::Event event;
	while( mWindow.pollEvent( event ) )
	{
		switch( event.type )
		{
		case sf::Event::KeyPressed:
			handlePlayerInput( event.key.code, true );
			break;
		case sf::Event::KeyReleased:
			handlePlayerInput( event.key.code, false );
			break;
		case sf::Event::Closed:
			mWindow.close();
			break;
		}
	}
}

																////////////////////////////////////////
																//     DEBUG COOLDOWN FOR FIREBALL    //
																int cooldown = 100;					  //  ( used in update and handlePlayerInput )
																int cooldownBurn = 0;				  //		REFACTOR TO SPELL CLASS (already has an animation cooldown "mSpellTimer")
																////////////////////////////////////////

void Game::update( sf::Time deltaTime )
{
													////////////////////////////////////////
													//     DEBUG COOLDOWN FOR FIREBALL    //
													cooldownBurn += 1;					  //
													if( cooldownBurn > cooldown ) {		  //
														cooldownBurn = cooldown + 1;	  //
													}									  //
													////////////////////////////////////////



	sf::Vector2f playerPos = ( mPlayerObj.mPlayerSprite.getPosition() );
	mView.setCenter( playerPos.x, playerPos.y );

	// CHECK FOR PLAYER DEATH
	checkForDeath();

	// MENU (Esc) HANDLER
	if( mUI.isMenuRequested ) {
		mUI.displayMenu( mWindow, mPlayerObj );
		clock.restart();
	}

	// USER INTERFACE
	mPlayerObj.updateData();  // set stats
	mUI.updateText( mPlayerObj );  // update stats in UI

	// ANIMATE PORTAL
	animatePortal();

	// LAMP TOGGLE
	toggleLamp();  // ** ??? Move to Player object ??? **
	
	// ANIMATE PLAYER MOVEMENT
	mPlayerObj.animateMovement( atkTimer, frameCounter, frameSpeed, switchFrame, clock2 );

	// GET THE TILE MAP
	std::vector< std::vector<Tile> > myTileMap = mLevelMaker.getTileMap();  // NEED TO USE QUAD TREE TO AVOID CHECKING EVERY TILE (maybe)

	// CHECK FOR FLASH USE
	playerFlash( myTileMap );  // ** ??? Put into MovementHandler class ??? **

	// CHECK FOR FIREBALL USE
	playerFireball( myTileMap, deltaTime, mt );

	// Click move
	clickMove( deltaTime, myTileMap );    // ** ??? Put into MovementHandler class ??? **


	// MOVE PLAYER
	movePlayer( deltaTime, myTileMap );   // ** ??? Put into MovementHandler class ??? **

	// CHECK MONSTERS IN RANGE
	checkMonstersInRange();

	// GOLD PICK UP
	for( int i = 0; i < mMonsters.size(); i++ )
	{
		if( mMonsters[i].mGold.shouldDisplay )
			goldCollision( mMonsters[i].mGold );
	}

	// UPDATE THE PLAYER'S ATTACK BOX
	updateSwordStatus();

	// MOVE MONSTERS
	moveMonsters( deltaTime, myTileMap );

	// PLAYER INTERACT WITH PORTAL
	if( collision( mPlayerObj, mLevelMaker.getPortal() ) )
	{
		// PORTAL IMPLEMENTATION (Change Level)
		if( mPlayerObj.mChangeLevel )
		{
			mMonsters.clear();
			mLevelMaker.makeLevel( mt, textures );
			placeCreatures();
			mPlayerObj.mPlayerSprite.setPosition( (float) mLevelMaker.getPlayerPosition().x + (mPlayerObj.mPlayerSprite.getTextureRect().width/2),
												  (float) mLevelMaker.getPlayerPosition().y + (mPlayerObj.mPlayerSprite.getTextureRect().height/2) );
			mPlayerObj.mChangeLevel = false;
		}
	}
	else
	{
		mPlayerObj.mChangeLevel = false;
	}

	// ANIMATE ENVIRONMENT (
	animateEnvironment();

	// UPDATE PLAYER HEALTHBAR
	float currentPercentHealth = 0.5f * (((float) mPlayerObj.mHealth) / ((float) mPlayerObj.mMaxHealth));
	mUI.mHealthBar.setTextureRect( sf::IntRect( 0, 0, (184 * currentPercentHealth), 10 ) );  // 184 pixels is the width of the image.

	// Kluge Potion Use
	int itemIndex = mInventoryObj.checkMouseCollision();
	if( mInventoryObj.useItem )
	{
		if( global_canUsePotion )
		{

			mPlayerObj.mHealth += mInventoryObj.mItems[itemIndex].mHealthRestore;
			global_canUsePotion = false;
		}
		if( mPlayerObj.mHealth > mPlayerObj.mMaxHealth )
			mPlayerObj.mHealth = mPlayerObj.mMaxHealth;
	}
	else
	{
		global_canUsePotion = true;
	}


}

void Game::render()
{
	mWindow.clear();



	// DRAW THE GAME SCREEN
	mLevelMaker.displayBackground(mWindow); //grass image


	/* DEBUG PLAYER HITBOX DRAWING */
	sf::RectangleShape hitbox(sf::Vector2f(mPlayerObj.getBox().width,mPlayerObj.getBox().height));
	//sf::RectangleShape hitbox(sf::Vector2f(100.f,100.f));
	hitbox.setPosition(mPlayerObj.getBox().left,mPlayerObj.getBox().top);
	hitbox.setFillColor(sf::Color(100,100,55));
	mWindow.draw(hitbox);
	/* END DEBUG PLAYER HITBOX DRAWING */


	//
	// DRAW PLAYER
	//		if/else determines whether to draw the sword on top of the player or opposite
	if( mPlayerObj.pAnimationSource.y == 0 || mPlayerObj.pAnimationSource.y == 2 )
	{
		mWindow.draw(mPlayerObj.mPlayerSprite); // player
		mWindow.draw(mPlayerSword);
	}
	else
	{
		
		mWindow.draw(mPlayerSword);
		mWindow.draw(mPlayerObj.mPlayerSprite); // player
	}

	//
	// DEBUG CENTER DOT DRAWING  ( dot at player's center, for comparing to flash positioning and drawing of sprite )
	//
	sf::CircleShape centerDot(1.f,10);
	centerDot.setPosition(mView.getCenter().x, mView.getCenter().y);
	centerDot.setFillColor(sf::Color(200,0,0) );
	mWindow.draw( centerDot );

	// DRAW FIREBALL
	mWindow.draw( mPlayerObj.mSpell.mSpellSprite ); // debug fireball

	// DRAW MONSTERS
	for( int i = 0; i < mMonsters.size(); i++ )	// monsters
	{
		mWindow.draw( mMonsters[i].mMonsterSprite );
		mWindow.draw( mMonsters[i].mMonsterHealthBar );
	}

	// DRAW ENVIRONMENT
	mLevelMaker.display(mWindow);  // environment ( trees, portal, etc. )

	displayGold();		// dropped by monsters


	// DRAW USER INTERFACE OVER GAME
	mWindow.setView( mWindow.getDefaultView()); 
	mWindow.draw(mFog);  // fog
	mUI.display( mWindow );  // user int (upper right)
	mInventoryObj.drawSlots( mWindow );
	mInventoryObj.drawItems( mWindow );
	
	mWindow.setView(mView);

	mWindow.display();
}
//-----------------------------------//





//--------OTHER FUNCTIONS--------//
void Game::handlePlayerInput( sf::Keyboard::Key key, bool isPressed )
{

	if( key == sf::Keyboard::W )
	{
		mPlayerObj.mIsMovingUp = isPressed;
	}
	else if( key == sf::Keyboard::S )
	{
		mPlayerObj.mIsMovingDown = isPressed;		
	}
	else if( key == sf::Keyboard::A )
	{
		mPlayerObj.mIsMovingLeft = isPressed;
	}
	else if( key == sf::Keyboard::D )
	{
		mPlayerObj.mIsMovingRight = isPressed;
	}
	else if( key == sf::Keyboard::Space )
	{
		mPlayerObj.mUsingSword = isPressed;
	}

	
	

	// TOGGLE ABILITIES / EVENTS
	if( isPressed )
	{
		if( key == sf::Keyboard::F )
		{
			mPlayerObj.mToggleLamp = isPressed;  // lamp on/off
		}
		else if( key == sf::Keyboard::E )
		{
			mPlayerObj.mChangeLevel = isPressed;  // change level
		}
		if( key == sf::Keyboard::Escape )
		{
			mUI.isMenuRequested = isPressed;
		}
		if( key == sf::Keyboard::Q )
		{
			mPlayerObj.mUsingFlash = isPressed;
		}
		if( key == sf::Keyboard::Num1 )
		{
			if( cooldownBurn > cooldown ) {
				mPlayerObj.mUsingSpell = isPressed;
				cooldownBurn = 0;
			}
		}

	}

}

bool Game::collision( BoxedObj x, BoxedObj y )
{
	sf::IntRect xBox = x.getBox();
	sf::IntRect yBox = y.getBox();

	if( xBox.left + xBox.width < yBox.left ||
		xBox.left > yBox.left + yBox.width ||
		xBox.top > yBox.top + yBox.height  ||
		xBox.top + xBox.height < yBox.top )
	{
		return false;
	}
	else
	{
		return true;
	}
}

bool Game::attackCollision( BoxedObj monster )
{
	if( mPlayerObj.mUsingSword )
	{
		sf::IntRect xBox( mPlayerSword.getPosition().x, mPlayerSword.getPosition().y, mPlayerSword.getTextureRect().width, mPlayerSword.getTextureRect().height );
		sf::IntRect yBox = monster.getBox();

		if( xBox.left + xBox.width < yBox.left ||
		xBox.left > yBox.left + yBox.width ||
		xBox.top > yBox.top + yBox.height  ||
		xBox.top + xBox.height < yBox.top )
		{
			return false;
		}
		else
		{
			return true;
		}
	}
	else
		return false;
}

bool Game::goldCollision( Gold & gold )
{
	sf::IntRect pBox = mPlayerObj.getBox();
	sf::IntRect gBox = gold.getBox();

	if( pBox.left + pBox.width < gBox.left ||
		pBox.left > gBox.left + gBox.width ||
		pBox.top > gBox.top + gBox.height ||
		pBox.top + pBox.height < gBox.top )
	{
		
		return false;
	}
	else
	{
		if( gold.shouldDisplay )
			mPlayerObj.mGold += gold.mAmount;
		gold.shouldDisplay = false;
		std::cout << mPlayerObj.mGold << std::endl;
		return true;
	}

}

// O( tiles * movingObjects ) complexity (oh god)
// O( n * m )
bool Game::environmentCollision( std::vector< std::vector<Tile> > & myTileMap, BoxedObj & myMover )
{
	int xDim = myTileMap.size();
	int yDim = myTileMap[0].size();
	for( int i = 0; i < xDim; i++ )
	{
		for( int j = 0; j < yDim; j++ )
		{
			if( mLevelMaker.tileAt(i,j).isCollidable )
			{
				//countComparisons++;  // DEBUG
				if( collision( myMover, myTileMap[i][j] ) )
					return true;
			}
		}
	}
	return false;
}

bool Game::mapBoundsCollision( Monster & monster )
{
	sf::IntRect monsterBox = monster.getBox();
	if( monsterBox.left < 0 ||
		monsterBox.left + monsterBox.width > mLevelMaker.getTileMap()[0].size() * 64 ||
		monsterBox.top < 0 ||
		monsterBox.top + monsterBox.height > mLevelMaker.getTileMap().size() * 64 )
	{
		return true;
	}
	return false;
}

void Game::movePlayer( sf::Time deltaTime, std::vector< std::vector<Tile> > & myTileMap )
{
	sf::Vector2f movement( 0.f, 0.f );
	sf::Vector2f previousPosition = mPlayerObj.mPlayerSprite.getPosition();
	bool isMoving = false;
	if( mPlayerObj.mIsMovingUp )
	{
		movement.y -= 100.f;
		mPlayerObj.pAnimationSource.y = 3;
		isMoving = true;
	}
	if( mPlayerObj.mIsMovingDown )
	{
		movement.y += 100.f;
		mPlayerObj.pAnimationSource.y = 0;
		isMoving = true;
	}
	if( mPlayerObj.mIsMovingLeft )
	{
		movement.x -= 100.f;
		mPlayerObj.pAnimationSource.y = 1;
		isMoving = true;
	}
	if( mPlayerObj.mIsMovingRight )
	{
		movement.x += 100.f;
		mPlayerObj.pAnimationSource.y = 2;
		isMoving = true;
	}


	if( isMoving )
	{
		mPlayerObj.mUsingClickMove = false;
	}

	mPlayerObj.mPlayerSprite.move( movement * deltaTime.asSeconds() );
	mPlayerObj.updateBox(); // update the position of the bounding box (based on sprite position)	
	if( environmentCollision( myTileMap, mPlayerObj ) )
	{
		mPlayerObj.mPlayerSprite.setPosition( previousPosition );
	}
}

void Game::moveMonsters( sf::Time deltaTime, std::vector< std::vector<Tile> > & myTileMap )
{
	monsterTimer++;
	bool resetMonsterTimer = false;
	
	for( int i = 0; i < mMonsters.size(); i++ )
	{
		if( mMonsters[i].mIsInRange )
		{
			if( mMonsters[i].isAlive )
			{
				mMonsters[i].previousMonsterPosition = mMonsters[i].mMonsterSprite.getPosition();
				mMonsters[i].previousMonstHealthBarPos = mMonsters[i].mMonsterHealthBar.getPosition();
				if( monsterTimer > maxTime )
				{
					resetMonsterTimer = true;
					mMonsters[i].chooseRandomDirection( mt );
				}
				mMonsters[i].move( deltaTime, mt );
				mMonsters[i].animate(frameSpeed,switchFrame);
				// ENVIRONMENT COLLISION	
				if( environmentCollision( myTileMap, mMonsters[i] ) )
				{
					mMonsters[i].mMonsterSprite.setTextureRect( sf::IntRect( 32,0,32,32 ) );
					mMonsters[i].mMonsterSprite.setPosition( mMonsters[i].previousMonsterPosition );
					mMonsters[i].mMonsterHealthBar.setPosition( mMonsters[i].previousMonstHealthBarPos );
				}

				monsterAttack( mMonsters[i], monsterTimer );
				playerAttack( mMonsters[i] );

			}
		}
	}
	if( resetMonsterTimer )
	{
		monsterTimer = 0;
	}
}

void Game::monsterAttack( Monster & monster, int monsterTimer )
{
	// MONSTER ATTACK
	if( collision( mPlayerObj, monster ) )
	{
		if( monsterTimer > maxTime - 1 )
		{
			mPlayerObj.mHealth -= 1;
			if( mPlayerObj.mHealth < 0 )
			{
				mPlayerObj.mHealth = 0;
			}
		}
	}
}

void Game::playerAttack( Monster & monster )
{
	// PLAYER ATTACK
	if( attackCollision( monster ) )  
	{
		monster.mMonsterSprite.setPosition( monster.previousMonsterPosition );
		monster.mMonsterHealthBar.setPosition( monster.previousMonstHealthBarPos );

		/** NOTE:  attackOccurred is used to limit the player to one attack per click. **/
		if( !attackOccurred )
			monster.mHealth -= mPlayerObj.mDamage;
		attackOccurred = true;
	}
	if( !mPlayerObj.mUsingSword )
	{
		attackOccurred = false; 
	}
	if( monster.mHealth <= 0 ) // when the monster dies.
	{
		monsterDeath( monster );
	}
		
	float currentMonstPercentHealth = (((float) monster.mHealth) / ((float)  monster.mMaxHealth));
	monster.mMonsterHealthBar.setTextureRect( sf::IntRect( 0, 0, (184 * currentMonstPercentHealth), 10 ) );  // 184 pixels is the width of the image.
}

void Game::animateEnvironment()
{
	envAnimTimer++;
	if( envAnimTimer > maxAniTime )
	{
		envAnimTimer = 0;
	}
	sf::Vector2i randomSprite;
	if( envAnimTimer > maxAniTime - 1 )
	{
		for( int i = 0; i < mLevelMaker.getTileMap().size(); i++ )
		{
			for( int j = 0; j < mLevelMaker.getTileMap()[i].size(); j++ )
			{
				randomSprite.x = 32 * ( mt() % 4 );
				randomSprite.y = 64;
				if( (mLevelMaker.getTileMap())[i][j].isCollidable )
				{
					(mLevelMaker.getTileMap())[i][j].setSprite( textures.get( Textures::Landscape ) , sf::IntRect( randomSprite.x , randomSprite.y, 32, 32 ) );
				}
			}
		}
	}
}

void Game::checkForDeath()
{
	if( mPlayerObj.mHealth <= 0 )
	{
		sf::Font mFontTimes;
		mFontTimes.loadFromFile("times.ttf");
		std::cout << "1" << std::endl;
		bool menuExited = false;
		sf::RectangleShape background;
		background.setSize( sf::Vector2f( SCREENX, SCREENY ) );
		background.setFillColor( sf::Color::Black );
		background.setPosition( mPlayerObj.mPlayerSprite.getPosition().x - SCREENX/2, mPlayerObj.mPlayerSprite.getPosition().y - SCREENY/2 );
		mWindow.draw(background);
		std::cout << "2" << std::endl;
		sf::Text myText("  You are dead.  Press P to start over.", mFontTimes, 20);
		myText.setPosition( mPlayerObj.mPlayerSprite.getPosition().x - 100, mPlayerObj.mPlayerSprite.getPosition().y - 32 );
		mWindow.draw(myText);
		std::cout << "3" << std::endl;
		mWindow.display();
		std::cout << "4" << std::endl;

		// Clear the Input Stream
		while( sf::Keyboard::isKeyPressed( sf::Keyboard::P ) )
		{
			sf::sleep( sf::milliseconds( 100 ) );
			std::cout << "5" << std::endl;
		}
		std::cout << "6" << std::endl;
		// Menu Loop
		while( !menuExited )
		{	
			std::cout << "7" << std::endl;
			if( sf::Keyboard::isKeyPressed( sf::Keyboard::P ) )
			{
				std::cout << "8" << std::endl;
				menuExited = true;
				restartGame();
				
			}
		}
		std::cout << "" << std::endl;
		clock.restart();  // reset clock used for 'deltaTime' in movement
	}
}

bool Game::attemptCreaturePlacement( sf::Vector2f & pos )
{
	Monster myMonster;
	myMonster.mMonsterSprite.setTexture( textures.get( Textures::Monster ) );
	myMonster.mMonsterHealthBar.setTexture( textures.get( Textures::HealthBar ) );
	myMonster.mMonsterSprite.setPosition( pos );
	myMonster.mMonsterHealthBar.setPosition( pos.x, pos.y - 20 );
	myMonster.mMonsterSprite.setScale( 2.f, 2.f );
	myMonster.updateBox();
	myMonster.mGold.mAmount = mt() % 10 + 2;
	myMonster.mGold.setPosition( 10,10 );
	myMonster.mGold.mGoldIcon.setTexture( textures.get( Textures::Gold ) );
	std::vector< std::vector< Tile > > myMap = mLevelMaker.getTileMap();
	//std::cout << "What" << std::endl;
	if( !environmentCollision( myMap, myMonster ) )
	{
		mMonsters.push_back( myMonster );
		return true;
	}
	else
	{
		std::cout << "This one didnt work" << std::endl;
		return false;
	}
}

void Game::placeCreatures()
{

	int numCreatures = 25;
	int counter = 0;
	sf::Vector2f pos;
	pos.x = 0; pos.y = 0;

	while( counter < numCreatures )
	{
		pos.x = mt() % ((mLevelMaker.getTileMap()[0].size() * 64) - 64);
		pos.y = mt() % ((mLevelMaker.getTileMap().size() * 64) - 64 );
		if( attemptCreaturePlacement( pos ) )
		{
			counter++;
		}
	}
}

void Game::updateMousePosition()
{
	mMousePosition = sf::Mouse::getPosition();
}

void Game::restartGame()
{
	mPlayerObj.mHealth = mPlayerObj.mMaxHealth;
	mPlayerObj.mLevel = 1;
	mPlayerObj.mExp = 0;


	mMonsters.clear();
	mLevelMaker.makeLevel( mt, textures );
	placeCreatures();
	mPlayerObj.mPlayerSprite.setPosition( (float) mLevelMaker.getPlayerPosition().x + mPlayerObj.mPlayerSprite.getTextureRect().width/2, (float) mLevelMaker.getPlayerPosition().y + mPlayerObj.mPlayerSprite.getTextureRect().height/2 );
}

void Game::updateSwordStatus()
{
	if( mPlayerObj.mUsingSword )
	{
		if( atkTimer < 9 )
		{
			atkTimer++;
			
			mPlayerSword.setTexture( textures.get(Textures::Sword) );
			//mPlayerSword.setPosition( mPlayerObj.mPlayerSprite.getPosition() );
			// PLAYER MOVING UP
			if( mPlayerObj.pAnimationSource.y == 3 ) 
			{
				mPlayerSword.setTextureRect( sf::IntRect(0,0,32,20) );
				swordAniSrc.y = 3;
				mPlayerSword.setPosition( mPlayerObj.mPlayerSprite.getPosition().x - 5.f, mPlayerObj.mPlayerSprite.getPosition().y - 30.f );
			}
			// PLAYER MOVING DOWN
			else if( mPlayerObj.pAnimationSource.y == 0 )
			{
				mPlayerSword.setTextureRect( sf::IntRect(0,32,32,20) );
				swordAniSrc.y = 0;
				mPlayerSword.setPosition( mPlayerObj.mPlayerSprite.getPosition().x - 25, mPlayerObj.mPlayerSprite.getPosition().y + 3.f );
			}
			// PLAYER MOVING RIGHT
			else if( mPlayerObj.pAnimationSource.y == 2 ) 
			{
				mPlayerSword.setTextureRect( sf::IntRect(32,0,20,32) );
				swordAniSrc.y = 2;
				mPlayerSword.setPosition( mPlayerObj.mPlayerSprite.getPosition().x, mPlayerObj.mPlayerSprite.getPosition().y - 8.f );
			}
			// PLAYER MOVING LEFT
			else if( mPlayerObj.pAnimationSource.y == 1 ) 
			{
				mPlayerSword.setTextureRect( sf::IntRect(32,32,20,32) );
				swordAniSrc.y = 1;
				mPlayerSword.setPosition( mPlayerObj.mPlayerSprite.getPosition().x - 30.f, mPlayerObj.mPlayerSprite.getPosition().y - 8.f );
			}
		}
		else // if atkTimer >= 50
		{
			mPlayerSword.setPosition( -50.f,-50.f );
			mPlayerObj.animateMovement(atkTimer,frameCounter,frameSpeed,switchFrame,clock2);
		}
	}
	else
	{
		atkTimer = 0;
		swordAniSrc.x = 0;
		mPlayerSword.setPosition( -50.f, -50.f );
	}

	if( swordAniSrc.y == 0 )
	{
		mPlayerSword.setTextureRect( sf::IntRect( swordAniSrc.x * 32, 0, 32, 32 ) );
	}
	else if( swordAniSrc.y == 1 )
	{
		mPlayerSword.setTextureRect( sf::IntRect( swordAniSrc.x * 32, 32, 32, 32 ) );
	}
	else if( swordAniSrc.y == 2 )
	{
		mPlayerSword.setTextureRect( sf::IntRect( swordAniSrc.x * 32, 64, 32, 32 ) );
	}
	else if( swordAniSrc.y == 3 )
	{
		mPlayerSword.setTextureRect( sf::IntRect( swordAniSrc.x * 32, 96, 32, 32 ) );
	}

	if( atkTimer % 3 == 0  && atkTimer != 0 )
	{
		// Update frame of sword swing
		swordAniSrc.x++;
	}
	if( swordAniSrc.x > 2 )
	{
		swordAniSrc.x = 0;
	}
}

void Game::toggleLamp()
{
	if( mPlayerObj.mToggleLamp )
	{
		mPlayerObj.mUsingLamp = !mPlayerObj.mUsingLamp;
		mPlayerObj.mToggleLamp = false;
	}
	// ACT ON RESULT OF LAMP TOGGLE
	if( mPlayerObj.mUsingLamp )
		mFog.setTexture( textures.get( Textures::Lamp ) );
	else
		mFog.setTexture( textures.get( Textures::Fog ) );
}

void Game::monsterDeath( Monster & monster )
{
	monster.isAlive = false;
	monster.mGold.setPosition( monster.mMonsterSprite.getPosition().x + 5, monster.mMonsterSprite.getPosition().y + 5 );
	monster.mGold.setBox( monster.mMonsterSprite.getPosition().x + 5, monster.mMonsterSprite.getPosition().y + 5, 20, 20 );
	monster.mGold.shouldDisplay = true;
	mPlayerObj.mExp += (monster.mLevel - mPlayerObj.mLevel) * 10 + 10;
	monster.mMonsterSprite.setTextureRect( sf::IntRect(0,0,0,0) );
}

void Game::displayGold()
{

	for( int i = 0; i < mMonsters.size(); i++ )
	{
		if( mMonsters[i].mGold.shouldDisplay )
		{
			mMonsters[i].mGold.display( mWindow );
		}
	}
}


// What is wrong in this function...
void Game::playerFlash( std::vector< std::vector<Tile> > & myTileMap )
{
	sf::Vector2f flashPrevPos = mPlayerObj.mPlayerSprite.getPosition();
	if( mPlayerObj.mUsingFlash )
	{
		mPlayerObj.mUsingClickMove = false;
		updateMousePosition();
		

		// get the distance between the cursor and the center of the screen in x,y
		float offset_x = 0;
		float offset_y = 0;
		offset_x = mMousePosition.x - (SCREENX/2);
		offset_y = mMousePosition.y - (SCREENY/2);

		std::cout << offset_x << " , " << offset_y << "       ---       ";
		sf::Vector2i maxFlashDistance( 110, 110 );

		//////////////
		// TO IMPLEMENT:
		// if the distance between the cursor and center of the screen is greater than the maxFlashRadius, set the flash distance to the max.
		//
		// else, set the flash distance to the distance between the cursor and center of screen
		//
		// use a unit vector
		//
		// get rid of this other shit.
		//
		///////////////

		if( abs(offset_x) > maxFlashDistance.x )
		{
			if( offset_x < 0 )
				offset_x = -maxFlashDistance.x ;//+ 16;
			else
				offset_x = maxFlashDistance.x;// - 16;
		}
		else
		{
			/*if( offset_x < 0 )
				offset_x += 16;
			else
				offset_x -= 16;*/
		}
		// if the cursor is further than the longest range flash
		if( abs(offset_y) > maxFlashDistance.y ) 
		{
			if( offset_y < 0 )  // if the cursor is in the upper half
			{
				// set the amount to increase y to
				//negative maxmaxFlashDistance + half player height
				offset_y = -maxFlashDistance.y;// + 16;
			}
			else  // if the cursor is in the lower half
			{
				// set the amount to increase y to
				// maxmaxFlashDistance - half player height
				offset_y = maxFlashDistance.y;// - 16;
			}
		}
		else  // if it is a short range flash
		{
			/*if( offset_y < 0 )
				offset_y += 16;
			else
				offset_y -= 16;*/
		}

		std::cout << offset_x << " , " << offset_y << std::endl;

		// Move Player
		sf::Vector2f newPosition(mPlayerObj.mPlayerSprite.getPosition().x + offset_x, mPlayerObj.mPlayerSprite.getPosition().y + offset_y);
		mPlayerObj.mPlayerSprite.setPosition( newPosition );
		mPlayerObj.updateBox();
		mPlayerObj.mUsingFlash = false;
		// IF THEY FAIL THE FLASH, UNDO IT.
		if( environmentCollision( myTileMap, mPlayerObj )  )
		{
			// reset their position
			mPlayerObj.mPlayerSprite.setPosition( flashPrevPos );
		}
		else if( newPosition == flashPrevPos )  // ALERT! Without this "else if", the game crashes when the player flashes to their position + offset
												// Caused by while loop below ( their new position will always == their flashPrevPos )
		{
			// reset their position
			mPlayerObj.mPlayerSprite.setPosition( flashPrevPos );
		}
		else // OTHERWISE CARRY OUT THE FLASH
		{
			// FLASH ANIMATION
			int tempTimer = 0;
			int tempAniTimer = 0;
			mPlayerObj.mPlayerSprite.setPosition( flashPrevPos );
			mPlayerObj.mPlayerSprite.setTexture( textures.get( Textures::Flash ) );
			mPlayerObj.mPlayerSprite.setTextureRect( sf::IntRect(0,0,40,40) );
			while( mPlayerObj.mPlayerSprite.getPosition() == flashPrevPos )
			{
				
				mPlayerObj.mPlayerSprite.setTextureRect( sf::IntRect( tempAniTimer * 40, 0, 40, 40 ) );
				if( tempTimer == 10 )
				{
					mPlayerObj.mPlayerSprite.setOrigin(16,16);
					mPlayerObj.mPlayerSprite.setPosition( mPlayerObj.mPlayerSprite.getPosition().x + offset_x, mPlayerObj.mPlayerSprite.getPosition().y + offset_y );
					mPlayerObj.mPlayerSprite.setTexture( textures.get( Textures::Player ) );
				}
				mWindow.draw( mPlayerObj.mPlayerSprite );
				render();
				tempTimer++;
				if( tempTimer % 2 == 0 && tempTimer != 0 )
					tempAniTimer++;
			}
		}
	}
}


void Game::playerFireball( std::vector< std::vector<Tile> > & myTileMap, sf::Time deltaTime, std::mt19937 & mt )
{
	float randomRotation = 0;
	Spell& fireball = mPlayerObj.mSpell;


	if( mPlayerObj.mUsingSpell )
	{
		if( fireball.mSpellTimer == 0 )
		{
			// set  the initial position of the fireball, when first cast
			fireball.mSpellSprite.setPosition(   mPlayerObj.mPlayerSprite.getPosition().x,
											     mPlayerObj.mPlayerSprite.getPosition().y   );
		}

		sf::Vector2f unitVector = MouseService::getMouseUnitVector( sf::Vector2f(SCREENX/2, SCREENY/2) );

		// ROTATION OF THE SPELL (buggy?)
		if( unitVector.x > 0 )
		{
			randomRotation = 80 + mt() % 21;
			fireball.mSpellSprite.setRotation( randomRotation );
		}
		else
		{
			randomRotation = 260 + mt() % 21;
			fireball.mSpellSprite.setRotation( randomRotation );
		}
		if( abs(unitVector.y) > abs(unitVector.x) )
		{
			if( unitVector.y > 0 )
			{
				randomRotation = 170 + mt() % 21;
				fireball.mSpellSprite.setRotation( randomRotation );
			}
			else
			{
				randomRotation = 0 + mt() % 11;
				fireball.mSpellSprite.setRotation( randomRotation );
			}
		}
		// !!!! bug?
		if( (unitVector.x > 0.25) && (-0.25 > unitVector.y) )
		{
			randomRotation = 35 + mt() % 21;
			//fireball.mSpellSprite.setRotation( 45.f );
			fireball.mSpellSprite.setRotation( randomRotation );
		}
		if( (unitVector.x < -0.25) && ( -0.25 > unitVector.y ) )
		{
			randomRotation = 305 + mt() % 21;
			fireball.mSpellSprite.setRotation( randomRotation );
		}
		if( (unitVector.x > 0.25) && (0.35 < unitVector.y ) )
		{
			randomRotation = 125 + mt() % 21;
			fireball.mSpellSprite.setRotation( randomRotation );
		}
		if( (unitVector.x < -0.25) && (0.35 < unitVector.y ) )
		{
			randomRotation = 215 + mt() % 21;
			fireball.mSpellSprite.setRotation( randomRotation );
		}
		// END ROTATION

		fireball.mSpellTimer++;

		if( fireball.mSpellTimer < fireball.mSpellTimerMax )
		{
			// move using the unit vector! :D			
			fireball.mSpellSprite.move( unitVector * ( fireball.mSpellSpeed * deltaTime.asSeconds() ) );
			fireball.updateBox();

			// CHECK FOR COLLISIONS HERE
			for( int i = 0; i < mMonsters.size(); i++ )
			{
				if( mMonsters[i].isAlive )
				{
					if( collision( mPlayerObj.mSpell, mMonsters[i] ) )
					{
						if( !attackOccurred )
						{
							// Animate an explosion..  (maybe use mSpell.explosionSprite)
							fireball.mSpellTimer = 0;
							fireball.mSpellSprite.setPosition( -100,-100 );
							mPlayerObj.mUsingSpell = false;
							mMonsters[i].mHealth -= 30;
							if( mMonsters[i].mHealth <= 0 )
							{
								mMonsters[i].mHealth = 0;
							}
						}
						attackOccurred = true;
					}
				}
			}
			if( environmentCollision( myTileMap, mPlayerObj.mSpell ) )
			{
				fireball.mSpellTimer = 0;
				fireball.mSpellTimer = 0;
				fireball.mSpellSprite.setPosition( -100,-100 );
				mPlayerObj.mUsingSpell = false;
			}
		}
		else
		{
			fireball.mSpellTimer = 0;
			fireball.mSpellSprite.setPosition( -100,-100 );
			mPlayerObj.mUsingSpell = false;
		}

		if( !mPlayerObj.mUsingSpell )
		{
			attackOccurred = false;
		}
	}


}

void Game::clickMove( sf::Time deltaTime, std::vector< std::vector<Tile> > & myTileMap )
{
	if( mPlayerObj.mIsMovingDown || mPlayerObj.mIsMovingLeft || mPlayerObj.mIsMovingRight || mPlayerObj.mIsMovingUp )
	{
		return;
	}


	if( sf::Mouse::isButtonPressed( sf::Mouse::Right ) )
	{
		updateMousePosition();
		if( mMousePosition.x > SCREENX / 6 && mMousePosition.x < (SCREENX / 6) * 5 &&
			mMousePosition.y > SCREENY / 6 && mMousePosition.y < (SCREENY / 6) * 5 && 
			mMousePosition.x != SCREENX/2 && mMousePosition.y != SCREENY/2 )
		{
			mPlayerObj.mUsingClickMove = true;
			mPlayerObj.mMouseNormal.x = mMousePosition.x - (SCREENX/2);
			mPlayerObj.mMouseNormal.y = mMousePosition.y - (SCREENY/2);



			mPlayerObj.mNormalizer = sqrt( (mPlayerObj.mMouseNormal.x * mPlayerObj.mMouseNormal.x) + (mPlayerObj.mMouseNormal.y * mPlayerObj.mMouseNormal.y) );

			mPlayerObj.mMouseNormal.x = mPlayerObj.mMouseNormal.x / mPlayerObj.mNormalizer;
			mPlayerObj.mMouseNormal.y = mPlayerObj.mMouseNormal.y / mPlayerObj.mNormalizer;
			if( abs(mPlayerObj.mMouseNormal.x) > abs(mPlayerObj.mMouseNormal.y) )
			{
				if( mMousePosition.x > (SCREENX/2) )
				{
					mPlayerObj.pAnimationSource.y = 2;
				}
				else
				{
					mPlayerObj.pAnimationSource.y = 1;
				}
			}
			else
			{
				if( mMousePosition.y < (SCREENY/2) )
				{
					mPlayerObj.pAnimationSource.y = 3;
				}
				else
				{
					mPlayerObj.pAnimationSource.y = 0;
				}
			}
		}
		else
		{
			return;
		}
			



	}
	else
	{
		mPlayerObj.mUsingClickMove = false;
	}

	if( mPlayerObj.mUsingClickMove )
	{
		//sf::Vector2f prevPos = mPlayerObj.mPlayerSprite.
		//std::cout << "mouseNormal: " << mPlayerObj.mMouseNormal.x << " , " << mPlayerObj.mMouseNormal.y << std::endl;
		mPlayerObj.mPlayerSprite.move( mPlayerObj.mMouseNormal * 100.f * deltaTime.asSeconds() );
		mPlayerObj.updateBox();
		if( environmentCollision( myTileMap, mPlayerObj ) )
		{
			mPlayerObj.mPlayerSprite.move( - ( mPlayerObj.mMouseNormal * 100.f * deltaTime.asSeconds() ) );
			mPlayerObj.mUsingClickMove = false;
			mPlayerObj.mDistanceMoved = 0;
		}
		mPlayerObj.mDistanceMoved += sqrt( (mPlayerObj.mMouseNormal.x * 100.f * deltaTime.asSeconds() ) * (mPlayerObj.mMouseNormal.x * 100.f * deltaTime.asSeconds() ) +
											(mPlayerObj.mMouseNormal.y * 100.f * deltaTime.asSeconds() ) * (mPlayerObj.mMouseNormal.y * 100.f * deltaTime.asSeconds() ) );

		if( mPlayerObj.mDistanceMoved >  sqrt( mPlayerObj.mNormalizer * mPlayerObj.mNormalizer ) )
		{
			mPlayerObj.mUsingClickMove = false;
			mPlayerObj.mDistanceMoved = 0;
		}

	}
}

void Game::animatePortal()
{
	if( portalAniTimer % 10 == 0 )
	{
		mLevelMaker.getPortal().getSprite().setTextureRect( sf::IntRect( (portalAniTimer / 10) * 64, 0, 64, 64 ) );
	}
	portalAniTimer++;
	if( portalAniTimer > 70 )
	{
		portalAniTimer = 0;
	}
}

void Game::checkMonstersInRange()
{
	for( int i = 0; i < mMonsters.size(); i++ )
	{
		sf::Vector2f monsterPos = mMonsters[i].mMonsterSprite.getPosition();
		sf::Vector2f playerPos = mPlayerObj.mPlayerSprite.getPosition();

		if( sqrt( ( monsterPos.x - playerPos.x ) * (monsterPos.x - playerPos.x)
				+ ( monsterPos.y - playerPos.y ) * (monsterPos.y - playerPos.y) ) < 500.f )
		{
			mMonsters[i].mIsInRange = true;
		}
		else
		{
			mMonsters[i].mIsInRange = false;
		}
	}
}