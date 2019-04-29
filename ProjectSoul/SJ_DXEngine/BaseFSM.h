#pragma once
#include"GameObject.h"
#include<functional>



class CBaseFSM : public CGameObject
{
private:
	friend class Engine;
	friend class CFSMDriver;

protected:
	std::function<void(const float&)>			m_CurFunction;


protected:
	template<typename T>
	void ChangeCurFunction(T* _pObj, void(T::*_Func)(const float&)) 
	{
		m_CurFunction = std::bind(_Func, _pObj, std::placeholders::_1);
	}


protected:
	virtual void Init() {}
	virtual void Update(const float& _DeltaTime);

protected:
	CBaseFSM();
	~CBaseFSM();
};

