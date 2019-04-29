#pragma once
#include <Logic.h>


class CNumberUI;
class CGaugeBar;
class CUnitPortrait;
class CBattleUnit;
class CUnitStatusBar : public CLogic
{
	static float				RelocationTime;
private:
	//	Bar 배경
	SPTR<CFixRenderer>			m_BarBack;
	

	//	유닛 초상화
	CBattleUnit*				m_PairUnit;
	SPTR<CFixRenderer>			m_UnitPortraitFrame;
	SPTR<CFixRenderer>			m_UnitPortrait;
	SPTR<CFixRenderer>			m_UnitLevelBox;
	SPTR<CFontRenderer>			m_UnitFontRen;
	Vec3						m_PortraitPivot;

	SPTR<CGaugeBar>				m_HpBar;
	SPTR<CGaugeBar>				m_MpBar;

	
	bool						m_bRelocationEvent;
	float						m_fAccTime;
	Vec3						m_RelocationStartPos;
	Vec3						m_RelocationEndPos;
public:
	void Init();
	void Update();
	void DebugRender();
	void FontRender();
	
public:
	void SetUnit(CBattleUnit* _Unit);
	// SetUnit에 들어온 유닛이 Char or Mon에 따라 다른 모양의 statusbar로 셋팅
	void SettingCharMode();
	void SettingMonMode();

	void StandBy();
	
private:
	void StandByMonsterMode();
	void StandByCharacterMode();
	
public:
	const Vec2 GetBarBackSize();

public:
	void CalPortraitPivot(const Vec2& _FrameSize);

public:
	void UIMove(const Vec3& _Pos);
	void On();
	void Off();
	void SetPos(const Vec3& _Pos);
	void SetAlpha(const float& _AlphaRatio);

	void UpdateHp(const UINT& _HP);
	void UpdateMp(const UINT& _MP);

	void UpdateHp(const UINT& _MaxHP,const UINT& _HP);
	void UpdateMp(const UINT& _MaxMP, const UINT& _MP);

	void RelocationEvent();
	void RelocationUpdate();
	void RelocationChar();
	void RelocationMon();

	void ChangeName(const tstring& _NewName);
public:
	CUnitStatusBar();
	~CUnitStatusBar();
};

