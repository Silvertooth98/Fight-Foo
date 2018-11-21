#include "ServerMessages.h"
#include "TextureManager.h"
#include "Game.h"

ServerMessages::ServerMessages()
{
	TheTexture::Instance()->LoadFontFromFile("Assets/Fonts/Denka.ttf", 100, "SERVER_MESSAGES_FONT");

	m_menuText.SetFont("SERVER_MESSAGES_FONT");
	m_menuText.SetColor(255, 235, 0);
	m_menuText.SetSize(300, 80);
}

void ServerMessages::SetHUDText(std::string message)
{
	m_menuText.SetText(message);
}

bool ServerMessages::Draw()
{
	m_menuText.Draw(100, 700);//set position here

	return true;
}

ServerMessages::~ServerMessages()
{
	TheTexture::Instance()->UnloadFromMemory(TextureManager::FONT_DATA, TextureManager::CUSTOM_DATA, "SERVER_MESSAGES_FONT");
}