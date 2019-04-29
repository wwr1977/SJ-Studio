#pragma once
#include"UserHeader.h"

class CDX9Core;
class CResolutionMgr
{
public:
	struct Resolution 
	{
	public:
		unsigned long W;
		unsigned long H;

		bool operator==(const Resolution& _Other)const 
		{
			return W == _Other.W && H == _Other.H;
		}
		bool operator!=(const Resolution& _Other)const 
		{
			return W != _Other.W && H != _Other.H;
		}
		bool IsZero() 
		{
			return W == 0 || H == 0;
		}
		RECT ToRect()
		{
			return RECT{ 0,0,(long)W,(long)H };
		}
	};

public:
	friend CDX9Core;

private:
	static  vector<Resolution>		m_vecRes;
	static  Resolution				m_CurRes;

private:
	static bool Init();
	static bool Terminal();
	
public:
	static Resolution FindResolution(Resolution _Resolution);

public:
	CResolutionMgr() {}
	~CResolutionMgr() {}
};

