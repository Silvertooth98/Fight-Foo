#include "ServerPlayer.h"
#include "TextureManager.h"
#include "InputManager.h"

ServerPlayer::ServerPlayer()
{
	TheTexture::Instance()->LoadTextureFromFile("Assets/Sprites/ServerFighter/FighterNormal1.png", "SERVER_PLAYER");
	TheTexture::Instance()->LoadTextureFromFile("Assets/Sprites/ServerFighter/FighterPunchSingle.png", "SERVER_PLAYER_PUNCH");
	TheTexture::Instance()->LoadTextureFromFile("Assets/Sprites/ServerFighter/FighterKick.png", "SERVER_PLAYER_KICK");
	TheTexture::Instance()->LoadTextureFromFile("Assets/Sprites/ServerFighter/FighterHadoukenStance.png", "SERVER_PLAYER_STANCE");
	TheTexture::Instance()->LoadTextureFromFile("Assets/Sprites/ServerFighter/HadoukenBeam.png", "SERVER_PLAYER_BEAM");
	TheTexture::Instance()->LoadTextureFromFile("Assets/Sprites/ServerFighter/FighterKnockback.png", "SERVER_PLAYER_KNOCKBACK");

	//307 85
	m_serverPlayerIdle.SetTexture("SERVER_PLAYER");
	m_serverPlayerIdle.SetAnimationVelocity(15.0f);
	m_serverPlayerIdle.SetSpriteDimension(125, 175);
	m_serverPlayerIdle.SetTextureDimension(5, 1, 62, 85);
	m_serverPlayerIdle.IsAnimationLooping() = true;

	//54 80
	m_serverPlayerAction.SetTexture("SERVER_PLAYER_PUNCH");
	m_serverPlayerAction.SetAnimationVelocity(10.0f);
	m_serverPlayerAction.SetSpriteDimension(125, 175);
	m_serverPlayerAction.SetTextureDimension(1, 1, 54, 80);
	m_serverPlayerAction.IsAnimationLooping() = true;

	m_position = glm::vec2(10, 400);

	m_serverPlayerBound.SetDimension(100, 175);
	m_serverPlayerBound.SetPosition(10, 400);
}

bool ServerPlayer::SetIsKicking(bool isKicking)
{
	m_isPlayerIdle = false;
	m_isKicking = isKicking;
	return true;
}

bool ServerPlayer::SetIsPunching(bool isPunching)
{
	m_isPlayerIdle = false;
	m_isPunching = isPunching;
	return true;
}

bool ServerPlayer::SetIsIdle(bool isIdle)
{
	m_isKicking = false;
	m_isPunching = false;
	m_isUsingSuper = false;
	m_isPlayerIdle = isIdle;
	return true;
}

bool ServerPlayer::SetIsUsingSuper(bool isUsingSuper)
{
	m_isPlayerIdle = false;
	m_isUsingSuper = isUsingSuper;
	return true;
}

void ServerPlayer::Update()
{
	m_serverPlayerBound.SetPosition(m_position.x, m_position.y);

	if (m_isUsingSuper)
	{
		m_hadoukenBound.SetPosition(m_position.x, m_position.y);
	}
	else
	{
		m_hadoukenBound.SetPosition(2000, 2000);
	}
}

void ServerPlayer::Normal()
{
	//307 85
	m_serverPlayerIdle.SetTexture("SERVER_PLAYER");
	m_serverPlayerIdle.SetAnimationVelocity(15.0f);
	m_serverPlayerIdle.SetSpriteDimension(125, 175);
	m_serverPlayerIdle.SetTextureDimension(5, 1, 62, 85);
	m_serverPlayerIdle.IsAnimationLooping() = true;

	m_serverPlayerBound.SetDimension(100, 175);
}

void ServerPlayer::Jump()
{
	if (m_isJumping == true && m_isFalling == false)
	{
		m_position.y -= m_jumpSpeed;

		if (m_position.y <= 300)
		{
			m_isJumping = false;
			m_isFalling = true;
		}
	}

	if (GetY() < m_groundHeight && m_isJumping == false && m_isFalling == true)
	{
		m_position.y += m_gravSpeed;
	}

	if (GetY() == m_groundHeight)
	{
		m_isFalling = false;

		Normal();
	}
}

void ServerPlayer::Punch()
{
	//54 80
	m_serverPlayerAction.SetTexture("SERVER_PLAYER_PUNCH");
	m_serverPlayerAction.SetAnimationVelocity(10.0f);
	m_serverPlayerAction.SetSpriteDimension(125, 175);
	m_serverPlayerAction.SetTextureDimension(1, 1, 54, 80);
	m_serverPlayerAction.IsAnimationLooping() = true;

	m_serverPlayerBound.SetDimension(100, 175);
}

void ServerPlayer::Kick()
{
	//68 82
	m_serverPlayerAction.SetTexture("SERVER_PLAYER_KICK");
	m_serverPlayerAction.SetAnimationVelocity(10.0f);
	m_serverPlayerAction.SetSpriteDimension(125, 175);
	m_serverPlayerAction.SetTextureDimension(1, 1, 68, 82);
	m_serverPlayerAction.IsAnimationLooping() = true;

	m_serverPlayerBound.SetDimension(100, 175);
}

void ServerPlayer::HadoukenStance()
{
	//291 84
	m_serverPlayerAction.SetTexture("FIGHTER_HADOUKEN_STANCE");
	m_serverPlayerAction.SetAnimationVelocity(5.0f);
	m_serverPlayerAction.SetSpriteDimension(125, 175);
	m_serverPlayerAction.SetTextureDimension(4, 1, 72, 84);
	m_serverPlayerAction.IsAnimationLooping() = true;

	m_serverPlayerBound.SetDimension(100, 175);
}

void ServerPlayer::HadoukenBeam()
{
	//381 129
	m_serverPlayerSuper.SetTexture("FIGHTER_HADOUKEN_BEAM");
	m_serverPlayerSuper.SetAnimationVelocity(10.0f);
	m_serverPlayerSuper.SetSpriteDimension(700, 200);
	m_serverPlayerSuper.SetTextureDimension(1, 1, 381, 129);
	m_serverPlayerSuper.IsAnimationLooping() = true;

	//700 200
	m_hadoukenBound.SetPosition(110, 200);
	m_hadoukenBound.SetDimension(700, 200);
}

void ServerPlayer::KnockBack()
{
	//49 74
	m_serverPlayerAction.SetTexture("SERVER_FIGHTER_KNOCKBACK");
	m_serverPlayerAction.SetAnimationVelocity(10.0f);
	m_serverPlayerAction.SetSpriteDimension(125, 175);
	m_serverPlayerAction.SetTextureDimension(1, 1, 49, 74);
	m_serverPlayerAction.IsAnimationLooping() = true;

	m_serverPlayerBound.SetDimension(100, 175);
}

bool ServerPlayer::Draw()
{
	if (m_isPlayerIdle)
	{
		m_serverPlayerIdle.Draw(m_position.x, m_position.y, 0.0f);
	}

	else if (m_isPunching || m_isKicking)
	{
		m_serverPlayerAction.Draw(m_position.x, m_position.y, 0.0f);
	}

	else if (m_isUsingSuper == true)
	{
		m_serverPlayerAction.Draw(m_position.x, m_position.y, 0.0f);
		m_serverPlayerSuper.Draw(m_position.x + 100.0f, m_position.y, 0.0f, Sprite::HORIZONTAL);
	}

	return true;
}

ServerPlayer::~ServerPlayer()
{
	TheTexture::Instance()->UnloadFromMemory(TextureManager::TEXTURE_DATA, TextureManager::CUSTOM_DATA, "SERVER_PLAYER");
	TheTexture::Instance()->UnloadFromMemory(TextureManager::TEXTURE_DATA, TextureManager::CUSTOM_DATA, "SERVER_PLAYER_KICK");
	TheTexture::Instance()->UnloadFromMemory(TextureManager::TEXTURE_DATA, TextureManager::CUSTOM_DATA, "SERVER_PLAYER_PUNCH");
	TheTexture::Instance()->UnloadFromMemory(TextureManager::TEXTURE_DATA, TextureManager::CUSTOM_DATA, "SERVER_PLAYER_STANCE");
	TheTexture::Instance()->UnloadFromMemory(TextureManager::TEXTURE_DATA, TextureManager::CUSTOM_DATA, "SERVER_PLAYER_BEAM");
	TheTexture::Instance()->UnloadFromMemory(TextureManager::TEXTURE_DATA, TextureManager::CUSTOM_DATA, "SERVER_PLAYER_KNOCKBACK");
}