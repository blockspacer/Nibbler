#include "ANetworkEntity.hpp"
#include "Nibbler.hpp"

#include <iostream>

ANetworkEntity::ANetworkEntity(void) { }

ANetworkEntity::~ANetworkEntity(void) { }

void			ANetworkEntity::sendMessage(e_message message)
{
	sf::Packet	packet;
	
	packet << sf::Int8(message);
	this->_socket.send(packet);

	// std::cout << "sendMessage(): " << message << std::endl;
}

void			ANetworkEntity::receiveMessages(void)
{
	sf::Packet	packet;
	sf::Socket::Status status;

	sf::Int8	message_;
	e_message	message;

	while ((status = this->_socket.receive(packet)) == sf::Socket::Done)
	{
		if (!(packet >> message_))
		{
			// can probably remove
			printf(" packet >> messageHeader_ FAILED \n");
		}

		message = static_cast<e_message>(message_);

		// std::cout << "receiveMessages(): " << message << std::endl;

		switch (message)
		{
			case P1_TURN_LEFT:
				// std::cout << "receiveMessage(): P1_TURN_LEFT" << std::endl;
				Nibbler::getInstance().turnLeftP1(true);
				break;
			case P1_TURN_RIGHT:
				// std::cout << "receiveMessage(): P1_TURN_RIGHT" << std::endl;
				Nibbler::getInstance().turnRightP1(true);
				break;
			case P2_TURN_LEFT:
				// std::cout << "receiveMessage(): P2_TURN_LEFT" << std::endl;
				Nibbler::getInstance().turnLeftP2(true);
				break;
			case P2_TURN_RIGHT:
				// std::cout << "receiveMessage(): P2_TURN_RIGHT" << std::endl;
				Nibbler::getInstance().turnRightP2(true);
				break;

			case START_NEW_ROUND:
				Nibbler::getInstance().startNewRound(true);
				break;

			case SELECT_MODULE_1:
				Nibbler::getInstance().selectModule1(true);
				break;
			case SELECT_MODULE_2:
				Nibbler::getInstance().selectModule2(true);
				break;
			case SELECT_MODULE_3:
				Nibbler::getInstance().selectModule3(true);
				break;

			case TERMINATE:
				Nibbler::getInstance().terminate(true);
				break;

			case SNAKE_SPAWNED:
				this->_handleSnakeSpawnInfo(packet);
				break;

			default:
				printf(" dafuq is this 🤷 \n");
				break;
		}
	}
}

void				ANetworkEntity::_handleSnakeSpawnInfo(sf::Packet & packet)
{
	sf::Int8		playerID_;
	sf::Int32		posX_;
	sf::Int32		posY_;
	sf::Int8		direction_;

	int				playerID;
	int				posX;
	int				posY;
	e_direction		direction;

	if (!(packet >> playerID_ >> posX_ >> posY_ >> direction_))
	{
		// can probably remove
		printf(" Client::_handleSnakeSpawnInfo() FAILED \n");
	}

	playerID = static_cast<int>(playerID_);
	posX = static_cast<int>(posX_);
	posY = static_cast<int>(posY_);
	direction = static_cast<e_direction>(direction_);

	Nibbler::getInstance().spawnNewSnake(playerID, posX, posY, direction);
}
