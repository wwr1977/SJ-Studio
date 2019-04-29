#include "Precom.h"
#include "MonDead.h"
#include "BattleUnit.h"
#include "TurnOrderUI.h"
#include "UnitStatusBar.h"
#include "BossGauge.h"


CMonDead::CMonDead()
	:m_bUpdate(false)
	, m_bUITriger(false)
{
}


CMonDead::~CMonDead()
{
}

void CMonDead::StateReset()
{
	m_bUpdate = true;
	m_bUITriger = false;
	m_fAccTime = 0.0f;

	m_pUnit->AniRenOff();
	m_pUnit->ReactionRenOn();
	m_pUnit->SetVib(0.4f);
	if (m_pUnit->IsBossMonster() && nullptr != m_pUnit->GetPairBossGauge())
		m_pUnit->GetPairBossGauge()->FadeEvent(false);
		//m_pUnit->InitSelectSkillData();
}
void CMonDead::Update() 
{
	if (false == m_bUpdate)
		return;


	if (m_fAccTime >= 0.3f && m_fAccTime < 1.f) 
	{
		float Alpha = (1.f - m_fAccTime) / 0.7f;  
		m_pUnit->SetAlpha(Alpha);
		
		if (false == m_bUITriger) 
		{
			m_pUnit->GetPairTurnUI()->MonsterDeadEvent();
			m_bUITriger = true;
		}
	}
	else if (m_fAccTime >= 1.f)
	{
		m_bUpdate = false;
		m_pUnit->UnitOff();
		return;
	}

	m_fAccTime += DELTATIME;
}