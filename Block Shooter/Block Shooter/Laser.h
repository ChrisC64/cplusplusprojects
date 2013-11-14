#ifndef LASER_H
#define LASER_H
#include <iostream>
#include "PlayerShip.h"
#include "AlienShip.h"
using namespace std;
class Laser : public PlayerShip, public AlienShip
{
private:

public:
	///////////////////////////
	//Constructor/Destructors//
	///////////////////////////
	Laser();
	~Laser();
};
#endif LASER_H