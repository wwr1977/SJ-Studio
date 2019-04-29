#include "stdafx.h"
#include "ResolutionMgr.h"

vector<CResolutionMgr::Resolution>	CResolutionMgr::m_vecRes;
CResolutionMgr::Resolution CResolutionMgr::m_CurRes =  CResolutionMgr::Resolution();

bool CResolutionMgr::Init() 
{
	int Count = 0;

	DEVMODE Dm = DEVMODE();

	while (EnumDisplaySettings(nullptr, Count, &Dm)) 
	{
		Resolution NewResolution = { Dm.dmPelsWidth,Dm.dmPelsHeight };

		Resolution  Fr = FindResolution(NewResolution);

		if (TRUE == Fr.IsZero()) 
		{
			m_vecRes.push_back(NewResolution);
		}
		++Count;
	}

	return TRUE;
}
CResolutionMgr::Resolution CResolutionMgr::FindResolution(Resolution _Resolution)
{
	if (0 == m_vecRes.size()) 
	{
		return { 0,0 };
	}

	for (size_t i = 0; i < m_vecRes.size(); i++)
	{
		if (m_vecRes[i] == _Resolution)
		{
			return _Resolution;
		}
	}

	return { 0,0 };
}

bool CResolutionMgr::Terminal()
{
	CResolutionMgr::m_vecRes.clear();

	return TRUE;
}