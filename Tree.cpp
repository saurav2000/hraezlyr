using namespace std;

void Tree::iterativeDeepening(Node *node, int tree_depth)
{
	for(int i =0; i<tree_depth; i++)
	{
		alphaBetaMinimax(node, true, i, NegInf, Inf);
	}
}

void Tree::alphaBetaMinimax(Node * node, bool isMax, int itHt, int alpha, int beta){	
	if(itHt==0)
		return node->getEval();

	int i =0;
	for(; i<node->children.size(); i++)
	{
		node->pruned = false;
		alphaBetaMinimax(node->children[i], !isMax, itHt-1, node->alpha, node->beta);
		node->children[i]->pruned = false;

		int val = node->children[i]->eval;
		if(isMax)
		{
			node->alpha = max(node->alpha, val);
			if(node->eval < val)
				node->eval = val;
		}
		else
		{
			node->beta = min(node->beta, val);
			if(node->eval > val)
				node->eval = val;
		}

		if(node->alpha >= node->beta)
			break;
	}

	for(; i<node->children.size(); i++)
	{
		node->children[i]->pruned = true;
	}

	if(isMax)
		node->sortChildrenDescending();
	else
		node->sortChildrenAscending();
	return;	
}
