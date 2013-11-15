#include "titleScreen.h"

GLfloat title_White[4] = {1.0f,1.0f,1.0f,1.0f};
GLfloat invisible[4] = {1.0f,1.0f,1.0f,0.0f};

void titleScreen::drawTitleScreen(point mousePos)
{
	
	logoBlock->draw(logoBlockEnt.pos,logoBlockEnt.size);
	logoMassacre->draw(logoMassEnt.pos,logoMassEnt.size);
	HUD::tween(logoBlock_Dest, logoBlockEnt.pos, animBlockRate);
	HUD::tween(logoMass_Dest, logoMassEnt.pos, animMassRate);
	dStructs::copyPoint(mousePos, t_mousePos);//Every frame update the title screen mouse pos
	
	flashingFunctions();

	if(flash == DIMMING || flash == DONE)
	{
	//Draw the buttons
	playButton->drawButton(mousePos);
	hsButton->drawButton(mousePos);
	quitButton->drawButton(mousePos);
	}

	drawFlash();


	

}

void titleScreen::flashingFunctions()
{
	if(flash ==  IDLE && logoBlockEnt.pos.x == logoBlock_Dest.x && logoBlockEnt.pos.y == logoBlock_Dest.y)
	{//if our flashstate is idle and our logo is at it's destination
		//we'll only test for one of the logos as they both reach their destination
		//at exactly the same frame
		flash = APPEARING;//change the flash's state to APPEARING
	}

	if(flash == APPEARING)
	{//if the flash should be appearing 
		//make it appear
		HUD::colourTween(invisible,flashCol,title_White,4);//happens over 3 frames
		
		//then check
		if(flashCol[3] == 1.0f)
		{
			flash = DIMMING;//change the flash's state to DIMMING	
		}
	}

	if(flash == DIMMING)
	{//then the same as for the appearing state, but in reverse
		HUD::colourTween(title_White, flashCol, invisible, 20);

		//then check
		if(flashCol[3] == invisible[3])
		{
			flash = DONE;//change the flash's state to DIMMING
		}

	}
}

void titleScreen::drawFlash()
{
	glColor4fv(flashCol);
	glBegin(GL_QUADS);
	glVertex2f(0,0);
	glVertex2f(0,flashDimensions.y);
	glVertex2f(flashDimensions.x, flashDimensions.y);
	glVertex2f(flashDimensions.x, 0);
	glEnd();

	glColor4f(1.0f,1.0f,1.0f,1.0f);
}

titleScreen::titleScreen(point winSize)
{
	//get our workspace size
	windowSize.x = winSize.x;
	windowSize.y = winSize.y;

	//set the dimensions of the flashbox
	flashDimensions = point(winSize);//the same size as the screen

	//We'll start by initialising the textures we're going to use
	logoBlock.reset(new Texture(512,128,"Sprites/Block_Logo.png"));
	logoMassacre.reset(new Texture(512,64,"Sprites/Massacre_Logo.png"));

	//logo sizes
	logoBlockEnt.size.x = 512*(1.3); logoBlockEnt.size.y = 128*(1.3);
	logoMassEnt.size.x = 512; logoMassEnt.size.y = 64;


	//logo destinations
	logoBlock_Dest.x = floor(winSize.x/2);
	logoBlock_Dest.y = floor( winSize.y - (logoBlockEnt.size.y/2) - 5);

	logoMass_Dest.x = floor(logoBlock_Dest.x);
	logoMass_Dest.y = floor(logoBlock_Dest.y - logoBlockEnt.size.y/2 - 10);

	//initial positions
	logoBlockEnt.pos.x = winSize.x/2;
	logoBlockEnt.pos.y = winSize.y+logoBlockEnt.size.y;
	
	logoMassEnt.pos.x = winSize.x/2;
	logoMassEnt.pos.y = 0 - logoMassEnt.size.y;

	//the number of frames for the initial logo animations
	animBlockFrames = 60;
	animMassFrames = 60;

	animBlockRate.x = (logoBlockEnt.pos.x-logoBlock_Dest.x)/animBlockFrames;
	animBlockRate.y = (logoBlockEnt.pos.y-logoBlock_Dest.y)/animBlockFrames;

	animMassRate.x = (logoMassEnt.pos.x-logoMass_Dest.x)/animMassFrames;
	animMassRate.y = (logoMassEnt.pos.y-logoMass_Dest.y)/animMassFrames;

	t_mousePos.x = 0;
	t_mousePos.y = 0;

	//create the buttons
	//////////////////////
	//PLAY BUTTON
	//first initialise the font we'll use for the button
	try
	{
	buttonFont.init("Visitor TT2 BRK Regular.ttf", 40);
	} catch (std::exception &e) {
		MessageBoxA(NULL, e.what(), "EXCEPTION CAUGHT", MB_OK | MB_ICONINFORMATION);
	
	}
	//now create the entity data (pos+size)
	playButtonEnt = entity(winSize.x/2 - (getStringWidth(buttonFont,0,0,"PLAY")/2), (winSize.y/2-20),0,0);
	playButton.reset(new MenuButton(playButtonEnt,"PLAY", MenuButton::PULSATE,buttonFont));
	
	//HIGHSCORE BUTTON
	hsButtonEnt = entity(winSize.x/2 - (getStringWidth(buttonFont,0,0,"HIGH-SCORES")/2), (playButtonEnt.pos.y-(buttonFont.h*(1.5*1))),0,0);
	hsButton.reset(new MenuButton(hsButtonEnt, "HIGH-SCORES", MenuButton::PULSATE,buttonFont,true));

	//QUIT BUTTON
	quitButtonEnt = entity(winSize.x/2 - (getStringWidth(buttonFont,0,0,"QUIT")/2), (playButtonEnt.pos.y-(buttonFont.h*(1.5*2))),0,0);
	quitButton.reset(new MenuButton(quitButtonEnt,"QUIT", MenuButton::PULSATE, buttonFont));
	
	//set the flashBox's state
	flash = IDLE;
	//set the flashBox's original colour
	flashCol[0] = 1.0;
	flashCol[1] = 1.0;
	flashCol[2] = 1.0;
	flashCol[3] = 0.0;

}

titleScreen::titleScreen(void)
{
}

titleScreen::~titleScreen(void)
{
}
