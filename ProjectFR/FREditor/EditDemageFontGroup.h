#pragma once
#include <Logic.h>

class CEditBattleUnit;
class CSkillMgr;
class CEditDemageFont;
class CEditDemageFontGroup : public CLogic
{
	static CSkillMgr*				DFGSkillMgr;
private:
	UINT							m_RenderingStyle;
	UINT							m_Option;
	
	// 그룹내의 셋팅된 Font의 갯수
	UINT							m_CurFontCount;
	list<SPTR<CEditDemageFont>>			m_FontList;
							
public:
	static void SetSkillMgr(CSkillMgr* _TurnMgr);

public:
	void Init();
	void Update();

public:
	CEditDemageFontGroup* CreateFontGroup( SPTR<CEditBattleUnit> _Caster,SPTR<CEditBattleUnit> _Target, LEVELDATA* _LevelData);
	POINT CalDemage(SPTR<CEditBattleUnit> _Caster, SPTR<CEditBattleUnit> _Target);
	bool IsCritical(SPTR<CEditBattleUnit> _Caster, SPTR<CEditBattleUnit> _Target);

	void CreateExpList(SPTR<CEditBattleUnit> _Target, LEVELDATA* _LevelData,const POINT& _Demage,const bool& _Critical);
	void CreateStairList(SPTR<CEditBattleUnit> _Target, LEVELDATA* _LevelData, const POINT& _Demage, const bool& _Critical);

	Vec3 GetStairDir(const int& _Option);

	void GroupFontReturn();
	
	bool DemageFontOn();
	bool IsEmpty();

public:
	CEditDemageFontGroup();
	~CEditDemageFontGroup();
};

