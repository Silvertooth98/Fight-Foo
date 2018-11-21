#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "AABB.h"
#include "InputManager.h"
#include "ServerPlayer.h"

class Player : public GameObject
{
public:
	Player();
	virtual ~Player();

	AABB GetPlayerBound() { return m_playerBound; }
	AABB GetHadoukenBound() { return m_hadoukenBound; }
	int GetX() { return m_position.x; }
	int GetY() { return m_position.y; }
	int GetHealth() { return m_health; }
	int GetHitsLanded() { return m_hitsLanded; }
	bool GetIsPunching() { return m_isPunching; }
	bool GetIsKicking() { return m_isKicking; }
	bool CanUseSuper() { return m_canUseSuper; }
	bool GetIsUsingSuper() { return m_isUsingSuper; }
	bool GetIsColliding() { return m_isColliding; }
	bool CheckForCollision(ServerPlayer* m_serverPlayer);
	bool CheckForSuperCollision(ServerPlayer* m_serverPlayer);

	bool SetHadoukenBound();
	bool SetIsColliding(bool isColliding);
	bool SetIsSuperColliding(bool isSuperColliding);
	bool SetCanUseSuper();
	bool SetTimer();
	int SetHitsLanded(int hitsLanded);

	void CheckBoundariesLeftSide();
	void CheckBoundariesRightSide();
	void DamageHealth();
	void SuperDamageHealth();
	void ResetHealth();
	virtual void Update();

	void Normal();
	void Jump();
	void Punch();
	void Kick();
	void HadoukenStance();
	void HadoukenBeam();
	void KnockBack();
	virtual bool Draw();

private:
	
	const Uint8* keys = TheInput::Instance()->GetKeyStates();

	AABB m_playerBound;
	AABB m_hadoukenBound;
	Animation m_playerIdle;
	Animation m_playerAction;
	Animation m_playerSuper;

	const int m_groundHeight = 400;
	const float m_gravSpeed = 5.0f;

	bool m_isPlayerIdle = true;
	bool m_isJumping = false;
	bool m_isFalling = false;
	bool m_isPlayerMoving = false;
	bool m_isPunching = false;
	bool m_isKicking = false;
	bool m_canUseSuper = false;
	bool m_isUsingSuper = false;
	bool m_isColliding = false;
	bool m_isSuperColliding = false;

	const float m_moveSpeed = 2.0f;
	const float m_jumpSpeed = 5.0f;

	int m_health;
	int m_startingHealth = 100;

	float m_timer;
	int m_hitsLanded;
};