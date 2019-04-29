#include "Precom.h"
#include "EventNPC.h"
#include "Player.h"
#include "GameMaster.h"


CEventNPC::CEventNPC()
	:m_EventCol(nullptr)
{
}


CEventNPC::~CEventNPC()
{
}

void CEventNPC::Init()
{

}
void CEventNPC::CreateSubCollision(const COLFIGURE& _Figure, const COLTYPE& _ColType)
{
	m_EventCol = ACTOR->CreateCom<CCollision>(COLDATA(_T("NPC")));
	m_EventCol->SetPivotPos(_Figure.Pos);
	m_EventCol->SetColSize(_Figure.Size);
	m_EventCol->SetColType(_ColType);

	m_EventCol->SetEnter(this, &CEventNPC::EventEnter);
	m_EventCol->SetStay(this, &CEventNPC::EventStay);
	m_EventCol->SetExit(this, &CEventNPC::EventExit);
}

void CEventNPC::EventEnter(SPTR<CCollision> _pThis, SPTR<CCollision> _pOtherCol)
{
	if (CClientGlobal::MainPlayer->ParentActObject() != _pOtherCol->ParentActObject())
		return;

	CClientGlobal::GameMaster->EventTriger(m_NpcName);
	

}
void CEventNPC::EventStay(SPTR<CCollision> _pThis, SPTR<CCollision> _pOther)
{

}
void CEventNPC::EventExit(SPTR<CCollision> _pThis, SPTR<CCollision> _pOther)
{

}