#pragma once
#include"UserHeader.h"

class CObjBase
{
private:
	tstring							m_Name;
	const type_info*				m_TypeInfo;
	int								m_iRefCount;
	bool							m_bDeath;
	bool							m_bActive;
	

public:
	void Name(tstring _Name) { m_Name = _Name; }
	tstring Name() { return m_Name; }
	const TCHAR* Name_str() const { return m_Name.c_str(); }
	void Active(bool _Value) { m_bActive = _Value; }
	void On() { m_bActive = true; }
	void Off() { m_bActive = false; }
	virtual void Death() { m_bDeath = true; }
	virtual bool IsDeath() { return m_bDeath; }
	bool IsActive() { return m_bActive; }
	void TypeInit() 
	{
		SetTypeInfo(this);
	}
	

	template<typename T>
	void SetTypeInfo(T* _p) 
	{
		m_TypeInfo = &typeid(*_p);
	}

	template<typename T>
	void TypeInfo() 
	{
		m_TypeInfo = &typeid(T);
	}

	template<typename T>
	bool EqualType() 
	{
		return m_TypeInfo == &typeid(T);
	}


	void AddRefCount() 
	{
		++m_iRefCount;
	}
	void ReleaseRef() 
	{
		--m_iRefCount;
		if (m_iRefCount <=0 ) 
		{
			
			delete this;
		}
	}
	void DestroyObject() 
	{
		m_iRefCount = 0;
		/*m_bInvalidate = true;
		delete this;*/
		ReleaseRef();
	}


public:
	CObjBase();
	CObjBase(tstring _Name );
	virtual ~CObjBase() =0;
};

