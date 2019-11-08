#include "State.h"
#ifndef NODE_H
#define NODE_H

class Node
{
	public:

	Move *move;
	State *state;
	int depth, eval, stateEval;
	bool pruned;
	std::vector<Node*> children;
	Node(State *s, Move *str, int d);
	~Node();
	void addChild(Node *n);
	void sortChildrenAscending();
	void sortChildrenDescending();
	bool hasNoChildren();
	void buildChildren();
};

#endif