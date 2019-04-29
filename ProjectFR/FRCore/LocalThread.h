#pragma once
#include "GameThread.h"
template<typename T>
class CLocalThread : public CGameThread
{
private:
	function<void(void* _Arg)>     m_LocalFunc;


public:
	template<typename T>
	void SetLocalFunc(T* _pObj, void(T::*pFunc)(void* _Arg)) 
	{
		m_LocalFunc = bind(pFunc, _pObj, placeholders::_1);
	}

public:
	CLocalThread();
	~CLocalThread();
};

