#ifndef NIBBLER_HPP
# define NIBBLER_HPP

# include "Player.hpp"
# include <string>
# include <iostream>
# include <vector>

# define MAX_PLAYERS	2
# define N_MODULES		3
# define LENGTH_TO_WIN	10

# define MAX_BOARD_LEN	50

class IModule;
class SDLModule;
class SFMLModule;
class Board;
class Snake;
class Enemy;
class SnakeCell;
class Server;
class Client;

class Nibbler
{
public:

	Nibbler(int boardWidth, int boardHeight, int numPlayers);		// local game constructor
	Nibbler(int boardWidth, int boardHeight, unsigned short port);	// online server constructor
	Nibbler(std::string & ipAddress, int port);						// online client constructor
	
	~Nibbler(void);

	void			terminate(void);
	void			selectNextModule(void);		// maybe get rid of this

	void			selectModule1(void);
	void			selectModule2(void);
	void			selectModule3(void);


	void			debugBoard(void);
	void			debugSnake(void);

	void			turnLeftP1(void);
	void			turnRightP1(void);

	void			turnLeftP2(void);
	void			turnRightP2(void);

	void			turnLeftP1_Online(void);
	void			turnRightP1_Online(void);

	void			turnLeftP2_Online(void);
	void			turnRightP2_Online(void);



	void			startNewRound(void);

	SnakeCell &		getPlayer1SnakeHeadCell(void) const;


	void			_update(void);			// move back to private later

private:

	int				_numPlayers;

	bool			_exitProgram;
	bool			_isRoundOver;
	int				_roundNumber;
	std::string		_winner;

	Board *			_board;

	IModule *		_modules[N_MODULES];
	int				_moduleIndex;

	Player								_players[MAX_PLAYERS];
	std::vector<std::unique_ptr<Snake>>	_snakes;
	std::vector<std::shared_ptr<Enemy>>	_enemies;

	Server *		_server;
	Client *		_client;

	Nibbler(void);
	Nibbler(const Nibbler & src);
	Nibbler & operator=(const Nibbler & rhs);

	void			_validateArgs(int boardWidth, int boardHeight, int numPlayers);

	void			_initModules(void);
	void			_toggleModules(int nextIndex);

	Snake *			_initSnakeFor1PGame(int playerIndex);
	Snake *			_initSnakeFor2PGame(int playerIndex);



	void			_loop(void);
	void			_handleEvents(void);
	// void			_update(void);
	void			_render(void);
	void			_displayGameStatus(void);

	void			_spawnEnemies(void);

	void			_checkIfRoundIsOver(void);



};

#endif
