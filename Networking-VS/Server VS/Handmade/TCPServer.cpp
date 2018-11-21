#include "TCPServer.h"

TCPServer::TCPServer()
{
}

bool TCPServer::Listen()
{
	TCPsocket tempSocket = nullptr;

	tempSocket = SDLNet_TCP_Accept(m_socket);

	if (!tempSocket)
	{
		std::cout << "Cannot connect with client." << std::endl << std::endl;
		SDL_Delay(500);
		return false;
	}

	m_clientSocket = tempSocket;
	
	std::cout << "Client connected via server." << std::endl;

	return true;
}

bool TCPServer::SendMessage(const std::string& message)
{
	int length = message.length() + 1;

	if (SDLNet_TCP_Send(m_clientSocket, message.c_str(), length) < length)
	{
		std::cout << "Error sending message!" << std::endl;
		system("pause");
		return false;
	}

	return true;
}

bool TCPServer::ReceiveMessage(std::string& response)
{
	char temp_response[2000];

	if (SDLNet_TCP_Recv(m_clientSocket, temp_response, 2000) <= 0)
	{
		std::cout << "Error receiving message!" << std::endl;
		system("pause");
		return false;
	}

	response = temp_response;

	return true;
}

void TCPServer::Close()
{
	SDLNet_TCP_Close(m_clientSocket);
}