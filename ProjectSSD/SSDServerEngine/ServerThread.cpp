#include "ServerThread.h"

/////////////////////////////////////////////	ũ��Ƽ�� ����			//////////////////////////////////////
void CServerCS::Enter()
{
	EnterCriticalSection(&m_CRI);
}
void CServerCS::Leave()
{
	LeaveCriticalSection(&m_CRI);
}


CServerCS::CServerCS()
{
	InitializeCriticalSection(&m_CRI);
}
CServerCS::CServerCS(const CServerCS& _Other)
{
	InitializeCriticalSection(&m_CRI);
}

CServerCS::~CServerCS()
{
	DeleteCriticalSection(&m_CRI);
}


LocalCS::LocalCS() : m_CS(nullptr)
{
}

LocalCS::LocalCS(CServerCS* _CS) : m_CS(_CS)
{
	m_CS->Enter();
}

LocalCS::~LocalCS()
{
	m_CS->Leave();
}

/////////////////////////////////////////////	 ������ ���� �Լ�		////////////////////////////////////


std::unordered_map<std::wstring, CServerThread*> CServerThread::m_mapThread;


CServerThread* CServerThread::FindAndCreateServerThread(const wchar_t* _ThreadName)
{
	std::unordered_map<std::wstring, CServerThread*>::iterator FindIter = m_mapThread.find(_ThreadName);
	CServerThread* Find = nullptr;

	if (FindIter == m_mapThread.end())
	{
		CServerThread* NewThread = new CServerThread();
		NewThread->SetName(_ThreadName);
		m_mapThread.insert(std::unordered_map<std::wstring, CServerThread*>::value_type(_ThreadName, NewThread));
		Find = NewThread;
	}
	else
		Find = FindIter->second;

	return Find;
}
CServerThread* CServerThread::FindServerThread(const wchar_t* _ThreadName)
{
	std::unordered_map<std::wstring, CServerThread*>::iterator FindIter = m_mapThread.find(_ThreadName);
	
	if (FindIter == m_mapThread.end())
		return nullptr;

	return FindIter->second;
}
CServerThread* CServerThread::CreateServerThread(const wchar_t* _ThreadName)
{
	std::unordered_map<std::wstring, CServerThread*>::iterator FindIter = m_mapThread.find(_ThreadName);

	if (FindIter != m_mapThread.end())
		return nullptr;

	CServerThread* NewThread = new CServerThread();
	NewThread->SetName(_ThreadName);
	m_mapThread.insert(std::unordered_map<std::wstring, CServerThread*>::value_type(_ThreadName, NewThread));
	return NewThread;
}


CServerThread* CServerThread::Processing(const wchar_t* _Name, _beginthreadex_proc_type _Type, void* _Arg)
{
	CServerThread* pThread = FindAndCreateServerThread(_Name);

	if (nullptr == pThread) 
	{
		assert(nullptr);
		return false;
	}

	CProcData Data;
	Data.m_Arg = _Arg;
	Data.m_GlobalProc = _Type;
	pThread->PushProc(Data);
	pThread->StartProcessing();
	return pThread;
}
// ���࿡ �ϰ� �ִ� ���� �ִٸ� ���̾ �ٷ� ���� �����Ҽ� �ְ� ����� ���´�.
bool CServerThread::PushProc(const wchar_t* _Name, _beginthreadex_proc_type _Type) 
{
	return true;
}
// �����.
bool CServerThread::Pause(const wchar_t* _Name) 
{
	return true;
}
void CServerThread::End() 
{
	std::unordered_map<std::wstring, CServerThread*>::iterator Start = m_mapThread.begin();
	std::unordered_map<std::wstring, CServerThread*>::iterator End = m_mapThread.end();

	for (;Start!= End ;++Start)
	{
		DELETE_POINTER(Start->second);
	}
}

/////////////////////////////////////////////	 ������ �ɹ� �Լ�		////////////////////////////////////


CServerThread::CServerThread()
	:m_ThreadObj(std::thread())
{
}


CServerThread::~CServerThread()
{
	m_ThreadObj.join();
}

void CServerThread::PushProc(const CProcData& _Proc)
{
	m_ThreadCS.Enter();
	m_listThreadProc.push_back(_Proc);
	m_ThreadCS.Leave();
}
const size_t CServerThread::GetProcCount()
{
	size_t Count;

	m_ThreadCS.Enter();
	Count = m_listThreadProc.size();
	m_ThreadCS.Leave();

	return Count;
}
void CServerThread::StartProcessing() 
{
	if (false == m_ThreadObj.joinable()) 
	{
		std::wstring Text = L"'" + GetName() + L"' Operation";
		OutputDebugString(Text.c_str());
		m_ThreadObj.swap(std::thread(ServerThreadBaseFunc, this));
	}
	else 
	{
		m_ThreadObj.join();
		std::wstring Text = L"'" + GetName() + L"' Operation";
		OutputDebugString(Text.c_str());
		m_ThreadObj.swap(std::thread(ServerThreadBaseFunc, this));
	}
}
void CServerThread::Processing() 
{
	m_ThreadCS.Enter();
	std::list<CProcData>::iterator Iter = m_listThreadProc.begin();
	
	if (nullptr != Iter->m_GlobalProc)
		Iter->m_GlobalProc(Iter->m_Arg);
	else if(nullptr != Iter->m_MemberProc)
		Iter->m_MemberProc(Iter->m_Arg);
	else
		assert(nullptr);
	
	m_listThreadProc.pop_front();
	m_ThreadCS.Leave();
}

unsigned int __stdcall ServerThreadBaseFunc(void* _Arg)
{
	if (nullptr == _Arg)
	{
		assert(nullptr);
		return -1;
	}

	CServerThread* pThread = (CServerThread*)_Arg;

	if (0 >= pThread->GetProcCount())
	{
		assert(nullptr);
		return -1;
	}

	while (0 != pThread->GetProcCount())
	{
		// ��û ª�� �Լ��� 100 �� �ð�
		pThread->Processing();
	}

	return 0;
}
