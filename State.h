#include "Cannon.h"

class State
{
	public:

	static int N, M, LIMIT;
	static std::vector<int> validMoves;
	static std::vector<int> validCannonForms;
	std::vector<int> grid;
	std::vector<Cannon> cannons;
	State(std::vector<int> ar);
	State(const State *s);
	State();
	State* doMove(int x_i, int y_i, int x_f, int y_f, char m, int id);
	int getEval();
	std::vector<Move*> getPossibleMoves(int id);
	static bool valid(int a);
};