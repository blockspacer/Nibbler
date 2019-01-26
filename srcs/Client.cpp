#include "Client.hpp"
#include "Nibbler.hpp"
#include "NibblerException.hpp"
#include "message.hpp"
#include <iostream>

Client::Client(Nibbler & nibbler, std::string & ipAddress, unsigned short port) : ANetworkEntity(nibbler)
{
	std::cout << "[Client] Attempting to connect to server at " << ipAddress << ":" << port << " ..." << std::endl;

	if (this->_socket.connect(ipAddress, port) != sf::Socket::Done)
		throw NibblerException("sf::TcpSocket::connect() failed");

	std::cout << "[Client] Connection successful!" << std::endl;
}

Client::~Client(void)
{
	printf("~Client() \n");
	this->_socket.disconnect();
}

void		Client::receiveBoardInfo(int & boardWidth, int & boardHeight)
{
	sf::Packet	packet;
	sf::Socket::Status status;

	sf::Int8	messageHeader_;
	sf::Int32	width_;
	sf::Int32	height_;

	e_message	messageHeader;
	

	// printf("  BEFORE socket.receive(packet) == DONE \n");
	while ((status = this->_socket.receive(packet)) != sf::Socket::Done)
	{
		// std::cout << "status = " << status << std::endl;
		;
	}
	// printf("  AFTER socket.receive(packet) == DONE \n");


	if (!(packet >> messageHeader_ >> width_ >> height_))
	{
		// error, 🤷
		printf(" packet >> FAILED \n");
	}

	messageHeader = static_cast<e_message>(messageHeader_);

	if (messageHeader != BOARD_INFO)
	{
		// error, 🤷
		printf(" messageHeader != BOARD_INFO \n");
	}


	boardWidth = static_cast<int>(width_);
	boardHeight = static_cast<int>(height_);

	// std::cout << "messageHeader = " << messageHeader << std::endl;
	// std::cout << "boardWidth = " << boardWidth << " END" << std::endl;
	// std::cout << "boardHeight = " << boardHeight << " END" << std::endl;

}

void		Client::receiveSnakeSpawnInfo(int & playerID, int & posX, int & posY, e_direction & direction)
{
	sf::Packet	packet;
	sf::Socket::Status status;

	sf::Int8	messageHeader_;
	sf::Int8	playerID_;
	sf::Int32	posX_;
	sf::Int32	posY_;
	sf::Int8	direction_;

	e_message	messageHeader;
	

	// printf("  BEFORE socket.receive(packet) == DONE \n");
	while ((status = this->_socket.receive(packet)) != sf::Socket::Done)
	{
		// std::cout << "status = " << status << std::endl;
		;
	}
	// printf("  AFTER socket.receive(packet) == DONE \n");


	if (!(packet >> messageHeader_ >> playerID_ >> posX_ >> posY_ >> direction_))
	{
		// error, 🤷
		printf(" packet >> FAILED \n");
	}

	messageHeader = static_cast<e_message>(messageHeader_);

	if (messageHeader != SNAKE_SPAWNED)
	{
		// error, 🤷
		printf(" messageHeader != SNAKE_SPAWNED \n");
	}

	playerID = static_cast<int>(playerID_);
	posX = static_cast<int>(posX_);
	posY = static_cast<int>(posY_);
	direction = static_cast<e_direction>(direction_);

	// std::cout << "messageHeader = " << messageHeader << std::endl;
	// std::cout << "playerID = " << playerID << std::endl;
	// std::cout << "posX = " << posX << std::endl;
	// std::cout << "posY = " << posY << std::endl;
	// std::cout << "direction = " << direction << std::endl;
}




































