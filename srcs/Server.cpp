#include "Server.hpp"
#include "NibblerException.hpp"
#include "message.hpp"
#include <iostream>

Server::Server(unsigned short port)
{
	if (this->_listener.listen(port) != sf::Socket::Done)
	{
		throw NibblerException("sf::TcpListener::listen() failed");
	}

	std::cout << "[Server] Waiting for connection..." << std::endl;

	if (this->_listener.accept(this->_client) != sf::Socket::Done)
	{
		throw NibblerException("sf::TcpSocket::accept() failed");	
	}

	std::cout << "[Server] Connection successful!" << std::endl;
}

Server::~Server(void)
{
	this->_client.disconnect();
	this->_listener.close();
}

void			Server::sendBoardDimensions(int boardWidth, int boardHeight)
{
	sf::Packet	packet;
	
	packet << sf::Int8(BOARD_DIMENSIONS) << sf::Int32(boardWidth) << sf::Int32(boardHeight);
	this->_client.send(packet);
}
