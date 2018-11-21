#include "HealthHUD.h"
#include "TextureManager.h"
#include "Game.h"

HealthHUD::HealthHUD()
{
	TheTexture::Instance()->LoadFontFromFile("Assets/Fonts/Liquid.ttf", 100, "HEALTH_HUD_FONT");

	m_playerHealthText.SetFont("HEALTH_HUD_FONT");
	m_playerHealthText.SetColor(255, 235, 0);
	m_playerHealthText.SetSize(200, 50);
	m_playerHealthText.SetText("Player Health: 100");

	m_opponentHealthText.SetFont("HEALTH_HUD_FONT");
	m_opponentHealthText.SetColor(255, 235, 0);
	m_opponentHealthText.SetSize(200, 50);
	m_opponentHealthText.SetText("Opponent Health: 100");
}

void HealthHUD::SetPlayerHealthText(const std::string message)
{
	m_playerHealthText.SetText(message);
}

void HealthHUD::SetOpponentHealthText(const std::string message)
{
	m_opponentHealthText.SetText(message);
}

bool HealthHUD::Draw()
{
	m_playerHealthText.Draw(100, 50);
	m_opponentHealthText.Draw(700, 50);

	return true;
}

HealthHUD::~HealthHUD()
{
	TheTexture::Instance()->UnloadFromMemory(TextureManager::FONT_DATA, TextureManager::CUSTOM_DATA, "HEALTH_HUD_FONT");
}