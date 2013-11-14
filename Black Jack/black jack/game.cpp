#include <iostream>
#include <ctime>
#include <Windows.h>
#include <vector>
#include "game.h"
#include <math.h>
#include <iomanip>
using namespace std;

//////////////////////////////////////////////////////
//				Constructors						//
//////////////////////////////////////////////////////
BlackJack::BlackJack()
{
	win = 0;
	loss = 0;
	draw = 0;
	simLoss = 0.0;
	simDraw = 0.0;
	simWin = 0.0;
	MCPercentage = 0.0;
	bDealerTurn = false;
	bPlayerLose = false;
	PlayerCardValue = 0;
	DealerCardValue = 0;
	SimCardValue = 0;
	PlayerCardVec.resize(0);
	DealerCardVec.resize(0);
	MCVec = PlayerCardVec;
	simCount = 1000;
}

//////////////////////////////////////////////////////
//				GET Functions						//
//////////////////////////////////////////////////////
void BlackJack::getBDealerTurn()
{
	cout << bDealerTurn; 
}
void BlackJack::getCard(int player)
{
	if(player == 1)
	{
		cout << PlayerCard << " ";
	}
	else if(player == 2)
	{
		cout << DealerCard << " ";
	}
}
void BlackJack::getCardValue(int player)
{
	if(player = 1)
	{
		//player's cardValue
		cout << PlayerCardValue;
	}
	else if (player = 2)
	{
		//dealer's value
		cout << DealerCardValue;
	}
}
void BlackJack::getWin()
{
	cout << win;
}
void BlackJack::getLoss()
{
	cout << loss;
}
void BlackJack::getDraw()
{
	cout << draw;
}
void BlackJack::getMCPercentage()
{
	cout << MCPercentage;
}
void BlackJack::getMove()
{
	cout << move;
}
//////////////////////////////////////////////////////
//				SET Functions						//
//////////////////////////////////////////////////////
void BlackJack::setCard(int player, int value)
{
	if(player == 1)
	{
		PlayerCard = value;
	}
	else if(player == 2)
	{
		DealerCard = value;
	}
	else if(player == 3)
	{
		SimCard = value;
	}
}
//void BlackJack::setCardValue(int player, int value)
//{
//	if(player == 1)
//	{
//		PlayerCardValue = value;
//	}
//	else if(player == 2)
//	{
//		DealerCardValue = value;
//	}
//	else if(player == 3)
//	{
//		SimCardValue = value;
//	}
//}
void BlackJack::setVector(vector<int>&hand, int value)
{
	hand.push_back(value);
}
void BlackJack::setMove()
{
	
}
void BlackJack::setWin()
{
	win +=1;
}
void BlackJack::setLoss()
{
	loss+=1;
}
void BlackJack::setDraw()
{
	draw +=1;
}
void BlackJack::setValue(int player)
{
	if(player == 1)//player
	{
		int oneCount = 0;//used to check numbers of "1" values in vector
		PlayerCardValue = 0;
		//go through vector in hand
		for(unsigned int i = 0; i < PlayerCardVec.size(); i++)
		{
			if(PlayerCardVec.at(i) == 1)
			{
				oneCount++;
			}
			PlayerCardValue += PlayerCardVec.at(i);
		}
	}
	else if(player == 2)//dealer
	{
		int oneCount = 0;//used to check numbers of "1" values in vector
		DealerCardValue = 0;
		//go through vector in hand
		for(unsigned int i = 0; i < DealerCardVec.size(); i++)
		{
			if(DealerCardVec.at(i) == 1)
			{
				oneCount++;
			}
			DealerCardValue += DealerCardVec.at(i);
		}
	}
	else if(player == 3)//dealer
	{
		int oneCount = 0;//used to check numbers of "1" values in vector
		SimCardValue = 0;
		//go through vector in hand
		for(unsigned int i = 0; i < MCVec.size(); i++)
		{
			if(MCVec.at(i) == 1)
			{
				oneCount++;
			}
			SimCardValue += MCVec.at(i);
		}
	}
}
//////////////////////////////////////////////////////
//			Gameplay Functions						//
//////////////////////////////////////////////////////
/****************Player's Turn Function**************/
void BlackJack::PlayerTurn()
{
	do
	{
		//check values
		if(CheckValues(PlayerCardVec, PlayerCardValue) == true)//if player cardValue < 21, player may choose to hit or stand
		{
			//cout << "\nYour card total is: " << PlayerCardValue << endl;
			//DisplayTable();
			MonteSim(PlayerCardVec);
			//ask user to hit or stay
			MakeMove();
			if(bMove == true) 
			{
				//draw cards for player
				setCard(1,DrawCard());
				//store value of card into hand
				setVector(PlayerCardVec, PlayerCard);
				setValue(1);
				DisplayTable();
			}
			else if(bMove == false)
			{
				// Do nothing
			}
		}
		else if(CheckValues(PlayerCardVec,PlayerCardValue) == false)
		{
				move = 's'; 
				cout << "\nYou bust!";
				DisplayTable();
				//GameEnd();
				bPlayerLose = true;
				bMove = false;
		}
	}while(move == 'h'||move == 'H');
		//if player cardValue > 21, player losses and move = 's'
		
}
int BlackJack::DrawCard()
{
	int rNumMax = 10;
	int rNumMin = 1;
	int rNum = rand() % rNumMax + rNumMin;
	return rNum;
}
bool BlackJack::CheckValues(vector<int> &hand, int &playerValue)
{
	int oneCount = 0; // Used to check numbers of "1" values in vector
	int CardValue = 0; // Total value of cards in current hand

	//go through vector in hand
	for(unsigned int i = 0; i < hand.size(); i++)
	{
		if(hand.at(i) == 1)
		{
			oneCount++;
		}
		CardValue += hand.at(i);
	}
	playerValue = CardValue;

	// Keep count of 1's in hand
	if(playerValue <= 10 && oneCount <= 1)
	{
		playerValue = playerValue + 10; //add 10 points to current card value (A = 1 || 11)
	}
	// Compare current value and return true or false
	if(playerValue == 21)
	{
		//if card value == 21, end turn for player
		return true;
	}
	else if(playerValue <= 21)
	{
		//if card value is under 21 return true 
		return true;
	}
	else if(playerValue > 21)
	{
		//if value is greater than 21, return false
		return false;
	}
	
}
/****************************************************************************************
**							DEALERS TURN												*
****************************************************************************************/
void BlackJack::DealerTurn()
{
	bDealerTurn = true;
	do
	{
		// Check card value
		int oneCount = 0; // Used to check numbers of "1" values in vector
		DealerCardValue = 0;
		// Go through vector in hand
		for(unsigned int i = 0; i < DealerCardVec.size(); i++)
		{
			if(DealerCardVec.at(i) == 1)
			{
				oneCount++;
			}
			DealerCardValue += DealerCardVec.at(i);
		}
		if(DealerCardValue <= 10 && oneCount >= 1)
		{
			DealerCardValue = DealerCardValue + 10; // Add 10 points more if value is 10 or under
		}
		if(DealerCardValue <= 16)
		{
			// AI will hit
			setCard(2,DrawCard());
			setVector(DealerCardVec,DealerCard);
			setValue(2);
			DisplayTable();
		}
		else if(DealerCardValue >=17)
		{
			// AI will stand
			DisplayTable();
		}
	}while(DealerCardValue <= 16);
}
/********************************************************************************************/
/*                              END DEALER'S TURN                                           */
/********************************************************************************************/
void BlackJack::DealCards()
{
	//deal 2 cards to each player
	setCard(1,DrawCard());
	setVector(PlayerCardVec,PlayerCard);
	setCard(1,DrawCard());
	setVector(PlayerCardVec,PlayerCard);
	setValue(1);//set card value
	setCard(2,DrawCard());
	setVector(DealerCardVec,DealerCard);
	setCard(2,DrawCard());
	setVector(DealerCardVec,DealerCard);
	setValue(2);//set dealer's card value
}
bool BlackJack::GameEnd()
{
	if(PlayerCardValue > DealerCardValue && PlayerCardValue <=21)//player Wins if hand is under 21 AND greater than dealer's hand
	{
		setWin();
		cout <<"\nYou won! Congratulations!" << endl;
		cout << "Here is your results: " << endl;
		cout << "Win: " << win << " Loss: " << loss << " Draw: " << draw << endl;
		return true;
	}
	else if(PlayerCardValue < DealerCardValue && DealerCardValue <=21 || PlayerCardValue > 21 && DealerCardValue < 21)//dealer wins if hand is greater than player's hand AND lower than 21
	{
		setLoss();
		cout <<"\nYou loss! Sorry!" << endl;
		cout << "Here is your results: " << endl;
		cout << "Win: " << win << " Loss: " << loss << " Draw: " << draw << endl;
		return true;
	}
	else if(PlayerCardValue > 21 && DealerCardValue > 21 || PlayerCardValue == DealerCardValue || PlayerCardValue > 21 && DealerCardValue > 21)//player AND dealer surpassed 21 OR both have same values
	{
		setDraw();
		cout <<"\nDraw!" << endl;
		cout << "Here is your results: " << endl;
		cout << "Win: " << win << " Loss: " << loss << " Draw: " << draw << endl;
		return true;
	}
}
//Check the game if it's over or continuing
bool BlackJack::CheckGame()
{
	if(bMove == false)
	{
		if(PlayerCardValue > DealerCardValue && PlayerCardValue <=21) // Player Wins if hand is under 21 AND greater than dealer's hand
		{
			return true;
		}
		else if(PlayerCardValue < DealerCardValue && DealerCardValue <=21 || PlayerCardValue > 21 && DealerCardValue < 21)// Dealer wins if hand is greater than player's hand AND lower than 21
		{
			return true;
		}
		else if(PlayerCardValue > 21 && DealerCardValue > 21 || PlayerCardValue == DealerCardValue || PlayerCardValue > 21 && DealerCardValue > 21)// Player AND dealer surpassed 21 OR both have same values
		{
			return true;
		}
	}
	else if(bMove == true)
	{
		return false;
	}
}
//check for MC Simulation
bool BlackJack::SimCheck(int dCard)
{
	if(SimCardValue > dCard && SimCardValue <=21) // Player Wins if hand is under 21 AND greater than dealer's hand
	{
		setSimWin();
		return true;
	}
	else if(SimCardValue < dCard && DealerCardValue <=21 || SimCardValue > 21 && dCard < 21)//dealer wins if hand is greater than player's hand AND lower than 21
	{
		setSimLoss();
		return true;
	}
	else if(SimCardValue > 21 && dCard > 21 || SimCardValue == dCard || SimCardValue > 21 && dCard > 21)//player AND dealer surpassed 21 OR both have same values
	{
		setSimDraw();
		return true;
	}
}
/************************************************************************
*							Display Output								*
************************************************************************/
void BlackJack::DisplayTable()
{
	cout << "\n==========================BLACK JACK========================\n";
	DisplayHand(2);
	DisplayHand(1);
	cout << "\n============================================================\n";
}
void BlackJack::DisplayHand(int player)
{
	switch(player)
	{
	case 1: // Player's hand
		cout << "\nYour Hand: \t\t\t(Your Score: " << PlayerCardValue << " )" << endl;
		for(unsigned int i = 0; i < PlayerCardVec.size(); i++)
		{
			if(PlayerCardVec.at(i) == 10)
			{
				cout << " F ";	
			}
			else if(PlayerCardVec.at(i) == 1)
			{
				cout << " A ";
			}
			else if (PlayerCardVec.at(i) > 1 && PlayerCardVec.at(i) < 10)
			{
				cout << PlayerCardVec.at(i) << " ";
			}
		}
		break;
	case 2:
		// If the Dealer has not moved yet, hide the second card's value
		if(bDealerTurn == false)
		{
			cout << "\nDealer's Hand: \t\t\t(Dealer's Value: ??)\n";
		}
		else if(bDealerTurn == true)
		{
			cout << "\nDealer's Hand: \t\t\t(Dealer's Value: " << DealerCardValue << ")\n";
		}

		// Display the cards in the dealer's hands and their values
		if(bDealerTurn == false)
		{
			for(unsigned int i = 0; i < DealerCardVec.size()-1; i++)
			{
				cout << DealerCardVec.at(i) << " ";
			}
			cout << " # ";//display last card as hidden
		}
		else if (bDealerTurn == true)//If Dealer's turn has happened, then we can reveal the hidden card
		{
			for(unsigned int i = 0; i < DealerCardVec.size(); i++)
			{
				if(DealerCardVec.at(i) == 10)
				{
					cout << " F ";	
				}
				else if(DealerCardVec.at(i) == 1)
				{
					cout << " A ";
				}
				else if (DealerCardVec.at(i) > 1 && DealerCardVec.at(i) < 10)
				{
					cout << DealerCardVec.at(i) << " ";
				}
			}
		}
		break;
	default:
		break;

		
	}//End Dealer's turn
}
/******************************************REPLAY GAME*****************************************/
void BlackJack::PlayGame()
{
	DealCards();
	DisplayTable();
	do
	{
		PlayerTurn();
		if(bPlayerLose == false)
		{
			DealerTurn();
		}
	}while(CheckGame() == false);
	GameEnd();
	
}
bool BlackJack::MakeMove()
{
	cout << "\nWould you like to hit or stay?(h/s): " << endl;
	cin >> move;
	move = tolower(move);
	switch(move)
	{
	case 'h':
		return bMove = true;
		break;

	case 's':
		return bMove = false;
		break;
	default:
		return bMove = false;
		break;
	}
}
/***************************************CLEAR GAME FOR NEW GAME*****************************/
void BlackJack::ClearGame()
{
	DealerCardVec.clear();
	PlayerCardVec.clear();
	DealerCardValue = 0;
	PlayerCardValue = 0;
	bDealerTurn = false;
	bPlayerLose = false;
}
//////////////////////////////////////////////////////////////////////////////////////////////
//								MONTE CARLO SIM												//
//////////////////////////////////////////////////////////////////////////////////////////////
void BlackJack::MonteSim(vector<int>&playerHand)
{
	cout << "\n**************** SIMULATION PLAY START **********************" << endl;
	cout << "\nEstimating the chances you could still win if you draw, please wait...\n";
	// Take the player's hand and copy it to the simulation's vector
	MCVec = playerHand;
	int DCard;
	// Read the dealer's hand and assign it two values based on size
	// If size of Dealer's hand is == 2, then take only the front number
	if(DealerCardVec.size() == 2)
	{
		DCard = DealerCardVec.front();
	}
	//else take the DealerCardValue if > 2
	else if(DealerCardVec.size() > 2)
	{
		DCard = DealerCardValue;
	}
	cout << "\nCurrently playing: " << simCount << " games....\n";
	for(unsigned int i = 0; i < simCount; i++)
	{
		// Take hit, add to vector, and find total
		setCard(3, DrawCard());
		setVector(MCVec, SimCard);
		setValue(3);

		// Find the value of SimCardValue
		CheckValues(MCVec,SimCardValue);
		// Compare to dealer's hand, and find game win state
		// Save wins/loss/draws
		SimCheck(DCard);
		MCVec.pop_back();
	}
	cout << "Here are the results of " << simCount << " games: \n";
	cout << "Wins: " << simWin << " Losses: " << simLoss << " Draws: " << simDraw << endl;
	cout << "You're win percentage is: ";
	MCPercentage = (simWin/simCount) * 100.0; 
	cout  << "(" << setprecision(4) << setw(4) << fixed <<  MCPercentage << "%)" << endl;
	cout << "\n**************** SIMULATION PLAY END ************************" << endl;
	resetCounts();	
}
/////////////////////////SET S WINS/LOSS/DRAWS///////////////////////////
void BlackJack::setSimWin()
{
	simWin++;
}
void BlackJack::setSimLoss()
{
	simLoss++;
}
void BlackJack::setSimDraw()
{
	simDraw++;
}
void BlackJack::resetCounts()
{
	simWin = 0;
	simLoss = 0;
	simDraw = 0;
}