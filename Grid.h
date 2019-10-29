#include "Global.h"

class Grid
{
	public:
	
	Grid(int[10][10] g, int n[4]);
	Grid(const Grid &g);
	~Grid();
	int grid[10][10];
	int num[4];

	int& operator() (int x, int y);
	int& operator[] (int x);
};