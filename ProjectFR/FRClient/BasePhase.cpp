#include "Precom.h"
#include "BasePhase.h"
#include "TurnMgr.h"
#include "BattleUnit.h"


CBasePhase::CBasePhase()
	:m_pTurnMgr(nullptr), m_pEffectMgr(nullptr)
	, m_pSkillEffectQueue(nullptr), m_pSceneEffectQueue(nullptr)
	, m_pSkillEffect(nullptr), m_pSceneEffectCount(nullptr)
	, m_pBattleScene(nullptr), m_pSoundVector(nullptr)
	, m_pAniQueue(nullptr), m_pMoveQueue(nullptr), m_pSoundQueue(nullptr)
	, m_pAniScriptCount(nullptr), m_pMoveScriptCount(nullptr)
	, m_pSkillActionTime(nullptr), m_pInitPos(nullptr)
	, m_pInitPosMode(nullptr), m_pInitPosPivot(nullptr)
	, m_pCurSkillData(nullptr), m_pCurSkillLevelData(nullptr)
	, m_pHitData(nullptr), m_pHitDataQueue(nullptr)
	, m_pCurTarget(nullptr)
{
}


CBasePhase::~CBasePhase()
{
}
void CBasePhase::Init(void* _TurnMgr)
{
	if (nullptr == _TurnMgr) 
	{
		assert(nullptr);
		return;
	}

	m_pTurnMgr = (CTurnMgr*)_TurnMgr;
	m_pEffectMgr = m_pTurnMgr->GetEffectMgr();

	m_pSoundVector = m_pTurnMgr->GetSoundVector();
	m_pSkillEffect = m_pTurnMgr->GetSkillEffectVector();

	m_pAniQueue = m_pTurnMgr->GetAniQueue();
	m_pMoveQueue = m_pTurnMgr->GetMoveQueue();
	m_pSoundQueue = m_pTurnMgr->GetSoundQueue();
	m_pSkillEffectQueue = m_pTurnMgr->GetSkillEffectQueue();
	m_pSceneEffectQueue = m_pTurnMgr->GetSceneEffectQueue();
	
	m_pAniScriptCount = m_pTurnMgr->GetAnimationCount();
	m_pMoveScriptCount = m_pTurnMgr->GetMoveCount();
	m_pSceneEffectCount = m_pTurnMgr->GetSceneEffectCount();

	m_pSkillActionTime = m_pTurnMgr->GetSkillActionTime();
	
	m_pInitPosMode = m_pTurnMgr->GetInitPosMode();
	m_pInitPos = m_pTurnMgr->GetInitPos();
	m_pInitPosPivot = m_pTurnMgr->GetInitPosPivot();


	m_pBattleScene = m_pTurnMgr->GetScene();

	m_pCurSkillData = m_pTurnMgr->GetSkillData();
	m_pCurSkillLevelData = m_pTurnMgr->GetSkillLevelData();
	m_pHitData = m_pTurnMgr->GetHitDataList();
	m_pHitDataQueue = m_pTurnMgr-> GetHitDataQueue();

	m_pCurTarget = m_pTurnMgr->GetCurTargetVector();

	InitPhase();
}
void CBasePhase::InitPhase()
{

}
CTurnMgr* CBasePhase::GetTurnMgr()
{
	return m_pTurnMgr;
}