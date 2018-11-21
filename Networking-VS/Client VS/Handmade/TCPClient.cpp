#include "TCPClient.h"

bool TCPClient::SendMessage(const std::string& message)
{
	int length = message.length() + 1;

	if (SDLNet_TCP_Send(m_socket, message.c_str(), length) < length)
	{
		std::cout << "Error sending message!" << std::endl;
		system("pause");
		return false;
	}

	return true;
}

bool TCPClient::ReceiveMessage(std::string& response)
{
	char temp_response[2000];

	if (SDLNet_TCP_Recv(m_socket, temp_response, 2000) <= 0)
	{
		std::cout << "Error receiving message!" << std::endl;
		system("pause");
		return false;
	}

	response = temp_response;

	return true;
}
