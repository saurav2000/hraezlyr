#include <string>
#include <vector>
#include <iostream>

extern int N, M, LIMIT;
extern std::vector<int> validCannonForms, validCannonFormsX, validCannonFormsY;

class Move
{
	public:
	
	int i, f;
	bool bomb;
	Move(int x, int y, bool b);
	Move(int x, int y);
	std::string toString(int n);
};