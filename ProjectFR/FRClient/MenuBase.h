#pragma once
#include <Logic.h>
class CMenuBase : public CLogic
{
private:


protected:
	class CMainMenuMgr*				m_pMenuMgr;
	bool							m_OnFirstFrame;
	
public:
	void InitData(CMainMenuMgr* _Mgr);
	virtual void MenuUpdate();
	virtual void MenuOnEvent() = 0;
public:
	void MenuOn();
	
	void MenuOff();

public:
	CMenuBase();
	~CMenuBase();
};

