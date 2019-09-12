#include <vector>
#include <string>
#include "State.h"
class Node
{
	public:
		std::string move;
		State *state;
		int depth, eval;
		std::vector<Node*> children;
		Node *parent;
		Node(State *s, std::string str, Node *n, int d);
		~Node();
		void addChild(Node *n);
		int getEval();
		void sortChildren();
};