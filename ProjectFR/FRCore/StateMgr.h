#pragma once
#include "ObjBase.h"
//#include"BaseState.h"


class CStateMgr :public CObjBase
{
protected:
	// state index 확인용(인게임에서 사용 X 디버그 용)
	int									m_iCurStateIndex;
	SPTR<CBaseState>					m_CurState;
	vector<SPTR<CBaseState>>			m_vecState;


public:
	void Update();


public:
	template<typename T>
	CBaseState* CreateState(void* _pLogic = nullptr) 
	{
		SPTR<CBaseState> State = FindState<T>();

		if (nullptr != State) 
		{
			return nullptr;
		}

		CBaseState* NewState = new T();
		NewState->TypeInfo<T>();
		NewState->Init(_pLogic);

		m_vecState.push_back(NewState);
		if (1 == m_vecState.size()) 
		{
			m_iCurStateIndex = 0;
			m_CurState = NewState;
		}
		return NewState;
	}

	template<typename T>
	SPTR<CBaseState> FindState() 
	{
		for (size_t i = 0; i < m_vecState.size(); i++)
		{
			if (true == m_vecState[i]->EqualType<T>()) 
			{
				return m_vecState[i];
			}
		}
		return nullptr;
	}
	template<typename T>
	int FindStateIndex()
	{
		for (size_t i = 0; i < m_vecState.size(); i++)
		{
			if (true == m_vecState[i]->EqualType<T>())
			{
				if (i < 0 || i >= m_vecState.size()) 
				{
					return -1;
				}
				return (int)i;
			}
		}
		
		return -1;
	}
	template<typename T>
	bool ChangeState() 
	{
		SPTR<CBaseState> State = FindState<T>();
		int	StateIndex = FindStateIndex<T>();

		if (nullptr == State) 
		{
			return FALSE;
		}
		m_CurState = State;
		m_iCurStateIndex = StateIndex;
		m_CurState->StateReset();
		return TRUE;
	}

public:
	bool ChangeState(int _Index);
	int GetCurStateIndex();


public:
	CStateMgr();
	virtual ~CStateMgr();
};

