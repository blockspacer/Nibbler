#include "Server.hpp"
#include "Nibbler.hpp"
#include "NibblerException.hpp"
#include "message.hpp"
#include "Board.hpp"
#include "Snake.hpp"
#include "SnakeCell.hpp"

#include <iostream>

Server::Server(unsigned short port) : ANetworkEntity()
{
	if (this->_listener.listen(port) != sf::Socket::Done)
		throw NibblerException("sf::TcpListener::listen() failed");

	std::cout << "[Server] Waiting for connection..." << std::endl;

	if (this->_listener.accept(this->_socket) != sf::Socket::Done)
		throw NibblerException("sf::TcpSocket::accept() failed");	

	std::cout << "[Server] Connection successful!" << std::endl;

	this->_listener.close();
	this->_socket.setBlocking(false);
}

Server::~Server(void)
{
	printf("~Server() \n");
	this->_socket.disconnect();
}

void			Server::sendBoardInfo(Board & board)
{
	sf::Packet	packet;
	
	packet << sf::Int8(BOARD_INFO)
		<< sf::Int32(board.getWidth())
		<< sf::Int32(board.getHeight());
	this->_socket.send(packet);
}

void			Server::sendSnakeSpawnInfo(int playerID, Snake & snake)
{
	printf("Server::sendSnakeSpawnInfo()\n");

	sf::Packet	packet;
	SnakeCell &	snakeHeadCell = snake.getHeadCell();

	packet << sf::Int8(SNAKE_SPAWNED)
		<< sf::Int8(playerID)
		<< sf::Int32(snakeHeadCell.getX())
		<< sf::Int32(snakeHeadCell.getY())
		<< sf::Int8(snakeHeadCell.getDirection());
	this->_socket.send(packet);

	printf("Server::sendSnakeSpawnInfo(): id=%d, posX=%d, posY=%d, direction=%d\n",
		playerID, snakeHeadCell.getX(), snakeHeadCell.getX(), snakeHeadCell.getDirection());
}


