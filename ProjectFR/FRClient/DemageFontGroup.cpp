#include "Precom.h"
#include "DemageFontGroup.h"
#include "TurnMgr.h"
#include "BattleUnit.h"
#include "DemageFont.h"

CTurnMgr* CDemageFontGroup::DFGTurnMgr = nullptr;

void CDemageFontGroup::SetTurnMgr(CTurnMgr* _TurnMgr)
{
	if (nullptr != _TurnMgr)
		DFGTurnMgr = _TurnMgr;
}
CDemageFontGroup::CDemageFontGroup()
	:m_RenderingStyle(0), m_Option(0), m_CurFontCount(0)
	, m_CurGroupIndex(0)
{
	m_FontList.clear();
}


CDemageFontGroup::~CDemageFontGroup()
{
	m_FontList.clear();
}

void CDemageFontGroup::Init()
{

}
void CDemageFontGroup::Update()
{

}
CDemageFontGroup* CDemageFontGroup::CreateFontGroup(SPTR<CBattleUnit> _Caster, SPTR<CBattleUnit> _Target, LEVELDATA* _LevelData)
{
	if (nullptr == DFGTurnMgr)
		return nullptr;

	if (nullptr == _Caster || nullptr == _Target || nullptr == _LevelData)
		return nullptr;

	POINT Demage = POINT{1000,2000};
	
	bool Critical = true;
	float CriMag = _Caster->GetCriticalMag();
	
	Demage = CalDemage(_Caster, _Target);
	Critical = IsCritical(_Caster, _Target);
	/*if (true == _Caster->IsCharacter()) 
	{*/
	//}

	switch (_LevelData->DemageStyle)
	{
	case DEMAGESTYLE_EXPANSION:
		CreateExpList(_Target, _LevelData, Demage, Critical, CriMag);
		break;
	case DEMAGESTYLE_STAIRS:
		CreateStairList(_Target, _LevelData, Demage, Critical, CriMag);
		break;
	default:
		break;
	}
	

	return this;
}
POINT CDemageFontGroup::CalDemage(SPTR<CBattleUnit> _Caster, SPTR<CBattleUnit> _Target)
{
	if (nullptr == _Caster || nullptr == _Target)
		return POINT{ 0,0 };

	POINT Demage = POINT{ 0,0 };

	// 데미지 계산 
	STAT CasterStat = _Caster->GetCurStat();
	STAT TargetStat = _Target->GetCurStat();

	int Min = CasterStat.MinDamage; 
	int Max = CasterStat.MaxDamage;
	int SubLimit = Min + Max / 2;

	
	/*정밀 계산식
	int Def = TargetStat.Defense;
	int LevelDiff = _Caster->GetLevel() - _Target->GetLevel();


	Min -= min(SubLimit,Def);
	Max -= min(SubLimit,Def);

	Min = Min * DefReductionRate(Def) * LevelBonusRate(LevelDiff);
	Max = Max * DefReductionRate(Def) * LevelBonusRate(LevelDiff);
	*/

	float DefReduce = CClientGlobal::GetSigmoidValue(_T("DefFunc"), (float)TargetStat.Defense);

	Demage.x = Min;
	Demage.y = Max;

	//Demage.x = (LONG)(Demage.x* (1.0f - DefReduce));
	//Demage.y = (LONG)(Demage.y* (1.0f - DefReduce));

	return Demage;
}
bool CDemageFontGroup::IsCritical(SPTR<CBattleUnit> _Caster, SPTR<CBattleUnit> _Target) 
{
	if (nullptr == _Caster || nullptr == _Target)
		return false;

	STAT CasterStat = _Caster->GetCurStat();
	STAT TargetStat = _Target->GetCurStat();


	//int CasterCri = CasterStat.Critical * 10;
	int CasterCri = CasterStat.Critical;
	int Cri = CSJRandom::RandomInt(0, 100);
	bool CriCheck = false;

	if (CasterCri >= Cri)
		CriCheck = true;

	return CriCheck;
}

void CDemageFontGroup::CreateExpList(SPTR<CBattleUnit> _Target, LEVELDATA* _LevelData, const POINT& _Demage, const bool& _Critical, const float& _CriMag)
{
	if (nullptr == _LevelData)
		return;

	m_FontList.clear();
	m_CurFontCount = 0;

	if (true == _Critical) 
	{
		for (size_t i = 0; i < _LevelData->HitDataCount; i++)
		{
			int Demage = CSJRandom::RandomInt(_Demage.x, _Demage.y);
			float SkillMag = _LevelData->HitData[i].HitDemageMag;
			if (0.0f == SkillMag)
				continue;
			Vec3 RenPos = _Target->GetUnitMiddlePos();
			RenPos += Vec3{ CSJRandom::RandomFloat(-50.0f,50.0f),CSJRandom::RandomFloat(-50.0f,50.0f),0.0f };
			m_FontList.push_back(DFGTurnMgr->GetDemageFont((int)(Demage* SkillMag * _CriMag), RenPos, DEMAGE_RED));
			DFGTurnMgr->PushDemage(m_CurGroupIndex, (UINT)(Demage * SkillMag * _CriMag));
		}
	
	}
	else 
	{
		for (size_t i = 0; i < _LevelData->HitDataCount; i++)
		{
			int Demage = CSJRandom::RandomInt(_Demage.x, _Demage.y);
			float SkillMag = _LevelData->HitData[i].HitDemageMag;
			if (0.0f == SkillMag)
				continue;
			Vec3 RenPos = _Target->GetUnitMiddlePos();
			RenPos += Vec3{ CSJRandom::RandomFloat(-50.0f,50.0f),CSJRandom::RandomFloat(-50.0f,50.0f),0.0f };
			m_FontList.push_back(DFGTurnMgr->GetDemageFont((int)(Demage* SkillMag), RenPos, DEMAGE_YELLOW));
			DFGTurnMgr->PushDemage(m_CurGroupIndex, (UINT)(Demage* SkillMag));
		}
	}

	list<SPTR<CDemageFont>>::iterator Start = m_FontList.begin();
	list<SPTR<CDemageFont>>::iterator End = m_FontList.end();

	for ( ;Start!= End ;++Start)
	{
		(*Start)->SetExpStyle();
		++m_CurFontCount;
	}

	return;
}
void CDemageFontGroup::CreateStairList(SPTR<CBattleUnit> _Target, LEVELDATA* _LevelData, const POINT& _Demage, const bool& _Critical, const float& _CriMag)
{
	if (nullptr == _LevelData)
		return;

	m_FontList.clear();
	m_CurFontCount = 0;

	Vec3 Dir = GetStairDir(_LevelData->Option);
	float Dis = 50.0f;

	if (true == _Critical)
	{
		for (size_t i = 0; i < _LevelData->HitDataCount; i++)
		{
			int Demage = CSJRandom::RandomInt(_Demage.x, _Demage.y);
			float SkillMag = _LevelData->HitData[i].HitDemageMag;
			if (0.0f == SkillMag)
				continue;
			Vec3 RenPos = _Target->GetUnitMiddlePos();
			RenPos += (Dir*(float)(Dis*i));
			m_FontList.push_back(DFGTurnMgr->GetDemageFont((int)(Demage* SkillMag * _CriMag), RenPos, DEMAGE_RED));
			DFGTurnMgr->PushDemage(m_CurGroupIndex, (UINT)(Demage * SkillMag * _CriMag));
		}
	}
	else
	{
		for (size_t i = 0; i < _LevelData->HitDataCount; i++)
		{
			int Demage = CSJRandom::RandomInt(_Demage.x, _Demage.y);
			float SkillMag = _LevelData->HitData[i].HitDemageMag;
			if (0.0f == SkillMag)
				continue;
			Vec3 RenPos = _Target->GetUnitMiddlePos();
			RenPos += (Dir*(float)(Dis*i));
			m_FontList.push_back(DFGTurnMgr->GetDemageFont((int)(Demage* SkillMag), RenPos, DEMAGE_YELLOW));
			DFGTurnMgr->PushDemage(m_CurGroupIndex, (UINT)(Demage* SkillMag));
		}
	}


	list<SPTR<CDemageFont>>::iterator Start = m_FontList.begin();
	list<SPTR<CDemageFont>>::iterator End = m_FontList.end();

	for (; Start != End; ++Start)
	{
		(*Start)->SetStairStyle(_LevelData->Option);
		++m_CurFontCount;
	}

}

Vec3 CDemageFontGroup::GetStairDir(const int& _Option)
{
	Vec3 DirY = Vec3{ 0.0f,1.0f,0.0f };
	Vec3 DirX = Vec3{ 0.025f,0.0f,0.0f };
	Vec3 Dir;

	//Option -10 ~ 10
	DirX *= (float)_Option;
	DirX += DirY;
	D3DXVec3Normalize(&Dir, &DirX);

	return Dir;
}
void CDemageFontGroup::GroupFontReturn()
{
	if (true == m_FontList.empty())
		return;


	list<SPTR<CDemageFont>>::iterator Start = m_FontList.begin();
	list<SPTR<CDemageFont>>::iterator End = m_FontList.end();

	for (; Start != End; )
	{
		DFGTurnMgr->ReturnDemageFont(*Start);
		Start = m_FontList.erase(Start);
	}
}

bool CDemageFontGroup::DemageFontOn()
{
	// 첫 폰트를 키는거면서 Style이 Stair인 경우 
	// 리스트안의 모든 폰트 On 한다.(Update만 수행)
	if (true == m_FontList.empty())
		return false;

	SPTR<CDemageFont> Font = m_FontList.front();

	Font->ActorOn();

	if (m_CurFontCount == m_FontList.size() && DEMAGESTYLE_STAIRS == m_RenderingStyle)
	{	
		list<SPTR<CDemageFont>>::iterator Start = m_FontList.begin();
		list<SPTR<CDemageFont>>::iterator End = m_FontList.end();

		for (;Start != End; ++Start)
		{
			(*Start)->ActorOn();
		}
	}

	m_FontList.pop_front();
	
	return true;
}
bool CDemageFontGroup::IsEmpty()
{
	return m_FontList.empty();
}
void CDemageFontGroup::SetGroupIndex(const UINT& _Index)
{
	m_CurGroupIndex = _Index;
}
