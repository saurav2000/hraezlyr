#include "Cannon.h"

class State
{
	public:

	std::vector<int> grid;
	std::vector<Cannon> cannons;
	State(int ar[]);
	State(const State *s);
	State();
	State* doMove(int x_i, int y_i, int x_f, int y_f, char m, int id);
	State* doMove(int p_i, int p_f, char m, int id);
	int getEval();
	std::vector<Move*> getPossibleMoves(int id);
	static bool valid(int a);
};