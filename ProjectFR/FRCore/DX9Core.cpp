#include "stdafx.h"
#include "DX9Core.h"
#include"CoreBuilder.h"

bool CDX9Core::m_bInit=false;
bool CDX9Core::m_bCoreDeath = false;
HINSTANCE CDX9Core::m_hInst=nullptr;
const type_info*	CDX9Core::m_TransComInfo = &typeid(CTransForm);

bool CDX9Core::Init(HINSTANCE _hInst, SPTR<CCoreBuilder> _pBuilder)
{

#ifdef _DEBUG
	int* i = new int();
#endif

	if (nullptr == _pBuilder) 
	{
		return FALSE;
	}

	if (TRUE == m_bInit) 
	{
		return FALSE;
	}

	assert(_hInst);

	m_hInst = _hInst;

	if (nullptr == m_hInst) 
	{
		return FALSE;
	}

	
	CWindowMgr::Inst().Init(m_hInst);
	CTimerMgr::Init();
	CFilePathMgr::Init();
	CResolutionMgr::Init();
	CCollisionFunc::ColFunctionInit();
	CResourceMgr::SoundInit();
	CGameFile::Init();
	
	if (false == _pBuilder->CoreBuild())
	{
		m_bCoreDeath = TRUE;
		return FALSE;
	}

	m_bCoreDeath = FALSE;
	return m_bInit = TRUE;
}
bool CDX9Core::Progress() 
{
	CWindowMgr::Inst().Start();

	return TRUE;
}
bool CDX9Core::ManagerRelease() 
{
	CTimerMgr::Terminal();
	CFilePathMgr::Terminal();
	CResolutionMgr::Terminal();

	return TRUE;
}
bool CDX9Core::ReduceProgress()
{
	CWindowMgr::Inst().ReduceStart();
	return TRUE;
}
const type_info* CDX9Core::TransComInfo() 
{
	return m_TransComInfo;
}