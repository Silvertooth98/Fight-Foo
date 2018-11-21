#pragma once

#include "GameObject.h"
#include "Text.h"

class HealthBars : public GameObject
{
public:
	HealthBars();
	virtual ~HealthBars();

	void SetPlayerHealthText(const std::string message);
	void SetOpponentHealthText(const std::string message);

	virtual void Update() {}
	virtual bool Draw();

private:

	Text m_playerHealthText;
	Text m_opponentHealthText;
};

