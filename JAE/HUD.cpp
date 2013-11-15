#include "HUD.h"
//WHERE YOU LEFT OFF:
//it's time to add
//"retry" functionality to the Game over screen
//Then the ability to skip the score ticking up. DONE
//The shutters coming down over the gems on-pause.
//The buttons on the titlescreen animating in
//The ability to skip the into animation

//The shutters will probably take the longest but not too long.

	//the colours
	GLfloat HUD::transparent[]= {1.0f,1.0f,1.0f,0.0f}; 
	GLfloat HUD::red[]= {0.7f,0.0f,0.0f,1.0f};
	GLfloat HUD::blue[]= {0.1f,0.2f,0.7f,0.7f};
	GLfloat HUD::white[]= {1.0f,1.0f,1.0f,1.0f}; 
	GLfloat HUD::scoreTextColour []= {1.0f,1.0f,1.0f,1.0f}; //White
	GLfloat HUD::scoreColour []= {0.7f,0.0f,0.0f,1.0f}; //Red
	GLfloat HUD::multiplierColour []= {0.5f, 1.0f, 0.2f, 0.0f};//gets randomized every frame at runtime
	GLfloat HUD::multiplierXColour []= {1.0f,1.0f,1.0f,0.0f}; //White
	GLfloat HUD::timerTextColour []= {1.0f,1.0f,1.0f,1.0f}; //White
	GLfloat HUD::timeColour []= {0.0f,0.0f,7.0f,1.0f}; //Blue
	GLfloat HUD::GO_scoreColour[]= {1.0f,1.0f,1.0f,1.0f};//

void HUD::drawHUD(int score, int multiplier, int timeRemaining)
{
	//////
	//Draw the score text
	/////
	char scoreAsChar[10];//limiting this to 10 will probably cause mad 
	//crashes when scores start getting smashed all round the gaff
	_itoa(score, scoreAsChar,10);
	//we want to pin the score to right-alignment, like in the game-over screen
	//calculate the score's width
	float scoreOffset = getStringWidth(scoreFont,0,0, scoreAsChar);
	printWithColour(scoreTextFont, scorePos.x, scorePos.y, scoreTextColour, scoreText);
	printWithColour(scoreFont, scorePos.x+205-scoreOffset, scorePos.y, scoreColour, scoreAsChar);

	drawMulti(multiplier);
	drawTimer(timeRemaining);
	
}

void HUD::drawGameOver(int score, int multiplier, int timeRemaining, int sizeX, int sizeY)
{
	drawHUD(score, multiplier, timeRemaining);
	//Draw a transparent grey quad over the screen to dim for pausing
	//Set colour
	glColor4f(0.4f,0.4f,0.4f,0.9f);
	glBegin(GL_QUADS);
	glVertex2f(0,0);
	glVertex2f(0,sizeY);
	glVertex2f(sizeX, sizeY);
	glVertex2f(sizeX, 0);
	glEnd();
	//set the colour back to normal
	glColor4fv(scoreTextColour);
	//write "TIME OVER"
	printWithColour(pauseFont, (sizeX/2)-(pauseFont.h*3),sizeY/2+(sizeY/3),scoreTextColour,"TIME OVER");
	if(GO_scoreTimer > 0)GO_scoreTimer--;//count down until we should show the score
	
	if(GO_scoreTimer == 0) //once our artificial interval is over
	{	
		//If the scoretimer has hit zero we can display the score 
		//ticking up to the score the player has reached
		char scoreAstext[10];
		sprintf(scoreAstext,"%.0f",GO_score);
		//We want to align the score RIGHT, as is the tradition with scores. So that the text moves
		//LEFT as the number of digits increases.

		float offset = getStringWidth(GO_scoreFont,0,0,scoreAstext);
		
		//UPDATE: SKIPPING FUNCTIONALITY BE HERE
		//by way of a skipping flag that is waved by the mainclass via a public
		if(skipGO_flag)//false by default
		{
			GO_score = score;//auto-meet the conditions to end the "ticking-up" loop
		}


		if(GO_score<score)
		{//If GO_score is less than score, 
			//print the score white as it is clocking up
			printWithColour(scoreTextFont,(sizeX/2)-100, (sizeY/2)+(sizeY/3)-((GO_scoreFont.h*2)-5),GO_scoreColour, "SCORE:");
			printWithColour(GO_scoreFont, (sizeX/2)+(sizeX/3)-offset,(sizeY/2)+(sizeY/3)-((GO_scoreFont.h*2)-5),GO_scoreColour,scoreAstext);
			float scorePerFrame = score/120;//continue adding to GO_score
			GO_score += scorePerFrame;//divided by the number of frames we want it to take
		}else{
			//if the GO_score has reached its destination, still draw it,
			//but start pulsating
			
			//ALSO throughout the 'clocking up', GO_Score can become 
			//distorted slightly through rounding up and whatnot. 
			//So once we're up here, let's just draw the ACTUAL score.

			char actualScoreAsText[10];
			_itoa(score, actualScoreAsText, 10);
			printWithColour(scoreTextFont,(sizeX/2)-100, (sizeY/2)+(sizeY/3)-((GO_scoreFont.h*2)-5),GO_scoreColour, "SCORE:");
			printWithColour(GO_scoreFont, (sizeX/2)+(sizeX/3)-offset,(sizeY/2)+(sizeY/3)-((GO_scoreFont.h*2)-5),GO_scoreColour,actualScoreAsText);
			pulsateColour(white,blue, GO_scoreColour, CS_GO_score, 20);

			//retry
			float retryOffset = getStringWidth(GO_retryFont,0,0,"Press 'r' to retry")/2;
			float quitOffset = getStringWidth(GO_retryFont,0,0,"Press 'q' to return to the menu")/2;
			printWithColour(GO_retryFont, (sizeX/2) - retryOffset,100, white,"Press 'r' to retry");
			printWithColour(GO_retryFont, (sizeX/2) - quitOffset,100-(GO_retryFont.h+3), white,"Press 'q' to return to the menu");
			
		}

	}

	
}

void HUD::drawMulti(int multiplier)
{

	//screw it, lets randomise the colour for some jazz hands
	multiplierColour[0] = (rand() % 11);
	multiplierColour[1] = (rand() % 11);
	multiplierColour[2] = (rand() % 11);
	//we need to divide these by 10 as a seperate step, otherwise for some reason it won't work
	//It appears that the rand function recognises the '/' operator and has other functions for it
	multiplierColour[0] = multiplierColour[0]/10; 
	multiplierColour[1] = multiplierColour[1]/10;
	multiplierColour[2] = multiplierColour[2]/10;

	//A value to hold the char array to be printed
	char number[3];
	
	//draw the text at whatever the required opacity
	if(multiplier >1)
	{
		//we ONLY update this number if the multiplier is NOT 0
		//otherwise it will revert to 0 and display as soon as the fade out begins
		//this means we will show where the multiplier peaked for a few seconds
		_itoa(multiplier, number, 10);
		printWithColour(multiplierFont,multiplierPos.x,multiplierPos.y,multiplierColour,number);
		printWithColour(multiplierXFont,(multiplierPos.x+(multiplierFont.h/2)),multiplierPos.y+8, multiplierXColour, "X");
		multiplierColour[3] = 1;//we want it visible, so set it's alpha to 1
		multiplierXColour[3] = 1;//same for the X
		multiplierTimer = 120;//reset the fadeout timer and multiplierfadeflag
		multiplierFadeFlag = false;
		multiplierPeek = multiplier;
	}else
	{//HOWEVER, if the multiplier IS ZERO, we display where the mulitplier previously peeked
		_itoa(multiplierPeek, number, 10);
		printWithColour(multiplierFont,multiplierPos.x,multiplierPos.y,multiplierColour,number);
		printWithColour(multiplierXFont,(multiplierPos.x+(multiplierFont.h/2)),multiplierPos.y+8, multiplierXColour, "X");
	
	}


	
	


	if(multiplier<=1)
	{
		if(multiplierTimer>0 && !multiplierFadeFlag)multiplierTimer--;//decrement the timer
		if(multiplierTimer<=0)multiplierFadeFlag = true;//if our timer has run out we let the program know its time to start fading
	}



	//Now if our flag is waving, we know it's time to fade
	if(multiplierFadeFlag)
	{
		multiplierColour[3] -= 0.05;//slowly fade the multiplier
		multiplierXColour[3] -= 0.05;
	}

	//and if we're completely faded, we can set our flag back to false, we no longer need to wave
	if((multiplierColour[3] <= 0) &&(multiplierColour[3] <= 0))
	{
		multiplierFadeFlag = false; 
	}

	

}

void HUD::drawTimer(int timeRemaining)
{
	//we don't want to draw anything if this goes below zero 
	//WRONG!! We want to draw zero if this goes below zero
	//we accomodate this is quite a long winded way (see else statement below)
	//we COOULD just make sure the timerDecrement function doesn't decrement below zero
	//But meh
	//(also due to the char* constraints, if it goes below -9, the game will crash)
	if(timeRemaining >=0 )
	{
		char timeAsChar[4];
		_itoa(timeRemaining, timeAsChar, 10);
		printWithColour(scoreTextFont, timerPos.x, timerPos.y, timerTextColour,"TIME REMAINING");
		printWithColour(multiplierFont, timerPos.x, timerPos.y - (multiplierFont.h +10), timeColour,timeAsChar); 
	}else{
	
		if(timeRemaining<0)
		{
			char timeAsChar[4];
		_itoa(0, timeAsChar, 10);
		printWithColour(scoreTextFont, timerPos.x, timerPos.y, timerTextColour,"TIME REMAINING");
		printWithColour(multiplierFont, timerPos.x, timerPos.y - (multiplierFont.h +10), timeColour,timeAsChar);
		}
	
	}

	
	
}

int HUD::numberOfZeros(float number)
{
	int zeros = 0;
	for(float i = number; i>=10; i=i/10)
	{
		zeros++;
		
	}
	return zeros;
}

int HUD::calculateScoreOffset(float number, int fontHeight)
{
	int offset = numberOfZeros(number);
	offset = offset * floor((float)fontHeight/2);
	return offset;
}

void HUD::pulsateColour(GLfloat *colour1, GLfloat *colour2, GLfloat *objectCol, ColourState& goingTo, int timeInFrames)
{
	GLfloat difference[4];
	//give us an array of values to change the array by in the alloted time
	difference[0] = sqrt(pow((colour1[0]-colour2[0])/timeInFrames,2));
	difference[1] = sqrt(pow((colour1[1]-colour2[1])/timeInFrames,2));
	difference[2] = sqrt(pow((colour1[2]-colour2[2])/timeInFrames,2));
	difference[3] = sqrt(pow((colour1[3]-colour2[3])/timeInFrames,2));

	//depending on the state, transform the array in one direction or another
	
	if(goingTo == ColourState::COLOUR2)//if we're moving toward colour 2
	{
		for(int i = 0; i<4; i++)
		{
			if(objectCol[i]<colour2[i])//if  we're lower than we need to be
			{
				objectCol[i] += difference[i];//add the difference till we get there
			}
			if(objectCol[i]>colour2[i])//if we're higher than we need to be
			{
				objectCol[i] -= difference[i];//subtract the difference till we get there
			}

			//we need to SNAP to the colour as we will not hit it every time using floats
			if( (objectCol[i]>=(colour2[i]-(difference[i]))) && (objectCol[i]<=(colour2[i]+(difference[i])))  )
			{//if we hit this tiny but huuuge target
				objectCol[i] = colour2[i];//SNAP
			}
		
		}
	}else{
		if(goingTo == ColourState::COLOUR1)
		{
			for(int i = 0; i<4; i++)
			{
				if(objectCol[i]<colour1[i])//if  we're lower than we need to be
				{
					objectCol[i] += difference[i];//add the difference till we get there
				}
				if(objectCol[i]>colour1[i])//if we're higher than we need to be
				{
					objectCol[i] -= difference[i];//subtract the difference till we get there
				}

				if( (objectCol[i]>=(colour1[i]-(difference[i]))) && (objectCol[i]<=(colour1[i]+(difference[i])))  )
				{//if we hit this tiny but huuuge target
					objectCol[i] = colour1[i];//SNAP
				}
				
			}
		}
	}

	if((objectCol[0] == colour1[0])&&(objectCol[1] == colour1[1])&&(objectCol[2] == colour1[2])&&(objectCol[3] == colour1[3]))
	{//if the objcolour == colour 1
		goingTo = ColourState::COLOUR2;//it's now time to move towards colour 2
	}else{
		if((objectCol[0] == colour2[0])&&(objectCol[1] == colour2[1])&&(objectCol[2] == colour2[2])&&(objectCol[3] == colour2[3]))
		{//if the objcolour == colour 2
			goingTo = ColourState::COLOUR1;//it's now time to move towards colour1	
		}
		
	}
}

void HUD::tween(point dest, point& pos, point rate)
{
	//let's make the rates positive no matter what
	rate.x = sqrt(pow(rate.x,2));
	rate.y = sqrt(pow(rate.y,2));

	//Snapping sometimes needs to occur, as miniscule values
	//are lost when rounding happens
	//Check if we need to SNAP this frame
	if( (pos.x >= (dest.x-(rate.x/2))) && (pos.x<= (dest.x+(rate.x/2))) )
	{//if we're less than HALF the move-rate away either side of the destination
		pos.x = (double)dest.x; //SNAP!
	}
	//Same for the Y value
	if( (pos.y >= (dest.y-(rate.y/2))) && (pos.y<= (dest.y+(rate.y/2))) )
	{//if we're less than HALF the move-rate away either side of the destination
		pos.y = (double)dest.y; //SNAP!
	}

	

	//Move our X vals
	if(pos.x > dest.x)//if we're to the right of it
	{
		pos.x -= rate.x;//go left
	}else{
		if(pos.x < dest.x)//if we're to the left of it
		{
			pos.x += rate.y;//go right
		}
	}
	//Move our Y vals
	if(pos.y > dest.y)//if we're above it
	{
		pos.y -= rate.y;//move it down
	}else{
		if(pos.y < dest.y)//if we're below it
		{
			pos.y += rate.y;
		}
	
	}

}

void HUD::colourTween(GLfloat startCol[4], GLfloat currentCol[4], GLfloat endCol[4], int numberOfFrames)
{
	GLfloat tweenBy[4];//holds the numbers we need to move RGB&A by each frame

	for(int i = 0; i<4; i++)
	{
		tweenBy[i] = sqrt(pow((endCol[i]-startCol[i])/numberOfFrames,2));// the difference between the two, made explicitly positive
	}
	//////////////////////
	//THAT GOOD SNAPPING TECHNOLOGY
	//////////////////////
	for(int i = 0; i<4; i++)
	{
		if(currentCol[i] >= endCol[i]-(tweenBy[i]/2) && currentCol[i] <= endCol[i]+(tweenBy[i]/2))
		{
			tweenBy[i] = endCol[i]; //SNAP
		}
	}

	for(int i = 0; i<4; i++)
	{
		if(currentCol[i] < endCol[i])
		{
			currentCol[i] += tweenBy[i];
		}

		if(currentCol[i] > endCol[i])
		{
			currentCol[i] -= tweenBy[i];
		}
	}
}

void HUD::skipGameOverTick()
{
	if(!skipGO_flag) skipGO_flag =! skipGO_flag;//if the skip flag is false, invert it to true
}

HUD::HUD(void)
{

	//The positions of the HUD text
	scorePos.x = 100; scorePos.y = 400;
	multiplierPos.x = 100; multiplierPos.y =  500;
	timerPos.x = 100; timerPos.y = 250;

	//initialise the multiplierfadeflag to FALSE
	multiplierFadeFlag = false;
	multiplierTimer = 120;
	multiplierPeek = 0;
	
	//game over score so that we can clock up the final score
	GO_score = 0;
	GO_scoreTimer = 60;
	//the text itself
	scoreText = "SCORE: ";
	//a bool to skip the flag with
	skipGO_flag = false;

	//initialise the fonts
	try
	{
	scoreTextFont.init("Visitor TT2 BRK Regular.ttf", 20);
	} catch (std::exception &e) {
		MessageBoxA(NULL, e.what(), "EXCEPTION CAUGHT", MB_OK | MB_ICONINFORMATION);
	
	}

	try
	{
	scoreFont.init("Visitor TT2 BRK Regular.ttf", 38);
	} catch (std::exception &e) {
		MessageBoxA(NULL, e.what(), "EXCEPTION CAUGHT", MB_OK | MB_ICONINFORMATION);
	
	}

	try
	{
	multiplierFont.init("Visitor TT2 BRK Regular.ttf", 80);
	} catch (std::exception &e) {
		MessageBoxA(NULL, e.what(), "EXCEPTION CAUGHT", MB_OK | MB_ICONINFORMATION);
	
	}

	try
	{
		multiplierXFont.init("Visitor TT2 BRK Regular.ttf", 20);
	} catch (std::exception &e) {
		MessageBoxA(NULL, e.what(), "EXCEPTION CAUGHT", MB_OK | MB_ICONINFORMATION);
	
	}

	try{
		pauseFont.init("Visitor TT2 BRK Regular.ttf", 50);
	} catch (std::exception &e) {
		MessageBoxA(NULL, e.what(), "EXCEPTION CAUGHT", MB_OK | MB_ICONINFORMATION);
	}

	try{
		GO_scoreFont.init("Visitor TT2 BRK Regular.ttf", 80);
	} catch (std::exception &e) {
		MessageBoxA(NULL, e.what(), "EXCEPTION CAUGHT", MB_OK | MB_ICONINFORMATION);
	}

	try{
		GO_retryFont.init("Visitor TT2 BRK Regular.ttf", 20);
	} catch (std::exception &e) {
		MessageBoxA(NULL, e.what(), "EXCEPTION CAUGHT", MB_OK | MB_ICONINFORMATION);
	}

	CS_GO_score =  ColourState::COLOUR2;

}


HUD::~HUD(void)
{
}

