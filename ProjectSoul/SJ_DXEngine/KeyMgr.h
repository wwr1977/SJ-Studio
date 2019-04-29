#pragma once

//#ifndef _INPUTCLASS_H_
//#define _INPUTCLASS_H_
//
//#define DIRECTINPUT_VERSION 0x0800
//
//#pragma comment(lib, "dinput8.lib")
//#pragma comment(lib, "dxguid.lib")
//
//#include <dinput.h>
//#endif
//


#include"ReferenceBase.h"
#include"NameBase.h"
#include<vector>
#include<map>
#include<unordered_map>
#include<string>
#include"DXMath.h"

#include<XInput.h>

#ifdef X64
#pragma comment(lib, "Xinput_x64.lib")
#else
#pragma comment(lib, "Xinput.lib")
#endif

#define MAXSTICKVALUE 32767
#define LSTICK_DEADZONE 7849
#define RSTICK_DEADZONE 8689


// Ű�� �������� �˰� ���� ���¿� ���ؼ� �˾ƺ���.
// �� ũ�Ⱑ ��� ���ȳ� �ȴ��ȳ�?
// ���� �� ������ ��ũ�Ҽ� �ִ°�? �� Ű�� �ȴ����ٰ� ���� ���ȳ�?
// ���� ���� �� üũ�Ҽ� �ִ°�?
// ����Ű �����Է�

class CKeyMgr
{
public:
	class CKey : public CReferenceBase , public  CNameBase
	{
	public:
		friend CKeyMgr;

	protected:
		// �Ű��?
		std::vector<int> m_VecKey;

		float m_fDoubleTime;
		float m_fCurDoubleTime;
		bool m_bDouble;											// ������
		bool m_bUp;												// ������
		bool m_bPush;											// ��������
		bool m_bKey;											// ���������� �׻� ������ �ִ°�?

	public:
		void PushKey(int _Key) 
		{
			m_VecKey.push_back(_Key);
		}

		void SetDoubleTime(float _Time)
		{
			m_fDoubleTime = _Time;
		}

	public:
		void InvalidateKey();
		void Update(const float& _DeltaTime);
		virtual bool KeyCheck();

	public:
		bool GetKey() {
			return m_bKey;
		}

		bool GetPush() {
			return m_bPush;
		}

		bool GetUp() {
			return m_bUp;
		}

		bool GetDouble() {
			return m_bDouble;
		} 

	public:
		CKey();
		~CKey();
	};


	class CPadKey : public CKey
	{
		enum PAD_TRIGGER 
		{
			PAD_LTRIGGER = -1,
			PAD_RTRIGGER = -2,
			PAD_NOTRIGGER = 0,
			MAX_PAD_TRIGGER
		};
	private:
		friend CKeyMgr;

	private:
		static XINPUT_STATE					PadState;
		static void UpdatePadState();


	private:
		int					m_PadNum;
		bool				m_bLTrigger;
		bool				m_bRTrigger;
		

	public:
		bool KeyCheck() override;


	public:
		CPadKey(const int& _PadNum);
		~CPadKey();
	};


private:
	static std::map<std::wstring, SPTR<CKey>>::iterator			m_KeyStartIter;
	static std::map<std::wstring, SPTR<CKey>>::iterator			m_KeyEndIter;
	
	// <���� Ű�̸� , Ű��ü>
	static std::map<std::wstring, SPTR<CKey>>					m_mapKey;
	// <��Ī, ���� Ű �̸�>
	static std::unordered_map<std::wstring, std::wstring>		m_hashBindingKey;


	static SPTR<CKey>											m_pCurCreate;
	static bool													m_bUpdate;
	
	static bool													m_bWheel;
	static int													m_Zeta;


private:
	static bool													m_bLStick;
	static bool													m_bRStick;
	static POINT												m_LStickValue;
	static POINT												m_RStickValue;

public:
	static void Init();
	static void Release();

private:
	// static const bool CreateGamePadDevice();

public:
	static bool IsUpdate();
	static void UpdateOn();
	static void UpdateOff();

private:
	static void LoopSetting() 
	{
		m_KeyStartIter = m_mapKey.begin();
		m_KeyEndIter = m_mapKey.end();
	}


public:
	static SPTR<CKey> FindKey(const std::wstring& _Key);
	static std::wstring FindKeyName(const std::wstring& _BindKey);
public:
	template<typename ... Rest>
	static bool CreateKey(const std::wstring& _Key, Rest ...Rst)
	{
		SPTR<CKey> pFind = FindKey(_Key);

		if (nullptr != pFind) {
			return false;
		}

		m_pCurCreate = new CKey();
		m_pCurCreate->SetName(_Key);

		PushKey(Rst...);

		m_mapKey.insert(std::map<std::wstring, SPTR<CKey>>::value_type(_Key, m_pCurCreate));

		return true;
	}

	template<typename ... Rest>
	static bool CreateKey(const std::wstring& _Key, float _fDoubleTime, Rest ...Rst)
	{
		SPTR<CKey> pFind = FindKey(_Key);

		if (nullptr != pFind) {
			return false;
		}

		m_pCurCreate = new CKey();
		m_pCurCreate->SetDoubleTime(_fDoubleTime);
		m_pCurCreate->SetName(_Key);

		PushKey(Rst...);

		m_mapKey.insert(std::map<std::wstring, SPTR<CKey>>::value_type(_Key, m_pCurCreate));

		return true;
	}

	template<typename T, typename ... Rest>
	static void PushKey(T _Key, Rest ...Rst)
	{
		m_pCurCreate->PushKey(_Key);

		PushKey(Rst...);
	}

	template<typename T>
	static void PushKey(T _Key)
	{
		m_pCurCreate->PushKey(_Key);
		// �� ������ ���ڰ� ���ų�.
	}

	static void CreatePadKey(const wchar_t* _Key, const int& _PadNum);

public:
	static void Update(const float& _DeltaTime);
	static void UpdatePadStick();
	static bool GetKey(const std::wstring& _Key);
	static bool GetPush(const std::wstring& _Key);
	static bool GetUp(const std::wstring& _Key);
	static bool GetDouble(const std::wstring& _Key);
	static bool GetKeyOrPush(const std::wstring& _Key);

	static void ResetWheelValue();
	static void MouseWheelEvent(const int _Zeta);
	static bool GetWheelEvent();
	static int GetWheelSign();

public:
	static const bool CheckLStick() { return m_bLStick; }
	static const bool CheckRStick() { return m_bRStick; }
	static const POINT GetLStickValue() { return m_LStickValue; }
	static const POINT GetRStickValue() { return m_RStickValue; }
	static const Vec2 GetLStick();
	static const Vec2 GetRStick();

public:
	static void BindingKey(const std::wstring& _BindKey, const std::wstring& _Key);

public:
	CKeyMgr();
	~CKeyMgr();
};


// ���� ����
#define KEYDOWN(NAME) CKeyMgr::GetPush(L##NAME##)
// ������ �ִ���
#define KEYPRESS(NAME) CKeyMgr::GetKey(L##NAME##)
// Ű�� ���ȴ������� Ȯ��
#define KEY(NAME) CKeyMgr::GetKeyOrPush(L##NAME##)
// Ű�� �������� ������°�
#define KEYUP(NAME) CKeyMgr::GetUp(L##NAME##)
// Ű�� �ι�° ���� �����ΰ�
#define KEYDOUBLE(NAME) CKeyMgr::GetDouble(L##NAME##)