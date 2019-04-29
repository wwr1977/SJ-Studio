#include "Precom.h"
#include "ExpGauge.h"
#include "CharResultUI.h"
#include "BattleUnit.h"


bool	CExpGauge::bGaugeUpdate = false;
void CExpGauge::StartGaugeUpdate()
{
	bGaugeUpdate = true;
}
void CExpGauge::InitGaugeUpdate()
{
	bGaugeUpdate = false;
}
CExpGauge::CExpGauge()
	:m_GaugeFrameRen(nullptr), m_ExpFont(nullptr), m_XpFont(nullptr)
	, m_pPairUI(nullptr), m_GaugeRen(nullptr)
	, m_iPrevGauge(0), m_iCurGauge(1), m_iMaxGauge(100)
	, m_MaxGaugeSize(Vec2{ 176.f ,19.f })
	, m_GaugePivot(Vec2{ 0.f,0.f })
	, m_LvUpArrow(nullptr)
	, m_fAccTime(0.f)
	, m_GaugeState(GAUGE_WAIT)
	, m_PrevCalExp(0), m_InitExp(0)
	, m_TotalAddExp(0)
	, m_AddXpFont(nullptr)
{
}


CExpGauge::~CExpGauge()
{

}
void CExpGauge::Init() 
{
	m_GaugeFrameRen = ACTOR->CreateCom<CFixRenderer>(RENDATA(LAYER_UIBACK, 0.9f, true));
	m_GaugeFrameRen->SetSprite(_T("XPFrame"));
	m_GaugeFrameRen->SetCustomSize(190.f, 27.f );

	
	m_GaugeRen = ACTOR->CreateCom<CFixRenderer>(RENDATA(LAYER_UIBACK, 0.9f, true));
	m_GaugeRen->SetSprite(_T("ColorBase"));
	m_GaugeRen->SetCustomSize(m_MaxGaugeSize);
	m_GaugeRen->SetMaterial(D3DXCOLOR(0.35f, 0.2f, 0.56f, 1.0f));
	m_GaugeRen->MaterialOn();

	m_LvUpArrow = ACTOR->CreateCom<CFixRenderer>(RENDATA(LAYER_UIBACK, 0.9f, true));
	m_LvUpArrow->SetSprite(_T("LevelUpArrow"));
	m_LvUpArrow->SetRenPivot(Vec2{ 95.f , 11.f });
	m_LvUpArrow->SetCustomSize(Vec2{ 20.0f,24.0f });
	m_LvUpArrow->Off();

	m_ExpFont = ACTOR->CreateCom<CFontRenderer>(RENDATA(LAYER_UI, 0.9f, true));
	m_ExpFont->SetFontStyle(_T("Lv폰트"));
	m_ExpFont->SetFontMag(2.3f);
	m_ExpFont->SetFontColor(D3DCOLOR_ARGB(255, 200, 200, 200));
	m_ExpFont->SetLeftStartPivot(POINT{ -88 , -2 });

	m_XpFont = ACTOR->CreateCom<CFontRenderer>(RENDATA(LAYER_UI, 0.9f, true));
	m_XpFont->SetFontStyle(_T("XP"));
	m_XpFont->SetFontColor(D3DCOLOR_ARGB(255, 200, 200, 200));
	m_XpFont->SetLeftStartPivot(POINT{ 63 , -5 });
	m_XpFont->SetString(_T("XP"));


	m_AddXpFont = ACTOR->CreateCom<CFontRenderer>(RENDATA(LAYER_UI, 0.9f, true));
	m_AddXpFont->SetFontStyle(_T("UI레벨폰트"));
	m_AddXpFont->SetFontColor(D3DCOLOR_ARGB(255, 200, 200, 200));
	m_AddXpFont->SetLeftStartPivot(POINT{ -63 , 20 });
	m_AddXpFont-> Off();


	ACTOR->Off();
}

void CExpGauge::Update()
{
	switch (m_GaugeState)
	{
	case GAUGE_WAIT:
		WaitStateFunc();
		return;
	case GAUGE_UPDATE:
		UpdateStateFunc();
		return;
	}
}
void CExpGauge::PostUpdate()
{
	if (m_iPrevGauge == m_iCurGauge)
		return;


	GaugeUpdate();
}
void CExpGauge::SetPairResultUI(CCharResultUI* _PairUI)
{
	if (nullptr == _PairUI)
		return;

	m_pPairUI = _PairUI;
	m_pPairUnit = _PairUI->GetPairUnit();
}
void CExpGauge::SetPos(const Vec3& _Pos)
{
	TRANS->SetPos(_Pos);
}
void CExpGauge::StandBy(const UINT& _AddExp)
{
	UINT CurLv = m_pPairUnit->GetLevel();

	if (MAXLEVEL <= CurLv)
	{
		m_iCurGauge = 0;
		m_iMaxGauge = 0;
		m_iPrevGauge = -1;
		return;
	}

	m_InitExp = m_pPairUnit->GetCurUnitExp();
	m_iCurGauge = m_InitExp;
	m_iMaxGauge = CClientGlobal::ExpTable[m_pPairUnit->GetLevel()];
	m_TotalAddExp = _AddExp;
	m_PrevCalExp = 0;
	GaugeUpdate();

	TCHAR Buf[MAXKEYLENGTH] = _T("");
	_stprintf_s(Buf, _T("+ %5dXP"), m_TotalAddExp);
	m_AddXpFont->SetString(Buf);

}
void CExpGauge::GaugeOn()
{
	ACTOR->On();
	m_LvUpArrow->Off();
	m_AddXpFont->Off();
	m_GaugeState = GAUGE_WAIT;
}
void CExpGauge::GaugeOff()
{
	ACTOR->Off();
}
void CExpGauge::GaugeUpdate()
{
	if (m_iCurGauge < 0)
		m_iCurGauge = 0;

	else if (m_iCurGauge > m_iMaxGauge) 
		m_iCurGauge = m_iMaxGauge;
	
	if (0 == m_iMaxGauge )
	{
		m_GaugeRen->SetCustomSize(m_MaxGaugeSize.x, m_MaxGaugeSize.y);
		m_GaugeRen->SetRenPivot(Vec2{ 0.f ,0.f });

		TCHAR Gauge[64] = _T("");
		_stprintf_s(Gauge, _T("%5d/%5d"), m_iMaxGauge, m_iMaxGauge);
		m_ExpFont->SetString(Gauge);
		m_ExpFont->SetFontMag(1.3f);
		return;
	}

	float CurRatio = (float)m_iCurGauge / (float)m_iMaxGauge;

	Vec2 CurGaugeSize = Vec2{ CurRatio*m_MaxGaugeSize.x,m_MaxGaugeSize.y };
	Vec2 CurGaugePivot = Vec2{ (m_MaxGaugeSize - CurGaugeSize).x* -0.5f ,0.0f };

	m_GaugeRen->SetCustomSize(CurGaugeSize);
	m_GaugeRen->SetRenPivot(CurGaugePivot + m_GaugePivot);

	TCHAR Gauge[64] = _T("");
	_stprintf_s(Gauge, _T("%5d/%5d"), m_iCurGauge, m_iMaxGauge);
	m_ExpFont->SetString(Gauge);
	m_ExpFont->SetFontMag(1.3f);

	m_iPrevGauge = m_iCurGauge;
}
void CExpGauge::SetCurGauge(const int& _Gauge) 
{
	m_iCurGauge = _Gauge;
}
void CExpGauge::SetMaxGauge(const int& _Gauge)
{
	m_iMaxGauge = _Gauge;
}
void CExpGauge::LevelUpEvent()
{
	m_LvUpArrow->On();
	m_pPairUI->LevelUpEvent();
}
void CExpGauge::WaitStateFunc()
{
	if (true != bGaugeUpdate) 
		return;
	
	m_GaugeState = GAUGE_UPDATE;
	m_AddXpFont->On();
	m_fAccTime = 0.f;

}
void CExpGauge::UpdateStateFunc()
{
	float t = m_fAccTime / 4.0f;
	float w = powf(t, 0.4f);

	if(t >= 1.f)
	{
		m_GaugeState = GAUGE_UPDATE_END;
		m_fAccTime = 0.f;
		return;
	}


	m_iCurGauge = m_InitExp + ((UINT)(m_TotalAddExp *w) - m_PrevCalExp);

	while (m_iCurGauge >= m_iMaxGauge) 
	{
		// Level 100 에서의 경험치 요구량은 0이다.
		if ( 0 == m_iMaxGauge)
			break;
	
		m_pPairUnit->LevelUp();
		LevelUpEvent();
		m_PrevCalExp += (m_iMaxGauge - m_InitExp);
		m_InitExp = 0;
		m_iCurGauge = m_InitExp + ((UINT)(m_TotalAddExp *w) - m_PrevCalExp);
		m_iMaxGauge = CClientGlobal::ExpTable[min(m_pPairUnit->GetLevel(),MAXLEVEL)];
		
	}
	
	m_pPairUnit->SetCurExp(m_iCurGauge);
	GaugeUpdate();

	m_fAccTime += DELTATIME;
}
bool CExpGauge::IsGaugeUpdateEnd()
{
	if (GAUGE_UPDATE_END == m_GaugeState)
		return true;

	return false;
}