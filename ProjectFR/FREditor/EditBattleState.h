#pragma once
#include <BaseState.h>

class CEditBattleUnit;
class CEditBattleState : public CBaseState
{
public:
	friend CEditBattleUnit;


	enum SCRIPTQUEUE
	{
		QUEUE_ANI,
		QUEUE_MOVE,
		QUEUE_SOUND,
		MAXSCRIPTQUEUE
	};
protected:
	CEditBattleUnit*		m_pUnit;
	tstring				m_UnitName;
	float				m_fAccTime;
	float				m_fActionTime;

	SPTR<CSoundPlayer>	m_UnitSoundPlayer;

public:
	void Init(void* _pLogic);
	CEditBattleUnit* GetPlayer();


public:
	CEditBattleState();
	~CEditBattleState();
};

