#pragma once
#include "SSDServerCore.h"
#include "ServerMacro.h"



class CIOCPServer : public SSDServerCore::CServerUpdater
{
private:
	friend class CServerManager;

public:
	static void LogError();


private:
	static HANDLE						IOCPCOM;
	static int							AcceptNumber;
	static UINT							IOCPThreadCount;
	static std::vector<std::thread>		vecThreadPool;
	static std::vector<HANDLE>			vecThreadHandle;
	static CSyncList<CServerUser*>		AllConnectUser;


public:
	static unsigned int _stdcall IOCPThreadProcess(void* _Arg);


private:
	// �� �Լ� ������ ��Ŷó���� �Ϸ������� ���� �Ŵ����� ��Ŷť�� ��Ŷ�� �߰����� �ʴ´�.
	static const bool PrePacketProcessing(CServerUser* _pUser, const UINT& _Size);
	static void PacketProcessing(PACKET& _Pack);

public:
	static void PreUpdate(const float& _DeltaTime);
	static void PostUpdate(const float& _DeltaTime);

private:
	static void ObserverConnectUser(const float& _DeltaTime);

public:
	static const bool ConnectUser(CServerUser::OVERBUFFER* _pOverPart);
	static void DisConnectUser(CServerUser* _pUser);


public:
	void ServerStart() override;
	void ServerEnd()override;

public:
	void Update();



	

public:
	CIOCPServer();
	~CIOCPServer() override;
};

