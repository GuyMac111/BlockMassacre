#include "MenuButton.h"

GLfloat red [4] = {1.0,0.2,0.2,1.0};
GLfloat grey[4] = {0.5,0.5,0.5,0.5};
GLfloat blue[4] = {0.0,0.0,1.0,1.0};
GLfloat mb_white[4] = {1.0,1.0,1.0,1.0};

bool MenuButton::mouseIsOverButton(point mousePos)
{
	if((mousePos.x >= buttonEntity.pos.x) && (mousePos.x <= buttonEntity.pos.x+buttonEntity.size.x ))
	{
		if((mousePos.y >= buttonEntity.pos.y) && (mousePos.y <= buttonEntity.pos.y+buttonEntity.size.y ))
		{
			//standard rectangular search 
			return true;	
		}else{return false;}
	}else{return false;}
}

void MenuButton::doButtonCalculations(point mousePos)
{
	//if the mouse is over the button
	//ensure that the state is HOVER
	if(mouseIsOverButton(mousePos))
	{
		//before we set the sound to hover, check if it's already hovered.
		//if not we should play the hover sound
		if(state!=HOVER && state!=DEAD)
		{
			sManager->playHoverFX();
		}

		if(state!=DEAD)state = HOVER;//only if it's not DEAD
	}else{
		//if the mouse is not over the button OR the button isn't dead
		//ensure the state is UP
		if(state != DEAD)state = UP;
	}

	if(state==HOVER)
	{
		HUD::pulsateColour(red, blue, colour, colourState, 15);
	}

	if(state==UP)
	{//if the state is UP, we need to set the colour back to it's default
		setColour(red);
	}else{
		if(state == DEAD)
		{
			setColour(grey);
		}
	}

	
	

}

void MenuButton::drawButton(point mousePos)
{	//Draw button is also responsible for processing button state
	doButtonCalculations(mousePos);
	printWithColour(font, buttonEntity.pos.x, buttonEntity.pos.y, colour, text.c_str());
	
}

void MenuButton::drawButton(point atPoint, point mousePos)
{
	//make atPoint or buttons new position
	dStructs::copyPoint(atPoint, buttonEntity.pos);
	//NOW we draw the button
	drawButton(mousePos);
}

void MenuButton::setColour(GLfloat* newColour)
{
	colour[0] = newColour[0];
	colour[1] = newColour[1];
	colour[2] = newColour[2];
	colour[3] = newColour[3];
}

MenuButton::MenuButton(entity details, string buttonText, HoverFunction onHoverFunc, font_data button_font, bool buttonIsDead)
{
	//set the details
	dStructs::copyEntity(details, buttonEntity);
	text = buttonText;
	instanceHovFunc = onHoverFunc;
	//set the PROPER dimensions of the button according to the the button text and font
	buttonEntity.size.x = sqrt(pow(getStringWidth(button_font,0,0,text.c_str()),2));
	buttonEntity.size.y = button_font.h;

	//initial colour is black
	colour[0] = 0.0f;
	colour[1] = 0.0f;
	colour[2] = 0.0f;
	colour[3] = 1.0f;

	//make a copy of the button_font for the purpose of this object
	font = freetype::font_data(button_font);

	//initialise the buttonState
	state = UP;
	//initialise the goingto ColourState for pulsating
	colourState = HUD::COLOUR2;

	sManager.reset(new SoundManager());

	if(buttonIsDead)
	{
		state = DEAD;
	}
}


MenuButton::MenuButton(void)
{
}


MenuButton::~MenuButton(void)
{
}
