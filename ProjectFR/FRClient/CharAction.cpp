#include "Precom.h"
#include "CharAction.h"
#include "BattleUnit.h"


CCharAction::CCharAction()
	:m_bEnd(false), m_iActionIndex(-1)
{

}


CCharAction::~CCharAction()
{
}
void CCharAction::StateReset()
{
	/*m_iActionIndex = m_pUnit->GetCutActionIndex();

	switch (m_iActionIndex)
	{

	case 0:m_pUnit->ChangeAni(Limit(m_UnitName)); break;
	case 1:
	{
		if (Vargas >= (CharacterIndex)m_pUnit->GetCharIndex())
		{
			m_pUnit->ChangeAni(DAtk(m_UnitName));
		}
		else
		{
			m_pUnit->ChangeAni(Atk(m_UnitName));
		}

	}
	break;
	default:
		break;
	}*/


	/*m_fAccTime = 0.0f;
	m_bEnd = false;
	m_pUnit->EndAction(false);*/
}
void CCharAction::Update()
{
	/*if (false == m_bEnd && m_pUnit->IsCurAniEnd())
	{
		m_fAccTime += DELTATIME;
	}

	if (false == m_bEnd && m_fAccTime >= 2.0f)
	{
		m_pUnit->EndAction();
		m_bEnd = true;
		m_fAccTime = 0.0f;
	}*/

	/*if (m_fActionTime <= m_fAccTime) 
	{
		m_pUnit->EndAction();
		m_bEnd = true;
		m_fAccTime = 0.0f;
	}


	ScriptUpdate();

	while (false == m_SoundQueue.empty()) 
	{
		m_UnitSoundPlayer->PlaySound(m_SoundQueue.front());
		m_SoundQueue.pop();
	}

	if (0 != m_iMoveScriptCount) 
	{
		if (MOVEDATA::HIDEMODE_NONE == m_CurMoveScript.HideMode) 
			MoveUpdate();
		else
			HideUpdate();
		
	}


	m_fAccTime += DELTATIME;*/
}
// 스킬을 시전한 캐릭터의 애니메이션&동작&사운드를 업데이트 하는 함수
//void CCharAction::ScriptUpdate()
//{
//	// 애니메이션 스크립트 & 애니메이션 업데이트
//	while (0 != m_iAniScriptCount && m_CurAniScript.StartTime <= m_fAccTime ) 
//	{
//		m_pUnit->ChangeAni(m_CurAniScript.AnimationIndex);
//		m_qAniScript.pop();
//		--m_iAniScriptCount;
//
//		if (0 != m_iAniScriptCount ) 
//		{
//			m_CurAniScript =  m_qAniScript.front();	
//		}
//	}
//
//	// Move Script 업데이트
//	while (0 != m_iMoveScriptCount && m_qMoveScript.front().MoveStartTime <= m_fAccTime )
//	{
//		m_CurMoveScript = m_qMoveScript.front();
//		m_qMoveScript.pop();
//		--m_iMoveScriptCount;
//		
//	}
//
//
//	// 사운드를 Queue에 대기
//	for (int i = 0; i < min((int)m_vecSoundScript.size(),ACTIONSCRIPT::MAXCHARSOUNDCOUNT); i++)
//	{
//		if (true == m_vecSoundScript[i].Playing) 
//		{
//			continue;
//		}
//
//		if (m_fAccTime >= m_vecSoundScript[i].StartTime) 
//		{
//			m_SoundQueue.push(m_vecSoundScript[i].SoundName);
//			m_vecSoundScript[i].Playing = true;
//		}
//	}
//
//
//}
//void CCharAction::HideUpdate() 
//{
//	if (m_CurMoveScript.MoveStartTime == 0.0f &&
//		m_CurMoveScript.MoveTime == 0.0f &&
//		m_CurMoveScript.HideTime == 0.0f)
//	{
//		return;
//	}
//
//	float AlphaRatio = (m_fAccTime - m_CurMoveScript.MoveStartTime) / m_CurMoveScript.HideTime;
//	
//	switch (m_CurMoveScript.HideMode)
//	{
//	case MOVEDATA::HIDEMODE_HIDE:
//		m_pUnit->SetAlpha(1.0f - AlphaRatio);
//		break;
//	case MOVEDATA::HIDEMODE_SHOW:
//		m_pUnit->SetAlpha(AlphaRatio);
//		break;
//	}
//
//}
//void CCharAction::MoveUpdate()
//{
//	if (m_CurMoveScript.MoveStartTime == 0.0f &&
//		m_CurMoveScript.MoveTime == 0.0f &&
//		m_CurMoveScript.HideTime == 0.0f	) 
//	{
//		return;
//	}
//
//	float Weight = (m_fAccTime - m_CurMoveScript.MoveStartTime ) / m_CurMoveScript.MoveTime;
//
//	Vec2 MovePos = CSJMath::Vec2Lerp(&m_CurMoveScript.MoveStartPos, &m_CurMoveScript.MoveEndPos, Weight);
//
//	m_pUnit->SetRenPivot(MovePos);
//}