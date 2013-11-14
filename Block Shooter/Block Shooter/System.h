#ifndef SYSTEM_H
#define SYSTEM_H
#include <iostream>
using namespace std;
class System
{
private:
	enum GAMESTATE
	{
		INTRO,
		MENU,
		OPTIONS,
		GAME,
		CREDITS,
		QUIT
	};
	int m_iState;
	//GAMESTATE eGameState;
public:
	//////////////////////////
	//Constructor/Destructor//
	//////////////////////////
	System();
	~System();
	//////////////////////////
	//Name: Update()
	//Return: Void
	//Parameters: float
	//Info: Our Update class will set up the FSM we are
	//in and then allow us to control the game's state based upon these FSM
	///////////////////////////
	void Update(float);

	//////////////////////////
	//Name: SetGameState
	//Return: Void
	//Parameters: Int
	//Info: Set our current FSM State
	///////////////////////////
	void SetGameState(int);

	//////////////////////////
	//Name: GetGameState
	//Return: int
	//Parameters: void
	//Info: This will return the current GameState
	///////////////////////////
	//GAMESTATE GetGameState();
	int GetGameState();
	//////////////////////////
	//Name: Init()
	//Return: Void
	//Parameters: Void
	//Info: This will begin initization of our class and its properties
	///////////////////////////
	void Init();
};
#endif SYSTEM_H