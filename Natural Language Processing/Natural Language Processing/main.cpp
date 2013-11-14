//Christopher Carlos
//Professor Geier
//GSP 480
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include "file reader.h"
using namespace std;

// Global Variables
char userChoice;
// Function Delcarations
bool checkChoice(char choice);
int main()
{
	cout << "Beginning Uniparse..." << endl;
	parseFile();
	cout << "Uniparse Complete!" << endl << "Now beginning Biparse...\n";
	biparseFile();
	cout << "BiParse Complete!" << endl << "Now beginning Triparse...\n";
	triparseFile();
	cout << "TriParse Complete!" << endl;

	//////////////////////////////////////////////////////////
	// Now we will take a sentence from the user			//
	// instruct the user to enter a sentence without space	//
	// convert the string into lowercase letters			//
	// send the string to the parseString()					//
	//////////////////////////////////////////////////////////
	do{
		string userString;
		cout << "Now I will try and parse a string you enter: ";
		cin >>userString;
		cout << "\nThank you, now I will begin parsing the string... please wait...\n";
		for(unsigned int i = 0; i < userString.size(); i++)
		{
			//lower string
			userString[i] = tolower(userString[i]);
		}
		parseString(userString);

		cout << endl << "Would you like to try another sentence? (y/n)" << endl;
		cin >> userChoice;
		userChoice = tolower(userChoice);
	}while(checkChoice(userChoice));
	return 0;
}

bool checkChoice(char choice)
{
	switch(userChoice)
	{
	case 'y':
		return true;
		break;
	case 'n':
		return false;
		break;
	default:
		return false;
		break;
	}
}