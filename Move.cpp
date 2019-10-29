#include "Move.h"
using namespace std;

Move::Move(int x_i, int y_i, int x_f, int y_f, bool b, bool y)
{
	this->x_i = x_i;
	this->x_f = x_f;
	this->y_i = y_i;
	this->y_f = y_f;
	bomb = b;
	cannon = y;
}

Move::Move(int x_i, int y_i, int x_f, int y_f, bool y)
{
	this->x_i = x_i;
	this->x_f = x_f;
	this->y_i = y_i;
	this->y_f = y_f;
	bomb = false;
	cannon = y;
}

Move::Move(int x_i, int y_i, int x_f, int y_f)
{
	this->x_i = x_i;
	this->x_f = x_f;
	this->y_i = y_i;
	this->y_f = y_f;
	bomb = false;
	cannon = false;
}

string Move::toString()
{
	string res = "S ";
	res+=to_string(x_i)+" "+to_string(y_i)+" ";
	if(bomb)
		res+="B ";
	else
		res+="M ";
	res+=to_string(x_f)+" "+to_string(y_f);
	return res;
}

bool Move::isHorizontal()
{
	return x_i == x_f;
}