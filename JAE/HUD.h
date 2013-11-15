#pragma once
#include "freetype.h"
#include "dStructs.h"
#include <stdio.h>
#include <cstdlib>
#include <math.h>
#include <windows.h>

using namespace freetype;
typedef dStructs::point point;

class HUD
{
public:
	HUD(void);
	~HUD(void);

	enum ColourState {COLOUR1, COLOUR2};

	void drawHUD(int score, int multiplier, int timeRemaining); //Draws the Hud Scene
	void drawGameOver(int score, int multiplier, int timeRemaining, int sizeX, int sizeY);//draws the hud, and then the game over screen on top
	static void tween(point destination, point& pos, point rate);//Tweens the position referenced towards the destination
	static void colourTween(GLfloat startCol[4], GLfloat currentCol[4], GLfloat endCol[4], int numberOfFrames);
	static void pulsateColour(GLfloat *colour1, GLfloat *colour2, GLfloat *objectCol, ColourState& goingTo, int timeInFrames);
	//Be Sensible when passing a rate. Too much will most-likely cause wierd-shit to happen
	
	
	void skipGameOverTick();//called within the main class for GAME_OVER state keyboard input


private:
	

	
	ColourState CS_GO_score;
	font_data scoreFont;
	font_data scoreTextFont;
	font_data multiplierFont;
	font_data multiplierXFont;
	font_data pauseFont;
	font_data GO_scoreFont;
	font_data GO_retryFont;

	
	
	point scorePos;
	point multiplierPos;
	point timerPos;

	static GLfloat transparent[];
	static GLfloat blue[];
	static GLfloat red[];
	static GLfloat white[];
	static GLfloat scoreTextColour[];
	static GLfloat scoreColour[];
	static GLfloat multiplierColour[];
	static GLfloat multiplierXColour[];
	static GLfloat timerTextColour[];
	static GLfloat timeColour[];
	static GLfloat GO_scoreColour[];

	const char* scoreText;

	void drawMulti(int multiplier);
	void drawTimer(int timeRemaining);

	///////////////
	///MULTI
	///////////////
	bool multiplierFadeFlag;//(init to FALSE)//flag enables a frame timer, which after a set ammount of frames
	//causes the multiplier to begin to fade out 
	int multiplierTimer; //a simple frame-based timer to ensure that the 
	//multiplier starts to fade in X ammount of frames
	int multiplierPeek;//records the highest point the multiplier has reached so far
	//updating every time the multiplie is above zero, It is THIS value that is drawn 
	//at fade-out time
	
	//////////////////
	///GAME OVER
	//////////////////
	bool skipGO_flag;
	float GO_score;
	int GO_scoreTimer;

	int numberOfZeros(float number);//calculates how many digits after the 1's column a number has
	int calculateScoreOffset(float number, int fontHeight);

	//pulsateColour takes 2 4f colour arrays and "bounces" the 4f colour array held in objectCol, between the two values 
	//MUST use 4d arrays
	//MUST a positive, non-Zero value for timeInFrames
	//failure to comply will result in unhandled exceptions on the scale of an ANCIENT CATASTROPHE
		
	//POSSIBLE ADDITION: add a "rest" so that it stays on 1 colour for a set ammount of frames

	//to do implement this




};

