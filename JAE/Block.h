#pragma once
#include <GL/glut.h> 
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "dStructs.h"
#include "texture.h"
#include <time.h>


class Block
{
	typedef dStructs::point point;

public:
	
	enum State	{STATIC, FALLING, HOVERING, SELECTED, SWAPPING};

	enum Colour	{RED, BLUE, PURPLE, GREEN, YELLOW, DEAD};

	Block(void);
	~Block(void);

	Block(int gridPosX, int gridPosY, point offset);
	Block(int gridPosX, int gridPosY, point destination, point offset);//A special constructor
	//it calls the regular constructor, but also after changes the worldPos,
	//and calls startcascadeblock to ensure it is born FALLING 
	
	point gridPos;//the blocks designated position in the 8x8 array
	point centerPos;//the blocks coordinates for the actual purpose of drawing
	
	point size;
	point hovSize;
	point selecSize;

	State blockState;//the state of the block
	Colour blockColour;//the colour of the block

	void drawBlock();//no need to include the drawing coords as parameters. the block knows where it should be. The grid will simply tell it to draw itself.
	

	


	//////////////////////
	//FOR MAKING BLOCKS CASCADE
	///////////
	void startCascadeBlock(point destination);//if the block isn't already falling: AKA STATIC 
	//sets the block's destination, and it's status to FALLING
	//also sets the block's fallRate based on how far the block is from it's destination

	bool cascadeBlock();//if the blocks status is FALLING the grid calls this function on the block.
	//it decrements the blocks position by the fallrate
	//it also returns true when the block reaches it's exact destination
	//as well as it switches the status of this block back to
	//STATIC
	//when the grid is handed this true return,
	//it then points the pointer in the array for the empty slot, to this block.
	//it THEN points this blocks previous slot to NULL, making it empty and completing the move. BAM! WHO'S NEXT!
	void resetFallVars();//oh, and resets the fallDestination and Fallrate to (-1,-1) and (0,0) respectively

	void startSwapBlock(point destination);//similar to the startcascadeblock func,
	//but instead of falling, we change the blocks state to SWAPPING instead
	

private:
	void fall();//decrements the blocks position
	void assignRandomColour();//assigns a random colour to the block dependant upon its' texture
	void generateWorldPos();//gets our block a center position, for us to later to use to draw it.
	//the above function will also inherently define the size of each cell in the grid. I'm thinking each
	//cell should be 40x40. So each cell will be offset initially by 20px by 20px. 

	
	point gridOffset;
	//for crashing blocks
	point fallDestination; //the grid position of the slot that we need the block to fall to.
	point fallRate;

};

