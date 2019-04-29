#pragma once
#include "GameThread.h"

template<typename T>
class CGlobalThread : public CGameThread
{
private:
	void(*m_Func)(T*);
	T* m_pData;

public:
	void SetData(T* _Data) 
	{
		m_pData = _Data;
	}
	void SetFunc(void(*_Func)(T*)) 
	{
		assert(_Func);
		m_Func = _Func;
	}

	static CGlobalThread<T>* CreateGlobalthread(void(*_Func)(T*), T* _Data) 
	{
		assert(_Func);

		return CGlobalThread(_ThreadName,_Func, _Data);
	}
	
public:
	void Run() 
	{
		m_Func(m_pData);
	}

public:
	CGlobalThread() {}
	CGlobalThread(void(*_Func)(T*),T* _Data)
	:m_Func(_Func), m_pData(_Data)
	{
	}
	~CGlobalThread() {}
};

