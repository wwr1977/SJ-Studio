#include "Precom.h"
#include "CharAppear.h"
#include"BattleUnit.h"

CCharAppear::CCharAppear()
	:m_fAppearTime(0.6f), m_fAccTime(0.f)
{
}


CCharAppear::~CCharAppear()
{
}
void CCharAppear::StateReset()
{
	m_fAccTime = 0.f;
	m_pUnit->InitAppear();
	m_pUnit->ChangeAni(Idle(m_UnitName));
}
void CCharAppear::Update()
{
	if (m_fAccTime >= m_fAppearTime)
	{
		m_pUnit->SetIdle();
		m_pUnit->SetAlpha(1.0f);
		return;
	}

	float Alpha = m_fAccTime / m_fAppearTime;
	m_pUnit->SetAlpha(Alpha);

	m_fAccTime += DELTATIME;
}