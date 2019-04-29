#pragma once
#include "GlobalThread.h"

template<typename T>
class CCriSectionObject 
{
private:
	static bool m_bInit;
	static CRITICAL_SECTION CS;
public:
	CCriSectionObject()
	{
		if (nullptr == CS.DebugInfo) 
		{
			InitializeCriticalSection(&CS);
			m_bInit = true;
		}
		EnterCriticalSection(&CS);
	}
	~CCriSectionObject()
	{
		LeaveCriticalSection(&CS);
	}
};

template<typename T>
typename bool CCriSectionObject<T>::m_bInit = false;

template<typename T>
typename CRITICAL_SECTION CCriSectionObject<T>::CS;


class CThreadMgr
{
private:
	static map<HANDLE, SPTR<CGameThread>>						m_mapGlobalThread;

public:
	static void Update();
	static bool IsEmpty();
	
public:
	template<typename T>
	static HANDLE CreateGlobvalThread(void(*_Func)(T*), T* _Data = nullptr) 
	{
		CGlobalThread<T>* pThread = new CGlobalThread<T>();
		pThread->SetData(_Data);
		pThread->SetFunc(_Func);

		if (false == pThread->CreateThread())
		{
			SAFE_DELETE(pThread);
			return false;
		}

		m_mapGlobalThread.insert(map<HANDLE, SPTR<CGameThread>>::value_type(pThread->GetHandle(), pThread));
		return pThread->GetHandle();
	
	}

	

public:
	CThreadMgr() {}
	~CThreadMgr() {}
};

