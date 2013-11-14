#ifndef GAMEFRAMEWORK_H
#define GAMEFRAMEWORK_H
#include <iostream>
#include "Menu.h"
using namespace std;
class GameFramework : public Menu
{
private:
	unsigned int m_iTotalScore;
	unsigned int m_iBlocksLeft;
	bool m_bIsGameOver;
	int	m_iPointValue;
	unsigned int m_iLife;
	float	m_fTimer;
public:
	///////////////////////////
	//Constructor/Destructors//
	///////////////////////////
	GameFramework();
	~GameFramework();

	/***************************
	/Name: SetTotalScore
	/Return: void
	/Param: int
	/Info: This will be setting our 
	/total score member.
	/****************************/
	void SetTotalScore(int);

	/***************************
	/Name: SetScore
	/Return: void
	/Param: int
	/Info: This will be setting our 
	/m_iPointValue member.
	/****************************/
	void SetScore(int);

	/***************************
	/Name: SetGameOver
	/Return: void
	/Param: void
	/Info: This will be setting our 
	/m_bIsGmeOver Bool member.
	/****************************/
	void SetGameOver();

	/***************************
	/Name: SetLifeCount
	/Return: void
	/Param: int
	/Info: This will be the user's
	/number of lives
	/****************************/
	void SetLifeCount(int);

	/***************************
	/Name: SetTimer
	/Return: void
	/Param: float
	/Info: This will be setting our 
	/timer member.
	/****************************/
	void SetTimer(float);

	/***************************
	/Name: GetTotalScore
	/Return: int
	/Param: void
	/Info: This will be getting our 
	/total score member.
	/****************************/
	int GetTotalScore();

	/***************************
	/Name: GetPointValue
	/Return: int
	/Param: void
	/Info: This will be getting our 
	/m_iPointValue member.
	/****************************/
	int GetPointValue();

	/***************************
	/Name: GetBlocksLeft
	/Return: int
	/Param: void
	/Info: This will be getting our 
	/blocks left member.
	/****************************/
	int GetBlocksLeft();

	/***************************
	/Name: GetGameOver
	/Return: bool
	/Param: void
	/Info: This will be getting our 
	/bool GameOver member.
	/****************************/
	bool GetGameOver();

	/*****************************
	/Name: GetLifeCount
	/Return: int
	/Param: void
	/Info: This will return our life count
	/******************************/
	int GetLifeCount();

	/***************************
	/Name: GetTimer
	/Return: float
	/Param: void
	/Info: This will be getting our 
	/timer member.
	/****************************/
	float GetTimer();

	/***************************
	/Name: Update
	/Return: void
	/Param: dt
	/Info: This update will manage the operations 
	/of our game conditions. After a set time has passed
	/we will send out an alien ship, and we will always be
	/checking the conditions of our game, seeing if any endgame
	/condition is met.
	/****************************/
	void Update(float);
};
#endif GAMEFRAMEWORK_H