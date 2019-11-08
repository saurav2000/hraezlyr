#include "Move.h"
#ifndef CANNON_H
#define CANNON_H

class Cannon
{
	public:

	int x, int y, int dx, int dy;
	Cannon(int a, int b, int c, int d);
	Cannon(const Cannon &other);
	bool isPresent(int a, int b);
	void getMoves(std::bitset<100> *soldiers, std::bitset<100> *townhalls, int id, std::vector<Move*> &res);
};

#endif