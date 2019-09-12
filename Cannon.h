#include <vector>
class Cannon
{
	public:
		int p1, p2, p3;
		Cannon(int a, int b, int c);
		void moveTo(int k);
		bool isCannonMove(int k);
		bool isPresent(int k);
		std::vector<int> getPossibleMoves();
};