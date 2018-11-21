#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "AABB.h"

class ServerPlayer : public GameObject
{
public:
	ServerPlayer();
	virtual ~ServerPlayer();

	AABB GetPlayerBound() { return m_serverPlayerBound; }
	AABB GetHadoukenBound() { return m_hadoukenBound; }
	int GetY() { return m_position.y; }

	bool SetIsKicking(bool isKicking);
	bool SetIsPunching(bool isPunching);
	bool SetIsIdle(bool isIdle);
	bool SetIsUsingSuper(bool isUsingSuper);

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

	AABB m_serverPlayerBound;
	AABB m_hadoukenBound;
	Animation m_serverPlayerIdle;
	Animation m_serverPlayerAction;
	Animation m_serverPlayerSuper;

	const int m_groundHeight = 400;
	const float m_gravSpeed = 5.0f;

	bool m_isPlayerIdle = true;
	bool m_isJumping = false;
	bool m_isFalling = false;
	bool m_isPunching = false;
	bool m_isKicking = false;
	bool m_canUseSuper = false;
	bool m_isUsingSuper = false;

	const float m_jumpSpeed = 5.0f;
};

