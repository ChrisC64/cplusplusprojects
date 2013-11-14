#include <iostream>
#include "game.h"
#include <Windows.h>
#include <ctime>
using namespace std;

bool checkChoice(char choice);

int main()
{
	srand(time(NULL));
	char option;
	BlackJack Table;
	//Table.DealCards();
	do
	{
			Table.PlayGame();
		//Table.PlayerTurn();
		//Table.DealerTurn();
		do
		{
			cout << "\nWould you like to play again(y/n)? ";
			cin >> option;
			option = tolower(option); // Lowercase the string before checking
		}while(!checkChoice(option));
		Table.ClearGame();
	}while(option == 'y' || option == 'Y');
	return 0;
}

// This will insure our choice entered is y/n or Y/N only
bool checkChoice(char choice)
{
	switch (choice)
	{
	case 'y':
		return true;
		break;
	case 'n':
		return true;
		break;
	default:
		return false;
	}
}