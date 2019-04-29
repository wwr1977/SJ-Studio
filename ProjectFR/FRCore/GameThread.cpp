#include "stdafx.h"
#include "GameThread.h"


CGameThread::CGameThread()
{
}


CGameThread::~CGameThread()
{
	WaitForSingleObject(m_ThreadHandle, INFINITE);
	CloseHandle(m_ThreadHandle);
	m_ThreadHandle = nullptr;
}
BOOL CGameThread::CreateThread() 
{
	m_ThreadHandle = (HANDLE)_beginthreadex(NULL, 0, ThreadProc, this, 0, nullptr);

	if (nullptr == m_ThreadHandle) 
		return FALSE;
	

	return TRUE;
}
unsigned int __stdcall CGameThread::ThreadProc(void* _Arg) 
{
	CGameThread* pThread = (CGameThread*)_Arg;

	pThread->Run();

	pThread->Death();

	return 0;
}