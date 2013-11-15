#pragma once

#include "dStructs.h"
#include "freetype.h"
#include "HUD.h"
#include <stdio.h>
#include "SoundManager.h"

typedef dStructs::entity entity;
typedef dStructs::point point;
typedef HUD::ColourState ColourState;

using namespace std;
using namespace freetype;

class MenuButton
{

enum ButtonState{UP, HOVER, DOWN, DEAD};



public:
	enum HoverFunction{PULSATE}; //needs to be public to be used by external classes. 
	//As a HoverFunction is passed as an argument in the constructor, it IS needed externally

	void setColour(GLfloat* newColour);
	void drawButton(point mousePos);//Overloaded to either be drawn at the position it's initialized to
	void drawButton(point atPos, point mousePos);//or to accept a point that then becomes the buttons details.pos and calls the 
	bool mouseIsOverButton(point mousePos);
	
	


	MenuButton(entity details, string buttonText, HoverFunction onHoverFunc, font_data ft_font, bool buttonIsDead = false);
	MenuButton(void);
	~MenuButton(void);

	
	ButtonState state; //holds the state of the button
	entity buttonEntity; //will contain the pos and size of the button
	string text; //holds the text that the button will display

private:
	HoverFunction instanceHovFunc; //Describes the hover function to be performed
	GLfloat colour[4];
	font_data font;


	ColourState colourState;
	
	unique_ptr<SoundManager> sManager;
	void doButtonCalculations(point mousePos);


};

