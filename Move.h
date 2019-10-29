#include <string>
#include <iostream>
#include "Global.h"

class Move
{
	public:
	
	int x_i, y_i, x_f, y_f;
	bool bomb, cannon;
	Move(int x_i, int y_i, int x_f, int y_f, bool b, bool y);
	Move(int x_i, int y_i, int x_f, int y_f, bool y);
	Move(int x_i, int y_i, int x_f, int y_f);
	std::string toString();
	bool isHorizontal();

};