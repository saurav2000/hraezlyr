#include "Node.h"
using namespace std;

Node::Node(State *s, Move *str, int d)
{
	depth = d;
	if(d&1)
		eval = -1000000000;
	else
		eval = 1000000000;
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

int Node::getStateEval(int id)
{
	return state->getEval(id);
}

bool compA(Node *n1, Node *n2)
{
	if(n1->pruned == n2->pruned)
		return n1->eval < n2->eval;
	return n1->pruned;
}

bool compD(Node *n1, Node *n2)
{
	if(n1->pruned == n2->pruned)
		return n1->eval > n2->eval;
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

void Node::buildChildren(int ID)
{
	int id = (1 - 2*(depth%2))*ID;
	vector<Move*> v = state->getPossibleMoves(id);
	for(int i=0;i<v.size();++i)
		this->addChild(new Node(state->doMove(v[i], id), v[i], depth+1));
}

bool Node::hasNoChildren()
{
	return children.empty();
}