#pragma once
#include <list>

template<typename T>
class SPTR 
{
public:
	T*		p;

public:
	void ChangePointer(T* _p = nullptr) 
	{
		if (nullptr != p) 
		{
			p->ReleaseRef();
		}

		p = _p;

		if (nullptr != p) 
		{
			p->AddRefCount();
		}
	}

public:
	operator void*()
	{
		return (void*)p;
	}

	operator int()
	{
		return (int)p;
	}

	operator bool()
	{
		if (nullptr == p)
		{
			return false;
		}

		return true;
	}

	T& operator*()
	{
		return (*p);
	}
	
	T* operator->()
	{
		return p;
	}
	
	bool operator==(const SPTR<T>& _Other)
	{
		return p == _Other.p;
	}

	SPTR<T>& operator=(T* _p)
	{
		ChangePointer(_p);
		return *this;
	}
	SPTR<T>& operator=(const SPTR<T>& _Other)
	{
		ChangePointer(_Other.p);
		return *this;
	}
	

	SPTR<T>& operator=(nullptr_t _null) 
	{
		ChangePointer();
		return *this;
	}



public:
	SPTR()
		: p(nullptr)
	{

	}
	SPTR(T* _p  )
		:p(nullptr)
	{
		ChangePointer(_p);
	}
	SPTR(const SPTR<T>& _Other)
		:p(nullptr)
	{
		ChangePointer(_Other.p);
	}

	SPTR(const nullptr_t _null) 
		:p(nullptr)
	{
	
	}
	~SPTR()
	{
		if (nullptr != p)
		{
			p->ReleaseRef();
		}
	}


};

template<typename T>
bool operator==(const SPTR<T>& _Left, T* _Right) 
{
	return _Left.p == _Right;
}

template<typename T>
bool operator==(T* _Left,const SPTR<T>& _Right)
{
	return _Left == _Right.p;
}

template<typename T>
bool operator==(const SPTR<T>& _Left, const nullptr_t& _Right)
{
	return _Left.p == _Right;
}

template<typename T>
bool operator==(const nullptr_t& _Left, const SPTR<T>& _Right)
{
	return _Left == _Right.p;
}

template<typename T>
bool operator!=(const SPTR<T>& _Left, T* _Right)
{
	return _Left.p != _Right;
}

template<typename T>
bool operator!=(T* _Left, const SPTR<T>& _Right)
{
	return _Left != _Right.p;
}

template<typename T>
bool operator!=(const SPTR<T>& _Left, const nullptr_t& _Right)
{
	return _Left.p != _Right;
}

template<typename T>
bool operator!=(const nullptr_t& _Left, const SPTR<T>& _Right)
{
	return _Left != _Right.p;
}


template<typename T>
bool operator>(const SPTR<T>& _Left, const  SPTR<T>& _Right)
{
	return _Left.p > _Right.p;
}

template<typename T>
bool operator>=(const SPTR<T>& _Left, const  SPTR<T>& _Right)
{
	return _Left.p >= _Right.p;
}

template<typename T>
bool operator<(const SPTR<T>& _Left, const  SPTR<T>& _Right)
{
	return _Left.p < _Right.p;
}

template<typename T>
bool operator<=(const SPTR<T>& _Left, const  SPTR<T>& _Right)
{
	return _Left.p <= _Right.p;
}

