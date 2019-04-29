#include "Precom.h"
#include "BattleEndPhase.h"
#include "TurnMgr.h"
#include "GameMaster.h"
#include "EndingSceneBuilder.h"


CBattleEndPhase::CBattleEndPhase()
	:m_bBattleWin(false), m_fAccTime(0.f)
	, m_bSceneEffectTriger(false), m_bWinPose(false)
{
}


CBattleEndPhase::~CBattleEndPhase()
{
}
void CBattleEndPhase::StateReset()
{
	m_fAccTime = 0.f;
	m_bBattleWin = m_pTurnMgr->IsCharacterBattleWin();
	m_bSceneEffectTriger = false;
	m_bWinPose = false;
	m_pTurnMgr->BgmOff();
	
	if (true == m_bBattleWin) 
	{
		m_pTurnMgr->SetCurBattleBgmIndex(VICTORY_SOUND);
		m_pTurnMgr->PlayBgm();
		//m_pTurnMgr->CharacterWinEvent();
		m_pTurnMgr->TrigerResultWindow();
		m_pTurnMgr->CurUIOff();
	}
	else
		m_pTurnMgr->CharacterLoseEvent();

	CClientGlobal::GameMaster->BattleWin(m_bBattleWin);
}
void CBattleEndPhase::Update() 
{
	if (true == m_bBattleWin)
		UpdateWinEvent();
	else
		UpdateLoseEvent();
	
}

void CBattleEndPhase::UpdateWinEvent() 
{
	if (false == m_bWinPose && m_fAccTime >= 2.f) 
	{
		m_bWinPose = true;
		m_pTurnMgr->CharacterWinEvent();
	}
	
	if (true == m_bSceneEffectTriger ) 
	{
		if (true == m_pTurnMgr->IsSceneChangeEventEnd()) 
		{
			m_pTurnMgr->ResultWindowOff();
			m_pTurnMgr->BgmOff();
			m_pTurnMgr->GlobalCharacterDataUpdate(m_bBattleWin);
			CClientGlobal::PrevScene = CClientGlobal::BATTLESCENE;
			CClientGlobal::MainWindow->ChangeScene(CClientGlobal::SceneName[CClientGlobal::MAINSCENE]);
			return;
		}
	}

	if (false == m_bSceneEffectTriger && true == m_pTurnMgr->CheckChangeScene())
	{
		m_pTurnMgr->TrigerSceneChangeEvent();
		m_bSceneEffectTriger = true;
		return;
	}


	m_fAccTime += DELTATIME;
}
void CBattleEndPhase::UpdateLoseEvent() 
{

	if (true == m_bSceneEffectTriger)
	{
		if (true == m_pTurnMgr->IsSceneChangeEventEnd())
		{

			CClientGlobal::PrevScene = CClientGlobal::BATTLESCENE;
			if (CClientGlobal::StoryBattle) 
			{
				CClientGlobal::EndingBuilder->GameOverEvent();
				CClientGlobal::MainWindow->ChangeScene(CClientGlobal::SceneName[CClientGlobal::ENDINGSCENE]);
			}
			else 
				CClientGlobal::MainWindow->ChangeScene(CClientGlobal::SceneName[CClientGlobal::MAINSCENE]);
			
		}
	}

	if (false == m_bSceneEffectTriger &&  m_fAccTime >= 1.0f) 
	{
		m_pTurnMgr->TrigerSceneChangeEvent();
		m_bSceneEffectTriger = true;
		return;
	}


	m_fAccTime += DELTATIME;
}