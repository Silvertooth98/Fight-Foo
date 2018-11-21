#pragma once

#include "GameObject.h"
#include "Text.h"

class HUD : public GameObject
{
public:
	HUD();
	virtual ~HUD();

	void SetHUDText(std::string message);

	virtual void Update() {}
	virtual bool Draw();

private:

	Text m_menuText;
};