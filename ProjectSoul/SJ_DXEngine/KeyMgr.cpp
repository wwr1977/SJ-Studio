#include "KeyMgr.h"
#include<Windows.h>
#include"SJ_DXCore.h"
#include"GameWindow.h"
#include"DXTemplate.h"
#include"DXMacro.h"

typedef CKeyMgr::CKey CKey;
typedef CKeyMgr::CPadKey CPadKey;



std::map<std::wstring, SPTR<CKey>>::iterator			CKeyMgr::m_KeyStartIter;
std::map<std::wstring, SPTR<CKey>>::iterator			CKeyMgr::m_KeyEndIter;
std::map<std::wstring, SPTR<CKey>>						CKeyMgr::m_mapKey;
std::unordered_map<std::wstring, std::wstring>			CKeyMgr::m_hashBindingKey;

SPTR<CKeyMgr::CKey>										CKeyMgr::m_pCurCreate = nullptr;
bool													CKeyMgr::m_bUpdate =true;

bool													CKeyMgr::m_bWheel = false;
int														CKeyMgr::m_Zeta = 0;

bool													CKeyMgr::m_bLStick = false;
bool													CKeyMgr::m_bRStick = false;
POINT													CKeyMgr::m_LStickValue = POINT{};
POINT													CKeyMgr::m_RStickValue = POINT{};
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

void CKey::Update(const float& _DeltaTime) {

	if (0 < m_fCurDoubleTime)
	{
		m_fCurDoubleTime -= _DeltaTime;
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

///////////////////////////////////////////////////////			PadKey				//////////////////////////////
XINPUT_STATE					CPadKey::PadState;

void CPadKey::UpdatePadState()
{
	ZeroMemory(&PadState, sizeof(XINPUT_STATE));


	DWORD dwResult = XInputGetState(0, &PadState);

	if (dwResult != ERROR_SUCCESS)
		ZeroMemory(&PadState, sizeof(XINPUT_STATE));

	
}
CPadKey::CPadKey(const int& _PadNum)
	:CKey(), m_PadNum(_PadNum), m_bLTrigger(false), m_bRTrigger(false)
{
	if (_PadNum == PAD_LTRIGGER)
		m_bLTrigger = true;
	else if (_PadNum == PAD_RTRIGGER)
		m_bRTrigger = true;
}
CPadKey::~CPadKey() 
{

}


bool CPadKey::KeyCheck()
{
	if (true == m_bLTrigger) 
	{
		if (0 != PadState.Gamepad.bLeftTrigger)
			return true;
		else
			return false;
	}

	if (true == m_bRTrigger)
	{
		if (0 != PadState.Gamepad.bRightTrigger)
			return true;
		else
			return false;
	}

	if (0 != (m_PadNum & PadState.Gamepad.wButtons) )
		return true;

	return false;
}

///////////////////////////////////////////////////////		 CInputMgr				//////////////////////////////

CKeyMgr::CKeyMgr() 
{
}

CKeyMgr::~CKeyMgr()
{
	m_mapKey.clear();
}

void CKeyMgr::Init()
{
	CKeyMgr::CreateKey(L"LMouse", VK_LBUTTON);
	CKeyMgr::CreateKey(L"RMouse", VK_RBUTTON);

	CKeyMgr::CreateKey(L"VK_UP", VK_UP);
	CKeyMgr::CreateKey(L"VK_DOWN", VK_DOWN);
	CKeyMgr::CreateKey(L"VK_LEFT", VK_LEFT);
	CKeyMgr::CreateKey(L"VK_RIGHT", VK_RIGHT);

	CKeyMgr::CreateKey(L"Q", 'Q');
	CKeyMgr::CreateKey(L"W", 'W');
	CKeyMgr::CreateKey(L"E", 'E');
	CKeyMgr::CreateKey(L"R", 'R');
	CKeyMgr::CreateKey(L"T", 'T');
	CKeyMgr::CreateKey(L"Y", 'Y');
	CKeyMgr::CreateKey(L"U", 'U');
	CKeyMgr::CreateKey(L"I", 'I');
	CKeyMgr::CreateKey(L"O", 'O');
	CKeyMgr::CreateKey(L"P", 'P');
	CKeyMgr::CreateKey(L"A", 'A');
	CKeyMgr::CreateKey(L"S", 'S');
	CKeyMgr::CreateKey(L"D", 'D');	
	CKeyMgr::CreateKey(L"F", 'F');
	CKeyMgr::CreateKey(L"G", 'G');
	CKeyMgr::CreateKey(L"H", 'H');
	CKeyMgr::CreateKey(L"J", 'J');
	CKeyMgr::CreateKey(L"K", 'K');
	CKeyMgr::CreateKey(L"L", 'L');
	CKeyMgr::CreateKey(L"Z", 'Z');
	CKeyMgr::CreateKey(L"X", 'X');
	CKeyMgr::CreateKey(L"C", 'C');
	CKeyMgr::CreateKey(L"V", 'V');
	CKeyMgr::CreateKey(L"B", 'B');
	CKeyMgr::CreateKey(L"N", 'N');
	CKeyMgr::CreateKey(L"M", 'M');
	CKeyMgr::CreateKey(L",", ',');
	CKeyMgr::CreateKey(L".", '.');
	CKeyMgr::CreateKey(L"`", '`');



	CKeyMgr::CreateKey(L"1", '1');
	CKeyMgr::CreateKey(L"2", '2');
	CKeyMgr::CreateKey(L"3", '3');
	CKeyMgr::CreateKey(L"4", '4');
	CKeyMgr::CreateKey(L"5", '5');
	CKeyMgr::CreateKey(L"6", '6');
	CKeyMgr::CreateKey(L"7", '7');
	CKeyMgr::CreateKey(L"8", '8');
	CKeyMgr::CreateKey(L"9", '9');
	CKeyMgr::CreateKey(L"0", '0');

	CKeyMgr::CreateKey(L"Tab", VK_TAB);
	CKeyMgr::CreateKey(L"Ctrl", VK_CONTROL);
	CKeyMgr::CreateKey(L"LShift", VK_LSHIFT);
	CKeyMgr::CreateKey(L"RShift", VK_RSHIFT);

	CKeyMgr::CreatePadKey(L"PAD_LTrigger", CPadKey::PAD_LTRIGGER);
	CKeyMgr::CreatePadKey(L"PAD_RTrigger", CPadKey::PAD_RTRIGGER);

	CKeyMgr::CreatePadKey(L"PAD_A", 4096);
	CKeyMgr::CreatePadKey(L"PAD_B", 8192);
	CKeyMgr::CreatePadKey(L"PAD_X", 16384);
	CKeyMgr::CreatePadKey(L"PAD_Y", 32768);

}

//const bool CKeyMgr::CreateGamePadDevice()
//{
//	/*HRESULT hr = DirectInput8Create(Core::GetWindowInstance(), DIRECTINPUT_VERSION, IID_IDirectInput8
//		, (void**)&m_pDirectInput, NULL);
//
//	if (S_OK != hr)
//		return false;
//	
//	hr = m_pDirectInput->CreateDevice(GUID_Joystick, &m_pGamePadDevice, NULL);
//
//	if (S_OK != hr)
//		return false;
//
//	hr = m_pGamePadDevice->SetCooperativeLevel(Core::GetWindowWnd(), DISCL_FOREGROUND | DISCL_EXCLUSIVE);
//
//	if (S_OK != hr)
//		return false;
//	
//	hr = m_pGamePadDevice->SetDataFormat(&c_dfDIJoystick);
//
//	if (S_OK != hr)
//		return false;
//
//	hr = m_pGamePadDevice->Acquire();*/
//
//	//if (S_OK != hr)
//	//	return false;
//
//	//XINPUT_STATE Temp;
//
//	//
//	//XInputGetState(0, &Temp);
//
//	//DWORD dwResult = XInputGetState(0, &Temp);
//
//	//if (dwResult == ERROR_SUCCESS)
//	//{
//	//	// Controller is connected 
//	//	int a = 0;
//	//}
//	//else
//	//{
//	//	// Controller is not connected 
//	//	int b = 0;
//	//}
//	//
//	//return true;
//}

void CKeyMgr::Release()
{
	//if (nullptr != m_pGamePadDevice)
	//{
	//	m_pGamePadDevice->Unacquire();
	//	m_pGamePadDevice->Release();
	//	m_pGamePadDevice = nullptr;
	//}

	//if (nullptr != m_pDirectInput)
	//{
	//	m_pDirectInput->Release();
	//	m_pDirectInput = nullptr;
	//}

}
SPTR<CKey> CKeyMgr::FindKey(const std::wstring& _Key)
{
	return MAPFINDSPTR<SPTR<CKey>>(m_mapKey,_Key);
}
std::wstring CKeyMgr::FindKeyName(const std::wstring& _BindKey)
{
	return MAPFINDVALUE<std::wstring>(m_hashBindingKey, _BindKey);
}
void CKeyMgr::CreatePadKey(const wchar_t* _Key, const int& _PadNum)
{
	m_mapKey.insert(std::map<std::wstring, SPTR<CKey>>::value_type(_Key, new CPadKey(_PadNum)));
}
void CKeyMgr::Update(const float& _DeltaTime)
{
	CPadKey::UpdatePadState();
	
	LoopSetting();
	
	if (true == m_bUpdate)
	{
		for (; m_KeyStartIter != m_KeyEndIter; m_KeyStartIter++)
		{
			m_KeyStartIter->second->Update(_DeltaTime);
		}
	}
	else 
	{
		for (; m_KeyStartIter != m_KeyEndIter; m_KeyStartIter++)
		{
			m_KeyStartIter->second->InvalidateKey();
		}
	}

	UpdatePadStick();
	return;
}
void CKeyMgr::UpdatePadStick()
{
	float LX = (float)CPadKey::PadState.Gamepad.sThumbLX;
	float LY = (float)CPadKey::PadState.Gamepad.sThumbLY;
	float RX = (float)CPadKey::PadState.Gamepad.sThumbRX;
	float RY = (float)CPadKey::PadState.Gamepad.sThumbRY;

	if (sqrtf(LX*LX + LY * LY) <= (float)LSTICK_DEADZONE) 
	{
		m_bLStick = false;
		m_LStickValue = POINT{ 0,0 };
	}
	else 
	{
		m_bLStick = true;
		m_LStickValue = POINT{ (LONG)LX ,(LONG)LY };
	}

	if (sqrtf(RX * RX + RY * RY) <= (float)RSTICK_DEADZONE) 
	{
		m_bRStick = false;
		m_RStickValue = POINT{ 0,0 };
	}
	else 
	{
		m_bRStick = true;
		m_RStickValue = POINT{ (LONG)RX ,(LONG)RY };
	}

}
bool CKeyMgr::GetKey(const std::wstring& _Key)
{
	SPTR<CKey> pKey = FindKey(FindKeyName(_Key));

	if (nullptr == pKey)
	{
		return false;
	}

	return pKey->GetKey();
}

bool CKeyMgr::GetPush(const std::wstring& _Key)
{
	
	SPTR<CKey> pKey = FindKey(FindKeyName(_Key));

	if (nullptr == pKey)
	{
		return false;
	}

	return pKey->GetPush();
}

bool CKeyMgr::GetUp(const std::wstring& _Key)
{
	SPTR<CKey> pKey = FindKey(FindKeyName(_Key));

	if (nullptr == pKey)
	{
		return false;
	}

	return pKey->GetUp();
}

bool CKeyMgr::GetDouble(const std::wstring& _Key)
{
	SPTR<CKey> pKey = FindKey(FindKeyName(_Key));

	if (nullptr == pKey)
	{
		return false;
	}

	return pKey->GetDouble();
}
 bool CKeyMgr::GetKeyOrPush(const std::wstring& _Key)
{
	 SPTR<CKey> pKey = FindKey(FindKeyName(_Key));

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
const Vec2 CKeyMgr::GetLStick() 
{
	Vec2 LStick = Vec2((float)m_LStickValue.x, (float)m_LStickValue.y);
	return LStick.GetNormalizeVec();
}
const Vec2 CKeyMgr::GetRStick()
{
	Vec2 RStick = Vec2((float)m_RStickValue.x, (float)m_RStickValue.y);
	return RStick.GetNormalizeVec();
}
 void CKeyMgr::BindingKey(const std::wstring& _BindKey, const std::wstring& _Key)
 {
	 std::unordered_map<std::wstring, std::wstring>::iterator Find = m_hashBindingKey.find(_Key);

	 if (Find == m_hashBindingKey.end())
		m_hashBindingKey.insert(std::unordered_map<std::wstring, std::wstring>::value_type(_BindKey, _Key));
	

 }