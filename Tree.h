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
	void alphaBetaMinimax(Node *node, bool isMax, int itHt, int alpha, int beta, double rem_time);
};

#endif