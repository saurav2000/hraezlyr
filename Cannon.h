#include "Move.h"
#include <vector>
#include <algorithm>

class Cannon
{
	public:
		static int N, M, LIMIT;
		static bool valid(int a);
		int p1, p2, p3, id;
		Cannon(int a, int b, int c, int i);
		void moveTo(int k);
		bool isCannonMove(int k);
		bool isPresent(int k);
		std::vector<Move*> getPossibleMoves(std::vector<int> grid);
};