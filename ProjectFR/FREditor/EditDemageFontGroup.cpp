#include "stdafx.h"
#include "EditDemageFontGroup.h"
#include "SkillMgr.h"
#include "EditBattleUnit.h"
#include "EditDemageFont.h"

CSkillMgr* CEditDemageFontGroup::DFGSkillMgr = nullptr;

void CEditDemageFontGroup::SetSkillMgr(CSkillMgr* _SkillMgr)
{
	if (nullptr != _SkillMgr)
		DFGSkillMgr = _SkillMgr;
}
CEditDemageFontGroup::CEditDemageFontGroup()
	:m_RenderingStyle(0), m_Option(0), m_CurFontCount(0)
{
	m_FontList.clear();
}


CEditDemageFontGroup::~CEditDemageFontGroup()
{
	m_FontList.clear();
}

void CEditDemageFontGroup::Init()
{

}
void CEditDemageFontGroup::Update()
{

}
CEditDemageFontGroup* CEditDemageFontGroup::CreateFontGroup(SPTR<CEditBattleUnit> _Caster, SPTR<CEditBattleUnit> _Target, LEVELDATA* _LevelData)
{
	if (nullptr == DFGSkillMgr)
		return nullptr;

	if (nullptr == _Caster || nullptr == _Target || nullptr == _LevelData)
		return nullptr;

	
	POINT Demage = POINT{100,200};
	bool Critical =true;
	
	if (true == _Caster->IsCharacter()) 
	{
		Demage = CalDemage(_Caster, _Target);
		Critical = IsCritical(_Caster, _Target);
	}
	
	

	switch (_LevelData->DemageStyle)
	{
	case DEMAGESTYLE_EXPANSION:
		CreateExpList(_Target, _LevelData, Demage, Critical);
		break;
	case DEMAGESTYLE_STAIRS:
		CreateStairList(_Target, _LevelData, Demage, Critical);
		break;
	default:
		break;
	}
	

	return this;
}
POINT CEditDemageFontGroup::CalDemage(SPTR<CEditBattleUnit> _Caster, SPTR<CEditBattleUnit> _Target)
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

	/* 정밀 계산식
	int Def = TargetStat.Defense;
	int LevelDiff = _Caster->GetLevel() - _Target->GetLevel();


	Min -= min(SubLimit,Def);
	Max -= min(SubLimit,Def);

	Min = Min * DefReductionRate(Def) * LevelBonusRate(LevelDiff);
	Max = Max * DefReductionRate(Def) * LevelBonusRate(LevelDiff);
*/

	Demage.x = Min;
	Demage.y = Max;

	return Demage;
}
bool CEditDemageFontGroup::IsCritical(SPTR<CEditBattleUnit> _Caster, SPTR<CEditBattleUnit> _Target)
{
	if (nullptr == _Caster || nullptr == _Target)
		return false;

	STAT CasterStat = _Caster->GetCurStat();
	STAT TargetStat = _Target->GetCurStat();


	int CasterCri = CasterStat.Critical * 10;
	int Cri = CSJRandom::RandomInt(0, 100);
	bool CriCheck = false;

	if (CasterCri >= Cri)
		CriCheck = true;

	return CriCheck;
}

void CEditDemageFontGroup::CreateExpList(SPTR<CEditBattleUnit> _Target, LEVELDATA* _LevelData, const POINT& _Demage, const bool& _Critical)
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
			m_FontList.push_back(DFGSkillMgr->GetDemageFont((int)(Demage* SkillMag), RenPos, DEMAGE_RED));
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
			m_FontList.push_back(DFGSkillMgr->GetDemageFont((int)(Demage* SkillMag), RenPos, DEMAGE_SP));
		}
	}

	list<SPTR<CEditDemageFont>>::iterator Start = m_FontList.begin();
	list<SPTR<CEditDemageFont>>::iterator End = m_FontList.end();

	for ( ;Start!= End ;++Start)
	{
		(*Start)->SetExpStyle();
		++m_CurFontCount;
	}

	return;
}
void CEditDemageFontGroup::CreateStairList(SPTR<CEditBattleUnit> _Target, LEVELDATA* _LevelData, const POINT& _Demage, const bool& _Critical)
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
			m_FontList.push_back(DFGSkillMgr->GetDemageFont((int)(Demage* SkillMag), RenPos, DEMAGE_RED));
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
			m_FontList.push_back(DFGSkillMgr->GetDemageFont((int)(Demage* SkillMag), RenPos, DEMAGE_SP));
		}
	}


	list<SPTR<CEditDemageFont>>::iterator Start = m_FontList.begin();
	list<SPTR<CEditDemageFont>>::iterator End = m_FontList.end();

	for (; Start != End; ++Start)
	{
		(*Start)->SetStairStyle(_LevelData->Option);
		++m_CurFontCount;
	}

}

Vec3 CEditDemageFontGroup::GetStairDir(const int& _Option)
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
void CEditDemageFontGroup::GroupFontReturn()
{
	if (true == m_FontList.empty())
		return;


	list<SPTR<CEditDemageFont>>::iterator Start = m_FontList.begin();
	list<SPTR<CEditDemageFont>>::iterator End = m_FontList.end();

	for (; Start != End; )
	{
		DFGSkillMgr->ReturnDemageFont(*Start);
		Start = m_FontList.erase(Start);
	}
}

bool CEditDemageFontGroup::DemageFontOn()
{
	// 첫 폰트를 키는거면서 Style이 Stair인 경우 
	// 리스트안의 모든 폰트 On 한다.(Update만 수행)
	if (true == m_FontList.empty())
		return false;

	SPTR<CEditDemageFont> Font = m_FontList.front();

	Font->ActorOn();

	if (m_CurFontCount == m_FontList.size() && DEMAGESTYLE_STAIRS == m_RenderingStyle)
	{	
		list<SPTR<CEditDemageFont>>::iterator Start = m_FontList.begin();
		list<SPTR<CEditDemageFont>>::iterator End = m_FontList.end();

		for (;Start != End; ++Start)
		{
			(*Start)->ActorOn();
		}
	}

	m_FontList.pop_front();

	if(0  >= m_FontList.size())
		return false;
	
	return true;
}
bool CEditDemageFontGroup::IsEmpty()
{
	return m_FontList.empty();
}