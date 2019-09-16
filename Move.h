#include <string>
#include <vector>
#include <iostream>

extern int N, M, LIMIT;
extern std::vector<int> validCannonForms, validCannonFormsX, validCannonFormsY;

class Move
{
	public:
	
	int i, f;
	bool bomb, cannon;
	Move(int x, int y, bool b, bool cannon);
	Move(int x, int y, bool cannon);
	Move(int x, int y);
	std::string toString();
};