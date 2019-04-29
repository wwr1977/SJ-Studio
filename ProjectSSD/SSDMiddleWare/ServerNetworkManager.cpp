#include "ServerNetworkManager.h"
#include "ServerCommendManager.h"
#include "ServerLogManager.h"
#include <IOCP.h>
#include "IOCPServerMgr.h"
#include "MiddleStruct.h"
#include <SSDServerCore.h>




unsigned int _stdcall CServerNetworkManager::NetworkThreadProcess(void* _Arg) 
{
	IOCPServerMgr* IOCPMgr = (IOCPServerMgr*)_Arg;

	if (nullptr == IOCPMgr)
	{
		std::wstring ErrorText = L"Create Network IOCP Thread Fail\n";
		OutputDebugString(ErrorText.c_str());
		return -1;
	}

	
	HANDLE hNetIOCP = IOCPMgr->pNetworkMgr->m_hNetIOCP;
	DWORD ByteSize = 0;
	CServerUser* pUser = nullptr;
	OVERLAPPED* POVERLAPPED = nullptr;

	while (true)
	{
		int  Result = GetQueuedCompletionStatus(hNetIOCP, &ByteSize, (PULONG_PTR)&pUser, &POVERLAPPED, INFINITE);

		if (ENDSIGNAL == ByteSize && ENDSIGNAL == pUser  &&  ENDSIGNAL == POVERLAPPED)
		{
			break;
		}

		CServerUser::OVERBUFFER* pOverPart = DeductionPointer(CServerUser::OVERBUFFER*, POVERLAPPED, m_Over);

		if (nullptr == pOverPart || nullptr == pOverPart->m_User)
			continue;

		switch (pOverPart->m_eType)
		{
			// Ŭ���̾�Ʈ ������ connect �Լ��� ȣ���ϴ� ���� 
		case CServerUser::WORK_TYPE::WORK_CONNET:
		{
			if (true == IOCPMgr->ConnectUser(pOverPart))
			{
				pUser = pOverPart->m_User;
				pUser->ConnectEvent();
				IOCPMgr->PrintLog(LogManager::MakeLogString("Connect Client IP : ", pUser->GetRemoteIPAddressString(), " , UserSocket : ", (int)pUser->m_UserSock));
				pUser->NotifyCompleteConnect();
				pUser->RecvPacket();
			}
			else
			{
				IOCPMgr->PrintLog(LogManager::MakeLogString("Connect Fail Remote IP : ", pUser->GetRemoteIPAddressString(), "  , Remote  Port Number : ", (int)pUser->m_UserSock));
			}
		}
		break;
		case CServerUser::WORK_TYPE::WORK_RECV:
		{
			// Ŭ���̾�Ʈ �ʿ��� closesocket�� ������ ��� GetQueued���� ����Ʈ ũ�Ⱚ����
			// 0�� ������ ������ Ŭ���̾�Ʈ ���� �����۾��� ���⼭ �����Ѵ�.
			if (0 == ByteSize) 
			{
				// ������ ������ ������ ��
				if (pUser->m_fAccCriticalTime >= MAXCRITICALTIME) 
				{
					IOCPMgr->DisConnectUser(pUser);

				}
				else 
				{
					PACKET Pack;
					Pack.Header.HeaderFlag = FLAG_DISCONNECT;
					Pack.pUser = pUser;
					IOCPMgr->PacketProcessing(Pack);
				}

				pUser->ResetCriticalTime();
				IOCPMgr->pCommandMgr->ExitRoom(pUser);
			}
			else 
			{
				// ��Ŷ ��ó�� �Լ����� ��Ŷ�� ó������ �ʾ����� ��Ŷť�� ��Ŷ�� �ְ�
				// ��Ŷ ó���� ���� �Ŵ����� �����ϵ��� �Ѵ�.
				//IOCPMgr->PrintLog(LogManager::MakeLogString("Recv Packet Size : ", ByteSize
				//	, " , Send IP : ", pUser->GetRemoteIPAddressString()
				//	, " ,Socket Number : ", (int)pUser->m_UserSock, " Room Number : ",(int)pUser->m_RoomNumber)  );
	
				IOCPMgr->PrePacketProcessing(pUser, ByteSize);
				// ��Ŷ �ؼ��� PingPacket�� �� ��� Ŭ���̾�Ʈ ������ �ٷ� PongPacket�� �����Ѵ�.
				if (true == pUser->DispatchPacket(ByteSize)) 
				{
					pUser->SendPong();
				}
				IOCPMgr->RecvUserPacket(pUser);
				pUser->RecvPacket();
			}
		}
		break;
		case CServerUser::WORK_TYPE::WORK_SEND:
		{

		}
		break;
		case CServerUser::WORK_TYPE::WORK_ACCEPT:
		{
			pOverPart->m_eType = CServerUser::WORK_TYPE::WORK_CONNET;
			IOCPMgr->PrintLog(LogManager::MakeLogString("Complete Accept Socket - Socket Number : ", (int)pOverPart->m_UserSock));

		}
		break;
		case CServerUser::WORK_TYPE::WORK_REUSE:
		{
			int Return = AcceptEx(SSDServerCore::AcceptSock, pUser->m_UserSock
				, &pUser->m_RecvBuffer.m_Packet, 0
				, sizeof(sockaddr_in) + 16, sizeof(sockaddr_in) + 16
				, &pUser->m_RecvBuffer.m_PacketSize, &pUser->m_RecvBuffer.m_Over);

			if (TRUE != Return)
			{
				DWORD errCode = WSAGetLastError();

				if (ERROR_IO_PENDING != errCode)
				{
					LPVOID lpMsgBuf = nullptr;
					::FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, errCode,
						MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&lpMsgBuf, 0, NULL);
					std::wstring LastError = (wchar_t*)lpMsgBuf;
					LocalFree(lpMsgBuf);

					assert(nullptr);
				}

			}

			pOverPart->m_eType = CServerUser::WORK_TYPE::WORK_CONNET;
			IOCPMgr->PrintLog(LogManager::MakeLogString("Complete Reuse Socket - Socket Number : ", (int)pOverPart->m_UserSock));

		}
		break;
		default:
			break;
		}
	}

	return 0;
}

CServerNetworkManager::CServerNetworkManager()
	:m_NetworkIOCP(nullptr), m_hNetIOCP(NULL), m_pIOCPMgr(nullptr)
{

}
CServerNetworkManager::CServerNetworkManager(IOCPServerMgr* _IOCPMgr)
	:m_NetworkIOCP(nullptr), m_hNetIOCP(NULL), m_pIOCPMgr(_IOCPMgr)
{
}


CServerNetworkManager::~CServerNetworkManager()
{
	m_hNetIOCP = NULL;
	DELETE_POINTER(m_NetworkIOCP);
}


void CServerNetworkManager::Init(const unsigned int& _ThreadCount)
{
	m_NetworkIOCP = new CIOCP();
	m_hNetIOCP = m_NetworkIOCP->CreateIOCP();


	for (size_t i = 0; i < _ThreadCount; i++)
	{
		MakeThread(NetworkThreadProcess, m_pIOCPMgr);
		//MakeThread(NetworkThreadProcess, m_pIOCPMgr, THREAD_PRIORITY_HIGHEST);
	}

	IOCPServerMgr::InitComplete();
}

void CServerNetworkManager::Release() 
{
	unsigned int ThreadCount = (unsigned int)m_vecThread.size();

	for (unsigned int i = 0; i < ThreadCount; i++)
	{
		PostQueuedCompletionStatus(m_hNetIOCP, ENDSIGNAL, ENDSIGNAL, ENDSIGNAL);
	}

	CThreadMaker::Release();
}

