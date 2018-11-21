#include "RoundHUD.h"
#include "TextureManager.h"
#include "Game.h"

RoundHUD::RoundHUD()
{
	TheTexture::Instance()->LoadFontFromFile("Assets/Fonts/RoundFont.ttf", 100, "ROUND_HUD_FONT");

	m_playerRoundText.SetFont("ROUND_HUD_FONT");
	m_playerRoundText.SetColor(255, 235, 0);
	m_playerRoundText.SetSize(25, 25);
	m_playerRoundText.SetText("0");

	m_roundCountSeparatorText.SetFont("ROUND_HUD_FONT");
	m_roundCountSeparatorText.SetColor(255, 235, 0);
	m_roundCountSeparatorText.SetSize(25, 25);
	m_roundCountSeparatorText.SetText(" : ");

	m_opponentRoundText.SetFont("ROUND_HUD_FONT");
	m_opponentRoundText.SetColor(255, 235, 0);
	m_opponentRoundText.SetSize(25, 25);
	m_opponentRoundText.SetText("0");
}

void RoundHUD::SetPlayerRoundText(const std::string message)
{
	m_playerRoundText.SetText(message);
}

void RoundHUD::SetOpponentRoundText(const std::string message)
{
	m_opponentRoundText.SetText(message);
}

void RoundHUD::playerWins()
{
	m_playerWins = m_playerWins + 1;
}

void RoundHUD::opponentWins()
{
	m_opponentWins = m_opponentWins + 1;
}

void RoundHUD::resetRounds()
{
	m_playerWins = 0;
	m_opponentWins = 0;
}

bool RoundHUD::Draw()
{
	m_opponentRoundText.Draw(400, 50);
	m_roundCountSeparatorText.Draw(450, 50);
	m_playerRoundText.Draw(500, 50);

	return true;
}

RoundHUD::~RoundHUD()
{
	TheTexture::Instance()->UnloadFromMemory(TextureManager::FONT_DATA, TextureManager::CUSTOM_DATA, "ROUND_HUD_FONT");
}