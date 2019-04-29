#include "Precom.h"
#include "StandByPhase.h"
#include "TurnMgr.h"
#include <Camera.h>


CStandByPhase::CStandByPhase()
	: m_fAccTime(0.f)
	, m_BattleCamera(nullptr)
	, m_StandByCamPos(Vec3{0.f,150.f,7.f})
	, m_bBGMPlay(false)
{
}


CStandByPhase::~CStandByPhase()
{
}

void CStandByPhase::InitPhase()
{
	if (nullptr == m_pBattleScene)
		return;

	m_BattleCamera =  m_pBattleScene->GetMainCamCom();
}
void CStandByPhase::StateReset() 
{
	// 모든 배틀 오브젝트를 Off 시킨다.
	//m_pTurnMgr->AllObjectOff();
	m_fAccTime = 0.f;
	m_bBGMPlay = false;


	m_pTurnMgr->CurActiveObjectOff();
	m_pTurnMgr->InitBattleRound();

	m_pTurnMgr->SettingBattleObject();
	m_pTurnMgr->SetFirstBattleUnit();
	m_pTurnMgr->SkillBoardOff();
	m_pTurnMgr->CurUnitCollisionOff();

	if (nullptr != m_BattleCamera)
		m_BattleCamera->SetCameraPos(m_StandByCamPos);
}
void CStandByPhase::Update()
{
	// 현재 전투를 수행해야하는 오브젝트를 On & 배치
	if (m_fAccTime >= 2.f && m_fAccTime < 3.5f) 
	{
		if (nullptr == m_BattleCamera)
			return;

		float w = (m_fAccTime - 2.f) / 1.3f;
		Vec3 Zero = GAMEVEC::ZERO;
		Vec3 Pos = CSJMath::Vec3Lerp(&m_StandByCamPos, &Zero, w);

		m_BattleCamera->SetCameraPos(Pos);
	} 
	else if (m_fAccTime >= 3.5f && m_fAccTime < 4.f) 
	{
		if (false == m_bBGMPlay) 
		{
			m_pTurnMgr->PlayBgm();
			m_bBGMPlay = true;
		}
	}
	else if (m_fAccTime >= 4.5f) 
	{
		m_pTurnMgr->UnitPairUIOn();
		m_pTurnMgr->ChangePhase(PHASE_SELECT);
		m_BattleCamera->InitPos();
		return;
	}

	m_fAccTime += DELTATIME;
}
