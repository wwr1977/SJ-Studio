#include "Precom.h"
#include "SkillDetailUI.h"
#include "StdGauge.h"
#include <GameMultiSprite.h>


SPTR<CGameMultiSprite> CSkillDetailUI::SkillDetailMainIconSprite = nullptr;

void CSkillDetailUI::InitDetailSkillUI(SPTR<CGameMultiSprite> _IconSprite) 
{
	if (nullptr == _IconSprite)
		return;

	SkillDetailMainIconSprite = _IconSprite;
}


CSkillDetailUI::CSkillDetailUI()
	:m_UIBack(nullptr), m_SkillIconRen(nullptr)
	, m_CurSkillData(SKILLUIDATA()), m_UIMainBack(nullptr)
	, m_SkillExpGauge(nullptr), m_SkillDetailRen(nullptr)
{

}


CSkillDetailUI::~CSkillDetailUI()
{
	m_SkillExpGauge = nullptr;
}
void CSkillDetailUI::Init()
{
	m_UIBack = ACTOR->CreateCom<CFixRenderer>(RENDATA(LAYER_UIBACK, 0.9f, true));
	m_UIBack->SetSprite(_T("ColorBase"));
	m_UIBack->SetMaterial(D3DXCOLOR(0.9f, 0.9f, 0.1f, 1.0f));
	m_UIBack->MaterialOn();
	m_UIBack->SetCustomSize(Vec2{ 844.f,254.f });

	m_UIMainBack = ACTOR->CreateCom<CFixRenderer>(RENDATA(LAYER_UIBACK, 1.0f, true));
	m_UIMainBack->SetSprite(_T("ColorBase"));
	m_UIMainBack->SetMaterial(D3DXCOLOR(0.275f, 0.275f, 0.275f, 1.0f));
	m_UIMainBack->MaterialOn();
	m_UIMainBack->SetCustomSize(Vec2{ 840.f,250.f });

	m_SkillIconRen = ACTOR->CreateCom<CFixRenderer>(RENDATA(LAYER_UI, 0.9f, true));
	m_SkillIconRen->SetCustomSize(Vec2{ 80.f,80.f });
	m_SkillIconRen->SetRenPivot(Vec2{ -365.f, 75.f });

	m_SkillTypeRen = ACTOR->CreateCom<CFixRenderer>(RENDATA(LAYER_UI, 0.7f, true));
	m_SkillTypeRen->SetCustomSize(Vec2{ 80.f,40.f });
	m_SkillTypeRen->SetRenPivot(Vec2{ -50.f, 57.f });

	m_DetailString.assign(MAXDETAILSTRING, _T(""));

	for (size_t i = 0; i < MAXDETAILSTRING; i++)
		m_DetailString[i].reserve(128);

	m_SkillExpGauge = CActObject::CreateActObject(_T("SkillExpGauge"), ACTOR->GetScene())->CreateCom<CStdGauge>();
	m_SkillExpGauge->SetGaugeFrame(_T("SkillXPFrame"));
	m_SkillExpGauge->CreateGaugeFont(_T("볼드굴림"));
	m_SkillExpGauge->SetFontString(0, _T("스킬 숙련도"));
	m_SkillExpGauge->SetFontMag(0,2.3f);
	m_SkillExpGauge->SetFontPivot(0, POINT{-175,18});
	m_SkillExpGauge->SetFrameSize(Vec2{ 400.f,25.f });
	m_SkillExpGauge->SetGaugeSize(Vec2{ 394.f,21.f });
	m_SkillExpGauge->SetPos(Vec3{ -30.f,-240.f,7.f });
	m_SkillExpGauge->SetGaugeColor(D3DXCOLOR(0.15f, 0.9f, 0.15f, 1.0f));


	m_SkillDetailRen = ACTOR->CreateCom<CScriptRenderer>(RENDATA(LAYER_UI, 1.f, true));
	m_SkillDetailRen->SetFontStyle(_T("큰UI레벨폰트"));
	m_SkillDetailRen->SetLineInterval(30);
	m_SkillDetailRen->SetLeftStartPivot(POINT{ 90,95 });
}
void CSkillDetailUI::Update()
{

}
void CSkillDetailUI::UIFontRender()
{
	COMRESMGR->DrawFont(_T("캐릭라인폰트"), m_DetailString[DETAIL_SKILLNAME], POINT{ -140 ,-145 }, 2.3f, D3DCOLOR_ARGB(255, 255, 220, 0));

	if(m_DetailString[DETAIL_SKILLLEVEL] == _T("Skill Lv Max"))
		COMRESMGR->DrawFont(_T("캐릭라인폰트"), m_DetailString[DETAIL_SKILLLEVEL], POINT{ -138 ,-185 }, 2.3f, D3DCOLOR_ARGB(255, 255, 25, 25));
	else 
		COMRESMGR->DrawFont(_T("캐릭라인폰트"), m_DetailString[DETAIL_SKILLLEVEL], POINT{ -138 ,-185 }, 2.3f, D3DCOLOR_ARGB(255, 255, 255, 255));

	//COMRESMGR->DrawFont(_T("큰UI레벨폰트"), m_DetailString[DETAIL_SKILLTYPE], POINT{ -228 ,-300 }, 2.3f, D3DCOLOR_ARGB(255, 255, 255, 255));
	COMRESMGR->DrawFont(_T("큰UI레벨폰트"), m_DetailString[DETAIL_SKILLDEMAGE], POINT{ -228 ,-290 }, 2.3f, D3DCOLOR_ARGB(255, 255, 255, 255));
	
	if(_T("") != m_DetailString[DETAIL_SKILLTARGET])
		COMRESMGR->DrawFont(_T("큰UI레벨폰트"), m_DetailString[DETAIL_SKILLTARGET], POINT{ -228 ,-315 }, 2.3f, D3DCOLOR_ARGB(255, 255, 255, 255));

	for (LONG i = DETAIL_COST1; i <= DETAIL_COST3; i++)
	{
		if (_T("") != m_DetailString[i])
			COMRESMGR->DrawFont(_T("큰UI레벨폰트"), m_DetailString[i], POINT{ 0 ,-190 -25*i }, 2.3f, D3DCOLOR_ARGB(255, 255, 255, 255));
	}

}
void CSkillDetailUI::SetSkillDetail(const size_t& _SkillNo)
{
	if (_SkillNo >= CClientGlobal::vecSkill.size())
		return;

	m_CurSkillData =  CClientGlobal::vecSkill[_SkillNo].SkillUIData;
	
	SKILLDATA::SKILLTYPE type = (SKILLDATA::SKILLTYPE)CClientGlobal::vecSkill[_SkillNo].SkillData.SkillType;
	
	UINT SkillLv = CClientGlobal::vecSkill[_SkillNo].CurSkillLevel;
	UINT CurExp = CClientGlobal::vecSkill[_SkillNo].CurSkillExp;

	if (4 == SkillLv) 
	{
		m_SkillExpGauge->SetMaxGauge(1);
		m_SkillExpGauge->SetCurGauge(1);
	}
	else
	{
		m_SkillExpGauge->SetMaxGauge(CClientGlobal::vecSkillMaxExp[SkillLv]);
		m_SkillExpGauge->SetCurGauge(CurExp);
	}

	LEVELDATA Data = CClientGlobal::vecSkillLevelData[_SkillNo][SkillLv];
	UINT Demage = 0;

	for (size_t i = 0; i < Data.HitDataCount; i++)
	{
		if (0 == Data.HitData[i].HitDemageMag)
			continue;

		Demage += (UINT)ceilf(Data.HitData[i].HitDemageMag * 100);
	}

	m_SkillIconRen->SetSprite(SkillDetailMainIconSprite->GetSprite(m_CurSkillData.SkillIconIndex));
	
	TCHAR Buf[MAXKEYLENGTH] = _T("");

	m_DetailString[DETAIL_SKILLNAME] = m_CurSkillData.SkillKName;

	if (4 == SkillLv)
		m_DetailString[DETAIL_SKILLLEVEL] = _T("Skill Lv Max");
	else 
	{
		_stprintf_s(Buf, sizeof(Buf) / sizeof(TCHAR), _T("Skill Lv %u"), SkillLv + 1);
		m_DetailString[DETAIL_SKILLLEVEL] = Buf;
	}

	switch (type)
	{				  
	case SKILLDATA::SKILLTYPE_ACTION:
		m_SkillTypeRen->SetSprite(_T("ActionDetail"));
		break;
	case SKILLDATA::SKILLTYPE_ABILITY:
		m_SkillTypeRen->SetSprite(_T("AbilityDetail"));
		break;
	case SKILLDATA::SKILLTYPE_BURSTSKILL:
		m_SkillTypeRen->SetSprite(_T("BurstDetail"));
		break;
	}

	_stprintf_s(Buf, sizeof(Buf) / sizeof(TCHAR), _T("데미지 : %u%%"),Demage);
	m_DetailString[DETAIL_SKILLDEMAGE] = Buf;

	m_DetailString[DETAIL_SKILLTARGET] = _T("");
	
	switch (CClientGlobal::vecSkill[_SkillNo].SkillData.EnemyTargetType)
	{
	case SKILLDATA::TARGET_SINGLE:
		m_DetailString[DETAIL_SKILLTARGET] = _T("타겟 : 단일");
		break;
	case SKILLDATA::TARGET_COLUMN:
		m_DetailString[DETAIL_SKILLTARGET] = _T("타겟 : 진영");
		break;
	case SKILLDATA::TARGET_ALL:
		m_DetailString[DETAIL_SKILLTARGET] = _T("타겟 : 전체");
		break;
	default:
		break;
	}

	for (size_t i = DETAIL_COST1; i <= DETAIL_COST3; i++)
		m_DetailString[i] = _T("");

	UINT Start = DETAIL_COST1;

	if (0 != Data.SkillCost[LEVELDATA::SKILLCOST_BURST]) 
	{
		_stprintf_s(Buf, sizeof(Buf) / sizeof(TCHAR), _T("소비 Burst : %3u"), Data.SkillCost[LEVELDATA::SKILLCOST_BURST]);
		m_DetailString[Start++] = Buf;
	}

	if (0 != Data.SkillCost[LEVELDATA::SKILLCOST_HP]) 
	{
		_stprintf_s(Buf, sizeof(Buf) / sizeof(TCHAR), _T("소비 HP : %3u"), Data.SkillCost[LEVELDATA::SKILLCOST_HP]);
		m_DetailString[Start++] = Buf;
	}

	if (0 != Data.SkillCost[LEVELDATA::SKILLCOST_MP])
	{
		_stprintf_s(Buf, sizeof(Buf) / sizeof(TCHAR), _T("소비 MP : %3u"), Data.SkillCost[LEVELDATA::SKILLCOST_MP]);
		m_DetailString[Start++] = Buf;
	}
	
	if (nullptr != m_SkillDetailRen)
		m_SkillDetailRen->SetScriptString(CClientGlobal::vecSkill[_SkillNo].SkillUIData.SkillDescription);
	

}
void CSkillDetailUI::SetPos(const Vec3& _Pos)
{
	TRANS->SetPos(_Pos);
}
void CSkillDetailUI::UIOn() 
{
	ACTOR->On();
	m_SkillExpGauge->GaugeOn();
}
void CSkillDetailUI::UIOff()
{
	ACTOR->Off();
	m_SkillExpGauge->GaugeOff();
}