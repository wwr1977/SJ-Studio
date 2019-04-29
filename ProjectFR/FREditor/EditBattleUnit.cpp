#include "stdafx.h"
#include "EditBattleUnit.h"
#include "SkillMgr.h"
#include <GameMultiSprite.h>
#include <GameSprite.h>
#include <StateMgr.h>
#include "EditCharIdle.h"
#include "EditCharAction.h"
#include "EditCharReAction.h"
#include "EditMonIdle.h"
#include "EditMonAction.h"
#include "EditMonReAction.h"


D3DXCOLOR CEditBattleUnit::FocusMonBackColor = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
CSkillMgr* CEditBattleUnit::pUnitSkillMgr = nullptr;

CEditBattleUnit::CEditBattleUnit()
	: m_bSelectAction(false), m_bEndAction(false)
	, m_iTurnCount(1), m_iCurActionIndex(-1), m_iCharIndex(MAXBATTLECHARACTER)
	, m_bCharacter(false), m_iLevel(1)
	, m_pUnitStatTable(nullptr), m_UnitPara(STATPARAMETERDATA())
	, m_CurUnitStat(STAT())
	, m_fFilterTime(0.1f), m_bFilterEnd(false), m_fAccFilterTime(0.0f)
	, m_iFilterCount(0), m_bFilterOn(false), m_bLevelUpdate(false)
	, m_fVibTime(0.0f), m_fAccVibTime(0.0f), m_bVibOn(false)
	, m_UnitSound(nullptr), m_fAlpha(0.9f), m_iTeamNumber(0)
	, m_MonReactionRen(nullptr), m_UnitMiddlePivot(Vec3(0.0f, 0.0f, 0.0f))
	, m_DemageFontPivot(Vec3(0.0f, 0.0f, 0.0f)), m_CurUnitState(BATTLE_IDLE)
	, m_UnitCol(nullptr)
	, m_bDead(false), m_SelectSkillNo(-1), m_SelectSkillLevel(-1)
	, m_SelectTarget(MAXTARGET)
{
	m_CurTargetUnit.clear();
}


CEditBattleUnit::~CEditBattleUnit()
{
	m_CurTargetUnit.clear();
}
void CEditBattleUnit::SetTurnMgr(CSkillMgr* _SkillMgr)
{
	if (nullptr == _SkillMgr)
		return;

	pUnitSkillMgr = _SkillMgr;
}
void CEditBattleUnit::InitData(const tstring& _UnitName)
{
	int CharIndex = 0;
	m_UnitSound = ACTOR->CreateCom<CSoundPlayer>();

	// 만일 캐릭터의 이름이 들어왔다면 이 배틀유닛을 캐릭터로 초기화

	for (; CharIndex < MAXBATTLECHARACTER; ++CharIndex)
	{
		if (_UnitName == CEditGlobal::CharacterName[CharIndex].GetString())
		{
			m_UnitName = _UnitName;
			m_iCharIndex = CharIndex;
			InitalizeCharacter();
			return;
		}
	}

	// 유닛이름으로 몬스터가 들어왔다면 글로벌에서 적용가능한지 여부를 파악한뒤
	// 몬스터로 초기화

	if (false == CEditGlobal::IsMonsterName(_UnitName))
	{
		return;
	}

	m_UnitName = _UnitName;

	InitalizeMonster();

}
void CEditBattleUnit::InitData(CharacterIndex _UnitIndex)
{
	m_UnitSound = ACTOR->CreateCom<CSoundPlayer>();

	if (MAXBATTLECHARACTER <= _UnitIndex)
	{
		assert(nullptr);
		return;
	}


	m_UnitName = CEditGlobal::CharacterName[_UnitIndex];
	m_iCharIndex = _UnitIndex;

	InitalizeCharacter();
}
void CEditBattleUnit::SetDead(const bool& _Dead/* = true*/)
{
	m_bDead = _Dead;
}
bool CEditBattleUnit::isDead()
{
	return m_bDead;
}
void CEditBattleUnit::Init()
{
	m_PairMgr = new CStateMgr();

}
void CEditBattleUnit::Update()
{
	m_PairMgr->Update();
	FilterUpdate();
	VibrationUpdate();
}
bool CEditBattleUnit::ChangeAni(const ANIMATIONNAME& _AniIndex)
{
	if (false == m_bCharacter)
	{
		return false;
	}

	if (_AniIndex >= MAXANINAME || _AniIndex < 0)
	{
		return false;
	}

	m_AniRen->ChangeAni(GetAnimationName(_AniIndex, m_UnitName));
	return  true;
}
bool CEditBattleUnit::ChangeAni(const int& _Index)
{
	return ChangeAni((ANIMATIONNAME)_Index);
}
bool CEditBattleUnit::ChangeAni(const tstring& _AniKey)
{
	return m_AniRen->ChangeAni(_AniKey);
}

int	CEditBattleUnit::GetCharIndex()
{
	return (int)m_iCharIndex;
}
tstring CEditBattleUnit::GetUnitName()
{
	return m_UnitName;
}
void CEditBattleUnit::ChangeState(int _Index)
{
	ChangeStateEvent();
	m_PairMgr->ChangeState(_Index);
}
int CEditBattleUnit::GetCurAniFrame()
{
	return m_AniRen->GetCurAniFrame();
}
bool CEditBattleUnit::IsCurAniEnd()
{
	return m_AniRen->IsEndAni();
}

SPTR<CSoundPlayer> CEditBattleUnit::GetUnitSoundPlayer()
{
	return m_UnitSound;
}
unordered_map<size_t, size_t>* CEditBattleUnit::GetUnitSkillDataMap()
{
	return &m_mapUnitSkillData;
}
const size_t CEditBattleUnit::GetSkillLevel(const size_t& _SkillNo)
{
	unordered_map<size_t, size_t>::iterator Find = m_mapUnitSkillData.find(_SkillNo);

	if (Find == m_mapUnitSkillData.end())
		return -1;

	return Find->second;
}


void CEditBattleUnit::On()
{
	ACTOR->On();
}
void CEditBattleUnit::On(const Vec3& _SelectPos)
{
	ACTOR->On();
}
void CEditBattleUnit::Off()
{
	ACTOR->Off();
	
	if (true == m_bVibOn)
	{
		m_bVibOn = false;
		m_fVibTime = 0.0f;
	}

	m_SelectSkillNo = -1;
}

void CEditBattleUnit::SetPos(const Vec2& _Pos)
{
	TRANS->SetPos(_Pos);

}
void CEditBattleUnit::SetPos(const Vec3& _Pos)
{
	TRANS->SetPos(_Pos);
	}
void CEditBattleUnit::SetIdle()
{
	m_PairMgr->ChangeState(BATTLE_IDLE);


	if (false == m_bCharacter)
	{
		if (nullptr != m_AniRen) 
			m_AniRen->On();
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
void CEditBattleUnit::SetAction()
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
void CEditBattleUnit::SetReAction()
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
void CEditBattleUnit::EndReAction()
{
	if (BATTLE_REACTION == m_CurUnitState) 
	{
		if(true == m_bCharacter)
			m_AniRen->MaterialOff();
		else if (false == m_bCharacter && nullptr != m_MonReactionRen)
			m_MonReactionRen->MaterialOff();

		m_bFilterEnd = true;
		SetIdle();
	}
}
const UNITSTATE CEditBattleUnit::GetCurUnitState()
{
	return m_CurUnitState;
}
// TurnMgr안의 Phase가 유닛의 상태를 조절

void CEditBattleUnit::SelectActionIndex(int _ActionIndex)
{
	m_iCurActionIndex = _ActionIndex;
}
int CEditBattleUnit::GetCutActionIndex()
{
	return m_iCurActionIndex;
}
// 외부에서 Unit의 상태를 조정,확인 
void CEditBattleUnit::EndAction(bool _EndAction /*= true*/)
{
	m_bEndAction = _EndAction;
}
void CEditBattleUnit::SelectAction(bool _SelectAction /*= true*/)
{
	m_bSelectAction = _SelectAction;
}
bool CEditBattleUnit::IsCharacter()
{
	return m_bCharacter;
}
bool CEditBattleUnit::IsEndAction()
{
	return m_bEndAction;
}
bool CEditBattleUnit::IsSelectAction()
{
	return m_bSelectAction;
}
void CEditBattleUnit::SelectSkill(const size_t& _SkillNo)
{
	if (_SkillNo == -1)
		return;

	if (_SkillNo >= CEditGlobal::vecSkill.size())
		return;


	unordered_map<size_t, size_t>::iterator  Find = m_mapUnitSkillData.find(_SkillNo);

	if (Find == m_mapUnitSkillData.end())
		return;

	m_SelectSkillNo = _SkillNo;
	m_SelectSkillLevel = Find->second;
}
void CEditBattleUnit::SetSelectTarget(const size_t& _Target)
{
	return SetSelectTarget((TARGET)_Target);
}
void CEditBattleUnit::SetSelectTarget(const TARGET& _Target)
{
	if (_Target >= MAXTARGET)
		return;

	m_SelectTarget = _Target;
}
const size_t CEditBattleUnit::GetCurSelectSkill()
{
	if (m_SelectSkillNo >= CEditGlobal::vecSkill.size())
		return -1;

	unordered_map<size_t, size_t>::iterator Find = m_mapUnitSkillData.find(m_SelectSkillNo);

	if (Find == m_mapUnitSkillData.end())
		return -1;

	return Find->second;
}
const size_t CEditBattleUnit::GetSelectSkill()
{
	return m_SelectSkillNo;
}
const TARGET CEditBattleUnit::GetSelectTarget()
{
	return m_SelectTarget;
}
void CEditBattleUnit::SetTurnCount(const int& _TurnCount)
{
	m_iTurnCount = _TurnCount;
}
int CEditBattleUnit::GetTurnCount()
{
	return m_iTurnCount;
}
bool CEditBattleUnit::ActionCheck()
{
	// 인자로 스킬이 들어오면 스킬의 마나와 현재 Unit의 마나를 비교하여
	// 현재 Unit의 마나가 더 많으면 true
	return true;
}
int CEditBattleUnit::CalTurnCount(const int& _SkillTurnCount)
{
	int TurnCount = _SkillTurnCount - m_iTurnCount;

	if (TurnCount <= 0)
		return 0;

	return TurnCount;
}


const Vec3 CEditBattleUnit::GetPos()
{
	return TRANS->GetPos();
}
// Vec2 버젼 포스
const Vec2 CEditBattleUnit::GetPosVec2()
{
	return Vec2{ TRANS->GetPos().x,TRANS->GetPos().y };
}
// 실제 랜더링 되는 위치

void CEditBattleUnit::SetFilter(const D3DXCOLOR& _Color /*= D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f)*/)
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
void CEditBattleUnit::FilterOff()
{
	if (nullptr != m_AniRen)
		m_AniRen->MaterialOff();

	if (false == m_bCharacter && nullptr != m_MonReactionRen)
		m_MonReactionRen->MaterialOff();

}
void CEditBattleUnit::InitalizeCharacter()
{
	m_AniRen = ACTOR->CreateCom<CAniRenderer>(RENDATA(LAYER_PLAYER, m_fAlpha));


	// 더블어택이 가능한 캐릭터는 해당 애니메이션을 적용
	if (Vargas >= m_iCharIndex)
	{
		m_AniRen->CreateAnimation(Ani_Atk1(m_UnitName));
		m_AniRen->CreateAnimation(Ani_Atk2(m_UnitName));
		m_AniRen->CreateAnimation(Ani_DAtk(m_UnitName));
	}
	else
	{
		m_AniRen->CreateAnimation(Ani_Atk(m_UnitName));
	}

	m_AniRen->CreateAnimation(Ani_Idle(m_UnitName));
	m_AniRen->CreateAnimation(Ani_Dead(m_UnitName));
	m_AniRen->CreateAnimation(Ani_Dying(m_UnitName));
	m_AniRen->CreateAnimation(Ani_Move(m_UnitName));
	m_AniRen->CreateAnimation(Ani_Limit(m_UnitName));
	m_AniRen->CreateAnimation(Ani_Stand(m_UnitName));
	m_AniRen->CreateAnimation(Ani_MAtk(m_UnitName));
	m_AniRen->CreateAnimation(Ani_MStand(m_UnitName));
	m_AniRen->CreateAnimation(Ani_Win(m_UnitName));
	m_AniRen->CreateAnimation(Ani_WinBe(m_UnitName));

	//m_AniRen->CreateAnimation(Ani_Jump(m_UnitName));

	m_PairMgr->CreateState<CEditCharIdle>(this);
	m_PairMgr->CreateState<CEditCharAction>(this);
	m_PairMgr->CreateState<CEditCharReAction>(this);
							
	m_PairMgr->ChangeState<CEditCharIdle>();

	ACTOR->SetImageScale(1.5f);

	m_bCharacter = true;

	m_pUnitStatTable = &(CEditGlobal::CharacterStatTable.find(m_UnitName)->second);
	m_UnitPara = CEditGlobal::CharacterParameter.find(m_UnitName)->second;

	if (nullptr == m_pUnitStatTable)
	{
		assert(nullptr);
		return;
	}


	SPTR<CGameMultiSprite> IdleSprite = COMRESMGR->FindMultiSprite(Idle(m_UnitName));
	if (nullptr != IdleSprite)
	{
		Vec3 SpritePivot = Vec3{ IdleSprite->GetSpritePivot().x, IdleSprite->GetSpritePivot().y,0.0f };
		Vec3 SpriteSize = Vec3{ 0.0f,IdleSprite->GetSpriteSize().y,0.0f };

		m_UnitMiddlePivot = SpritePivot + SpriteSize * ACTOR->GetMagnification()*0.5f;
		m_DemageFontPivot = SpritePivot + SpriteSize * ACTOR->GetMagnification();
	}
	LoadUseSkill();
}
void CEditBattleUnit::LoadUseSkill()
{
	for (size_t i = 0; i < CEditGlobal::vecSkill.size(); i++)
	{
		if (CEditGlobal::vecSkill[i].GetUnitIndex() == m_iCharIndex)
		{
			// 일단 모든 스킬레벨을 1로 가정
			int RandomLevel = CSJRandom::RandomInt(0, 4);
			m_mapUnitSkillData.insert(unordered_map<size_t, size_t>::value_type(CEditGlobal::vecSkill[i].SkillNo, RandomLevel));
			//UNITSKILLDATA Data = UNITSKILLDATA(CClientGlobal::vecSkill[i].SkillNo, 0);
			//m_vecUnitSkillData.push_back(Data);
		}
	}

}
void CEditBattleUnit::InitalizeMonster()
{

	m_AniRen = ACTOR->CreateCom<CAniRenderer>(RENDATA(LAYER_MONSTER, m_fAlpha));

	m_MonReactionRen = ACTOR->CreateCom<CFixRenderer>(RENDATA(LAYER_MONSTER, m_fAlpha));
	m_MonReactionRen->SetSprite(COMRESMGR->FindMultiSpriteToIndex(Atk(m_UnitName), 0));
	m_MonReactionRen->Off();

	Vec2 size = m_MonReactionRen->GetCurSpriteSize();

	if (nullptr == m_AniRen || nullptr == m_PairMgr)
	{
		return;
	}

	m_AniRen->CreateAnimation(Ani_Idle(m_UnitName));
	m_AniRen->CreateAnimation(Ani_Atk(m_UnitName));

	m_AniRen->ChangeAni(Idle(m_UnitName));

	m_PairMgr->CreateState<CEditMonIdle>(this);
	m_PairMgr->CreateState<CEditMonAction>(this);
	m_PairMgr->CreateState<CEditMonReAction>(this);

	m_PairMgr->ChangeState<CEditMonIdle>();

	ACTOR->Off();
	ACTOR->SetImageScale(1.5f);

	SPTR<CGameMultiSprite> IdleSprite = COMRESMGR->FindMultiSprite(Idle(m_UnitName));

	if (nullptr != IdleSprite)
	{
		Vec3 SpritePivot = Vec3{ IdleSprite->GetSpritePivot().x, IdleSprite->GetSpritePivot().y,0.0f };
		Vec3 SpriteSize = Vec3{ 0.0f,IdleSprite->GetSpriteSize().y,0.0f };

		m_UnitMiddlePivot = SpritePivot + SpriteSize * ACTOR->GetMagnification() * 0.5f;
		m_DemageFontPivot = SpritePivot + SpriteSize * ACTOR->GetMagnification() * 0.9f;
	}

	Vec2 CircleSize = Vec2{ IdleSprite->GetSpriteSize().x, IdleSprite->GetSpriteSize().x };


}
int CEditBattleUnit::GetCurAniLooping()
{
	return m_AniRen->GetCurAniCount();
}
STAT CEditBattleUnit::SetLevel(const int& _Level)
{
	if (_Level <= 0 || _Level > 100)
	{
		return STAT();
	}

	m_iLevel = _Level;
	m_CurUnitStat = (*m_pUnitStatTable)[_Level];
	m_bLevelUpdate = true;
	return m_CurUnitStat;
}
int	 CEditBattleUnit::GetLevel()
{
	return m_iLevel;
}
const tstring CEditBattleUnit::GetLevelString()
{
	TCHAR Level[8] = _T("");
	_stprintf_s(Level, _T("%d"), m_iLevel);
	tstring LevelString = Level;

	return LevelString;
}
bool CEditBattleUnit::IsUpdateLevel()
{
	return m_bLevelUpdate;
}
void CEditBattleUnit::UpdateLevel()
{
	m_bLevelUpdate = false;
}
STAT CEditBattleUnit::GetCurStat()
{
	return m_CurUnitStat;
}
STATPARAMETERDATA CEditBattleUnit::GetStatParameter()
{
	return m_UnitPara;
}
// 유닛의 색상을 단색 계통으로 바꿀때 사용하는 함수
bool CEditBattleUnit::SetFilter(const D3DXCOLOR& _Color, const float& _FilterTime, const int& _Count /*= 1*/)
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
	if (false == m_bCharacter && nullptr != m_MonReactionRen)
	{
		m_MonReactionRen->SetMaterial(_Color);
		m_MonReactionRen->MaterialOn();
	}

	return true;
}
// 필터를 적용,해제 하는 함수
void CEditBattleUnit::FilterUpdate()
{
	if (true == m_bFilterEnd)
	{
		return;
	}

	if (true == m_bFilterOn && m_fAccFilterTime >= m_fFilterTime)
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
	else if (false == m_bFilterOn && m_fAccFilterTime >= m_fFilterTime)
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
bool CEditBattleUnit::ReActionEvent(const int& _ReactionMode)
{
	return ReActionEvent((REACTION)_ReactionMode);
}
bool CEditBattleUnit::ReActionEvent(const REACTION& _ReactionMode)
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
CActObject* CEditBattleUnit::CreateActObject(const tstring& _Name /*= _T("")*/)
{
	return CActObject::CreateActObject(_Name, ACTOR->GetScene());
}
void CEditBattleUnit::SetVib(const float& _Time /*= LARGEFLOAT*/)
{
	m_fAccVibTime = 0.0f;
	m_fVibTime = _Time;
	m_bVibOn = true;
}
void CEditBattleUnit::VibrationUpdate()
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

void CEditBattleUnit::SetRenPivot(const Vec2& _Pivot)
{
	m_AniRen->SetRenPivot(_Pivot);

	if (nullptr != m_MonReactionRen)
		m_MonReactionRen->SetRenPivot(_Pivot);

}
void CEditBattleUnit::SetAlpha()
{
	if (nullptr == m_AniRen)
	{
		return;
	}

	m_AniRen->SetAlpha(m_fAlpha);

	if (nullptr != m_MonReactionRen)
		m_MonReactionRen->SetAlpha(m_fAlpha);

}
void CEditBattleUnit::SetAlpha(const float& _AlphaRatio)
{
	if (_AlphaRatio < 0.0f)
	{
		m_AniRen->SetAlpha(0.0f);
	}
	m_AniRen->SetAlpha(m_fAlpha* _AlphaRatio);

	if (false == m_bCharacter && nullptr != m_MonReactionRen)
		m_MonReactionRen->SetAlpha(m_fAlpha);

}
void CEditBattleUnit::SetTeamNumber(const int& _TeamIndex)
{
	if (_TeamIndex < 0 || _TeamIndex >= MAXTEAM)
	{
		return;
	}

	m_iTeamNumber = _TeamIndex;
}
int CEditBattleUnit::GetTeamNumber()
{
	return m_iTeamNumber;
}
bool CEditBattleUnit::PlaySound(const tstring& _SoundName)
{
	return m_UnitSound->PlaySound(_SoundName);
}
void CEditBattleUnit::InitSelectSkillData()
{
	m_SelectSkillNo = -1;
	m_SelectSkillLevel = -1;
	m_SelectTarget = MAXTARGET;
	m_bSelectAction = false;
	ClearCurTarget();
}
void CEditBattleUnit::AniRenOn()
{
	if (nullptr != m_AniRen)
		m_AniRen->On();
}
void CEditBattleUnit::ReactionRenOn()
{
	if (false == m_bCharacter && nullptr == m_MonReactionRen)
		m_MonReactionRen->On();
}
void CEditBattleUnit::AniRenOff()
{
	if (nullptr != m_AniRen)
		m_AniRen->Off();
}
void CEditBattleUnit::ReactionRenOff()
{
	if (false == m_bCharacter && nullptr == m_MonReactionRen)
		m_MonReactionRen->Off();

}
// State Change 되기 전에 한번 수행되는 함수
void CEditBattleUnit::ChangeStateEvent()
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
void CEditBattleUnit::ClearCurTarget()
{
	m_CurTargetUnit.clear();
}
bool CEditBattleUnit::SetCurTarget(const SPTR<CEditBattleUnit> _TargetUnit)
{
	if (nullptr == _TargetUnit)
	{
		return false;
	}

	m_CurTargetUnit.push_back(_TargetUnit);
	return true;
}
list<SPTR<CEditBattleUnit>>* CEditBattleUnit::GetCurTargetUnit()
{
	return &m_CurTargetUnit;
}
// 실제 랜더링 위치 + (UnitSprite* ACTORIMAGEMAG)*0.5f = UnitMiddlePos
const Vec3 CEditBattleUnit::GetUnitMiddlePos()
{
	return GetPos() + m_UnitMiddlePivot;
}
// 실제 랜더링 위치 + (UnitSprite* ACTORIMAGEMAG)*0.9f = UnitMiddlePos
const Vec3 CEditBattleUnit::GetUnitDemageFontPos()
{
	return GetPos() + m_DemageFontPivot;
}
const STAT CEditBattleUnit::GetCurUnitStat()
{
	return m_CurUnitStat;
}

const size_t CEditBattleUnit::GetTotalSkillCount()
{
	return m_mapUnitSkillData.size();
}
void CEditBattleUnit::Enter(SPTR<CCollision> _pThis, SPTR<CCollision> _pOtherCol)
{

}
void CEditBattleUnit::Stay(SPTR<CCollision> _pThis, SPTR<CCollision> _pOtherCol)
{

}
void CEditBattleUnit::Exit(SPTR<CCollision> _pThis, SPTR<CCollision> _pOtherCol)
{

}
void CEditBattleUnit::CollisionComOff()
{
	if (nullptr != m_UnitCol)
		m_UnitCol->Off();
}
void CEditBattleUnit::CollisionComOn()
{
	if (nullptr != m_UnitCol)
		m_UnitCol->On();
}
const Vec2 CEditBattleUnit::GetUnitFrontPos() 
{
	float HalfSizeX = m_AniRen->GetCurAniSpriteSize().x* ACTOR->GetMagnification()*0.5f;

	return Vec2{ TRANS->GetPos().x + HalfSizeX,TRANS->GetPos().y };
}
const Vec2 CEditBattleUnit::GetUnitBackPos()
{
	float HalfSizeX = m_AniRen->GetCurAniSpriteSize().x* ACTOR->GetMagnification()*0.5f;

	return Vec2{ TRANS->GetPos().x - HalfSizeX,TRANS->GetPos().y };
}
void CEditBattleUnit::TargetUnitEndReaction()
{
	list<SPTR<CEditBattleUnit>>::iterator Start = m_CurTargetUnit.begin();
	list<SPTR<CEditBattleUnit>>::iterator End = m_CurTargetUnit.end();

	for (; Start != End; ++Start)
	{
		if (nullptr == (*Start))
			(*Start)->EndReAction();
	}
}
void CEditBattleUnit::SetUnitImageMag(const float& _Mag)
{
	ACTOR->SetImageScale(_Mag);
}