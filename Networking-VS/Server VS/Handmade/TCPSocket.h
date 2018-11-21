#pragma once

#include <iostream>
#include <string>
#include <SDL.h>
#include <SDL_net.h>

class TCPSocket
{
public:
	TCPSocket();

public:
	bool Init(Uint16 port, const char* ipAddress = NULL);
	bool Open();
	void Close();
	void ShutDown();

protected:
	IPaddress m_IP;
	TCPsocket m_socket;
};

