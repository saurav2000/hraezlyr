#include "Global.h"

class Grid
{
	public:
	
	Grid(int[10][10] g, int n[4]);
	Grid(int[] g);
	Grid(const Grid &g);
	~Grid();
	int grid[10][10];
	int num[4];//bs-ws-bt-wt

	int& operator() (int x, int y);
	int& operator[] (int x);
};