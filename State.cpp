#include "State.h"
using namespace std;

State::State(int ar[])
{
	grid.insert(grid.begin(), ar, ar + LIMIT);
	cannons.reserve(14);
}

State::State(const State *s)
{
	grid.reserve(N*M);
	cannons.reserve(14);
	cerr<<"is it coming here\n";
	grid.insert(grid.begin(), s->grid.begin(), s->grid.end());
	cerr<<"is it coming here2\n";
	cannons.insert(cannons.begin(), s->cannons.begin(), s->cannons.end());
	
	// copy(s->grid.begin(), s->grid.end(), grid.begin());
	
	// copy(s->cannons.begin(), s->cannons.end(), cannons.begin());
	cerr<<"is it coming here3\n";
}

State::State()
{
	grid.reserve(N*M);
	cannons.reserve(14); //Approx max no of cannons
}

State* State::doMove(int x_i, int y_i, int x_f, int y_f, char m, int id)
{
	int p_i = x_i + N*y_i;
	int p_f = x_f + N*y_f;
	return doMove(p_i, p_f, m, id);
}

State* State::doMove(int p_i, int p_f, char m, int id)
{
	cerr<<"Coming into function\n";
	State *res = new State(this);
	cerr<<"copying properly\n";
	if(m=='M')
	{
		cerr<<p_i<<" "<<p_f<<"\n";
		res->grid[p_f] = res->grid[p_i];
		res->grid[p_i] = 0;
		cerr<<"Hmm\n";
		for(int i=res->cannons.size()-1;i>=0;--i)
		{
			// cerr<<"ayya lol"<<i<<"\n";
			//OPTIMIZE///////////////////////////////////////
			bool del = false;
			if(res->cannons[i].isPresent(p_i))
			{	
				//removal of res->cannons of initial pos
				// if(!res->cannons[i].isCannonMove(p_i, p_f))
					del = true;
			}
			//removal of res->cannons of final pos
			if(res->cannons[i].isPresent(p_f))
				del = true;

			if(del)
				res->cannons.erase(res->cannons.begin()+i);
			/////////////////////////////////////////////////
		}

		for(int i=0;i<validCannonForms.size();i+=2)
		{
			// cerr<<"wtf is going on\n";
			if(valid(p_f+validCannonForms[i])&&valid(p_f+validCannonForms[i+1])&&grid[p_f+validCannonForms[i]]*id==1&&grid[p_f+validCannonForms[i+1]]*id==1)
			{
				Cannon temp(p_f, p_f+validCannonForms[i], p_f+validCannonForms[i+1], id);
				res->cannons.push_back(temp);
			}
		}
		cerr<<"Out of cannon form loop\n";
	}
	else
	{
		res->grid[p_f] = 0;
		for(int i=res->cannons.size()-1;i>=0;--i)
		{
			if(res->cannons[i].isPresent(p_f))
				res->cannons.erase(res->cannons.begin()+i);
		}	
	}
	cerr<<"ebfore return\n";
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
	vector<Move*> res;
	for(int i=0;i<LIMIT;++i)
	{
		if(grid[i]*id!=1)
			continue;
		bool surr = false;
		for(int j=0;j<3;++j)
		{
			int t = i+validMoves[j]*id;
			if(valid(t)&&grid[t]*id!=1)
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
			if(valid(i+(2*(N+1)*id))&&grid[i+(2*(N+1)*id)]*id<=0)
				res.push_back(new Move(i, i+((N+1)*id)));
			if(valid(i+(2*(N-1)*id))&&grid[i+(2*(N-1)*id)]*id<=0)
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

