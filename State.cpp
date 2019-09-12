#include "State.h"
#include <algorithm>
using namespace std;

State::State(vector<int> ar)
{
	copy(ar.begin(), ar.end(), grid.begin());
}

State::State(const State *s)
{
	copy(s->grid.begin(), s->grid.end(), grid.begin());
	copy(s->cannon.begin(), s->cannon.end(), cannon.begin());
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

