#pragma once
#include "NPCBase.h"
class CEventNPC : public CNPCBase
{
private:
	SPTR<CCollision>				m_EventCol;


public:
	void Init();

public:
	void CreateSubCollision(const COLFIGURE& _Figure, const COLTYPE& _ColType);

public:
	void EventEnter(SPTR<CCollision> _pThis, SPTR<CCollision> _pOtherCol);
	void EventStay(SPTR<CCollision> _pThis, SPTR<CCollision> _pOtherCol);
	void EventExit(SPTR<CCollision> _pThis, SPTR<CCollision> _pOtherCol);


public:
	CEventNPC();
	~CEventNPC();
};

