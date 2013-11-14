#ifndef FMODSOUND_H
#define FMODSOUND_H
//headers to include
#include <fmod.hpp>
#include <iostream>
using namespace std;
// Macro to release COM objects fast and safely
#define SAFE_RELEASE(x) if(x){x->Release(); x = 0;}
class Sound 
{
public:
	////////////////////////////////////////////////////////////////////////
	// FMOD Sound Objects
	////////////////////////////////////////////////////////////////////////
	FMOD::Sound*		m_pLaserSound;		// Sound Object - Laser
	FMOD::Sound*		m_pHitSound;		// hit sound for objects
	FMOD::Sound*		m_pExplosionSound;	//Explosion Sound for death
	FMOD::Sound*		m_pFBGMStream;		// Stream Object- BGM 
	FMOD::Channel*		m_pFChannel;		// Channel Object
	FMOD::System*		m_pFSystem;			// System object
	bool m_bIsPlaying;
	///////////////////////////
	//Constructor/Destructors//
	///////////////////////////
	Sound();
	~Sound();
	///////////////////////////////
	//Name: Init
	//Return: void
	//Parameters: Void
	//Info: We initialize all our objects, sound files, and
	//begin to set them up for operation later.
	///////////////////////////////
	void Init();
	//////////////////////////////
	//Name: Update
	//Return: Void
	//Parameters: Void
	//Info: Our classes update class. For playing
	//music based upon the game state we are in
	//as well as other sound effects.
	//////////////////////////////
	void Update();
	///////////////////////////
	//Name: SetIsPlaying
	//Return: Void
	//Parameters: Void
	//Info: Calling this class will swap our bool between
	//True/False
	///////////////////////////
	void SetIsPlaying();
	//////////////////////////
	//Name: GetIsPlaying
	//Return: bool
	//Param: Void
	//Info: Return m_bIsPlaying
	///////////////////////////
	bool GetIsPlaying();

	void PlayLaser();

	void PlayHit();

	void PlayBGM();
	//Class Object
	//Menu MenuObj;
};
#endif FMODSOUND_H