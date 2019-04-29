#pragma once
#include "MenuBase.h"

class CRootMenu : public CMenuBase
{
private:
	SPTR<CFixRenderer>						m_MainBackRen;
	SPTR<CFixRenderer>						m_SubBackRen;
	SPTR<CFixRenderer>						m_GoldBackRen;
	
	vector<SPTR<class CMenuTab>>			m_vecRootTab;
	int										m_CurTabIndex;

	vector<Vec3>							m_vecUIPosition;
public:
	void Init();
	void Update();
	void MenuOnEvent();
public:
	void PrevTab();
	void NextTab();

public:
	CRootMenu();
	~CRootMenu();
};

