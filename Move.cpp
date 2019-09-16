#include "Move.h"
using namespace std;

Move::Move(int a, int b, bool z, bool y)
{
	i = a;
	f = b;
	bomb = z;
	cannon = y;
}

Move::Move(int a, int b, bool z)
{
	i = a;
	f = b;
	cannon = z;
	bomb = false;
}

Move::Move(int a, int b)
{
	i = a;
	f = b;
	bomb = false;
	cannon = false;
}

string Move::toString()
{
	string res = "S ";
	res+=to_string(i%N)+" "+to_string(i/N)+" ";
	if(bomb)
		res+="B ";
	else
		res+="M ";
	res+=to_string(f%N)+" "+to_string(f/N);
	return res;
}