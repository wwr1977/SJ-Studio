#pragma once
#include <Logic.h>
class CExpGauge : public CLogic
{
	friend class CCharResultUI;
	friend class CBattleResultWindow;

	enum 
	{
		GAUGE_WAIT,
		GAUGE_UPDATE,
		GAUGE_UPDATE_END,
		MAXGAUGESTATE,
	};

	static bool						bGaugeUpdate;
public:
	static void StartGaugeUpdate();
	static void InitGaugeUpdate();
private:
	class CBattleUnit*						m_pPairUnit;
	class CCharResultUI*					m_pPairUI;
	SPTR<CFixRenderer>						m_GaugeFrameRen;
	SPTR<CFixRenderer>						m_GaugeRen;
	SPTR<CFixRenderer>						m_LvUpArrow;
	SPTR<CFontRenderer>						m_ExpFont;
	SPTR<CFontRenderer>						m_XpFont;
	SPTR<CFontRenderer>						m_AddXpFont;

	int										m_iPrevGauge;
	int										m_iCurGauge;
	int										m_iMaxGauge;
	Vec2									m_MaxGaugeSize;
	Vec2									m_GaugePivot;

	float									m_fAccTime;
	UINT									m_GaugeState;

	UINT									m_InitExp;
	UINT									m_TotalAddExp;
	UINT									m_PrevCalExp;
	//bool									temp;
public:
	void Init();
	void Update();
	void PostUpdate();

public:
	void SetPairResultUI(CCharResultUI* _PairUI);
	void SetPos(const Vec3& _Pos);

public:
	void StandBy(const UINT& _AddExp);
	void GaugeOn();
	void GaugeOff();
	void GaugeUpdate();

public:
	void SetCurGauge(const int& _Gauge);
	void SetMaxGauge(const int& _Gauge);

public:
	void LevelUpEvent();

public:
	void WaitStateFunc();
	void UpdateStateFunc();

	bool IsGaugeUpdateEnd();

public:
	CExpGauge();
	~CExpGauge();
};

