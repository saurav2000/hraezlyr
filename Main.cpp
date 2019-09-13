#include "Tree.h"
#include <iostream>
#include <chrono>
#include <random>
using namespace std;

int id, N , M , LIMIT, time_limit;
std::vector<int> validMoves, validCannonForms;

State* initialise()
{
	int directions[] = {1, 2, -1, -2, M, 2*M, -2*M, -1*M, M+1, 2*M+2, M-1, 2*M-2, 1-M, 2-2*M, -1-M, -2-2*M, 1, -1, M, -1*M, M+1, -1-M, M-1, 1-M};	
	int moves[] = {-M, -M+1, -M-1};
	validCannonForms.insert(validCannonForms.begin() , begin(directions), end(directions));
	validMoves.insert(validMoves.begin(), begin(moves), end(moves));
	int grid[LIMIT];

	
	for(int i =0; i<N; i++)
		for(int j =0; j<M; j++)
			grid[i*M+j] = 0;
				
	for(int i =0 ; i <M; i= i+2)//white townhalls
		grid[i] = -2;
		
	for(int i =1 ; i <M; i= i+2)//black townhalls
		grid[(N-1)*M + i] = 2;
		
	for(int i = 1; i<M; i= i+2)//white pawns
		for(int j = 0; j < 3; j++)
			grid[j*M + i]  = -1;
		
	for(int i = 0; i<M; i= i+2)//black pawns
		for(int j = 0; j < 3; j++)
			grid[(N-j-1)*M + i]  = 1;

	State *state = new State(grid);

	return state;
}

int main()
{
	cin>>id>>N>>M>>time_limit;
	id = 1-id*2;

	State *state = initialise();

	std::random_device dev;
	std::mt19937 rng(dev());
	char m;
	int x_i, x_f, y_i, y_f;

	if(id == -1)
	{
		cin>>m ;
		cin>>x_i>>y_i>>m>>x_f>>y_f;
		state->doMove(x_i, y_i, x_f, y_f, m, 1);
	}
	

	while(true)
	{
		std::vector<Move*> possibleMoves = state->getPossibleMoves(id);
		std::uniform_int_distribution<std::mt19937::result_type> dist6(0,possibleMoves.size()-1); // distribution in range [1, 6]
		int rand = dist6(rng);
		Move *move = possibleMoves[rand];
		m = (move->bomb)? 'B' : 'M';
		state->doMove(move->i, move->f, m, id);
		cout<<move->to_string(N)<<"\n";

		cin>>m ;
		cin>>x_i>>y_i>>m>>x_f>>y_f;
		state->doMove(x_i, y_i, x_f, y_f, m, id*-1);

		break;
	}

	return 0;
}