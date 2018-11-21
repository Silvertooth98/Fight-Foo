#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "AABB.h"

class ConnectedPlayer : public GameObject
{
public:
	ConnectedPlayer();
	virtual ~ConnectedPlayer();

	AABB GetPlayerBound() { return m_clientPlayerBound; }
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

	AABB m_clientPlayerBound;
	AABB m_hadoukenBound;
	Animation m_connectedPlayerIdle;
	Animation m_connectedPlayerAction;
	Animation m_connectedPlayerSuper;

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

