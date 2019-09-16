#include "Cannon.h"

class State
{
	public:

	int num[4];//bs-ws-bt-st;
	std::vector<int> grid;
	std::vector<Cannon> cannons;
	std::vector<Move*> possibleMovesB, possibleMovesW;
	State(int ar[]);
	State(const State *s);
	State();
	~State();
	State* doMove(int x_i, int y_i, int x_f, int y_f, char m, int id);
	State* doMove(int p_i, int p_f, char m, int id);
	State* doMove(Move *m, int id);
	int getEval(int id);
	std::vector<Move*> getPossibleMoves(int id);
	std::vector<Move*> setPossibleMoves(int id);
	static bool valid(int a, int b);

	private:
	void updateCount();
};