#pragma once
#include <BaseState.h>

class CBattleUnit;
class CBattleState : public CBaseState
{
public:
	friend CBattleUnit;

	//enum UNITSTATE
	//{
	//	BATTLE_IDLE,
	//	BATTLE_ACTION,
	//	BATTLE_REACTION,
	//};

	enum SCRIPTQUEUE
	{
		QUEUE_ANI,
		QUEUE_MOVE,
		QUEUE_SOUND,
		MAXSCRIPTQUEUE
	};
protected:
	CBattleUnit*		m_pUnit;
	tstring				m_UnitName;
	float				m_fAccTime;
	float				m_fActionTime;

	SPTR<CSoundPlayer>	m_UnitSoundPlayer;

public:
	void Init(void* _pLogic);
	CBattleUnit* GetPlayer();


public:
	CBattleState();
	~CBattleState();
};

