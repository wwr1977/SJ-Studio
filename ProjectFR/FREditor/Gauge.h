#pragma once
#include<Logic.h>
class CGauge : public CLogic
{
private:
	SPTR<CFixRenderer>					m_Back;
	SPTR<CFixRenderer>					m_Gauge;
	SPTR<CFixRenderer>					m_GaugeFrame;

	Vec2								m_GaugePivot;
	Vec2								m_MaxGaugeSize;
	int									m_iCurGauge;
	int									m_iPrevGauge;
	int									m_iMaxGauge;


public:
	void Init();
	void Update();
	
public:
	void SetGaugeBack(const tstring& _SpriteKey, const Vec2& _RenPivot = Vec2{0.0f,0.0f});
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

	void GaugeUpdate();
	

public:
	CGauge();
	~CGauge();
};

