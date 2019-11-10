#include "State.h"
using namespace std;

State::State(const State *s)
{
	soldiers[0] = s->soldiers[0];
	soldiers[1] = s->soldiers[1];

	townhalls[0] = s->townhalls[0];
	townhalls[1] = s->townhalls[1];
	cannons[0] = s->cannons[0];
	cannons[1] = s->cannons[1];
	setMoves[0] = false;
	setMoves[1] = false;
	isTerminal = s->isTerminal;
}

State::State(const State &s)
{
	soldiers[0] = s.soldiers[0];
	soldiers[1] = s.soldiers[1];

	townhalls[0] = s.townhalls[0];
	townhalls[1] = s.townhalls[1];
	cannons[0] = s.cannons[0];
	cannons[1] = s.cannons[1];
	setMoves[0] = false;
	setMoves[1] = false;
	isTerminal = s.isTerminal;
}

State::State()
{
	setMoves[0] = false;
	setMoves[1] = false;
	isTerminal = false;
}

State::~State()
{
	for(int i=moves[0].size()-1;i>=0;--i)
		delete moves[0][i];
	for(int i=moves[1].size()-1;i>=0;--i)
		delete moves[1][i];
}

// id is who is doing the move (Changes within the tree)
void State::doMove(int x_i, int y_i, int x_f, int y_f, bool bomb, int id)
{
	int i = x_i + N*y_i, f = x_f+N*y_f;
	if(id&&y_f==M-1)
		townhalls[0].reset(f);
	if(!id&&y_f==0)
		townhalls[1].reset(f);
	if(bomb)
	{
		soldiers[!id].reset(f);
		for(int a=cannons[!id].size()-1;a>=0;--a)
		{
			if(cannons[!id][a].isPresent(x_f, y_f))
				cannons[!id].erase(cannons[!id].begin() + a);
		}
	}
	else
	{
		if(soldiers[!id][f])
		{
			for(int a=cannons[!id].size()-1;a>=0;--a)
			{
				if(cannons[!id][a].isPresent(x_f, y_f))
					cannons[!id].erase(cannons[!id].begin() + a);
			}
		}
		soldiers[id].reset(i);
		soldiers[id].set(f);
		soldiers[!id].reset(f);
		for(int a=cannons[id].size()-1;a>=0;--a)
		{
			if(cannons[id][a].isPresent(x_i, y_i))
				cannons[id].erase(cannons[id].begin() + a);
		}

		for(int dx = -1, dx2 = -2;dx<=1;++dx, dx2+=2)
		{
			for(int dy = -1, dy2 = -2; dy<=1; ++dy, dy2+=2)
			{
				if((dx||dy) && x_f+dx>=0 && x_f+dx2>=0 && y_f+dy>=0 && y_f+dy2>=0 && x_f+dx<N && x_f+dx2<N && y_f+dy<M && y_f+dy2<M && soldiers[id][f+dx + N*dy] && soldiers[id][f+dx2 + N*dy2])
					cannons[id].push_back(Cannon(x_f+dx, y_f+dy, dx, dy));
			}
		}

		for(int dx = -1;dx<1;++dx)
		{
			for(int dy = -1;dy<=1;++dy)
			{
				if((dx||dy) && x_f+dx>=0 && x_f-dx>=0 && y_f+dy>=0 && y_f-dy>=0 && x_f+dx<N && x_f-dx<N && y_f+dy<M && y_f-dy<M && soldiers[id][x_f+dx + N*(y_f+dy)] && soldiers[id][x_f-dx + N*(y_f-dy)])
					cannons[id].push_back(Cannon(x_f, y_f, dx, dy));
			}
		}
	}

	isTerminal = (townhalls[0].count() == (N/2)-2) || (townhalls[1].count() == (N/2)-2) || !soldiers[0].count() || !soldiers[1].count();
}

void State::doMove(Move *m, int id)
{
	this->doMove(m->x_i, m->y_i, m->x_f, m->y_f, m->bomb, id);
}

bool State::operator== (const State& other)
{
	return soldiers[0]==other.soldiers[0] && soldiers[1]==other.soldiers[1] && townhalls[0]==other.townhalls[0] && townhalls[1]==other.townhalls[1];
}

// ID is who we are in total
double State::getEval()//(bool terminal)
{
	double res = 0;	
	// double soldier_wt = 50, townhall_wt = 10000, cannonGood = 1, cannonOk = 0.25, cannonOk2 = 0.40, soldierMobAdv = 0.005, soldierMobSupAdv = 0.01;// cannonHoriz = 0.5, cannonExist = 0.4;
	// double soldier_wt = 100, townhall_wt = 10000, cannonGood = 1, cannonOk = 0.25, cannonOk2 = 0.40, soldierMobAdv = 1, soldierMobSupAdv = 2;// cannonHoriz = 0.5, cannonExist = 0.4;
	double soldier_wt = 50, soldier_self_wt = 0.08, townhall_wt = 10000, cannonGood = 1, cannonOk = 0.25, cannonOk2 = 0.40, soldierMobAdv = 1, soldierMobSupAdv = 2;// cannonHoriz = 0.5, cannonExist = 0.4;
	if(N == 8 && M == 8)
		soldier_wt = 50, soldier_self_wt = 0, townhall_wt = 10000, cannonGood = 1, cannonOk = 0.25, cannonOk2 = 0.40, soldierMobAdv = 0.005, soldierMobSupAdv = 0.01;// cannonHoriz = 0.5, cannonExist = 0.4;

	res+= soldier_wt * (((int)soldiers[ID].count()- (int)soldiers[!ID].count()) + soldier_self_wt*((int)soldiers[ID].count()));
	// res+= soldier_wt * ((int)soldiers[ID].count()- (int)soldiers[!ID].count());

	int del_townhalls = ((int)townhalls[ID].count() - (int)townhalls[!ID].count()), tScore = 0;
	
	if(isTerminal)
	{
		if(del_townhalls==2)
			tScore = 10;
		else if(del_townhalls==-2)
			tScore = -10;
		else if(!soldiers[!ID].count())
		{
			if(del_townhalls==1)
				tScore = 10;
			else if(del_townhalls ==0)
				tScore = 6;
			else if(del_townhalls==-1)
				tScore = 2;
		}
		else if(!soldiers[ID].count())
		{
			if(del_townhalls==1)
				tScore = -2;
			else if(del_townhalls ==0)
				tScore = -6;
			else if(del_townhalls==-1)
				tScore = -10;
		}
	}
	else
	{
		if(soldiers[ID].count()==1)
		{
			if(del_townhalls==1)
				tScore=2;
			else if (del_townhalls==-1)
				tScore=-6;
			else
				tScore=-2;
		}
		else if(soldiers[!ID].count()==1)
		{
			if(del_townhalls==1)
				tScore=6;
			else if(del_townhalls==-1)
				tScore=-2;
			else
				tScore=2;
		}
		else if(del_townhalls==1)
			tScore = 6;
		else if(del_townhalls==-1)
			tScore = -6;
		else if(townhalls[ID].count() == N/2-1)
			tScore = -2;
	}

	res+= tScore*townhall_wt;
	double cx = 4, cy = 4, score[2] = {0};
	for(int y=0,p=0;y<M;++y)
	{
		for(int x=0;x<N;++x,++p)
		{
			if(!soldiers[0][p] && !soldiers[1][p])
				continue;

			if(soldiers[0][p])
			{
				if(y < M/2-1)
					score[0]+= soldierMobSupAdv*(M - 1 - y);
				else //if (y >= M/2)
					score[0]+= soldierMobAdv*(M - 1 - y);
				// else
				// 	score[0]+= soldierMobRisky;
			}
			else if(soldiers[1][p])
			{
				if(y < M/2 + 1)
					score[1]+= soldierMobAdv*( y);
				else //if (y >= M/2)
					score[1]+= soldierMobSupAdv*(y);

				// if(y >= M - 3)
				// 	score[1]+= soldierMobSupAdv*(y - M + 4);
				// else if (y < M/2)
				// 	score[1]+= soldierMobAdv;
				// else
				// 	score[1]+= soldierMobRisky;
			}
				
		}
	}
	
	// score[0] /= soldiers[0].count() + soldiers[1].count();
	// score[1] /= soldiers[0].count() + soldiers[1].count();

	for(int id=0; id<2; ++id)
	{
		for(int i=cannons[id].size()-1; i>=0; --i)
		{
			int x = cannons[id][i].x, y = cannons[id][i].y, dx = cannons[id][i].dx, dy = cannons[id][i].dy;
			if((id && y > M-5)||(!id && y <4))
				continue;
			
			if(dx*dy > 0)
			{
				int s = id*2 -1; //1->white; -1->black 
				bool hasForwardMove =  (x+s*2 >= 0) && (x+s*2 < N) && (y+s*2) >= 0 && (y+s*2) < M && !soldiers[0][x+s*2 + (y+s*2)*N] && !soldiers[1][x+s*2 + (y+s*2)*N];
				
				if(hasForwardMove)
					score[id] += cannonGood;
			}
			else if(dx*dy < 0)
			{
				int s = id*2 -1; //1->white; -1->black 
				bool hasForwardMove =  (x-s*2 >= 0) && (x-s*2 < N) && (y+s*2) >= 0 && (y+s*2) < M && !soldiers[0][x-s*2 + (y+s*2)*N] && !soldiers[1][x-s*2 + (y+s*2)*N];
				
				if(hasForwardMove)
					score[id] += cannonGood;
			}
			else if(dx == 0)
			{
				int s = id*2 -1; //1->white; -1->black 
				bool hasForwardMove =  (y+s*2) >= 0 && (y+s*2) < M && !soldiers[0][x + (y+s*2)*N] && !soldiers[1][x + (y+s*2)*N];
				
				if(!hasForwardMove)
					continue;

				if((x == N-1 && id) || (x == 0 && !id))
					score[id] += cannonOk2;
				else
					score[id] += cannonOk;
			}
			else//dy == 0
			{
				score[id] += cannonOk;
			}
			
		}
	}

	res += (score[ID]-score[!ID]);
	return res;
}

vector<Move*> State::getPossibleMoves(int id)
{
	if(setMoves[id])
		return moves[id];
	setMoves[id] = true;
	return moves[id] = setPossibleMoves(id);
}

vector<Move*> State::setPossibleMoves(int id)
{
	vector<Move*> res;
	int cid = ((id<<1) - 1);
	int Nid = N*cid;
	for(int y=0,p=0;y<M;++y)
	{
		for(int x=0;x<N;++x,++p)
		{
			if(!soldiers[id][p])
				continue;
			bool surr = false;
			//3 forward moves
			if(y+cid>=0 && y+cid<M)
			{
				for(int a=-1;a<=1;++a)
				{
					if(x+a>=0 && x+a<N && !soldiers[id][p+Nid+a])
					{
						res.push_back(new Move(x,y,x+a,y+cid));
						if(soldiers[!id][p+Nid+a])
							surr = true;
					}
				}
			}

			//2 side moves
			if(x-1>=0 && x-1<N && (soldiers[!id][p-1] || townhalls[!id][p-1]))
			{
				if(soldiers[!id][p-1])
					surr = true;
				res.push_back(new Move(x,y,x-1,y));
			}
			if(x+1>=0 && x+1<N && (soldiers[!id][p+1] || townhalls[!id][p+1]))
			{
				if(soldiers[!id][p+1])	
					surr = true;
				res.push_back(new Move(x,y,x+1,y));
			}

			cid = cid<<1;
			Nid = Nid<<1;

			//retreating moves
			if(surr && y-cid>=0 && y-cid<M)
			{
				for(int a=-2;a<=2;a+=2)
				{
					if(x+a>=0 && x+a<N && !soldiers[id][p+a-Nid] && !townhalls[id][p+a-Nid])
						res.push_back(new Move(x,y,x+a,y-cid));
				}
			}

			cid = cid>>1;
			Nid = Nid>>1;
		}
	}

	for(int i=cannons[id].size()-1;i>=0;--i)
		cannons[id][i].getMoves(soldiers, townhalls, id, res, id!=ID);

	return res;
}