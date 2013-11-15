//1.0-=-CRASHING BUG
//
//	The first crash-causing bug was caused by the blocks only being checked for their 
//	state being "STATIC" in the startCascadeBlock function. Whereas, it is better to make
//	sure that they are simply not already falling. 
//
//	The old way, if the mouse is over the block and it needs to fall, it's state will be stuck 
//	"hovering" and therefore, cannot fall as the startCascadeBlock function (which as the title 
//	suggests, starts the blocks cascading) only checked the block state against STATIC.
//
//	================
//	OLD CODE
//	================
//	void Block::startCascadeBlock(point destination)
//{
//	if(blockState == STATIC)
//	{
//	//set the blocks fall destination
//	fallDestination.x = destination.x;
//	fallDestination.y = destination.y;
//	//convert this into a worldPos-type value to check whether we have hit yet
//	fallDestination.x = (fallDestination.x * size.x) + (size.x/2) + gridOffset.x; 
//	fallDestination.y = (fallDestination.y * size.y) + (size.y/2) + gridOffset.y;
//
//	//set the blocks fallRate
//	fallRate.x = (gridPos.x-destination.x)/(size.x);//move them in multiples of size.y or size.x in order to ensure a clean transition that locks. 
//	fallRate.y = (gridPos.y-destination.y)/(size.y); // also eliminates 
//	//problems from recurring number from the division
//	
//	fallRate.x = (fallRate.x * size.x);//multiply that by size.y to get the worldPos-type value to move the block by
//	fallRate.y = (fallRate.y * size.y);//multiply that by size.y to get the worldPos-type value to move the block by
//	
//	blockState = FALLING;//set the blocks state to falling. 
//	//until the blocks centerpos reaches equality with the falldestination, the block will fall by a relative distance each time
//	}
//}
//
//	The new way, the block's state is checked for being ANYTHING other than already
//	falling. This means that even if the block is being hovered over, its state will
//	be force-switched to FALLING, allowing the grid to continue cascading.
//
//	===============
//	NEW CODE
//	===============
//	void Block::startCascadeBlock(point destination)
//{
//	if(blockState != FALLING)
//	{
//	//set the blocks fall destination
//	fallDestination.x = destination.x;
//	fallDestination.y = destination.y;
//	//convert this into a worldPos-type value to check whether we have hit yet
//	fallDestination.x = (fallDestination.x * size.x) + (size.x/2) + gridOffset.x; 
//	fallDestination.y = (fallDestination.y * size.y) + (size.y/2) + gridOffset.y;
//
//	//set the blocks fallRate
//	fallRate.x = (gridPos.x-destination.x)/(size.x);//move them in multiples of size.y or size.x in order to ensure a clean transition that locks. 
//	fallRate.y = (gridPos.y-destination.y)/(size.y); // also eliminates 
//	//problems from recurring number from the division
//	
//	fallRate.x = (fallRate.x * size.x);//multiply that by size.y to get the worldPos-type value to move the block by
//	fallRate.y = (fallRate.y * size.y);//multiply that by size.y to get the worldPos-type value to move the block by
//	
//	blockState = FALLING;//set the blocks state to falling. 
//	//until the blocks centerpos reaches equality with the falldestination, the block will fall by a relative distance each time
//	}
//}


//2.0-=-HUD::Tween not "snapping"
//
//	Object "fits" when it reaches it's destination.
//	This was due to the rate variable not being converted into a positive number 
//	until AFTER the snapping was done, meaning that checking that the object was
//	within the range to snap could be made erratic by negative numbers. 
//	=========
//	OLD CODE
//	=========
//
//	//Snapping sometimes needs to occur, as miniscule values
//	//are lost when rounding happens
//	//Check if we need to SNAP this frame
//	if( (pos.x >= (dest.x-(rate.x/2))) && (pos.x<= (dest.x+(rate.x/2))) )
//	{//if we're less than HALF the move-rate away either side of the destination
//		pos.x = (double)dest.x; //SNAP!
//	}
//	//Same for the Y value
//	if( (pos.y >= (dest.y-(rate.y/2))) && (pos.y<= (dest.y+(rate.y/2))) )
//	{//if we're less than HALF the move-rate away either side of the destination
//		pos.y = (double)dest.y; //SNAP!
//	}
//
//
//
//	//Move our X vals
//	if(pos.x > dest.x)//if we're to the right of it
//	{
//		pos.x -= rate.x;//go left
//	}else{
//		if(pos.x < dest.x)//if we're to the left of it
//		{
//			pos.x += rate.y;//go right
//		}
//	}
//	//Move our Y vals
//	if(pos.y > dest.y)//if we're above it
//	{
//		pos.y -= rate.y;//move it down
//	}else{
//		if(pos.y < dest.y)//if we're below it
//		{
//			pos.y += rate.y;
//		}
//
//	}
//
//}
//
////let's make the rates positive no matter what
//rate.x = sqrt(pow(rate.x,2));
//rate.y = sqrt(pow(rate.y,2));
//
//
//=============
//	NEW CODE
//	=============
//
//	//let's make the rates positive no matter what
//	rate.x = sqrt(pow(rate.x,2));
//rate.y = sqrt(pow(rate.y,2));
//
//
////Snapping sometimes needs to occur, as miniscule values
////are lost when rounding happens
////Check if we need to SNAP this frame
//if( (pos.x >= (dest.x-(rate.x/2))) && (pos.x<= (dest.x+(rate.x/2))) )
//{//if we're less than HALF the move-rate away either side of the destination
//	pos.x = (double)dest.x; //SNAP!
//}
////Same for the Y value
//if( (pos.y >= (dest.y-(rate.y/2))) && (pos.y<= (dest.y+(rate.y/2))) )
//{//if we're less than HALF the move-rate away either side of the destination
//	pos.y = (double)dest.y; //SNAP!
//}
//
//
//
////Move our X vals
//if(pos.x > dest.x)//if we're to the right of it
//{
//	pos.x -= rate.x;//go left
//}else{
//	if(pos.x < dest.x)//if we're to the left of it
//	{
//		pos.x += rate.y;//go right
//	}
//}
////Move our Y vals
//if(pos.y > dest.y)//if we're above it
//{
//	pos.y -= rate.y;//move it down
//}else{
//	if(pos.y < dest.y)//if we're below it
//	{
//		pos.y += rate.y;
//	}
//
//}
//
//}
/*
===================
FPS COUNTER BUG
===================
The FPS counter was going crazy after the game was restarted. Dropping. 
This was because the callback func FPSmanagement was being registered multiple times
, therefore making the times it was called per second increase. As the inner variable 'cycle' was constantly
being reset, this made the fps timer spaz-out and appear to be lower than it was.

It was fixed by moving the FPSManagement function out of the gameplay init function. And into the main() func. This guarantee's that 
it is exectued exactly once and only once.*/ 

//================
//Unresolved External Symbols
//================
//Always a real bitch. Can be anything from a .lib file not being mentioned 
//in the additional dependancies, to your static variables being declared in the wrong place.
//A few things to try:
//
//	1) Ensure the correct headers are included. (the obvious one)
//
//	2) Go into project properties > Linker > input > additional dependancies and ensure your library 
//	is properly represented. Also check the config (debug/release) are both identical. If not, one of 
//	these could be the problem, or you could be using the wrong one. For default libs such as windows stuff,
//try:
//	copying the stuff that it's set to depend on to somewhere safe. Then go to edit > check "inheret blah blah from parent/blah". 
//	This has reset them to the stuff that it's just supposed to be anyway. (In fairness
//	i did screw-up and accidently delete them whilst altering the config).
//	DON'T FORGET to paste your own .lib dependencies back in!!!
//
//	3) I've been given UES errors when using static objects in the past. Ensure 
//	these are declared at the top of the .cpp, and not the header file.
//	I'm assuming this is because the compiler likes to compile these first, as they don't go out of scope.
//	See the SoundManager.cpp as an example.





