#include "stdafx.h"
#include "SkillMgr.h"
#include "EditBattleUnit.h"
#include "EffectDlg.h"
#include "ActionScriptDlg.h"
#include "EditEffect.h"
#include "EditEffectMgr.h"
#include "EditEffect.h"
#include "EditDemageFont.h"
#include "EditDemageFontGroup.h"


CSkillMgr::CSkillMgr()
	:m_pSkillScene(nullptr), m_pSkillSceneBuilder(nullptr)
	, m_fAccTime(0.0f), m_SkillMgrState(STATE_INIT)
	, m_CharacterPos(Vec3(0.0f, 0.0f, 0.0f)), m_CurCharIndex(MAXBATTLECHARACTER)
	, m_MonsterPos(Vec3(0.0f, 0.0f, 0.0f))
	, m_InitMonName(_T("BabyTurtle"))
	, m_CurMonsterIndex(0)
	, m_SkillMgrSpeaker(nullptr)
	, m_CurActionUnit(nullptr)
	, m_iAniScriptCount(0)
	, m_iMoveScriptCount(0)
	, m_CurAniScript(ANIMATIONSCRIPT())
	, m_CurMoveScript(MOVEDATA()), m_CurHideScript(MOVEDATA())
	, m_bSkillStandby(false), m_bCreateUnit(false)
	, m_iCurOnDemageCount(0)
	, m_bAllReturn(false)
	, m_CurSkillData(SKILLDATA())
	, m_CurSkillLevelData(LEVELDATA())
	, m_InitPosMode(0)
	, m_InitFixPos(Vec2{ 0.0f,0.0f }), m_InitPosPivot(Vec2{ 0.0f,0.0f })
	, m_ProgressState(STATE_NONEPROGRESS)
	, m_CurSkillLevel(0), m_MonTargetUnit(nullptr)
	, m_bCharTurn(true)
{
	memset(m_ThreadHandle, 0, sizeof(HANDLE)*FOLDERMAX);
	m_pCurEffect.assign(ACTIONSCRIPT::MAXEFFECTCOUNT, nullptr);

	m_vecSkill.clear();
	m_vecSkillLevelData.clear();
	m_HitData.clear();
	CEditNumberUI::InitFontNumberData();
	CEditDemageFontGroup::SetSkillMgr(this);
	Off();
}
CSkillMgr::CSkillMgr(CGameScene* _Scene, CSceneBuilder* _Builder)
	: m_pSkillScene(_Scene), m_pSkillSceneBuilder(_Builder)
	, m_fAccTime(0.0f), m_SkillMgrState(STATE_INIT)
	, m_CharacterPos(Vec3(0.0f, 0.0f, 0.0f)), m_CurCharIndex(MAXBATTLECHARACTER)
	, m_MonsterPos(Vec3(0.0f, 0.0f, 0.0f))
	, m_InitMonName(_T("BabyTurtle"))
	, m_CurMonsterIndex(0)
	, m_CurActionUnit(nullptr)
	, m_iAniScriptCount(0)
	, m_iCurSceneEffectCount(0)
	, m_CurAniScript(ANIMATIONSCRIPT())
	, m_CurMoveScript(MOVEDATA()), m_CurHideScript(MOVEDATA())
	, m_CurSkillData(SKILLDATA())
	, m_CurSkillLevelData(LEVELDATA())
	, m_InitPosMode(0)
	, m_InitFixPos(Vec2{ 0.0f,0.0f }), m_InitPosPivot(Vec2{ 0.0f,0.0f })
	, m_ProgressState(STATE_NONEPROGRESS)
	, m_CurSkillLevel(0)
{
	if (nullptr == m_pSkillScene || nullptr == m_pSkillSceneBuilder)
		assert(nullptr);

	memset(m_ThreadHandle, 0, sizeof(HANDLE)*FOLDERMAX);
	m_pCurEffect.assign(ACTIONSCRIPT::MAXEFFECTCOUNT, nullptr);
	
	CEditNumberUI::InitFontNumberData();
	CEditDemageFontGroup::SetSkillMgr(this);
	Off();
	m_vecSkill.clear();
	m_vecSkillLevelData.clear();
}

CSkillMgr::~CSkillMgr()
{

}
void CSkillMgr::ClearAllQueue()
{
	queue<ANIMATIONSCRIPT> EmptyAni;
	queue<MOVEDATA> EmptyMove;
	queue<tstring>	EmptySound;
	queue<SKILLEFFECTDATA>	EmpSkillEffect;
	queue<SCENEEFFECTDATA>	EmpSceneEffect;
	queue<HITDATA>			EmpHitData;

	swap(m_qAniScript, EmptyAni);
	swap(m_qMoveScript, EmptyMove);
	swap(m_SoundQueue, EmptySound);
	swap(m_qSkillEffect, EmpSkillEffect);
	swap(m_qSceneEffect, EmpSceneEffect);
	swap(m_qHitData, EmpHitData);

}
void CSkillMgr::SetTarget(SPTR<CEditBattleUnit> Target)
{
	if (nullptr == Target)
		return;

	m_vecCurTurnTarget.clear();

	m_vecCurTurnTarget.push_back(Target);
}
void CSkillMgr::SetCurSkillLevel(const UINT& _Level)
{
	if (_Level >= LEVELDATA::MAXSKILLLEVEL)
		return;
	
	m_CurSkillLevel = _Level;
}
bool CSkillMgr::SetSkill(const size_t& _SkillNo)
{
	return SetSkill(_SkillNo, m_CurSkillLevel);
}
bool CSkillMgr::SetSkill(const size_t& _SkillNo, const size_t& _SkillLevel)
{
	m_bSkillStandby = true;
	
	if (_SkillNo >= m_vecSkill.size())
		return false;
	
	m_CurSkill = m_vecSkill[_SkillNo];
	m_CurSkillLevel = (UINT)_SkillLevel;

	ClearAllQueue();

	SetActionScript(_SkillNo);
	SetSkillData(_SkillNo, _SkillLevel);
	

	if (0 != m_iAniScriptCount)
	{
		if (false == m_qAniScript.empty())
			m_CurAniScript = m_qAniScript.front();
	}
	else
		m_CurAniScript = ANIMATIONSCRIPT();


	m_CurHideScript = MOVEDATA();
	m_CurMoveScript = MOVEDATA();

	return true;
}
void CSkillMgr::SetActionScript(ACTIONSCRIPT* _ActionScript)
{
	if (nullptr == _ActionScript)
	{
		return;
	}

	//CharacterOn(_ActionScript->CharIndex);

	m_vecSkillEffect.clear();
	m_vecSoundScript.clear();
	
	for (size_t i = 0; i < m_pCurEffect.size(); i++)
	{
		if (nullptr != m_pCurEffect[i]) 
		{
			if(true == m_pCurEffect[i]->IsPlay())
				m_pCurEffect[i]->EffectOff();
			m_pCurEffect[i] = nullptr;
		}
	}

	m_iAniScriptCount = 0;
	m_iMoveScriptCount = 0;
	m_iCurSceneEffectCount = 0;


	for (UINT i = 0; i < min(_ActionScript->AniCount, ACTIONSCRIPT::MAXANISCRIPTCOUNT); i++)
	{
		m_qAniScript.push(_ActionScript->AniSctipt[i]);
		++m_iAniScriptCount;
	}

	// MoveData를 큐에 넣을때 StartTime 빠른 순으로 넣어주기 위한 작업
	list<MOVEDATA> TempMoveData;
	TempMoveData.clear();

	for (UINT i = 0; i < min(_ActionScript->MoveCount, ACTIONSCRIPT::MAXMOVECOUNT); ++i)
	{
		TempMoveData.push_back(_ActionScript->CharMoveData[i]);
		++m_iMoveScriptCount;
	}

	TempMoveData.sort();

	for (int i = 0; i < m_iMoveScriptCount; i++)
	{
		m_qMoveScript.push(TempMoveData.front());
		TempMoveData.pop_front();
	}

	for (UINT i = 0; i < min(_ActionScript->CharSoundCount, ACTIONSCRIPT::MAXCHARSOUNDCOUNT); ++i)
	{
		m_vecSoundScript.push_back(SoundData(_ActionScript->CharSound[i]));
	}

	m_fSkillActionTime = _ActionScript->TotalSkillTime;

	for (UINT i = 0; i < min(_ActionScript->EffectCount, ACTIONSCRIPT::MAXEFFECTCOUNT); i++)
	{
		m_vecSkillEffect.push_back(_ActionScript->SkillEffect[i]);
	}

	for (UINT i = 0; i < min(_ActionScript->SceneEffectCount, ACTIONSCRIPT::MAXSCENEEFFECTCOUNT); i++)
	{
		m_qSceneEffect.push(_ActionScript->SceneEffectData[i]);
		++m_iCurSceneEffectCount;
	}

}
void CSkillMgr::SetActionScript(const size_t& _SkillNo)
{
	if (_SkillNo >= m_vecSkill.size())
	{
		return;
	}

	return SetActionScript(&m_vecSkill[_SkillNo].SkillActionScript);
}
bool CSkillMgr::SetSkillData(const size_t& _SkillNo, const size_t& _SkillLevel)
{
	if (_SkillNo >= m_vecSkill.size() ||
		_SkillLevel >= m_vecSkillLevelData[_SkillNo].size())
	{
		return false;
	}

	// Queue는 SetSkill을 호출하면 시작하자마자 Queue부터 비우고 시작
	// SetSkill(_Index) -> SetActionScript(_Index)
	//					-> SetSkillData(_Index) 
	//					-> 순으로 호출

	m_HitData.clear();

	m_CurSkillData = m_vecSkill[_SkillNo].SkillData;

	// SkillLevelData[Skill번호][Unit의 현재 스킬레벨]
	m_CurSkillLevelData = m_vecSkillLevelData[_SkillNo][_SkillLevel];

	for (size_t i = 0; i < m_CurSkillLevelData.HitDataCount; i++)
	{
		m_HitData.push_back(m_CurSkillLevelData.HitData[i]);
		m_HitData.back().Apply = false;
	}
	// HitData의 StartTime 순으로 정렬
	m_HitData.sort();

	m_InitPosMode = (size_t)m_CurSkillData.InitPosMode;
	m_InitFixPos = m_CurSkillData.InitFixPos;
	m_InitPosPivot = m_CurSkillData.InitPosPivot;

	m_bCharTurn = m_CurSkillData.CharacterSkill;

	return true;
}
void CSkillMgr::ScriptUpdate()
{
	// 애니메이션 스크립트 & 애니메이션 업데이트
	 while (0 != m_iAniScriptCount && m_CurAniScript.StartTime <= m_fAccTime)
	{
		m_CurActionUnit->ChangeAni(m_CurAniScript.AnimationIndex);
		m_qAniScript. pop();
		--m_iAniScriptCount;

		if (0 != m_iAniScriptCount)
		{
			m_CurAniScript = m_qAniScript.front();
		}
	}

	// Move Script 업데이트
	while (0 < m_iMoveScriptCount && m_qMoveScript.front().MoveStartTime <= m_fAccTime)
	{
		MOVEDATA PopData = m_qMoveScript.front();
		m_qMoveScript.pop();
		--m_iMoveScriptCount;

		if (MOVEDATA::MOVEMODE_MOVE == PopData.MoveMode)
		{
			m_CurMoveScript = PopData;

			if (0 >= m_iMoveScriptCount)
			{
				break;
			}

			if (m_qMoveScript.front().MoveStartTime <= m_fAccTime
				&&	MOVEDATA::MOVEMODE_MOVE != m_qMoveScript.front().MoveMode)
			{
				m_CurHideScript = m_qMoveScript.front();
				m_qMoveScript.pop();
				--m_iMoveScriptCount;
			}
		}
		else
		{
			m_CurHideScript = PopData;

			if (0 >= m_iMoveScriptCount)
			{
				break;
			}

			if (m_qMoveScript.front().MoveStartTime <= m_fAccTime
				&&	MOVEDATA::MOVEMODE_MOVE == m_qMoveScript.front().MoveMode)
			{
				m_CurMoveScript = m_qMoveScript.front();
				m_qMoveScript.pop();
				--m_iMoveScriptCount;
			}
		}

	}


	// 사운드를 Queue에 대기
	for (int i = 0; i < min((int)m_vecSoundScript.size(), ACTIONSCRIPT::MAXCHARSOUNDCOUNT); i++)
	{
		if (true == m_vecSoundScript[i].Playing)
		{
			continue;
		}

		if (m_fAccTime >= m_vecSoundScript[i].StartTime)
		{
			m_SoundQueue.push(m_vecSoundScript[i].SoundName);
			m_vecSoundScript[i].Playing = true;
		}
	}


	// SkillEffect Script Update
	for (size_t i = 0; i < m_vecSkillEffect.size(); ++i)
	{
		if (true == m_vecSkillEffect[i].Apply)
		{
			continue;
		}

		if (m_fAccTime >= m_vecSkillEffect[i].StartTime)
		{
			m_vecSkillEffect[i].PIndex = i;
			m_qSkillEffect.push(m_vecSkillEffect[i]);
			m_vecSkillEffect[i].Apply = true;
		}
	}

	// SceneEffect ScriptUpdate & SceneEffectUpdate 
	if (0 != m_iCurSceneEffectCount)
	{
		if (m_qSceneEffect.front().SceneEffectStartTime <= m_fAccTime)
		{
			if (nullptr != m_pSkillScene)
			{
				m_pSkillScene->SetSceneEffect(m_qSceneEffect.front(), m_CurActionUnit->GetUnitMiddlePos());
			}
			m_qSceneEffect.pop();
			--m_iCurSceneEffectCount;
		}

	}

}
void CSkillMgr::ActionUpdate()
{
	// Move  & Hide Update
	HideUpdate();
	MoveUpdate();


	// Sound Update
	while (false == m_SoundQueue.empty())
	{
		m_CurActionUnit->PlaySound(m_SoundQueue.front());
		m_SoundQueue.pop();
	}

	// SkillEffect Update
	while (false == m_qSkillEffect.empty())
	{
		SKILLEFFECTDATA SkillEffect = m_qSkillEffect.front();
		m_qSkillEffect.pop();

		//SPTR<CEffect> Effect = m_pEffectMgr->FindEffect(SkillEffect.EffectIndex);
		m_pCurEffect[SkillEffect.PIndex] = CEditGlobal::EffectMgr->EffectOn(SkillEffect.EffectIndex);

		if (nullptr == m_pCurEffect[SkillEffect.PIndex])
		{
			continue;
		}

		Vec2 SkillCasterPos = Vec2{ m_CurActionUnit->GetPos().x,m_CurActionUnit->GetPos().y };

		m_pCurEffect[SkillEffect.PIndex]->SetSkillEffectData(SkillEffect, SkillCasterPos);
	}

	SkillEffectUpdate();

}
void CSkillMgr::HideUpdate()
{
	if (m_CurHideScript.MoveTime == 0.0f &&
		m_CurHideScript.HideTime == 0.0f)
	{
		return;
	}

	float AlphaRatio = (m_fAccTime - m_CurHideScript.MoveStartTime) / m_CurHideScript.HideTime;

	if (AlphaRatio > 1.0f)
	{
		m_CurHideScript = MOVEDATA();
		return;
	}

	switch (m_CurHideScript.MoveMode)
	{
	case MOVEDATA::MOVEMODE_HIDE:
		m_CurActionUnit->SetAlpha(1.0f - AlphaRatio);
		break;
	case MOVEDATA::MOVEMODE_SHOW:
		m_CurActionUnit->SetAlpha(AlphaRatio);
		break;
	}

}
void CSkillMgr::MoveUpdate()
{
	// Character를 자신의 TeamPos로 이동
	if (true == m_CurMoveScript.bSetTeamPos)
	{
		Vec2 CharPos = Vec2{ m_CharacterPos.x,m_CharacterPos.y };
		m_CurActionUnit->SetPos(CharPos);
		m_CurActionUnit->SetRenPivot(Vec2{ 0.0f,0.0f });
		m_CurMoveScript = MOVEDATA();
		return;
	}

	// Character를 스킬 InitPos로 이동
	if (true == m_CurMoveScript.bSetInitPos)
	{
		m_CurActionUnit->SetRenPivot(Vec2{ 0.0f,0.0f });
		m_CurMoveScript = MOVEDATA();
		return;
	}

	if (m_CurMoveScript.MoveStartTime == 0.0f &&
		m_CurMoveScript.MoveTime == 0.0f)
	{
		return;
	}


	float Weight = (m_fAccTime - m_CurMoveScript.MoveStartTime) / m_CurMoveScript.MoveTime;

	if (Weight > 1.0f)
	{
		m_CurMoveScript = MOVEDATA();

		return;
	}

	Vec2 MovePos = CSJMath::Vec2Lerp(&m_CurMoveScript.MoveStartPos, &m_CurMoveScript.MoveEndPos, Weight);

	m_CurActionUnit->SetRenPivot(MovePos);
}
void CSkillMgr::SkillEffectUpdate()
{
	for (size_t i = 0; i < m_vecSkillEffect.size(); i++)
	{
		if (false == m_vecSkillEffect[i].Apply)
			continue;

		// 해당 이팩트의 MoveTime == 0.0f 이면 동작 X
		if (0 == m_vecSkillEffect[i].MoveData.MoveTime)
			continue;

		// 위치 이동 가중치가 1.0f 이상하면 이미 
		// 이전프레임때 도착지점에 도달한경우 이므로 적용이 필요 X
		if (MOVEDATA::MOVEMODE_MOVE == m_vecSkillEffect[i].MoveData.MoveMode)
			EffectMoveUpdate(i);
		else
			EffectHideUpdate(i);
		}
}
void CSkillMgr::EffectMoveUpdate(const size_t& _EffectIndex)
{
	if (_EffectIndex >= m_vecSkillEffect.size())
		return;

	float EMoveTime = m_vecSkillEffect[_EffectIndex].MoveData.MoveTime;
	float EHideTime = m_vecSkillEffect[_EffectIndex].MoveData.HideTime;
	float EStartTime = m_vecSkillEffect[_EffectIndex].MoveData.MoveStartTime;

	Vec2 Start = m_vecSkillEffect[_EffectIndex].MoveData.MoveStartPos;
	Vec2 End = m_vecSkillEffect[_EffectIndex].MoveData.MoveEndPos;

	float Weight = (m_fAccTime - EStartTime) / EMoveTime;

	if (1.2f < Weight)
		return;

	if (nullptr == m_pCurEffect[m_vecSkillEffect[_EffectIndex].PIndex])
		return;

	// Effect의 움직임을 업데이트
	Vec2 MovePos = CSJMath::Vec2Lerp(&Start, &End, Weight);
	m_pCurEffect[m_vecSkillEffect[_EffectIndex].PIndex]->SetRenPivot(MovePos);

}
void CSkillMgr::EffectHideUpdate(const size_t& _EffectIndex)
{
	if (_EffectIndex >= m_vecSkillEffect.size())
		return;

	int HideMode = m_vecSkillEffect[_EffectIndex].MoveData.MoveMode;
	size_t PIndex = m_vecSkillEffect[_EffectIndex].PIndex;

	if (PIndex >= m_pCurEffect.size() || nullptr == m_pCurEffect[PIndex])
		return;

	SPTR<CEditEffect> PEffect = m_pCurEffect[PIndex];
		
	float EMoveTime = m_vecSkillEffect[_EffectIndex].MoveData.MoveTime;
	float EHideTime = m_vecSkillEffect[_EffectIndex].MoveData.HideTime;
	float EStartTime = m_vecSkillEffect[_EffectIndex].MoveData.MoveStartTime;

	if (EMoveTime == 0.0f &&  EHideTime == 0.0f)
		return;
	
	float AlphaRatio = (m_fAccTime - EStartTime) / EHideTime;

	if (AlphaRatio > 1.2f)
		return;

	switch (HideMode)
	{
	case MOVEDATA::MOVEMODE_HIDE:
		PEffect->SetAlpha(1.0f - AlphaRatio);
		break;
	case MOVEDATA::MOVEMODE_SHOW:
		PEffect->SetAlpha(AlphaRatio);
		break;
	}

}
void CSkillMgr::SkillDataTimingCheck()
{
	// HitData는 SetSkillData()에서 Sort작업을 거쳣으므로
	// 리스트의 가장 앞의 데이터를 기준으로 확인작업 시작
	while (true != m_HitData.empty())
	{
		if (m_HitData.front().StartTime <= m_fAccTime)
		{
			m_qHitData.push(m_HitData.front());
			m_HitData.pop_front();
		}
		else
			break;
	}

	//while(m_pHit)
}
// 스킬데이터 업데이트에서 활성화된 Hit 데이터를 업데이트
void CSkillMgr::SkillDataUpdate()
{

	while (false == m_qHitData.empty())
	{
		HitDataUpdate(m_qHitData.front());
		m_qHitData.pop();
	}
	
}
void CSkillMgr::HitDataUpdate(const HITDATA& _HitData)
{
	// HitData 는 3가지 Index와 DemageFont 출력정보를 갖는다


	// Index(size_t)가 -1인 경우 출력하지 않는다
	//1. HitEffect 
	if (MAXINDEX != _HitData.HitEffectIndex)
	{

		if (HITDATA::POS_CASTER == _HitData.PosOption)
		{
			SPTR<CEditEffect> Hit = CEditGlobal::EffectMgr->EffectOn(_HitData.HitEffectIndex);
			Hit->SetHitEffectData(_HitData);
			Vec2 CasterPos = Vec2{ m_CurActionUnit->GetPos().x ,m_CurActionUnit->GetPos().y } +_HitData.HitEffectPivot;
			Hit->SetPos(CasterPos);
			
			if (_HitData.HitEffectCustomSize.x > 0.0f && _HitData.HitEffectCustomSize.y > 0.0f)
				Hit->SetCustomSize(_HitData.HitEffectCustomSize);
			else
				Hit->SetImageScale(_HitData.HitEffectImageMag);
		}
		else if(HITDATA::POS_TARGET == _HitData.PosOption)
		{
			for (size_t i = 0; i < m_vecCurTurnTarget.size(); i++)
			{
				SPTR<CEditEffect> Hit = CEditGlobal::EffectMgr->EffectOn(_HitData.HitEffectIndex);
				Hit->SetHitEffectData(_HitData);
				Vec2 TargetPos = Vec2{ m_vecCurTurnTarget[i]->GetPos().x ,m_vecCurTurnTarget[i]->GetPos().y } +_HitData.HitEffectPivot;
				Hit->SetPos(TargetPos);
				
				if (_HitData.HitEffectCustomSize.x > 0.0f && _HitData.HitEffectCustomSize.y > 0.0f)
					Hit->SetCustomSize(_HitData.HitEffectCustomSize);
				else
					Hit->SetImageScale(_HitData.HitEffectImageMag);
			}
		}
		else if (HITDATA::POS_TARGETMIDDLE == _HitData.PosOption)
		{
			for (size_t i = 0; i < m_vecCurTurnTarget.size(); i++)
			{
				SPTR<CEditEffect> Hit = CEditGlobal::EffectMgr->EffectOn(_HitData.HitEffectIndex);
				Hit->SetHitEffectData(_HitData);
				Vec2 TargetPos = Vec2{ m_vecCurTurnTarget[i]->GetUnitMiddlePos().x ,m_vecCurTurnTarget[i]->GetUnitMiddlePos().y } +_HitData.HitEffectPivot;
				Hit->SetPos(TargetPos);

				if (_HitData.HitEffectCustomSize.x > 0.0f && _HitData.HitEffectCustomSize.y > 0.0f)
					Hit->SetCustomSize(_HitData.HitEffectCustomSize);
				else
					Hit->SetImageScale(_HitData.HitEffectImageMag);
			}
		}

	}


	//2. HitSound
	if (MAXINDEX != _HitData.HitSoundIndex)
	{
		for (size_t i = 0; i < m_vecCurTurnTarget.size(); ++i)
		{
			m_vecCurTurnTarget[i]->PlaySound(CEditGlobal::GetHitSound(_HitData.HitSoundIndex).GetString());
		}
	}

	//3. HitReaction
	if (MAXINDEX != _HitData.ReActionIndex)
	{
		for (size_t i = 0; i < m_vecCurTurnTarget.size(); ++i)
		{
			if (BATTLE_REACTION != m_vecCurTurnTarget[i]->GetCurUnitState())
				m_vecCurTurnTarget[i]->SetReAction();

			m_vecCurTurnTarget[i]->ReActionEvent(_HitData.ReActionIndex);
		}
	}


	// 변경 4. DemageFont 출력 - > DemageFontGroup에서 한개씩 뽑아준다.
	if(0.0f !=_HitData.HitDemageMag )
		for (size_t i = 0; i < m_CurDemageGroup.size(); i++)
		{
			if (nullptr != m_CurDemageGroup[i])
			{
				if (false == m_CurDemageGroup[i]->DemageFontOn())
				{
					m_CurDemageGroup[i] = nullptr;
				}
			}
		}
}
const bool CSkillMgr::IsSkillProgress()
{
	if (STATE_SKILLPROGRESS == m_ProgressState)
		return true;

	return false;
}
const bool CSkillMgr::CreateAllUnit()
{
	return m_bCreateUnit;
}
const SKILLMGRSTATE CSkillMgr::GetSkillMgrState()
{
	return m_SkillMgrState;
}
vector<SKILL>* CSkillMgr::GetSkillVector()
{
	return &m_vecSkill;
}
vector<LEVELDATA>* CSkillMgr::GetVecLevelData(const int& _SkillLevel)
{
	if (m_vecSkillLevelData.size() <= _SkillLevel)
		return nullptr;

	return &m_vecSkillLevelData[_SkillLevel];
}
const UINT CSkillMgr::GetCurSkillNo()
{
	return  m_CurSkill.SkillNo;
}
const size_t CSkillMgr::GetSkillCount()
{
	return m_vecSkill.size();
}
const SKILL CSkillMgr::GetSkill(const int& _SkillNo)
{
	if (_SkillNo >= m_vecSkill.size())
		return SKILL();
	
	return m_vecSkill[_SkillNo];
}
void CSkillMgr::InitSkillMgr()
{
	//ThreadTextureLoading();

	POINT WndSize = CEditGlobal::MainWindow->WndSize();

	Vec3 RightDown = { (float)WndSize.x* 0.5f,(float)WndSize.y*-0.5f,80.0f };
	Vec3 LeftDown = { (float)WndSize.x* -0.5f,(float)WndSize.y*-0.5f,80.0f };

	m_CharacterPos = Vec3{ 408.0f,0.0f,100.0f };
	m_MonsterPos  = Vec3{ -408.0f,0.0f,100.0f };
	



	m_SkillMgrState = STATE_LOADING;
	SerialTextureLoading();
	CreateAniSprite();
	AnimationDataLoaing();
	m_SkillMgrState = STATE_COMPLETE;

}
void CSkillMgr::ThreadTextureLoading()
{
	int LoadingCount = 0;

	for (size_t i = 0; i < EFFECT; i++)
	{
		if (FALSE == CEditGlobal::vecFolderData[i].CheckLoadingStart)
		{
			m_ThreadHandle[LoadingCount] = CThreadMgr::CreateGlobvalThread(LoadingTexture, &CEditGlobal::vecFolderData[i]);
			CEditGlobal::vecFolderData[i].CheckLoadingStart = TRUE;
			++LoadingCount;
		}
	}

	WaitForMultipleObjects(LoadingCount, m_ThreadHandle, TRUE, INFINITE);
	memset(m_ThreadHandle, 0, sizeof(HANDLE)*FOLDERMAX);

	CEditGlobal::Update();
}
void CSkillMgr::SerialTextureLoading()
{
	for (size_t i = 0; i < EFFECT; i++)
	{
		if (FALSE == CEditGlobal::vecFolderData[i].CheckLoadingStart)
		{
			LoadingTexture(&CEditGlobal::vecFolderData[i]);
			CEditGlobal::vecFolderData[i].CheckLoadingStart = TRUE;
		}
	}

	CEditGlobal::CurLoadTexMap();
}
void CSkillMgr::CreateAniSprite()
{
	for (size_t i = 0; i < EFFECT; i++)
	{
		CGameFile RFile = CGameFile(_T("EditData"), CGameFile::vecSpriteDataFileName[i]);

		if (FALSE == RFile.Open(_T("rb")))
			continue;

		vector<MULTISPRITEDATA> vecRegData;
		vecRegData.reserve(128);

		if (FALSE == CEditGlobal::EditResourceMgr->LoadSpriteData(RFile.GetFile(), (ANISPRITETYPE)i, &vecRegData))
			continue;

		for (size_t i = 0; i < vecRegData.size(); i++)
		{
			CEditGlobal::EditResourceMgr->CreateEditGameSprite(
				vecRegData[i].TexKey
				, vecRegData[i].MultiSpriteKey
				, { vecRegData[i].StartPosX,vecRegData[i].StartPosY }
				, { vecRegData[i].SizeX,vecRegData[i].SizeY }
			);


			ANIDATA AniData(vecRegData[i].MultiSpriteKey, vecRegData[i].MultiSpriteKey);
			AniData.SettingFullRage({ 0,vecRegData[i].ImageCount - 1 });

			CEditGlobal::MBaseAniCom->CreateEditAnimation(AniData);
		}
	}
	
}
void CSkillMgr::AnimationDataLoaing()
{
	for (size_t i = 0; i < EFFECT; i++)
	{
		CGameFile RFile = CGameFile(_T("EditData"), CGameFile::vecAniDataFileName[i]);

		if (FALSE == RFile.Open(_T("rb")))
			continue;
		
		map<tstring, ANIMATIONDATA*> TempMap;
		ANIMATIONDATA::ReadAnimationData(RFile.GetFile(), &TempMap);
		
		if (0 >= TempMap.size())
			continue;
		
		map<tstring, ANIMATIONDATA*>::iterator Iter = TempMap.begin();

		int RegAniCount = 0;

		for (; Iter != TempMap.end(); ++Iter)
		{
			CAniRenderer::CAnimation* NewAni;
			
			if (nullptr != (NewAni = CEditGlobal::AAnimationCom->CreateEditAnimation(Iter->second->GetAniData())))
			{
				if ((int)MAXANISTYLE == Iter->second->AniStyle)
				{
					int a = 0;
				}
				NewAni->SetAniStyle((ANISTYLE)Iter->second->AniStyle);
				++RegAniCount;
			}
		}
	
		CEditGlobal::AnimationDataMap.insert(TempMap.begin(), TempMap.end());
	}
	
}
void CSkillMgr::CraeteEffect()
{
	CGameFile LoadFile(_T("EditData"), _T("EffectData.bin"));

	if (FALSE == LoadFile.Open(_T("rb")))
		return;

	vector<EFFECTDATA> TempVec;
	TempVec.reserve(64);
	EFFECTDATA::LoadDataToFile(LoadFile.GetFile(), &TempVec);
	BOOL Check = CEditGlobal::EffectMgr->LoadEffectData(LoadFile.GetFile(), &TempVec);

	if (Check == FALSE)
	{
		int a = 0;
		assert(nullptr);
	}

	if(nullptr!= CEditGlobal::EffectDlg)
		CEditGlobal::EffectDlg->CompleteEffectSetting();
}
bool CSkillMgr::CreateBattleUnit()
{
	if ( STATE_COMPLETE != m_SkillMgrState ||  true == m_bCreateUnit)
		return false;


	for (size_t i = 0; i < MAXBATTLECHARACTER; i++)
	{
		CCriSectionObject<CGameScene>();
		CCriSectionObject<CActObject>();
		CActObject* Unit = CActObject::CreateActObject(CEditGlobal::CharacterName[i].GetString(), m_pSkillScene);
		Unit->Off();
		CEditBattleUnit* UnitCom = Unit->CreateCom<CEditBattleUnit>((CharacterIndex)i);

		UnitCom->SetPos(m_CharacterPos);
		UnitCom->SetLevel(99);

		if (nullptr != UnitCom)
			m_vecCharacter.push_back(UnitCom);
		
	}
	
	CCriSectionObject<CGameScene>();
	CCriSectionObject<CActObject>();
	CActObject* Unit = CActObject::CreateActObject(CEditGlobal::CharacterName[DemonRain].GetString(), m_pSkillScene);
	m_MonTargetUnit = Unit->CreateCom<CEditBattleUnit>((CharacterIndex)DemonRain);

	m_MonTargetUnit->SetPos(m_CharacterPos);
	m_MonTargetUnit->SetLevel(99);
	Unit->SetImageScale(2.0f);
	Unit->Off();


	set<CString>::iterator Start =  CEditGlobal::MonsterName.begin();
	set<CString>::iterator End = CEditGlobal::MonsterName.end();

	for (;Start!= End ; ++Start)
	{
		CCriSectionObject<CGameScene>();
		CCriSectionObject<CActObject>();
		CActObject* Unit = CActObject::CreateActObject(Start->GetString(), m_pSkillScene);
		Unit->Off();
		CEditBattleUnit* UnitCom = Unit->CreateCom<CEditBattleUnit>(Start->GetString());

		UnitCom->SetPos(m_MonsterPos);

		if (nullptr != UnitCom)
			m_vecMonster.push_back(UnitCom);
	}

	InitMonsterOn();
	m_bCreateUnit = true;

	CreateDemageFont();
	m_ProgressState = STATE_KEYCHECK;
	return true;
}
bool CSkillMgr::CreateDemageFont()
{
	for (size_t i = 0; i < MAXDEMAGEFONTCOUNT; ++i)
	{
		m_vecDemageFont.push_back(CreateActObject(_T("DemageFont"))->CreateCom<CEditDemageFont>(_T("Number_Red")));
		m_vecDemageFont[i]->SetSkillMgr(this);
		m_DemageReadyList.push_back(m_vecDemageFont[i]);
		m_vecDemageFont[i]->Off();
	}

	return true;
}
void CSkillMgr::SkillStandby(const bool _Standby) 
{
	m_bSkillStandby = _Standby;
}
const bool CSkillMgr::IsSkillStandby()
{
	return m_bSkillStandby;
}
void CSkillMgr::MgrOn()
{
	if (STATE_COMPLETE != m_SkillMgrState)
		return;

	if (false == m_bCreateUnit)
		CreateBattleUnit();


	if (m_CurCharIndex < m_vecCharacter.size() && nullptr != m_vecCharacter[m_CurCharIndex])
		m_vecCharacter[m_CurCharIndex]->On();

	if (m_CurMonsterIndex < m_vecMonster.size() && nullptr != m_vecMonster[m_CurMonsterIndex])
		m_vecMonster[m_CurMonsterIndex]->On();

	On();
}
void CSkillMgr::MgrOff()
{
	if (STATE_COMPLETE != m_SkillMgrState)
		return;

	if (false == m_bCreateUnit)
		CreateBattleUnit();


	if(m_CurCharIndex < m_vecCharacter.size() && nullptr != m_vecCharacter[m_CurCharIndex])
		m_vecCharacter[m_CurCharIndex] ->Off();

	if (m_CurMonsterIndex < m_vecMonster.size() && nullptr != m_vecMonster[m_CurMonsterIndex])
		m_vecMonster[m_CurMonsterIndex]->Off();

	Off();
}
void CSkillMgr::Update() 
{
	if (false == IsActive())
		return;

	switch (m_ProgressState)
	{
	case STATE_KEYCHECK:
		KeyCheck();
		break;
	case STATE_SKILLPROGRESS:
		SkillProcess();
		break;
	case STATE_OBJECTWAIT:
		ObjectWait();
		break;
	}

}

void CSkillMgr::SkillProcess()
{
	if (true == CheckSkillEnd())
		return;


	if (true == m_bCharTurn)
		CharSkillProcess();
	else
		MonSkillProcess();
}
const bool CSkillMgr::CheckSkillEnd()
{
	if (m_fAccTime < m_fSkillActionTime)
		return false;

	
	if (true == m_CurSkill.SkillData.CharacterSkill)
		m_CurActionUnit->SetPos(m_CharacterPos);
	else
		m_CurActionUnit->SetPos(m_MonsterPos);

	m_CurActionUnit->SetRenPivot(Vec2{ 0.0f,0.0f });
	m_CurActionUnit->SetAlpha();
	m_CurActionUnit->SetIdle();

	m_ProgressState = STATE_OBJECTWAIT;

	// 스킬 진행중에 연결한 모든 Effect Point를 놓아준다
	// Release X , 연결만 끊음
	for (size_t i = 0; i < m_pCurEffect.size(); i++)
	{
		if (nullptr != m_pCurEffect[i] && true == m_pCurEffect[i]->IsPlay())
			m_pCurEffect[i]->EffectOff();
	}


	m_pCurEffect.assign(ACTIONSCRIPT::MAXEFFECTCOUNT, nullptr);
	CEditGlobal::EditMainCamera->SetZoomRatio(1.0f);
	m_fAccTime = 0.0f;

	//m_CurActionUnit->TargetUnitEndReaction();

	return true;
}
void CSkillMgr::CharSkillProcess()
{
	ScriptUpdate();
	SkillDataTimingCheck();

	ActionUpdate();
	SkillDataUpdate();

	m_fAccTime += DELTATIME;
}
void CSkillMgr::MonSkillProcess() 
{
	if (BATTLE_IDLE != m_CurActionUnit->GetCurUnitState())
		return;

	ScriptUpdate();
	SkillDataTimingCheck();

	ActionUpdate();
	SkillDataUpdate();

	m_fAccTime += DELTATIME;
}
void CSkillMgr::ObjectWait()
{
	if (true == m_bAllReturn || m_fAccTime >= 4.0f) 
	{
		m_ProgressState = STATE_KEYCHECK;
		m_fAccTime = 0.0f;

		for (size_t i = 0; i < m_CurDemageGroup.size(); i++)
		{
			if (nullptr != m_CurDemageGroup[i])
				m_CurDemageGroup[i]->GroupFontReturn();
		}

		m_CurDemageGroup.clear();
		MergeDemageList();
		m_fAccTime = 0.0f;
		
		for (size_t i = 0; i < m_vecCurTurnTarget.size(); i++)
		{
			if (nullptr != m_vecCurTurnTarget[i])
				m_vecCurTurnTarget[i]->EndReAction();
		}

		return;
	}

	m_fAccTime += DELTATIME;
}
void CSkillMgr::CharacterOn(const CharacterIndex& _CharIndex)
{
	if (_CharIndex >= m_vecCharacter.size())
		return;

	if(m_CurCharIndex < m_vecCharacter.size() && nullptr != m_vecCharacter[m_CurCharIndex])
		m_vecCharacter[m_CurCharIndex]->Off();

	if ( nullptr != m_vecCharacter[_CharIndex])
	{
		m_vecCharacter[_CharIndex]->On();
		m_CurActionUnit = m_vecCharacter[_CharIndex];
		m_CurCharIndex = _CharIndex;
	}


}
void CSkillMgr::CharacterOn(const int& _CharIndex)
{
	return CharacterOn((CharacterIndex)_CharIndex);
}
void CSkillMgr::MonTargetUnitOn()
{
	if (nullptr != m_MonTargetUnit)
		m_MonTargetUnit->On();
}
void CSkillMgr::KeyCheck()
{
	if (STATE_COMPLETE != m_SkillMgrState)
		return;

	if (true == KEYDOWN("NextMonster")) 
	{
		m_vecMonster[m_CurMonsterIndex]->Off();
		NextMonster();
	}
	else if (true == KEYDOWN("PrevMonster"))
	{
		m_vecMonster[m_CurMonsterIndex]->Off();
		PrevMonster();
	}
	else if (true == KEYDOUBLE("EffectKey")) 
	{
		int EffectNo = CEditGlobal::ActionScriptDlg->GetCurEffectNo();
		if (nullptr != CEditGlobal::EffectMgr)
			CEditGlobal::EffectMgr->EffectOn(EffectNo);
	}
	else if (true == KEYDOWN("SkillTrigger"))
	{
		if (false == m_bSkillStandby)
			return;

		SetSkill(m_CurSkill.SkillNo,m_CurSkillLevel);
		m_fAccTime = 0.0f;
		m_ProgressState = STATE_SKILLPROGRESS;

		if (true == m_CurSkill.SkillData.CharacterSkill)
		{
			SetTarget(m_vecMonster[m_CurMonsterIndex]);
			SetInitCharPos();
			CreateDemageGroup(m_CurSkill.SkillData.CharacterSkill);
		}
		else 
		{
			SetTarget(m_MonTargetUnit);
		 	CreateDemageGroup(m_CurSkill.SkillData.CharacterSkill);
			m_CurActionUnit = m_vecMonster[m_CurMonsterIndex];
			m_CurActionUnit->SetAction();
		}

		CEditGlobal::EffectMgr->SettingCurEffectLayer(m_bCharTurn);
	}
}
void CSkillMgr::CurCharacterOn() 
{
	if (m_CurCharIndex < m_vecCharacter.size() && nullptr != m_vecCharacter[m_CurCharIndex])
		m_vecCharacter[m_CurCharIndex]->On();
}
void CSkillMgr::CurCharacterOff() 
{
	if (m_CurCharIndex < m_vecCharacter.size() && nullptr != m_vecCharacter[m_CurCharIndex])
		m_vecCharacter[m_CurCharIndex]->Off();

}
void CSkillMgr::MonTargetUnitOff()
{
	if (nullptr != m_MonTargetUnit)
		m_MonTargetUnit->Off();
}
void CSkillMgr::InitMonsterOn()
{
	if (m_CurMonsterIndex < m_vecMonster.size())
		m_vecMonster[m_CurMonsterIndex]->Off();

	for (size_t i = 0; i < m_vecMonster.size(); i++)
	{
		if (m_vecMonster[i]->GetUnitName() == m_InitMonName) 
		{
			m_vecMonster[i]->On();
			m_CurMonsterIndex = (UINT)i;
		}
	}
}
void CSkillMgr::NextMonster()
{
	++m_CurMonsterIndex;

	if (m_CurMonsterIndex >= (UINT)m_vecMonster.size())
		m_CurMonsterIndex = 0;

	m_vecMonster[m_CurMonsterIndex]->On();
}
void CSkillMgr::PrevMonster()
{
	--m_CurMonsterIndex;

	if (m_CurMonsterIndex == -1)
		m_CurMonsterIndex = (UINT)m_vecMonster.size() -1;

	m_vecMonster[m_CurMonsterIndex]->On();
}
void CSkillMgr::PlaySkillSound(const tstring& _SoundName)
{
	if (m_CurCharIndex >= m_vecCharacter.size())
		return;

	if (nullptr == m_vecCharacter[m_CurCharIndex])
		return;
}
const UINT CSkillMgr::GetCurSkillLevel()
{
	return m_CurSkillLevel;
}
const UINT CSkillMgr::SearchNextIndex() 
{
	for (size_t i = 0; i < m_vecSkill.size(); i++)
	{
		if (m_vecSkill[i].SkillNo == -1)
			return (UINT)i;
	}

	return (UINT)m_vecSkill.size();
}
BOOL CSkillMgr::CreateSkill(const ACTIONSCRIPT& _Script)
{
	if (nullptr == &_Script)
		return FALSE;

	UINT No = _Script.SkillNo;

	if (No == m_vecSkill.size()) 
	{
		SKILL Skill = SKILL();
		Skill.SkillActionScript = _Script;
		Skill.SkillData.SkillNo = No;
		Skill.SkillData.SkillType = 0;

		m_vecSkill.push_back(Skill);
		
		LEVELDATA TD = LEVELDATA();
		TD.SkillNo = No;

		vector<LEVELDATA> Temp;
		
		Temp.assign(LEVELDATA::MAXSKILLLEVEL, TD);

		//for (size_t i = 0; i < LEVELDATA::MAXSKILLLEVEL; ++i) 
		m_vecSkillLevelData.push_back(Temp);
			
		
	}
	else 
	{
		m_vecSkill[No].SkillActionScript = _Script;
		if (-1 == m_vecSkill[No].SkillData.SkillType)
			m_vecSkill[No].SkillData.SkillType = 0;
	}
	
	m_vecSkill[No].SkillNo = No;
	m_vecSkill[No].SkillData.SkillNo = No;
	m_vecSkill[No].SkillUIData.SkillNo = No;

	if (_Script.CharIndex < CEditGlobal::CharacterName.size()) 
	{
		CString TempName;
		TempName.Format(_T("%d) %s_Skill"), No, CEditGlobal::CharacterName[_Script.CharIndex] );
		m_vecSkill[No].SetSkillName(TempName.GetString());
	}
	
	
	return TRUE;
}
BOOL CSkillMgr::CreateSkill(const float& _TotalTime ,const SKILLDATA& _SkillData, vector<LEVELDATA>* _LevelData)
{
	// 사실상 몬스터 스킬 전용 생성 함수
	if (nullptr == _LevelData || LEVELDATA::MAXSKILLLEVEL != _LevelData->size())
		return FALSE;

	UINT No = _SkillData.SkillNo;

	if (No == m_vecSkill.size())
	{
		SKILL Skill = SKILL();
		Skill.SkillNo = No;
		Skill.SkillData = _SkillData;
		Skill.SkillActionScript.SkillNo = No;
		Skill.SkillActionScript.TotalSkillTime = _TotalTime;
		Skill.SkillUIData.SkillNo = No;

		m_vecSkill.push_back(Skill);
	
		vector<LEVELDATA> Temp;
		Temp.assign(LEVELDATA::MAXSKILLLEVEL, LEVELDATA());
		for (size_t i = 0; i < LEVELDATA::MAXSKILLLEVEL; ++i)
			Temp[i] = (*_LevelData)[i];
		
		m_vecSkillLevelData.push_back(Temp);
	}
	else
	{
		m_vecSkill[No].SkillData = _SkillData;
		
		for (size_t i = 0; i < LEVELDATA::MAXSKILLLEVEL; ++i)
			m_vecSkillLevelData[No][i] = (*_LevelData)[i];

		m_vecSkill[No].SkillNo = No;
		m_vecSkill[No].SkillActionScript.SkillNo = No;
		m_vecSkill[No].SkillUIData.SkillNo = No;
		m_vecSkill[No].SkillActionScript.TotalSkillTime = _TotalTime;
		
	}

	CString TempName;
	TempName.Format(_T("Monster_Skill"));
	m_vecSkill[No].SetSkillName(TempName.GetString());
	
	return TRUE;
}
BOOL CSkillMgr::DeleteSkill(const int& _SkillNo)
{
	if (_SkillNo >= m_vecSkill.size())
		return FALSE;

	m_vecSkill[_SkillNo] = SKILL();
	return TRUE;
}
BOOL CSkillMgr::ChangeActionScript(const int& _SkillNo,const ACTIONSCRIPT& _Script)
{
	if (_SkillNo >= m_vecSkill.size())
		return FALSE;

	
	//if (true == _Script.isEmpty())
	//	return FALSE;

	m_vecSkill[_SkillNo].SkillActionScript = _Script;
	if (-1 == m_vecSkill[_SkillNo].SkillData.SkillType)
		m_vecSkill[_SkillNo].SkillData.SkillType = 0;

	return TRUE;

}
BOOL CSkillMgr::ChangeSkillData(const int& _SkillNo, const SKILLDATA& _SkillData, vector<LEVELDATA>* _LevelData)
{
	if (_SkillNo >= m_vecSkill.size() || _SkillNo >= m_vecSkillLevelData.size())
		return FALSE;

	if (LEVELDATA::MAXSKILLLEVEL != _LevelData->size())
		return FALSE;


	m_vecSkill[_SkillNo].SkillData = _SkillData;

	for (size_t i = 0; i < LEVELDATA::MAXSKILLLEVEL; i++) 
	{
		m_vecSkillLevelData[_SkillNo][i] = (*_LevelData)[i];
		m_vecSkillLevelData[_SkillNo][i].SkillNo = _SkillNo;
	}
	
	return TRUE;
}
BOOL CSkillMgr::LoadSkillFile(FILE* _ReadFile)
{
	if (nullptr == _ReadFile)
		return FALSE;

	size_t LoadCount = -1;
	SKILLSAVEDATA SkillData = SKILLSAVEDATA();
	m_vecSkill.clear();
	
	list<SKILLSAVEDATA> SkillList;
	SkillList.clear();
	UINT MaxSkillNo = 0;

	while (LoadCount = fread_s(&SkillData, sizeof(SkillData), sizeof(SKILLSAVEDATA), 1, _ReadFile))
	{
		SkillList.push_back(SkillData);
		MaxSkillNo = max(MaxSkillNo, SkillData.ActionScript.SkillNo);
	}

	m_vecSkill.assign(MaxSkillNo + 1, SKILL());
	
	vector<LEVELDATA> EmpLevelData;
	EmpLevelData.assign(LEVELDATA::MAXSKILLLEVEL, LEVELDATA());
	m_vecSkillLevelData.assign(MaxSkillNo + 1, EmpLevelData);

	list<SKILLSAVEDATA>::iterator Start = SkillList.begin();
	list<SKILLSAVEDATA>::iterator End = SkillList.end();

	for (;Start!= End; ++Start)
	{
		UINT No = Start->ActionScript.SkillNo;

		if (No >= m_vecSkill.size())
			continue;
		
		m_vecSkill[No].SkillActionScript = Start->ActionScript;
		m_vecSkill[No].SkillData = Start->SkillData;
		m_vecSkill[No].SkillNo = No;

		for (size_t i = 0; i < LEVELDATA::MAXSKILLLEVEL; i++)
			m_vecSkillLevelData[No][i].SkillNo = No;

		
		// 스킬 임시 이름 지정
		UINT CharIndex = m_vecSkill[No].SkillActionScript.CharIndex;

		if (CharIndex < CEditGlobal::CharacterName.size())
		{
			CString TempName;
			TempName.Format(_T("%d) %s_Skill"), No, CEditGlobal::CharacterName[CharIndex]);
			m_vecSkill[No].SetSkillName(TempName.GetString());
		}
	}

	CGameFile SkillFile(_T("EditData"), _T("SkillUIData.bin"));

	if (FALSE == SkillFile.Open(_T("rt"))) 
	{
		assert(nullptr);
		return FALSE;
	}

	list<SKILLUIDATA> TempList;
	TempList.clear();

	SKILLUIDATA::LoadTextFile(SkillFile.GetFile(), &TempList);

	list<SKILLUIDATA>::iterator UIStart = TempList.begin();
	list<SKILLUIDATA>::iterator UIEnd = TempList.end();

	for (; UIStart != UIEnd; ++UIStart)
	{
		UINT No = UIStart->SkillNo;
		
		if (No >= m_vecSkill.size() || -1 == m_vecSkill[No].SkillNo )
			continue;

		m_vecSkill[No].SkillUIData = *UIStart;
		m_vecSkill[No].SetSkillName(UIStart->SkillName);
	}
	
	return TRUE;
}
BOOL CSkillMgr::LoadSkillFile(FILE* _SkillFile, FILE* _LevelFile)
{
	if (nullptr == _SkillFile || nullptr == _LevelFile)
		return FALSE;

	size_t LoadCount = -1;
	SKILLSAVEDATA SkillData = SKILLSAVEDATA();
	m_vecSkill.clear();

	list<SKILLSAVEDATA> SkillList;
	SkillList.clear();
	UINT MaxSkillNo = 0;

	while (LoadCount = fread_s(&SkillData, sizeof(SkillData), sizeof(SKILLSAVEDATA), 1, _SkillFile))
	{
		SkillList.push_back(SkillData);
		MaxSkillNo = max(MaxSkillNo, SkillData.ActionScript.SkillNo);
	}

	m_vecSkill.assign(MaxSkillNo + 1, SKILL());

	vector<LEVELDATA> EmpLevelData;
	EmpLevelData.assign(LEVELDATA::MAXSKILLLEVEL, LEVELDATA());
	m_vecSkillLevelData.assign(MaxSkillNo + 1, EmpLevelData);

	list<SKILLSAVEDATA>::iterator Start = SkillList.begin();
	list<SKILLSAVEDATA>::iterator End = SkillList.end();

	for (; Start != End; ++Start)
	{
		UINT No = Start->ActionScript.SkillNo;

		if (No >= m_vecSkill.size())
			continue;

		m_vecSkill[No].SkillActionScript = Start->ActionScript;
		m_vecSkill[No].SkillData = Start->SkillData;
		m_vecSkill[No].SkillNo = No;

		for (size_t i = 0; i < LEVELDATA::MAXSKILLLEVEL; i++)
			m_vecSkillLevelData[No][i].SkillNo = No;


		// 스킬 임시 이름 지정
		UINT CharIndex = m_vecSkill[No].SkillActionScript.CharIndex;

		CString TempName;

		if (false == m_vecSkill[No].SkillData.CharacterSkill)
			continue;
		else if (CharIndex < CEditGlobal::CharacterName.size())
		{
			TempName.Format(_T("%d) %s_Skill"), No, CEditGlobal::CharacterName[CharIndex]);
			m_vecSkill[No].SetSkillName(TempName.GetString());
		}
	}

	// SkillLevelData Load + SkillUIData Load Logic 추가
	LoadCount = -1;
	list<SAVELEVELDATA> TempLevelList;
	TempLevelList.clear();

	SAVELEVELDATA  BufLevelData = SAVELEVELDATA();

	while (LoadCount = BufLevelData.LoadDataFile(_LevelFile))
		TempLevelList.push_back(BufLevelData);
		
	list<SAVELEVELDATA>::iterator LStart = TempLevelList.begin();
	list<SAVELEVELDATA>::iterator LEnd = TempLevelList.end();

	for (; LStart != LEnd; ++LStart)
	{
		UINT No = LStart->SkillNo;

		if ( No >= m_vecSkillLevelData.size())
			continue;

		for (size_t i = 0; i < LEVELDATA::MAXSKILLLEVEL; i++) 
			m_vecSkillLevelData[No][i] = LStart->LevelData[i];
		
	}


	CGameFile SkillFile(_T("EditData"), _T("SkillUIData.bin"));

	if (FALSE == SkillFile.Open(_T("rt")))
	{
		assert(nullptr);
		return FALSE;
	}

	list<SKILLUIDATA> TempList;
	TempList.clear();

	SKILLUIDATA::LoadTextFile(SkillFile.GetFile(), &TempList);

	list<SKILLUIDATA>::iterator UIStart = TempList.begin();
	list<SKILLUIDATA>::iterator UIEnd = TempList.end();

	for (; UIStart != UIEnd; ++UIStart)
	{
		UINT No = UIStart->SkillNo;

		if (No >= m_vecSkill.size() || -1 == m_vecSkill[No].SkillNo)
			continue;

		m_vecSkill[No].SkillUIData = *UIStart;
		m_vecSkill[No].SetSkillName(UIStart->SkillName);
	}

	return TRUE;

}
BOOL CSkillMgr::SaveSkillFile(FILE* _WriteFile)
{
	if (nullptr == _WriteFile)
		return FALSE;

	if (0 == m_vecSkill.size())
		return FALSE;

	for (size_t i = 0; i < m_vecSkill.size(); i++)
	{
		if (-1 == m_vecSkill[i].SkillNo)
			continue;

		SKILLSAVEDATA TempData(m_vecSkill[i]);
		size_t a =TempData.WriteData(_WriteFile);
	}

	return TRUE;
}
BOOL CSkillMgr::SaveSkillFile(FILE* _SkillFile, FILE* _LevelFile)
{
	if (nullptr == _SkillFile || nullptr == _LevelFile)
		return FALSE;

	if (0 == m_vecSkill.size())
		return FALSE;

	for (size_t i = 0; i < m_vecSkill.size(); i++)
	{
		if (-1 == m_vecSkill[i].SkillNo)
			continue;

		SKILLSAVEDATA TempData(m_vecSkill[i]);
		size_t a = TempData.WriteData(_SkillFile);
	}

	for (size_t i = 0; i < m_vecSkillLevelData.size(); i++)
	{
		if (-1 == m_vecSkill[i].SkillNo)
			continue;

		SAVELEVELDATA SaveData(&m_vecSkillLevelData[i]);
		size_t b = SaveData.SaveDataFile(_LevelFile);
	}

	return TRUE;
}
SPTR<CEditDemageFont> CSkillMgr::GetDemageFont(const int& _Number, const Vec3& _RenderPos, const size_t& NumberSpriteIndex)
{
	if (true == m_DemageReadyList.empty())
	{
		return nullptr;
	}

	SPTR<CEditDemageFont> PopDemage = m_DemageReadyList.front();
	PopDemage->MiddleOn(_Number, Vec2{ _RenderPos.x ,_RenderPos.y }, NumberSpriteIndex);
	m_DemageReadyList.pop_front();
	++m_iCurOnDemageCount;
	PopDemage->Off();

	m_bAllReturn = false;

	return PopDemage;
}
void CSkillMgr::ReturnDemageFont(SPTR<CEditDemageFont> _Demage)
{
	if (nullptr == _Demage)
	{
		assert(nullptr);
		return;
	}

	m_DemageReturnList.push_back(_Demage);

	if (m_DemageReturnList.size() + m_DemageReadyList.size() == MAXDEMAGEFONTCOUNT)
	{
		m_bAllReturn = true;
	}

}
bool CSkillMgr::IsFontAllReturn()
{
	return m_bAllReturn;
}
void CSkillMgr::MergeDemageList()
{
	for (size_t i = 0; i < m_vecDemageFont.size(); i++)
	{
		m_vecDemageFont[i]->Off();
	}

	m_DemageReturnList.sort(CEditDemageFont::RenderOrder);

	m_DemageReadyList.merge(m_DemageReturnList, CEditDemageFont::RenderOrder);
	m_DemageReturnList.clear();

	if (MAXDEMAGEFONTCOUNT != m_DemageReadyList.size())
	{
		m_DemageReadyList.clear();

		for (size_t i = 0; i < MAXDEMAGEFONTCOUNT; i++)
		{
			m_DemageReadyList.push_back(m_vecDemageFont[i]);
		}
	}

	m_bAllReturn = true;
}
const size_t CSkillMgr::GetReadyListSize()
{
	return m_DemageReadyList.size();
}
const size_t CSkillMgr::GetReturnListSize()
{
	return m_DemageReturnList.size();
}
CActObject* CSkillMgr::CreateActObject(const tstring& _ObjName)
{
	return CActObject::CreateActObject(_ObjName, m_pSkillScene);
}

void CSkillMgr::SetInitCharPos()
{
	if (nullptr == m_CurActionUnit)
 		return;

	// 이때 설정되어있는 값은 INITPOS_FIXPOS에서 사용하는 값 
	Vec2 InitPos = Vec2{ 0.0f,0.0f };
	
	Vec2 PosPivot = m_CurSkillData.InitPosPivot;


	size_t InitPosMode = SKILLDATA::INITPOS_NONE;// 만약 이상한 값이 들어올경우 무조건 제자리로

	InitPosMode = m_CurSkillData.InitPosMode;

	switch (InitPosMode)
	{
	case SKILLDATA::INITPOS_NONE:	//	제자리
	{
		InitPos = Vec2{ m_CurActionUnit->GetPos().x ,m_CurActionUnit->GetPos().y };
	}
	break;
	case SKILLDATA::INITPOS_FIXPOS: 
	{
		InitPos = m_CurSkillData.InitFixPos;
	}
	break;
	case SKILLDATA::INITPOS_TARGETPOS:	// SingleTarget 전용
	{
		if (nullptr != m_vecCurTurnTarget[0] && 1 == m_vecCurTurnTarget.size())
		{
			SPTR<CEditBattleUnit> Target = m_vecCurTurnTarget.front();

			if (nullptr != Target)
				InitPos = Target->GetPosVec2();
		}

		// 확인용
		if (m_vecCurTurnTarget.size() != 1)
			assert(nullptr);
	}
	break;
	case SKILLDATA::INITPOS_TARGET_FRONTPOS: 
	{
		if (nullptr != m_vecCurTurnTarget[0] && 1 == m_vecCurTurnTarget.size())
		{
			SPTR<CEditBattleUnit> Target = m_vecCurTurnTarget.front();

			if (nullptr != Target)
				InitPos = Target->GetUnitFrontPos();
		}

	}
	break;
	case SKILLDATA::INITPOS_TARGET_BACKPOS:
	{
		if (nullptr != m_vecCurTurnTarget[0] && 1 == m_vecCurTurnTarget.size())
		{
			SPTR<CEditBattleUnit> Target = m_vecCurTurnTarget.front();

			if (nullptr != Target)
				InitPos = Target->GetUnitBackPos();
		}

	}
	break;
	default:
		return;
	}

	
	m_CurActionUnit->SetPos(InitPos + m_CurSkillData.InitPosPivot);

 }
void CSkillMgr::CreateDemageGroup(const bool& _bChar)
{
	if (true == _bChar) 
	{
		m_CurDemageGroup.clear();

		for (size_t i = 0; i < m_vecCurTurnTarget.size(); i++)
		{
			CActObject* Obj = CActObject::CreateActObject(_T("DegGroup"), m_pSkillScene);
			CEditDemageFontGroup* Group = Obj->CreateCom<CEditDemageFontGroup>();
			Group->CreateFontGroup(m_CurActionUnit, m_vecCurTurnTarget[i], &m_CurSkillLevelData);
			m_CurDemageGroup.push_back(Group);
		}
	}
	else 
	{
		m_CurDemageGroup.clear();

		for (size_t i = 0; i < m_vecCurTurnTarget.size(); i++)
		{
			CActObject* Obj = CActObject::CreateActObject(_T("DegGroup"), m_pSkillScene);
			CEditDemageFontGroup* Group = Obj->CreateCom<CEditDemageFontGroup>();
			Group->CreateFontGroup(m_vecMonster[m_CurMonsterIndex], m_vecCurTurnTarget[i], &m_CurSkillLevelData);
			m_CurDemageGroup.push_back(Group);
		}
	}
}