#include "GAMEText.h"
#include "TextureManager.h"

GAMEText::GAMEText()
{
	TheTexture::Instance()->LoadTextureFromFile("Assets/Textures/GAME.png", "GAME_TEXT");

	//800 164
	m_image.SetTexture("GAME_TEXT");
	m_image.SetSpriteDimension(750, 150);
	m_image.SetTextureDimension(1, 1, 800, 164);
}

bool GAMEText::Draw()
{
	m_image.Draw(125, 100);

	return true;
}

GAMEText::~GAMEText()
{
	TheTexture::Instance()->UnloadFromMemory(TextureManager::TEXTURE_DATA, TextureManager::CUSTOM_DATA, "GAME_TEXT");
}