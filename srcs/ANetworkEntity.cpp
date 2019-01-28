#include "ANetworkEntity.hpp"
#include "Nibbler.hpp"

ANetworkEntity::ANetworkEntity(void) { }

ANetworkEntity::~ANetworkEntity(void) { }

void			ANetworkEntity::sendMessage(e_message message)
{
	sf::Packet	packet;
	
	packet << sf::Int8(message);
	this->_socket.send(packet);
}
