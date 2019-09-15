#include "Tree.h"
#include <random>
using namespace std;

int id, N , M , LIMIT, time_limit;
std::vector<int> validCannonForms, validCannonFormsX, validCannonFormsY;

State* initialise()
{
	int directions[] = {1, 2, -1, -2, M, 2*M, -2*M, -1*M, M+1, 2*M+2, M-1, 2*M-2, 1-M, 2-2*M, -1-M, -2-2*M, 1, -1, M, -1*M, M+1, -1-M, M-1, 1-M};
	int directionsX[] = {1, 2, -1, -2, 0, 0, 0, 0, 1, 2, -1, -2, 1, 2, -1, -2, 1, -1, 0, 0, 1, -1, -1, 1};
	int directionsY[] = {0, 0, 0, 0, 1, 2, -2, -1, 1, 2, 1, 2, -1, -2, -1, -2, 0, 0, 1, -1, 1, -1, 1, -1};

	validCannonForms.insert(validCannonForms.begin() , begin(directions), end(directions));
	validCannonFormsX.insert(validCannonFormsX.begin() , begin(directionsX), end(directionsX));
	validCannonFormsY.insert(validCannonFormsY.begin() , begin(directionsY), end(directionsY));
	int grid[LIMIT] = {0};
	// for (int i = 0; i < N; ++i)
	// {
	// 	for (int j = 0; j < M; ++j)
	// 	{
	// 		grid[8*i+j] = 0;
	// 	}
	// }
				
	for(int i=0; i<M; i+=2)
	{
		grid[i] = -2;//white townhalls
		grid[(N-1)*M + i+1] = 2;//black townhalls
	}	
	
	vector<int> temp;
	vector<Cannon> cans;
	cans.reserve(14);
	for(int i=1; i<M; i+=2)//white pawns
	{
		for(int j=0; j<3; j++)
		{
			grid[j*M + i] = -1;
			temp.push_back(j*M + i);
		}
		Cannon c(temp, -1);
		cans.push_back(c);
		temp.clear();
	}
		
	for(int i=0; i<M; i+=2)//black pawns
	{
		for(int j=0; j<3; j++)
		{
			grid[(N-j-1)*M + i] = 1;
			temp.push_back((N-j-1)*M + i);
		}
		Cannon c(temp, 1);
		cans.push_back(c);
		temp.clear();	
	}

	State *state = new State(grid);
	state->cannons = cans;
	return state;
}

int main()
{
	cin >> id >> N >> M >> time_limit;
	LIMIT = N*M;
	id--;
	id = 1-id*2;
	cerr<<id<<" "<<LIMIT<<" "<<N<<" "<<M<<" "<<"\n";

	State *mainState = initialise();
	std::random_device dev;
	std::mt19937 rng(dev());
	char m;
	int x_i, x_f, y_i, y_f;

	if(id == -1)
	{
		cin>>m ;
		cin>>x_i>>y_i>>m>>x_f>>y_f;
		mainState = mainState->doMove(x_i, y_i, x_f, y_f, m, 1);
		// cerr<<"He played and "<<mainState->cannons.size()<<"\n";
	}
	

	while(true)
	{
		Node *n = new Node(mainState, NULL, 0);
		std::vector<Move*> possibleMoves = mainState->getPossibleMoves(id);
		std::uniform_int_distribution<std::mt19937::result_type> dist6(0,possibleMoves.size()-1); // distribution in range [1, 6]
		int rand = dist6(rng);
		Move *move = possibleMoves[rand];
		cout<<move->toString(N)<<"\n";
		mainState = mainState->doMove(move, id);
		// cerr<<"I played and "<<mainState->cannons.size()<<"\n";
		cin>>m ;
		cin>>x_i>>y_i>>m>>x_f>>y_f;
		mainState = mainState->doMove(x_i, y_i, x_f, y_f, m, -id);
		// cerr<<"He played and "<<mainState->cannons.size()<<"\n";
	}

	return 0;
}