#include "State.h"

class Node
{
	public:
		Move *move;
		State *state;
		int depth, eval;
		std::vector<Node*> children;
		Node(State *s, Move *str, int );
		~Node();
		void addChild(Node *n);
		int getStateEval();
		void sortChildrenAscending();
		void sortChildrenDescending();
};