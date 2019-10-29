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

bool Cannon::isCannonMove(int a, int k)
{
	return ((a==p1)&&((k-p3)==(p2-p1))) || ((a==p3)&&((p1-k)==(p2-p1)));

}

bool Cannon::isPresent(int k)
{
	return p1==k||p2==k||p3==k;
}

bool Cannon::valid(int a, int b)
{
	return a>=0&&a<M&&b>=0&&b<N;
}

vector<Move*> Cannon::getPossibleMoves(Grid grid)
{
	int p1_x = p1%M, p1_y = p1/M, p2_x = p2%M, p2_y = p2/M, p3_x = p3%M, p3_y = p3/M;
	vector<Move*> res;
	int d_x = p2_x - p1_x;
	int d_y = p2_y - p1_y;
	int d = p2-p1;
	if(valid(p1_x-d_x, p1_y-d_y)&&grid(p1_x-d_x, p1_y-d_y)==0)
	{
		res.push_back(new Move(p3_x, p3_y, p1_x-d_x, p1_y-d_y true));
		if(valid(p1_x-2*d_x, p1_y-2*d_y)&&grid(p1_x-2*d_x, p1_y-2*d_y)*id<=0)
			res.push_back(new Move(p2_x, p2_y, p1_x-2*d_x, p1_y-2*d_y, true, true));
		if(valid(p1_x-3*d_x, p1_y-3*d_y)&&grid(p1_x-3*d_x, p1_y-3*d_y)*id<=0)
			res.push_back(new Move(p2_x, p2_y, p1_x-3*d_x, p1_y-3*d_y, true, true));
	}

	if(valid(p3_x+d_x, p3_y+d_y)&&grid(p3_x+d_x, p3_y+d_y)==0)
	{
		res.push_back(new Move(p1_x, p1_y, p3_x+d_x, p3_y+d_y true));
		if(valid(p3_x+2*d_x, p3_y+2*d_y)&&grid(p3_x+2*d_x, p3_y+2*d_y)*id<=0)
			res.push_back(new Move(p2_x, p2_y, p3_x+2*d_x, p3_y+2*d_y, true, true));
		if(valid(p3_x+3*d_x, p3_y+3*d_y)&&grid(p3_x+3*d_x, p3_y+3*d_y)*id<=0)
			res.push_back(new Move(p2_x, p2_y, p3_x+3*d_x, p3_y+3*d_y, true, true));
	}
	return res;
}



