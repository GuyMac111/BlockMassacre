#pragma once
#include "dStructs.h"
#include "Block.h"
#include <vector>
#include <time.h>
#include "scorePopUp.h"
#include "SoundManager.h"

typedef dStructs::point point;
using namespace std;

const int xMax = 8;
const int yMax = 8;
const int baseScore = 100;

class Grid
{


public:
	
	enum State
	{
		STATIC, CASCADING, REPLACING_GEMS, SWAPPING, SWAPPING_BACK
	};

	
	unique_ptr<Block> gemGrid[xMax][yMax]; 
	point hoveringBlock;
	point selecBlock;
	point gridOffset;
	State gridState;
	void drawGrid();//draws the grid on the screen by looping through and calling each blocks moving function
	void doGridCalculations(); //scans through the grid, doing what needs to be done
	//acts as one massive switch, dependant on what value gridState currently holds
	//it can scan the grid for matches 
	//cascade
	//replace
	void hoverOverBlocks(point mousePosition); //described below in FOR SELECTING BLOCKS

	void toggleSelected(point mousePosition);
	void unselect();

	/////////////
	//SCORE MECHANICS
	////////////
	int getScore();
	int getMultiplier();

	////////////
	///The score popups
	////////////
	void animatePopups();
	void drawPopups();

	bool initialBreakIsDone();//a way for other objects to see whether the initial Break is done
	//and can go about their buisiness

	Grid(point offset);
	Grid(void);
	~Grid(void);


private: 

	shared_ptr<SoundManager> sManager;
	
	point offset;
	point recentBlockSwitched1; //the recent blocks that were switched. stored for swapping back
	point recentBlockSwitched2;

	vector<scorePopUp> popupList;

	int allowedRad;

	vector<point> allowedSwapSlots;//holds the slots a player may swap into once a block is selected

	//for checking the grid
	bool slotIsValid(int slotX, int slotY); //checks if a slot exists on the grid. eg. to avoid out of range exceptions etc
	bool slotIsEmpty(int slotX, int slotY);//checks to see if the slot is currently occupied by a block
	bool checkUp(int slotX, int slotY); //checks the two blocks above for matching colours. Marking any matching blocks for destruction
	bool checkDown(int slotX, int slotY); //checks the two blocks below for matching colours
	bool checkLeft(int slotX, int slotY); // checks the two blocks left for matches
	bool checkRight(int slotX, int slotY); //checks the two blocks right for matching colours
	bool checkSorroundings(int slotX, int slotY); //encapsulates all of the above. Returns a bool so we know whether there actually IS anything to destroy.
	void scanGridForMatches();//cycles through the whole grid applying the above function. 
	//IF it finds blocks to destroy, it calls destroyAllMarkedBlocks

	
	///for destroying blocks
	void destroyAllMarkedBlocks();//destroys any blocks marked for death using the below function.
	//if No blocks need destroying return False. Genius
	//if blocks WERE destroyed, activate the checkSorroundings function. 
	//then CLEARS the deathlist
	//ALSO responsible for playing the breaking sound according to the multiplier we're on.
	void destroyBlock(int slotX, int slotY); //destroys a block
	void addBlockToDeathList(int slotX, int slotY);//checks the block doesn't already exist on the deathlist. if it doesn't, it adds it
	vector<point> deathList; //a list of all the vectors that need destroying this turn 
		
	//for swapping blocks
	void swapBlocks(point block1, point block2);
	//swaps 2 blocks over
	//checks grid for matches
	//activate destroyAllMarkedBlocks. If function returns false, swap the blocks back, nothing more.
	//if the function returns true, destruction and rechecking will occur and this function should break

	//for moving blocks
	void cascadeBlocks(); //cycles through the moveList, applying each block on it's move function.
	vector<point> empty; // lists all the empty spaces. Populated by destroyBlock
	bool thereAreEmptyBlocks();//returns true if there are empty blocks to cascade into.
	bool emptyHasBlocksAbove(int x, int y);//if true get the nearest above block and make it fall
	//if false this means that the column needs refilling
	
	bool blocksHaveSettled(); //scans the empty list asking if the empties have blocks above
	//DGC asks this, it will return false AS SOON as emptyHasBlocksAbove returns true
	//if no calling of emptyHasBlocksAbove returns true, the blocks have settled, and the func
	//returns true
	//IMPORTANT: make sure that cascadeBlocks is called BEFORE blocksHaveSettled.
	
	//For replacing blocks
	//first query thereAreEmptyBlocks()

	void whoNeedsABlock();//cycles through rows of the grid calling the below function and adding them to the list if need be
	bool blockNeededInRow(unsigned int row);//chacks if there is empty space in the row
	//vector<int> rowsInNeed;//An array of ints containing the locations of rows in need of at least 1 block




	//////////////////////
	//FOR SELECTING BLOCKS
	//////////////////////
	bool mouseIsOverBlock(int bX, int bY, int mX, int mY); //returns true if the mouse is within the blocks boundaries
	void setSwapBlockSelection(int x, int y, int allowedRadius);
	void mouseIsOverAllowedBlock(int mX, int mY);
	void select2ndBlock(point mousePosition, point firstBlock);//cycle through the list and call blocksSwitchPos
	

	//VVVV///////////ONLY CALLED IF the gridState is static//////VVVVVVVV//////
	//void hoverOverBlocks(point mousePosition); //scans through the grid, FIRST making sure that the block in question is not SELECTED
	
	//(dont want to switch that one back to STATIC now do we)
	//if the mouse is over a block, 
	//it changes that cell to HOVERING, while switching all others back to STATIC




	//////////////////////
	///FOR SWAPPING BLOCKS
	//////////////////////
	bool scanGridForMatchesAfterSwap();//like the above scangridformatches method but instead of implementing the destroyallmarkedblocksfunction
	//it simply returns true or false as to whether there were matches or not 
	//if it returns true, we should set the grid to STATIC and let play continue
	void blocksSwitchPos(point b1, point b2, State swapOrBack);//func starts the physical swapping of the 2 blocks
	//also switch the state of the grid to swapping
	//param swapOrBack MUST be either SWAPPING or SWAPPING_BACK
	bool swapBlocks();//similar to cascadeBlocks but minus the emptying blocks functionality.

	//////////////////////////
	////GRID SCORING MECHANICS
	//////////////////////////

	//const int baseScore = 10;//REMINDER (it is declared globally) //This is the base score awarded for breaking a block.
	//When actually added to the grids score, it is first individually multiplied by...
	int scoreMultiplier;//(initialize to 0 to init) This multiplies with the base score each time a block breaks.
	//I add one to the multiplier each time destroyAllMarkedBlocks is called. 
	//destroyAllMarkedBlocks() is called ONCE per time blocks need destroying. Really convenient for 
	//our needs.
	//It gets reset to zero each time scanGridForMatches' thereIsStuffToKill bool is found to be false
	//This should ensure that it is only reset when the grid is settled AND there are no more matches
	int gridScore; //holds the score accumulated through grid-play
	void addPointsToGridScore();//Function adds the the base-score multiplied
	//by the scoreMultiplier to the gridScore 
	//This function should be called in the destroyBlock funtion.
	
	//NOTE: the getScore & getMultiplier functions are PUBLIC and therefore defined above

	bool initialBreakFlag;//(init to false) A flag that lets us know when the initial breaks made when the grid is created are done.
	//score things don't happen until this is TRUE
	//Don't wanna be giving the player points for free right.
	//This variable is first set to True when the grid is first found to be static

	//NOTE FOR LATER (ANIMATION) the animated popup score should also be created by the destroyBlock function.
	//the popups should be drawn as a part of the draw-grid function



};

