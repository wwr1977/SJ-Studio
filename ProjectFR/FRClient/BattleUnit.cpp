#include "Precom.h"
#include "BattleUnit.h"
#include <StateMgr.h>
#include <GameSprite.h>
#include <GameMultiSprite.h>
#include "CharIdle.h"
#include "CharAction.h"
#include "CharReAction.h"
#include "CharDying.h"
#include "CharDead.h"
#include "CharWin.h"
#include "CharAppear.h"
#include "MonIdle.h"
#include "MonAction.h"
#include "MonReAction.h"
#include "MonDead.h"
#include "MonAppear.h"

#include "CharSelectEffect.h"
#include "UnitStatusBar.h"
#include "TurnOrderUI.h"
#include "TurnMgr.h"
#include "TargetUI.h"
#include "UnitFont.h"
#include "CharResultUI.h"
#include "BossGauge.h"


D3DXCOLOR CBattleUnit::FocusMonBackColor = D3DXCOLOR(1.0f,0.0f,0.0f,1.0f);
CTurnMgr* CBattleUnit::pUnitTurnMgr = nullptr;


CBattleUnit::CBattleUnit()
	:m_PairStatus(nullptr), m_bSelectAction(false), m_bEndAction(false)
	, m_iTurnCount(1), m_iCurActionIndex(-1), m_iCharIndex(MAXBATTLECHARACTER)
	, m_PairTurnUI(nullptr), m_bCharacter(false), m_iLevel(1)
	, m_pUnitStatTable(nullptr), m_UnitPara(STATPARAMETERDATA())
	, m_CurUnitStat(STAT())
	, m_fFilterTime(0.1f), m_bFilterEnd(false), m_fAccFilterTime(0.0f)
	, m_iFilterCount(0), m_bFilterOn(false), m_bLevelUpdate(false)
	, m_fVibTime(0.0f), m_fAccVibTime(0.0f), m_bVibOn(false)
	, m_UnitSound(nullptr), m_fAlpha(0.9f), m_iTeamNumber(0)
	, m_MonReactionRen(nullptr), m_UnitMiddlePivot(Vec3(0.0f, 0.0f, 0.0f))
	, m_DemageFontPivot(Vec3(0.0f, 0.0f, 0.0f)), m_CurUnitState(BATTLE_IDLE)
	, m_UnitCol(nullptr), m_PairTargetUI(nullptr)
	, m_bDead(false), m_SelectSkillNo(-1), m_SelectSkillLevel(-1)
	, m_SelectTarget(MAXTARGET), m_UnitPosRen(nullptr)
	, m_ItemStat(STAT()), m_BaseStat(STAT())
	, m_PairUnitFont(nullptr), m_bCurBattle(false)
	, m_bTargetOverride(false), m_bCalExp(false)
	, m_PairResultUI(nullptr), m_UnitName(UNITNAME())
	, m_CharacterLine(0), m_BossGauge(nullptr)
	, m_bBoss(false)
{
	memset(m_CurUnitStatus, 0, LEVELDATA::MAXSKILLCOST);
	m_CurUnitStatus[2] = -1;	// 버스트 게이지 만들때까진 무제한
	m_CurTargetUnit.clear();

}


CBattleUnit::~CBattleUnit()
{
	m_CurTargetUnit.clear();
	m_PairUnitFont = nullptr;
}
void CBattleUnit::SetTurnMgr(CTurnMgr* _TurnMgr)
{
	if (nullptr == _TurnMgr)
		return;

	pUnitTurnMgr = _TurnMgr;
}
void CBattleUnit::InitData(const tstring& _UnitName)
{
	int CharIndex = 0;
	m_UnitSound = ACTOR->CreateCom<CSoundPlayer>();

	// 만일 캐릭터의 이름이 들어왔다면 이 배틀유닛을 캐릭터로 초기화

	for (; CharIndex < MAXBATTLECHARACTER; ++CharIndex)
	{
		if (_UnitName == CClientGlobal::CharacterName[CharIndex].UnitName) 
		{
			m_UnitName = CClientGlobal::CharacterName[CharIndex];
			m_iCharIndex = CharIndex;
			InitalizeCharacter();
			return;
		}
	}

	// 유닛이름으로 몬스터가 들어왔다면 글로벌에서 적용가능한지 여부를 파악한뒤
	// 몬스터로 초기화
	if (false == CClientGlobal::IsMonsterName(_UnitName)) 
		return;
	
	m_UnitName.UnitName = _UnitName;
	InitalizeMonster();

}
void CBattleUnit::InitData(CharacterIndex _UnitIndex)
{
	m_UnitSound = ACTOR->CreateCom<CSoundPlayer>();

	if (MAXBATTLECHARACTER <= _UnitIndex) 
	{
		assert(nullptr);
		return;
	}


	m_UnitName = CClientGlobal::CharacterName[_UnitIndex];
	m_iCharIndex = _UnitIndex;

	InitalizeCharacter();
}
void CBattleUnit::SetDead(const bool& _Dead/* = true*/)
{
	m_bDead = _Dead;
}
bool CBattleUnit::isDead()
{
	return m_bDead;
}
void CBattleUnit::Init() 
{
	m_PairMgr = new CStateMgr();

	CActObject* Target = CActObject::CreateActObject(_T("TargetUI"), ACTOR->GetScene());
	m_PairTargetUI = Target->CreateCom<CTargetUI>();
	m_PairTargetUI->SetUnit(this);
	m_PairTargetUI->TargetUIOff();


	m_PairUnitFont = pUnitTurnMgr->CreateActObject(_T("UnitFont"))->CreateCom<CUnitFont>(this);
	
	/*m_UnitPosRen = ACTOR->CreateCom<CLineRenderer>(RENDATA(LAYER_UIBACK, 0.5f));
	m_UnitPosRen->SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
	m_UnitPosRen->SetLineThick(2.0f);*/
}
void CBattleUnit::Update() 
{
	m_PairMgr->Update();
	FilterUpdate();
	VibrationUpdate();

}
void CBattleUnit::PostUpdate()
{
	if (true == TRANS->ChangeTranspose())
		SyncroDebugLine();

	/*if (TRUE == KEYDOWN("ChangeDebugMode"))
		m_UnitPosRen->ReverseRendering();
*/
}
void CBattleUnit::DebugRender()
{
	
}
bool CBattleUnit::ChangeAni(const ANIMATIONNAME& _AniIndex) 
{
	if (false == m_bCharacter) 
	{
		return false;
	}

	if (_AniIndex >= MAXANINAME || _AniIndex < 0) 
	{
		return false;
	}

	m_AniRen->ChangeAni( GetAnimationName(_AniIndex, m_UnitName.UnitName));
	return  true;
}
bool CBattleUnit::ChangeAni(const int& _Index) 
{
	return ChangeAni((ANIMATIONNAME)_Index);
}
bool CBattleUnit::ChangeAni(const tstring& _AniKey)
{
	return m_AniRen->ChangeAni(_AniKey);
}
SPTR<CTargetUI>	CBattleUnit::GetTargetUI()
{
	return m_PairTargetUI;
}
int	CBattleUnit::GetCharIndex()
{
	return (int)m_iCharIndex;
}
tstring CBattleUnit::GetUnitName()
{
	return m_UnitName.UnitName;
}
tstring CBattleUnit::GetUnitKName()
{
	return m_UnitName.UnitKName;
}
void CBattleUnit::ChangeState(int _Index)
{
	ChangeStateEvent();
	m_PairMgr->ChangeState(_Index);
}
int CBattleUnit::GetCurAniFrame()
{
	return m_AniRen->GetCurAniFrame();
}
bool CBattleUnit::IsCurAniEnd()
{
	return m_AniRen->IsEndAni();
}
bool CBattleUnit::IsBossMonster()
{
	return m_bBoss;
}
void CBattleUnit::SetStatusBar(CUnitStatusBar* _Pair)
{
	if (nullptr == _Pair) 
	{
		assert(nullptr);
		return;
	}

	m_PairStatus = _Pair;
}
void CBattleUnit::SetTurnUI(CTurnOrderUI* _Pair)
{
	if (nullptr == _Pair)
	{
		assert(nullptr);
		return;
	}

	m_PairTurnUI = _Pair;
}
void CBattleUnit::SetResultUI(CCharResultUI* _Pair)
{
	if (nullptr == _Pair)
	{
		assert(nullptr);
		return;
	}

	m_PairResultUI = _Pair;
}
SPTR<CSoundPlayer> CBattleUnit::GetUnitSoundPlayer()
{
	return m_UnitSound;
}
unordered_map<size_t, size_t>* CBattleUnit::GetUnitSkillDataMap()
{
	return &m_mapUnitSkillData;
}
const size_t CBattleUnit::GetSkillLevel(const size_t& _SkillNo)
{
	unordered_map<size_t, size_t>::iterator Find = m_mapUnitSkillData.find(_SkillNo);

	if (Find == m_mapUnitSkillData.end())
		return -1;

	return Find->second;
}

CUnitStatusBar* CBattleUnit::GetPairStatusBar()
{
	return m_PairStatus;
}
CTurnOrderUI* CBattleUnit::GetPairTurnUI()
{
	return m_PairTurnUI;
}
CCharResultUI* CBattleUnit::GetPairResultUI()
{
	return m_PairResultUI;
}
class CBossGauge* CBattleUnit::GetPairBossGauge()
{
	return m_BossGauge;
}
void CBattleUnit::On()
{
	ACTOR->On();
	SetAlpha();
	if (nullptr == m_PairStatus || nullptr == m_PairTurnUI)
	{
		assert(nullptr);
		return;
	}

	m_PairStatus->On();
	m_PairTurnUI->On();
	SyncroDebugLine();
}
void CBattleUnit::On(const Vec3& _SelectPos)
{
	ACTOR->On();
	
	if (nullptr == m_PairStatus || nullptr == m_PairTurnUI) 
	{
		assert(nullptr);
		return;
	}

	m_PairStatus->On();
	m_PairTurnUI->On();

	if (true == m_bCharacter) 
		m_SelectEffect->SetPos(_SelectPos);
	
	SyncroDebugLine();
}
void CBattleUnit::UnitOn(const Vec3& _SelectPos)
{
	ACTOR->On();

	if (true == m_bCharacter && nullptr != m_SelectEffect)
		m_SelectEffect->SetPos(_SelectPos);

	SyncroDebugLine();
}
void CBattleUnit::PairUIOn()
{
	if (nullptr == m_PairStatus || nullptr == m_PairTurnUI)
	{
		assert(nullptr);
		return;
	}

	m_PairStatus->On();
	m_PairTurnUI->On();
}
void CBattleUnit::Off()
{
	ACTOR->Off();
	m_PairStatus->Off();
	m_PairTurnUI->Off();
	m_PairTargetUI->TargetUIOff();

	if (true == m_bCharacter)
	{
		m_SelectEffect->Off();
	}

	if (true == m_bVibOn) 
	{
		m_bVibOn = false;
		m_fVibTime = 0.0f;
	}

	if (m_bBoss && nullptr != m_BossGauge)
		m_BossGauge->ActorOff();

	m_SelectSkillNo = -1;
}
void CBattleUnit::UnitOff()
{
	ACTOR->Off();

	if (true == m_bCharacter)
		m_SelectEffect->Off();
	

	if (true == m_bVibOn)
	{
		m_bVibOn = false;
		m_fVibTime = 0.0f;
	}

	if (m_bBoss && nullptr != m_BossGauge)
		m_BossGauge->ActorOff();

	m_SelectSkillNo = -1;
}
void CBattleUnit::SetPos(const Vec2& _Pos)
{
	TRANS->SetPos(_Pos);
	m_SelectEffect->SetPos(Vec3{ _Pos.x,_Pos.y,0.f });
	//m_PairTargetIcon->SetPos(_Pos + Vec2{ m_DemageFontPivot.x,m_DemageFontPivot.y });
}
void CBattleUnit::SetPos(const Vec3& _Pos)
{
	TRANS->SetPos(_Pos);
	m_SelectEffect->SetPos(_Pos);
	//m_PairTargetIcon->SetPos(_Pos + m_DemageFontPivot);
}
void CBattleUnit::SetIdle() 
{
	m_PairMgr->ChangeState(BATTLE_IDLE);
	
	if (false == m_bCharacter)
	{
		if (nullptr != m_AniRen) 
		{
			m_AniRen->On();
		}
		if (nullptr != m_MonReactionRen)
		{
			m_MonReactionRen->MaterialOff();
			m_MonReactionRen->Off();
		}
	}
	else
	{
		if (nullptr != m_AniRen)
			m_AniRen->MaterialOff();
	}

	m_CurUnitState = BATTLE_IDLE;
}
void CBattleUnit::SetAction() 
{
	m_PairMgr->ChangeState(BATTLE_ACTION);
	
	if (false == m_bCharacter)
	{
		if (nullptr != m_AniRen)
			m_AniRen->On();
		if (nullptr != m_MonReactionRen)
			m_MonReactionRen->Off();
	}

	m_CurUnitState = BATTLE_ACTION;
}
void CBattleUnit::SetReAction() 
{
	m_PairMgr->ChangeState(BATTLE_REACTION);
	
	if (false == m_bCharacter)
	{
		if (nullptr != m_AniRen)
			m_AniRen->Off();
		if (nullptr != m_MonReactionRen)
			m_MonReactionRen->On();
	}

	m_CurUnitState = BATTLE_REACTION;
}
void CBattleUnit::SetAppear()
{
	if (true == m_bCharacter) 
	{
		if (nullptr != m_AniRen)
			m_AniRen->On();

		m_CurUnitState = BATTLE_CHARAPPEAR;
		m_PairMgr->ChangeState(BATTLE_CHARAPPEAR);
	}
	else
	{
		if (nullptr != m_AniRen)
			m_AniRen->Off();
		if (nullptr != m_MonReactionRen)
			m_MonReactionRen->On();
		
		m_CurUnitState = BATTLE_MONAPPEAR;
		m_PairMgr->ChangeState(BATTLE_MONAPPEAR);
	}
}
void CBattleUnit::SetDying() 
{
	if (false == m_bCharacter)
		return;

	
	if (nullptr != m_AniRen)
		m_AniRen->On();
	if (nullptr != m_MonReactionRen)
		m_MonReactionRen->Off();

	m_PairMgr->ChangeState(BATTLE_DYING);
	m_CurUnitState = BATTLE_DYING;
}
void CBattleUnit::SetDead() 
{
	if (false == m_bCharacter)
	{
		if (nullptr != m_AniRen)
			m_AniRen->On();
		if (nullptr != m_MonReactionRen)
			m_MonReactionRen->Off();
		m_PairMgr->ChangeState(BATTLE_MONDEAD);
		m_CurUnitState = BATTLE_MONDEAD;
	}
	else 
	{
		if (nullptr != m_AniRen)
			m_AniRen->On();
		if (nullptr != m_MonReactionRen)
			m_MonReactionRen->Off();

		m_PairMgr->ChangeState(BATTLE_DEAD);
		m_CurUnitState = BATTLE_DEAD;
	}
}
void CBattleUnit::SetInitLevel()
{
	SetLevel((int)CClientGlobal::CharacterLine[m_CharacterLine][m_LineIndex].CharacterLv);
}
void CBattleUnit::EndReAction()
{
	if (BATTLE_REACTION == m_CurUnitState) 
	{
		m_bFilterEnd = true;
		m_fAccFilterTime = 0.0f;
		SetIdle();
	}
}
const UNITSTATE CBattleUnit::GetCurUnitState()
{
	return (UNITSTATE)m_PairMgr->GetCurStateIndex();
	//return m_CurUnitState;
}
// TurnMgr안의 Phase가 유닛의 상태를 조절

void CBattleUnit::SelectActionIndex(int _ActionIndex)
{
	m_iCurActionIndex = _ActionIndex;
}
int CBattleUnit::GetCutActionIndex()
{
	return m_iCurActionIndex;
}
// 외부에서 Unit의 상태를 조정,확인 
void CBattleUnit::EndAction(bool _EndAction /*= true*/)
{
	m_bEndAction = _EndAction;
}
void CBattleUnit::SelectAction(bool _SelectAction /*= true*/)
{
	m_bSelectAction = _SelectAction;
}
bool CBattleUnit::IsCharacter()
{
	return m_bCharacter;
}
const bool CBattleUnit::IsCurBattle()
{
	return m_bCurBattle;
}
void CBattleUnit::CurBattle(const bool& _Battle)
{
	m_bCurBattle = _Battle;
}
bool CBattleUnit::IsEndAction() 
{
	return m_bEndAction;
}
bool CBattleUnit::IsSelectAction() 
{
	return m_bSelectAction;
}
void CBattleUnit::SelectSkill(const size_t& _SkillNo)
{
	if (_SkillNo == -1)
		return;

	if (_SkillNo >= CClientGlobal::vecSkill.size())
		return;

	
	unordered_map<size_t, size_t>::iterator  Find = m_mapUnitSkillData.find(_SkillNo);

	if (Find == m_mapUnitSkillData.end())
		return;
		
	m_SelectSkillNo = _SkillNo;
	m_SelectSkillLevel = Find->second;
}
void CBattleUnit::SelectSkill(const size_t& _SkillNo, const size_t& _SkillLevel)
{
	if (_SkillNo == -1)
		return;

	if (_SkillNo >= CClientGlobal::vecSkill.size())
		return;

	if (_SkillLevel >= LEVELDATA::MAXSKILLLEVEL )
		return;

	m_SelectSkillNo = _SkillNo;
	m_SelectSkillLevel = _SkillLevel;
}
void CBattleUnit::SetSelectTarget(const size_t& _Target)
{
	return SetSelectTarget((TARGET)_Target);
}
void CBattleUnit::SetSelectTarget(const TARGET& _Target)
{
	if (_Target >= MAXTARGET)
		return;

	m_SelectTarget = _Target;
}
const size_t CBattleUnit::GetCurSelectSkill()
{
	if (m_SelectSkillNo >= CClientGlobal::vecSkill.size())
		return -1;

	unordered_map<size_t, size_t>::iterator Find = m_mapUnitSkillData.find(m_SelectSkillNo);

	if (Find == m_mapUnitSkillData.end())
		return -1;

	return Find->second;
}
const size_t CBattleUnit::GetSelectSkill()
{
	return m_SelectSkillNo;
}
const TARGET CBattleUnit::GetSelectTarget()
{
	return m_SelectTarget;
}
void CBattleUnit::SetTurnCount(const int& _TurnCount)
{
	m_iTurnCount = _TurnCount;
}
void CBattleUnit::SetCurExp(const int& _Exp)
{
	m_CurUnitStat.Exp = _Exp;
}
int CBattleUnit::GetTurnCount()
{
	return m_iTurnCount;
}
bool CBattleUnit::ActionCheck()
{
	// 인자로 스킬이 들어오면 스킬의 마나와 현재 Unit의 마나를 비교하여
	// 현재 Unit의 마나가 더 많으면 true
	return true;
}
int CBattleUnit::CalTurnCount(const int& _SkillTurnCount) 
{
	int TurnCount = _SkillTurnCount - m_iTurnCount;

	if (TurnCount <= 0)
		return 0;

	return TurnCount;
}
CActObject* CBattleUnit::CreateActObject(const tstring& _Name /*= _T("")*/)
{
	return CActObject::CreateActObject(_Name, ACTOR->GetScene());
}
void CBattleUnit::SelectEffectOn() 
{
	if (true == m_bCharacter) 
	{
		m_SelectEffect->SetPos(TRANS->GetPos());
		m_SelectEffect->On();
	}
}
void CBattleUnit::SelectEffectOff()
{
	if (true == m_bCharacter)
	{
		m_SelectEffect->Off();
	}
}
const Vec3 CBattleUnit::GetPos() 
{
	return TRANS->GetPos();
}
// Vec2 버젼 포스
const Vec2 CBattleUnit::GetPosVec2()
{
	return Vec2{ TRANS->GetPos().x,TRANS->GetPos().y };
}
// 실제 랜더링 되는 위치

void CBattleUnit::SetFilter(const D3DXCOLOR& _Color /*= D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f)*/)
{
	if (nullptr == m_AniRen)
		return;

	m_AniRen->SetMaterial(_Color);
	m_AniRen->MaterialOn();
	// 몬스터의 경우 리액션전용 랜더러에게도 필터적용
	if (nullptr != m_MonReactionRen && false == m_bCharacter)
	{
		m_MonReactionRen->SetMaterial(_Color);
		m_MonReactionRen->MaterialOn();
	}

}
void CBattleUnit::FilterOff()
{
	if( nullptr != m_AniRen )
		m_AniRen->MaterialOff();

	if (false == m_bCharacter && nullptr != m_MonReactionRen)
		m_MonReactionRen->MaterialOff();


	
}
void CBattleUnit::InitalizeCharacter()
{
	m_AniRen = ACTOR->CreateCom<CAniRenderer>(RENDATA(LAYER_PLAYER, m_fAlpha));
	
	m_SelectEffect = CreateActObject(_T("SelectEffect"))->CreateCom<CCharSelectEffect>(this);

	m_SelectEffect->Off();

	m_CharacterLine = CClientGlobal::FindCharacterLineNo((CharacterIndex)m_iCharIndex);
	m_LineIndex = CClientGlobal::FindLineIndex((CharacterIndex)m_iCharIndex);

	
	// 더블어택이 가능한 캐릭터는 해당 애니메이션을 적용
	if (Vargas >= m_iCharIndex)
	{
		m_AniRen->CreateAnimation(Ani_Atk1(m_UnitName.UnitName));
		m_AniRen->CreateAnimation(Ani_Atk2(m_UnitName.UnitName));
		m_AniRen->CreateAnimation(Ani_DAtk(m_UnitName.UnitName));
	}
	else
	{
		m_AniRen->CreateAnimation(Ani_Atk(m_UnitName.UnitName));
	}

	m_AniRen->CreateAnimation(Ani_Idle(m_UnitName.UnitName));
	m_AniRen->CreateAnimation(Ani_Dead(m_UnitName.UnitName));
	m_AniRen->CreateAnimation(Ani_Dying(m_UnitName.UnitName));
	m_AniRen->CreateAnimation(Ani_Move(m_UnitName.UnitName));
	m_AniRen->CreateAnimation(Ani_Limit(m_UnitName.UnitName));
	m_AniRen->CreateAnimation(Ani_Stand(m_UnitName.UnitName));
	m_AniRen->CreateAnimation(Ani_MAtk(m_UnitName.UnitName));
	m_AniRen->CreateAnimation(Ani_MStand(m_UnitName.UnitName));
	m_AniRen->CreateAnimation(Ani_Win(m_UnitName.UnitName));
	m_AniRen->CreateAnimation(Ani_WinBe(m_UnitName.UnitName));

	//m_AniRen->CreateAnimation(Ani_Jump(m_UnitName));

	m_PairMgr->CreateState<CCharIdle>(this);
	m_PairMgr->CreateState<CCharAction>(this);
	m_PairMgr->CreateState<CCharReAction>(this);
	m_PairMgr->CreateState<CCharDying>(this);
	m_PairMgr->CreateState<CCharDead>(this);
	m_PairMgr->CreateState<CCharWin>(this);	
	m_PairMgr->CreateState<CCharAppear>(this);
	m_PairMgr->ChangeState<CCharIdle>();

	ACTOR->SetImageScale(1.7f);
	
	m_bCharacter = true;

	m_pUnitStatTable = &(CClientGlobal::CharacterStatTable.find(m_UnitName.UnitName)->second);
	m_UnitPara = CClientGlobal::CharacterParameter.find(m_UnitName.UnitName)->second;

	if (nullptr == m_pUnitStatTable) 
	{
		assert(nullptr);
		return;
	}


	SPTR<CGameMultiSprite> IdleSprite = COMRESMGR->FindMultiSprite(Idle(m_UnitName.UnitName));
	if (nullptr != IdleSprite) 
	{
		Vec3 SpritePivot = Vec3{ IdleSprite->GetSpritePivot().x, IdleSprite->GetSpritePivot().y,0.0f };
		Vec3 SpriteSize = Vec3{ 0.0f,IdleSprite->GetSpriteSize().y,0.0f };

		m_UnitMiddlePivot = SpritePivot + SpriteSize * ACTOR->GetMagnification()*0.5f;
		m_DemageFontPivot = SpritePivot + SpriteSize * ACTOR->GetMagnification();
	}
	LoadUseSkill();

	
}
void CBattleUnit::LoadUseSkill()
{
	for (size_t i = 0; i < CClientGlobal::vecSkill.size(); i++)
	{
		if (CClientGlobal::vecSkill[i].GetUnitIndex() == m_iCharIndex && true == CClientGlobal::vecSkill[i].SkillData.CharacterSkill)
		{
			//int RandomLevel = CSJRandom::RandomInt(0, 4);
			//UNITSKILLDATA Data = UNITSKILLDATA(CClientGlobal::vecSkill[i].SkillNo, 0);
			//m_vecUnitSkillData.push_back(Data);
			m_mapUnitSkillData.insert(unordered_map<size_t, size_t>::value_type(CClientGlobal::vecSkill[i].SkillNo, CClientGlobal::vecSkill[i].CurSkillLevel));
		}
	}

}
void CBattleUnit::InitalizeMonster()
{

	m_AniRen = ACTOR->CreateCom<CAniRenderer>(RENDATA(LAYER_MONSTER, m_fAlpha));

	m_MonReactionRen = ACTOR->CreateCom<CFixRenderer>(RENDATA(LAYER_MONSTER, m_fAlpha));
	m_MonReactionRen->SetSprite(COMRESMGR->FindMultiSpriteToIndex(Atk(m_UnitName.UnitName), 0));
	m_MonReactionRen->Off();

	Vec2 size = m_MonReactionRen->GetCurSpriteSize();

	m_UnitCol = ACTOR->CreateCom<CCollision>(COLDATA(_T("UI")));
	// 유닛의 크기에 비례하여 충돌체 설정
	m_UnitCol->SetColSize(size*0.7f);
	// 유닛의 크기에 관계없이 크기 고정의 충돌체 설정
	m_UnitCol->SetColSize(Vec2{100.0f,100.0f });
	m_UnitCol->SetPivotPos( -m_MonReactionRen->GetCurSpritePivot());
	m_UnitCol->SetEnter(this, &CBattleUnit::Enter);
	m_UnitCol->SetStay(this, &CBattleUnit::Stay);
	m_UnitCol->SetExit(this, &CBattleUnit::Exit);

	m_SelectEffect = CreateActObject(_T("SelectEffect"))->CreateCom<CCharSelectEffect>(this);

	m_SelectEffect->Off();

	if (nullptr == m_AniRen || nullptr == m_PairMgr)
	{
		return;
	}

	m_AniRen->CreateAnimation(Ani_Idle(m_UnitName.UnitName));
	m_AniRen->CreateAnimation(Ani_Atk(m_UnitName.UnitName));

	m_AniRen->ChangeAni(Idle(m_UnitName.UnitName));

	m_PairMgr->CreateState<CMonIdle>(this);
	m_PairMgr->CreateState<CMonAction>(this);
	m_PairMgr->CreateState<CMonReAction>(this);
	m_PairMgr->CreateState<CMonDead>(this);
	m_PairMgr->CreateState<CMonAppear>(this);
	
	m_PairMgr->ChangeState<CMonIdle>();

	ACTOR->Off();
	ACTOR->SetImageScale(1.5f);

	SPTR<CGameMultiSprite> IdleSprite = COMRESMGR->FindMultiSprite(Idle(m_UnitName.UnitName));
	
	if (nullptr != IdleSprite)
	{
		Vec3 SpritePivot = Vec3{ IdleSprite->GetSpritePivot().x, IdleSprite->GetSpritePivot().y,0.0f };
		Vec3 SpriteSize = Vec3{ 0.0f,IdleSprite->GetSpriteSize().y,0.0f };

		m_UnitMiddlePivot = SpritePivot + SpriteSize * ACTOR->GetMagnification() * 0.5f;
		m_DemageFontPivot = SpritePivot + SpriteSize * ACTOR->GetMagnification() * 0.9f;
	}

	Vec2 CircleSize = Vec2{ IdleSprite->GetSpriteSize().x, IdleSprite->GetSpriteSize().x };
	m_PairTargetUI->SetSize( CircleSize * (ACTOR->GetMagnification() + 0.2f ));
	
	m_CurUnitStatus[LEVELDATA::SKILLCOST_HP] = 1000;
	m_CurUnitStatus[LEVELDATA::SKILLCOST_MP] = 1000;
}
int CBattleUnit::GetCurAniLooping()
{
	return m_AniRen->GetCurAniCount();
}
STAT CBattleUnit::SetLevel(const int& _Level) 
{
	if (_Level <= 0 || _Level > 100) 
	{
		return STAT();
	}
	
	m_iLevel = _Level;
	m_BaseStat = (*m_pUnitStatTable)[m_iLevel - 1];

	if (true == m_bCharacter)
	{
		m_BaseStat.Critical = m_UnitPara.Critical;
		m_BaseStat.CriticalDamage = m_UnitPara.CriticalDamage;
		m_BaseStat.Avoidability = m_UnitPara.Avoidability;
	}

	m_CurUnitStat = STAT::AddUnit_ItemStat(m_BaseStat, m_UnitPara, m_ItemStat);

	m_CurUnitStatus[LEVELDATA::SKILLCOST_HP] = m_CurUnitStat.Heath;
	m_CurUnitStatus[LEVELDATA::SKILLCOST_MP] = m_CurUnitStat.Mana;

	m_PairStatus->UpdateHp(m_CurUnitStat.Heath, m_CurUnitStatus[LEVELDATA::SKILLCOST_HP]);
	m_PairStatus->UpdateMp(m_CurUnitStat.Mana, m_CurUnitStatus[LEVELDATA::SKILLCOST_MP]);
	//m_CurUnitSkillCost[2] 버스트 게이지 BattleUnit의 Static 변수로 만들 예정
	
	m_bLevelUpdate = true;

	CClientGlobal::CharacterLine[m_CharacterLine][m_LineIndex].CharacterLv = _Level;
	CClientGlobal::CharacterLine[m_CharacterLine][m_LineIndex].CurHp = m_CurUnitStat.Heath;
	CClientGlobal::CharacterLine[m_CharacterLine][m_LineIndex].MaxHp = m_CurUnitStat.Heath;
	CClientGlobal::CharacterLine[m_CharacterLine][m_LineIndex].CurMp = m_CurUnitStat.Mana;
	CClientGlobal::CharacterLine[m_CharacterLine][m_LineIndex].MaxMp = m_CurUnitStat.Mana;
	CClientGlobal::CharacterLine[m_CharacterLine][m_LineIndex].CurExp = 0;
	CClientGlobal::CharacterLine[m_CharacterLine][m_LineIndex].MaxExp = CClientGlobal::ExpTable[_Level];
	return m_CurUnitStat;
}
STAT CBattleUnit::LevelUp(const int& _Up)
{
	return SetLevel(m_iLevel + _Up);
}
int	 CBattleUnit::GetLevel()
{
	return m_iLevel;
}
const tstring CBattleUnit::GetLevelString()
{
	TCHAR Level[8] = _T("");
	_stprintf_s(Level, _T("%d"), m_iLevel);
	tstring LevelString = Level;
	
	return LevelString;
}
bool CBattleUnit::IsUpdateLevel()
{
	return m_bLevelUpdate;
}
void CBattleUnit::UpdateLevel()
{
	m_bLevelUpdate = false;
}
STAT CBattleUnit::GetCurStat() 
{
	return m_CurUnitStat;
}
STATPARAMETERDATA CBattleUnit::GetStatParameter() 
{
	return m_UnitPara;
}
// 유닛의 색상을 단색 계통으로 바꿀때 사용하는 함수
bool CBattleUnit::SetFilter(const D3DXCOLOR& _Color, const float& _FilterTime, const int& _Count /*= 1*/)
{
	if (0 >= _FilterTime || 0 >= _Count) 
	{
		return false;
	}

	m_bFilterEnd = false;
	m_bFilterOn = true;
	m_fAccFilterTime = 0.0f;
	m_fFilterTime = _FilterTime;
	m_iFilterCount = _Count;

	if (true == m_bCharacter)
	{
		m_AniRen->SetMaterial(_Color);
		m_AniRen->MaterialOn();
	}
	else if (false == m_bCharacter && nullptr != m_MonReactionRen) 
	{
		m_MonReactionRen->SetMaterial(_Color);
		m_MonReactionRen->MaterialOn();
	}

	return true;
}
// 필터를 적용,해제 하는 함수
void CBattleUnit::FilterUpdate()
{
	if (true ==  m_bFilterEnd )
	{
		return;
	}

	if ( true == m_bFilterOn && m_fAccFilterTime >= m_fFilterTime)
	{
		if (0 >= --m_iFilterCount) 
		{
			m_bFilterEnd = true;
		}
		m_AniRen->MaterialOff();
		
		if (nullptr != m_MonReactionRen)
			m_MonReactionRen->MaterialOff();
		

		m_bFilterOn = false;
		m_fAccFilterTime = 0.0f;
		return;
	}
	else if(false == m_bFilterOn && m_fAccFilterTime >= m_fFilterTime)
	{
		m_AniRen->MaterialOn();
		
		if (nullptr != m_MonReactionRen)
			m_MonReactionRen->MaterialOn();

		m_bFilterOn = true;
		m_fAccFilterTime = 0.0f;
		return;
	}

	m_fAccFilterTime += DELTATIME;
}
bool CBattleUnit::ReActionEvent(const int& _ReactionMode)
{
	return ReActionEvent((REACTION)_ReactionMode);
}
bool CBattleUnit::ReActionEvent(const REACTION& _ReactionMode)
{
	bool Setting = false;

	if (_ReactionMode < REACTION_DEFAULT || _ReactionMode >= MAXREACTION) 
	{
		return Setting;
	}

	switch (_ReactionMode)
	{
	case REACTION_DEFAULT:
	{
		SetFilter(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), 0.08f);
		Setting = true;
	}
	break;
	case REACTION_VIBRATION:
	{
		//SetFilter(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), 0.2f);
		SetVib(0.5f);
		Setting = true;
	}
	break;
	case REACTION_RED_VIB:
	{
		SetFilter(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), 0.08f);
		SetVib(0.5f);
		Setting = true;
	}
	break;
	case REACTION_COLD:
	{
		SetFilter(D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f), LARGEFLOAT);
		Setting = true;
	}
	break;
	}
	return Setting;
}
void CBattleUnit::SetVib(const float& _Time /*= LARGEFLOAT*/)
{
	m_fAccVibTime = 0.0f;
	m_fVibTime = _Time;
	m_bVibOn = true;
}
void CBattleUnit::VibrationUpdate() 
{
	if (false == m_bVibOn) 
	{
		return;
	}

	if (m_fAccVibTime >= m_fVibTime) 
	{
		m_bVibOn = false;
		m_fAccVibTime = 0.0f;
		m_AniRen->SetRenPivot(Vec2{ 0.0f,0.0f });
	
		if (nullptr != m_MonReactionRen)
			m_MonReactionRen->SetRenPivot(Vec2{ 0.0f,0.0f });
	
		return;
	}

	float Vib = CSJRandom::RandomFloat(-10.0f, 10.0f);
	m_AniRen->SetRenPivot(Vec2{ Vib,0.0f });

	if (nullptr != m_MonReactionRen)
		m_MonReactionRen->SetRenPivot(Vec2{ Vib,0.0f });
	

	m_fAccVibTime += DELTATIME;
	return;
}

void CBattleUnit::SetRenPivot(const Vec2& _Pivot)
{
	m_AniRen->SetRenPivot(_Pivot);
	
	if (nullptr != m_MonReactionRen)
		m_MonReactionRen->SetRenPivot(_Pivot);
	
}
void CBattleUnit::SetAlpha()
{
	if (nullptr == m_AniRen) 
	{
		return;
	}

	m_AniRen->SetAlpha(m_fAlpha);

	if ( nullptr != m_MonReactionRen) 
		m_MonReactionRen->SetAlpha(m_fAlpha);
	
}
void CBattleUnit::SetAlpha(const float& _AlphaRatio)
{
	if (_AlphaRatio < 0.0f) 
	{
		m_AniRen->SetAlpha(0.0f);
	}

	m_AniRen->SetAlpha(m_fAlpha* _AlphaRatio);

	if (false == m_bCharacter && nullptr != m_MonReactionRen)
		m_MonReactionRen->SetAlpha(m_fAlpha*_AlphaRatio);
	
	//if (m_bBoss && nullptr != m_BossGauge)
	//	m_BossGauge->SetAlpha(_AlphaRatio);
}
void CBattleUnit::SetTeamNumber(const int& _TeamIndex)
{
	if (_TeamIndex < 0 || _TeamIndex >= MAXTEAM) 
	{
		return;
	}

	m_iTeamNumber = _TeamIndex;
}
int CBattleUnit::GetTeamNumber()
{
	return m_iTeamNumber;
}
bool CBattleUnit::PlaySound(const tstring& _SoundName)
{
	return m_UnitSound->PlaySound(_SoundName);
}
void CBattleUnit::InitSelectSkillData()
{
	m_SelectSkillNo = -1;
	m_SelectSkillLevel = -1;
	m_SelectTarget = MAXTARGET;
	m_bSelectAction = false;
	ClearCurTarget();
}
const float CBattleUnit::GetCriticalMag()
{
	return (float)m_CurUnitStat.CriticalDamage / (float)100;
}
void CBattleUnit::AniRenOn()
{
	if (nullptr != m_AniRen)
		m_AniRen->On();
}
void CBattleUnit::ReactionRenOn() 
{
	if (false == m_bCharacter && nullptr == m_MonReactionRen)
		m_MonReactionRen->On();
}
void CBattleUnit::AniRenOff() 
{
	if (nullptr != m_AniRen)
		m_AniRen->Off();
}
void CBattleUnit::ReactionRenOff()
{
	if (false == m_bCharacter && nullptr == m_MonReactionRen)
		m_MonReactionRen->Off();

}
// State Change 되기 전에 한번 수행되는 함수
void CBattleUnit::ChangeStateEvent()
{
	if (nullptr != m_AniRen)
		m_AniRen->MaterialOff();

	if (nullptr != m_MonReactionRen)
		m_MonReactionRen->MaterialOff();

	m_fAccVibTime = 0.0f;
	m_fAccFilterTime = 0.0f;
	m_bVibOn = false;
	m_bFilterEnd = true;
	m_bFilterOn = false;
}
void CBattleUnit::ClearCurTarget() 
{
	m_CurTargetUnit.clear();
}
bool CBattleUnit::SetCurTarget(const SPTR<CBattleUnit> _TargetUnit)
{
	if (nullptr == _TargetUnit) 
	{
		return false;
	}
	
	m_CurTargetUnit.push_back(_TargetUnit);
	return true;
}
list<SPTR<CBattleUnit>>* CBattleUnit::GetCurTargetUnit()
{
	return &m_CurTargetUnit;
}
// 실제 랜더링 위치 + (UnitSprite* ACTORIMAGEMAG)*0.5f = UnitMiddlePos
const Vec3 CBattleUnit::GetUnitMiddlePos() 
{
	return GetPos() + m_UnitMiddlePivot;
}
// 실제 랜더링 위치 + (UnitSprite* ACTORIMAGEMAG)*0.9f = UnitMiddlePos
const Vec3 CBattleUnit::GetUnitDemageFontPos() 
{
	return GetPos() + m_DemageFontPivot;
}
const STAT CBattleUnit::GetCurUnitStat()
{
	return m_CurUnitStat;
}
bool CBattleUnit::CheckSkillAvailable(const LEVELDATA& _Data)
{
	for (size_t i = 0; i < LEVELDATA::MAXSKILLCOST; i++)
	{
		if (m_CurUnitStatus[i] < _Data.SkillCost[i])
			return false;
	}

	return true;
}
void CBattleUnit::CalSkillCost(const LEVELDATA& _Data)
{
	for (size_t i = 0; i < LEVELDATA::MAXSKILLCOST; i++)
	{
		m_CurUnitStatus[i] = max(m_CurUnitStatus[i] - _Data.SkillCost[i], 0);
	}
	
	GetPairStatusBar()->UpdateHp(m_CurUnitStatus[LEVELDATA::SKILLCOST_HP]);
	GetPairStatusBar()->UpdateMp(m_CurUnitStatus[LEVELDATA::SKILLCOST_MP]);
}
const size_t CBattleUnit::GetTotalSkillCount() 
{
	return m_mapUnitSkillData.size();
}
void CBattleUnit::Enter(SPTR<CCollision> _pThis, SPTR<CCollision> _pOtherCol)
{
	if (nullptr != pUnitTurnMgr && true == pUnitTurnMgr->IsSelectSkill())
	{
		pUnitTurnMgr->CheckSkillTarget(m_bCharacter, (size_t)m_iTeamNumber);
	}

	m_PairTargetUI->SetPos(TRANS->GetPos());
}
void CBattleUnit::Stay(SPTR<CCollision> _pThis, SPTR<CCollision> _pOtherCol) 
{
	if (true == KEYDOWN("LMouse"))
	{
		pUnitTurnMgr->TriggerSkill();
	}
}
void CBattleUnit::Exit(SPTR<CCollision> _pThis, SPTR<CCollision> _pOtherCol) 
{
	if (nullptr != pUnitTurnMgr && true == pUnitTurnMgr->IsSelectSkill())
	{
		pUnitTurnMgr->CurTargetUIOff();
	}

	m_PairTargetUI->TargetUIOff();
}
void CBattleUnit::CollisionComOff() 
{
	if (nullptr != m_UnitCol)
		m_UnitCol->Off();
}
void CBattleUnit::CollisionComOn() 
{
	if(nullptr != m_UnitCol)
		m_UnitCol->On();
}
const Vec2 CBattleUnit::GetUnitFrontPos()
{
	float HalfSizeX = m_AniRen->GetCurAniSpriteSize().x* ACTOR->GetMagnification()*0.5f;

	return Vec2{ TRANS->GetPos().x + HalfSizeX,TRANS->GetPos().y };
}
const Vec2 CBattleUnit::GetUnitBackPos()
{
	float HalfSizeX = m_AniRen->GetCurAniSpriteSize().x* ACTOR->GetMagnification()*0.5f;

	return Vec2{ TRANS->GetPos().x - HalfSizeX,TRANS->GetPos().y };
}
void CBattleUnit::TargetUnitEndReaction()
{
	list<SPTR<CBattleUnit>>::iterator Start = m_CurTargetUnit.begin();
	list<SPTR<CBattleUnit>>::iterator End = m_CurTargetUnit.end();

	for (; Start != End; ++Start)
	{
		if (nullptr != (*Start) && false == (*Start)->isDead())
			(*Start)->EndReAction();
	}
	
	
}
void CBattleUnit::LastAttackEvent()
{
	if (nullptr == m_PairUnitFont)
		return;

	UINT Lv = pUnitTurnMgr->GetCurSkillLv() + 1 ;
	UINT MaxMana = m_CurUnitStat.Mana;
	UINT CurMana = m_CurUnitStatus[LEVELDATA::SKILLCOST_MP];
	UINT RecoveryMana = (UINT)(m_UnitPara.MpPara*2*Lv );

	m_CurUnitStatus[LEVELDATA::SKILLCOST_MP] = min(MaxMana, CurMana + RecoveryMana);
	
	if (CurMana + RecoveryMana >= MaxMana) 
		RecoveryMana = MaxMana - CurMana;
	
	m_PairStatus->UpdateMp(m_CurUnitStatus[LEVELDATA::SKILLCOST_MP]);
	
	TCHAR Str[MAXKEYLENGTH] = _T("");
	_stprintf_s(Str, _T("MP +%3d"), RecoveryMana);
	
	m_PairUnitFont->FontOn(TRANS->GetPos());
	m_PairUnitFont->SetFontColor(D3DCOLOR_ARGB(255, 0, 120, 200));
	m_PairUnitFont->SetFontString(Str);

	
}
void CBattleUnit::DemagedEvent(const UINT& _Demage)
{
	if (-1 == _Demage)
		return;

	if (true == m_bCharacter && true == CClientGlobal::MasterMode)
		return;


	int CurHp = m_CurUnitStatus[LEVELDATA::SKILLCOST_HP];

	m_CurUnitStatus[LEVELDATA::SKILLCOST_HP] = max(CurHp - (int)_Demage, 0);

	m_PairStatus->UpdateHp(m_CurUnitStatus[LEVELDATA::SKILLCOST_HP]);
	if (nullptr != m_BossGauge)
		m_BossGauge->UpdateHp(m_CurUnitStatus[LEVELDATA::SKILLCOST_HP]);

	if (0 >= m_CurUnitStatus[LEVELDATA::SKILLCOST_HP]) 
		m_bDead = true;
	

}
const UINT CBattleUnit::GetCurHp()
{
	return m_CurUnitStatus[LEVELDATA::SKILLCOST_HP];
}
const UINT CBattleUnit::GetCurMp()
{
	return m_CurUnitStatus[LEVELDATA::SKILLCOST_MP];
}
const UINT CBattleUnit::GetMaxHp()
{
	return m_CurUnitStat.Heath;
}
const UINT CBattleUnit::GetMaxMp()
{
	return m_CurUnitStat.Mana;
}
void CBattleUnit::StandBy()
{
	m_bDead = false;
	m_bCurBattle = true;
	m_bCalExp = false;

	if (true == m_bCharacter)
		StandByCharacter();
	else
		StandByMonster();

	SetIdle();
	InitSelectSkillData();
}
void CBattleUnit::StandByCharacter()
{
	// 맵씬에서 마지막으로 업데이트된 스킬레벨을 자신의 스킬 레벨 저장소에
	// 업데이트
	unordered_map<size_t, size_t>::iterator Start = m_mapUnitSkillData.begin();
	unordered_map<size_t, size_t>::iterator End = m_mapUnitSkillData.end();
	
	for (; Start != End; ++Start)
		Start->second = CClientGlobal::vecSkill[Start->first].CurSkillLevel;

	m_ItemStat = STAT();

	CHARACTERDATA CurData = CClientGlobal::GetCurCharData((CharacterIndex)m_iCharIndex);

	for (size_t i = 0; i < MAXITEMEQUIP; i++) 
	{
		size_t ItemNo = CurData.EquipItem[i];
		
		if (ItemNo >= CClientGlobal::vecItem.size())
			continue;

		m_ItemStat += CClientGlobal::vecItem[CurData.EquipItem[i]].ItemStat;
	}

	m_BaseStat = (*m_pUnitStatTable)[CurData.CharacterLv -1];

	m_BaseStat.Critical = m_UnitPara.Critical;
	m_BaseStat.CriticalDamage = m_UnitPara.CriticalDamage;
	m_BaseStat.Avoidability = m_UnitPara.Avoidability;

	m_CurUnitStat =  STAT::AddUnit_ItemStat( m_BaseStat ,m_UnitPara, m_ItemStat);


	m_CurUnitStatus[LEVELDATA::SKILLCOST_HP] = m_CurUnitStat.Heath;
	m_CurUnitStatus[LEVELDATA::SKILLCOST_MP] = m_CurUnitStat.Mana;
	m_CurUnitStat.Exp = (UINT)CurData.CurExp;

	m_PairStatus->UpdateHp(m_CurUnitStat.Heath, m_CurUnitStatus[LEVELDATA::SKILLCOST_HP]);
	m_PairStatus->UpdateMp(m_CurUnitStat.Mana, m_CurUnitStatus[LEVELDATA::SKILLCOST_MP]);
}
void CBattleUnit::StandByMonster()
{
	m_AniRen->On();
	m_AniRen->ChangeAni(Idle(m_UnitName.UnitName));
	m_AniRen->SetAlpha(m_fAlpha);

	if (nullptr != m_MonReactionRen) 
	{
		m_MonReactionRen->Off();
		m_MonReactionRen->SetAlpha(m_fAlpha);
	}

	m_CurUnitStat.Heath = 10;
	m_CurUnitStatus[LEVELDATA::SKILLCOST_HP] = 10;

	m_CurUnitStat.Heath = m_BaseStat.Heath;
	m_CurUnitStatus[LEVELDATA::SKILLCOST_HP] = m_CurUnitStat.Heath;

	if (nullptr != m_BossGauge) 
	{
		m_BossGauge->StandBy();
		m_BossGauge->ActorOn();
	}
}
void CBattleUnit::SyncroDebugLine()
{
	Vec3 StartPos = TRANS->GetPos() + Vec3{ -100.0f,0.0f,0.0f };
	Vec3 EndPos = StartPos + Vec3{ 200.0f,0.0f,0.0f };

	if (nullptr != m_UnitPosRen)
		m_UnitPosRen->SettingLineData(StartPos, EndPos);
}
void CBattleUnit::UnitPosLineOn()
{
	if (nullptr != m_UnitPosRen)
		m_UnitPosRen->On();
}
void CBattleUnit::UnitPosLineOff()
{
	if (nullptr != m_UnitPosRen)
		m_UnitPosRen->Off();
}
void CBattleUnit::ReverseLineRen()
{
	if(nullptr != m_UnitPosRen)
		m_UnitPosRen->ReverseRendering();
}
void CBattleUnit::InitAppear()
{
	m_bFilterEnd = true;

	m_AniRen->MaterialOff();

	if (nullptr != m_MonReactionRen)
		m_MonReactionRen->MaterialOff();
	
	m_bFilterOn = false;
	m_fAccFilterTime = 0.0f;


	m_bVibOn = false;
	m_fAccVibTime = 0.0f;
	m_AniRen->SetRenPivot(Vec2{ 0.0f,0.0f });

	if (nullptr != m_MonReactionRen)
		m_MonReactionRen->SetRenPivot(Vec2{ 0.0f,0.0f });

	if (m_bBoss && nullptr != m_BossGauge) 
	{
		m_BossGauge->ActorOn();
		m_BossGauge->FadeEvent(true);
		//m_BossGauge->SetAlpha(0.f);
	}
}
void CBattleUnit::TargetOverride(const bool _Override)
{
	m_bTargetOverride = _Override;
}
const bool CBattleUnit::IsTargetOverrideByChangeRound()
{
	return m_bTargetOverride;
}
void CBattleUnit::SettingMonsterData()
{
	unordered_map<tstring,MONSTERDATA>::iterator Find =  CClientGlobal::MonsterDataMap.find(m_UnitName.UnitName);

	if (Find == CClientGlobal::MonsterDataMap.end())
		return;

	m_BaseStat = Find->second.MonsterStat;
	m_CurUnitStat = m_BaseStat;

	m_CurUnitStatus[LEVELDATA::SKILLCOST_HP] = m_CurUnitStat.Heath;
	m_CurUnitStatus[LEVELDATA::SKILLCOST_MP] = m_CurUnitStat.Mana;

	for (size_t i = 0; i < Find->second.MonsterSkillCount; i++)
	{
		if(-1 != Find->second.MonsterSkill[i])
			m_vecMonsterSkillNo.push_back(Find->second.MonsterSkill[i]);
	}

	if (_T("") != Find->second.MonsterKName)
		m_PairStatus->ChangeName(Find->second.MonsterKName);

	if (Find->second.bBoss) 
	{
		m_BossGauge = CActObject::CreateActObject(_T("BossGauge"), ACTOR->GetScene())->CreateCom<CBossGauge>();
		m_BossGauge->SetPairUnit(this);
		m_BossGauge->ActorOff();
		m_bBoss = true;
	}

	m_UnitName = UNITNAME(Find->second.MonsterName, Find->second.MonsterKName,_T(""));
	
}
const UINT CBattleUnit::GetRandomSkill()
{
	if (0 >= m_vecMonsterSkillNo.size())
		return -1;

	if(1 < m_vecMonsterSkillNo.size())
		CSJRandom::ShuffleVector(m_vecMonsterSkillNo);

	return m_vecMonsterSkillNo.front();
}
const size_t CBattleUnit::CurRandomSkillNo() 
{
	return m_SelectSkillNo;
}
const size_t CBattleUnit::CurRandomSkillLv()
{
	return m_SelectSkillLevel;
}
const UINT CBattleUnit::CurRandomSkillCount()
{
	return CClientGlobal::vecSkillLevelData[m_SelectSkillNo][m_SelectSkillLevel].SkillCount;
}
const bool CBattleUnit::CompareDex(SPTR<CBattleUnit> _Other)
{
	if (nullptr == _Other)
	{
		assert(nullptr);
		return false;
	}

	return m_CurUnitStat.Dex > _Other->GetCurStat().Dex;
}

const UINT CBattleUnit::GetExp()
{
	if (true == m_bCharacter)
		return 0;

	if (true == m_bDead && false == m_bCalExp)
	{	
		m_bCalExp = true;
		return m_CurUnitStat.Exp;
	}

	return 0;
}
// 레벨업을 한경우 출력값으로 true를 반환
const UINT CBattleUnit::GetExpEvent(const UINT& _Exp)
{
	if (0 >= _Exp)
		return 0;


	// 캐릭터의 경우 STAT안의 Exp는 현재 경험치량을 나타낸다.
	TCHAR Str[MAXKEYLENGTH] = _T("");
	_stprintf_s(Str, _T("Exp +%6d"), _Exp);

	m_PairUnitFont->FontOn(TRANS->GetPos());
	m_PairUnitFont->SetFontColor(D3DCOLOR_ARGB(255, 220, 120,10));
	m_PairUnitFont->SetFontString(Str);

	if (m_iLevel == MAXLEVEL)
		return 0;

	
	UINT AccExp = m_CurUnitStat.Exp + _Exp;
	UINT LevelUpExp = CClientGlobal::ExpTable[m_iLevel];
	UINT UpLevel = 0;


	do
	{
		if (AccExp <= LevelUpExp) 
			break;

		AccExp = AccExp - LevelUpExp;
		++UpLevel;
		
		if (UpLevel + m_iLevel >= MAXLEVEL) 
		{
			m_CurUnitStat.Exp = 0;
			CClientGlobal::CharacterLine[m_CharacterLine][m_LineIndex].CurExp = 0;
			SetLevel(MAXLEVEL);
			return UpLevel;
		}

		LevelUpExp = CClientGlobal::ExpTable[m_iLevel + UpLevel];
	}while(true);

	if (0 != UpLevel)
		SetLevel(min(UpLevel + m_iLevel,MAXLEVEL));

	m_CurUnitStat.Exp = AccExp;
	CClientGlobal::CharacterLine[m_CharacterLine][m_LineIndex].CurExp = AccExp;

	return UpLevel;
}
const UINT CBattleUnit::GetCurUnitExp()
{
	return m_CurUnitStat.Exp;
}
void CBattleUnit::PairUIOff()
{
	if(nullptr != m_PairTurnUI)
		m_PairTurnUI->Off();
	if (nullptr != m_PairStatus)
	m_PairStatus->Off();
}
void CBattleUnit::CharacterDataUpdate(const bool& _bWin)
{
	if (m_CharacterLine >= CClientGlobal::CharacterLine.size() ||
		m_LineIndex >= CClientGlobal::CharacterLine[m_CharacterLine].size()) 
		return;
	

	if (true == _bWin) 
	{
		CClientGlobal::CharacterLine[m_CharacterLine][m_LineIndex].CurHp = m_CurUnitStatus[LEVELDATA::SKILLCOST_HP];
		CClientGlobal::CharacterLine[m_CharacterLine][m_LineIndex].CurMp = m_CurUnitStatus[LEVELDATA::SKILLCOST_MP];
	}
	else 
	{
		CClientGlobal::CharacterLine[m_CharacterLine][m_LineIndex].CurHp = 10;
		CClientGlobal::CharacterLine[m_CharacterLine][m_LineIndex].CurMp = 0;
	}
		
	CClientGlobal::CharacterLine[m_CharacterLine][m_LineIndex].CurExp = m_CurUnitStat.Exp;
}