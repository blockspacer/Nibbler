#include "Nibbler.hpp"
#include "NibblerException.hpp"
#include "Board.hpp"
#include "IModule.hpp"
#include "SDLModule.hpp"
#include "SFMLModule.hpp"
#include "OpenGLModule.hpp"
#include "Snake.hpp"
#include "direction.hpp"
#include "AudioManager.hpp"
#include "Enemy.hpp"
#include "Server.hpp"
#include "Client.hpp"

#include <chrono>





e_direction		rand_direction(void)
{
	return (static_cast<e_direction>(std::rand() % 4));
}

SnakeCell &		Nibbler::getPlayer1SnakeHeadCell(void) const
{
	return (this->_snakes[0]->getHeadCell());
}

/* Local Game Constructor */
Nibbler::Nibbler(int boardWidth, int boardHeight, int numPlayers) :
	_numPlayers(numPlayers), _exitProgram(false),  _isRoundOver(false), _roundNumber(0)
{
	printf("Nibbler() local constructor\n");

	std::srand(std::time(0));

	this->_server = nullptr;
	this->_client = nullptr;
	
	this->_validateArgs(boardWidth, boardHeight, numPlayers);
	this->_board = new Board(boardWidth, boardHeight);
	this->_snakes.resize(this->_numPlayers);

	this->startNewRound();

	this->_initModules();
	this->_loop();
}

/* Online 2P Server Constructor */
Nibbler::Nibbler(int boardWidth, int boardHeight, unsigned short port) :
	_numPlayers(2), _exitProgram(false), _isRoundOver(false), _roundNumber(0)
{
	printf("Nibbler() server constructor\n");

	std::srand(std::time(0));

	this->_validateArgs(boardWidth, boardHeight, 2);

	this->_server = new Server(*this, port);
	this->_client = nullptr;

	this->_board = new Board(boardWidth, boardHeight);
	this->_server->sendBoardInfo(*this->_board);


	this->_snakes.resize(2);

	this->startNewRound();

	this->_initModules();
	this->_loop();

	
}

/* Online 2P Client Constructor */
Nibbler::Nibbler(std::string & ipAddress, int port) :
	_numPlayers(2), _exitProgram(false), _isRoundOver(false), _roundNumber(0)
{
	printf("Nibbler() client constructor\n");

	std::srand(std::time(0));

	this->_server = nullptr;
	this->_client = new Client(*this, ipAddress, port);

	int boardWidth, boardHeight;
	this->_client->receiveBoardInfo(boardWidth, boardHeight);
	this->_board = new Board(boardWidth, boardHeight);

	this->_snakes.resize(2);

	this->startNewRound();

	this->_initModules();
	this->_loop();
	
}

void		Nibbler::_validateArgs(int boardWidth, int boardHeight, int numPlayers)
{
	// numPlayers must be 1 or 2
	if (!(1 <= numPlayers && numPlayers <= MAX_PLAYERS))
		throw NibblerException("Invalid number of players (currently support 1 xor 2 players)");

	// 1P Game: board size must be at least 7x7
	if (numPlayers == 1)
	{
		if (!(7 <= boardWidth && boardWidth <= MAX_BOARD_LEN &&
			7 <= boardHeight && boardHeight <= MAX_BOARD_LEN))
		throw NibblerException("Invalid board size (1P board min size: 7x7, max size: 50x50)");
	}
	// 2P Game: board size must be at least 14x14
	else
	{
		if (!(14 <= boardWidth && boardWidth <= MAX_BOARD_LEN &&
			14 <= boardHeight && boardHeight <= MAX_BOARD_LEN))
		throw NibblerException("Invalid board size (2P board min size: 14x14, max size: 50x50)");
	}
}

void		Nibbler::_initModules(void)
{
	this->_modules[0] = new SFMLModule(*this, *this->_board);
	this->_modules[0]->disable();

	this->_modules[1] = new SDLModule(*this, *this->_board);
	this->_modules[1]->disable();

	this->_modules[2] = new OpenGLModule(*this, *this->_board);
	this->_modules[2]->disable();

	this->_moduleIndex = 0;
	this->_modules[this->_moduleIndex]->enable();
}

void		Nibbler::startNewRound(void)
{
	this->_isRoundOver = false;
	this->_roundNumber++;
	this->_board->clearAllCells();
	this->_enemies.clear();

	// spawn snakes as server or local game
	if (this->_client == nullptr)
	{
		for (int i = 0; i < this->_numPlayers; i++)
		{
			if (this->_numPlayers == 1)
				this->_snakes[i] = std::unique_ptr<Snake>(this->_initSnakeFor1PGame(i));
			else
				this->_snakes[i] = std::unique_ptr<Snake>(this->_initSnakeFor2PGame(i));
		}
	}

	// server sends snake spawn info
	if (this->_server != nullptr)
	{
		printf("Nibbler server startNewRound()\n");

		this->_server->sendSnakeSpawnInfo(0, *this->_snakes[0]);
		this->_server->sendSnakeSpawnInfo(1, *this->_snakes[1]);
	}

	// client receives snake spawn info
	if (this->_client != nullptr)
	{
		int			playerIndex;
		int			posX;
		int			posY;
		e_direction	direction;

		printf("Nibbler client startNewRound()\n");

		this->_client->receiveSnakeSpawnInfo(playerIndex, posX, posY, direction);
		this->_snakes[0] = std::unique_ptr<Snake>(new Snake(this->_players[playerIndex], *this->_board, posX, posY, direction));

		this->_client->receiveSnakeSpawnInfo(playerIndex, posX, posY, direction);
		this->_snakes[1] = std::unique_ptr<Snake>(new Snake(this->_players[playerIndex], *this->_board, posX, posY, direction));
	}

	
	// this->_board->generateFood();
	AudioManager::getInstance().playBGM();
}

// need at least 7x7 board
Snake *		Nibbler::_initSnakeFor1PGame(int playerIndex)
{
	int			xCenter = this->_board->getWidth() / 2;
	int			yCenter = this->_board->getHeight() / 2;
	Player &	player = this->_players[playerIndex];

	return (new Snake(player, *this->_board, xCenter, yCenter, rand_direction()));
}

// need at least 14 x 14 board
Snake *		Nibbler::_initSnakeFor2PGame(int playerIndex)
{
	int			xCenter;
	int			yCenter;
	Player &	player = this->_players[playerIndex];

	// P1 snake starts at upper left corner
	if (playerIndex == 0)
	{
		xCenter = this->_board->getWidth() / 4;
		yCenter = this->_board->getHeight() / 4;
	}
	// P2 snake starts at bottom right corner
	else
	{
		xCenter = this->_board->getWidth() / 4 * 3;
		yCenter = this->_board->getHeight() / 4 * 3;
	}
	return (new Snake(player, *this->_board, xCenter, yCenter, rand_direction()));
}

Nibbler::~Nibbler(void)
{
	delete this->_server;
	delete this->_client;

	delete this->_board;
	for (int i = 0; i < N_MODULES; i++)
		delete this->_modules[i];
}

# define MS_PER_UPDATE 200

void		Nibbler::_loop(void)
{
	double t = 0.0;
	std::chrono::time_point<std::chrono::high_resolution_clock> previousTime = std::chrono::high_resolution_clock::now();

	while (!this->_exitProgram)
	{
		std::chrono::time_point<std::chrono::high_resolution_clock> currentTime = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> elapsedTime = currentTime - previousTime;
		std::chrono::milliseconds ms = std::chrono::duration_cast<std::chrono::milliseconds>(elapsedTime);
		t += ms.count();
		previousTime = currentTime;

		this->_handleEvents();

		if (this->_client != nullptr)
			this->_client->receiveMessages();
		if (this->_server != nullptr)
			this->_server->receiveMessages();


		if (t >= MS_PER_UPDATE)
		{
			this->_update();
			t -= MS_PER_UPDATE;
		}
		this->_render();
		// this->_displayGameStatus();
	}
}

void		Nibbler::_handleEvents(void)
{
	this->_modules[this->_moduleIndex]->handleEvents();
}

void		Nibbler::_update(void)
{
	if (this->_isRoundOver)
		return;

	for (int i = 0; i < this->_numPlayers; i++)
		this->_snakes[i]->update();

	this->_checkIfRoundIsOver();
	if (this->_isRoundOver)
		return;

	for (size_t i = 0; i < this->_enemies.size(); i++)
		this->_enemies[i]->update();

	// remove all dead enemies
	this->_enemies.erase(
		std::remove_if(this->_enemies.begin(), this->_enemies.end(), [](std::shared_ptr<Enemy> & enemy){ return (enemy->isDead()); }),
		this->_enemies.end());

	this->_spawnEnemies();

	this->_checkIfRoundIsOver();
}

// # define	MAX_ENEMIES 5

# define	SPAWN_RATE		5

void		Nibbler::_spawnEnemies(void)
{
	// spawn enemies that go WEST
	if (std::rand() % 100 < SPAWN_RATE)
	{
		int		x = this->_board->getWidth() - 1;
		int		y = std::rand() % this->_board->getHeight();

		if (this->_board->isEmptyCell(x, y))
			this->_enemies.push_back(std::make_shared<Enemy>(x, y, WEST, *this->_board));
	}

	// spawn enemies that go EAST
	if (std::rand() % 100 < SPAWN_RATE)
	{
		int		x = 0;
		int		y = std::rand() % this->_board->getHeight();

		if (this->_board->isEmptyCell(x, y))
			this->_enemies.push_back(std::make_shared<Enemy>(x, y, EAST, *this->_board));
	}

	// spawn enemies that go SOUTH
	if (std::rand() % 100 < SPAWN_RATE)
	{
		int		x = std::rand() % this->_board->getWidth();
		int		y = 0;

		if (this->_board->isEmptyCell(x, y))
			this->_enemies.push_back(std::make_shared<Enemy>(x, y, SOUTH, *this->_board));
	}

	// spawn enemies that go NORTH
	if (std::rand() % 100 < SPAWN_RATE)
	{
		int		x = std::rand() % this->_board->getWidth();
		int		y = this->_board->getHeight() - 1;

		if (this->_board->isEmptyCell(x, y))
			this->_enemies.push_back(std::make_shared<Enemy>(x, y, NORTH, *this->_board));
	}
}



void		Nibbler::_render(void)
{
	this->_modules[this->_moduleIndex]->render();
}

void		Nibbler::_checkIfRoundIsOver(void)
{
	// 1P Game
	if (this->_numPlayers == 1)
	{
		// P1 dieded
		if (this->_snakes[0]->isDead())
		{
			this->_isRoundOver = true;
			this->_winner = "Nobody";
			AudioManager::getInstance().playSFX("1p_lose");
		}
		// P1 won
		else if (this->_snakes[0]->getLength() >= LENGTH_TO_WIN)
		{
			this->_isRoundOver = true;
			this->_winner = "Player 1";
			AudioManager::getInstance().playSFX("1p_win");
			this->_players[0].incrementWinCount();
		}
	}
	// 2P Game
	else
	{
		// All dieded
		if (this->_snakes[0]->isDead() && this->_snakes[1]->isDead())
		{
			this->_isRoundOver = true;
			this->_winner = "Nobody";
			AudioManager::getInstance().playSFX("2p_all_lose");
		}
		// P1 won
		else if (this->_snakes[1]->isDead() || this->_snakes[0]->getLength() >= LENGTH_TO_WIN)
		{
			this->_isRoundOver = true;
			this->_winner = "Player 1";
			AudioManager::getInstance().playSFX("2p_p1_win");
			this->_players[0].incrementWinCount();
		}
		// P2 won
		else if (this->_snakes[0]->isDead()  || this->_snakes[1]->getLength() >= LENGTH_TO_WIN)
		{
			this->_isRoundOver = true;
			this->_winner = "Player 2";
			AudioManager::getInstance().playSFX("2p_p2_win");
			this->_players[1].incrementWinCount();
		}
	}
}



void		Nibbler::_displayGameStatus(void)
{
	system("clear");

	printf("Win Condition: First to length %d wins the round!\n\n", LENGTH_TO_WIN);
	printf("Round %d\n", this->_roundNumber);
	printf("%9s %12s  %12s  %12s  %12s\n", "", "Status", "Length", "Score", "Wins");
	for (int i = 0; i < this->_numPlayers; i++)
	{
		Player & player = this->_players[i];
		std::string status = this->_snakes[i]->isDead() ? "Dead" : "Alive";
		printf("Player %d: %12s  %12d  %12d  %12d\n",
			i + 1,
			status.c_str(),
			this->_snakes[i]->getLength(),
			player.getScore(),
			player.getWinCount());
	}
	if (this->_isRoundOver)
		printf("\nWinner: %s\n", this->_winner.c_str());
}

void		Nibbler::terminate(void)
{
	this->_exitProgram = true;
}

void		Nibbler::selectNextModule(void)
{
	this->_modules[this->_moduleIndex]->disable();
	this->_moduleIndex = (this->_moduleIndex + 1) % N_MODULES;
	this->_modules[this->_moduleIndex]->enable();
}

void		Nibbler::debugBoard(void)
{
	this->_board->debug();
}

void		Nibbler::debugSnake(void)
{
	this->_snakes[0]->debug();
}

void		Nibbler::turnLeftP1(void)
{
	if (this->_client == nullptr)
		this->_snakes[0]->turnLeft();

	if (this->_server != nullptr)
		this->_server->sendMessage(P1_TURN_LEFT);
}

void		Nibbler::turnRightP1(void)
{
	if (this->_client == nullptr)
		this->_snakes[0]->turnRight();

	if (this->_server != nullptr)
		this->_server->sendMessage(P1_TURN_RIGHT);
}

void		Nibbler::turnLeftP2(void)
{
	if (this->_numPlayers == 2 && this->_server == nullptr)
		this->_snakes[1]->turnLeft();

	if (this->_client != nullptr)
		this->_client->sendMessage(P2_TURN_LEFT);
}

void		Nibbler::turnRightP2(void)
{
	if (this->_numPlayers == 2 && this->_server == nullptr)
		this->_snakes[1]->turnRight();

	if (this->_client != nullptr)
		this->_client->sendMessage(P2_TURN_RIGHT);
}

void		Nibbler::selectModule1(void)
{
	this->_toggleModules(0);
}

void		Nibbler::selectModule2(void)
{
	this->_toggleModules(1);
}

void		Nibbler::selectModule3(void)
{
	this->_toggleModules(2);
}

void		Nibbler::_toggleModules(int nextIndex)
{
	if (this->_moduleIndex != nextIndex)
	{
		for (int i = 0; i < N_MODULES; i++)
			this->_modules[i]->disable();

		this->_moduleIndex = nextIndex;
		this->_modules[this->_moduleIndex]->enable();
	}
}







