#include "ConnectedPlayer.h"
#include "TextureManager.h"
#include "InputManager.h"
#include <glm.hpp>

ConnectedPlayer::ConnectedPlayer()
{
	TheTexture::Instance()->LoadTextureFromFile("Assets/Sprites/ClientFighter/FighterNormal2.png", "CLIENT_FIGHTER");
	TheTexture::Instance()->LoadTextureFromFile("Assets/Sprites/ClientFighter/Fighter2Punch.png", "CLIENT_FIGHTER_PUNCH");
	TheTexture::Instance()->LoadTextureFromFile("Assets/Sprites/ClientFighter/Fighter2Kick.png", "CLIENT_FIGHTER_KICK");
	TheTexture::Instance()->LoadTextureFromFile("Assets/Sprites/ClientFighter/Fighter2HadoukenStance.png", "CLIENT_FIGHTER_STANCE");
	TheTexture::Instance()->LoadTextureFromFile("Assets/Sprites/ClientFighter/HadoukenBeamFlipped.png", "CLIENT_FIGHTER_BEAM");
	TheTexture::Instance()->LoadTextureFromFile("Assets/Sprites/ClientFighter/Fighter2Knockback.png", "CLIENT_FIGHTER_KNOCKBACK");

	//307 85
	m_connectedPlayerIdle.SetTexture("CLIENT_FIGHTER");
	m_connectedPlayerIdle.SetAnimationVelocity(15.0f);
	m_connectedPlayerIdle.SetSpriteDimension(125, 175);
	m_connectedPlayerIdle.SetTextureDimension(5, 1, 62, 85);
	m_connectedPlayerIdle.IsAnimationLooping() = true;

	//54 80
	m_connectedPlayerAction.SetTexture("CLIENT_FIGHTER_PUNCH");
	m_connectedPlayerAction.SetAnimationVelocity(10.0f);
	m_connectedPlayerAction.SetSpriteDimension(125, 175);
	m_connectedPlayerAction.SetTextureDimension(1, 1, 54, 80);
	m_connectedPlayerAction.IsAnimationLooping() = true;

	//381 129
	m_connectedPlayerSuper.SetTexture("FIGHTER_HADOUKEN_BEAM");
	m_connectedPlayerSuper.SetAnimationVelocity(10.0f);
	m_connectedPlayerSuper.SetSpriteDimension(700, 200);
	m_connectedPlayerSuper.SetTextureDimension(1, 1, 381, 129);
	m_connectedPlayerSuper.IsAnimationLooping() = true;

	m_position = glm::vec2(800, 400);

	m_clientPlayerBound.SetDimension(100, 175);
	m_clientPlayerBound.SetPosition(800, 400);
}

bool ConnectedPlayer::SetIsKicking(bool isKicking)
{
	m_isPlayerIdle = false;
	m_isKicking = isKicking;
	return true;
}

bool ConnectedPlayer::SetIsPunching(bool isPunching)
{
	m_isPlayerIdle = false;
	m_isPunching = isPunching;
	return true;
}

bool ConnectedPlayer::SetIsIdle(bool isIdle)
{
	m_isKicking = false;
	m_isPunching = false;
	m_isUsingSuper = false;
	m_isPlayerIdle = isIdle;
	return true;
}

bool ConnectedPlayer::SetIsUsingSuper(bool isUsingSuper)
{
	m_isPlayerIdle = false;
	m_isUsingSuper = isUsingSuper;
	return true;
}

void ConnectedPlayer::Update()
{
	m_clientPlayerBound.SetPosition(m_position.x, m_position.y);

	if (m_isUsingSuper)
	{
		m_hadoukenBound.SetPosition(m_position.x, m_position.y);
	}
	else
	{
		m_hadoukenBound.SetPosition(2000, 2000);
	}
}

void ConnectedPlayer::Normal()
{
	//307 85
	m_connectedPlayerIdle.SetTexture("CLIENT_FIGHTER");
	m_connectedPlayerIdle.SetAnimationVelocity(15.0f);
	m_connectedPlayerIdle.SetSpriteDimension(125, 175);
	m_connectedPlayerIdle.SetTextureDimension(5, 1, 62, 85);
	m_connectedPlayerIdle.IsAnimationLooping() = true;

	m_clientPlayerBound.SetDimension(100, 175);
}

void ConnectedPlayer::Jump()
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

void ConnectedPlayer::Punch()
{
	//54 80
	m_connectedPlayerAction.SetTexture("CLIENT_FIGHTER_PUNCH");
	m_connectedPlayerAction.SetAnimationVelocity(10.0f);
	m_connectedPlayerAction.SetSpriteDimension(125, 175);
	m_connectedPlayerAction.SetTextureDimension(1, 1, 54, 80);
	m_connectedPlayerAction.IsAnimationLooping() = true;

	m_clientPlayerBound.SetDimension(100, 175);
}

void ConnectedPlayer::Kick()
{
	//68 82
	m_connectedPlayerAction.SetTexture("CLIENT_FIGHTER_KICK");
	m_connectedPlayerAction.SetAnimationVelocity(10.0f);
	m_connectedPlayerAction.SetSpriteDimension(125, 175);
	m_connectedPlayerAction.SetTextureDimension(1, 1, 68, 82);
	m_connectedPlayerAction.IsAnimationLooping() = true;

	m_clientPlayerBound.SetDimension(100, 175);
}

void ConnectedPlayer::HadoukenStance()
{
	//291 84
	m_connectedPlayerAction.SetTexture("CLIENT_FIGHTER_STANCE");
	m_connectedPlayerAction.SetAnimationVelocity(5.0f);
	m_connectedPlayerAction.SetSpriteDimension(125, 175);
	m_connectedPlayerAction.SetTextureDimension(4, 1, 72, 84);
	m_connectedPlayerAction.IsAnimationLooping() = true;

	m_clientPlayerBound.SetDimension(100, 175);
}

void ConnectedPlayer::HadoukenBeam()
{
	//381 129
	m_connectedPlayerSuper.SetTexture("FIGHTER_HADOUKEN_BEAM");
	m_connectedPlayerSuper.SetAnimationVelocity(10.0f);
	m_connectedPlayerSuper.SetSpriteDimension(700, 200);
	m_connectedPlayerSuper.SetTextureDimension(1, 1, 381, 129);
	m_connectedPlayerSuper.IsAnimationLooping() = true;

	m_hadoukenBound.SetPosition(125, 200);
	m_hadoukenBound.SetDimension(700, 200);
}

void ConnectedPlayer::KnockBack()
{
	//49 74
	m_connectedPlayerAction.SetTexture("CLIENT_FIGHTER_KNOCKBACK");
	m_connectedPlayerAction.SetAnimationVelocity(10.0f);
	m_connectedPlayerAction.SetSpriteDimension(125, 175);
	m_connectedPlayerAction.SetTextureDimension(1, 1, 49, 74);
	m_connectedPlayerAction.IsAnimationLooping() = true;

	m_clientPlayerBound.SetDimension(100, 175);
}

bool ConnectedPlayer::Draw()
{
	if (m_isPlayerIdle)
	{
		m_connectedPlayerIdle.Draw(m_position.x, m_position.y, 0.0f, Sprite::HORIZONTAL);
	}

	else if (m_isPunching || m_isKicking)
	{
		m_connectedPlayerAction.Draw(m_position.x, m_position.y, 0.0f, Sprite::HORIZONTAL);
	}

	else if (m_isUsingSuper == true)
	{
		m_connectedPlayerAction.Draw(m_position.x, m_position.y, 0.0f, Sprite::HORIZONTAL);
		m_connectedPlayerSuper.Draw(m_position.x - 675.0f, m_position.y, 0.0f, Sprite::HORIZONTAL);
	}

	return true;
}

ConnectedPlayer::~ConnectedPlayer()
{
	TheTexture::Instance()->UnloadFromMemory(TextureManager::TEXTURE_DATA, TextureManager::CUSTOM_DATA, "CLIENT_FIGHTER");
	TheTexture::Instance()->UnloadFromMemory(TextureManager::TEXTURE_DATA, TextureManager::CUSTOM_DATA, "CLIENT_FIGHTER_KICK");
	TheTexture::Instance()->UnloadFromMemory(TextureManager::TEXTURE_DATA, TextureManager::CUSTOM_DATA, "CLIENT_FIGHTER_PUNCH");
	TheTexture::Instance()->UnloadFromMemory(TextureManager::TEXTURE_DATA, TextureManager::CUSTOM_DATA, "CLIENT_FIGHTER_STANCE");
	TheTexture::Instance()->UnloadFromMemory(TextureManager::TEXTURE_DATA, TextureManager::CUSTOM_DATA, "CLIENT_FIGHTER_BEAM");
	TheTexture::Instance()->UnloadFromMemory(TextureManager::TEXTURE_DATA, TextureManager::CUSTOM_DATA, "CLIENT_FIGHTER_KNOCKBACK");
}