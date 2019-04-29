#pragma once
#include <Logic.h>
class CCharResultUI : public CLogic
{
	enum
	{
		NO_LEVELUP,
		LEVELUP,
		MAXSTATE,
	};

private:
	class CBattleUnit*						m_pPairUnit;
	class CExpGauge*						m_pPairGauge;
	const Vec3								m_GaugePivot;

	SPTR<CFixRenderer>						m_BackFrame;
	SPTR<CFixRenderer>						m_CharPortrait;
	SPTR<CFixRenderer>						m_LevelUpRen;
	SPTR<CFontRenderer>						m_CharNameFont;
	SPTR<CFontRenderer>						m_CharLevelFont;

	Vec2									m_InitFrameSize;
	Vec2									m_LevelUpFrameSize;
	float									m_fAccTime;

	UINT									m_UIState;
	bool									m_bFirstLevelUpEvent;
public:
	void Init();
	void Update();

public:
	void SetPos(const Vec3& _Pos);
	void SetPairUnit(CBattleUnit* _Unit);
	CBattleUnit* GetPairUnit();

public:
	void StandBy(const UINT& _AddExp);
	void UIOn();
	void UIOff();

public:
	void LevelUpEvent();
	void LevelUpUpdate();

public:
	const bool IsGaugeUpdateEnd();
public:
	CCharResultUI();
	~CCharResultUI();
};

