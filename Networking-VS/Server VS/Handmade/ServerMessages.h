#pragma once

#include "GameObject.h"
#include "Text.h"

class ServerMessages : public GameObject
{
public:
	ServerMessages();
	virtual ~ServerMessages();

	void SetHUDText(std::string message);

	virtual void Update() {}
	virtual bool Draw();

private:

	Text m_menuText;
};

