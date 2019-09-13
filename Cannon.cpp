#include "Cannon.h"
using namespace std;

Cannon::N = 2;
Cannon::M = 3;
Cannon::LIMIT = 6;

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

bool Cannon::isCannonMove(int a, int k)
{
	return ((a==p1)&&((k-p3)==(p2-p1))) || ((a==p3)&&((p1-k)==(p2-p1)));

}

bool Cannon::isPresent(int k)
{
	return p1==k||p2==k||p3==k;
}

bool Cannon::valid(int a)
{
	return a>=0&&a<LIMIT;
}

vector<Move*> Cannon::getPossibleMoves(std::vector<int> grid)
{
	vector<Move*> res;
	int d = p2-p1;
	if(valid(p1-d)&&grid[p1-d]==0)
	{
		res.push_back(new Move(p3, p1-d));
		if(valid(p1-2*d)&&grid[p1-2*d]*id>=0)
			res.push_back(new Move(p2, p1-2*d, true));
		if(valid(p1-3*d)&&grid[p1-3*d]*id>=0)
			res.push_back(new Move(p2, p1-3*d, true));
	}

	if(valid(p3+d)&&grid[p3+d]==0)
	{
		res.push_back(new Move(p1, p3+d));
		if(valid(p3+2*d)&&grid[p3+2*d]*id>=0)
			res.push_back(new Move(p2, p3+2*d, true));
		if(valid(p3+3*d)&&grid[p3+3*d]*id>=0)
			res.push_back(new Move(p2, p3+3*d, true));
	}
	return res;
}



