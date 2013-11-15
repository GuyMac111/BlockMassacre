#include "SoundManager.h"
#include <Windows.h>

//We may need multiple instances of the irrKlang engine. But we want to keep them static.
//So we're declaring them here

//There was quite some faf with the shared_ptr ownership and releasing. Long story short, 
//make sure you call grab() if you don't want your IRefCounted instances pulled out from under
//you on destruction

////WHERE YOU LEFT OFF
//Go have a second check on where you do the text for score popups, as we've increased the base-score 
//we may not have enough space in the char* for all the chars needed

static vector<shared_ptr<ISoundSource>>dropFX;
static vector<shared_ptr<ISoundSource>>breakFX;
static shared_ptr<ISoundSource> swapSound;
static shared_ptr<ISoundSource> hoverSound;
static shared_ptr<ISoundSource> confirmSound;
static shared_ptr<ISoundSource> mainBGM;
static shared_ptr<ISound> bgmInterface;
static shared_ptr<ISoundEngine> sEngine;


template<class T>
struct IrrDeleter
{
	void operator() (T* t)
	{
		t->drop();
	}
};

void SoundManager::startBGM()
{
	if(mainBGM)//if there's a main BGM
	{
		if(sEngine->isCurrentlyPlaying(mainBGM.get())==false)
		{
			bgmInterface.reset(sEngine->play2D(mainBGM.get(),true,false,true),IrrDeleter<ISound>());
		}
		bgmInterface->grab();
	}
}

void SoundManager::dimBGM()
{
	if(bgmInterface)
	{
		bgmInterface->setVolume(0.2f);
	}
}

void SoundManager::loudBGM()
{
	if(bgmInterface)
	{
		bgmInterface->setVolume(0.5f);
	}
}

void SoundManager::playHoverFX()
{
	if(hoverSound)
	{
		sEngine->play2D(hoverSound.get(),false,false,false,false);
	}

}

void SoundManager::playConfirmFX()
{
	if(confirmSound)
	{
		sEngine->play2D(confirmSound.get(),false,false,false,false);
	}
}

void SoundManager::playSwapFX()
{
	if(swapSound)
	{
		sEngine->play2D(swapSound.get(),false,false,false,false);
	}
}

void SoundManager::playDropFX(int multiplier)
{
	//This method also ensures that if multiplier is 0 by accident, that we don't attempt to access any sound.
	if(multiplier>0 && multiplier<11)
	{//if the multiplier is between the bounds of the sounds we have prepared
		if(!sEngine->isCurrentlyPlaying(dropFX.at(multiplier-1).get()))
		{//and if the relevant sound isnt already playing
			sEngine->play2D(dropFX.at(multiplier-1).get());//play the relevant sound
		}
	}
	else
	{//if not
		if(multiplier>10)
		{//if it's higher than we have anticipated
			if(!sEngine->isCurrentlyPlaying(dropFX.at(9).get()))
			{//and if it's not already playing the maximum sound
				sEngine->play2D(dropFX.at(9).get());//play the highest sound we have
			}
		}
	
	} 

}

void SoundManager::playBreakFX(int multiplier)
{
	//This method also ensures that if multiplier is 0 by accident, that we don't attempt to access any sound.
	if(multiplier>0 && multiplier<11)
	{//if the multiplier is between the bounds of the sounds we have prepared
		if(!sEngine->isCurrentlyPlaying(breakFX.at(multiplier-1).get()))
		{//and if the relevant sound isnt already playing
			sEngine->play2D(breakFX.at(multiplier-1).get());//play the relevant sound
		}
	}
	else
	{//if not
		if(multiplier>10)
		{//if it's higher than we have anticipated
			if(!sEngine->isCurrentlyPlaying(breakFX.at(9).get()))
			{//and if it's not already playing the maximum sound
				sEngine->play2D(breakFX.at(9).get());//play the highest sound we have
			}
		}
	
	} 

}

SoundManager::SoundManager(void)
{
	//first we need to create the irrKlang sound engine instance
	if(!sEngine)
	{
		sEngine.reset(createIrrKlangDevice(),IrrDeleter<ISoundEngine>());
	}
	sEngine->grab();
	
	if(!swapSound)swapSound.reset(sEngine->addSoundSourceFromFile("Sounds/swap.wav"),IrrDeleter<ISoundSource>());
	if(!hoverSound)hoverSound.reset(sEngine->addSoundSourceFromFile("Sounds/ButtonHover.mp3"),IrrDeleter<ISoundSource>());
	if(!confirmSound)confirmSound.reset(sEngine->addSoundSourceFromFile("Sounds/ButtonConfirm.mp3"),IrrDeleter<ISoundSource>());
	if(!mainBGM)mainBGM.reset(sEngine->addSoundSourceFromFile("Sounds/mainBGM.mp3"),IrrDeleter<ISoundSource>());
	

	//load in the drop sounds iteratively
	if(dropFX.empty())
	{
		for(int i = 0; i<10;i++)
		{//make the filename
			string filename = "Sounds/drop";
			char fileNo[4];
			_itoa(i+1,fileNo,10);
			filename.append(fileNo);
			filename.append(".wav");
			shared_ptr<ISoundSource> tempDropFXSource;
			tempDropFXSource.reset(sEngine->addSoundSourceFromFile(filename.c_str()),IrrDeleter<ISoundSource>());
			dropFX.push_back(tempDropFXSource);
			dropFX.at(i)->setDefaultVolume(0.5F);
			dropFX.at(i)->grab();
		}
	}

	//same as above for the break sounds
	if(breakFX.empty())
	{
		for(int i = 0; i<10;i++)
		{//make the filename
			string filename = "Sounds/break";
			char fileNo[4];
			_itoa(i,fileNo,10);
			filename.append(fileNo);
			filename.append(".wav");
			shared_ptr<ISoundSource> tempBreakFXSource;
			tempBreakFXSource.reset(sEngine->addSoundSourceFromFile(filename.c_str()),IrrDeleter<ISoundSource>());
			breakFX.push_back(tempBreakFXSource);
			breakFX.at(i)->setDefaultVolume(0.5F);
			breakFX.at(i)->grab();
		}
	}

	////////
	//Be sure to grab() all the sounds we create. This is like [NSObject retain]
	hoverSound->grab();
	confirmSound->grab();
	mainBGM->grab();
	swapSound->grab();

	//set some default volumes
	hoverSound->setDefaultVolume(1.0f);
	confirmSound->setDefaultVolume(0.4f);
	mainBGM->setDefaultVolume(0.5f);
	swapSound->setDefaultVolume(0.3f);


}


SoundManager::~SoundManager(void)
{	
}
