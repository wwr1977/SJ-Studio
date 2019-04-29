#pragma once
#include "ObjBase.h"

class CGameThread : public CObjBase
{
private:
	HANDLE				m_ThreadHandle;

public:
	HANDLE GetHandle() 
	{
		return m_ThreadHandle;
	}

public:
	BOOL CreateThread();

	virtual void Run() = 0;

	static unsigned int __stdcall ThreadProc(void* _Arg);

public:
	CGameThread();
	~CGameThread();
};

