#include "Precom.h"
#include "CharResultUI.h"
#include "BattleUnit.h"
#include "TurnMgr.h"
#include"ExpGauge.h"

CCharResultUI::CCharResultUI()
	:m_pPairUnit(nullptr), m_CharPortrait(nullptr)
	, m_CharLevelFont(nullptr), m_pPairGauge(nullptr)
	, m_GaugePivot(Vec3{38.f,-33.f,0.f})
	, m_LevelUpRen(nullptr), m_fAccTime(0.f)
	, m_InitFrameSize(Vec2{ 300.f, 112.f })
	, m_LevelUpFrameSize(Vec2{ 300.f, 170.f })
	, m_UIState(NO_LEVELUP), m_bFirstLevelUpEvent(true)
{
}


CCharResultUI::~CCharResultUI()
{
}
void CCharResultUI::Init() 
{
	m_BackFrame = ACTOR->CreateCom<CFixRenderer>(RENDATA(LAYER_UIBACK, 0.75f, true));
	m_BackFrame->SetSprite(_T("ResultBackFrame"));
	m_BackFrame->SetCustomSize(m_InitFrameSize);

	m_CharPortrait = ACTOR->CreateCom<CFixRenderer>(RENDATA(LAYER_UI, 0.9f, true));
	m_CharPortrait->SetRenPivot(Vec2{ -95.f,10.0f });
	m_CharPortrait->SetCustomSize(70.f, 70.f);

	m_CharNameFont = ACTOR->CreateCom<CFontRenderer>(RENDATA(LAYER_UI, 0.9f,true));
	m_CharNameFont->SetFontStyle(_T("MP회복폰트"));
	m_CharNameFont->SetFontMag(2.3f);
	m_CharNameFont->SetFontColor(D3DCOLOR_ARGB(255, 200, 200, 200));
	m_CharNameFont->SetLeftStartPivot(POINT{ -38 , 0 });

	m_CharLevelFont = ACTOR->CreateCom<CFontRenderer>(RENDATA(LAYER_UI, 0.9f, true));
	m_CharLevelFont->SetFontStyle(_T("Lv폰트"));
	m_CharLevelFont->SetFontColor(D3DCOLOR_ARGB(255, 0, 70, 200));
	m_CharLevelFont->SetLeftStartPivot(POINT{ -130 , -45 });

	m_LevelUpRen = ACTOR->CreateCom<CFixRenderer>(RENDATA(LAYER_UIBACK, 0.9f, true));
	m_LevelUpRen->SetSprite(_T("LevelUpFont"));
	m_LevelUpRen->SetCustomSize(Vec2{ 256.f,64.f });
	m_LevelUpRen->SetRenPivot(Vec2{ 0.f,-75.f });
	m_LevelUpRen->Off();


	m_pPairGauge = CClientGlobal::TurnMgr->CreateActObject(_T("ExpGauge"))->CreateCom<CExpGauge>();

	ACTOR->Off();
}
void CCharResultUI::Update()
{
	switch (m_UIState)
	{
	case LEVELUP:
		LevelUpUpdate();
		return;
	}


}
void CCharResultUI::SetPos(const Vec3& _Pos)
{
	TRANS->SetPos(_Pos);
	m_pPairGauge->SetPos( _Pos + m_GaugePivot);
}
void CCharResultUI::SetPairUnit(CBattleUnit* _Unit) 
{
	if (nullptr == _Unit)
		return;

	m_pPairUnit = _Unit;

	m_CharPortrait->SetSprite(Icon(_Unit->GetUnitName()));

	m_CharNameFont->SetString(m_pPairUnit->GetUnitKName());
	m_pPairGauge->SetPairResultUI(this);

}
CBattleUnit* CCharResultUI::GetPairUnit()
{
	return m_pPairUnit;
}
void CCharResultUI::StandBy(const UINT& _AddExp)
{
	if (nullptr == m_pPairUnit)
		return;

	m_bFirstLevelUpEvent = true;
	m_BackFrame->SetRenPivot(Vec2{ 0.f,0.f });
	m_BackFrame->SetCustomSize(m_InitFrameSize);

	TCHAR Temp[MAXKEYLENGTH] = _T("");

	_stprintf_s(Temp, _T("Lv %3d"), m_pPairUnit->GetLevel());
	m_CharLevelFont->SetString(Temp);
	m_CharLevelFont->SetFontColor(D3DCOLOR_ARGB(255, 0, 70, 200));

	if (nullptr != m_pPairGauge)
		m_pPairGauge->StandBy(_AddExp);

	m_UIState = NO_LEVELUP;
}
void CCharResultUI::UIOn()
{
	ACTOR->On();
	m_pPairGauge->GaugeOn();
}
void CCharResultUI::UIOff() 
{
	ACTOR->Off();
	m_LevelUpRen->Off();

	if (nullptr != m_pPairGauge)
		m_pPairGauge->GaugeOff();
}
void CCharResultUI::LevelUpEvent()
{
	if (nullptr == m_pPairUnit)
		return;

	TCHAR Temp[MAXKEYLENGTH] = _T("");

	_stprintf_s(Temp, _T("Lv %3d"), m_pPairUnit->GetLevel());
	m_CharLevelFont->SetString(Temp);
	m_CharLevelFont->SetFontColor(D3DCOLOR_ARGB(255, 200, 50, 0));

	m_fAccTime = 0.f;

	if (true == m_bFirstLevelUpEvent) 
	{
		m_UIState = LEVELUP;
		m_bFirstLevelUpEvent = false;
	}
}
void CCharResultUI::LevelUpUpdate()
{
	float t = m_fAccTime / 0.4f;

	if (t >= 1.f)
	{
		m_BackFrame->SetRenPivot(Vec2{ 0.f,-0.5f *(m_LevelUpFrameSize.y - m_InitFrameSize.y) });
		m_BackFrame->SetCustomSize(m_LevelUpFrameSize);
		m_LevelUpRen->On();
		m_UIState = NO_LEVELUP;
		return;
	}


	Vec2 Pivot = Vec2{ 0.f, -0.5f* t *(m_LevelUpFrameSize.y - m_InitFrameSize.y) };
	Vec2 Size = CSJMath::Vec2Lerp(&m_InitFrameSize, &m_LevelUpFrameSize, t);

	m_BackFrame->SetRenPivot(Pivot);
	m_BackFrame->SetCustomSize(Size);



	m_fAccTime += DELTATIME;
}
const bool CCharResultUI::IsGaugeUpdateEnd()
{
	return m_pPairGauge->IsGaugeUpdateEnd();
}