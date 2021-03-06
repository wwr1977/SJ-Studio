#pragma once
#include"SJ_DXCore.h"
#include"GameObject.h"
#include<map>

class CTimerMgr 
{
public:
	class CTimer : public  CGameObject
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
		CTimer();
		~CTimer();
	};

	// n초 마다 true를 반환하는 타이머
	class CPulse : public CGameObject
	{
	public:
		friend CTimerMgr;

	private:
		bool								m_bPulse;
		float								m_fAccTime;
		float								m_fPulseTime;

	public:
		void Update(const float& _DeltaTime);
		bool CheckPulse();

	public:
		CPulse(const float& _PulseTime);
		~CPulse();

	};
	


	friend class CSJ_DXCore;

private:
	static float											m_fFPS;
	static float											m_fFPSCalTime;
	static float											m_fFPSCount;


	static CTimer											m_MainTimer;

private:
	static bool Init();
	static const bool Update();

public:
	static float DeltaTime() { return m_MainTimer.DeltaTime(); }
	static float FPS() { return m_fFPS; }
	// _AccTime + DeltaTime 이 _Timing 보다 커지는 순간에만 true를 반환
	static const bool TimingCheck(const float& _AccTime, const float& _Timing);

private:
	static std::map<float, SPTR<CPulse>>					m_mapPulse;

public:
	static CPulse* CreatePulse(const float& _PulseTime);
	static SPTR<CPulse> FindPulse(const float& _PulseTime);
	static bool Pulse(const float& _PulseTime);


};



#define DELTATIME CTimerMgr::DeltaTime()
#define FPS CTimerMgr::FPS()