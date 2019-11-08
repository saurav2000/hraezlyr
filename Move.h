#include <string>
#include <iostream>
#include <bitset>
#include "Global.h"
#ifndef MOVE_H
#define MOVE_H

class Move
{
	public:
	
	int x_i, y_i, x_f, y_f;
	bool bomb;
	Move(int x_i, int y_i, int x_f, int y_f, bool b);
	Move(int x_i, int y_i, int x_f, int y_f);
	std::string toString();
	bool isHorizontal();

};

#endif