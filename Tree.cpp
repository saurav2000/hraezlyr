#include "Tree.h"
using namespace std;

Tree::Tree(Node *n, int i)
{
	root = n;
	root->pruned = false;
	id = i;
}

Tree::~Tree()
{
	delete root;
}

void Tree::iterativeDeepening(int tree_depth)
{
	for(int i=0; i<=tree_depth; ++i)
	{
		cerr<<i<<"\n";
		alphaBetaMinimax(root, true, i, -1000000000, 1000000000);
	}
}

void Tree::alphaBetaMinimax(Node *node, bool isMax, int itHt, int alpha, int beta)
{	
	if(itHt==0)
	{
		node->eval = node->getStateEval(id);
		return;
	}

	if(node->hasNoChildren())
		node->buildChildren(id);

	int i=0;
	for(; i<node->children.size(); ++i)
	{
		node->pruned = false;
		alphaBetaMinimax(node->children[i], !isMax, itHt-1, alpha, beta);
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

	for(; i<node->children.size(); ++i)
		node->children[i]->pruned = true;
	

	if(isMax)
		node->sortChildrenDescending();
	else
		node->sortChildrenAscending();
	return;	
}
