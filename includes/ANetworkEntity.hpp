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

	sf::TcpSocket	_socket;

	ANetworkEntity(void);

private:

	ANetworkEntity(const ANetworkEntity & src);
	ANetworkEntity &	operator=(const ANetworkEntity & rhs);

	void			_handleSnakeSpawnInfo(sf::Packet & packet);

};

#endif
