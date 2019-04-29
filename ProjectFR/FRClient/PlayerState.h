#pragma once
#include <BaseState.h>

class CPlayerLogic;
class CPlayerState : public CBaseState
{
protected:
	// 플레이어 상태 값
	enum
	{
		IDLE,
		ATTACK,
		REACTION,
		DEATH,
	};

	CPlayerLogic*		m_pPlayer;

public:
	void Init(void* _pLogic);
	

	CPlayerLogic* GetPlayer();
public:
	CPlayerState();
	~CPlayerState();
};

