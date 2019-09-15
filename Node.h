#include "State.h"

class Node
{
	public:

	Move *move;
	State *state;
	int depth, eval;
	bool pruned;
	std::vector<Node*> children;
	Node(State *s, Move *str, int d);
	~Node();
	void addChild(Node *n);
	int getStateEval(int id);
	void sortChildrenAscending();
	void sortChildrenDescending();
	bool hasNoChildren();
	void buildChildren(int ID);
};