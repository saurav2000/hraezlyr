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
	grid.insert(grid.begin(), s->grid.begin(), s->grid.end());
	cannons.insert(cannons.begin(), s->cannons.begin(), s->cannons.end());
}

State::State()
{
	grid.reserve(N*M);
	cannons.reserve(14); //Approx max no of cannons
}

State* State::doMove(int x_i, int y_i, int x_f, int y_f, char m, int id)
{
	int p_i = x_i + M*y_i;
	int p_f = x_f + M*y_f;
	State *res = new State(this);
	if(m=='M')
	{
		res->grid[p_f] = res->grid[p_i];
		res->grid[p_i] = 0;
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
			if(valid(x_f+validCannonFormsX[i], y_f+validCannonFormsY[i])&&valid(x_f+validCannonFormsX[i+1], y_f+validCannonFormsY[i+1])&&res->grid[p_f+validCannonForms[i]]*id==1&&res->grid[p_f+validCannonForms[i+1]]*id==1)
			{
				Cannon temp(p_f, p_f+validCannonForms[i], p_f+validCannonForms[i+1], id);
				res->cannons.push_back(temp);
			}
		}
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
	// for(int i =0; i< cannons.size(); i++)
	// 	cerr << res->cannons[i].p1 << " "<< res->cannons[i].p2 << " "<< res->cannons[i].p3 <<" "<< res->cannons[i].id<< "\n";
	// cerr<<"\n";
	return res;
}

State* State::doMove(int p_i, int p_f, char m, int id)
{
	return doMove(p_i%M, p_i/M, p_f%M, p_f/M, m, id);
}

int State::getEval()
{
	return 0;
}

bool State::valid(int a, int b)
{
	return a>=0&&a<M&&b>=0&&b<N;
}

vector<Move*> State::getPossibleMoves(int id)
{
	vector<Move*> res;
	for(int i=0;i<LIMIT;++i)
	{
		if(grid[i]*id!=1)
			continue;
		int i_x = i%M, i_y = i/M;
		bool surr = false;
		for(int j=0;j<3;++j)
		{
			int t_x = i_x+(j-1), t_y = i_y-id, t = t_x + M*t_y;
			if(valid(t_x, t_y)&&grid[t]*id!=1)
			{
				res.push_back(new Move(i, t));
				if(grid[t]*id==-1)
					surr = true;
			}

		}

		if(valid(i_x-1, i_y)&&grid[i-1]*id<0)
		{
			res.push_back(new Move(i, i-1));
			if(grid[i-1]*id==-1)
				surr = true;
		}
		if(valid(i_x+1, i_y)&&grid[i+1]*id<0)
		{
			res.push_back(new Move(i, i+1));
			if(grid[i+1]*id==-1)
				surr = true;
		}
		if(surr)
		{
			if(valid(i_x-2, i_y+2*id)&&grid[i+(2*(M*id -1))]*id<=0)
				res.push_back(new Move(i, i+(2*(M*id -1))));
			if(valid(i_x, i_y+2*id)&&grid[i+(2*(M*id))]*id<=0)
				res.push_back(new Move(i, i+(2*(M*id))));
			if(valid(i_x+2, i_y+2*id)&&grid[i+(2*(M*id +1))]*id<=0)
				res.push_back(new Move(i, i+(2*(M*id +1))));
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

