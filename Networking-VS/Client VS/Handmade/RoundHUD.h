#pragma once

#include "GameObject.h"
#include "Text.h"

class RoundHUD : public GameObject
{
public:
	RoundHUD();
	virtual ~RoundHUD();

	void SetPlayerRoundText(const std::string message);
	void SetOpponentRoundText(const std::string message);

	int GetPlayerWins() { return m_playerWins; }
	int GetOpponentWins() { return m_opponentWins; }

	void playerWins();
	void opponentWins();
	void resetRounds();

	virtual void Update() {}
	virtual bool Draw();

private:

	Text m_playerRoundText;
	Text m_roundCountSeparatorText;
	Text m_opponentRoundText;

	int m_playerWins = 0;
	int m_opponentWins = 0;
};