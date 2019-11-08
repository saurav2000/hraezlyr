#include "Tree.h"
using namespace std;
#define ERROR_VALUE 1e-2

Tree::Tree(Node *n)
{
	root = n;
	root->pruned = false;
}

Tree::~Tree()
{
	delete root;
}

void Tree::iterativeDeepening(int tree_depth, double time)
{
	auto startTime = std::chrono::high_resolution_clock::now();
	for(int i=0; i<=tree_depth; ++i)
	{
		auto finishTime = std::chrono::high_resolution_clock::now();
		double timeCount = std::chrono::duration_cast<std::chrono::duration<double> >(finishTime - startTime).count();
		if(alphaBetaMinimax(root, true, i, -1000000000, 1000000000, time-timeCount))
			cerr<<"Time over at depth "<<i<<"\n";
	}
	cerr<<"Finished at depth" <<tree_depth<< "with time left";
}

int Tree::alphaBetaMinimax(Node *node, bool isMax, int itHt, int alpha, int beta, double rem_time)
{
	auto startTime = std::chrono::high_resolution_clock::now();
	if(!itHt)
		return 0;

	if(node->hasNoChildren())
		node->buildChildren();

	if(node->hasNoChildren())
		return 0;

	int i=0;
	for(;i<node->children.size(); ++i)
	{
		node->pruned = false;
		auto finishTime = std::chrono::high_resolution_clock::now();
		double timeCount = std::chrono::duration_cast<std::chrono::duration<double> >(finishTime - startTime).count();
		if(rem_time - timeCount < ERROR_VALUE)
			return 1;
		if(alphaBetaMinimax(node->children[i], !isMax, itHt-1, alpha, beta, rem_time - timeCount))
			return 1;
		node->children[i]->pruned = false;

		int val = node->children[i]->eval;
		if(isMax)
		{
			alpha = max(alpha, val);
			if(node->eval < val)
				node->eval = val;
		}
		else
		{
			beta = min(beta, val);
			if(node->eval > val)
				node->eval = val;
		}

		if(alpha >= beta)
			break;
	}

	for(;i<node->children.size(); ++i)
		node->children[i]->pruned = true;
	

	if(isMax)
		node->sortChildrenDescending();
	else
		node->sortChildrenAscending();
	return 0;	
}
