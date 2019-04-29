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
	
	// �׷� �����̳� �ȿ����� �ڽ��� �ε���
	UINT							m_CurGroupIndex;
	// �׷쳻�� ���õ� Font�� ����
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

