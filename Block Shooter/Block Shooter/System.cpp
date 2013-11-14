#include "System.h"
#include <iostream>
using namespace std;

System::System()
{
	m_iState		= INTRO;
	
}
System::~System()
{

}

void System::Init()
{
	m_iState = INTRO;
}
void System::Update(float dt)
{

}
void System::SetGameState(int state)
{
	if(state == 0)
	{
		m_iState = 0;
	}
	if(state == 1)
	{
		m_iState = 1;
	}
	if(state == 2)
	{
		m_iState = 2;
	}
	if(state == 3)
	{
		m_iState = 3;
	}
	if(state == 4)
	{
		m_iState = 4;
	}
	if(state == 5)
	{
		m_iState = 5;
	}
}
int System::GetGameState()
{
	return m_iState;
}