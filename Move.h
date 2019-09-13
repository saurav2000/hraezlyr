#include <string>
#include <vector>

extern int N, M, LIMIT;
extern std::vector<int> validMoves, validCannonForms;

class Move
{
	public:
	
	int i, f;
	bool bomb;
	Move(int x, int y, bool b);
	Move(int x, int y);
	std::string to_string(int n);
};