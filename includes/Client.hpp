#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "ANetworkEntity.hpp"
# include "direction.hpp"

class Nibbler;

class Client : public ANetworkEntity
{
public:

	Client(Nibbler & nibbler, std::string & ipAddress, unsigned short port);
	~Client(void);

	void			receiveBoardInfo(int & boardWidth, int & boardHeight);
	void			receiveSnakeSpawnInfo(int & playerID, int & posX, int & posY, e_direction & direction);

private:

	Client(void);
	Client(const Client & src);
	Client & operator=(const Client & rhs);

};

#endif
