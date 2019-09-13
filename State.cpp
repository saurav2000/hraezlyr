#include "State.h"
using namespace std;

State::State(int ar[])
{
	grid.insert(grid.begin(), ar, ar + LIMIT);
	cannons.reserve(10);
}

State::State(const State *s)
{
	copy(s->grid.begin(), s->grid.end(), grid.begin());
	copy(s->cannons.begin(), s->cannons.end(), cannons.begin());
}

State::State()
{
	grid.reserve(N*M);
	cannons.reserve(10); //Approx max no of cannons
}

State* State::doMove(int x_i, int y_i, int x_f, int y_f, char m, int id)
{
	int p_i = x_i + N*y_i;
	int p_f = x_f + N*y_f;
	return doMove(p_i, p_f, m, id);
}

State* State::doMove(int p_i, int p_f, char m, int id)
{
	State *res = new State(this);
	if(m=='M')
	{
		res->grid[p_f] = res->grid[p_i];
		res->grid[p_i] = 0;

		for(int i=cannons.size()-1;i>=0;--i)
		{
			//OPTIMIZE///////////////////////////////////////
			bool del = false;
			if(cannons[i].isPresent(p_i))
			{	
				//removal of cannons of initial pos
				// if(!cannons[i].isCannonMove(p_i, p_f))
					del = true;
			}
			//removal of cannons of final pos
			if(cannons[i].isPresent(p_f))
				del = true;

			if(del)
				cannons.erase(cannons.begin()+i);
			/////////////////////////////////////////////////
		}

		for(int i=0;i<validCannonForms.size();i+=2)
		{
			if(valid(p_f+validCannonForms[i])&&valid(p_f+validCannonForms[i+1])&&grid[p_f+validCannonForms[i]]*id==1&&grid[p_f+validCannonForms[i+1]]*id==1)
			{
				Cannon temp(p_f, p_f+validCannonForms[i], p_f+validCannonForms[i+1], id);
				cannons.push_back(temp);
			}
		}
	}
	else
	{
		res->grid[p_f] = 0;
		for(int i=cannons.size()-1;i>=0;--i)
		{
			if(cannons[i].isPresent(p_f))
				cannons.erase(cannons.begin()+i);
		}	
	}

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

		if(valid(i-1)&&grid[i-1]*id<0)
		{
			res.push_back(new Move(i, i-1));
			if(grid[i-1]*id==-1)
				surr = true;
		}
		if(valid(i+1)&&grid[i+1]*id<0)
		{
			res.push_back(new Move(i, i+1));
			if(grid[i+1]*id==-1)
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

