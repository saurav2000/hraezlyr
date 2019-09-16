#include "State.h"
using namespace std;

State::State(int ar[])
{
	grid.resize(N*M);
	// grid.insert(grid.begin(), ar, ar + LIMIT);
	for(int i=0;i<N*M;++i)
		grid[i]=(ar[i]);
	cannons.reserve(14);
	num[0] = 12;
	num[1] = 12;
	num[2] = 4;
	num[3] = 4;
	// cerr<<"create state "<<" "<<this<<"\n";
}

State::State(const State *s)
{
	int n = s->cannons.size();
	grid.resize(N*M);
	cannons.resize(n);
	// cerr<<s<<"\n";
	for(int i=0;i<N*M;++i)
		grid[i]=(s->grid[i]);
	// n = s->cannons.size();
	for(int i=0;i<n;++i)
		cannons[i]=(s->cannons[i]);
	// cerr<<"create state "<<" "<<this<<"\n";
	// cerr<<s<<"\n";
	for(int i=0;i<4;++i)
		num[i] = s->num[i];
}

State::State()
{
	grid.reserve(N*M);
	cannons.reserve(14); //Approx max no of cannons
	num[0] = 12;
	num[1] = 12;
	num[2] = 4;
	num[3] = 4;
}

State::~State()
{
	// cerr<<"delete "<<this<<"\n";
	grid.clear();
	cannons.clear();
	// for(int i=possibleMovesB.size()-1;i>=0;--i)
		// delete possibleMovesB[i];
	// for(int i=possibleMovesW.size()-1;i>=0;--i)
		// delete possibleMovesW[i];
}

State* State::doMove(int x_i, int y_i, int x_f, int y_f, char m, int id)
{
	int p_i = x_i + M*y_i;
	int p_f = x_f + M*y_f;
	State *res = new State(this);

	if(grid[p_f] == 1)
		res->num[0]--;
	else if(grid[p_f] == -1)
		res->num[1]--;
	else if(grid[p_f] == 2)
		res->num[2]--;
	else if(grid[p_f] == -2)
		res->num[3]--;
		
	if(m=='M')
	{
		res->grid[p_f] = res->grid[p_i];
		res->grid[p_i] = 0;
		for(int i=res->cannons.size()-1;i>=0;--i)
		{
			bool del = false;
			if(res->cannons[i].isPresent(p_i) || res->cannons[i].isPresent(p_f))
				res->cannons.erase(res->cannons.begin()+i);
		}

		for(int i=0;i<validCannonForms.size();i+=2)
		{
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
	// int w_t = 100, w_s = 10, w_c = 20, w_ms = 0, w_mc = 0, w_bc = 30, w_as = 0;
	// int w_t = 100, w_s = 50, w_c = 15, w_ms = 0, w_mc = 0, w_bc = 40, w_as = 20;
	int w_t = 2000, w_s = 200, w_non_hb = 20, w_hb = 1, w_at_b = 30;
	int eval = 0;
	int del_t = 0, del_s = 0;// del_c = 0, del_ms = 0, del_mc = 0, del_bc = 0, del_as = 0;

	del_s = num[0] - num[1];
	del_t = num[2] - num[3];
	eval = (w_t*del_t) + (w_s * del_s);

	// for(int i =0; i< cannons.size(); i++){
	// 	del_c += cannons[i].id;
	// }

	// eval += (w_c * del_c);
	int hb =0, non_hb = 0, at_b = 0;
	vector<Move*> whiteMoves = getPossibleMoves(-1);
	for(int i = 0; i<whiteMoves.size(); i++)
	{
		// bool bomb = whiteMoves[i]->bomb, cannon = whiteMoves[i]->cannon;
		// int f = whiteMoves[i]->f;
		// if(bomb && cannon && grid[f] > 0)
		// 	del_bc--;
		// else if(!bomb && cannon)
		// 	del_mc--;
		// else if(!cannon && grid[f] > 0)
		// 	del_as--;
		// else if(!cannon && grid[f] == 0)
		// 	del_ms--;
		if(!whiteMoves[i]->bomb)
			continue;
		if(grid[whiteMoves[i]->f]!=0)
			--at_b;
		if(whiteMoves[i]->isHorizontal())
			--hb;
		else 
			--non_hb;
	}

	vector<Move*> blackMoves = getPossibleMoves(1);
	for(int i = 0; i<blackMoves.size(); i++)
	{
		// bool bomb = blackMoves[i]->bomb, cannon = blackMoves[i]->cannon;
		// int f = blackMoves[i]->f;
		// if(bomb && cannon && grid[f]<0)
		// 	del_bc++;
		// else if(!bomb && cannon)
		// 	del_mc++;
		// else if(!cannon && grid[f] == 0)
		// 	del_ms++;
		// else if(!cannon && grid[f] < 0)
		// 	del_as++;
		if(!blackMoves[i]->bomb)
			continue;
		if(grid[blackMoves[i]->f]!=0)
			++at_b;
		if(blackMoves[i]->isHorizontal())
			++hb;
		else 
			++non_hb;
	}
	eval += (hb*w_non_hb) +(w_hb*hb) + (w_at_b*at_b);
	cerr << id*eval << " " <<  del_t << " " << del_s << " " << hb << " " <<  non_hb  << " " <<  at_b << "\n";
	// eval += (w_ms * del_ms) +(w_mc * del_mc) + (w_bc * del_bc) +(w_as * del_as);
	// cerr <<del_t <<" "<< del_s <<" "<< del_c <<" "<< del_ms <<" "<< del_mc <<" "<< del_bc <<" "<< del_as <<"\n";
	return id * eval;
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

