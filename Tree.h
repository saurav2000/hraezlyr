#include "Node.h"
#include <chrono>

class Tree
{
	public:

	Node *root;
	int id;
	Tree(Node *n, int i);
	~Tree();
	void iterativeDeepening(Node *node, int tree_depth);
	void alphaBetaMinimax(Node *node, bool isMax, int itHt, int alpha, int beta);
};