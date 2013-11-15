#pragma once
#include "freetype.h"
#include "dStructs.h"
#include "Texture.h"
#include "HUD.h"
#include "MenuButton.h"

typedef dStructs::point point;
typedef dStructs::entity entity;

class titleScreen
{

private:
	enum FlashState{IDLE,APPEARING,DIMMING,DONE};
	FlashState flash;
	
	point windowSize; //the window Size
	point t_mousePos; //the stored position of the mouse 
	
	point flashDimensions; //the white flash that happens onscreen when the options appear
	GLfloat flashCol[4];
	void drawFlash();
	void flashingFunctions();
	

	point logoBlock_Dest; //the resting destination of the 'block' part of the title
	point logoMass_Dest; //same for the 'massacre' part

	entity logoBlockEnt;
	entity logoMassEnt;

	unique_ptr<Texture> logoBlock; //the texture of the block text
	unique_ptr<Texture> logoMassacre; // same for massacre

	////////////
	//for the logo animations
	int animBlockFrames;
	int animMassFrames;

	point animBlockRate;
	point animMassRate;

	/////////////////
	//THE BUTTON INFO
	////////////////
	//the font for the titcle screen buttons
	font_data buttonFont;
	
	//button entities, carry the pos and size of the button
	entity playButtonEnt;
	entity hsButtonEnt;
	entity quitButtonEnt;
	


public:
	void drawTitleScreen(point mousePos);

	unique_ptr<MenuButton> playButton;//button objects need to be public so that 
						  //the main class can access them
	unique_ptr<MenuButton> quitButton;
	unique_ptr<MenuButton> hsButton;

	titleScreen(point winSize);
	titleScreen(void);
	~titleScreen(void);
};

