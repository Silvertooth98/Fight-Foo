#include "TCPSocket.h"

TCPSocket::TCPSocket()
{
}

bool TCPSocket::Init(Uint16 port, const char* ipAddress)
{
	if(SDLNet_Init() == -1)
	{
		std::cout << "Error with SDL Net!" << std::endl;
		system("pause");
		return false;
	}

	std::cout << "SDL fine" << std::endl;

	//Setup the server
	if (SDLNet_ResolveHost(&m_IP, ipAddress, port) == -1)
	{
		std::cout << "Error connecting to the server" << std::endl;
		return false;
	}

	std::cout << "Connected to the server = true" << std::endl;

	return true;
}

bool TCPSocket::Open()
{
	m_socket = SDLNet_TCP_Open(&m_IP);

	if (!m_socket)
	{
		std::cout << "Couldn't connect to the server" << std::endl;
		return false;
	}

	std::cout << "Socket - Connected to server." << std::endl;

	return true;
}

void TCPSocket::Close()
{
	SDLNet_TCP_Close(m_socket); //Close the socket we opened
}

void TCPSocket::ShutDown()
{
	//SDLNet_TCP_Close(m_socket); 
	SDLNet_Quit(); //Close both SDL and SDLNets
}