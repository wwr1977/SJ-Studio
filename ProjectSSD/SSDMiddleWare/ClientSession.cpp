#include "ClientSession.h"
#include "ClientConnectMgr.h"
#include "ClientReceiver.h"
#include "ClientLogMgr.h"
#include "ClientSender.h"

void _stdcall CClientSession::SessionThreadProcess(void* _pArg)
{
	CClientSession* Session = reinterpret_cast<CClientSession*>(_pArg);

	HANDLE SessionHandle = Session->m_SessionIOCPHandle;

	DWORD Signal = 0;
	ULONG_PTR ComKey = NULL;
	OVERLAPPED* POVERLAPPED = nullptr;


	BOOL  Result;
	std::chrono::system_clock::time_point Ping;
	std::chrono::system_clock::time_point Pong = std::chrono::system_clock::now();
	std::chrono::duration<float> Time;


	while (true) 
	{
		// 1. ���� ������ ����
		GETSINGLE(CClientConnectMgr).m_Sender.SendPing();
		// 2. ����Ŷ�� ������ �پ������� ���
		Result = GetQueuedCompletionStatus(SessionHandle, &Signal, (PULONG_PTR)&ComKey, &POVERLAPPED, INFINITE);

		if (ENDSIGNAL == Signal && ENDSIGNAL == ComKey  &&  ENDSIGNAL == POVERLAPPED)
		{
			break;
		}

		if (Signal != PINGSIGNAL) 
		{
			Sleep(1000);
			continue;
		}

		// 3. �̶� Ÿ�̸Ӹ� �۵���Ű�� ���ù��� �� ��Ŷ�� ���������� ���(5�� ���)
		Ping = std::chrono::system_clock::now();
		Result = GetQueuedCompletionStatus(SessionHandle, &Signal, (PULONG_PTR)&ComKey, &POVERLAPPED, 5000);

		if (Result == FALSE)
		{
			Sleep(1000);
			continue;
		}

		if (ENDSIGNAL == Signal && ENDSIGNAL == ComKey  &&  ENDSIGNAL == POVERLAPPED)
		{
			break;
		}

		if (Signal != PONGSIGNAL)
		{
			Sleep(1000);
			continue;
		}

		//4. �� ��Ŷ�� ������ Ÿ�̸Ӹ� ������Ű�� ������ �ð��� Ŭ���̾�Ʈ Ŀ���� ���� �����Ѵ�.
		Pong = std::chrono::system_clock::now();
		Time = Pong - Ping;
		GETSINGLE(CClientConnectMgr).RecordPing(Time.count());


		Sleep(1000);
	}
	
	

	
}

CClientSession::CClientSession()
{
}


CClientSession::~CClientSession()
{
}

void CClientSession::Init()
{
	m_SessionIOCPHandle = m_SessionIOCP.CreateIOCP();
	MakeThread(SessionThreadProcess, this);
}

void CClientSession::Release()
{
	m_SessionIOCP.TerminateIOCP();
	m_SessionIOCPHandle = NULL;
	CThreadMaker::Release();
}

void CClientSession::SendPingEvent() 
{
	PostQueuedCompletionStatus(m_SessionIOCPHandle, PINGSIGNAL, NULL, NULL);
}
void CClientSession::RecvPongEvent()
{
	PostQueuedCompletionStatus(m_SessionIOCPHandle, PONGSIGNAL, NULL, NULL);
}