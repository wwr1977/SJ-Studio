#include "stdafx.h"
#include "ThreadMgr.h"

 map<HANDLE, SPTR<CGameThread>>					CThreadMgr::m_mapGlobalThread;

void CThreadMgr::Update()
{
	CCriSectionObject<CThreadMgr> CS;

	map<HANDLE, SPTR<CGameThread>>::iterator m_StartIter = m_mapGlobalThread.begin();
	map<HANDLE, SPTR<CGameThread>>::iterator m_EndIter = m_mapGlobalThread.end();

	for (; m_StartIter != m_EndIter;)
	{
		if (true == m_StartIter->second->IsDeath())
		{
			m_StartIter = m_mapGlobalThread.erase(m_StartIter);
		}
		else
		{
			++m_StartIter;
		}
	}

}
bool CThreadMgr::IsEmpty()
{
	if (0 == m_mapGlobalThread.size()) 
	{
		return true;
	}
	return false;
}