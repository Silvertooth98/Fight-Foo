#pragma once

#include "TCPSocket.h"
#include <iostream>
#include <string>
#include <SDL.h>
#include <SDL_net.h>

class TCPServer : public TCPSocket
{
public:
	TCPServer();

public:
	bool Listen();
	bool SendMessage(const std::string& message);
	bool ReceiveMessage(std::string& message);
	void Close();

private:
	TCPsocket m_clientSocket;
};