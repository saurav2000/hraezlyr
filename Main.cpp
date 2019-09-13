#include "Tree.h"
#include <iostream>
#include <chrono>
#include <random>
using namespace std;

//int State::N = 1;
int id, N = 8, M = 8, LIMIT = N*M;
std::vector<int> validMoves, validCannonForms;

vector<string> split(string s, char c);

int main()
{
	int time_limit;
	
	//cin >> id >> N >> M >> time_limit;
	id =0 ; N = 8; M = 8; time_limit = 120;
	LIMIT = N*M;

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

    State* state = new State(grid);

    std::random_device dev;
    std::mt19937 rng(dev());
    string doMove;

    if(id == 1){
    	cin >> doMove;
    	std::vector<string> instr = split(doMove, ' ');        
        int x_i = stoi(instr[1]), y_i = stoi(instr[2]), x_f = stoi(instr[4]), y_f = stoi(instr[5]);
       
    	state->doMove(x_i, y_i, x_f, y_f, instr[3][0], 1); 
    }
    
    int parityId = (id == 0)? 1: -1;
    
    int count = 0;
    while(true && count++  < 100){
    	std::vector<Move*> possibleMoves = state->getPossibleMoves(parityId);
    	std::uniform_int_distribution<std::mt19937::result_type> dist6(0,possibleMoves.size()-1); // distribution in range [1, 6]
    	int rand = 0;//dist6(rng);
    	//Move * move = possibleMoves[rand];
    	char m = 'M';
    	//cout << move->bomb << " ";
    	// if(move->bomb)
    	// 	m = 'B';
    	// else
    	// 	m = 'M';
    	// int x_i = move->i%M, y_i = move->i/M, x_f = move->f%M, y_f  = move->f/M;
    	// cout << "S " << x_i << " " << y_i << " " << m << " " << x_f << " " << y_f << "\n";
    	// cout << move->i << move->f ;
    	// state->doMove(move->i, move->f, m, parityId);

    	cin >> doMove;
    	std::vector<string> instr = split(doMove, ' ');
    	cout << instr.size();
    	// if(instr.size()!= 6){
    	// 	cout << "Invalid input";
    	// 	break;
    	// }        
     //    int x_i = stoi(instr[1]), y_i = stoi(instr[2]), x_f = stoi(instr[4]), y_f = stoi(instr[5]);
       
    	//state->doMove(x_i, y_i, x_f, y_f, instr[3][0], -1*parityId);
    	break;
    }

	// cin>>id;
	// cin>>N;
	// cin>>M;
	// cin>>time_limit;
	//id = 1-id*2;
	// int State::N = n;
	// int State::M = m;
	// int State::LIMIT = n*m;
	// int Cannon::N = n;
	// int Cannon::M = m;
	// int Cannon::LIMIT = n*m;
	// while(true)
	// {
	// 	break;
	// }
	return 0;
}

vector<string> split(string s, char c){
    vector<string> result;
    string temp = "";
    for(int i =0 ; i< s.size(); i++)
    {
    	if(s[i] == c)
    	{
            result.push_back(temp);
            temp = "";
            continue;
        }
        temp += s[i];
        cout << "1 "<< temp<< "\n";
        
    }
    return result;
}