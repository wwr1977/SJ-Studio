#pragma once
#include "ObjBase.h"

class CDX9Core;
class CWindowMgr;
class CTimerMgr
{

public:
	class CTimer : public CObjBase 
	{
	public:
		friend CTimerMgr;

	private:
		LARGE_INTEGER			m_ClockFreg;
		LARGE_INTEGER			m_PrevCount;
		LARGE_INTEGER			m_CurCount;
		float					m_fDeltaTime;


	public:
		bool  Init();
		float Update();
		float DeltaTime() { return m_fDeltaTime; }

	public:
		CTimer(tstring _Name = _T(""));
		~CTimer();
	};
	// n초 마다 true를 반환하는 타이머
	class CPulse : public CObjBase 
	{
	public:
		friend CTimerMgr;

	private:
		bool								m_bPulse;
		float								m_fAccTime;
		float								m_fPulseTime;

	public:
		void Update();
		bool CheckPulse();

	public:
		CPulse(const float& _PulseTime);
		~CPulse();

	};



public:
	friend CWindowMgr;
	friend CDX9Core;

private:
	static float									m_fFPS;
	static float									m_fFPSCalTime;
	static float									m_fFPSCount;


	static CTimer									m_MainTimer;
	static map<tstring, SPTR<CTimer>>				m_mapTimer;
	static map<float, SPTR<CPulse>>					m_mapPulse;

private:
	static bool Init();
	static bool Update();
	static bool PulseUpdate();
	static bool Terminal();

public:
	static float DeltaTime() { return m_MainTimer.DeltaTime();}
	static float FPS() { return m_fFPS; }

public:
	static bool CreateTimer(tstring _Name);
	static SPTR<CTimer> FindTimer(tstring _Name);
	static CPulse* CreatePulse(const float& _PulseTime);
	static SPTR<CPulse> FindPulse(const float& _PulseTime);
	static bool Pulse(const float& _PulseTime);
private:
	CTimerMgr() {}
	~CTimerMgr() {}
};

