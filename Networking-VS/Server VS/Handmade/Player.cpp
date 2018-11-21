#include "Player.h"
#include "TextureManager.h"
#include <glm.hpp>


Player::Player()
{
	TheTexture::Instance()->LoadTextureFromFile("Assets/Sprites/ServerFighter/FighterNormal1.png", "FIGHTER_IDLE");
	TheTexture::Instance()->LoadTextureFromFile("Assets/Sprites/ServerFighter/FighterPunchSingle.png", "FIGHTER_PUNCH");
	TheTexture::Instance()->LoadTextureFromFile("Assets/Sprites/ServerFighter/FighterKick.png", "FIGHTER_KICK");
	TheTexture::Instance()->LoadTextureFromFile("Assets/Sprites/ServerFighter/FighterHadoukenStance.png", "FIGHTER_HADOUKEN_STANCE");
	TheTexture::Instance()->LoadTextureFromFile("Assets/Sprites/ServerFighter/HadoukenBeam.png", "FIGHTER_HADOUKEN_BEAM");
	TheTexture::Instance()->LoadTextureFromFile("Assets/Sprites/ServerFighter/FighterKnockback.png", "FIGHTER_KNOCKBACK");
	
	//307 85
	m_playerIdle.SetTexture("FIGHTER_IDLE");
	m_playerIdle.SetAnimationVelocity(15.0f);
	m_playerIdle.SetSpriteDimension(125, 175);
	m_playerIdle.SetTextureDimension(5, 1, 62, 85);
	m_playerIdle.IsAnimationLooping() = true;

	//54 80
	m_playerAction.SetTexture("FIGHTER_PUNCH");
	m_playerAction.SetAnimationVelocity(10.0f);
	m_playerAction.SetSpriteDimension(125, 175);
	m_playerAction.SetTextureDimension(1, 1, 54, 80);
	m_playerAction.IsAnimationLooping() = true;

	//381 129
	m_playerSuper.SetTexture("FIGHTER_HADOUKEN_BEAM");
	m_playerSuper.SetAnimationVelocity(10.0f);
	m_playerSuper.SetSpriteDimension(300, 100);
	m_playerSuper.SetTextureDimension(1, 1, 381, 129);
	m_playerSuper.IsAnimationLooping() = true;

	m_position = glm::vec2(10, 400);

	m_playerBound.SetDimension(100, 175);
	m_playerBound.SetPosition(10, 400);

	m_health = m_startingHealth;
}

bool Player::CheckForCollision(ConnectedPlayer* m_clientPlayer)
{
	if (m_playerBound.IsColliding(m_clientPlayer->GetPlayerBound()))
	{
		m_isColliding = true;
	}

	else
	{
		m_isColliding = false;
	}

	return m_isColliding;
}

bool Player::CheckForSuperCollision(ConnectedPlayer * m_clientPlayer)
{
	if (m_playerBound.IsColliding(m_clientPlayer->GetHadoukenBound()) ||
		m_hadoukenBound.IsColliding(m_clientPlayer->GetPlayerBound()))
	{
		m_isSuperColliding = true;
	}
	else
	{
		m_isSuperColliding = false;
	}

	return m_isSuperColliding;
}

bool Player::SetHadoukenBound()
{
	if (GetIsUsingSuper() == true)
	{
		m_hadoukenBound.SetPosition(110, 200);
		m_hadoukenBound.SetDimension(700, 200);
	}	

	return false;
}

bool Player::SetIsColliding(bool isColliding)
{
	m_isColliding = isColliding;
	return true;
}

bool Player::SetIsSuperColliding(bool isSuperColliding)
{
	m_isSuperColliding = isSuperColliding;
	return true;
}

bool Player::SetCanUseSuper()
{
	if (m_hitsLanded >= 5)
	{
		m_canUseSuper = true;
	}
	else
	{
		m_canUseSuper = false;
	}

	return m_canUseSuper;
}

bool Player::SetTimer()
{
	m_timer = 0.0f;

	while (m_timer <= 3.0f)
	{
		SetCanUseSuper();
		m_timer += 0.016f;
	}

	SetHitsLanded(0);
	SetCanUseSuper();

	return false;
}

int Player::SetHitsLanded(int hitsLanded)
{
	m_hitsLanded = hitsLanded;
	return hitsLanded;
}

void Player::CheckBoundariesLeftSide()
{
	if (m_position.x <= 0)
	{
		m_position.x = 0;
	}
}

void Player::CheckBoundariesRightSide()
{
	if (m_position.x >= 900)
	{
		m_position.x = 900;
	}
}

void Player::DamageHealth()
{
	m_health = m_health - 5;
}

void Player::SuperDamageHealth()
{
	m_health = m_health - 1;
}

void Player::ResetHealth()
{
	m_health = m_startingHealth;
}

void Player::Update()
{	
	//Move right
	if (keys[SDL_SCANCODE_RIGHT] && m_isColliding == false)
	{
		m_isPlayerMoving = true;

		m_position.x += m_moveSpeed;

		CheckBoundariesRightSide();
	}

	//Move left
	if (keys[SDL_SCANCODE_LEFT])
	{
		m_isPlayerMoving = true;

		m_position.x -= m_moveSpeed;

		CheckBoundariesLeftSide();
	}

	//Press space to jump
	if (keys[SDL_SCANCODE_SPACE] && m_isFalling == false)
	{
		m_isJumping = true;
		Jump();
	}
	else if (keys[SDL_SCANCODE_SPACE] == 0)
	{
		m_isJumping = false;
		m_isFalling = true;
		Jump();
	}

	//Punch
	if (keys[SDL_SCANCODE_X] && m_isPlayerMoving == false)
	{
		m_isPlayerIdle = false;
		m_isPunching = true;
		Punch();
	}

	//Kick
	else if (keys[SDL_SCANCODE_Z] && m_isPlayerMoving == false)
	{
		m_isPlayerIdle = false;
		m_isKicking = true;
		Kick();
	}

	//Super
	else if (keys[SDL_SCANCODE_S] && m_isPlayerMoving == false)
	{
		SetCanUseSuper();

		if (m_canUseSuper)
		{
			m_isPlayerIdle = false;
			m_isUsingSuper = true;
			if (m_isUsingSuper)
			{
				SetTimer();
			}
			HadoukenStance();
			HadoukenBeam();
		}
	}

	else
	{
		m_isPlayerIdle = true;
		m_isColliding = false;
		m_isSuperColliding = false;
		m_isPunching = false;
		m_isKicking = false;
		m_isUsingSuper = false;
		m_isPlayerMoving = false;
		Normal();
	}
	
	m_playerBound.SetPosition(m_position.x, m_position.y);

	if (m_isUsingSuper)
	{
		m_hadoukenBound.SetPosition(m_position.x + 100, m_position.y);
	}
	else
	{
		m_hadoukenBound.SetPosition(2000, 2000);
	}
}

void Player::Normal()
{
	//307 85
	m_playerIdle.SetTexture("FIGHTER_IDLE");
	m_playerIdle.SetAnimationVelocity(15.0f);
	m_playerIdle.SetSpriteDimension(125, 175);
	m_playerIdle.SetTextureDimension(5, 1, 62, 85);
	m_playerIdle.IsAnimationLooping() = true;

	m_playerBound.SetDimension(100, 175);
}

void Player::Jump()
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

	else if (GetY() == m_groundHeight)
	{
		m_isFalling = false;
		Normal();
	}
}

void Player::Punch()
{
	//54 80
	m_playerAction.SetTexture("FIGHTER_PUNCH");
	m_playerAction.SetAnimationVelocity(10.0f);
	m_playerAction.SetSpriteDimension(125, 175);
	m_playerAction.SetTextureDimension(1, 1, 54, 80);
	m_playerAction.IsAnimationLooping() = true;

	m_playerBound.SetDimension(100, 175);
}

void Player::Kick()
{
	//68 82
	m_playerAction.SetTexture("FIGHTER_KICK");
	m_playerAction.SetAnimationVelocity(10.0f);
	m_playerAction.SetSpriteDimension(125, 175);
	m_playerAction.SetTextureDimension(1, 1, 68, 82);
	m_playerAction.IsAnimationLooping() = true;

	m_playerBound.SetDimension(100, 175);
}

//291 84 - Stance
void Player::HadoukenStance()
{
	//291 84
	m_playerAction.SetTexture("FIGHTER_HADOUKEN_STANCE");
	m_playerAction.SetAnimationVelocity(5.0f);
	m_playerAction.SetSpriteDimension(125, 175);
	m_playerAction.SetTextureDimension(4, 1, 72, 84);
	m_playerAction.IsAnimationLooping() = true;

	m_playerBound.SetDimension(100, 175);
}

//381 129 - Beam
void Player::HadoukenBeam()
{
	//381 129
	m_playerSuper.SetTexture("FIGHTER_HADOUKEN_BEAM");
	m_playerSuper.SetAnimationVelocity(10.0f);
	m_playerSuper.SetSpriteDimension(700, 200);
	m_playerSuper.SetTextureDimension(1, 1, 381, 129);
	m_playerSuper.IsAnimationLooping() = true;

	SetHadoukenBound();
}

void Player::KnockBack()
{
	m_position.x = m_position.x - 50;

	CheckBoundariesLeftSide();

	//49 74
	m_playerIdle.SetTexture("FIGHTER_KNOCKBACK");
	m_playerIdle.SetAnimationVelocity(10.0f);
	m_playerIdle.SetSpriteDimension(125, 175);
	m_playerIdle.SetTextureDimension(1, 1, 49, 74);
	m_playerIdle.IsAnimationLooping() = true;

	m_playerBound.SetDimension(100, 175);
}

bool Player::Draw()
{
	if (m_isPlayerIdle)
	{
		m_playerIdle.Draw(m_position.x, m_position.y, 0.0f);
	}

	else if (m_isPunching || m_isKicking)
	{
		m_playerAction.Draw(m_position.x, m_position.y, 0.0f);
	}

	else if (m_isUsingSuper)
	{
		m_playerAction.Draw(m_position.x, m_position.y, 0.0f);
		m_playerSuper.Draw(m_position.x + 100.0f, m_position.y, 0.0f);
	}

	return true;
}

Player::~Player()
{
	TheTexture::Instance()->UnloadFromMemory(TextureManager::TEXTURE_DATA, TextureManager::CUSTOM_DATA, "FIGHTER_IDLE");
	TheTexture::Instance()->UnloadFromMemory(TextureManager::TEXTURE_DATA, TextureManager::CUSTOM_DATA, "FIGHTER_PUNCH");
	TheTexture::Instance()->UnloadFromMemory(TextureManager::TEXTURE_DATA, TextureManager::CUSTOM_DATA, "FIGHTER_KICK");
	TheTexture::Instance()->UnloadFromMemory(TextureManager::TEXTURE_DATA, TextureManager::CUSTOM_DATA, "FIGHTER_HADOUKEN_STANCE");
	TheTexture::Instance()->UnloadFromMemory(TextureManager::TEXTURE_DATA, TextureManager::CUSTOM_DATA, "FIGHTER_HADOUKEN_BEAM");
	TheTexture::Instance()->UnloadFromMemory(TextureManager::TEXTURE_DATA, TextureManager::CUSTOM_DATA, "FIGHTER_KNOCKBACK");
}