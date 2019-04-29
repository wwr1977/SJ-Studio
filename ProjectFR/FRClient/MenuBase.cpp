#include "Precom.h"
#include "MenuBase.h"


CMenuBase::CMenuBase()
	:m_OnFirstFrame(false)
{
}


CMenuBase::~CMenuBase()
{
}
void CMenuBase::InitData(CMainMenuMgr* _Mgr)
{
	if(nullptr != _Mgr)
		m_pMenuMgr = _Mgr;


	this->Active(false);
}
void CMenuBase::MenuUpdate()
{
	if (true == m_OnFirstFrame) 
	{
		m_OnFirstFrame = false;
		return;
	}

	this->Update();
}

void CMenuBase::MenuOn() 
{
	ACTOR->On();
	m_OnFirstFrame = true;
	
	this->MenuOnEvent();
}
void CMenuBase::MenuOff()
{
	ACTOR->Off();
}