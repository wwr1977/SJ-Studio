#include "stdafx.h"
#include "TimerMgr.h"

float CTimerMgr::m_fFPS = 0.0f;
float CTimerMgr::m_fFPSCount = 0.0f;
float CTimerMgr::m_fFPSCalTime = 0.0f;
CTimerMgr::CTimer CTimerMgr::m_MainTimer = CTimerMgr::CTimer(_T("MainTimer"));
map<tstring, SPTR<CTimerMgr::CTimer>>  CTimerMgr::m_mapTimer;
map<float, SPTR<CTimerMgr::CPulse>>				CTimerMgr::m_mapPulse;

bool CTimerMgr::Init() 
{
	m_MainTimer = CTimer();
	return m_MainTimer.Init();

}
bool CTimerMgr::Update() 
{
	m_fFPSCalTime += m_MainTimer.Update();

	++m_fFPSCount; // 600

	if (1.0f <= m_fFPSCalTime) // 1.1f
	{
		// 600 / 1.1f;
		// 593.245162478979456

		m_fFPS = m_fFPSCount / m_fFPSCalTime;

		m_fFPSCount = 0.0f;
		m_fFPSCalTime = 0.0f;
	}

	CTimerMgr::PulseUpdate();

	return TRUE;
}
bool CTimerMgr::PulseUpdate()
{
	map<float, SPTR<CPulse>>::iterator Start = m_mapPulse.begin();
	map<float, SPTR<CPulse>>::iterator End = m_mapPulse.end();

	for (; Start != End; ++Start) 
	{
		Start->second->Update();
	}
	return true;
}
bool CTimerMgr::CreateTimer(tstring _Name)
{
	SPTR<CTimer> Timer = FindTimer(_Name);

	if (nullptr == Timer) 
	{
		return FALSE;
	}

	CTimer* NewTimer = new CTimer(_Name);

	m_mapTimer.insert(map<tstring, SPTR<CTimer>>::value_type(NewTimer->Name(), NewTimer));

	return TRUE;
}
SPTR<CTimerMgr::CTimer> CTimerMgr::FindTimer(tstring _Name) 
{
	return FINDMAP_SPTR<CTimer>(_Name,m_mapTimer);
}
bool CTimerMgr::Terminal() 
{
	m_mapTimer.clear();
	return TRUE;
}


CTimerMgr::CPulse* CTimerMgr::CreatePulse(const float& _PulseTime)
{
	SPTR<CPulse> FindPulse = CTimerMgr::FindPulse(_PulseTime);

	if (nullptr == FindPulse) 
	{
		CPulse* NewPulse = new CPulse(_PulseTime);

		if (nullptr == NewPulse) 
		{
			assert(NewPulse);
			return nullptr;
		}

		m_mapPulse.insert(map<float, SPTR<CPulse>>::value_type(_PulseTime, NewPulse));
	}

	return nullptr;
}
SPTR<CTimerMgr::CPulse> CTimerMgr::FindPulse(const float& _PulseTime) 
{
	return FINDMAP_SPTR<CTimerMgr::CPulse>(_PulseTime, m_mapPulse);
}
bool CTimerMgr::Pulse(const float& _PulseTime) 
{
	SPTR<CPulse> FindPulse = CTimerMgr::FindPulse(_PulseTime);

	// 인자로 받은 펄스타임을 체크하는 펄스가 없으면 새로 만들어서
	// 맵에 넣는다
	if (nullptr == FindPulse) 
	{
		CPulse* NewPulse = CTimerMgr::CreatePulse(_PulseTime);
		
		if (nullptr == NewPulse) 
		{
			return false;
		}

		return NewPulse->CheckPulse();
	}

	return FindPulse->CheckPulse();
}


//////////////////////////////////////////////////////////////////////////////////////////////////

bool CTimerMgr::CTimer::Init() 
{
	QueryPerformanceFrequency(&m_ClockFreg);
	QueryPerformanceCounter(&m_PrevCount);
	
	
	return TRUE;
}

float CTimerMgr::CTimer::Update() 
{
	QueryPerformanceCounter(&m_CurCount);

	m_fDeltaTime = (float)(m_CurCount.QuadPart - m_PrevCount.QuadPart);
	m_fDeltaTime /= (float)m_ClockFreg.QuadPart;

	m_PrevCount = m_CurCount;

	return m_fDeltaTime;
}
CTimerMgr::CTimer::CTimer(tstring _Name /*= _T("")*/) 
	:CObjBase(_Name)
{

}
CTimerMgr::CTimer::~CTimer() 
{

}


////////////////////////////////////////////////		Pulse	/////////////////////


CTimerMgr::CPulse::CPulse(const float& _PulseTime) 
	:m_fPulseTime(_PulseTime),m_bPulse(false)
	, m_fAccTime(0.0f)
{

}
CTimerMgr::CPulse::~CPulse() 
{

}
void CTimerMgr::CPulse::Update() 
{
	m_fAccTime += DELTATIME;

	if (m_fPulseTime > m_fAccTime) 
	{
		m_bPulse = false;
		return;
	}

	while (m_fAccTime >= m_fPulseTime) 
	{
		m_fAccTime -= m_fPulseTime;
		m_bPulse = true;
	}


	return;
}
bool CTimerMgr::CPulse::CheckPulse()
{
	return m_bPulse;
}