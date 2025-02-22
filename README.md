# Hraezlyr
Bot for Cannon, an abstract strategy board game and a simulator supporting a user interface for the game.

## Rules
The rules of the original game can be found [here](https://nestorgames.com/rulebooks/CANNON_EN.pdf)

### Rule Clarifications:  
+ The soldier can retreat back if and only if it is adjacent to an enemy soldier.
+ The soldier can kill an enemy soldier or a townhall in a retreat step.
+ Cannon shifts along its length cannot capture a soldier or a townhall.
+ Blank cannon shots are allowed (take care of stagnant game) and are considered valid moves.


### Stalemate
Case 1: Player A (or B) kills the last soldier of Player B (or A).  
Case 2: Both Players have soldiers left, but Player B (or A) has no immediate moves to play

### Stagnant Game
If exactly the same board position is repeated thrice after the move of a player then the player is said to have forced a  Stagnant Game.

## Dependencies
+ Python2.7
+ Chrome Webdriver
+ Jinja2
+ Multiset
+ Numpy
+ Selenium

```sh
pip install -r requirements.txt
```

Download the chrome driver executable according to your chrome version from the following link:
https://chromedriver.chromium.org/downloads

You can check you chrome version following the steps below:
- Launch Google Chrome.
- Click on the icon in the upper right corner that looks like 3 short bars.
- Select About Google Chrome to display the version number.

## Main Files
+ `game.py` - This has an instance of the game. It can be run locally to hand play a game of Cannon or re-play a recorded game. Should be run in `GUI` mode to make game board visible.
+ `RandomPlayer.py` - This is an implementation of a random bot. It is a good place to start understanding the flow of the game and the various game states.
+ `client.py` - This will encapsulate your process and help it connect to the game server.
  > `ip` (mandatory) - The Server IP.  
  > `port` (mandatory) - The Server Port.  
  > `exe` (mandatory) - The Executable.  
  > `mode` (optional) - The View Mode ('GUI' / 'CUI'). Default: 'CUI'  
+ `server.py` - This connects the clients and manages the transfer of information.
  > `port` (mandatory) - The Server Port.  
  > `ip` (optional) - The Server IP. Default: 0.0.0.0   
  > `n` (optional) - The Board Row Size. Default: 8  
  > `m` (optional) - The Board Column Size. Default: 8  
  > `NC` (optional) - Number of Clients. Default: 2  
  > `TL` (optional) - Time Limit. Default:150  
  > `LOG` (optional) - The Log File.  

## Run Instructions
Here are the sample instructions used to match two random players against each other over the server network. The .py file can be replaced by the run.sh bash file which runs the C++ or java executable.

### Setup Server
```sh
python server.py 10000 -n 8 -m 8 -NC 2 -TL 150 -LOG server.log
```
### Setup Client 1
```sh
export PATH=$PATH:'/home/chrome_driver_directory_path'
python client.py 0.0.0.0 10000 RandomPlayer.py -mode GUI
```

### Setup Client 2
```sh
export PATH=$PATH:'/home/chrome_driver_directory_path'
python client.py 0.0.0.0 10000 RandomPlayer.py
```

## Gameplay
The game play consists of the players executing a sequence of moves in a single turn.
A move is a triple: `type` `x` `y`.  

### Movetype
+ `S` - Select a soldier
+ `M` - Move a soldier
+ `B` - Throw a Bomb

### Board Settings
The board is an **n x m** board.
The top-left corner point is the origin.
The horizontal direction towards the right is the positive x-axis.
The vertical direction towards down is the positive y-axis.
The indexing begins with 0 in both the directions.

#### Move a Soldier
To move a soldier from (1, 2) to (2, 4).  
`S 1 2 M 2 4`

#### Throw a Bomb
To throw a bomb, select any of the soldiers of a cannon, and throw it at any viable target of the cannon(s).  
`S 2 4 B 6 4`

### Replay
A server.log file is created during the gameplay that consist of the moves played in the game. You can simulate/re-run it using the following command:
```sh
python game.py server.log
```

## Scoring
At the end of a game both players will be given a score. The scoring for all the three configurations of the game will be as follows: 

### The Town Hall Margin
This score will be based on the extent of victory. It is calculated as follows:  

|Town Halls killed by A	|Town Halls killed by B	|Town Hall Margin Score: A	|Town Hall Margin Score: B|
| ------------- | ------------- | ------------- | ------------- | 
| 2 | 0 | 10 | 0 |
| 2 | 1 | 8  | 2 |

#### Stalemate

**Case 1**: Player A kills the last soldier of Player B.

|Town Halls killed by A	|Town Halls killed by B	|Town Hall Margin Score: A	|Town Hall Margin Score: B|
| ------------- | ------------- | ------------- | ------------- | 
| 1 |	0 |	10 |	0 |
| 0 |	0 |	8  |	2 |
| 1 |	1 |	8  |	2 |
| 0 |	1 |	6  |	4 |

**Case 2**: Both Players have soldiers left, but Player B has no immediate moves to play

|Town Halls killed by A	|Town Halls killed by B	|Town Hall Margin Score: A	|Town Hall Margin Score: B|
| ------------- | ------------- | ------------- | ------------- | 
| 1 |	0 |	8 |	2 |
| 0 |	0 |	6 |	4 |
| 1 |	1 |	6 |	4 |
| 0 |	1 |	4 |	6 |

#### Stagnant Game

Player A forced a Stagnant Game.

|Town Halls killed by A	|Town Halls killed by B	|Town Hall Margin Score: A	|Town Hall Margin Score: B|
| ------------- | ------------- | ------------- | ------------- | 
| 1 |	0 |	7 |	3 |
| 0 |	0 |	5 |	5 |
| 1 |	1 |	5 |	5 |
| 0 |	1 |	3 |	7 |

#### Timeout or Invalid Move

**Note:** In case a player suffers a TIMEOUT or INVALID move, it will automatically lose the game and it will count as a (2 - *x*) defeat towards the player and a (*x* - 2) win for the opponent, where *x* is the number of Town Halls killed by the opponent. 

### The Army Margin
This score directly depends on the number of soldiers you have left at the end of the game. It is calculated as follows:  
```Army Margin Score = # (Soldiers Remaining) / 100```

### Final Score
The final score is simply: ```(Town Hall Margin Score).(Army Margin Score)```
Example. Assume the following:  
Player 1 killed 2 Town Halls and has 12 soldiers left on the board.  
Player 2 killed 1 Town Hall and has 9 soldiers left on the board.  
Player 1 score will be: **8.12**  
Player 2 score will be: **2.09**  
