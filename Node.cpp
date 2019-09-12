#include "Node.h"
#include <algorithm>
using namespace std;

Node::Node(State *s, string str, Node *n, int d)
{
	depth = d;
	state = s;
	move = str;
	parent = n;
}

Node::~Node()
{
	for(int i = children.size();i>=0;--i)
		delete children[i];
	children.clear();

	delete state;
	delete parent;
}

void Node::addChild(Node *n)
{
	children.push_back(n);
}

int Node::getEval()
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