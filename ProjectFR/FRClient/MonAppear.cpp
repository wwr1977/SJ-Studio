#include "Precom.h"
#include "MonAppear.h"
#include "BattleUnit.h"

CMonAppear::CMonAppear()
	:m_fAppearTime(0.6f), m_fAccTime(0.0f)
{
}


CMonAppear::~CMonAppear()
{
}
void CMonAppear::StateReset()
{
	m_fAccTime = 0.f;
	m_pUnit->InitAppear();
}
void CMonAppear::Update()
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