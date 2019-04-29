#include "Precom.h"
#include "ActionPhase.h"
#include "BattleUnit.h"
#include "TurnMgr.h"
#include "BattleSceneBuilder.h"
#include "EffectMgr.h"
#include "Effect.h"
#include "DemageFontGroup.h"


CActionPhase::CActionPhase()
	:m_CurActionUnit(nullptr), m_fAccTime(0.0f)
	, m_CurAniScript(ANIMATIONSCRIPT()), m_CurMoveScript(MOVEDATA())
	,m_CurHideScript(MOVEDATA())
	, m_fAccWaitTime(0.0f), m_fMaxWaitTime(4.0f), m_bCurSkillEnd(false)
	, m_AccHitCount(-1), m_bLastHitCheck(false)
	, m_CurActPhaseState(SKILL_UPDATE)
	, m_bCharacterTurn(true)
{
	m_pCurEffect.assign(ACTIONSCRIPT::MAXEFFECTCOUNT, nullptr);
	m_CurDemageGroup.clear();
}


CActionPhase::~CActionPhase()
{
	m_CurDemageGroup.clear();
}

void CActionPhase::StateReset()
{
	m_CurActionUnit = m_pTurnMgr->GetCurUnit();
	m_CurActPhaseState = SKILL_UPDATE;
	m_pTurnMgr->ClearAllDemageQueue();

	assert(m_CurActionUnit);
	
	m_CurActionUnit->SelectEffectOff();
	m_CurActionUnit->SetAction();
	
	
	if (true == m_CurActionUnit->IsCharacter())
		SettingCharacterSkill();
	else
		SettingMonsterSkill();


	m_fAccTime = 0.0f;
	m_fAccWaitTime = 0.0f;
	m_bCurSkillEnd = false;
	m_AccHitCount = 0;
	m_bLastHitCheck = m_pTurnMgr->IsLastHitCheckAction();
}

void CActionPhase::SettingCharacterSkill()
{
	size_t SkillNo = m_CurActionUnit->GetSelectSkill();
	size_t SkillLv = m_CurActionUnit->GetSkillLevel(SkillNo);


	// ���� ��°��� �߸����°�� �ٷ� �����̼� ������� �̵�
	if (SkillNo >= CClientGlobal::vecSkill.size() || -1 == SkillLv)
	{
		m_CurActionUnit->SetIdle();
		m_CurActionUnit->SelectEffectOff();
		m_pTurnMgr->ChangePhase(PHASE_ROTATE);
		return;
	}

	m_pTurnMgr->SetSkill(SkillNo, SkillLv);

	// ���� ��ų�� �����ϱ� ������ Cost�� ���ڸ��ԵǸ�
	// ��ų�� ������� �ʰ� ������ ���� �ѱ��.
	if (false == CClientGlobal::MasterMode)
	{
		if (false == CheckSkillCost())
		{
			m_CurActionUnit->SetIdle();
			m_CurActionUnit->SelectEffectOff();
			m_pTurnMgr->ChangePhase(PHASE_ROTATE);
			m_pTurnMgr->SetRotateCount(-1);
			return;
		}
		CalSkillCost();
	}
	// ���� Ÿ���� ������ ��� �׾� ���� ����� �Ҿ��� ���
	// ���ݴ���� ���Ƿ� �������ϴ� �۾�
	CheckTargerOverride();
	CheckUnitTarget();
	CharacterStandby();
	CreateDemageGroup();
	m_CurActionUnit->InitSelectSkillData();

	m_bCharacterTurn = CClientGlobal::vecSkill[SkillNo].SkillData.CharacterSkill;
	m_pEffectMgr->SettingCurEffectLayer(m_bCharacterTurn);
}
void CActionPhase::SettingMonsterSkill()
{	
	size_t SkillNo = m_CurActionUnit->CurRandomSkillNo();
	size_t SkillLv = m_CurActionUnit->CurRandomSkillLv();

	if(SkillNo>= CClientGlobal::vecSkill.size() || SkillLv >= LEVELDATA::MAXSKILLLEVEL)
		m_pTurnMgr->SetSkill(25, 0);
	else
		m_pTurnMgr->SetSkill(SkillNo, SkillLv);
	


	m_bCharacterTurn = false;
	m_pEffectMgr->SettingCurEffectLayer(m_bCharacterTurn);
	
	m_pCurTarget->clear();
	MonsterTargetSetting();

	MonsterStandby();
	CreateDemageGroup();
	m_CurActionUnit->InitSelectSkillData();
}
void CActionPhase::Update()
{
	switch (m_CurActPhaseState)
	{
	case SKILL_UPDATE:
		SkillProcess();
		break;
	case  SKILL_ENDWAIT:
		CheckTurnChange();
		break;
	}
}
void CActionPhase::SkillProcess()
{
	if (true == SkillEndChck())
		return;

	if (true == m_CurActionUnit->IsCharacter())
		CharacterSkillProcess();
	else
		MonsterSkillProcess();

}
bool CActionPhase::SkillEndChck()
{
	if (m_fAccTime < (*m_pSkillActionTime))
		return false;

	m_bCurSkillEnd = true;

	for (size_t i = 0; i < m_CurDemageGroup.size(); i++)
	{
		if (nullptr != m_CurDemageGroup[i] && true == m_CurDemageGroup[i]->IsEmpty())
			m_CurDemageGroup[i] = nullptr;
	}

	m_CurActPhaseState = SKILL_ENDWAIT;
	return true;
}
void CActionPhase::CharacterSkillProcess()
{
	// ��ũ��Ʈ or SkillData�� ���۽ð��� üũ�ѵ�
	// ���۽ð��� �Ǿ����� ��ũ��Ʈ�� Ȱ��ȭ
	ScriptUpdate();
	SkillDataTimingCheck();

	// ���� �ܰ迡�� Ȱ��ȭ �� ��ũ��Ʈ�� �����͸� 
	// �۵� or Update
	ActionUpdate();
	SkillDataUpdate();	

	m_fAccTime += DELTATIME;
}
void CActionPhase::MonsterSkillProcess()
{
	if (BATTLE_IDLE != m_CurActionUnit->GetCurUnitState())
		return;

	ScriptUpdate();
	SkillDataTimingCheck();

	ActionUpdate();
	SkillDataUpdate();

	m_fAccTime += DELTATIME;

	
}
void CActionPhase::CheckTurnChange()
{
	if (false == m_bCurSkillEnd)
		return;
	
	// TurnMgr�� ������ ��Ʈ�� ��� ����Ʈ�� ���ƿ��������� ���
	// 4.0���̻���� �̸� �����ϰ� �ٷ� ���� �ѱ�
	if (m_fAccWaitTime >= m_fMaxWaitTime || true == m_pTurnMgr->IsFontAllReturn())
	{
		if(true == m_bCharacterTurn)
			m_CurActionUnit->SetPos(m_pTurnMgr->GetCharacterPos(m_CurActionUnit->GetTeamNumber()));
		else 
			m_CurActionUnit->SetPos(m_pTurnMgr->GetMonsterPos(m_CurActionUnit->GetTeamNumber()));

		m_CurActionUnit->SetRenPivot(Vec2{ 0.0f,0.0f });
		m_CurActionUnit->SetAlpha();
		m_CurActionUnit->SetIdle();

		for (size_t i = 0; i < m_CurDemageGroup.size(); i++)
		{
			if (nullptr != m_CurDemageGroup[i])
				m_CurDemageGroup[i]->GroupFontReturn();
		}

		m_CurDemageGroup.clear();

		// ��ų �����߿� ������ ��� Effect Point�� �����ش�
		// Release X , ���Ḹ ����
		m_pCurEffect.assign(ACTIONSCRIPT::MAXEFFECTCOUNT, nullptr);
	
		CurTargetUnitEndReaction();
		m_pCurTarget->clear();

		m_pTurnMgr->ChangePhase(PHASE_CHECK);

		return;
	}

	m_fAccWaitTime += DELTATIME;

}
void CActionPhase::ScriptUpdate() 
{
	// �ִϸ��̼� ��ũ��Ʈ & �ִϸ��̼� ������Ʈ
	while (0 != *m_pAniScriptCount && m_CurAniScript.StartTime <= m_fAccTime)
	{
		m_CurActionUnit->ChangeAni(m_CurAniScript.AnimationIndex);
		m_pAniQueue->pop();
		--(*m_pAniScriptCount);

		if (0 != (*m_pAniScriptCount))
		{
			m_CurAniScript = m_pAniQueue->front();
		}
	}

	// Move Script ������Ʈ
	while (0 < (*m_pMoveScriptCount )&& m_pMoveQueue->front().MoveStartTime <= m_fAccTime)
	{
		MOVEDATA PopData = m_pMoveQueue->front();
		m_pMoveQueue->pop();
		--(*m_pMoveScriptCount);

		if (MOVEDATA::MOVEMODE_MOVE ==  PopData.MoveMode)
		{
			m_CurMoveScript = PopData;

			if (0 >= (*m_pMoveScriptCount))
			{
				break;
			}

			if (m_pMoveQueue->front().MoveStartTime <= m_fAccTime
				&&	MOVEDATA::MOVEMODE_MOVE !=  m_pMoveQueue->front().MoveMode )
			{
				m_CurHideScript = m_pMoveQueue->front();
				m_pMoveQueue->pop();
				--(*m_pMoveScriptCount);
			}
		}
		else 
		{
			m_CurHideScript = PopData;

			if (0 >= (*m_pMoveScriptCount))
			{
				break;
			}

			if (m_pMoveQueue->front().MoveStartTime <= m_fAccTime
				&&	MOVEDATA::MOVEMODE_MOVE ==  m_pMoveQueue->front().MoveMode)
			{
				m_CurMoveScript = m_pMoveQueue->front();
				m_pMoveQueue->pop();
				--(*m_pMoveScriptCount);
			}
		}
		
	}


	// ���带 Queue�� ���
	for (int i = 0; i < min( (int)m_pSoundVector->size(), ACTIONSCRIPT::MAXCHARSOUNDCOUNT); i++)
	{
		if (true == (*m_pSoundVector)[i].Playing)
		{
			continue;
		}

		if (m_fAccTime >= (*m_pSoundVector)[i].StartTime)
		{
			m_pSoundQueue->push( (*m_pSoundVector)[i].SoundName);
			(*m_pSoundVector)[i].Playing = true;
		}
	}


	// SkillEffect Script Update
	for (size_t i = 0; i < m_pSkillEffect->size(); ++i)
	{
		if (true == (*m_pSkillEffect)[i].Apply) 
		{
			continue;
		}

		if (m_fAccTime >= (*m_pSkillEffect)[i].StartTime) 
		{
			(*m_pSkillEffect)[i].PIndex = i;
			m_pSkillEffectQueue->push((*m_pSkillEffect)[i]);
			(*m_pSkillEffect)[i].Apply = true;
		}
	}

	// SceneEffect ScriptUpdate & SceneEffectUpdate 
	if (0 != *m_pSceneEffectCount) 
	{
		if (m_pSceneEffectQueue->front().SceneEffectStartTime <= m_fAccTime) 
		{
			if (nullptr != m_pBattleScene) 
			{
				m_pBattleScene->SetSceneEffect(m_pSceneEffectQueue->front(), m_CurActionUnit->GetUnitMiddlePos());
			}
			m_pSceneEffectQueue->pop();
			--(*m_pSceneEffectCount);
		}

	}


}
void CActionPhase::ActionUpdate() 
{
	// Move  & Hide Update
	HideUpdate();
	MoveUpdate();
	

	// Sound Update
	while (false == m_pSoundQueue->empty())
	{
		m_CurActionUnit->PlaySound(m_pSoundQueue->front());
		m_pSoundQueue->pop();
	}

	// SkillEffect Update
	while (false == m_pSkillEffectQueue->empty()) 
	{
		SKILLEFFECTDATA SkillEffect = m_pSkillEffectQueue->front();
		m_pSkillEffectQueue->pop();
		
		//SPTR<CEffect> Effect = m_pEffectMgr->FindEffect(SkillEffect.EffectIndex);
		m_pCurEffect[SkillEffect.PIndex] = m_pEffectMgr->EffectOn(SkillEffect.EffectIndex);

		if (nullptr == m_pCurEffect[SkillEffect.PIndex])
		{
			continue;
		}
		
		Vec2 SkillCasterPos = Vec2{ m_CurActionUnit->GetPos().x,m_CurActionUnit->GetPos().y };
		
		m_pCurEffect[SkillEffect.PIndex]->SetSkillEffectData(SkillEffect,SkillCasterPos);
	}

	SkillEffectUpdate();
	
}
// ��ų ������ ���õ� �����͸� Ÿ�ֿ̹� ���߿� Ȱ��ȭ
void CActionPhase::SkillDataTimingCheck()
{
	if (nullptr == m_pHitData) 
		return;
	
	// HitData�� SetSkillData()���� Sort�۾��� �ū����Ƿ�
	// ����Ʈ�� ���� ���� �����͸� �������� Ȯ���۾� ����
	while (true != m_pHitData->empty())
	{
		if (m_pHitData->front().StartTime <= m_fAccTime)
		{
			m_pHitDataQueue->push(m_pHitData->front());
			m_pHitData->pop_front();
		}
		else
			break;	
	}

	//while(m_pHit)
}
// ��ų������ ������Ʈ���� Ȱ��ȭ�� Hit �����͸� ������Ʈ
void CActionPhase::SkillDataUpdate() 
{
	if (nullptr == m_pHitDataQueue)
		return;

	while (false == m_pHitDataQueue->empty()) 
	{
		HitDataUpdate(m_pHitDataQueue->front());
		m_pHitDataQueue->pop();
	}
}
void CActionPhase::HideUpdate()
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
void CActionPhase::MoveUpdate()
{
	// Character�� �ڽ��� TeamPos�� �̵�
	if (true == m_CurMoveScript.bSetTeamPos) 
	{
		Vec2 CharPos = m_pTurnMgr->GetCharacterPos(m_CurActionUnit->GetTeamNumber());
		m_CurActionUnit->SetPos(CharPos);
		m_CurActionUnit->SetRenPivot(Vec2{ 0.0f,0.0f });
		m_CurMoveScript = MOVEDATA();
		return;
	}

	// Character�� ��ų InitPos�� �̵�
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
void CActionPhase::HitDataUpdate(const HITDATA& _HitData)
{
	// HitData �� 3���� Index�� DemageFont ��������� ���´�
	if (nullptr == m_pCurTarget)
		return;

	// Index(size_t)�� -1�� ��� ������� �ʴ´�
	//1. HitEffect 
	if (MAXINDEX != _HitData.HitEffectIndex) 
	{
		
		if (HITDATA::POS_CASTER == _HitData.PosOption) 
		{
			SPTR<CEffect> Hit = m_pEffectMgr->EffectOn(_HitData.HitEffectIndex);
			Hit->SetHitEffectData(_HitData);
			Vec2 CasterPos = Vec2{ m_CurActionUnit->GetPos().x ,m_CurActionUnit->GetPos().y } +_HitData.HitEffectPivot;
			Hit->SetPos(CasterPos);
			if (_HitData.HitEffectCustomSize.x > 0.0f && _HitData.HitEffectCustomSize.y > 0.0f)
				Hit->SetCustomSize(_HitData.HitEffectCustomSize);
			else
				Hit->SetImageScale(_HitData.HitEffectImageMag);
				
		}
		else if (HITDATA::POS_TARGET == _HitData.PosOption)
		{
			for (size_t i = 0; i < m_pCurTarget->size(); i++)
			{
				SPTR<CEffect> Hit = m_pEffectMgr->EffectOn(_HitData.HitEffectIndex);
				Hit->SetHitEffectData(_HitData);
				Vec2 TargetPos = Vec2{ (*m_pCurTarget)[i]->GetPos().x ,(*m_pCurTarget)[i]->GetPos().y } +_HitData.HitEffectPivot;
				Hit->SetPos(TargetPos);
			
				if (_HitData.HitEffectCustomSize.x > 0.0f && _HitData.HitEffectCustomSize.y > 0.0f)
					Hit->SetCustomSize(_HitData.HitEffectCustomSize);
				else
					Hit->SetImageScale(_HitData.HitEffectImageMag);
			}
		}
		else if (HITDATA::POS_TARGETMIDDLE == _HitData.PosOption)
		{
			for (size_t i = 0; i < m_pCurTarget->size(); i++)
			{
				SPTR<CEffect> Hit = m_pEffectMgr->EffectOn(_HitData.HitEffectIndex);
				Hit->SetHitEffectData(_HitData);
				Vec2 TargetPos = Vec2{ (*m_pCurTarget)[i]->GetUnitMiddlePos().x ,(*m_pCurTarget)[i]->GetUnitMiddlePos().y } +_HitData.HitEffectPivot;
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
		for (size_t i = 0; i < m_pCurTarget->size(); ++i)
		{
			(*m_pCurTarget)[i]->PlaySound(CClientGlobal::GetHitSound(_HitData.HitSoundIndex));
		}
	}

	//3. HitReaction
	if (MAXINDEX != _HitData.ReActionIndex)
	{
		for (size_t i = 0; i < m_pCurTarget->size(); ++i)
		{
			if (BATTLE_REACTION != (*m_pCurTarget)[i]->GetCurUnitState())
				(*m_pCurTarget)[i]->SetReAction();

			(*m_pCurTarget)[i]->ReActionEvent(_HitData.ReActionIndex);
		}
	}


	// ���� 4. DemageFont ��� - > DemageFontGroup���� �Ѱ��� �̾��ش�.
	if (0.0f != _HitData.HitDemageMag)
		for (size_t i = 0; i < m_CurDemageGroup.size(); i++)
		{
			
			if (nullptr != m_CurDemageGroup[i]) 
			{
				if (false == m_CurDemageGroup[i]->DemageFontOn())
					m_CurDemageGroup[i] = nullptr;
				else
					(*m_pCurTarget)[i]->DemagedEvent(m_pTurnMgr->PopDemage((UINT)i));
			
				if (true == m_CurDemageGroup[i]->IsEmpty())
					m_CurDemageGroup[i] = nullptr;
			}
			
		}


	if (false == m_bLastHitCheck)
		return;
	
	// ���� ������ Attack or DoubleAttack�� ��� 
	// ������ Ÿ�ݽ� Mp�� ������ ȸ���ϰ��ϰ�
	// ���� �Ӹ����� Mp�� �󸶳� ȸ���Ǿ����� ��Ʈ�� ����ش�.
	if (true  == m_pTurnMgr->DetectionLastHit(m_AccHitCount)) 
	{
		m_CurActionUnit->LastAttackEvent();
	}

	++m_AccHitCount;

}
void CActionPhase::CharacterStandby()
{
	
	SetInitCharPos();

	if (0 != (*m_pAniScriptCount))
	{
		if (false == m_pAniQueue->empty())
			m_CurAniScript = m_pAniQueue->front();
	}
	else
		m_CurAniScript = ANIMATIONSCRIPT();

	
	m_CurHideScript = MOVEDATA();
	m_CurMoveScript = MOVEDATA();
	
}
void CActionPhase::MonsterStandby()
{
	m_CurActionUnit->SetAction();

	if (0 != (*m_pAniScriptCount))
	{
		if (false == m_pAniQueue->empty())
			m_CurAniScript = m_pAniQueue->front();
	}
	else
		m_CurAniScript = ANIMATIONSCRIPT();


	m_CurHideScript = MOVEDATA();
	m_CurMoveScript = MOVEDATA();

}
void CActionPhase::SkillEffectUpdate()
{
	for (size_t i = 0; i < m_pSkillEffect->size(); i++)
	{
		if (false == (*m_pSkillEffect)[i].Apply)
			continue;

		// �ش� ����Ʈ�� MoveTime == 0.0f �̸� ���� X
		if (0 == (*m_pSkillEffect)[i].MoveData.MoveTime)
			continue;

		// ��ġ �̵� ����ġ�� 1.0f �̻��ϸ� �̹� 
		// ���������Ӷ� ���������� �����Ѱ�� �̹Ƿ� ������ �ʿ� X
		float Weight = (m_fAccTime - (*m_pSkillEffect)[i].MoveData.MoveStartTime) / (*m_pSkillEffect)[i].MoveData.MoveTime;
		
		if (1.2f < Weight) 
			continue;

		if (nullptr == m_pCurEffect[(*m_pSkillEffect)[i].PIndex])
			continue;
		
		// Effect�� �������� ������Ʈ
		Vec2 MovePos = CSJMath::Vec2Lerp(&(*m_pSkillEffect)[i].MoveData.MoveStartPos, &(*m_pSkillEffect)[i].MoveData.MoveEndPos, Weight);
		m_pCurEffect[(*m_pSkillEffect)[i].PIndex]->SetRenPivot(MovePos);
	}
}

void CActionPhase::SetInitCharPos()
{
	if (nullptr == m_CurActionUnit)
		return;

	// �̶� �����Ǿ��ִ� ���� INITPOS_FIXPOS���� ����ϴ� �� 
	Vec2 InitPos = Vec2{ 0.0f,0.0f };
					
	Vec2 PosPivot = m_pCurSkillData->InitPosPivot;


	size_t InitPosMode = SKILLDATA::INITPOS_NONE;// ���� �̻��� ���� ���ð�� ������ ���ڸ���

	InitPosMode = m_pCurSkillData->InitPosMode;

	switch (InitPosMode)
	{
	case SKILLDATA::INITPOS_NONE:	//	���ڸ�
	{
		InitPos = Vec2{ m_CurActionUnit->GetPos().x ,m_CurActionUnit->GetPos().y };
	}
	break;
	case SKILLDATA::INITPOS_FIXPOS:
	{
		InitPos = m_pCurSkillData->InitFixPos;
	}
	break;
	case SKILLDATA::INITPOS_TARGETPOS:	// SingleTarget ����
	{
		if (nullptr != (*m_pCurTarget)[0] && 1 == m_pCurTarget->size())
		{
			SPTR<CBattleUnit> Target = (*m_pCurTarget).front();

			if (nullptr != Target)
				InitPos = Target->GetPosVec2();
		}

		// Ȯ�ο�
		if (m_pCurTarget->size() != 1)
			assert(nullptr);
	}
	break;
	case SKILLDATA::INITPOS_TARGET_FRONTPOS:
	{
		if (nullptr != (*m_pCurTarget)[0] && 1 == m_pCurTarget->size())
		{
			SPTR<CBattleUnit> Target = (*m_pCurTarget).front();

			if (nullptr != Target)
				InitPos = Target->GetUnitFrontPos();
		}

	}
	break;
	case SKILLDATA::INITPOS_TARGET_BACKPOS:
	{
		if (nullptr != (*m_pCurTarget)[0] && 1 == m_pCurTarget->size())
		{
			SPTR<CBattleUnit> Target = (*m_pCurTarget).front();

			if (nullptr != Target)
				InitPos = Target->GetUnitBackPos();
		}

	}
	break;
	default:
		return;
	}


	m_CurActionUnit->SetPos(InitPos + m_pCurSkillData->InitPosPivot);

}

void CActionPhase::CheckUnitTarget()
{
	m_pCurTarget->clear();
	list<SPTR<CBattleUnit>>* Target = m_CurActionUnit->GetCurTargetUnit();

	size_t Count = 0;
	list<SPTR<CBattleUnit>>::iterator Start = Target->begin();
	list<SPTR<CBattleUnit>>::iterator End = Target->end();

	if (false == Target->empty())
	{
		for (; Start!= End;)
		{
			if (nullptr == (*Start))
			{
				Start = Target->erase(Start);
				continue;
			}

			if (true == (*Start)->isDead())
			{
				Start = Target->erase(Start);
				continue;
			}

			++Count;
			++Start;
		}
	}


	// ���� Action Unit���� ��� Target�� ��ȿ�� ���
	// TurnMgr���� ������ Target ������ ��û

	while (0 == Count)
	{
		Count = m_pTurnMgr->SettingRandomTarget();
	}

	Start = Target->begin();
	End = Target->end();

	if (nullptr != Target && nullptr != m_pCurTarget) 
	{
		for (; Start != End ;++Start)
		{
			if (nullptr == (*Start))
				continue;

			m_pCurTarget->push_back((*Start));
		}
	}
	
	
	//m_pCurTarget->assign(Target->begin(), Target->end());
}
void CActionPhase::CheckTargerOverride()
{
	if (false == m_CurActionUnit->IsTargetOverrideByChangeRound())
		return;

	m_CurActionUnit->ClearCurTarget();
	
	TARGET Tar = m_CurActionUnit->GetSelectTarget();

	switch (Tar)
	{
	case TARGET_SINGLE:
		m_pTurnMgr->SingleRandomTarget(m_CurActionUnit);
		break;
	case TARGET_FORWARDCOLUMN:
		m_pTurnMgr->ForwardTarget(m_CurActionUnit);
		break;
	case TARGET_REARCOLUMN:
		m_pTurnMgr->RearTarget(m_CurActionUnit);
		break;
	case TARGET_ALL:
		m_pTurnMgr->AllTarget(m_CurActionUnit);
		break;
	}

	m_CurActionUnit->TargetOverride(false);
}
void CActionPhase::CreateDemageGroup()
{
	m_CurDemageGroup.clear();

	for (size_t i = 0; i < m_pCurTarget->size(); i++)
	{
		CActObject* Obj = CActObject::CreateActObject(_T("DegGroup"), m_pBattleScene);
		CDemageFontGroup* Group = Obj->CreateCom<CDemageFontGroup>();
		Group->SetGroupIndex((UINT)i);
		Group->CreateFontGroup(m_CurActionUnit,(*m_pCurTarget)[i], m_pCurSkillLevelData);
		m_CurDemageGroup.push_back(Group);	
	}
}
const bool CActionPhase::CheckSkillCost()
{
	return m_pTurnMgr->CheckSkillCost();
}
void CActionPhase::CalSkillCost()
{
	m_pTurnMgr->CalSkillCost();
}
void CActionPhase::MonsterTargetSetting()
{
	if (m_pTurnMgr->GetCurSkillNo() >= CClientGlobal::vecSkill.size())
		return;	
	SKILL Temp = CClientGlobal::vecSkill[m_pTurnMgr->GetCurSkillNo()];

	switch (Temp.SkillData.EnemyTargetType)
	{
	case SKILLDATA::TARGET_SINGLE:
		RandomTarget(1);
		return;
	case SKILLDATA::TARGET_COLUMN:
		return;
	case SKILLDATA::TARGET_ALL:
		SettingAllTarget();
		return;
	case SKILLDATA::TARGET_RANDOM_ONE: 
		RandomTarget(1);
		return;
	case SKILLDATA::TARGET_RANDOM_TWO:
		RandomTarget(2);
		return;
	case SKILLDATA::TARGET_RANDOM_THREE:
		RandomTarget(3);
		return;
	case SKILLDATA::TARGET_RANDOM_FOUR:
		RandomTarget(4);
		return;
	}
}
void CActionPhase::RandomTarget(const UINT& _RanCount)
{
	if (true == m_bCharacterTurn)
		MonRandomTarget(_RanCount);
	else 
		CharRandomTarget2(_RanCount);
}
void CActionPhase::CharRandomTarget(const UINT& _RanCount) 
{
	vector<SPTR<CBattleUnit>>* Char = m_pTurnMgr->GetCurBattleCharVec();

	UINT Ran = _RanCount;
	list<size_t> LiveUnit;

	for (size_t i = 0; i < Char->size(); i++)
	{
		if (nullptr == (*Char)[i])
			continue;

		if (false == (*Char)[i]->isDead())
			LiveUnit.push_back(i);
	}

	list<size_t>::iterator Start;
	list<size_t>::iterator End;


	if (LiveUnit.size() <= _RanCount) 
	{
		Start = LiveUnit.begin();
		End = LiveUnit.end();
	
		for (; Start!= End; ++Start)
			m_pCurTarget->push_back((*Char)[*Start]);
		return;
	}


	while (Ran > 0) 
	{
		int Index = CSJRandom::RandomInt(0, (int)LiveUnit.size() - 1);
		
		Start = LiveUnit.begin();
		End = LiveUnit.end();


		while (Index > 0 && Start!=End) 
		{
			++Start;
			--Index;
		}

		if (Start != End) 
		{
			m_pCurTarget->push_back((*Char)[*Start]);
			LiveUnit.erase(Start);
		}

		--Ran;
	}

	return;
}
void CActionPhase::CharRandomTarget2(const UINT& _RanCount)
{
	vector<SPTR<CBattleUnit>>* Char = m_pTurnMgr->GetCurBattleCharVec();

	UINT Ran = _RanCount;
	vector<size_t> LiveUnit;

	for (size_t i = 0; i < Char->size(); i++)
	{
		if (nullptr == (*Char)[i])
			continue;

		if (false == (*Char)[i]->isDead())
			LiveUnit.push_back(i);
	}

	CSJRandom::ShuffleVector(LiveUnit);

	for (size_t i = 0; i < _RanCount && false == LiveUnit.empty(); i++) 
	{
		m_pCurTarget->push_back((*Char)[LiveUnit.back()]);
		LiveUnit.pop_back();
	}

	
	LiveUnit.clear();
	return;
}
void CActionPhase::MonRandomTarget(const UINT& _RanCount) 
{
	vector<SPTR<CBattleUnit>>* Mon = m_pTurnMgr->GetCurBattleMonVec();

	UINT Ran = _RanCount;
	list<size_t> LiveUnit;

	for (size_t i = 0; i < Mon->size(); i++)
	{
		if (nullptr == (*Mon)[i])
			continue;

		if (false == (*Mon)[i]->isDead())
			LiveUnit.push_back(i);
	}

	list<size_t>::iterator Start;
	list<size_t>::iterator End;

	if (LiveUnit.size() <= _RanCount)
	{
		Start = LiveUnit.begin();
		End = LiveUnit.end();

		for (; Start != End; ++Start)
			m_pCurTarget->push_back((*Mon)[*Start]);

		return;
	}

	while (Ran > 0)
	{
		int Index = CSJRandom::RandomInt(0, (int)LiveUnit.size() - 1);

		Start = LiveUnit.begin();
		End = LiveUnit.end();

		while (Index > 0 && Start != End)
		{
			++Start;
			--Index;
		}

		if (Start != End)
		{
			m_pCurTarget->push_back((*Mon)[*Start]);
			LiveUnit.erase(Start);
		}

		--Ran;
	}

	return;
}
void CActionPhase::RandomColumnTarget() 
{
	vector<SPTR<CBattleUnit>>* Char = m_pTurnMgr->GetCurBattleCharVec();

	UINT FrontCount = m_pTurnMgr->GetLiveUnitCount(CHARACTER_FRONT);
	UINT RearCount = m_pTurnMgr->GetLiveUnitCount(CHARACTER_REAR);


	if (0 == FrontCount && 0 == RearCount)
		return;
	else if (0 != FrontCount && 0 == RearCount)
		SettingColumnTarget(CHARACTER_FRONT);
	else if (0 == FrontCount && 0 == RearCount)
		SettingColumnTarget(CHARACTER_REAR);
	else 
	{
		UNITCOLUMN Ran = (UNITCOLUMN)CSJRandom::RandomInt(CHARACTER_FRONT, CHARACTER_REAR);

		SettingColumnTarget(Ran);
	}
	return;
}
void CActionPhase::SettingColumnTarget(const UNITCOLUMN& _Op)
{
	vector<SPTR<CBattleUnit>>* Char = m_pTurnMgr->GetCurBattleCharVec();
	vector<SPTR<CBattleUnit>>* Mon = m_pTurnMgr->GetCurBattleCharVec();

	if (nullptr == Char || nullptr == Mon)
		return;

	switch (_Op)
	{
	case CHARACTER_FRONT: 
	{
		for (size_t i = 0; i < 2; i++)
		{
			if (nullptr == (*Char)[i] || true == (*Char)[i]->isDead())
				continue;

			m_pCurTarget->push_back((*Char)[i]);
		}
	}
		return;
	case CHARACTER_REAR: 
	{
		for (size_t i = 2; i < 5; i++)
		{
			if (nullptr == (*Char)[i] || true == (*Char)[i]->isDead())
				continue;

			m_pCurTarget->push_back((*Char)[i]);
		}
	}
		return;
	case MONSTER_FRONT: 
	{
		for (size_t i = 1; i < 3; i++)
		{
			if (nullptr == (*Mon)[i] || true == (*Mon)[i]->isDead())
				continue;

			m_pCurTarget->push_back((*Mon)[i]);
		}
	}
		return;
	case MONSTER_REAR: 
	{
		if (nullptr == (*Mon)[0] || true == (*Mon)[0]->isDead())
			return;

		m_pCurTarget->push_back((*Mon)[0]);
		
	}
		return;
	}
}
void CActionPhase::SettingAllTarget()
{
	vector<SPTR<CBattleUnit>>* Char = m_pTurnMgr->GetCurBattleCharVec();

	for (size_t i = 0; i < Char->size(); i++)
	{
		if (nullptr == (*Char)[i])
			continue;

		if (true == (*Char)[i]->isDead())
			continue;

		m_pCurTarget->push_back((*Char)[i]);
	}
}
void CActionPhase::CurTargetUnitEndReaction()
{
	for (size_t i = 0; i < m_pCurTarget->size(); i++)
	{
		if (nullptr == (*m_pCurTarget)[i])
			continue;

		if (false == (*m_pCurTarget)[i]->isDead())
			(*m_pCurTarget)[i]->EndReAction();
	}

}