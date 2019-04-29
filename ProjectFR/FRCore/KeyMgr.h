#pragma once

// Ű�� �������� �˰� ���� ���¿� ���ؼ� �˾ƺ���.
// �� ũ�Ⱑ ��� ���ȳ� �ȴ��ȳ�?
// ���� �� ������ ��ũ�Ҽ� �ִ°�? �� Ű�� �ȴ����ٰ� ���� ���ȳ�?
// ���� ���� �� üũ�Ҽ� �ִ°�?
// ����Ű �����Է�

class CKeyMgr
{

public:
	class CKey : public CObjBase
	{
	public:
		friend CKeyMgr;

	private:
		// �Ű��?
		vector<int> m_VecKey;

		float m_fDoubleTime;
		float m_fCurDoubleTime;
		bool m_bDouble;   // ������
		bool m_bUp;   // ������
		bool m_bPush; // ��������
		bool m_bKey; // ���������� �׻� ������ �ִ°�?

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
		void Update();
		bool KeyCheck();

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

		// Push�� 

	public:
		CKey();
		~CKey();
	};

	//typedef CKeyMgr::CKey CKey;

private:
	static map<tstring, SPTR<CKey>>::iterator			m_KeyStartIter;
	static map<tstring, SPTR<CKey>>::iterator			m_KeyEndIter;
	static map<tstring, SPTR<CKey>>						m_mapKey;
	static SPTR<CKey>									m_pCurCreate;
	static bool											m_bUpdate;
	
	static bool											m_bWheel;
	static int											m_Zeta;


public:
	static bool IsUpdate();
	static void UpdateOn();
	static void UpdateOff();

private:
	static void LoopSetting() {
		m_KeyStartIter = m_mapKey.begin();
		m_KeyEndIter = m_mapKey.end();
	}

public:
	static SPTR<CKey> FindKey(const tstring& _Key);

public:
	template<typename ... Rest>
	static bool CreateKey(const tstring& _Key, Rest ...Rst)
	{
		SPTR<CKey> pFind = FindKey(_Key);

		if (nullptr != pFind) {
			return false;
		}

		m_pCurCreate = new CKey();
		m_pCurCreate->Name(_Key);

		PushKey(Rst...);

		m_mapKey.insert(map<tstring, SPTR<CKey>>::value_type(_Key, m_pCurCreate));

		return true;
	}

	template<typename ... Rest>
	static bool CreateKey(const tstring& _Key, float _fDoubleTime, Rest ...Rst)
	{
		SPTR<CKey> pFind = FindKey(_Key);

		if (nullptr != pFind) {
			return false;
		}

		m_pCurCreate = new CKey();
		m_pCurCreate->SetDoubleTime(_fDoubleTime);
		m_pCurCreate->Name(_Key);

		PushKey(Rst...);

		m_mapKey.insert(map<tstring, SPTR<CKey>>::value_type(_Key, m_pCurCreate));

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

public:
	static void Update();

	static bool GetKey(const tstring& _Key);
	static bool GetPush(const tstring& _Key);
	static bool GetUp(const tstring& _Key);
	static bool GetDouble(const tstring& _Key);
	static bool GetKeyOrPush(const tstring& _Key);

	static void ResetWheelValue();
	static void MouseWheelEvent(const int _Zeta);
	static bool GetWheelEvent();
	static int GetWheelSign();
	//void PushKey()
	//{
	//	// �� ������ ���ڰ� ���ų�.
	//}


	// � �ֵ��� �����.
	// ���״� ��δ� ����� ��ġ�� ���� ���ڸ� �ϳ��� ������ �ִ�.

public:
	CKeyMgr();
	~CKeyMgr();
};

