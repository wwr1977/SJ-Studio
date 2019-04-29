#include "TimerMgr.h"
#include"DXTemplate.h"

float CTimerMgr::m_fFPS = 0.0f;
float CTimerMgr::m_fFPSCount = 0.0f;
float CTimerMgr::m_fFPSCalTime = 0.0f;
CTimerMgr::CTimer CTimerMgr::m_MainTimer = CTimerMgr::CTimer();

std::map<float, SPTR<CTimerMgr::CPulse>>			CTimerMgr::m_mapPulse;

bool CTimerMgr::Init() 
{
	m_MainTimer = CTimer();
	return m_MainTimer.Init();

}
const bool CTimerMgr::Update()
{
	m_fFPSCalTime += m_MainTimer.Update();

	++m_fFPSCount; // 600

	if (1.0f <= m_fFPSCalTime) // 1.1f
	{
		m_fFPS = m_fFPSCount / m_fFPSCalTime;

		m_fFPSCount = 0.0f;
		m_fFPSCalTime = 0.0f;
	}

	for ( std::map< float,SPTR<CPulse>>::value_type& MapValue : m_mapPulse)
	{
		if (nullptr == MapValue.second)
			continue;

		MapValue.second->Update(m_MainTimer.m_fDeltaTime);
	}

	return true;
}

const bool CTimerMgr::TimingCheck(const float& _AccTime, const float& _Timing)
{
	if (_AccTime < _Timing && _AccTime + m_MainTimer.m_fDeltaTime >= _Timing)
		return true;

	return false;
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

		m_mapPulse.insert(std::map<float, SPTR<CPulse>>::value_type(_PulseTime, NewPulse));
	}

	return nullptr;
}
SPTR<CTimerMgr::CPulse> CTimerMgr::FindPulse(const float& _PulseTime)
{
	return MAPFINDSPTR<SPTR<CTimerMgr::CPulse>>(m_mapPulse , _PulseTime);
}
bool CTimerMgr::Pulse(const float& _PulseTime)
{
	SPTR<CPulse> FindPulse = CTimerMgr::FindPulse(_PulseTime);

	// ���ڷ� ���� �޽�Ÿ���� üũ�ϴ� �޽��� ������ ���� ����
	// �ʿ� �ִ´�
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


//////////////////////////////////////////////////////////////////			Timer			////////////////////////////

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
CTimerMgr::CTimer::CTimer()
{

}
CTimerMgr::CTimer::~CTimer() 
{

}

//////////////////////////////////////////////////////////				Pulse			/////////////////////////////////


CTimerMgr::CPulse::CPulse(const float& _PulseTime)
	:m_fPulseTime(_PulseTime), m_bPulse(false)
	, m_fAccTime(0.0f)
{

}
CTimerMgr::CPulse::~CPulse()
{

}
void CTimerMgr::CPulse::Update(const float& _DeltaTime)
{
	m_fAccTime += _DeltaTime;
	m_bPulse = false;
	
	if (m_fAccTime >= m_fPulseTime)
	{
		m_bPulse = true;
		m_fAccTime = 0.f;
	}


	//if (m_fPulseTime > m_fAccTime)
	//{
	//	m_bPulse = false;
	//	return;
	//}

	/*while (m_fAccTime >= m_fPulseTime)
		m_fAccTime -= m_fPulseTime;*/
	
	//m_bPulse = true;
	return;
}
bool CTimerMgr::CPulse::CheckPulse()
{
	return m_bPulse;
}

