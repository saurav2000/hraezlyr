#include "Tree.h"
using namespace std;

int ID, N , M , LIMIT, time_limit;

string initBoardState[3][11]={
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
	State s1;
	cin >> ID >> M >> N >> time_limit;
	--ID;

	State *mainState = initialise();
	char m;
	int x_i, x_f, y_i, y_f, ply = 5;
	bool stagnant = false;

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
		if(stagnant)
		{
			cout<<tree->root->children[1]->move->toString()<<"\n";
			stagnant = false;
		}
		else
			cout<<tree->root->children[0]->move->toString()<<"\n";
			
		

		auto end  = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
		cerr<<duration<<"\n";
		mainState = new State(tree->root->children[0]->state);


		if(s1 == *mainState)
			stagnant = true;
		s1 = *mainState;

		for(int i=0;i<tree->root->children.size();++i)
			cerr<<tree->root->children[i]->move->toString()<<" "<<tree->root->children[i]->eval<<" "<<tree->root->children[i]->stateEval<<"\n";
		cerr<<stagnant<<"\n";
		int b = (tree->root->children.size() + tree->root->children[0]->children.size())/2; 
		if(b<10)
			ply = 12;
		if(b <15)
			ply = 10;
		else if(b < 22)
			ply = 9;
		delete tree;

		cin>>m ;
		cin>>x_i>>y_i>>m>>x_f>>y_f;
		mainState->doMove(x_i, y_i, x_f, y_f, m=='B', !ID);
	}
	return 0;
}