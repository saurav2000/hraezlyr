#include "Node.h"
using namespace std;

Node::Node(State *s, Move *str, int d)
{
	depth = d;
	stateEval = s->getEval();
	eval = stateEval;
	pruned = false;
	state = s;
	move = str;
}

Node::~Node()
{
	for(int i = children.size()-1;i>=0;--i)
		delete children[i];
	delete state;
}

void Node::addChild(Node *n)
{
	children.push_back(n);
}

bool compA(Node *n1, Node *n2)
{
	if(n1->pruned == n2->pruned)
		return n1->eval == n2->eval ? n1->stateEval < n2->stateEval : n1->eval < n2->eval;
	return n1->pruned;
}

bool compD(Node *n1, Node *n2)
{
	if(n1->pruned == n2->pruned)
		return n1->eval == n2->eval ? n1->stateEval > n2->stateEval : n1->eval > n2->eval;
	return n1->pruned;
}

void Node::sortChildrenAscending()
{
	sort(children.begin(), children.end(), compA);
}

void Node::sortChildrenDescending()
{
	sort(children.begin(), children.end(), compD);
}

void Node::buildChildren()
{
	int id = ID ^ (depth&1);
	vector<Move*> v = state->getPossibleMoves(id);
	for(int i=0;i<v.size();++i)
	{
		State *s = new State(state);
		s->doMove(v[i], id);
		this->addChild(new Node(s, v[i], depth+1));
	}
}

bool Node::hasNoChildren()
{
	return children.empty();
}