#include "State.h"
using namespace std;

State::State(vector<int> ar)
{
	copy(ar.begin(), ar.end(), grid.begin());
}

State::State(const State *s)
{
	copy(s->grid.begin(), s->grid.end(), grid.begin());
	copy(s->cannons.begin(), s->cannons.end(), cannons.begin());
}

State* State::doMove(int x_i, int y_i, int x_f, int y_f, char m)
{
	State *res = new State(this);
	if(m=='M')
	{
		res->grid[x_f + M*y_f] = res->grid[x_i + M*y_i];
		res->grid[x_i + M*y_i] = 0;
	}
	else
		res->grid[x_f + M*y_f] = 0;

	return res;
}

int State::getEval()
{
	return 0;
}

bool State::valid(int a)
{
	return a>=0&&a<LIMIT;
}

vector<Move*> State::getPossibleMoves(int id)
{
	vector<Move*> res(100);
	for(int i=0;i<LIMIT;++i)
	{
		if(grid[i]*id!=1)
			continue;
		bool surr = false;
		for(int j=0;j<3;++j)
		{
			int t = i+validMoves[j]*id;
			if(valid(t))
			{
				res.push_back(new Move(i, t));
				if(grid[t]*id==-1)
					surr = true;
			}

		}

		if(valid(i-1)&&grid[i-1]*id==-1)
		{
			res.push_back(new Move(i, i-1));
			surr = true;
		}
		if(valid(i+1)&&grid[i+1]*id==-1)
		{
			res.push_back(new Move(i, i+1));
			surr = true;
		}
		if(surr)
		{
			if(valid(i+((N+1)*id)))
				res.push_back(new Move(i, i+((N+1)*id)));
			if(valid(i+((N-1)*id)))
				res.push_back(new Move(i, i+((N-1)*id)));
		}
	}

	for(int i=0;i<cannons.size();++i)
	{
		if(cannons[i].id!=id)
			continue;
		vector<Move*> temp = cannons[i].getPossibleMoves(grid);
		res.insert(res.end(), temp.begin(), temp.end());
	}

	return res;
}

