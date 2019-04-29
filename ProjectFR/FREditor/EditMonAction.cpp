#include "stdafx.h"
#include "EditMonAction.h"
#include "EditBattleUnit.h"

CEditMonAction::CEditMonAction()
	:m_bEnd(false), m_iActionIndex(0)
	, m_iAniLooping(3)
{
}


CEditMonAction::~CEditMonAction()
{
}

void CEditMonAction::StateReset()
{
	m_pUnit->ChangeAni(Atk(m_UnitName));
	m_fAccTime = 0.0f;
	m_bEnd = false;
	m_pUnit->EndAction(false);
}
void CEditMonAction::Update()
{
	if (false == m_bEnd  && m_iAniLooping <= m_pUnit->GetCurAniLooping())
	{
		m_pUnit->EndAction();
		m_pUnit->SetIdle();
		m_bEnd = true;
	}




}