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

State::~State()
{
	grid.clear();
	cannons.clear();
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

State* State::doMove(Move *m, int id)
{
	return doMove(m->i, m->f, m->bomb?'B':'M', id);
}

int State::getEval(int id)
{
	int w_t = 10, w_s = 1, w_ms = 1, w_mc = 1, w_bc = 1, w_as = 1;
	int eval = 0;
	int del_t = 0, det_s = 0, del_c = 0, del_ms = 0, del_mc = 0, del_bc = 0, del_as = 0;

	for(int i =0; i<LIMIT; i++){
		if(grid[i] == -1)//white soilder
			del_s = del_s - 1;
		if(grid[i] == 1)//black soilder
			del_s = del_s + 1;
		if(grid[i] == -2)//white townhall
			del_t = del_t - 1;
		if(grid[i] == 2)//black townhall
			del_t = del_t + 1;
	}
		
	eval = (w_t*del_t) + (w_s * del_s);

	for(int i =0; i< this.cannons.size(); i++){
		del_c += cannons.id;
	}

	eval += (w_c * del_c);

	vector<Move*> whiteMoves = this.getPossibleMoves(1);
	for(int i = 0; i<whiteMoves.size(); i++)
	{
		bool bomb = whiteMoves[i].bomb, cannon = whiteMoves[i].cannon;
		if(bomb && cannon)
			del_bc--;
		else if(!bomb && cannon)
			del_mc--;
		else if(!bomb && !cannon)
			del_ms--;
		else if(bomb && !cannon)
			del_as--;
	}

	vector<Move*> blackMoves = this.getPossibleMoves(-1);
	for(int i = 0; i<blackMoves.size(); i++)
	{
		bool bomb = blackMoves[i].bomb, cannon = blackMoves[i].cannon;
		if(bomb && cannon)
			del_bc++;
		else if(!bomb && cannon)
			del_mc++;
		else if(!bomb && !cannon)
			del_ms++;
		else if(bomb && !cannon)
			del_as++;
	}
	
	eval += (w_ms * del_ms) +(w_mc * del_mc) + (w_bc * del_bc) +(w_as * del_as);
	
	return id * eval;
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

