#pragma once

#include <irrKlang.h>
#include <memory>
#include <string>
#include <vector>

using namespace std;
using namespace irrklang;

class SoundManager
{
public:
	SoundManager(void);
	~SoundManager(void);

	void startBGM();
	void loudBGM();//used to make the BGM it's regular volume for gameplay
	void dimBGM();//used to make the BGM quiet for the game over screen
	void playHoverFX();
	void playConfirmFX();
	void playSwapFX();
	void playDropFX(int multiplier);
	void playBreakFX(int multiplier);

private:
	
	////////
	//static unique_ptr<ISoundEngine> sEngine; //the engine itself
	////////
	//
	//
	//static ISoundSource* hoverSound;
	//static ISoundSource* confirmSound;
	//static ISoundSource* mainBGM;

	////we only really need an ISound interface
	////for the BGM
	//static ISound* bgmInterface;

};

