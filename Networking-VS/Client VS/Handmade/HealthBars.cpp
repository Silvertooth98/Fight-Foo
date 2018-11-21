#include "HealthBars.h"
#include "TextureManager.h"
#include "Game.h"

HealthBars::HealthBars()
{
	TheTexture::Instance()->LoadFontFromFile("Assets/Fonts/Liquid.ttf", 100, "HEALTH_BARS_FONT");

	m_playerHealthText.SetFont("HEALTH_BARS_FONT");
	m_playerHealthText.SetColor(255, 235, 0);
	m_playerHealthText.SetSize(200, 50);
	m_playerHealthText.SetText("Player Health: 100");

	m_opponentHealthText.SetFont("HEALTH_BARS_FONT");
	m_opponentHealthText.SetColor(255, 235, 0);
	m_opponentHealthText.SetSize(200, 50);
	m_opponentHealthText.SetText("Opponent Health: 100");
}

void HealthBars::SetPlayerHealthText(const std::string message)
{
	m_playerHealthText.SetText(message);
}

void HealthBars::SetOpponentHealthText(const std::string message)
{
	m_opponentHealthText.SetText(message);
}

bool HealthBars::Draw()
{
	m_playerHealthText.Draw(700, 50);
	m_opponentHealthText.Draw(100, 50);

	return true;
}

HealthBars::~HealthBars()
{
	TheTexture::Instance()->UnloadFromMemory(TextureManager::FONT_DATA, TextureManager::CUSTOM_DATA, "HEALTH_BARS_FONT");
}