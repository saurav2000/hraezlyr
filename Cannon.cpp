#include "Cannon.h"
using namespace std;

Cannon::Cannon(int a, int b, int c, int d)
{
	x = a;
	y = b;
	dx = c;
	dy = d;
}

Cannon::Cannon(const Cannon &other)
{
	x = other.x;
	y = other.y;
	dx = other.dx;
	dy = other.dy;
}

bool Cannon::isPresent(int a, int b)
{
	return (x==a&&y==b) || (x+dx==a&&y+dy==b) || (x-dx==a&&y-dy==b);
}

void getMoves(bitset<100> *soldiers, bitset<100> *townhalls, int id, vector<Move*> &res, bool empty)
{
	int p = x + M*y;
	int dx2 = dx<<1, dx3 = dx2+dx, dx4 = dx2<<1;
	int dy2 = dy<<1, dy3 = dy3+dy, dy4 = dy2<<1;
	int Mdy = M*dy, Mdy2 = M*dy2, Mdy3 = M*dy3, Mdy4 = M*dy4;

	if(x+dx2>=0 && x+dx2<N && y+dy2>=0 && y+dy2<M && !soldiers[0][p+dx2+Mdy2] && !soldiers[1][p+dx2+Mdy2] && !townhalls[0][p+dx2+Mdy2] && !townhalls[1][p+dx2+Mdy2])
	{
		res.push_back(new Move(x-dx, y-dy, x+dx2, y+dy2));
		if(!empty)
		{
			if(x+dx3>=0 && x+dx3<N && y+dy3>=0 && y+dy3<M && (soldiers[!id][p+dx3+Mdy3] || townhalls[!id][p+dx3+Mdy3]))
				res.push_back(new Move(x, y, x+dx3, y+dy3, true));
			if(x+dx4>=0 && x+dx4<N && y+dy4>=0 && y+dy4<M && (soldiers[!id][p+dx4+Mdy4] || townhalls[!id][p+dx4+Mdy4]))
				res.push_back(new Move(x, y, x+dx4, y+dy4, true));
		}
		else
		{
			if(x+dx3>=0 && x+dx3<N && y+dy3>=0 && y+dy3<M && (!soldiers[id][p+dx3+Mdy3] && !townhalls[id][p+dx3+Mdy3]))
				res.push_back(new Move(x, y, x+dx3, y+dy3, true));
			if(x+dx4>=0 && x+dx4<N && y+dy4>=0 && y+dy4<M && (!soldiers[id][p+dx4+Mdy4] && !townhalls[id][p+dx4+Mdy4]))
				res.push_back(new Move(x, y, x+dx4, y+dy4, true));
		}
		
	}
	
	if(x-dx2>=0 && x-dx2<N && y-dy2>=0 && y-dy2<M && !soldiers[0][p-dx2-Mdy2] && !soldiers[1][p-dx2-Mdy2] && !townhalls[0][p-dx2-Mdy2] && !townhalls[1][p-dx2-Mdy2])
	{
		res.push_back(new Move(x+dx, y+dy, x-dx2, y-dy2));
		if(!empty)
		{
			if(x-dx3>=0 && x-dx3<N && y-dy3>=0 && y-dy3<M && (soldiers[!id][p-dx3-Mdy3] || townhalls[!id][p-dx3-Mdy3]))
				res.push_back(new Move(x, y, x-dx3, y-dy3, true));
			if(x-dx4>=0 && x-dx4<N && y-dy4>=0 && y-dy4<M && (soldiers[!id][p-dx4-Mdy4] || townhalls[!id][p-dx4-Mdy4]))
				res.push_back(new Move(x, y, x-dx4, y-dy4, true));
		}
		else
		{
			if(x-dx3>=0 && x-dx3<N && y-dy3>=0 && y-dy3<M && (!soldiers[id][p-dx3-Mdy3] && !townhalls[id][p-dx3-Mdy3]))
				res.push_back(new Move(x, y, x-dx3, y-dy3, true));
			if(x-dx4>=0 && x-dx4<N && y-dy4>=0 && y-dy4<M && (!soldiers[id][p-dx4-Mdy4] && !townhalls[id][p-dx4-Mdy4]))
				res.push_back(new Move(x, y, x-dx4, y-dy4, true));
		}
	}
}