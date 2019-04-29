#pragma once
#include <Logic.h>
class CStdGauge : public CLogic
{
private:
	SPTR<CFixRenderer>						m_GaugeFrameRen;
	SPTR<CFixRenderer>						m_FrontFrameRen;

	SPTR<CFixRenderer>						m_GaugeRen;
	SPTR<CFontRenderer>						m_GaugeFontRen;
	vector<SPTR<CFontRenderer>>				m_vecGaugeFont;
	
	
	bool									m_bUpdate;
	int										m_iCurGauge;
	int										m_iMaxGauge;
	Vec2									m_MaxGaugeSize;
	Vec2									m_GaugePivot;

public:
	void Init();
	void Update();
	void GaugeUpdate();


public:
	const size_t CreateGaugeFont(const tstring& _Style, const tstring _String = _T(""), const float _FontMag = 1.3f);
	const size_t CreateGaugeFont(const tstring& _Style,const POINT StartPivot
		,const FONTPIVOTOPTION _StartOp = LEFTSTART
		, const tstring _String = _T("")
		, const float _FontMag = 1.3f);

public:
	void GaugeOn();
	void GaugeOff();

public:
	void SetPos(const Vec3& _Pos);
	void SetMag(const float& _Mag);
	void SetGaugeFrame(const tstring& _SpriteKey, const Vec2& _RenPivot = Vec2{ 0.0f,0.0f });
	void CreateGaugeFrontFrame(const tstring& _SpriteKey, const Vec2& _RenPivot = Vec2{ 0.0f,0.0f });
	void SetFrameSize(const Vec2& _Size);
	void SetGaugeSize(const Vec2& _Size);
	void SetGaugePivot(const Vec2& _Pivot);
	void SetCurGauge(const int& _Gauge);
	void SetGaugeColor(const D3DXCOLOR& _Color);
	void SetMaxGauge(const int& _Gauge);
	void SetGaugeFont(const size_t& _FontIndex);
	void SetFontPivot(const size_t& _FontIndex, const POINT& _Pivot,const FONTPIVOTOPTION& _Op = LEFTSTART);
	void SetFontPivot(const size_t& _FontIndex, const Vec3& _MidPivot);
	void SetFontString(const size_t& _FontIndex, const tstring& _String);
	void SetFontStyle(const size_t& _FontIndex, const tstring& _Style);
	void SetFontColor(const size_t& _FontIndex, const DWORD& _Color);
	void SetFontMag(const size_t& _FontIndex, const float& _Mag);

public:
	CStdGauge();
	~CStdGauge();
};

