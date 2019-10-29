#include "State.h"
using namespace std;

State::State(int ar[])
{
	grid = new Grid(ar);
	cannons.reserve(20);
}

State::State(const State *s)
{
	int n = s->cannons.size();
	cannons.resize(n);
	for(int i=0;i<n;++i)
		cannons[i]=(s->cannons[i]);
	grid = s->grid;
}

State::State()
{
	grid.reserve(N*M);
	cannons.reserve(14); //Approx max no of cannons
}

State::~State()
{
	cannons.clear();
	for(int i=possibleMovesB.size()-1;i>=0;--i)
		delete possibleMovesB[i];
	for(int i=possibleMovesW.size()-1;i>=0;--i)
		delete possibleMovesW[i];
}

State* State::doMove(int x_i, int y_i, int x_f, int y_f, char m, int id)
{
	int p_i = x_i + M*y_i;
	int p_f = x_f + M*y_f;
	State *res = new State(this);

	if((*grid)(x_f, y_f) == 1)
		(*(res->grid))[0]--;
	else if((*grid)(x_f, y_f) == -1)
		(*(res->grid))[1]--;
	else if((*grid)(x_f, y_f) == 2)
		(*(res->grid))[2]--;
	else if((*grid)(x_f, y_f) == -2)
		(*(res->grid))[3]--;
		
	if(m=='M')
	{
		(*(res->grid))(x_f, y_f) = (*(res->grid))(x_i, y_i);
		(*(res->grid))(x_i, y_i) = 0;
		for(int i=res->cannons.size()-1;i>=0;--i)
		{
			bool del = false;
			if(res->cannons[i].isPresent(p_i) || res->cannons[i].isPresent(p_f))
				res->cannons.erase(res->cannons.begin()+i);
		}

		for(int i=0;i<validCannonForms.size();i+=2)
		{
			if(valid(x_f+validCannonFormsX[i], y_f+validCannonFormsY[i])&&valid(x_f+validCannonFormsX[i+1], y_f+validCannonFormsY[i+1])&&(*(res->grid))(x_f+validCannonFormsX[i], y_f+validCannonFormsY[i])*id==1&&(*(res->grid))(x_f+validCannonFormsX[i+1], y_f+validCannonFormsY[i+1])*id==1)
			{
				Cannon temp(p_f, p_f+validCannonForms[i], p_f+validCannonForms[i+1], id);
				res->cannons.push_back(temp);
			}
		}
	}
	else
	{
		(*(res->grid))(x_f, y_f) = 0;
		for(int i=res->cannons.size()-1;i>=0;--i)
		{
			if(res->cannons[i].isPresent(p_f))
				res->cannons.erase(res->cannons.begin()+i);
		}	
	}
	return res;
}

State* State::doMove(int p_i, int p_f, char m, int id)
{
	return doMove(p_i%M, p_i/M, p_f%M, p_f/M, m, id);
}

State* State::doMove(Move *m, int id)
{
	return doMove(m->x_i, m->y_i, m->x_f, m->y_f, m->bomb?'B':'M', id);
}

int State::getEval(int id)
{
	int w_t = 200, w_s = 50, w_as = 1, w_a_nhc = 8 , w_a_hc = 6, w_rs = 2;
	int eval = 0;
	int del_t = 0, del_s = 0, del_as = 0, del_a_nhc = 0, del_a_hc = 0, del_rs = 0;// del_c = 0, del_ms = 0, del_mc = 0, del_bc = 0, del_as = 0;

	del_s = num[0] - num[1];
	del_t = num[2] - num[3];
	eval = ((w_t*del_t) + (w_s * del_s))*id;
	vector<Move*> whiteMoves = getPossibleMoves(-1);
	for(int i = 0; i<whiteMoves.size(); i++)
	{
		if(!whiteMoves[i]->cannon && grid[whiteMoves[i]->f] > 0)
			--del_as;
		if(!whiteMoves[i]->cannon && ((whiteMoves[i]->f/M) - (whiteMoves[i]->i/M))<0)
			--del_rs;
		
		if(!whiteMoves[i]->cannon || grid[whiteMoves[i]->f]==0)//no attack
			continue;

		if(whiteMoves[i]->isHorizontal())
			--del_a_hc;
		else 
			--del_a_nhc;
	}

	vector<Move*> blackMoves = getPossibleMoves(1);
	for(int i = 0; i<blackMoves.size(); i++)
	{
		if(!blackMoves[i]->cannon && grid[blackMoves[i]->f]<0)
			++del_as;
		if(!blackMoves[i]->cannon && ((blackMoves[i]->f)/M - (blackMoves[i]->i)/M)>0)
			++del_rs;
		
		if(!blackMoves[i]->cannon || grid[blackMoves[i]->f]==0)//no attack
			continue;

		if(blackMoves[i]->isHorizontal())
			++del_a_hc;
		else 
			++del_a_nhc;
	}
	int del_off = ((w_a_nhc*del_a_nhc) + (w_as*del_as))*id;
	int del_def = ((del_a_hc*w_a_hc) + (w_rs*del_rs))*id;
	if((del_off - del_def) > 0)
		eval += 2*del_off + del_def;
	else
		eval += 2*del_off + 5*del_def;
	return eval;
}

bool State::valid(int a, int b)
{
	return a>=0&&a<M&&b>=0&&b<N;
}

vector<Move*> State::getPossibleMoves(int id)
{
	if(id==1)
	{
		if(possibleMovesB.empty())
			return possibleMovesB = setPossibleMoves(1);
		return possibleMovesB;
	}
	else
	{
		if(possibleMovesW.empty())
			return possibleMovesW = setPossibleMoves(-1);
		return possibleMovesW;	
	}
}

vector<Move*> State::setPossibleMoves(int id)
{
	vector<Move*> res;
	for(int i_x=0;i_x<M;++i_x)
	{
		for(int i_y=0;i_y<N;++i_y)
		{
			if(grid[i]*id!=1)
			continue;
		
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
	}

	for(int i=0;i<cannons.size();++i)
	{
		if(cannons[i].id!=id)
			continue;
		vector<Move*> temp = cannons[i].getPossibleMoves(*grid);
		res.insert(res.end(), temp.begin(), temp.end());
	}

	return res;
}
