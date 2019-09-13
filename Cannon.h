#include "Move.h"
#include <algorithm>

class Cannon
{
	public:
	
	static bool valid(int a, int b);
	int p1, p2, p3, id;
	Cannon(int a, int b, int c, int i);
	Cannon(std::vector<int> v, int i);
	void moveTo(int k);
	bool isCannonMove(int a, int k);
	bool isPresent(int k);
	std::vector<Move*> getPossibleMoves(std::vector<int> grid);
};