#pragma once
#include "SSDServerCore.h"

class CServerManager
{
protected:
	CServerCS								SyncCS;
	CPacketQueue							MainPacketQueue;

public:
	virtual void Update(const float& _DeltaTime) = 0;
	virtual void PushPacket(CServerUser* _pUser,const unsigned int& _DataLength) = 0;
	virtual void PushPacket(CServerUser* _pUser) = 0;
	virtual void DisConnectUser(CServerUser* _pUser) = 0;
public:
	// ���� �⺻���� ó����(��,���� �� ���� �������������� �⺻ ó�� �۾�
	void PacketPreProcessing(PACKET& _Pack);

	

public:
	CServerManager();
	virtual ~CServerManager();
};

