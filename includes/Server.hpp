#ifndef SERVER_HPP
# define SERVER_HPP

# include <SFML/Network.hpp>

class Server
{
public:
	
	Server(unsigned short port);
	~Server(void);

	void				sendBoardDimensions(int boardWidth, int boardHeight);

private:

	sf::TcpListener		_listener;
	sf::TcpSocket		_client;

	Server(void);
	Server(const Server & src);
	Server & operator=(const Server & rhs);

};

#endif
