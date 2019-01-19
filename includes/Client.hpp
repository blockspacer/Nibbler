#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <string>
# include <SFML/Network.hpp>

class Client
{
public:

	Client(std::string & ipAddress, unsigned short port);
	~Client(void);

	void			getBoardDimensions(int & boardWidth, int & boardHeight);

private:

	sf::TcpSocket	_socket;

	Client(void);
	Client(const Client & src);
	Client & operator=(const Client & rhs);

};

#endif
