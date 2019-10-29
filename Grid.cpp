#include "Grid.h"
using namespace std;

Grid::Grid(int[][10] g, int n[4])
{
	for(int i=0;i<M;++i)
	{
		for(int j=0;j<N;++j)
			grid[i][j] = g[i][j];
	}
	num[0] = n[0];
	num[1] = n[1];
	num[2] = n[2];
	num[3] = n[3];
}

Grid::Grid(int[] g)
{
	for(int i=0;i<M;++i)
	{
		for(int j=0;j<N;++j)
			grid[i][j] = g[i+M*j];
	}
	num[0] = M==10?15:12;
	num[1] = M==10?15:12;
	num[2] = M==10?5:4;
	num[3] = M==10?5:4;
}

Grid::Grid(const Grid &g)
{
	for(int i=0;i<M;++i)
	{
		for(int j=0;j<N;++j)
			grid[i][j] = g.grid[i][j];
	}
	num[0] = g.num[0];
	num[1] = g.num[1];
	num[2] = g.num[2];
	num[3] = g.num[3];
}

int& Grid::operator() (int x, int y)
{
	return grid[x][y];
}

int& Grid::operator[] (int x)
{
	return num[x];
}