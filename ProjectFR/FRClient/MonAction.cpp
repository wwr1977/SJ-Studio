#include "Precom.h"
#include "MonAction.h"
#include"BattleUnit.h"

CMonAction::CMonAction()
	:m_bEnd(false), m_iActionIndex(0)
	, m_iAniLooping(3)
{
}


CMonAction::~CMonAction()
{
}

void CMonAction::StateReset() 
{
	m_pUnit->ChangeAni(Atk(m_UnitName));
	m_fAccTime = 0.0f;
	m_bEnd = false;
	m_pUnit->EndAction(false);
}
void CMonAction::Update()
{
	if (false == m_bEnd  && m_iAniLooping <= m_pUnit->GetCurAniLooping())
	{
		m_pUnit->EndAction();
		m_pUnit->SetIdle();
		m_bEnd = true;
	}
}