#include "stdafx.h"
#include "StateMgr.h"


CStateMgr::CStateMgr()
{
}


CStateMgr::~CStateMgr()
{
	m_vecState.clear();
}

void CStateMgr::Update()
{
	m_CurState->Update();
}
bool CStateMgr::ChangeState(int _Index)
{
	if (_Index < 0 || _Index >= (int)m_vecState.size()) 
	{
		return FALSE;
	}

	m_CurState = m_vecState[_Index];
	m_iCurStateIndex = _Index;
	m_CurState->StateReset();
	return TRUE;
}
int CStateMgr::GetCurStateIndex()
{
	return m_iCurStateIndex;
}