#ifndef A_NETWORK_ENTITY_HPP
# define A_NETWORK_ENTITY_HPP

# include "message.hpp"

# include <string>
# include <SFML/Network.hpp>

class Nibbler;

class ANetworkEntity
{
public:

	virtual ~ANetworkEntity(void);

	void			sendMessage(e_message message);
	void			receiveMessages(void);

protected:

	Nibbler &		_nibbler;
	sf::TcpSocket	_socket;

	ANetworkEntity(Nibbler & nibbler);

private:

	ANetworkEntity(const ANetworkEntity & src);
	ANetworkEntity &	operator=(const ANetworkEntity & rhs);

};

#endif