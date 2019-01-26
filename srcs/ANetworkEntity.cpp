#include "ANetworkEntity.hpp"
#include "Nibbler.hpp"

ANetworkEntity::ANetworkEntity(Nibbler & nibbler) : _nibbler(nibbler)
{

}

ANetworkEntity::~ANetworkEntity(void) { }

void			ANetworkEntity::sendMessage(e_message message)
{
	sf::Packet	packet;
	
	packet << sf::Int8(message);
	this->_socket.send(packet);
}

void			ANetworkEntity::receiveMessages(void)
{
	sf::Packet	packet;
	sf::Socket::Status status;

	sf::Int8	message_;
	e_message	message;

	// while ((status = this->_socket.receive(packet)) != sf::Socket::Disconnected)
	// {
	// 	if (status == sf::Socket::Done)
	// 	{
	// 		if (!(packet >> message_))
	// 		{
	// 			// error, 🤷
	// 			printf(" packet >> messageHeader_ FAILED \n");
	// 		}

	// 		message = static_cast<e_message>(message_);

	// 		switch (message)
	// 		{
	// 			case P1_TURN_LEFT:
	// 				this->_nibbler.turnLeftP1();
	// 				break;
	// 			case P1_TURN_RIGHT:
	// 				this->_nibbler.turnRightP1();
	// 				break;
	// 			case P2_TURN_LEFT:
	// 				this->_nibbler.turnLeftP2();
	// 				break;
	// 			case P2_TURN_RIGHT:
	// 				this->_nibbler.turnRightP2();
	// 				break;
	// 			default:
	// 				printf(" error 🤷 \n");
	// 				break;
	// 		}
	// 	}
	// }

	if ((status = this->_socket.receive(packet)) == sf::Socket::Done)
	{
		if (!(packet >> message_))
		{
			// error, 🤷
			printf(" packet >> messageHeader_ FAILED \n");
		}

		message = static_cast<e_message>(message_);

		switch (message)
		{
			case P1_TURN_LEFT:
				this->_nibbler.turnLeftP1();
				break;
			case P1_TURN_RIGHT:
				this->_nibbler.turnRightP1();
				break;
			case P2_TURN_LEFT:
				this->_nibbler.turnLeftP2();
				break;
			case P2_TURN_RIGHT:
				this->_nibbler.turnRightP2();
				break;
			default:
				printf(" error 🤷 \n");
				break;
		}
	}
}
