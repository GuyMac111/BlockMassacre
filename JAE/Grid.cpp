#include "Grid.h"

///////////////////////////
/////THE EVERYTHING BURRITO
///////////////////////////


void Grid::doGridCalculations()
{
	switch(gridState)
	{
		case STATIC:
			//scan the grid, destroy the blocks, check if the empty list is populated
			//if it is, it's time to switch to CASCADING mode
			scanGridForMatches();
			if(thereAreEmptyBlocks())
			{
				if(!blocksHaveSettled())
				{
					gridState = CASCADING;
				}
				else
				{
					gridState = REPLACING_GEMS;
				}
			}
			else{
				initialBreakFlag = true;//we're good to go adding + multiplying score
				///////Score mechanic
				if(scoreMultiplier != 0)
				{
					scoreMultiplier = 0; //reset the score multiplie var
				}
			}

			break;

		case CASCADING:
			//scan the empty list, calling emptyHasBlocksAboveIt func
			cascadeBlocks();
			if(blocksHaveSettled())
			{
				gridState = STATIC;
				//we revert to STATIC to ensure that ALL the matches 
				//are destroyed before we end the round and replace the gems
			}

			break;

		case REPLACING_GEMS:
			whoNeedsABlock();
			break;

		case SWAPPING:
			if(swapBlocks())//if the blocks are finished swapping
			{
				if(scanGridForMatchesAfterSwap())//and if there are matches in the grid as a result
				{
					gridState = STATIC; //set the gridstate to STATIC so that the game can continue like normal
					//if problems with destroying blocks occur
					//try clearing the deathList here
				
				}else
				{//if there were no blocks matching, set the blocks to head back, and the gridpos to Swapping back, to avoid infinites
					blocksSwitchPos(recentBlockSwitched1, recentBlockSwitched2, SWAPPING_BACK);
										
				}
			}
			break;

		case SWAPPING_BACK:
			if(swapBlocks())//if the blocks are done swapping back
			{
				gridState = STATIC;//the grid reverts to static and play continues
			}
			break;


			



		


	}
}

//////////////////////////
/////////FOR CHECKING GRID
//////////////////////////
bool Grid::slotIsValid(int slotX, int slotY)
{
	if(		(0<=slotX)  &&  (slotX<xMax)	)
	{
		if(		(0<=slotY)  &&  (slotY<yMax)	)
		{
			return true;
		}
		else
		{
			//printf("\nGrid slot (%i,%i) does not exist, but is being requested", slotX, slotY);
			return false;
		}
	}
	else
	{
		//printf("\nGrid slot (%i,%i) does not exist, but is being requested", slotX, slotY);
		return  false;
	}
}

bool Grid::slotIsEmpty(int slotX, int slotY)
{
	if(slotIsValid(slotX,slotY))//check if the slot is valid first
	{
		if(!gemGrid[slotX][slotY])//if the gemgrid array returns anything but Null, that slot is occupado 
		{
			return true;
		}
		else
		{
			//printf("\nSlot (%i,%i) is full",slotX, slotY);
			return false;
		}
	}
	else{
		return false;
	}
	
}

bool Grid::checkUp(int slotX, int slotY)
{
	/////important it's done in this order, in order to save lots of time.
	/////Means that edge blocks and blocks near the edge will not bother checking in 1 or 2 directions.
	if(slotIsValid(slotX,slotY+2) && !slotIsEmpty(slotX,slotY+2))//if the slot 2 ABOVE is valid
	{
		if(slotIsValid(slotX,slotY+1) && !slotIsEmpty(slotX,slotY+1))//AND if the slot DIRECTLY ABOVE is valid
		{
			//perform our colour checks
			if(	(gemGrid[slotX][slotY]->blockColour == gemGrid[slotX][slotY+1]->blockColour) && (gemGrid[slotX][slotY]->blockColour == gemGrid[slotX][slotY+2]->blockColour))
			{
				//if they're all the same colour. DESTROY (or add them to the deathlist)
				addBlockToDeathList(slotX,slotY);//initial block marked for death
				addBlockToDeathList(slotX,slotY+1);//block above marked for death
				addBlockToDeathList(slotX,slotY+2);//top block marked for death
				return true;
			}
			else{
				return false;//if not a match, return false
			}
		}
		else{ return false;//if 1 slot above is not valid return false
		}		
	}
	else{
		return false;//if top slot above is not valid
	}
}

bool Grid::checkDown(int slotX,int slotY)
{
	/////important it's done in this order, in order to save lots of time.
	/////Means that edge blocks and blocks near the edge will not bother checking in 1 or 2 directions.
	if(slotIsValid(slotX,slotY-2) && !slotIsEmpty(slotX,slotY-2))//if the slot 2 below is valid
	{
		if(slotIsValid(slotX,slotY-1)  && !slotIsEmpty(slotX,slotY-1))//AND if the slot DIRECTLY BELOW is valid
		{
			//perform our colour checks
			if(    (gemGrid[slotX][slotY]->blockColour == gemGrid[slotX][slotY-1]->blockColour) && (gemGrid[slotX][slotY]->blockColour == gemGrid[slotX][slotY-2]->blockColour))
			{
				//if they're all the same colour. DESTROY (or add them to the deathlist)
				addBlockToDeathList(slotX,slotY);//initial block marked for death
				addBlockToDeathList(slotX,slotY-1);//block below marked for death
				addBlockToDeathList(slotX,slotY-2);//bottom marked for death
				return true;
			}
			else{
				return false;//if not a match, return false
			}
		}
		else{ return false;//if 1 slot above is not valid return false
		}		
	}
	else{
		return false;//if top slot above is not valid
	}
}

bool Grid::checkLeft(int slotX, int slotY)
{
	/////important it's done in this order, in order to save lots of time.
	/////Means that edge blocks and blocks near the edge will not bother checking in 1 or 2 directions.
	if(slotIsValid(slotX-2,slotY) && !slotIsEmpty(slotX-2,slotY))//if the slot 2 LEFT is valid
	{
		if(slotIsValid(slotX-1,slotY) && !slotIsEmpty(slotX-1,slotY) )//AND if the slot DIRECTLY LEFT is valid
		{
			//perform our colour checks
			if(  (gemGrid[slotX][slotY]->blockColour == gemGrid[slotX-1][slotY]->blockColour) && (gemGrid[slotX][slotY]->blockColour == gemGrid[slotX-2][slotY]->blockColour))
			{
				//if they're all the same colour. DESTROY (or add them to the deathlist)
				addBlockToDeathList(slotX,slotY);//initial block marked for death
				addBlockToDeathList(slotX-1,slotY);//block 1 left marked for death
				addBlockToDeathList(slotX-2,slotY);//block 2 left marked for death
				return true;
			}
			else{
				return false;//if not a match, return false
			}
		}
		else{ return false;//if 1 slot above is not valid return false
		}		
	}
	else{
		return false;//if top slot above is not valid
	}
}

bool Grid::checkRight(int slotX, int slotY)
{
	/////important it's done in this order, in order to save lots of time.
	/////Means that edge blocks and blocks near the edge will not bother checking in 1 or 2 directions.
	if(slotIsValid(slotX+2,slotY) && !slotIsEmpty(slotX+2,slotY))//if the slot 2 RIGHT is valid
	{
		if(slotIsValid(slotX+1,slotY) && !slotIsEmpty(slotX+1,slotY))//AND if the slot DIRECTLY RIGHT is valid
		{
			//perform our colour checks
			if(	(gemGrid[slotX][slotY]->blockColour == gemGrid[slotX+1][slotY]->blockColour) && (gemGrid[slotX][slotY]->blockColour == gemGrid[slotX+2][slotY]->blockColour))
			{
				//if they're all the same colour. DESTROY (or add them to the deathlist)
				addBlockToDeathList(slotX,slotY);//initial block marked for death
				addBlockToDeathList(slotX+1,slotY);//block 1 right marked for death
				addBlockToDeathList(slotX+2,slotY);//block 2 right marked for death
				return true;
			}
			else{
				return false;//if not a match, return false
			}
		}
		else{ return false;//if 1 slot above is not valid return false
		}		
	}
	else{
		return false;//if top slot above is not valid
	}
}

bool Grid::checkSorroundings(int slotX, int slotY)
{
	bool thereAreBlocksToKill = false;
	if(checkUp(slotX,slotY))//if the checking funtion yields true, there are blocks to kill
	{
		thereAreBlocksToKill = true;
	}
	
	if(checkDown(slotX,slotY))//if the checking funtion yields true, there are blocks to kill
	{
		thereAreBlocksToKill = true;
	}

	if(checkLeft(slotX,slotY))//if the checking funtion yields true, there are blocks to kill
	{
		thereAreBlocksToKill = true;
	}

	if(checkRight(slotX,slotY))//if the checking funtion yields true, there are blocks to kill
	{
		thereAreBlocksToKill = true;
	}

	if(thereAreBlocksToKill)//if at the end of all this, the Deathlist is populated, return true
	{
		return true;
	}
	else{
		return false;
	}
}

void Grid::scanGridForMatches()
{
	bool thereIsStuffToKill = false;
	for(int i = 0; i<xMax; i++)//cycle through grid 
	{
		for(int j = 0; j<yMax; j++)
		{
			if(!slotIsEmpty(i,j))
			{
				if(checkSorroundings(i,j))//checking each blocks sorroundings
				{
					thereIsStuffToKill = true;//if it returns true, there is stuff to kill
				}
			}
		}
	}
	if(thereIsStuffToKill)//if after all that there's stuff in the deathlist
	{
		/////Score mechanic
		if(initialBreakFlag)
		{
			scoreMultiplier++;//increment the score multiplier
		}
		//////

		destroyAllMarkedBlocks();//destroy all the marked blocks and clear the deathlist
		sManager->playBreakFX(scoreMultiplier);//Also play the breaking sound
	}else{
		///////Score mechanic
		//if(scoreMultiplier != 0)
		//{
		//	scoreMultiplier = 0; //reset the score multiplie var
		//}
	}
}


///////////////////////
//FOR DESTROYING BLOCKS
///////////////////////
void Grid::addBlockToDeathList(int slotX, int slotY)
{
	bool isAlreadyMarked = false;

	for(unsigned int i = 0; i<deathList.size();i++)//iterate throught the DL
	{
		if(		(slotX == deathList.at(i).x) && (slotY == deathList.at(i).y)	)//if we have a match, leave it off the list by checking the flag
		{
			isAlreadyMarked = true;
		}
	}

	if(!isAlreadyMarked)//if we make it to the end of the loop, and the slot is not already marked for death
	{
		point pointToAdd = point(slotX,slotY);
		deathList.push_back(pointToAdd); //it's now marked for death
	}
	else
	{
		//printf("\n(%i,%i)Already on the deathlist.", slotX,slotY);
	}
}

void Grid::destroyBlock(int slotX, int slotY)
{
	if(slotIsValid(slotX,slotY))//just to be sure, and incase we decide to use the function outside of it's intended context
	{
		if(initialBreakFlag)
		{
			addPointsToGridScore();//Add the points the player earned by destroying this block
			//Then create a new scorePopUp for that position and the number of points that that block accumulated
			//But before we do this, we check to see if there are any SPU's in that grid position
			//if there are, we destroy them first to prevent overlapping
			for(int i = 0; i<popupList.size();i++)
			{
				if(popupList.at(i).popupGridPos.x == slotX && popupList.at(i).popupGridPos.y == slotY)
				{//if it matches one in the list. Delete the one in the list first.
					popupList.erase(popupList.begin()+i);
				}
			}
			popupList.push_back(scorePopUp((baseScore*scoreMultiplier),point(slotX,slotY),gemGrid[slotX][slotY]->centerPos));

		}
			
		//the popup score should also be created here

		///Maybe one day, put a death animation for the gem in here
		gemGrid[slotX][slotY].release();//set that grid slot to null
		
		point emptySlot = point(slotX,slotY);//add the  empty node to the empty vector
		
		empty.push_back(emptySlot);//adds this slot to an array of empty slots

		
	}
}

void Grid::destroyAllMarkedBlocks()
{
	for(unsigned int i = 0; i<deathList.size(); i++)//cycle through the list
	{
		destroyBlock(deathList.at(i).x,deathList.at(i).y);//turn the block to ashes
	}

	deathList.clear();//empty the Deathlist, everything on it is now dead
}

void Grid::drawGrid()
{
	for(int i = 0; i<8; i++)
	{
		for(int j = 0; j<8; j++)
		{
			if(gemGrid[i][j])//make sure it's not null, avoiding making calls to null
			{
			//do stuff here
			gemGrid[i][j]->drawBlock();
			}
		}
	}
}




//////////////////////
//FOR CASCADING BLOCKS
//////////////////////
bool Grid::thereAreEmptyBlocks()
{
	if(empty.size()>0)
	{
		return true;
	}
	else return false;
}

bool Grid::emptyHasBlocksAbove(int x, int y)
{
	

	for(int i = (y); i<yMax; i++)//check the slots above our empty . 
	{
		if(!slotIsEmpty(x,i))
		{
			//RUN THE BLOCKS START CASCADE FUNCTION IN HERE
			point dropBlockTo =point(x,y);
			gemGrid[x][i]->startCascadeBlock(dropBlockTo);//tells the block it needs to be falling now
			//if it isn't already. As the grid scans from bottom to top
			//it should prevent blocks having their destination changed if they
			//are already in motion

			return true;//return that we have found blocks above
		}
		
	}//if it gets all the way through here it means there are no blocks in the valid space above
		//and it should return false
	return false;

}

void Grid::cascadeBlocks()
{
	for(int i = 0; i<xMax; i++)
	{
		for(int j = 0; j<yMax; j++)
		{
			if(gemGrid[i][j])
			{
			if(gemGrid[i][j]->blockState == Block::FALLING)//if the block's state is falling
			{
				
				if(gemGrid[i][j]->cascadeBlock())//move the block downward
				{

					////WHERE YOU LEFT OFF
					//USING THE playDropFX() func here as a placeholder for the real dropSoundsFunc
					//IT MAY ACTUALLY BE A GOOD IDEA TO USE THESE FOR THE BLOCKS DROPPING, 
					//AND ANOTHER SET FOR THEM BREAKING (LOUDER, MORE LIKE A SONIC RING PING....KINDA)
					//DONT FORGET TO ADD TO NOTES IN THE .H FILE WHAT YOU'VE DONE
					sManager->playDropFX(scoreMultiplier);
					//if the block has finished falling
					point blockOldPos = point(i,j);//store the old block position
					point destination;
					////Create a  point to temporarily hold where the newly-finished moving block should be
					//so we can rid them from the empty list later
					destination.x = gemGrid[i][j]->gridPos.x;
					destination.y = gemGrid[i][j]->gridPos.y;
					
					//as the blocks gridpos should now be identical to the empty slot 
					//it has visually fallen into
					//we will point that gemGrid slots pointer to THIS block
					gemGrid[(int)gemGrid[i][j]->gridPos.x][(int)gemGrid[i][j]->gridPos.y].reset(gemGrid[i][j].get());
					//and now we point THIS slot at NULL
					
					//we need ensure that we don't delete ourselves if were spawning a new block
					//destined for the top slot
					//Therefore we need to NOT nullify/empty the top slot if our
					//current slot and destination are the same slot
					if(destination.y != blockOldPos.y)
					{
						gemGrid[(int)blockOldPos.x][(int)blockOldPos.y].release();
						//I believe that's called "The Old Switcheroo"!
						//(......I'm such a genius)
					
						//OH! don't forget to add the oldBlockSlot to the 'empty list'
						empty.push_back(blockOldPos);//after all...it's empty now
					
					}
						
						
					

					//ALSO dont forget to remove the destination slot from the empty array
					//THIS will require some looping
					for(unsigned int e = 0; e<empty.size(); e++)
					{
						if(		(empty.at(e).x == destination.x )	&&	(empty.at(e).y == destination.y)	)
						{
							empty.erase(empty.begin()+e);//get it out of the array, as it's no longer empty
							//printf("Block (%i,%i) removed from the empty array",destination.x, destination.y );
						}
					}

					//finally reset the now occupied block's fall variables
					
					gemGrid[(int)destination.x][(int)destination.y]->resetFallVars();


				}
			}
		}
		
		}
	}
}

bool Grid::blocksHaveSettled()
{

	bool stillBlocksToDrop = false;//STARTS OFF FALSE. 
	//This will allow us to drop more blocks at once
	for(unsigned int i = 0; i<empty.size(); i++) //cycle the empty slot list
	{	
		if(emptyHasBlocksAbove(empty.at(i).x,empty.at(i).y)) //if this member of the empty list has a block above it
		{
			stillBlocksToDrop = true; //WE AINT DONE CASCADING YET!!!
		}
	}
	if(!stillBlocksToDrop)
	{//if we make it here, we inherently have no empties left with blocks
	//above. Therefore we can move-on to replacing the empty space
		return true;
	}
	else{
		return false;
	}

}

//////////////////////
//FOR REPLACING BLOCKS
//////////////////////
bool Grid::blockNeededInRow(unsigned int row)
{

	for(unsigned int i = 0; i<yMax; i++)
	{
		if(slotIsEmpty(row,i))
		{
			//ADD THE BLOCK HERE, at y value 8, to generate it at the top
			//also give it a destination using the special constructor
			point destination = point(row,i);
			gemGrid[row][yMax-1].reset( new Block(row,(yMax-1), destination, gridOffset));
			return true;//and break out of the function here if we have dropped a block
		}


	}//if we make it here there was no block needed in the row and this function
	//will return false
	return false;

}

void Grid::whoNeedsABlock()
{
	bool thereAreBlocksInNeedOfDropping = false;
	for(int i = 0; i<xMax; i++)//cycle through the rows
	{
		if(blockNeededInRow(i))//if any row needs a block
		{
			thereAreBlocksInNeedOfDropping = true;//then we should change our 
			//state in order to cascade it downward
			//but dont break out here, otherwise we'll do one row at a time
		}
	}
	if(thereAreBlocksInNeedOfDropping)gridState = CASCADING;//self explanitory
	else
	{
		//if we get here it inherently means there is no more space in the grid to be droppin blocks
		gridState = STATIC;//and we revert to where the game started
	}
	
	
}


////////////////////////
///FOR SELECTING BLOCKS
////////////////////////
bool Grid::mouseIsOverBlock(int bX, int bY, int mX, int mY)
{
	
	point bSize = gemGrid[bX][bY]->size;///get the blocks size
	point bCenterPos = gemGrid[bX][bY]->centerPos;///get the blocks centerPos
	if((mX>(bCenterPos.x-(bSize.x/2)))	&&	(mX<(bCenterPos.x+(bSize.x/2))))//if the mouses x is between blocks width boundaries
	{
		if((mY>(bCenterPos.y-(bSize.y/2)))	&&	(mY<(bCenterPos.y+(bSize.y/2))))//same for the y value
		{
			//then that means that the mouse is within this blocks boundaries
			return true; //and we should change it's state according
		}
		else{
			return false;//if not then the block is not being hovered and should be 
		}
		
	}
	else{
		return false;//if not then the block is not being hovered
	}
	
}

void Grid::toggleSelected(point mousePosition)
{ 
	if(!thereAreEmptyBlocks())
	{
		if((selecBlock.x ==-2) && (selecBlock.y ==-2))
		{
			for(int i = 0; i<xMax; i++)//cycle through the grid
			{
				for(int j = 0; j<yMax; j++)
				{
					if(gemGrid[i][j]->blockState != Block::SELECTED)//we don't want to bother checking the SELECTED block and messing it up
					{
						if(mouseIsOverBlock(i,j,(int)mousePosition.x,(int)mousePosition.y ))
						{
							//the mouse is over the block so we will set it to Selected
							gemGrid[i][j]->blockState = Block::SELECTED;
							//set the hovering block
							selecBlock.x = i;
							selecBlock.y = j;
							allowedSwapSlots.clear();
							setSwapBlockSelection(i,j,allowedRad);
						}
						else
						{
							//if the mouse isn't over the block make sure it stays STATIC
							gemGrid[i][j]->blockState = Block::STATIC;
						}
					}
					else{//if the block IS selected
						gemGrid[i][j]->blockState = Block::STATIC;//toggle it back to static
						selecBlock.x = -2; //empty the selecBlock
						selecBlock.y = -2;
						allowedSwapSlots.clear();
						setSwapBlockSelection(-2,-2,allowedRad);
					}
				}
			}
		}else{//if the selecblock is -2-2 there is a block selected
			//engage the swap
			select2ndBlock(mousePosition, selecBlock);
		
		}
	}
}

void Grid::unselect()
{
	if(selecBlock.x != -2 && selecBlock.y != -2)
	{
		if(gridState == STATIC)
		{
			for(int i = 0; i<xMax; i++)
			{
				for(int j = 0; j<yMax; j++)
				{
					if(gemGrid[i][j]->blockState==Block::SELECTED)
					{
						gemGrid[i][j]->blockState = Block::STATIC;//toggle it back to static
						selecBlock.x = -2; //empty the selecBlock
						selecBlock.y = -2;
						allowedSwapSlots.clear();
						setSwapBlockSelection(-2,-2,allowedRad);
					}
				}
			} 
		}
	}
}

void Grid::hoverOverBlocks(point mousePosition)
{
	if(!thereAreEmptyBlocks())
	{
		if((selecBlock.x == -2)&&(selecBlock.y == -2))//if there is NO selected block
		{
			for(int i = 0; i<xMax; i++)//cycle through the grid
			{
				for(int j = 0; j<yMax; j++)
				{
					if(gemGrid[i][j]->blockState != Block::SELECTED)//we don't want to bother checking the SELECTED block and mess it up
					{
						if(mouseIsOverBlock(i,j,(int)mousePosition.x,(int)mousePosition.y ))
						{

							//the mouse is over the block so we will set it to HOVERING
							gemGrid[i][j]->blockState = Block::HOVERING;
							//set the hovering block
							hoveringBlock.x = i;
							hoveringBlock.y = j;
						}
						else
						{
							//if the mouse isn't over the block make sure it stays STATIC
							gemGrid[i][j]->blockState = Block::STATIC;
						}
					}
				}
			}
		}
		else{//if its NOT -2-2 there is a block selected and we nbeed only check the allowed blocks array
			mouseIsOverAllowedBlock(mousePosition.x, mousePosition.y);
		}
	}
}

void Grid::setSwapBlockSelection(int x, int y, int allowedRadius)
{
	//set the right allowed blocks
	for(int i = 1; i<=allowedRadius; i++)
	{	
		if(	(0<=(x+1))	&&	((x+1)<xMax)	)
		{
		point allowedSlot = point(x+i,y);
		allowedSwapSlots.push_back(allowedSlot);
		}
	}

	//set the left allowed blocks
	for(int i = 1; i<=allowedRadius; i++)
	{	
		if(	(0<=(x-1))	&&	((x-1)<xMax)	)
		{
		point allowedSlot = point(x-i,y);
		allowedSwapSlots.push_back(allowedSlot);
		}
	}

	//set up
	for(int i = 1; i<=allowedRadius; i++)
	{	
		if(	(0<=(y+1))	&&	((y+1)<xMax)	)
		{
		point allowedSlot = point(x,y+1);
		allowedSwapSlots.push_back(allowedSlot);
		}
	}
	
	//set down
	for(int i = 1; i<=allowedRadius; i++)
	{	
		if(	(0<=(y-1))	&&	((y-1)<xMax)	)
		{
		point allowedSlot = point(x,y-1);
		allowedSwapSlots.push_back(allowedSlot);
		}
	}
}

void Grid::mouseIsOverAllowedBlock(int mX, int mY)
{
	
	for(unsigned int i = 0; i<allowedSwapSlots.size(); i++)//cycle through allowed list
	{
		int a = allowedSwapSlots.at(i).x;
		int b = allowedSwapSlots.at(i).y;


		if(gemGrid[a][b]->blockState != Block::SELECTED)//we don't want to bother checking the SELECTED block and messing it up
					{
						if(mouseIsOverBlock(a,b,(int)mX,(int)mY ))
						{
							//the mouse is over the block so we will set it to HOVERING
							gemGrid[a][b]->blockState = Block::HOVERING;
							//set the hovering block
							hoveringBlock.x = a;
							hoveringBlock.y = b;
						}
						else
						{
							//if the mouse isn't over the block make sure it stays STATIC
							gemGrid[a][b]->blockState = Block::STATIC;
						}
					}
	}
}


/////////////////////
//FOR SWAPPING BLOCKS
/////////////////////

bool Grid::swapBlocks()
{
	vector<point> swapList;//a vector containing the grid location of the two blocks to offically swap when the swap is complete

	for(int i = 0; i<xMax; i++)
	{
		for(int j = 0; j<yMax; j++)
		{
			if(gemGrid[i][j])
			{
				if(gemGrid[i][j]->blockState == Block::SWAPPING)//if the block's state is SWAPPING
				{

					if(gemGrid[i][j]->cascadeBlock())//move the block to where it needs to be
					{
						//if the block has finished falling
						point blockOldPos = point(i,j);//store the old block position
						point destination;
						////Create a  point to temporarily hold where the newly-finished moving block should be
						destination.x = gemGrid[i][j]->gridPos.x;
						destination.y = gemGrid[i][j]->gridPos.y;

						//add this block to the swap list so that we know to offically swap it's vector 
						swapList.push_back(destination);
						
						


					}
				}
			}

		}
	}
	//if after all that there are 2 blocks to be swapped
	if(swapList.size()==2)
	{
	//we officially swap their grid positions, by using a temporary Block ptr
		unique_ptr<Block> tempholder( gemGrid[(int)swapList.at(0).x][(int)swapList.at(0).y].release());
		gemGrid[(int)swapList.at(0).x][(int)swapList.at(0).y].reset( gemGrid[(int)swapList.at(1).x][(int)swapList.at(1).y].release());
		gemGrid[(int)swapList.at(1).x][(int)swapList.at(1).y].reset( tempholder.release());
		
		//
		//finally reset the now occupied block's fall variables, and set them to static
		gemGrid[(int)swapList.at(0).x][(int)swapList.at(0).y]->resetFallVars();
		//gemGrid[(int)swapList.at(0).x][(int)swapList.at(0).y]->blockState = STATIC;
		gemGrid[(int)swapList.at(1).x][(int)swapList.at(1).y]->resetFallVars();
		//gemGrid[(int)swapList.at(1).x][(int)swapList.at(1).y]->blockState = STATIC;

		//and clear the swaplist
		swapList.clear();

		//ensure we set the selected block point to -2-2
		selecBlock.x = -2;
		selecBlock.y = -2;

		return true;
		//return true if the blocks have finished swapping
	}
	else{
	return false;//if not they havent finished swapping and we should let the grid know that 
	//so that we can tell it to continue the swapping loop
	}
}

void Grid::blocksSwitchPos(point b1, point b2, State swapOrBack)
{
	//set them both to static so that swapping can begin
	gemGrid[(int)b1.x][(int)b1.y]->blockState = Block::STATIC;
	gemGrid[(int)b2.x][(int)b2.y]->blockState = Block::STATIC;
	//set both blocks to swapping state along with their destinations
	gemGrid[(int)b1.x][(int)b1.y]->startSwapBlock(b2);//set the 1st blocks destination to the b2 position
	gemGrid[(int)b2.x][(int)b2.y]->startSwapBlock(b1);//set the 1st blocks destination to the b2 position

	if(swapOrBack == SWAPPING_BACK)
	{
		gridState = SWAPPING_BACK;
		//Play the swap sound
		sManager->playSwapFX();
	}
	else{//set the gridState to swapping
		//we only need to set the recent block switch if we're SWAPPING
		recentBlockSwitched1 = b1;
		recentBlockSwitched2 = b2;
		
		gridState = SWAPPING;
		//Play the swap sound
		sManager->playSwapFX();

		allowedSwapSlots.clear();
	}
}

bool Grid::scanGridForMatchesAfterSwap()
{
	bool thereIsStuffToKill = false;
	for(int i = 0; i<xMax; i++)//cycle through grid 
	{
		for(int j = 0; j<yMax; j++)
		{
			if(!slotIsEmpty(i,j))
			{
				if(checkSorroundings(i,j))//checking each blocks sorroundings
				{
					thereIsStuffToKill = true;//if it returns true, there is stuff to kill
				}
			}
		}
	}
	if(thereIsStuffToKill)//if after all that there's stuff in the deathlist
	{
		return true; //let the grid know it needs to switch to static and go back to the beginning 
	}
	else{
		return false;//there are no swaps , so we need to swap back
	}
}

void Grid::select2ndBlock(point mousePosition, point firstBlock)
{
	for(unsigned int i = 0; i<allowedSwapSlots.size(); i++)//cycle through allowed list
	{
		
		int a = allowedSwapSlots.at(i).x;
		int b = allowedSwapSlots.at(i).y;
		
		if(gemGrid[a][b]->blockState != Block::SELECTED)//we don't want to bother checking the SELECTED block and messing it up
		{
			if(mouseIsOverBlock(a,b,(int)mousePosition.x,(int)mousePosition.y ))//if the mouse is over the allowed block in question
			{
				blocksSwitchPos(allowedSwapSlots.at(i), firstBlock, SWAPPING);
			}
			else
			{
				//if the mouse isn't over the block make sure it stays STATIC
				gemGrid[a][b]->blockState = Block::STATIC;
			}
		}
	}
}


//////////////////////////////
//GRID SCORING MECHANICS
//////////////////////////////
void Grid::addPointsToGridScore()
{
	gridScore = gridScore + (baseScore * scoreMultiplier);//self explanitory
	//if you need reminding go look at the desc in the header
}

int Grid::getScore()
{
	return gridScore;
}

int Grid::getMultiplier()
{
	return scoreMultiplier;
}

void Grid::animatePopups()
{
	for(int i = 0; i<popupList.size();i++)
	{
		popupList.at(i).animatePopUp();//animate it
			
	}
}

void Grid::drawPopups()
{
	for(int i = 0; i<popupList.size(); i++)
	{
		popupList.at(i).drawPopUp();//animate it
			//AND if it's completed, kill it
			if(popupList.at(i).isComplete()) 
				{
					popupList.erase(popupList.begin()+i);
				}
		
	}
}

////////////////////////
//MISCELLENEOUS FUNCTIONS
////////////////////////

bool Grid::initialBreakIsDone()
{
	return initialBreakFlag;
}


////////////////////
//CONSTRUCTORS
///////////////////
Grid::Grid(point offset)
{
	//send the random number generator
	time_t now; //get a time value to seed the rand generator
	time(&now);
	srand((unsigned int)now);
	//initialise the maximum swap radius
	allowedRad = 1;
	
	//set the selecBlock to -2-2
	selecBlock.x = -2;
	selecBlock.y = -2;

	//initialise the initialBreakFlag gridScore & scoreMultiplier
	gridScore = 0;
	scoreMultiplier = 0;
	initialBreakFlag = false;

	//set the gridOffest
	gridOffset = offset;
	
	////////////////////////
	for(unsigned int i = 0; i<xMax; i++)
	{
		for(unsigned int k = 0; k<yMax; k++)
		{
			//do stuff here
			gemGrid[i][k].reset(new Block(i,k, gridOffset));
		}
	}

	


	gridState = STATIC;//set the initial gridstate. STATIC makes sense

	sManager.reset(new SoundManager());
}

Grid::Grid(void)
{
}


Grid::~Grid(void)
{
}
