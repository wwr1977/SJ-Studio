#include "stdafx.h"
#include "KeyMgr.h"

typedef CKeyMgr::CKey CKey;



map<tstring, SPTR<CKey>>::iterator			CKeyMgr::m_KeyStartIter;
map<tstring, SPTR<CKey>>::iterator			CKeyMgr::m_KeyEndIter;
map<tstring, SPTR<CKey>>					CKeyMgr::m_mapKey;

SPTR<CKeyMgr::CKey> CKeyMgr::m_pCurCreate = nullptr;
bool										CKeyMgr::m_bUpdate =true;

bool										CKeyMgr::m_bWheel = false;
int											CKeyMgr::m_Zeta = 0;
/////////////////////// Key

CKey::CKey()
	: m_fDoubleTime(0.2f)
	, m_fCurDoubleTime(0.0f)
{
	m_bUp = false;
	m_bPush = false;
	m_bKey = false;
}

CKey::~CKey()
{
}

bool CKey::KeyCheck() {

	for (size_t i = 0; i < m_VecKey.size(); i++)
	{
		if (0 == (GetAsyncKeyState(m_VecKey[i]) & 0x8000)) 
		{
			return false;
		}
	}

	return true;
}

void CKey::Update() {

	if (0 < m_fCurDoubleTime)
	{
		m_fCurDoubleTime -= DELTATIME;
	}

	// 키가 모두 눌렸다.
	if (true == KeyCheck())
	{
		if (false == m_bKey)
		{
			m_bPush = true;

			// 여기서는 두가지 상황
			// 이미 체크하고 있는가?

			if (0 >= m_fCurDoubleTime)
			{
				m_fCurDoubleTime = m_fDoubleTime;
			} 
			else 
			{
				m_bDouble = true;
			}
		}
		else {
			m_bDouble = false;
			m_bPush = false;
		}

		m_bKey = true;
		m_bUp = false;
	}
	else 
	{
		if (true == m_bKey)
		{
			m_bUp = true;
		}
		else {
			m_bUp = false;
		}

		m_bPush = false;
		m_bKey = false;
	}


}
void CKey::InvalidateKey()
{
	m_bDouble = false;
	m_bKey = false;
	m_bPush = false;
	m_bUp = false;
	m_fCurDoubleTime = 0.0f;
}
////////////////////// CInputMgr

CKeyMgr::CKeyMgr() 
{
}

CKeyMgr::~CKeyMgr()
{
	m_mapKey.clear();
}

SPTR<CKey> CKeyMgr::FindKey(const tstring& _Key)
{
	return FINDMAP_SPTR<CKey>(_Key, m_mapKey);
}

void CKeyMgr::Update()
{
	CCriSectionObject<CKeyMgr>();
	LoopSetting();
	
	if (true == m_bUpdate)
	{
		for (; m_KeyStartIter != m_KeyEndIter; m_KeyStartIter++)
		{
			m_KeyStartIter->second->Update();
		}
	}
	else 
	{
		for (; m_KeyStartIter != m_KeyEndIter; m_KeyStartIter++)
		{
			m_KeyStartIter->second->InvalidateKey();
		}
	}

	return;
}

bool CKeyMgr::GetKey(const tstring& _Key)
{
	SPTR<CKey> pKey = FindKey(_Key);

	if (nullptr == pKey)
	{
		return false;
	}

	return pKey->GetKey();
}

bool CKeyMgr::GetPush(const tstring& _Key)
{
	SPTR<CKey> pKey = FindKey(_Key);

	if (nullptr == pKey)
	{
		return false;
	}

	return pKey->GetPush();
}

bool CKeyMgr::GetUp(const tstring& _Key)
{
	SPTR<CKey> pKey = FindKey(_Key);

	if (nullptr == pKey)
	{
		return false;
	}

	return pKey->GetUp();
}

bool CKeyMgr::GetDouble(const tstring& _Key)
{
	SPTR<CKey> pKey = FindKey(_Key);

	if (nullptr == pKey)
	{
		return false;
	}

	return pKey->GetDouble();
}
 bool CKeyMgr::GetKeyOrPush(const tstring& _Key)
{
	 SPTR<CKey> pKey = FindKey(_Key);

	 if (nullptr == pKey)
	 {
		 return false;
	 }

	 return pKey->GetKey() || pKey->GetPush();
}
 void CKeyMgr::ResetWheelValue()
 {
	 m_bWheel = false;
	 m_Zeta = 0;
 }
 void CKeyMgr::MouseWheelEvent(const int _Zeta) 
 {
	 m_bWheel = true;
	 m_Zeta = _Zeta;
	 int a = 0;
 }
 bool CKeyMgr::GetWheelEvent() 
 {
	 return m_bWheel;
 }
 int CKeyMgr::GetWheelSign() 
 {
	 return m_Zeta;
 }
 bool CKeyMgr::IsUpdate() 
 {
	 return m_bUpdate; 
 }
 void CKeyMgr::UpdateOn() 
 {
	 m_bUpdate = true; 
 }
 void CKeyMgr::UpdateOff() 
 {
	 m_bUpdate = false;
 }