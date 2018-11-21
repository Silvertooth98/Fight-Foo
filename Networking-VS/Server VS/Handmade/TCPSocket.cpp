#include "TCPSocket.h"

TCPSocket::TCPSocket()
{
}

bool TCPSocket::Init(Uint16 port, const char * ipAddress)
{
	if(SDLNet_Init() == -1)
	{
		std::cout << "Error with SDL Net!" << std::endl;
		system("pause");
		return false;
	}

	std::cout << "SDL fine" << std::endl;

	//Setup the server
	if (SDLNet_ResolveHost(&m_IP, NULL, 1234) == -1)
	{
		std::cout << "Error setting up the server" << std::endl;
		return false;
	}

	std::cout << "Server setup" << std::endl;

	return true;
}

bool TCPSocket::Open()
{
	m_socket = SDLNet_TCP_Open(&m_IP);

	if (!m_socket)
	{
		std::cout << "Error opening the socket" << std::endl;
		system("pause");
		return false;
	}

	std::cout << "Socket open. Waiting for clients. ";

	return true;
}

void TCPSocket::Close()
{
	SDLNet_TCP_Close(m_socket);
}

void TCPSocket::ShutDown()
{
	SDLNet_TCP_Close(m_socket); //Close the socket we opened
	SDLNet_Quit(); //Close both SDL and SDLNets
}