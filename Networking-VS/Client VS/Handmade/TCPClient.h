#pragma once

#include "TCPSocket.h"
#include <iostream>
#include <SDL.h>
#include <SDL_net.h>
#include <string>

class TCPClient : public TCPSocket
{
public:
	bool SendMessage(const std::string& message);
	bool ReceiveMessage(std::string& message);
};

