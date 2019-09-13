#include "Node.h"

class Tree
{
	public:

	Node *root;
	Tree(Node *n);
	~Tree();
	void iterativeDeepening(Node *node, int tree_depth);
	void alphaBetaMinimax(Node *node, bool isMax, int itHt, int alpha, int beta);
};