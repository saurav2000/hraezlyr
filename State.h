#include "Cannon.h"
#ifndef STATE_H
#define STATE_H value

class State
{
	public:
	std::bitset<100> soldiers[2];
	std::bitset<100> townhalls[2];
	std::vector<Cannon> cannons[2];
	std::vector<Move*> moves[2];
	bool setMoves[2], isTerminal;
	State();
	State(const State *s);
	State(const State &s);
	~State();
	void doMove(int x_i, int y_i, int x_f, int y_f, bool bomb, int id);
	void doMove(Move *m, int id);
	double getEval();
	std::vector<Move*> getPossibleMoves(int id);
	std::vector<Move*> setPossibleMoves(int id);
	bool operator== (const State& other);
};

#endif