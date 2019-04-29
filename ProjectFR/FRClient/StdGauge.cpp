#include "Precom.h"
#include "StdGauge.h"


CStdGauge::CStdGauge()
	: m_iCurGauge(0), m_iMaxGauge(0)
	, m_MaxGaugeSize(Vec2{ 0.f,0.f }), m_GaugePivot(Vec2{0.f,0.f})
	, m_GaugeRen(nullptr), m_GaugeFrameRen(nullptr)
	, m_bUpdate(false), m_GaugeFontRen(nullptr)
	, m_FrontFrameRen(nullptr)
{
	m_vecGaugeFont.clear();
}


CStdGauge::~CStdGauge()
{
	m_vecGaugeFont.clear();
}
void CStdGauge::Init() 
{
	m_GaugeFrameRen = ACTOR->CreateCom<CFixRenderer>(RENDATA(LAYER_UIBACK, 0.9f, true));
	m_GaugeRen = ACTOR->CreateCom<CFixRenderer>(RENDATA(LAYER_UI, 0.9f, true));
	m_GaugeRen->SetSprite(_T("ColorBase"));

}
void CStdGauge::Update() 
{
	if (false == m_bUpdate)
		return;

	GaugeUpdate();
}
void CStdGauge::GaugeUpdate()
{
	if (m_iCurGauge < 0)
		m_iCurGauge = 0;

	else if (m_iCurGauge > m_iMaxGauge)
		m_iCurGauge = m_iMaxGauge;

	float CurRatio = (float)m_iCurGauge / (float)m_iMaxGauge;

	Vec2 CurGaugeSize = Vec2{ CurRatio*m_MaxGaugeSize.x,m_MaxGaugeSize.y };
	Vec2 CurGaugePivot = Vec2{ (m_MaxGaugeSize - CurGaugeSize).x* -0.5f ,0.0f };

	m_GaugeRen->SetCustomSize(CurGaugeSize);
	m_GaugeRen->SetRenPivot(CurGaugePivot + m_GaugePivot);

	if (nullptr == m_GaugeFontRen)
		return;

	TCHAR Gauge[64] = _T("");
	_stprintf_s(Gauge, _T("%6d/%6d"), m_iCurGauge, m_iMaxGauge);
	m_GaugeFontRen->SetString(Gauge);
	m_GaugeFontRen->SetFontMag(1.3f);
}
const size_t CStdGauge::CreateGaugeFont(const tstring& _Style, const tstring _String /*= _T("")*/, const float _FontMag /*= 1.3f*/)
{
	size_t NewIndex = m_vecGaugeFont.size();

	m_vecGaugeFont.push_back(ACTOR->CreateCom<CFontRenderer>(RENDATA(LAYER_UI, 0.8f, true)));
	m_vecGaugeFont[NewIndex]->SetString(_String);
	m_vecGaugeFont[NewIndex]->SetFontStyle(_Style);
	m_vecGaugeFont[NewIndex]->SetFontMag(_FontMag);
	m_vecGaugeFont[NewIndex]->SetMiddleStartPivot(Vec3{ 0.f,0.f,0.f });
	return NewIndex;

}
const size_t CStdGauge::CreateGaugeFont(const tstring& _Style, const POINT StartPivot
	, const FONTPIVOTOPTION _StartOp /*= LEFTSTART*/
	, const tstring _String /*= _T("")*/
	, const float _FontMag /*= 1.3f*/) 
{
	size_t NewIndex = m_vecGaugeFont.size();

	m_vecGaugeFont.push_back(ACTOR->CreateCom<CFontRenderer>(RENDATA(LAYER_UI, 0.8f, true)));
	m_vecGaugeFont[NewIndex]->SetString(_String);
	m_vecGaugeFont[NewIndex]->SetFontStyle(_Style);
	m_vecGaugeFont[NewIndex]->SetFontMag(_FontMag);
	
	switch (_StartOp)
	{
	case LEFTSTART:
		m_vecGaugeFont[NewIndex]->SetLeftStartPivot(StartPivot);
		break;
	case RIGHTSTART:
		m_vecGaugeFont[NewIndex]->SetRightStartPivot(StartPivot);
		break;
	}

	return NewIndex;
}
void CStdGauge::GaugeOn() 
{
	ACTOR->On();
}
void CStdGauge::GaugeOff() 
{
	ACTOR->Off();
}
void CStdGauge::SetGaugeFrame(const tstring& _SpriteKey, const Vec2& _RenPivot) 
{
	if (nullptr == m_GaugeFrameRen)
		return;

	m_GaugeFrameRen->SetSprite(_SpriteKey);
	m_GaugeFrameRen->SetRenPivot(_RenPivot);

}
void CStdGauge::CreateGaugeFrontFrame(const tstring& _SpriteKey, const Vec2& _RenPivot /*= Vec2{ 0.0f,0.0f }*/)
{
	if (nullptr == m_FrontFrameRen)
		m_FrontFrameRen = ACTOR->CreateCom<CFixRenderer>(RENDATA(LAYER_UI, 0.9f, true));
	
	m_FrontFrameRen->SetSprite(_SpriteKey);
	m_FrontFrameRen->SetRenPivot(_RenPivot);

	m_GaugeFrameRen->Off();

}
void CStdGauge::SetPos(const Vec3& _Pos)
{
	ACTOR->SetPos(_Pos);
}
void CStdGauge::SetMag(const float& _Mag)
{
	ACTOR->SetImageScale(_Mag);
}
void CStdGauge::SetFrameSize(const Vec2& _Size)
{
	if(nullptr != m_GaugeFrameRen)
		m_GaugeFrameRen->SetCustomSize(_Size);

	if (nullptr != m_FrontFrameRen)
		m_FrontFrameRen->SetCustomSize(_Size);
}
void CStdGauge::SetGaugeSize(const Vec2& _Size) 
{
	m_GaugeRen->SetCustomSize(_Size);
	m_MaxGaugeSize = _Size;
}
void CStdGauge::SetGaugePivot(const Vec2& _Pivot)
{
	if (nullptr == m_GaugeRen)
		return;

	m_GaugeRen->SetRenPivot(_Pivot);
}
void CStdGauge::SetCurGauge(const int& _Gauge)
{
	m_iCurGauge = _Gauge;
	m_bUpdate = true;
}
void CStdGauge::SetGaugeColor(const D3DXCOLOR& _Color)
{
	if (nullptr == m_GaugeRen)
		return;

	m_GaugeRen->SetMaterial(_Color);
	m_GaugeRen->MaterialOn();
}
void CStdGauge::SetMaxGauge(const int& _Gauge)
{
	m_iMaxGauge = _Gauge;
	m_bUpdate = true;
}
void CStdGauge::SetGaugeFont(const size_t& _FontIndex)
{
	if (_FontIndex >= m_vecGaugeFont.size() || nullptr == m_vecGaugeFont[_FontIndex])
		return;

	m_GaugeFontRen = m_vecGaugeFont[_FontIndex];
}
void CStdGauge::SetFontPivot(const size_t& _FontIndex, const POINT& _LeftPivot, const FONTPIVOTOPTION& _Op /*= LEFTSTART*/)
{
	if (_FontIndex >= m_vecGaugeFont.size() || nullptr == m_vecGaugeFont[_FontIndex])
		return;
	
	switch (_Op)
	{
	case LEFTSTART:
		m_vecGaugeFont[_FontIndex]->SetLeftStartPivot(_LeftPivot);
		return;
	case RIGHTSTART:
		m_vecGaugeFont[_FontIndex]->SetRightStartPivot(_LeftPivot);
		return;
	}
}
void CStdGauge::SetFontPivot(const size_t& _FontIndex, const Vec3& _MidPivot) 
{
	if (_FontIndex >= m_vecGaugeFont.size() || nullptr == m_vecGaugeFont[_FontIndex])
		return;

	m_vecGaugeFont[_FontIndex]->SetMiddleStartPivot(_MidPivot);
}
void CStdGauge::SetFontString(const size_t& _FontIndex, const tstring& _String) 
{
	if (_FontIndex >= m_vecGaugeFont.size() || nullptr == m_vecGaugeFont[_FontIndex])
		return;

	m_vecGaugeFont[_FontIndex]->SetString(_String);
}
void CStdGauge::SetFontStyle(const size_t& _FontIndex, const tstring& _Style) 
{
	if (_FontIndex >= m_vecGaugeFont.size() || nullptr == m_vecGaugeFont[_FontIndex])
		return;

	m_vecGaugeFont[_FontIndex]->SetFontStyle(_Style);
}
void CStdGauge::SetFontColor(const size_t& _FontIndex, const DWORD& _Color) 
{
	if (_FontIndex >= m_vecGaugeFont.size() || nullptr == m_vecGaugeFont[_FontIndex])
		return;

	m_vecGaugeFont[_FontIndex]->SetFontColor(_Color);
}
void CStdGauge::SetFontMag(const size_t& _FontIndex, const float& _Mag)
{
	if (_FontIndex >= m_vecGaugeFont.size() || nullptr == m_vecGaugeFont[_FontIndex])
		return;

	m_vecGaugeFont[_FontIndex]->SetFontMag(_Mag);
}