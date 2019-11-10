#include "Node.h"
#include <chrono>
#ifndef TREE_H
#define TREE_H

class Tree
{
	public:

	Node *root;
	Tree(Node *n);
	~Tree();
	void iterativeDeepening(int tree_depth, double time);
	int alphaBetaMinimax(Node *node, bool isMax, int itHt, double alpha, double beta, double rem_time);
};

#endif