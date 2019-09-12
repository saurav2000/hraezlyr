#include "Node.h"
using namespace std;

Node::Node(State *s, Move *str, int d)
{
	depth = d;
	if(d&1)
		eval = -1000000000;
	else
		eval = 1000000000;
	state = s;
	move = str;
}

Node::~Node()
{
	for(int i = children.size();i>=0;--i)
		delete children[i];
	children.clear();

	delete state;
	delete move;
}

void Node::addChild(Node *n)
{
	children.push_back(n);
}

int Node::getStateEval()
{
	return state->getEval();
}

bool compA(Node *n1, Node *n2)
{
	return n1->eval < n2->eval;
}

bool compD(Node *n1, Node *n2)
{
	return n1->eval > n2->eval;
}

void Node::sortChildrenAscending()
{
	sort(children.begin(), children.end(), compA);
}

void Node::sortChildrenDescending()
{
	sort(children.begin(), children.end(), compD);
}