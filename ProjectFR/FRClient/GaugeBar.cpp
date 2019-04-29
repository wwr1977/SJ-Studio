#include "Precom.h"
#include "GaugeBar.h"


CGaugeBar::CGaugeBar()
	:m_Back(nullptr), m_Gauge(nullptr), m_GaugeFrame(nullptr)
	, m_iMaxGauge(0), m_iPrevGauge(-1), m_iCurGauge(-1)
	, m_MaxGaugeSize(Vec2{ 0.0f,0.0f }), m_GaugePivot(Vec2{ 0.0f,0.0f })
	, m_pUnitStatusBar(nullptr), m_GaugeFont(nullptr)
	, m_FontPivot(Vec3(0.0f,0.0f,0.0f))
	,m_ColorUpdateFunc(nullptr)
{

}


CGaugeBar::~CGaugeBar()
{
}
void CGaugeBar::Init()
{
	m_Back = ACTOR->CreateCom<CFixRenderer>(RENDATA(LAYER_UIBACK, 0.7f, true));
	m_Gauge = ACTOR->CreateCom<CFixRenderer>(RENDATA(LAYER_UI, 0.9f, true));
	m_GaugeFrame = ACTOR->CreateCom<CFixRenderer>(RENDATA(LAYER_UI, 1.0f, true));

	m_Gauge->SetSprite(_T("ColorBase"));
	//m_Back->SetSprite(_T("ColorBase"));
	//m_Back->SetMaterial(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));
	//m_Back->MaterialOn();

	m_GaugeFont = ACTOR->CreateCom<CFontRenderer>(RENDATA(LAYER_UI, 1.0f, true));
}
void CGaugeBar::InitData( CUnitStatusBar* _ParentBar)
{
	assert(_ParentBar);

	m_pUnitStatusBar = _ParentBar;
}
void CGaugeBar::Update()
{
	if (m_iPrevGauge == m_iCurGauge)
	{
		return;
	}

	GaugeUpdate();

	if (nullptr != m_ColorUpdateFunc) 
	{
		m_ColorUpdateFunc();
	}

	TCHAR Gauge[64] = _T("");
	_stprintf_s(Gauge, _T("%d/%d"), m_iCurGauge, m_iMaxGauge);
	m_GaugeFont->SetString(Gauge);
	m_GaugeFont->SetFontMag(1.3f);
	

	m_iPrevGauge = m_iCurGauge;
}
void CGaugeBar::CompulsionUpdate()
{
	GaugeUpdate();

	if (nullptr != m_ColorUpdateFunc)
	{
		m_ColorUpdateFunc();
	}

	TCHAR Gauge[64] = _T("");
	_stprintf_s(Gauge, _T("%d/%d"), m_iCurGauge, m_iMaxGauge);
	m_GaugeFont->SetString(Gauge);
	m_GaugeFont->SetFontMag(1.3f);


	m_iPrevGauge = m_iCurGauge;
}

void CGaugeBar::SetGaugeBack(const tstring& _SpriteKey, const Vec2& _RenPivot /*= Vec2{ 0.0f,0.0f }*/)
{
	m_Back->SetSprite(_SpriteKey);
	m_Back->SetRenPivot(_RenPivot);

}
void CGaugeBar::SetGaugeFrame(const tstring& _SpriteKey, const Vec2& _RenPivot/* = Vec2{ 0.0f,0.0f }*/)
{
	m_GaugeFrame->SetSprite(_SpriteKey);
	m_GaugeFrame->SetRenPivot(_RenPivot);
}
void CGaugeBar::SetBackSize(const Vec2& _Size)
{
	m_Back->SetCustomSize(_Size);
}
void CGaugeBar::SetGaugeSize(const Vec2& _Size)
{
	m_Gauge->SetCustomSize(_Size);
	m_MaxGaugeSize = _Size;
}
void CGaugeBar::SetFrameSize(const  Vec2& _Size)
{
	m_GaugeFrame->SetCustomSize(_Size);
}
void CGaugeBar::SetBackPivot(const Vec2& _Pivot)
{
	m_Back->SetRenPivot(_Pivot);
}
void CGaugeBar::SetGaugePivot(const Vec2& _Pivot)
{
	m_Gauge->SetRenPivot(_Pivot);
	m_GaugePivot = _Pivot;
}
void CGaugeBar::SetFramePivot(const Vec2& _Pivot)
{
	m_GaugeFrame->SetRenPivot(_Pivot);
}
void CGaugeBar::SetBackColor(D3DXCOLOR _Color)
{
	m_Back->SetMaterial(_Color);
	m_Back->MaterialOn();
}
void CGaugeBar::SetGaugeColor(D3DXCOLOR _Color)
{
	m_Gauge->SetMaterial(_Color);
	m_Gauge->MaterialOn();
}
void CGaugeBar::SetFrameColor(D3DXCOLOR _Color)
{
	m_GaugeFrame->SetMaterial(_Color);
	m_GaugeFrame->MaterialOn();
}

void CGaugeBar::SetCurGauge(const int& _Gauge)
{
	m_iCurGauge = _Gauge;

}
void CGaugeBar::SetMaxGauge(const int& _Gauge)
{
	m_iMaxGauge = _Gauge;
}
void CGaugeBar::SetPos(const Vec2& _Pos)
{
	TRANS->SetPos(_Pos);
}
void CGaugeBar::SetPivot(const Vec2& _Pivot)
{
	SetBackPivot(_Pivot);
	SetGaugePivot(_Pivot);
	SetFramePivot(_Pivot);
	m_GaugeFont->SetMiddleStartPivot(Vec3(m_FontPivot + Vec3{ _Pivot.x,_Pivot.y,0.0f}));
}
void CGaugeBar::GaugeUpdate()
{

	if (m_iCurGauge < 0)
	{
		m_iCurGauge = 0;
	}
	else if (m_iCurGauge > m_iMaxGauge)
	{
		m_iCurGauge = m_iMaxGauge;
	}

	float CurRatio = (float)m_iCurGauge / (float)m_iMaxGauge;
	Vec2 CurGaugeSize = Vec2{ CurRatio*m_MaxGaugeSize.x,m_MaxGaugeSize.y };
	Vec2 CurGaugePivot = Vec2{ (m_MaxGaugeSize - CurGaugeSize).x* -0.5f ,0.0f };

	m_Gauge->SetCustomSize(CurGaugeSize);
	m_Gauge->SetRenPivot(CurGaugePivot + m_GaugePivot);


}
void CGaugeBar::Reset()
{
	//m_iCurGauge = m_iMaxGauge;
	GaugeUpdate();
}
void CGaugeBar::SetFontPivot(const Vec3& _Pivot)
{
	m_FontPivot = _Pivot;
}
void CGaugeBar::SetAlpha(const float& _AlphaRatio)
{
	m_GaugeFont->SetAlpha(1.f*_AlphaRatio);
	m_Back->SetAlpha(0.7f*_AlphaRatio);
	m_Gauge->SetAlpha(0.9f*_AlphaRatio);
	m_GaugeFrame->SetAlpha(1.f*_AlphaRatio);
}
void CGaugeBar::SetHpColorUpdateFunc()
{
	SetColorUpdateFunc(this, &CGaugeBar::HpColorUpdate);
}
void CGaugeBar::HpColorUpdate()
{
	if ((float)m_iCurGauge / (float)m_iMaxGauge <= 0.3f) 
	{
		SetGaugeColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
	}
	else 
	{
		SetGaugeColor(D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));
	}
}
void CGaugeBar::InvalidatePrevGauge()
{
	m_iPrevGauge = 0xFFFFFFFF;
}

