#include "Client.hpp"
#include "NibblerException.hpp"
#include "message.hpp"
#include <iostream>

Client::Client(std::string & ipAddress, unsigned short port)
{
    sf::Socket::Status	status;

    std::cout << "[Client] Attempting to connect to server at " << ipAddress << ":" << port << " ..." << std::endl;

    status = this->_socket.connect(ipAddress, port);
    if (status != sf::Socket::Done)
    	throw NibblerException("sf::TcpSocket::connect() failed");

    std::cout << "[Client] Connection successful!" << std::endl;
}

Client::~Client(void)
{
	this->_socket.disconnect();
}

void		Client::getBoardDimensions(int & boardWidth, int & boardHeight)
{
	sf::Packet	packet;
	sf::Int8	messageHeader;
	sf::Int32	width;
	sf::Int32	height;

	this->_socket.receive(packet);

	if (!((packet >> messageHeader) &&
		(messageHeader == BOARD_DIMENSIONS) &&
		(packet >> width >> height)))
		throw NibblerException("Client::getBoardDimensions(): invalid packet data received\n");

	boardWidth = static_cast<int>(width);
	boardHeight = static_cast<int>(height);
}
