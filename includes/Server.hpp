#ifndef SERVER_HPP
# define SERVER_HPP

# include "ANetworkEntity.hpp"

class Nibbler;
class Board;
class Snake;

class Server : public ANetworkEntity
{
public:
	
	Server(Nibbler & nibbler, unsigned short port);
	~Server(void);

	void				sendBoardInfo(Board & board);
	void				sendSnakeSpawnInfo(int playerID, Snake & snake);

private:

	sf::TcpListener		_listener;

	Server(void);
	Server(const Server & src);
	Server & operator=(const Server & rhs);

};

#endif
