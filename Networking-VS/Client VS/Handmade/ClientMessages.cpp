#include "ClientMessages.h"
#include "TextureManager.h"
#include "Game.h"

ClientMessages::ClientMessages()
{
	TheTexture::Instance()->LoadFontFromFile("Assets/Fonts/Denka.ttf", 100, "CLIENT_MESSAGES_FONT");

	m_menuText.SetFont("CLIENT_MESSAGES_FONT");
	m_menuText.SetColor(255, 235, 0);
	m_menuText.SetSize(300, 80);
}

void ClientMessages::SetHUDText(std::string message)
{
	m_menuText.SetText(message);
}

bool ClientMessages::Draw()
{
	m_menuText.Draw(100, 700);

	return true;
}

ClientMessages::~ClientMessages()
{
	TheTexture::Instance()->UnloadFromMemory(TextureManager::FONT_DATA, TextureManager::CUSTOM_DATA, "CLIENT_MESSAGES_FONT");
}