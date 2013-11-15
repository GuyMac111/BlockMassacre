#pragma once
#include "freetype.h"
#include "dStructs.h"
#include "HUD.h"

using namespace freetype;
typedef dStructs::point point;

class scorePopUp
{
enum PopUpState{ANIM_IN, HOLD, ANIM_OUT, COMPLETE};

public:
	void animatePopUp();
	void drawPopUp();
	bool isComplete(); //how we check if our popup is ready to be deleted
	point popupGridPos;
	scorePopUp(int score, point gridPos, point worldPos);
	scorePopUp(void);
	~scorePopUp(void);
private:

	void animInFunc();
	void animHoldFunc();
	void animOutFunc();

	font_data popUpFont;
	point leftScorePos;//the left half of the score
	point rightScorePos;//the right half of the score
	char scoreAsText[10];
	point textCentre;//The left-most point of the text
	
	int holdTimer;//once the numbers reach the textCenter, this begins counting down. 
	//Once it reaches zero, the numbers dissipate.
	point animRate;

	GLfloat popupColour[4]; //holds the colour of the popup 
	PopUpState state;

	point scoreUpDest;//the dest for one of the scores (we'll say the left score) after the hold period is done
	point scoreDownDest;
};

