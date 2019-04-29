#include "stdafx.h"
#include "Gauge.h"


CGauge::CGauge()
	:m_Back(nullptr), m_Gauge(nullptr), m_GaugeFrame(nullptr)
	, m_iMaxGauge(0), m_iPrevGauge(-1), m_iCurGauge(-1)
	, m_MaxGaugeSize(Vec2{ 0.0f,0.0f }), m_GaugePivot(Vec2{0.0f,0.0f})
{
}


CGauge::~CGauge()
{
}
void CGauge::Init()
{
	m_Back = ACTOR->CreateCom<CFixRenderer>(RENDATA(LAYER_UIBACK, 0.7f, true));
	m_Gauge = ACTOR->CreateCom<CFixRenderer>(RENDATA(LAYER_UI, 0.5f, true));
	m_GaugeFrame = ACTOR->CreateCom<CFixRenderer>(RENDATA(LAYER_UI, 1.0f, true));

	m_Gauge->SetSprite(_T("ColorBase"));
	TRANS->SetPos({ -300.0f,-200.0f ,30.0f});
}
void CGauge::Update()
{
	if (m_iPrevGauge == m_iCurGauge)
	{
		return;
	}

	GaugeUpdate();
	m_iPrevGauge = m_iCurGauge;
}


void CGauge::SetGaugeBack(const tstring& _SpriteKey, const Vec2& _RenPivot /*= Vec2{ 0.0f,0.0f }*/) 
{
	m_Back->SetSprite(_SpriteKey);
	m_Back->SetRenPivot(_RenPivot);

}
void CGauge::SetGaugeFrame(const tstring& _SpriteKey, const Vec2& _RenPivot/* = Vec2{ 0.0f,0.0f }*/) 
{
	m_GaugeFrame->SetSprite(_SpriteKey);
	m_GaugeFrame->SetRenPivot(_RenPivot);
}
void CGauge::SetBackSize(const Vec2& _Size) 
{
	m_Back->SetCustomSize(_Size);
}
void CGauge::SetGaugeSize(const Vec2& _Size)
{
	m_Gauge->SetCustomSize(_Size);
	m_MaxGaugeSize = _Size;
}
void CGauge::SetFrameSize(const  Vec2& _Size) 
{
	m_GaugeFrame->SetCustomSize(_Size);
}
void CGauge::SetBackPivot(const Vec2& _Pivot) 
{
	m_Back->SetRenPivot(_Pivot);
}
void CGauge::SetGaugePivot(const Vec2& _Pivot) 
{
	m_Gauge->SetRenPivot(_Pivot);
	m_GaugePivot = _Pivot;
}
void CGauge::SetFramePivot(const Vec2& _Pivot)
{
	m_GaugeFrame->SetRenPivot(_Pivot);
}
void CGauge::SetBackColor(D3DXCOLOR _Color) 
{
	m_Back->SetMaterial(_Color);
	m_Back->MaterialOn();
}
void CGauge::SetGaugeColor(D3DXCOLOR _Color) 
{
	m_Gauge->SetMaterial(_Color);
	m_Gauge->MaterialOn();
}
void CGauge::SetFrameColor(D3DXCOLOR _Color)
{
	m_GaugeFrame->SetMaterial(_Color);
	m_GaugeFrame->MaterialOn();
}

void CGauge::SetCurGauge(const int& _Gauge) 
{
	m_iCurGauge = _Gauge;

}
void CGauge::SetMaxGauge(const int& _Gauge) 
{
	m_iMaxGauge = _Gauge;
}
void CGauge::SetPos(const Vec2& _Pos)
{
	TRANS->SetPos(_Pos);
}

void CGauge::GaugeUpdate()
{

	if (m_iCurGauge < 0) 
	{
		m_iCurGauge = 0;
	}
	else if(m_iCurGauge > m_iMaxGauge)
	{
		m_iCurGauge = m_iMaxGauge;
	}
	 
	float CurRatio = (float)m_iCurGauge / (float)m_iMaxGauge;
	Vec2 CurGaugeSize = Vec2{ CurRatio*m_MaxGaugeSize.x,m_MaxGaugeSize.y };
	Vec2 CurGaugePivot = Vec2{ (m_MaxGaugeSize - CurGaugeSize).x* -0.5f ,0.0f};
	
	m_Gauge->SetCustomSize(CurGaugeSize);
	m_Gauge->SetRenPivot(CurGaugePivot+m_GaugePivot);
	
}
