#include "Cannon.h"
#include <string>
class State
{
	public:

	static int N, M;
	int LIMIT = N*M - 1;
	std::vector<int> grid;
	std::vector<Cannon> cannon;
	State(std::vector<int> ar);
	State(const State *s);
	State* doMove(int x_i, int y_i, int x_f, int y_f, char m);
	int getEval();
	std::vector<std::string> getPossibleMoves(int id);
};