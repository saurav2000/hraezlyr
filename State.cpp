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
	grid.clear();
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
	// int w_t = 2000, w_s = 300, w_non_hb = 25, w_hb = 5, w_at_b = 50, w_uat_b = -10, w_as = 20, w_mb = 10;
	//int w_t = 2000, w_s = 300, w_non_hb = 20, w_hb = 10, w_at_b = 50, w_uat_b = 0, w_as = 20, w_mb = 10;
	int w_t = 200, w_s = 50, w_as = 1, w_a_nhc = 8 , w_a_hc = 6, w_rs = 2;
	// int w_t = 200, w_s = 40, w_as = 1, w_a_nhc = 8, w_a_hc = 4, w_rs = 1;
	int eval = 0;
	int del_t = 0, del_s = 0, del_as = 0, del_a_nhc = 0, del_a_hc = 0, del_rs = 0;// del_c = 0, del_ms = 0, del_mc = 0, del_bc = 0, del_as = 0;

	del_s = num[0] - num[1];
	del_t = num[2] - num[3];
	eval = ((w_t*del_t) + (w_s * del_s))*id;

	// eval += (w_c * del_c);
	//int hb =0, non_hb = 0, at_b = 0, uat_b = 0;
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

		// // bool bomb = blackMoves[i]->bomb, cannon = blackMoves[i]->cannon;
		// // int f = blackMoves[i]->f;
		// // if(bomb && cannon && grid[f]<0)
		// // 	del_bc++;
		// // else if(!bomb && cannon)
		// // 	del_mc++;
		// // else if(!cannon && grid[f] == 0)
		// // 	del_ms++;
		// if(!blackMoves[i]->cannon && grid[blackMoves[i]->f] < 0)
		// 	del_as++;
		// if(!blackMoves[i]->bomb)
		// 	continue;
		// if(grid[blackMoves[i]->f]!=0)
		// 	++at_b;
		// else
		// 	++uat_b;
		// if(blackMoves[i]->isHorizontal())
		// 	++hb;
		// else 
		// 	++non_hb;
	}
	int del_off = ((w_a_nhc*del_a_nhc) + (w_as*del_as))*id;
	int del_def = ((del_a_hc*w_a_hc) + (w_rs*del_rs))*id;
	if((del_off - del_def) > 0)
		eval += 2*del_off + del_def;
	else
		eval += 2*del_off + 5*del_def;
	// int del_mb = (int)blackMoves.size() - (int)whiteMoves.size() ;
	// eval += (hb*w_non_hb) +(w_hb*hb) + (w_at_b*at_b) + (w_uat_b*uat_b) + (w_mb*del_mb)+ (w_as * del_as);
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

