#pragma once

#include "GameObject.h"
#include "Text.h"

class ClientMessages : public GameObject
{
public:
	ClientMessages();
	virtual ~ClientMessages();

	void SetHUDText(std::string message);

	virtual void Update() {}
	virtual bool Draw();

private:

	Text m_menuText;
};

