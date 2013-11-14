/***************************************
/ This Class is responsible for our Gameplay
/ rules and scoring systems, along with endgame
/ conditions. 
/****************************************/
#include "GameFramework.h"
#include <iostream>;
#include <ctime>
#include <Windows.h>
using namespace std;

GameFramework::GameFramework()
{
	m_iTotalScore = 0;
	m_iPointValue = 0;
	m_bIsGameOver = false;
	m_iBlocksLeft = 5;
	m_iLife		  = 3;
	m_fTimer	  = 0.0f;

}
GameFramework::~GameFramework()
{

}

void GameFramework::SetTotalScore(int score)
{
	m_iTotalScore += score;
}

void GameFramework::SetScore(int points)
{
	m_iPointValue = points;
}

void GameFramework::SetLifeCount(int life)
{
	m_iLife = life;
}

void GameFramework::SetGameOver()
{
	if(m_bIsGameOver)
	{
		m_bIsGameOver = false;
	}
	if(!m_bIsGameOver)
	{
		m_bIsGameOver = true;
	}
}

void GameFramework::SetTimer(float time)
{
	m_fTimer = time;
}

int GameFramework::GetBlocksLeft()
{
	return m_iBlocksLeft;
}

int GameFramework::GetLifeCount()
{
	return m_iLife;
}

int GameFramework::GetTotalScore()
{
	return m_iTotalScore;
}

int GameFramework::GetPointValue()
{
	return m_iPointValue;
}

bool GameFramework::GetGameOver()
{
	return m_bIsGameOver;
}

float GameFramework::GetTimer()
{
	return m_fTimer;
}

void GameFramework::Update(float dt)
{
	//run the game, while the endgame condition is not met OR the user has not run out of lives
	if(!m_bIsGameOver)
	{
		//game will play
	}
	else if(m_bIsGameOver)
	{
		//stop game update and return to menu
	}
}