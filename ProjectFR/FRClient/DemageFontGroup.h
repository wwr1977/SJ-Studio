#pragma once
#include <Logic.h>

class CBattleUnit;
class CTurnMgr;
class CDemageFont;
class CDemageFontGroup : public CLogic
{
	static CTurnMgr*				DFGTurnMgr;	
private:
	UINT							m_RenderingStyle;
	UINT							m_Option;
	
	// 그룹 컨테이너 안에서의 자신의 인덱스
	UINT							m_CurGroupIndex;
	// 그룹내의 셋팅된 Font의 갯수
	UINT							m_CurFontCount;
	list<SPTR<CDemageFont>>			m_FontList;
							
public:
	static void SetTurnMgr(CTurnMgr* _TurnMgr);

public:
	void Init();
	void Update();

public:
	CDemageFontGroup* CreateFontGroup( SPTR<CBattleUnit> _Caster,SPTR<CBattleUnit> _Target, LEVELDATA* _LevelData);
	POINT CalDemage(SPTR<CBattleUnit> _Caster, SPTR<CBattleUnit> _Target);
	bool IsCritical(SPTR<CBattleUnit> _Caster, SPTR<CBattleUnit> _Target);

	void CreateExpList(SPTR<CBattleUnit> _Target, LEVELDATA* _LevelData,const POINT& _Demage,const bool& _Critical,const float& _CriMag);
	void CreateStairList(SPTR<CBattleUnit> _Target, LEVELDATA* _LevelData, const POINT& _Demage, const bool& _Critical, const float& _CriMag);

	Vec3 GetStairDir(const int& _Option);

	void GroupFontReturn();
	
	bool DemageFontOn();
	bool IsEmpty();

	void SetGroupIndex(const UINT& _Index);

public:
	CDemageFontGroup();
	~CDemageFontGroup();
};

