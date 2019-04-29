#pragma once
#include <Logic.h>

class CUnitStatusBar;
class CGaugeBar : public CLogic
{

private:
	CUnitStatusBar*						m_pUnitStatusBar;
	SPTR<CFontRenderer>					m_GaugeFont;
	SPTR<CFixRenderer>					m_Back;
	SPTR<CFixRenderer>					m_Gauge;
	SPTR<CFixRenderer>					m_GaugeFrame;

	Vec2								m_GaugePivot;
	Vec2								m_MaxGaugeSize;
	int									m_iCurGauge;
	int									m_iPrevGauge;
	int									m_iMaxGauge;

	Vec3								m_FontPivot;

	
	function<void(void)>				m_ColorUpdateFunc;

private:
	template<typename T>
	void SetColorUpdateFunc(T* _Obj, void(T::*_pFunc)(void))
	{
		if (nullptr == _pFunc || nullptr == _Obj)
		{
			assert(nullptr);
			return;
		}
		m_ColorUpdateFunc = bind(_pFunc, _Obj);
	}


public:
	virtual void InitData(CUnitStatusBar* _ParentBar);
	
	void Init();
	void Update();
	void CompulsionUpdate();

public:
	void SetGaugeBack(const tstring& _SpriteKey, const Vec2& _RenPivot = Vec2{ 0.0f,0.0f });
	void SetGaugeFrame(const tstring& _SpriteKey, const Vec2& _RenPivot = Vec2{ 0.0f,0.0f });
	void SetBackSize(const Vec2& _Size);
	void SetGaugeSize(const Vec2& _Size);
	void SetFrameSize(const  Vec2& _Size);
	void SetBackPivot(const Vec2& _Pivot);
	void SetGaugePivot(const Vec2& _Pivot);
	void SetFramePivot(const Vec2& _Pivot);
	void SetBackColor(D3DXCOLOR _Color);
	void SetGaugeColor(D3DXCOLOR _Color);
	void SetFrameColor(D3DXCOLOR _Color);

	void SetCurGauge(const int& _Gauge);
	void SetMaxGauge(const int& _Gauge);

	void SetPos(const Vec2& _Pos);
	void SetPivot(const Vec2& _Pivot);
	void GaugeUpdate();

	void Reset();
	void SetFontPivot(const Vec3& _Pivot);
	void SetAlpha(const float& _AlphaRatio);

	void SetHpColorUpdateFunc();
	void HpColorUpdate();
	void InvalidatePrevGauge();

public:
	CGaugeBar();
	~CGaugeBar();
};

