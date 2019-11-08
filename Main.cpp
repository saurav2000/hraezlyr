#include "Tree.h"
#include <fstream>
using namespace std;

int ID, N , M , LIMIT, time_limit;

State* initialise(string filename)
{
	string bs, bt, ws, wt;
	ifstream fin(filename);
	fin>>bs;

	fin>>bt;
	fin>>ws;
	fin>>wt;
	fin.close();
	State *s = new State();
	s->soldiers[0] = bitset<100>(bs);
	s->soldiers[1] = bitset<100>(ws);
	s->townhalls[0] = bitset<100>(bt);
	s->townhalls[1] = bitset<100>(wt);
	for(int x = 0;x<N;x+=2)
	{
		s->cannons[0].push_back(Cannon(x,M-2,0,1));
		s->cannons[1].push_back(Cannon(x+1,1,0,1));
	}
	return s;
}

int main()
{
	cin >> ID >> M >> N >> time_limit;
	LIMIT = N*M;
	string filename = to_string(LIMIT)+".dat";
	--ID;

	State *mainState = initialise(filename);
	// std::random_device dev;
	// std::mt19937 rng(dev());
	char m;
	int x_i, x_f, y_i, y_f, ply = 5;

	if(ID)
	{
		cin>>m;
		cin>>x_i>>y_i>>m>>x_f>>y_f;
		mainState->doMove(x_i, y_i, x_f, y_f, m=='B', 0);
	}	
	
	while(true)
	{
		auto start = std::chrono::high_resolution_clock::now();
		Tree *tree = new Tree(new Node(new State(mainState), NULL, 0));
		tree->iterativeDeepening(ply, 2.5);
		auto end  = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
		cerr<<duration<<"\n";
		// for(int i = 0;i<tree->root->children.size();++i)
		// {
		// 	cerr<<tree->root->children[i]->move->toString()<<" "<<tree->root->children[i]->eval<<" "<<tree->root->children[i]->getStateEval(id)<<"\n";
		// }
		// int size = 1;
		// for(int i = 1; i<tree->root->children.size(); i++){
		// 	if(tree->root->children[0]->eval != tree->root->children[i]->eval)
		// 		break;
		// 	size++;
		// }

		// std::uniform_int_distribution<std::mt19937::result_type> dist6(0,size-1); // distribution in range [1, 6]
		// int rand = dist6(rng);
		// cout<<tree->root->children[0]->move->toString()<<"\n";
		mainState = new State(tree->root->children[0]->state);
		for(int i=0;i<tree->root->children.size();++i)
			cerr<<tree->root->children[i]->move->toString()<<"\n";
		cerr<<"\n";
		int b = (tree->root->children.size() + tree->root->children[0]->children.size())/2; 
		if(b<10)
			ply = 7;
		if(b <15)
			ply = 6;
		else if(b < 22)
			ply = 5;
		delete tree;
		cin>>m ;
		cin>>x_i>>y_i>>m>>x_f>>y_f;
		mainState->doMove(x_i, y_i, x_f, y_f, m=='B', !ID);
	}
	return 0;
}