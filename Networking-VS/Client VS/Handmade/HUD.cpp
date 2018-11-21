#include "HUD.h"
#include "TextureManager.h"
#include "Game.h"

HUD::HUD()
{
	TheTexture::Instance()->LoadFontFromFile("Assets/Fonts/Denka.ttf", 100, "HUD_FONT");

	m_menuText.SetFont("HUD_FONT");
	m_menuText.SetColor(255, 235, 0);
	m_menuText.SetSize(300, 80);
	m_menuText.SetText("Fight Fuu");
}

bool HUD::Draw()
{
	m_menuText.Draw(200, 100);
	
	return true;
}

void HUD::SetHUDText(std::string message)
{
	m_menuText.SetText(message);
}

HUD::~HUD()
{
	TheTexture::Instance()->UnloadFromMemory(TextureManager::FONT_DATA, TextureManager::CUSTOM_DATA, "HUD_FONT");
}