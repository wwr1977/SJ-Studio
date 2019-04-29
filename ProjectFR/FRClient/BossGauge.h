#pragma once
#include <Logic.h>
class CBossGauge : public CLogic
{
	enum 
	{
		FADE_IN,
		FADE_OUT,
		MAXFADE
	}m_CurFade;
private:
	SPTR<CFixRenderer>					m_GaugeBack;
	SPTR<CFixRenderer>					m_GaugeFrame;
	SPTR<CFixRenderer>					m_Gauge;
	SPTR<CGameSprite>					m_GaugeSprite;
	VERTEX								m_CurVtx[4];

	class CBattleUnit*					m_PairUnit;
	int									m_iCurHp;
	int									m_iMaxHp;

	float								m_fAccTime;
	float								m_fFadeTime;
	bool								m_bFadeUpdate;
public:
	void Init();
	void Update();

public:
	void FadeInUpdate();
	void FadeOutUpdate();
	void FadeEvent(const bool& _FadeIn);

public:
	void StandBy();

public:
	void InitVtx();
	void SetPairUnit(CBattleUnit* _Unit);
	void SetAlpha(const float& _Alpha);
public:
	void UpdateHp(const int& _CurHp);
	void UpdateGauge();
	void SetMaxHp(const int& _MaxHp);


public:
	CBossGauge();
	~CBossGauge();
};

