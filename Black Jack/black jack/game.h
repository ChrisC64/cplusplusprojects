#ifndef GAME_H
#define GAME_H
#include <vector>
using namespace std;
class BlackJack
{
private:
	// GAME VARIABLES
	int PlayerCard, DealerCard;
	int PlayerCardValue;
	int DealerCardValue;
	int win, loss, draw; 
	bool bDealerTurn, bPlayerLose;
	double MCPercentage;
	bool bMove;

	/*********************************/

	// SIMULATION VARIABLES
	vector<int> MCVec;
	int SimCardValue;			// Value of card total in simulation
	int SimCard;				// Simulation's card choice
	int simCount;				// Number of simulations to perform
	double simWin, simLoss, simDraw;	// Sim's count of wins, losses, and draws
	bool SimCheck(int);
	void setSimWin();
	void setSimLoss();
	void setSimDraw();
	void resetCounts();

	/*********************************/

	// COMMAND VARIABLES
	char move;				   // Hit or stand option for player
	vector<int>PlayerCardVec;  // Used to store player's cards
	vector<int>DealerCardVec;  // Used to store the dealer's cards
public:

	//CONSTRUCTORS
	BlackJack();
	/*get functions, allowing access to private functions*/
	void getCard(int);
	void getCardValue(int);
	void getWin();
	void getLoss();
	void getDraw();
	void getBDealerTurn();
	void getMCPercentage();
	void getCardVec();
	void getMove();
	/*set functions - set values of private variables*/
	void setCard(int,int);//paremeter: ((player = 1, dealer = 2), value of Card
	void setCardValue(int,int);//parameter: ((player = 1, dealer = 2), value of card)
	void setWin();
	void setLoss();
	void setDraw();
	void setMove();
	void setValue(int);
	void setDealerTurn(bool);
	void setMCPercentage(double);
	/*Set the Vector up*/
	void setVector(vector<int>&player, int); //push in a card value into the vector
	//////////////////////////////////////////////////////////////////////
	//					GAMEPLAY FUNCTIONS								//
	//////////////////////////////////////////////////////////////////////
	int DrawCard(); //Draws a random card value between 1-10
	bool CheckValues(vector<int>&hand, int&);//check the value of the card if over 21 or under 21; pass vector of hand, and 
	void PlayerTurn();
	void DealerTurn();
	void DealCards();//this is the start of the game, cards are dealt to both player and dealer
	bool GameEnd();//this will check the values of the cards, and return the winner
	bool CheckGame();
	bool MakeMove();//ask user to hit or stand
	//------------------------------------------------------------------//
	//					Display Outpout									//
	//------------------------------------------------------------------//
	void DisplayTable();
	void DisplayHand(int);
	//-----------------------------------------------------------------//
	//replay game function
	bool replay();
	void PlayGame();
	void ClearGame();
	/********************************************************************/
	//							MONTE CARLO SIM							//
	//////////////////////////////////////////////////////////////////////
	void MonteSim(vector<int>&);//the simulation for the user's monte carlo sim
};


#endif GAME_H