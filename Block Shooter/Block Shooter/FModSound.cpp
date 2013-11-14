#include "FModSound.h"
#include <iostream>
using namespace std;

Sound::Sound()
{

}
Sound::~Sound()
{

}

void Sound::Init()
{
	FMOD_RESULT result;
	/////////////////////////////////////
	// FMOD SOUND OBJECTS CREATED HERE //
	/////////////////////////////////////
	result = FMOD::System_Create(&m_pFSystem);
	if(result != FMOD_OK)
	{
		result = FMOD::System_Create(&m_pFSystem);
		//should have error message
	}
	result = m_pFSystem->init(1000, FMOD_INIT_NORMAL, 0);
	if(result != FMOD_OK)
	{
		result = m_pFSystem->init(100, FMOD_INIT_NORMAL, 0);
		//should have error message
	}
	/********************************************************************************************************/
	//CREATE SOUNDS
	//SOUND 1 - SWISH
	result = m_pFSystem->createSound("Content\\Sound\\shoot.wav",
		FMOD_DEFAULT, 0, &m_pLaserSound);
	if(result != FMOD_OK)
	{
		result = m_pFSystem->init(100, FMOD_INIT_NORMAL, 0);
		//should have error message
	}
	//SOUND 2 - CHORD
	result = m_pFSystem->createSound("Content\\Sound\\invaderkilled.wav",
		FMOD_DEFAULT, 0, &m_pHitSound);
	if(result != FMOD_OK)
	{
		result = m_pFSystem->init(100, FMOD_INIT_NORMAL, 0);
		//should have error message
	}
	//SOUND LOOP - DING
	result = m_pFSystem->createStream("Content\\Sound\\GT19.mp3",
		FMOD_DEFAULT, 0, &m_pFBGMStream);
	if(result != FMOD_OK)
	{
		result = m_pFSystem->init(100, FMOD_INIT_NORMAL, 0);
		//should have error message
	}
	//*************************************************************************
}

void Sound::Update()
{
	
}
void Sound::SetIsPlaying()
{
	if(m_bIsPlaying == true)
	{
		m_bIsPlaying = false;
	}
	if(m_bIsPlaying == false)
	{
		m_bIsPlaying = true;
	}
}
bool Sound::GetIsPlaying()
{
	return m_bIsPlaying;
}

void Sound::PlayBGM()
{
	FMOD_RESULT result;
	result = m_pFSystem->playSound(FMOD_CHANNEL_FREE, m_pFBGMStream, false, 0);
	if(result != FMOD_OK)
	{
		printf("Couldn't play sound", result);
	}
}

void Sound::PlayLaser()
{
	FMOD_RESULT result;
	result = m_pFSystem->playSound(FMOD_CHANNEL_FREE, m_pLaserSound, false, 0);
	if(result != FMOD_OK)
	{
		printf("Couldn't play sound", result);
	}
}

void Sound::PlayHit()
{
	FMOD_RESULT result;
	result = m_pFSystem->playSound(FMOD_CHANNEL_FREE, m_pHitSound, false, 0);
	if(result != FMOD_OK)
	{
		printf("Couldn't play sound", result);
	}
}