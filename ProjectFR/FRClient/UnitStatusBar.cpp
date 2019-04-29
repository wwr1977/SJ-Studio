#include "Precom.h"
#include "UnitStatusBar.h"
#include "GaugeBar.h"
#include "UnitPortrait.h"
#include"NumberUI.h"
#include"BattleUnit.h"

float	CUnitStatusBar::RelocationTime = 1.f;

CUnitStatusBar::CUnitStatusBar()
	: m_UnitPortrait(nullptr),	m_BarBack(nullptr)
	,  m_PortraitPivot({0.0f,0.0f,0.0f})
	, m_UnitLevelBox(nullptr), m_UnitFontRen(nullptr)
	, m_bRelocationEvent(false), m_UnitPortraitFrame(nullptr)
	, m_RelocationStartPos(Vec3{0.0f,0.0f,0.0f})
	, m_RelocationEndPos(Vec3{0.0f,0.0f,0.0f})
	, m_fAccTime(0.f)
	
{
}


CUnitStatusBar::~CUnitStatusBar()
{
}

void CUnitStatusBar::Init()
{
	

}
void CUnitStatusBar::Update()
{
	RelocationUpdate();

	if (true == m_PairUnit->IsCharacter() && true == m_PairUnit->IsUpdateLevel())
	{
		m_UnitFontRen->SetString(m_PairUnit->GetLevelString());
		m_PairUnit->UpdateLevel();
	}


}
void CUnitStatusBar::DebugRender()
{
}
void CUnitStatusBar::FontRender()
{
	
}
void CUnitStatusBar::UIMove(const Vec3& _Pos)
{
	TRANS->SetPos(_Pos);
}
void CUnitStatusBar::SetUnit(CBattleUnit* _Unit)
{
	if (nullptr == _Unit)
	{
		assert(nullptr);
		return;
	}

	m_PairUnit = _Unit;
	
	if (true == m_PairUnit->IsCharacter()) 
	{
		SettingCharMode();
	}
	else 
	{
		SettingMonMode();
	}

}
const Vec2 CUnitStatusBar::GetBarBackSize()
{
	return m_BarBack->GetCurSpriteSize();
}
void CUnitStatusBar::On()
{
	ACTOR->On();
	SetAlpha(1.f);
	/*if (true == m_PairUnit->IsCharacter()) 
	{
		m_MpBar->Reset();
	}
	m_HpBar->Reset();*/
}
void CUnitStatusBar::Off() 
{
	ACTOR->Off();
}
void CUnitStatusBar::SetPos(const Vec3& _Pos)
{
	TRANS->SetPos(_Pos);
}
void CUnitStatusBar::SetAlpha(const float& _AlphaRatio)
{
	if(nullptr != m_BarBack)
		m_BarBack->SetAlpha(0.5f*_AlphaRatio);

	if (nullptr != m_UnitPortraitFrame)
		m_UnitPortraitFrame->SetAlpha(_AlphaRatio);

	if (nullptr != m_UnitPortrait)
		m_UnitPortrait->SetAlpha(0.9f*_AlphaRatio);

	if (nullptr != m_UnitLevelBox)
		m_UnitLevelBox->SetAlpha(0.8f*_AlphaRatio);

	if (nullptr != m_UnitFontRen)
		m_UnitFontRen->SetAlpha(_AlphaRatio);

	if (nullptr != m_HpBar)
		m_HpBar->SetAlpha(_AlphaRatio);

	if (nullptr != m_MpBar)
		m_MpBar->SetAlpha(_AlphaRatio);

}
void CUnitStatusBar::UpdateHp(const UINT& _HP)
{
	m_HpBar->SetCurGauge(_HP);
}
void CUnitStatusBar::UpdateMp(const UINT& _MP)
{
	m_MpBar->SetCurGauge(_MP);
}
void CUnitStatusBar::UpdateHp(const UINT& _MaxHP, const UINT& _HP) 
{
	m_HpBar->SetMaxGauge(_MaxHP);
	m_HpBar->SetCurGauge(_HP);
}
void CUnitStatusBar::UpdateMp(const UINT& _MaxMP, const UINT& _MP)
{
	m_MpBar->SetMaxGauge(_MaxMP);
	m_MpBar->SetCurGauge(_MP);
}
void CUnitStatusBar::SettingCharMode()
{
	m_BarBack = ACTOR->CreateCom<CFixRenderer>(RENDATA(LAYER_UIBACK, 0.5f, true));
	m_BarBack->SetSprite(_T("CharBarBack"));
	
	m_UnitPortraitFrame = ACTOR->CreateCom<CFixRenderer>(RENDATA(LAYER_UI, 1.0f, true));
	m_UnitPortraitFrame->SetSprite(_T("PortraitFrame"));
	m_UnitPortraitFrame->SetCustomSize({ 40.0f, 54.0f });
	
	CalPortraitPivot(m_UnitPortraitFrame->GetCustomSize());
	m_UnitPortraitFrame->SetRenPivot(m_PortraitPivot);

	m_UnitPortrait = ACTOR->CreateCom<CFixRenderer>(RENDATA(LAYER_UI, 0.9f, true));
	m_UnitPortrait->SetCustomSize({ 40.0f, 50.0f });
	m_UnitPortrait->SetRenPivot(m_PortraitPivot);
	m_UnitPortrait->SetSprite(Icon(m_PairUnit->GetUnitName()));

	m_UnitLevelBox = ACTOR->CreateCom<CFixRenderer>(RENDATA(LAYER_UI, 0.8f, true));
	m_UnitLevelBox->SetSprite(_T("LevelBox"));
	m_UnitLevelBox->SetCustomSize(Vec2{ 20.0f,20.0f });
	m_UnitLevelBox->SetRenPivot({ -41.0f,18.0f });

	m_HpBar = ACTOR->CreateCom<CGaugeBar>(this);
	m_HpBar->SetGaugeFrame(_T("GaugeFrame"));
	m_MpBar = ACTOR->CreateCom<CGaugeBar>(this);
	m_MpBar->SetGaugeFrame(_T("GaugeFrame"));

	//int Level = 10;					//CSJRandom::RandomInt(10, 99);
	//m_PairUnit->SetLevel(Level);
	m_PairUnit->SetInitLevel();

	m_HpBar->SetHpColorUpdateFunc();

	m_HpBar->SetFrameSize(Vec2{ 110.0f,24.0f });
	m_HpBar->SetGaugeSize(Vec2{ 106.0f,20.0f });
	m_HpBar->SetGaugeColor(D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));
	m_HpBar->SetMaxGauge(m_PairUnit->GetMaxHp());
	m_HpBar->SetCurGauge(m_PairUnit->GetCurHp());
	m_HpBar->SetFontPivot({ 0.0f,-2.0f,0.0f });
	m_HpBar->SetPivot(Vec2{ 20.0f,10.0f });
	
	m_MpBar->SetFrameSize(Vec2{ 90.0f,24.0f });
	m_MpBar->SetGaugeSize(Vec2{ 86.0f,20.0f });

	m_MpBar->SetGaugeColor(D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f));
	m_MpBar->SetMaxGauge(m_PairUnit->GetMaxMp());
	m_MpBar->SetCurGauge(m_PairUnit->GetCurMp());
	m_MpBar->SetFontPivot({ 0.0f,-3.0f,0.0f });
	m_MpBar->SetPivot({ 10.0f,-10.0f });

	m_UnitFontRen = ACTOR->CreateCom<CFontRenderer>(RENDATA(LAYER_UI, 1.0f,true));
	m_UnitFontRen->SetMiddleStartPivot(Vec3({ -40.0f,11.0f,0.0f }));
	m_UnitFontRen->SetFontMag(2.0f);
	
}
void CUnitStatusBar::SettingMonMode()
{
	m_BarBack = ACTOR->CreateCom<CFixRenderer>(RENDATA(LAYER_UIBACK, 0.7f, true));
	m_BarBack->SetSprite(_T("MonBarBack"));

	m_UnitPortrait = ACTOR->CreateCom<CFixRenderer>(RENDATA(LAYER_UI, 0.9f, true));
	m_UnitPortrait->SetRenPivot({ -67.0f, 0.0f });
	m_UnitPortrait->SetSprite(Icon(m_PairUnit->GetUnitName()));



	m_HpBar = ACTOR->CreateCom<CGaugeBar>(this);
	
	m_HpBar->SetHpColorUpdateFunc();
	
	m_HpBar->SetGaugeFrame(_T("GaugeFrame"));

	m_HpBar->SetFrameSize(Vec2{ 138.0f, 25.0f });
	m_HpBar->SetGaugeSize(Vec2{ 134.0f,21.0f });
	m_HpBar->SetGaugeColor(D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));
	m_HpBar->SetMaxGauge(m_PairUnit->GetCurHp());
	m_HpBar->SetCurGauge(m_PairUnit->GetCurMp());
	m_HpBar->SetFontPivot({ 0.0f,-2.0f,0.0f });
	m_HpBar->SetPivot({ 30.0f,-14.5f });
	

	m_UnitFontRen = ACTOR->CreateCom<CFontRenderer>(RENDATA(LAYER_UI, 1.0f));
	m_UnitFontRen->SetLeftStartPivot(POINT({ -38,7 }));
	m_UnitFontRen->SetFontMag(2.0f);
	m_UnitFontRen->SetString(m_PairUnit->GetUnitName());
}
void CUnitStatusBar::StandBy()
{
	if (nullptr == m_PairUnit)
		return;

	if (true == m_PairUnit->IsCharacter())
		StandByCharacterMode();	
	else 
		StandByMonsterMode();

}

void CUnitStatusBar::StandByMonsterMode()
{
	m_HpBar->SetMaxGauge(m_PairUnit->GetMaxHp());
	m_HpBar->SetCurGauge(m_PairUnit->GetCurHp());
	m_HpBar->CompulsionUpdate();
}
void CUnitStatusBar::StandByCharacterMode()
{
	m_HpBar->SetMaxGauge(m_PairUnit->GetMaxHp());
	m_HpBar->SetCurGauge(m_PairUnit->GetCurHp()); 

	
	m_MpBar->SetMaxGauge(m_PairUnit->GetMaxMp());
	m_MpBar->SetCurGauge(m_PairUnit->GetCurMp());
	
	m_HpBar->CompulsionUpdate();
	m_MpBar->CompulsionUpdate();
}
void CUnitStatusBar::CalPortraitPivot(const Vec2& _FrameSize)
{
	Vec2 BarMiddle = m_BarBack->GetCurSpriteSize()*0.5f;

	m_PortraitPivot = Vec3{ -(BarMiddle.x - _FrameSize.x * 0.5f - 1) ,0.0f ,0.0f};
}
void CUnitStatusBar::RelocationEvent()
{
	m_bRelocationEvent = true;
	m_fAccTime = 0.0f;

	m_RelocationEndPos = TRANS->GetPos();
	m_RelocationStartPos = m_RelocationEndPos + Vec3{0.f,-100.f,0.f};
}
void CUnitStatusBar::RelocationUpdate() 
{
	if (false == m_bRelocationEvent)
		return;
	
	bool Char = m_PairUnit->IsCharacter();

	if(m_fAccTime >= RelocationTime) 
	{
		m_bRelocationEvent = false;
		TRANS->SetPos(m_RelocationEndPos);
		SetAlpha(1.f);
		return;
	}

	
	if (true == Char)
		RelocationChar();
	else
		RelocationMon();

	m_fAccTime += DELTATIME;
}
void CUnitStatusBar::RelocationChar()
{

}
void CUnitStatusBar::RelocationMon()
{
	float t = m_fAccTime / RelocationTime;

	TRANS->SetPos(CSJMath::Vec3Lerp(&m_RelocationStartPos, &m_RelocationEndPos, t));
	SetAlpha(t);

}
void CUnitStatusBar::ChangeName(const tstring& _NewName)
{
	m_UnitFontRen->SetString(_NewName);
}