
#include "Block.h"
#include "Texture.h"


//textures created statically, in order to avoid ramming the  video memory full (no pun intended)
	static unique_ptr<Texture> redtex;
	static unique_ptr<Texture> bluetex;
	static unique_ptr<Texture> greentex;
	static unique_ptr<Texture> purpletex;
	static unique_ptr<Texture> yellowtex;
	
	
	

Block::Block(int gridPosX, int gridPosY, point offset)
{
	size.x = 40;//In fact, let's give our blocks a size variable.
	size.y = 40;
	hovSize.x = size.x + (size.x/10);
	hovSize.y = size.y + (size.y/10);
	selecSize.x = size.x + (size.x/5);
	selecSize.y = size.y + (size.y/5);


	//initialise the gridPos of this particular block
	gridPos.x = gridPosX;
	gridPos.y = gridPosY;
	//grid offset needs setting BEFORE generateworldpos is called
	gridOffset.x = offset.x;
	gridOffset.y = offset.y;
	//based on this, give our block an initial world drawing position
	generateWorldPos();

	//initialise the blockState to STATIC
	blockState = STATIC;

	//give our block a colour
	assignRandomColour();

	greentex.reset( new Texture(32,32,"Sprites/Green.png"));
	redtex.reset( new Texture(32, 32, "Sprites/Red.png"));
	bluetex.reset( new Texture(32,32,"Sprites/Blue.png"));
	purpletex.reset( new Texture(32,32,"Sprites/Purple.png"));
	yellowtex.reset( new Texture(32,32,"Sprites/Yellow.png"));
	
}

Block::Block(int gridPosX, int gridPosY, point destination, point offset)
{	
	//THIS IS NOT LEGAL////////////////////////////WE ARE NOT USING OBJECTIVE-C GUY
	//Block(gridPosX, gridPosY, offset);//call the regular constructor
	////////////////////////////////////////////////////////////////////////
	////////LAZY COPYPASTA TIME
	///////////////////////////////

	size.x = 40;//In fact, let's give our blocks a size variable.
	size.y = 40;
	hovSize.x = size.x + (size.x/10);
	hovSize.y = size.y + (size.y/10);
	selecSize.x = size.x + (size.x/5);
	selecSize.y = size.y + (size.y/5);
	

	//initialise the gridPos of this particular block
	gridPos.x = gridPosX;
	gridPos.y = gridPosY;
	//grid offset needs setting BEFORE generateworldpos is called
	gridOffset.x = offset.x;
	gridOffset.y = offset.y;
	//based on this, give our block an initial world drawing position
	generateWorldPos();

	//initialise the blockState to STATIC
	blockState = STATIC;

	//give our block a colour
	assignRandomColour();

	greentex.reset( new Texture(32,32,"Sprites/Green.png"));
	redtex.reset( new Texture(32, 32, "Sprites/Red.png"));
	bluetex.reset( new Texture(32,32,"Sprites/Blue.png"));
	purpletex.reset( new Texture(32,32,"Sprites/Purple.png"));
	yellowtex.reset( new Texture(32,32,"Sprites/Yellow.png"));

	//////////////////////////////////////
	//////////////////////////////////////
	
	centerPos.y += size.y;//ensure that the centerpos starts 1 square ABOVE the grid
	//however this is deceptive, as when the Block is born, it will technically
	//"occupy" slot 8
	startCascadeBlock(destination);//These special blocks are born falling :)
	//once this has happened, as we have passed it a real destination
	//the cascade function should safely catch the block in the right place

	//however we must edit the fallRate.y
	//needs setting differently from when we regularly call the block
	fallRate.y = (centerPos.y-fallDestination.y)/(size.y/10);
	

}

void Block::generateWorldPos()
{
	//multiply it by the size of the cells, add the 20px to reach the center, then add the offset
	centerPos.x = (gridPos.x * size.x)+(size.x/2)+gridOffset.x;
	centerPos.y = (gridPos.y * size.y)+(size.y/2)+gridOffset.y;
	//printf("worldPos is (%f,%f)", centerPos.x, centerPos.y);

}

void Block::assignRandomColour()
{
	
	int tempRand;
	tempRand = rand() % 5 + 1;//generate a random number between 1 and 5 (no 6 as we don't want to init any blocks to "void")
	switch (tempRand)
	{
		case 1:
			blockColour = RED;
			//printf("\nBlock %i,%i colour set to RED", gridPos.x, gridPos.y);
			break;

		case 2:
			blockColour = BLUE;
			//printf("\nBlock %i,%i colour set to BLUE", gridPos.x, gridPos.y);
			break;

		case 3:
			blockColour = PURPLE;
			//printf("\nBlock %i,%i colour set to PURPLE", gridPos.x, gridPos.y);
			break;

		case 4:
			blockColour = GREEN;
			//printf("\nBlock %i,%i colour set to GREEN", gridPos.x, gridPos.y);
			break;

		case 5:
			blockColour = YELLOW;
			//printf("\nBlock %i,%i colour set to YELLOW", gridPos.x, gridPos.y);
			break;

		default:
			blockColour = PURPLE; //look out for an abnormal number of purples
			//printf("\nDEFAULTDEFAULTDEFAULTDEFAULTDEFAULT", gridPos.x, gridPos.y);
			break;
	}
}

void Block::drawBlock()
{
	switch(blockState)//a switch to draw the blocks differently dependant on their state
	{ 
	case HOVERING:
		switch(blockColour)
		{
		case RED:

			redtex->draw(centerPos, hovSize);
			//printf("\ndrawing RED");
			break;

		case BLUE:
			bluetex->draw(centerPos, hovSize);
			//printf("\ndrawing BLUE");
			break;

		case PURPLE:
			purpletex->draw(centerPos, hovSize);
			//printf("\ndrawing PURPLE");
			break;

		case GREEN:
			greentex->draw(centerPos, hovSize);
			//printf("\ndrawing GREEN");
			break;

		case YELLOW:
			yellowtex->draw(centerPos, hovSize);
			//printf("\ndrawing YELLOW");
			break;

		case DEAD:
			//printf("\n DEAD BLOCK");
			break;

		default:
			//printf("\n DRAW \n\n\n\n\n\n\n\nDEFAULT");
			break;
		} 
		break;

	case SELECTED:
		switch(blockColour)
		{
		case RED:

			redtex->draw(centerPos, selecSize);
			//printf("\ndrawing RED");
			break;

		case BLUE:
			bluetex->draw(centerPos, selecSize);
			//printf("\ndrawing BLUE");
			break;

		case PURPLE:
			purpletex->draw(centerPos, selecSize);
			//printf("\ndrawing PURPLE");
			break;

		case GREEN:
			greentex->draw(centerPos, selecSize);
			//printf("\ndrawing GREEN");
			break;

		case YELLOW:
			yellowtex->draw(centerPos, selecSize);
			//printf("\ndrawing YELLOW");
			break;

		case DEAD:
			//printf("\n DEAD BLOCK");
			break;

		default:
			//printf("\n DRAW \n\n\n\n\n\n\n\nDEFAULT");
			break;
		} 
		break;

	default:
		//HOW we will draw the blocks by default
		//well use a switch to determine which block colour is drawn
		switch(blockColour)
		{
		case RED:

			redtex->draw(centerPos, size);
			//printf("\ndrawing RED");
			break;

		case BLUE:
			bluetex->draw(centerPos, size);
			//printf("\ndrawing BLUE");
			break;

		case PURPLE:
			purpletex->draw(centerPos, size);
			//printf("\ndrawing PURPLE");
			break;

		case GREEN:
			greentex->draw(centerPos, size);
			//printf("\ndrawing GREEN");
			break;

		case YELLOW:
			yellowtex->draw(centerPos, size);
			//printf("\ndrawing YELLOW");
			break;

		case DEAD:
			//printf("\n DEAD BLOCK");
			break;

		default:
			//printf("\n DRAW \n\n\n\n\n\n\n\nDEFAULT");
			break;
		} 
		break;
	
	}																				//Switches within switches bitches
}

//////////////////////////
//FOR MAKING BLOCKS CASCADE
///////////////////////////
void Block::startCascadeBlock(point destination)
{
	if(blockState != FALLING)
	{
	//set the blocks fall destination
	fallDestination.x = destination.x;
	fallDestination.y = destination.y;
	//convert this into a worldPos-type value to check whether we have hit yet
	fallDestination.x = (fallDestination.x * size.x) + (size.x/2) + gridOffset.x; 
	fallDestination.y = (fallDestination.y * size.y) + (size.y/2) + gridOffset.y;

	//set the blocks fallRate
	fallRate.x = (gridPos.x-destination.x)/(size.x/10);//move them in multiples of size.y or size.x in order to ensure a clean transition that locks. 
	fallRate.y = (gridPos.y-destination.y)/(size.y/10); // also eliminates 
	//problems from recurring number from the division
	
	fallRate.x = (fallRate.x * size.x);//multiply that by size.y to get the worldPos-type value to move the block by
	fallRate.y = (fallRate.y * size.y);//multiply that by size.y to get the worldPos-type value to move the block by
	
	blockState = FALLING;//set the blocks state to falling. 
	//until the blocks centerpos reaches equality with the falldestination, the block will fall by a relative distance each time
	}
}

void Block::startSwapBlock(point destination)
{
	if(blockState == STATIC)
	{
	//set the blocks fall destination
	fallDestination.x = destination.x;
	fallDestination.y = destination.y;
	//convert this into a worldPos-type value to check whether we have hit yet
	fallDestination.x = (fallDestination.x * size.x) + (size.x/2) + gridOffset.x; 
	fallDestination.y = (fallDestination.y * size.y) + (size.y/2) + gridOffset.y;

	//set the blocks fallRate
	fallRate.x = (gridPos.x-destination.x)/(size.x/8);//move them in multiples of size.y or size.x in order to ensure a clean transition that locks. 
	fallRate.y = (gridPos.y-destination.y)/(size.y/8); // also eliminates 
	//problems from recurring number from the division
	
	fallRate.x = (fallRate.x * size.x);//multiply that by size.y to get the worldPos-type value to move the block by
	fallRate.y = (fallRate.y * size.y);//multiply that by size.y to get the worldPos-type value to move the block by
	
	blockState = SWAPPING;//set the blocks state to swapping. 
	//until the blocks centerpos reaches equality with the falldestination, the block will fall by a relative distance each time
	}
}

bool Block::cascadeBlock()
{
	centerPos.x -= fallRate.x;//decrement
	centerPos.y -= fallRate.y;//decrement

	if(	(centerPos.x == fallDestination.x)	&&	(centerPos.y == fallDestination.y) )
	{
		//if the positions are equal, we have reached our destination, and should therefore
		gridPos.x = (centerPos.x - gridOffset.x - (size.x/2))/size.x; //inverse the worldpos formula to get back to a gridpos
		gridPos.y = (centerPos.y - gridOffset.y - (size.y/2))/size.y;

		//we then neen to set the blocks state to STATIC, as it has 
		//reached it's destination and should stop falling
		blockState = STATIC;
		
		//but we need to let the grid know this. So to give it a hint we will return true
		return true;
		//now the grid knows that this block has fallen to where it needed to be
	}
	else{//but if we havent reached our falling destination yet, this shizzle should carry on falling
	return false;
	}
}

void Block::resetFallVars()
{
	//now to avoid any unpredictable problems, we're going to set our falldest and fallrate to absurdity
	fallRate.x = 0;
	fallRate.y = 0;
	fallDestination.x = -1;
	fallDestination.y = -1;
}

Block::Block(void)
{
	
}

Block::~Block(void)
{
}
