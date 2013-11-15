#include "scorePopUp.h"

const int width = 30;
const int animDuration = 10;
const int holdDuration = 40; //holds for 60 frames
GLfloat SP_white[4] = {1.0,1.0,1.0,1.0};

void scorePopUp::drawPopUp()
{
	printWithColour(popUpFont, leftScorePos.x, leftScorePos.y, popupColour,scoreAsText);
	printWithColour(popUpFont, rightScorePos.x, rightScorePos.y, popupColour,scoreAsText);
}

void scorePopUp::animInFunc()
{
	if(leftScorePos.x != textCentre.x || rightScorePos.x != textCentre.x)//OR
		{
			//move them
			HUD::tween(textCentre, leftScorePos, animRate);
			HUD::tween(textCentre, rightScorePos, animRate);
		
			//and tween their colour
			if(popupColour[3] != SP_white[3])
			{
				float colourRate = 1.0f/animDuration;
				popupColour[3] += colourRate;
			}
					//WHERE YOU LEFT OFF:
			//YOU WERE CHECKING TO SEE WHY THE COLOUR ABOVE NEVER CHANGES IT'S ALPHA VALUE
		}
		else{//if they're both in place, switch state to HOLD
			state = HOLD;
		}
}

void scorePopUp::animHoldFunc()
{
	if(holdTimer>0)
	{
		holdTimer--;//decrement until we reach zero
	}else{//once we hit zero or less 
		state = ANIM_OUT;
	}
}

void scorePopUp::animOutFunc()
{
	if(leftScorePos.y != scoreUpDest.y || rightScorePos.y != scoreDownDest.y)
	{//while they aren't equal to the vertical destination, tween them
		HUD::tween(scoreUpDest,leftScorePos,animRate);
		HUD::tween(scoreDownDest,rightScorePos,animRate);

		//and tween their colour/alpha
		if(popupColour[3] > 0)
		{
			float colourRate = 1.0f/animDuration;
			popupColour[3] -= colourRate;
		}

	}else{//if we're done tweening, switch state to COMPLETE
		state = COMPLETE;	
	}


}

void scorePopUp::animatePopUp()
{
	switch(state)
	{
	case ANIM_IN:
		animInFunc();
		break;

	case HOLD:
		animHoldFunc();
		break;

	case ANIM_OUT:
		animOutFunc();

		
	}
}

bool scorePopUp::isComplete()
{
	if(state == COMPLETE)
	{
		return true;
	}else{
	return false;
	}
}

scorePopUp::scorePopUp(int score, point gridPos, point worldPos)
{
	////init the font
	try
	{
	popUpFont.init("Visitor TT2 BRK Regular.ttf", 20);
	} catch (std::exception &e) {
		MessageBoxA(NULL, e.what(), "EXCEPTION CAUGHT", MB_OK | MB_ICONINFORMATION);
	
	}

	//gridPos is now a grid position not a world position
	popupGridPos = point(gridPos);

	////make the score passed to the popup a char*
	_itoa(score, scoreAsText, 10);

	////now use this to work out the actual centerpoint of the text, converting the gridPos to a worldPos

	float offset = getStringWidth(popUpFont,0,0,scoreAsText)/2;
	textCentre = point(worldPos.x-offset, worldPos.y-(popUpFont.h/4));

	leftScorePos = point(textCentre.x - width, textCentre.y);
	rightScorePos = point(textCentre.x + width, textCentre.y);

	scoreUpDest = point(textCentre.x, textCentre.y+width/2);
	scoreDownDest = point(textCentre.x, textCentre.y-width/2);

	popupColour[0] = 1.0f;//the current colour
	popupColour[1] = 1.0f;
	popupColour[2] = 1.0f;
	popupColour[3] = 0.0f;

	animRate =point(width/animDuration, width/animDuration); 
	state = ANIM_IN;

	holdTimer = holdDuration;//set holdtimer by the const we have set in the header: currently 60 frames

}

scorePopUp::scorePopUp(void)
{
}


scorePopUp::~scorePopUp(void)
{
}
