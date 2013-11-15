//gl//////////////////////////////////////////////////////
//
// Guy McLean - Gem Miners
//
////////////////////////////////////////////////////////
#include <stdio.h>
#include <GL/glut.h> 
#include <math.h>
#include "Block.h"
#include "dStructs.h"
#include "Grid.h"
#include "Texture.h"
#include "freetype.h"
#include <Windows.h>
#include "HUD.h"
#include "titleScreen.h"
#include "SoundManager.h"


//////////////////////////////////////////////////////
///Declare a couple of textures - for the background
//////////////////////////////////////////
unique_ptr<Texture> bg;
unique_ptr<Texture> bg2;

unique_ptr<Grid> theGrid;
unique_ptr<HUD> theHud;
unique_ptr<titleScreen> menu;
unique_ptr<SoundManager> sManager;
/////////////////////////////////////////////////
///Declare our fonts
/////////////////////////////////////////////////
freetype::font_data pauseFont;
freetype::font_data fpsFont;
/////////////////////////////////////////////////////////
//Initialize the variables
///////////////////////////////////////////////////////
typedef dStructs::point point;
const int XSize = 755, YSize = 600;
int timeRemaining;


point offset = point(333,145);
point mousePos = point(0,0);
point bgLoc = point(XSize/2,YSize/2);
point bgSize = point(XSize,YSize);

GLfloat white[] = {1.0f,1.0f,1.0f,1.0f};

//////////////////////////////////////
//FPS Variables
//////////////////////////////////////
int framesThisSecond;
const int maxFPS = 60;
GLfloat fps;

clock_t cycleTime;


/////////////////////////////////////
//Holds the gameplaystate
////////////////////////////////////
enum State {MENU, GAMEPLAY, PAUSED, GAME_OVER};
State GameState;
enum FlipSwitch {ON, OFF}; 
FlipSwitch Debug_Mode;
//////////////////
//FPS
//////////////////
void startFPSLimiting()
{
	cycleTime = clock();
}

void endFPSLimiting(int limit)
{
	//The time elapsed (in processor cycles since startFPSLimiting was called)
	cycleTime = clock()-cycleTime;
	//divide that by ammount of times YOUR clock ticks per second to get how many seconds the calls between
	//startFPSLimiting and endFPSLimiting 
	float operationTime = 1000 * ((float)cycleTime/(float)CLOCKS_PER_SEC);
	//reset the cycleTime var, (just to be fresh)
	cycleTime = 0;
	//How much time should the operations take according to the fps (in milliseconds)
	int targetTime = floor((1000/limit)+0.5); 
	//time left to fill this cycle
	int excessTime = targetTime - operationTime;
	//if the excessTime is greater than zero, that means we need to sleep for a while before drawing that shizz again
	if(excessTime>0)
	{
		Sleep(excessTime);
	}
}

void fpsManagement(int value)
{
	fps =  framesThisSecond;
	framesThisSecond = 0;
	glutTimerFunc(1000, fpsManagement,value);
}

void timerDecrement(int value)
{
	//Will only count down' if the game is paused
	//Technically, the player possibly earns an extra second if they pause JUST
	//before this happens
	//But for our purposes that's not too important
	if(GameState == GAMEPLAY)
	{
		if(theGrid->initialBreakIsDone())
		{//only if our initial break is done
			timeRemaining--; //decrement the timer
		}
		if(timeRemaining < 0)//1 second after zero on the clock
		{
			if(theGrid->gridState==Grid::STATIC)
			{
				GameState = GAME_OVER;
				timeRemaining = 0;//Make timeRemaining = zero so that the timed out clock
				//still displays	
				sManager->dimBGM();

			}
			
		}
	}
	if((GameState!=GAME_OVER) && (GameState!=MENU))
	{
		//if the game is in the menu or Timed-out, let the timer function die! 
		//So that it can safely be recalled again.
		glutTimerFunc(1000, timerDecrement, value); 
	}
}

////////////////
//INIT FUNCTIONS
////////////////
void initGameplay(void)
{
	GameState = GAMEPLAY;
	//printf("\n......Hello Guy. \n....\nInitilising");
	glMatrixMode(GL_PROJECTION);	
	glLoadIdentity();
	gluOrtho2D(0,XSize,0,YSize);
	timeRemaining = 30;//initialise the timer to 31 seconds
	Debug_Mode = ON;

	//Initialise the framesThisSecond var to 0 
	framesThisSecond = 0;
	fps = 0;//& fps




	
	///////////////////////////////////////////////////////////////
	///bg new MEMORY MANAGED EDITION
	//////////////////////////////////////////////////////////////////
	bg.reset(new Texture(1024,1024,"Sprites/BackGround.png"));
	//bg2.reset(new Texture(1024,1024,"Sprites/BackGround2.png"));
	theGrid.reset(new Grid(offset));
	theHud.reset(new HUD());
	/////////////////////////////////////////////////
	/// Old bad-memory-management style of pointed objects
	/////////////////////////////////////////////////
	//bg = new Texture(1024,1024,"Sprites/BackGround.png");
	//bg2 = new Texture(1024,1024,"Sprites/BackGround2.png");
	//theGrid = new Grid(offset);
	
	glClearColor(0.0,0.0,0.5,1.0);
	
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);//activate the alpha blending functionality
	glEnable(GL_BLEND);
	glLineWidth(2);			// Width of the drawing line
	glMatrixMode(GL_MODELVIEW);	
	glDisable(GL_DEPTH_TEST);
	
	//init the font for the pause screen
	try{
		pauseFont.init("Visitor TT2 BRK Regular.ttf", 50);
	} catch (std::exception &e) {
		MessageBoxA(NULL, e.what(), "EXCEPTION CAUGHT", MB_OK | MB_ICONINFORMATION);
	}

	//init the fps font
	try{
		fpsFont.init("Visitor TT2 BRK Regular.ttf", 20);
	} catch (std::exception &e) {
		MessageBoxA(NULL, e.what(), "EXCEPTION CAUGHT", MB_OK | MB_ICONINFORMATION);
	}

	//set the timerFunc in motion
	timerDecrement(1);
	sManager->loudBGM();
	

}

void initTitle()
{
	GameState = MENU;
	glMatrixMode(GL_PROJECTION);	
	glLoadIdentity();
	gluOrtho2D(0,XSize,0,YSize);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);//activate the alpha blending functionality
	glEnable(GL_BLEND);
	glLineWidth(2);			// Width of the drawing line
	glMatrixMode(GL_MODELVIEW);	
	glDisable(GL_DEPTH_TEST);

	glClearColor(0.7,0.7,1.0,1.0);

	point screenSize;
	screenSize.x = XSize; screenSize.y= YSize;
	menu.reset(new titleScreen(screenSize));//create our new titlescreen
	
	sManager->startBGM();
	sManager->dimBGM();

}

void startGame()//starts and initialises a new game of Block Massacre
{
	if(menu)//if there's a menu in existance
	{
		menu.reset(NULL);//delete it
	}

	if(theHud)//if there's a HUD in existance
	{
		theHud.reset(NULL);//delete it
	}

	if(theGrid)//if there's a gemgrid in existance
	{
		theGrid.reset(NULL);//delete it
	}

	initGameplay();//initialise all we need for gameplay. If problems emerge, 
	//look to the fonts and how we can deallocate the memory to those befor we do this


}

void goToMenu()//starts and initialises a new game of Block Massacre
{
	if(menu)//if there's a menu in existance
	{
		menu.reset(NULL);//delete it
	}

	if(theHud)//if there's a HUD in existance
	{
		theHud.reset(NULL);//delete it
	}

	if(theGrid)//if there's a gemgrid in existance
	{
		theGrid.reset(NULL);//delete it
	}

	initTitle();//initialise all we need for the menu screen.


}

/////////////////
//INPUT FUNCTIONS
/////////////////

void myGamePlayUpKeyFunc(unsigned char key, int x, int y)
{
	switch(key)
	{
	case'p':
		if(GameState == GAMEPLAY)
		{
			GameState = PAUSED;
			//dim the sound when we pause
			sManager->dimBGM();
			sManager->playConfirmFX();
		}
		break;
	case'P':
		if(GameState == GAMEPLAY)
		{
			GameState = PAUSED;
			sManager->dimBGM();
			sManager->playConfirmFX();
		}
		break;
	}
}

void myPausedUpKeyFunc(unsigned char key, int x, int y)
{
	switch(key)
	{
	case'p':
		if(GameState == PAUSED)
		{
			GameState = GAMEPLAY;
			sManager->loudBGM();
			//make the BGM its gameplay volume when we unpause
			sManager->playConfirmFX();
		}
		break;
	case'P':
		if(GameState == PAUSED)
		{
			GameState = GAMEPLAY;
			sManager->loudBGM();
			//make the BGM its gameplay volume when we unpause
			sManager->playConfirmFX();
		}
		break;
	}
}

void myGameOverUpKeyFunc(unsigned char key, int x, int y)
{
	switch(key)
	{
	case 32://space ascii code= 32
		theHud->skipGameOverTick();
		break;

	case 'r':
		startGame();//doubles as a restart function 
		//as it kills off all old instances
		break;
	case 'R':
		startGame();
		break;

	case 'q':
		goToMenu();
		break;

	case 'Q':
		goToMenu();
		break;
	}

}

void myUpKey(unsigned char key, int x, int y)
{

	switch(GameState) 
	{
	case GAMEPLAY:
		myGamePlayUpKeyFunc(key,x,y);
		break;

	case PAUSED:
		myPausedUpKeyFunc(key,x,y);
		break;

	case GAME_OVER:
		myGameOverUpKeyFunc(key,x,y);
		break;


	}

}

void gamePlayMouseFunc(int button, int state, int x, int y)
{
	switch(state)
	{
	case GLUT_UP:
		switch(button)
		{
		case GLUT_LEFT_BUTTON:
			//printf("Mouse up at point (%i,%i)",x,y);

			//if you can highlight or toggle while the grid is cascading,
			//Crashes Happen!!! So check first!
			if(theGrid->gridState!=Grid::CASCADING)
			{
				theGrid->toggleSelected(mousePos);
			}
			break;


		case GLUT_RIGHT_BUTTON:
			if(theGrid->gridState!=Grid::CASCADING)
			{
				theGrid->unselect();
			}
			break;

		}
		break;
	}
}

void titleMenuMouseFunc(int button, int state, int x, int y)
{
	switch(state)
	{
	case GLUT_UP:
		switch(button)
		{
			case GLUT_LEFT_BUTTON:
				if(menu->quitButton->mouseIsOverButton(point(x,y)))
				{
					exit(0);
				}

				if(menu->playButton->mouseIsOverButton(point(x,y)))
				{
					sManager->playConfirmFX();
					startGame();
				}
			
				break;
		}
		break;
	}
}

void mymouse(int button, int state, int x, int y)
{
	//The mouse is now handled by seperate functions. In order to create seperation between the
	//different states of play
	/*This function will essentially be a switch that filters through to the correct function for the current State enum variable
	In exactly the same way as the display function now works.*/

	//stupid OGL coordinate system, needs flipping
	y = YSize - y;

	switch(GameState)
	{
		case GAMEPLAY:
			//so for example in here we call the function we want performed when the mouse is clicked during Gameplay
			gamePlayMouseFunc(button, state, x,y);
			break;

		case MENU:
			titleMenuMouseFunc(button, state, x,y);
			break;

	}

	
}

void myPassiveMouse(int x, int y)
{
	//Stupid OGL coordinate system
	y = YSize - y;
	mousePos.x = x;
	mousePos.y = y;
	//printf("\nthe mouse coordinates are (%f,%f)",mousePos.x, mousePos.y);
}

///////////////////
//Display Functions
///////////////////

void menuDisplayFunc()
{
	menu->drawTitleScreen(mousePos);
}

void gameOverDisplayFunc()
{
	////////////
	//Mainly ripped straight from the pausedDisplayFunc
	//Gets displayed when the state is recognised as GAME_OVER
	/////////////

	theGrid->drawGrid();
	theGrid->drawPopups();
	bg->draw(bgLoc,bgSize);
	theHud->drawGameOver(theGrid->getScore(), theGrid->getMultiplier(), timeRemaining,XSize, YSize);

	
}

void gamePlayDisplayFunc()
{
	//Called each frame when the state is recognised as GAMEPLAY

	theGrid->doGridCalculations();//MODEL
	theGrid->animatePopups();//the popupslist version of dogridcalculations
	//bg2->draw(bgLoc,bgSize);//DISPLAY
	theGrid->drawPopups();
	theGrid->drawGrid();
	bg->draw(bgLoc,bgSize);
	theHud->drawHUD(theGrid->getScore(), theGrid->getMultiplier(), timeRemaining);

	if(theGrid->gridState == Grid::STATIC)
	{
		theGrid->hoverOverBlocks(mousePos);//CONTROLLER
	}
}

void pausedDisplayFunc()
{
	//Called each frame when the state is recognised as PAUSED
	//Displays the game. Does not update by doGridCalculations and
	//does not check for hovering of blocks

	//bg2->draw(bgLoc,bgSize);//DISPLAY
	theGrid->drawGrid();
	theGrid->drawPopups();
	bg->draw(bgLoc,bgSize);
	theHud->drawHUD(theGrid->getScore(), theGrid->getMultiplier(), timeRemaining);

	//Draw a transparent grey quad over the screen to dim for pausing
	//Set colour
	glColor4f(0.4f,0.4f,0.4f,0.5f);
	glBegin(GL_QUADS);
	glVertex2f(0,0);
	glVertex2f(0,YSize);
	glVertex2f(XSize, YSize);
	glVertex2f(XSize, 0);
	glEnd();
	//set the colour back to normal
	glColor4f(1.0f,1.0f,1.0f,1.0f);
	//write "PAUSED"
	printWithColour(pauseFont, (XSize/2)-(pauseFont.h*2),YSize/2,white,"PAUSED");


}

void displayFPS()
{
	//convert the fps to a string
	char fpsAsChar[10];
	sprintf(fpsAsChar,"%.2f", fps);
	printWithColour(fpsFont, (XSize-(10*fpsFont.h)),YSize-(fpsFont.h+5), white,"FPS: ");
	printWithColour(fpsFont, (XSize-(6*fpsFont.h)),YSize-(fpsFont.h+5), white,fpsAsChar);
}

//////////////////////////////////////////////////////
void display()
{
	//start the cycle timerfor the FPS
	startFPSLimiting();

	////printf("\nBeginning Display");
	glClear(GL_COLOR_BUFFER_BIT);//clear the colour buffer

	glPushMatrix();
	
	switch(GameState)//Performs a different display func dependant on state
	{
	case GAMEPLAY:
		gamePlayDisplayFunc();
		break;

	case PAUSED:
		pausedDisplayFunc();
		break;

	case GAME_OVER:
		gameOverDisplayFunc();
		break;

	case MENU:
		menuDisplayFunc();
		break;
	}
	 
	if(Debug_Mode==ON)
	{
		displayFPS();
	}

	glPopMatrix();

	glFlush();	// Finish the drawing
	glutSwapBuffers();
 	////printf("\nFresh Display Loaded");

	framesThisSecond++;//This frame is done, framesThisSecond+1
	endFPSLimiting(60);
	glutPostRedisplay();
}

int main(int argc, char** argv) 
{
	glutInit(&argc, argv);	// GLUT Initialization 
	glutInitDisplayMode(GLUT_RGBA|GLUT_DOUBLE); // Initializing the Display mode
	glutInitWindowSize(755,600);	// Define the window size
	glutCreateWindow("Block Massacre!");	// Create the window, with caption.
	
	//GameState enum, is initiated within the respective initialisers
	sManager.reset(new SoundManager());
	initGameplay();	// All Gameplay initialization
	initTitle(); // Menu initialisation
	//set the fpsManager in motion
	
	fpsManagement(1);
	//-- Callback functions ---------------------
	glutDisplayFunc(display);
	glutMouseFunc(mymouse);
	glutPassiveMotionFunc(myPassiveMouse);
	glutKeyboardUpFunc(myUpKey);
	glutMainLoop();	// Loop waiting for event 
}
