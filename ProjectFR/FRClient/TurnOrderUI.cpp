#include "Precom.h"
#include "TurnOrderUI.h"
#include"BattleUnit.h"
#include<GameMultiSprite.h>


SPTR<CGameMultiSprite>				CTurnOrderUI::m_ActionIconSprite = nullptr ;
bool								CTurnOrderUI::m_bSetSprite = false;

CTurnOrderUI::CTurnOrderUI()
	:m_FrameRen(nullptr), m_pPairUnit(nullptr)
	, m_ActionIconRen(nullptr), m_ActionIcon(nullptr)
	, m_bUnit(true), m_ActionIconBack(nullptr)
	, m_fAccTime(0.0f)
	, m_CurEvent(EVENT_NO)
	, m_bEntryUI(false)
{

}


CTurnOrderUI::~CTurnOrderUI()
{
}
void CTurnOrderUI::SetActionIcon(SPTR<CGameMultiSprite> _MultiSprite)
{
	if (nullptr == _MultiSprite) 
	{
		assert(nullptr);
		return;
	}

	m_ActionIconSprite = _MultiSprite;
	m_bSetSprite = true;
}
void CTurnOrderUI::Init()
{
	if (false == m_bSetSprite) 
	{
		SetActionIcon(COMRESMGR->FindMultiSprite(_T("ActionIcon")));
	}

	m_FrameRen = ACTOR->CreateCom<CFixRenderer>(RENDATA(LAYER_UI,0.9f,true));
	m_ActionIconRen = ACTOR->CreateCom<CFixRenderer>(RENDATA(LAYER_UI, 0.9f, true));
	m_ActionIconRen->SetCustomSize(30.0f, 30.0f);


	m_ActionIconBack = ACTOR->CreateCom<CFixRenderer>(RENDATA(LAYER_UI, 0.9f, true));
	m_ActionIconBack->SetSprite(_T("ActionIconBack"));
	m_ActionIconBack->SetCustomSize(Vec2{30.0f,45.0f});
	m_ActionIconBack->Off();

	m_ActionIcon = ACTOR->CreateCom<CAniRenderer>(RENDATA(LAYER_UI, 0.9f, {0.0f,0.0f,0.0f}, true));
	

	ANIDATA AniData(_T("ArmorIcon"), _T("ArmorIcon"));
	AniData.SettingFullRage({ 0,4 }, 0.2f);
	m_ActionIcon->CreateAnimation(AniData);
	AniData.AniSpriteName = _T("HitIcon"); AniData.AnimationName = _T("HitIcon");
	m_ActionIcon->CreateAnimation(AniData);
	AniData.AniSpriteName = _T("ShieldIcon"); AniData.AnimationName = _T("ShieldIcon");
	m_ActionIcon->CreateAnimation(AniData);
	AniData.AniSpriteName = _T("SkillIcon"); AniData.AnimationName = _T("SkillIcon");
	m_ActionIcon->CreateAnimation(AniData);


	m_ActionIcon->ChangeAni(_T("SkillIcon"));
	m_ActionIcon->Off();


}
void CTurnOrderUI::Update()
{
	if (EVENT_NO == m_CurEvent)
		return;


	switch (m_CurEvent)
	{
	case CTurnOrderUI::EVENT_DEAD:
		DeadUpdate();
		break;
	case CTurnOrderUI::EVENT_RELOCATION:
		RelocationUpdate();
		break;
	}
	

	m_fAccTime += DELTATIME;
}
void CTurnOrderUI::FontRender()
{

}
void CTurnOrderUI::SetUnit(CBattleUnit* _pUnit)
{
	if (nullptr == _pUnit) 
	{
		assert(nullptr);
		return;
	}

	m_pPairUnit = _pUnit;
	
	if (nullptr == m_FrameRen) 
	{
		assert(nullptr);
		return;
	}

	m_FrameRen->SetSprite(Frame(m_pPairUnit->GetUnitName()));
	m_bUnit = m_pPairUnit->IsCharacter();
}

void CTurnOrderUI::On()
{
	ACTOR->On();
	m_FrameRen->SetAlpha(0.9f);
	m_ActionIconBack->SetAlpha(0.9f);
	m_ActionIcon->SetAlpha(0.9f);
	m_ActionIconRen->SetAlpha(0.9f);
}
void CTurnOrderUI::Off()
{
	ACTOR->Off();
	m_bEntryUI = false;
}
void CTurnOrderUI::ActionIconOn(const int& _IconIndex)
{
	SetActionIcon(_IconIndex);
	//m_ActionIconRen->On();
	m_ActionIconBack->On();
	m_ActionIcon->On();
}
void CTurnOrderUI::ActionIconOff() 
{
	m_ActionIconRen->Off();
	m_ActionIconBack->Off();
	m_ActionIcon->Off();
}
CBattleUnit* CTurnOrderUI::GetBattleUnit()
{
	return m_pPairUnit;
}

void CTurnOrderUI::SetPos(const Vec3& _Pos)
{
	ACTOR->SetPos(_Pos);
}
void CTurnOrderUI::SetUISize(const Vec2& _Size)
{
	m_FrameRen->SetCustomSize(_Size);
	
	
	Vec2 IconPos = 0.5f* Vec2(_Size.x, -_Size.y);
	

	IconPos -= Vec2{ 3.0f,-4.0f };
	m_ActionIconBack->SetRenPivot(IconPos + Vec2{-2.0f,0.0f});
	m_ActionIcon->SetRenPivot(IconPos);

}

tstring CTurnOrderUI::GetUnitName()
{
	if (nullptr == m_pPairUnit) 
	{
		return _T("");
	}

	return m_pPairUnit->GetUnitName();
}
const CTurnOrderUI::TURNUIEVENT CTurnOrderUI::GetCurEvent()
{
	return m_CurEvent;
}
void CTurnOrderUI::SetUIAlpha(const float& _Alpha)
{
	if (nullptr == m_FrameRen) 
	{
		assert(nullptr);
		return;
	}
	
	m_FrameRen->SetAlpha(0.9f*_Alpha);
}
void CTurnOrderUI::SetActionIcon(const int& _IconIndex)
{
	/*if (_IconIndex >= (int)m_ActionIconSprite->GetSpriteCount()) 
	{
		return;
	}

	m_ActionIconRen->SetSprite(m_ActionIconSprite->GetSprite(_IconIndex));*/
}
void CTurnOrderUI::EntryUI(const bool& _Entry)
{
	m_bEntryUI = _Entry;
}
const bool CTurnOrderUI::IsEntryUI()
{
	return m_bEntryUI;
}
void CTurnOrderUI::DeadUpdate()
{
	if (m_fAccTime > 0.6f)
	{
		m_CurEvent = EVENT_NO;
		m_fAccTime = 0.f;
		Off();
		return;
	}

	float W = m_fAccTime / 0.6f;

	Vec3 MovePos = CSJMath::Vec3Lerp(&m_EventStartPos, &m_EventEndPos, W);

	// 모드1 왼쪽이동
	TRANS->SetPos(MovePos);

	// 모드2 알파 감소
	float Alpha = 1.f - W;

	SetUIAlpha(Alpha);


}
void CTurnOrderUI::RelocationUpdate()
{
	if (m_fAccTime > 0.6f)
	{
		m_CurEvent = EVENT_NO;
		m_fAccTime = 0.f;
		return;
	}

	float W = m_fAccTime / 0.6f;

	Vec3 MovePos = CSJMath::Vec3Lerp(&m_EventStartPos, &m_EventEndPos, W);

	// 모드1 오른쪽이동
	TRANS->SetPos(MovePos);

	// 모드2 알파 증가
	if (true == m_bEntryUI) 
		SetUIAlpha(W);
	

}
void CTurnOrderUI::StandBy()
{
	m_ActionIconBack->Off();
	m_ActionIcon->Off();
	SetUIAlpha(1.0f);
	m_CurEvent = EVENT_NO;
	m_fAccTime = 0.f;
}
bool CTurnOrderUI::IsUnit()
{
	return m_bUnit;
}
const bool CTurnOrderUI::IsUnitDead()
{
	if (nullptr == m_pPairUnit)
		return false;


	return m_pPairUnit->isDead();
}
const bool CTurnOrderUI::IsPopUI()
{
	if (nullptr == m_pPairUnit)
		return false;

	// 몬스터이면서 죽은 경우에는 리스트에서 빼도록 한다
	if (false == m_bUnit && true == m_pPairUnit->isDead())
		return true;

	return false;

}
void CTurnOrderUI::MonsterDeadEvent()
{
	// 유닛이 캐릭터인경우 이 동작을 수행하지 않는다.
	if (true == m_bUnit)
		return;

	m_CurEvent = EVENT_DEAD;
	m_fAccTime = 0.0f;
	m_EventStartPos = TRANS->GetPos();
	m_EventEndPos = m_EventStartPos + Vec3{ -75.0f,0.0f,0.0f };
}

void CTurnOrderUI::RelocationEvent(const Vec3& _EndPos)
{
	if (true == m_bEntryUI) 
	{
		m_EventEndPos = _EndPos;
		m_EventStartPos = _EndPos + Vec3{-75.f,0.f,0.f};
	}
	else 
	{
		if (_EndPos == TRANS->GetPos())
			return;
		m_EventEndPos = _EndPos;
		m_EventStartPos = TRANS->GetPos();
	}
	m_CurEvent = EVENT_RELOCATION;
	m_fAccTime = 0.f;
}
const bool CTurnOrderUI::CompareDex(SPTR<CTurnOrderUI> _Other)
{
	if (nullptr ==  m_pPairUnit || nullptr == _Other)
	{
		assert(nullptr);
		return false;
	}
	return m_pPairUnit->CompareDex(_Other->GetBattleUnit());
}
const bool CTurnOrderUI::operator>(SPTR<CTurnOrderUI> _Other)
{
	if (nullptr == m_pPairUnit || nullptr == _Other || nullptr == _Other->GetBattleUnit())
	{
		assert(nullptr);
		return false;
	}
	UINT Left = m_pPairUnit->GetCurStat().Dex;
	UINT Right = m_pPairUnit->GetCurStat().Dex;

	return Left > Right;
}
const bool CTurnOrderUI::operator<(SPTR<CTurnOrderUI> _Other)
{
	if (nullptr == m_pPairUnit || nullptr == _Other || nullptr == _Other->GetBattleUnit())
	{
		assert(nullptr);
		return false;
	}
	UINT Left = m_pPairUnit->GetCurStat().Dex;
	UINT Right = m_pPairUnit->GetCurStat().Dex;

	return Left < Right;
}