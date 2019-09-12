#include "Cannon.h"
using namespace std;

Cannon::Cannon(int a, int b, int c, int i)
{
	p1 = a;
	p2 = b;
	p3 = c;
	id = i;

	if(p1>p3)
		swap(p1,p3);
	if(p1>p2)
		swap(p1,p2);
	if(p2>p3)
		swap(p2,p3);
}

void Cannon::moveTo(int k)
{
	if(k>p3)
	{
		p1 = p2;
		p2 = p3;
		p3 = k;
	}
	else
	{
		p3 = p2;
		p2 = p1;
		p1 = k;
	}
}

bool Cannon::isCannonMove(int k)
{
	if((p1-k)==(p2-p1)) return true;
	if((k-p3)==(p2-p1)) return true;
	return false;
}

bool Cannon::isPresent(int k)
{
	if(p1==k||p2==k||p3==k) return true;
	return false;
}

bool Cannon::valid(int a)
{
	return a>=0&&a<LIMIT;
}

vector<Move*> Cannon::getPossibleMoves(std::vector<int> grid)
{
	vector<Move*> res;
	int d = p2-p1;
	if(valid(p1-d))
	{
		res.push_back(new Move(p2, p1-d));
		if(valid(p1-2*d))
			res.push_back(new Move(p2, p1-2*d, true));
		if(valid(p1-3*d))
			res.push_back(new Move(p2, p1-3*d, true));
	}

	if(valid(p3+d))
	{
		res.push_back(new Move(p2, p3+d));
		if(valid(p3+2*d))
			res.push_back(new Move(p2, p3+2*d, true));
		if(valid(p3+3*d))
			res.push_back(new Move(p2, p3+3*d, true));
	}
	return res;
}



