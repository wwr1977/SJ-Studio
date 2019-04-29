#include "Precom.h"
#include "NPCBase.h"
#include "Player.h"
#include "Effect.h"
#include "GameMaster.h"
#include "StoreUI.h"
#include <GameSprite.h>
vector<tstring>		CNPCBase::IdleDir;

bool CNPCBase::bScriptEvent = true;

void CNPCBase::InitNpcBase()
{
	IdleDir.clear();

	IdleDir.push_back(_T("_Idle_Down"));
	IdleDir.push_back(_T("_Idle_Up"));
	IdleDir.push_back(_T("_Idle_Left"));
	IdleDir.push_back(_T("_Idle_Right"));


}
CNPCBase::CNPCBase()
	:m_FixNpcRen(nullptr), m_AniNpcRen(nullptr)
	, m_PlayerPushCol(nullptr)
	, m_IdleType(FIXIDLE)
	, m_NpcKName(_T(""))
	, m_ScriptCol(nullptr)
	, m_DefaultDir(IDLE_DOWN)
	, m_fAccTime(0.f)
	, m_CurHideType(HIDE_DEFAULT)
	, m_bHide(false)
	, m_bHideEventEnd(false)
	, m_bEffectPlay(false)
	, m_CurNPCType(DIALOG_NPC)
{
	m_vecFixIdleSprite.clear();
	m_vecFixIdleSprite.assign(MAXIDLEDIR,nullptr);
}


CNPCBase::~CNPCBase()
{
	m_vecFixIdleSprite.clear();
	IdleDir.clear();
}
void CNPCBase::Init()
{

}
void CNPCBase::InitData(const NPCDATA& _Data)
{
	if (_T("") == _Data.NpcName || _Data.IdleType == MAXIDLETYPE)
		return;

	m_NpcName = _Data.NpcName;
	m_NpcKName = _Data.NpcKName;

	switch (_Data.IdleType)
	{
	case FIXIDLE:
		SettingFixNpc();
		return;
	case ANIIDLE:
		SettingAniNpc();
		return;
	}

}
void CNPCBase::Update()
{
	if(!m_bHideEventEnd  && m_bHide)
		HideUpdate();
}
void CNPCBase::LastUpdate()
{
	if (nullptr == m_PlayerPushCol || true == m_PlayerPushCol->IsEmptyColSet())
		return;

	m_PlayerPushCol->ClearSet();
}
void CNPCBase::CreatePlayerCollision(const COLFIGURE& _Figure, const COLTYPE& _ColType)
{
	m_PlayerPushCol = ACTOR->CreateCom<CCollision>(COLDATA(_T("NPC")));
	m_PlayerPushCol->SetPivotPos(_Figure.Pos);
	m_PlayerPushCol->SetColSize(_Figure.Size);
	m_PlayerPushCol->SetColType (_ColType);


	m_PlayerPushCol->SetEnter(this, &CNPCBase::PushEnter);
	m_PlayerPushCol->SetStay(this, &CNPCBase::PushStay);
	m_PlayerPushCol->SetExit(this, &CNPCBase::PushExit);

}
void CNPCBase::CreateScriptCollision(const COLFIGURE& _Figure, const COLTYPE& _ColType)
{
	m_ScriptCol = ACTOR->CreateCom<CCollision>(COLDATA(_T("NPC")));
	m_ScriptCol->SetPivotPos(_Figure.Pos);
	m_ScriptCol->SetColSize(_Figure.Size);
	m_ScriptCol->SetColType(_ColType);

	m_ScriptCol->SetEnter(this, &CNPCBase::ScriptExit);
	m_ScriptCol->SetStay(this, &CNPCBase::ScriptStay);
	m_ScriptCol->SetExit(this, &CNPCBase::ScriptExit);
}
void CNPCBase::CreateSubCollision(const COLFIGURE& _Figure, const COLTYPE& _ColType)
{

}
void CNPCBase::HideUpdate()
{

	switch (m_CurHideType)
	{
	case CNPCBase::HIDE_DEFAULT:
		DefaultHideUpdate();
		break;
	case CNPCBase::HIDE_VIBRATION:
		VibrationHideUpdate();
		break;
	case CNPCBase::HIDE_EFFECT:
		EffectHideUpdate();
		break;
	case CNPCBase::HIDE_DEADANI:
		DeadHideUpdate();
		break;
	}

	m_fAccTime += DELTATIME;
}
void CNPCBase::DefaultHideUpdate() 
{
	if (m_fAccTime >= 2.f) 
	{
		HideEventEnd();
		return;
	}

	float Alpha = 1.3f - m_fAccTime;
	SetAlpha(Alpha);
}
void CNPCBase::VibrationHideUpdate() 
{
	if (m_fAccTime >= 2.8f)
	{
		HideEventEnd();
		return;
	}


	if (m_fAccTime < 0.8f) 
	{
		if (true == CTimerMgr::Pulse(0.025f)) 
		{
			Vec2 Pivot = Vec2{ CSJRandom::RandomFloat(-20.f,20.f),0.f };
			SetRenPivot((0.8f - m_fAccTime)*Pivot);
		}
	}
	else if (m_fAccTime >= 0.8f) 
	{
		float Alpha = 1.8f - m_fAccTime;
		SetRenPivot(Vec2{0.f,0.f});
		SetAlpha(Alpha);
	}
}
void CNPCBase::EffectHideUpdate() 
{
	if (m_fAccTime >= 3.f)
	{
		HideEventEnd();
		return;
	}

	if (m_fAccTime >= 0.6f && !m_bEffectPlay)
	{
		m_bEffectPlay = true;
		CClientGlobal::GameMaster->EffectOn(_T("HideEffect"), TRANS->GetPos(), false);
	}

	float Alpha = 1.4f - m_fAccTime;
	SetAlpha(Alpha);

}
void CNPCBase::DeadHideUpdate()
{
	if (m_fAccTime >= 1.8f)
	{
		HideEventEnd();
		return;
	}

	float Alpha = 1.5f - m_fAccTime;
	SetAlpha(Alpha);
}
void CNPCBase::SetAniIntervalTime(const float& _Time)
{
	if (nullptr == m_AniNpcRen)
		return;

	m_AniNpcRen->SetInterval(_Time);
}
void CNPCBase::SetStoreNPC(const NPCTYPE& _NPCType)
{
	m_CurNPCType = _NPCType;
}
void CNPCBase::SettingFixNpc()
{
	m_FixNpcRen = ACTOR->CreateCom<CFixRenderer>(RENDATA(LAYER_MAPOBJECT, 0.95f));
	
	for (size_t i = 0; i < MAXIDLEDIR; i++)
	{
		if (nullptr != COMRESMGR->FindGameSprite(m_NpcName + IdleDir[i]))
			m_vecFixIdleSprite[i] = COMRESMGR->FindGameSprite(m_NpcName + IdleDir[i]);
	}

	m_FixNpcRen->SetSprite(m_vecFixIdleSprite[IDLE_DOWN]);
}
void CNPCBase::SettingAniNpc() 
{
	m_IdleType = ANIIDLE;
	m_AniNpcRen = ACTOR->CreateCom<CAniRenderer>(RENDATA(LAYER_MAPOBJECT, 0.95f));

	for (size_t i = 0; i < MAXIDLEDIR; i++)
			m_AniNpcRen->CreateAnimation(m_NpcName + IdleDir[i], ANI_LOOP);
		
	m_AniNpcRen->ChangeAni(m_NpcName + IdleDir[IDLE_DOWN]);
}
void CNPCBase::SetAlpha(const float& _Alpha)
{
	switch (m_IdleType)
	{
	case FIXIDLE:
		if (nullptr != m_FixNpcRen)
			m_FixNpcRen->SetAlpha(_Alpha);
		return;
	case ANIIDLE:
		if (nullptr != m_AniNpcRen)
			m_AniNpcRen->SetAlpha(_Alpha);
		return;
	}
}
void CNPCBase::SetRenPivot(const Vec2& _Pivot)
{
	switch (m_IdleType)
	{
	case FIXIDLE:
		if (nullptr != m_FixNpcRen)
			m_FixNpcRen->SetRenPivot(_Pivot);
		return;
	case ANIIDLE:
		if (nullptr != m_AniNpcRen)
			m_AniNpcRen->SetRenPivot(_Pivot);
		return;
	}
}
void CNPCBase::SetHideType(const HIDETYPE& _Type)
{
	m_CurHideType = _Type;
}
void CNPCBase::SetDead_Dying_NPC()
{
	switch (m_IdleType)
	{
	case FIXIDLE:
		break;
	case ANIIDLE:
		m_AniNpcRen->CreateAnimation(m_NpcName + _T("_Dead"), ANI_LOOP);
		m_AniNpcRen->CreateAnimation(m_NpcName + _T("_Dying"), ANI_LOOP);
		break;
	}

}
void CNPCBase::ChangeAni_Dying() 
{
	switch (m_IdleType)
	{
	case FIXIDLE:
		break;
	case ANIIDLE:
		m_AniNpcRen->ChangeAni(m_NpcName + _T("_Dying"));
		break;
	}
}
void CNPCBase::ChangeAni_Dead()
{
	switch (m_IdleType)
	{
	case FIXIDLE:
		break;
	case ANIIDLE:
		m_AniNpcRen->ChangeAni(m_NpcName + _T("_Dead"));
		break;
	}
}
void CNPCBase::Reset()
{
	m_bHide = false;
	m_bHideEventEnd = false;
	m_bEffectPlay = false;
	m_fAccTime = 0.f;
	ACTOR->Off();
}
const bool CNPCBase::IsHide()
{
	return m_bHide;
}
const bool CNPCBase::IsHideEventEnd() 
{
	return m_bHideEventEnd;
}
void CNPCBase::InitDefaultDir()
{
	ChangeIdleDir(m_DefaultDir);
}
void CNPCBase::SetDefaultDir(const IDLEDIR& _Dir)
{
	if (_Dir >= MAXIDLEDIR)
		return;

	m_DefaultDir = _Dir;
}
void CNPCBase::ChangeIdleDir(const IDLEDIR& _Dir)
{
	if (_Dir >= MAXIDLEDIR)
		return;

	switch (m_IdleType)
	{
	case FIXIDLE:
		if (nullptr != m_FixNpcRen && nullptr != m_vecFixIdleSprite[_Dir])
			m_FixNpcRen->SetSprite(m_vecFixIdleSprite[_Dir]);
		return;
	case ANIIDLE: 
		if (nullptr != m_AniNpcRen)
			m_AniNpcRen->ChangeAni(m_NpcName + IdleDir[_Dir]);
		return;
	}
}
const CNPCBase::IDLEDIR CNPCBase::CalNPCDir()
{
	Vec3 PlayerPos = CClientGlobal::MainPlayer->GetActorPos();
	Vec3 NPCPos = TRANS->GetPos();

	Vec2 CalPos = Vec2(PlayerPos.x - NPCPos.x, PlayerPos.y - NPCPos.y);
	D3DXVec2Normalize(&CalPos, &CalPos);

	if (CSJMath::ABS(CalPos.x) > CSJMath::ABS(CalPos.y)) 
	{
		if (CalPos.x >= 0)
			return IDLE_RIGHT;
		else
			return IDLE_LEFT;
	}
	else 
	{
		if (CalPos.y >= 0)
			return IDLE_UP;
		else
			return IDLE_DOWN;
	}

	return MAXIDLEDIR;
}
void CNPCBase::PushEnter(SPTR<CCollision> _pThis, SPTR<CCollision> _pOtherCol)
{
	if (CClientGlobal::MainPlayer->ParentActObject() != _pOtherCol->ParentActObject())
		return;

	Vec3 PushDir = -CClientGlobal::MainPlayer->GetCurMoveDir();

	float Push = 0.01f;

	while (TRUE == _pThis->FICheck(_pOtherCol, Push*PushDir))
		Push += 0.01f;

	CClientGlobal::MainPlayer->PushPlayer(Push*PushDir);
	
}
void CNPCBase::PushStay(SPTR<CCollision> _pThis, SPTR<CCollision> _pOtherCol)
{
	if (CClientGlobal::MainPlayer->ParentActObject() != _pOtherCol->ParentActObject())
		return;

	int a = 0;
}
void CNPCBase::PushExit(SPTR<CCollision> _pThis, SPTR<CCollision> _pOtherCol)
{
	if (CClientGlobal::MainPlayer->ParentActObject() != _pOtherCol->ParentActObject())
		return;

	int a = 0;
}
void CNPCBase::ScriptEnter(SPTR<CCollision> _pThis, SPTR<CCollision> _pOtherCol)
{
	if (CClientGlobal::MainPlayer->ParentActObject() != _pOtherCol->ParentActObject())
		return;

	int a = 0;
}
void CNPCBase::ScriptStay(SPTR<CCollision> _pThis, SPTR<CCollision> _pOtherCol) 
{
	if (CClientGlobal::MainPlayer->ParentActObject() != _pOtherCol->ParentActObject())
		return;

	if (false == bScriptEvent)
		return;

	if (TRUE == KEYDOWN("Enter") && !CClientGlobal::GameMaster->IsStortCurtainWait()) 
	{
		switch (m_CurNPCType)
		{
		case CNPCBase::DIALOG_NPC:
		{
			bScriptEvent = false;
			CClientGlobal::GameMaster->MenuMgrEventOn(false);
			CClientGlobal::MainPlayer->PlayerPause(true);
			ChangeIdleDir(CalNPCDir());
			CClientGlobal::GameMaster->ShowScriptEvent(m_NpcName);
		}
			break;
		case CNPCBase::WEAPON_STORE_NPC:
			CClientGlobal::GameMaster->StoreOpenEvent(WEAPON_STORE);
			break;
		case CNPCBase::ARMOR_STORE_NPC:
			CClientGlobal::GameMaster->StoreOpenEvent(ARMOR_STORE);
			break;
		case CNPCBase::ACCESSORIES_STORE_NPC:
			CClientGlobal::GameMaster->StoreOpenEvent(ACCESSORIES_STORE);
			break;
		}
	}
	
}
void CNPCBase::ScriptExit(SPTR<CCollision> _pThis, SPTR<CCollision> _pOtherCol)
{
	if (CClientGlobal::MainPlayer->ParentActObject() != _pOtherCol->ParentActObject())
		return;

	int a = 0;
}
void CNPCBase::NPCHideEvent()
{
	m_bHide = true;
	m_bHideEventEnd = false;
	m_fAccTime = 0.f;

	if (HIDE_DEADANI == m_CurHideType)
		ChangeAni_Dead();
}
void CNPCBase::HideEventEnd()
{
	SetAlpha(1.f);
	m_bHideEventEnd = true;
	ACTOR->Off();
}