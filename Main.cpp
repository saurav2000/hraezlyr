#include "Tree.h"
using namespace std;

int ID, N , M , LIMIT, time_limit;

string initBoardState[3][4]={
	{"0101010101010101010101010000000000000000000000000000000000000000",
	 "1010101000000000000000000000000000000000000000000000000000000000",
	 "0000000000000000000000000000000000000000101010101010101010101010",
	 "0000000000000000000000000000000000000000000000000000000001010101"},
	{"01010101010101010101010100000000000000000000000000000000000000000000000000000000",
	 "10101010000000000000000000000000000000000000000000000000000000000000000000000000",
	 "00000000000000000000000000000000000000000000000000000000101010101010101010101010",
	 "00000000000000000000000000000000000000000000000000000000000000000000000001010101"},
	{"0101010101010101010101010101010000000000000000000000000000000000000000000000000000000000000000000000",
	 "1010101010000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000",
	 "0000000000000000000000000000000000000000000000000000000000000000000000101010101010101010101010101010",
	 "0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000101010101"}};

State* initialise()
{
	int index = 0;
	if(N==10)
		index = 2;
	else if(M==10)
		index = 1;
	
	State *s = new State();
	s->soldiers[0] = bitset<100>(initBoardState[index][0]);
	s->townhalls[0] = bitset<100>(initBoardState[index][1]);
	s->soldiers[1] = bitset<100>(initBoardState[index][2]);
	s->townhalls[1] = bitset<100>(initBoardState[index][3]);
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
	--ID;

	State *mainState = initialise();
	char m;
	int x_i, x_f, y_i, y_f, ply = 4;

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
		cout<<tree->root->children[0]->move->toString()<<"\n";
		auto end  = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
		cerr<<duration<<"\n";
		mainState = new State(tree->root->children[0]->state);
		for(int i=0;i<tree->root->children.size();++i)
			cerr<<tree->root->children[i]->move->toString()<<" "<<tree->root->children[i]->eval<<" "<<tree->root->children[i]->stateEval<<"\n";
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