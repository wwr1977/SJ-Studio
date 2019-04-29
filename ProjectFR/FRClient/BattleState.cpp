#include "Precom.h"
#include "BattleState.h"
#include"BattleUnit.h"

CBattleState::CBattleState()
	:m_UnitName(_T("")),m_pUnit(nullptr)
	,m_fAccTime(0.0f), m_UnitSoundPlayer(nullptr)
	, m_fActionTime(0.0f)
{

}


CBattleState::~CBattleState()
{
	
}
void CBattleState::Init(void* _pLogic) 
{
	m_pUnit = (CBattleUnit*)_pLogic;
	m_UnitSoundPlayer = m_pUnit->GetUnitSoundPlayer();
	m_UnitName = m_pUnit->GetUnitName();
}
CBattleUnit* CBattleState::GetPlayer()
{
	return m_pUnit;
}

//void CBattleState::SetActionScript(ACTIONSCRIPT* _ActionScript)
//{
//	if (nullptr == _ActionScript) 
//	{
//		return;
//	}
//	
//	ClearAllQueue();
//	m_vecSoundScript.clear();
//	m_iAniScriptCount = 0;
//	m_iMoveScriptCount = 0;
//
//	for (int i = 0; i < min(_ActionScript->AniCount,ACTIONSCRIPT::MAXANISCRIPTCOUNT); i++)
//	{
//		m_qAniScript.push(_ActionScript->AniSctipt[i]);
//		++m_iAniScriptCount;
//	}
//
//
//	for (int i = 0; i < min(_ActionScript->MoveCount,ACTIONSCRIPT::MAXMOVECOUNT); ++i)
//	{
//		m_qMoveScript.push(_ActionScript->CharMoveData[i]);
//		++m_iMoveScriptCount;
//	}
//
//	for (int i = 0; i < min(_ActionScript->CharSoundCount,ACTIONSCRIPT::MAXCHARSOUNDCOUNT); ++i)
//	{
//		m_vecSoundScript.push_back(SoundData(_ActionScript->CharSound[i]));
//	}
//	
//	m_fActionTime = _ActionScript->TotalSkillTime;
//	
//	if (0 != m_iAniScriptCount)
//		m_CurAniScript = m_qAniScript.front();
//	else 
//		m_CurAniScript = ANIMATIONSCRIPT();
//	
//
//	/*if (0 != m_iMoveScriptCount)
//		m_CurMoveScript = m_qMoveScript.front();
//	else*/
//
//	m_CurMoveScript = MOVEDATA();
//	m_pUnit->SetPos(_ActionScript->InitPos);
//
//
//}
//void CBattleState::ClearAllQueue()
//{
//	queue<ANIMATIONSCRIPT> EmptyAni;
//	queue<MOVEDATA> EmptyMove;
//	queue<tstring>	EmptySound;
//
//	swap(m_qAniScript, EmptyAni);
//	swap(m_qMoveScript, EmptyMove);
//	swap(m_SoundQueue, EmptySound);
//}
//void CBattleState::ClearQueue(const SCRIPTQUEUE& _Index)
//{
//	switch (_Index)
//	{
//	case CBattleState::QUEUE_ANI:
//	{
//		queue<ANIMATIONSCRIPT> EmptyQueue;
//		swap(m_qAniScript, EmptyQueue);
//	}
//	break;
//	case CBattleState::QUEUE_MOVE:
//	{
//		queue<MOVEDATA> EmptyQueue;
//		swap(m_qMoveScript, EmptyQueue);
//	}
//	break;
//	case CBattleState::QUEUE_SOUND:
//	{
//		queue<tstring>	EmptyQueue;
//		swap(m_SoundQueue, EmptyQueue);
//	}
//	break;
//	}
//}
//void CBattleState::ClearQueue(const int& _Index) 
//{
//	return ClearQueue((SCRIPTQUEUE)_Index);
//}