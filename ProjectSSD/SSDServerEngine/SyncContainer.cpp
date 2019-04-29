#include "SyncContainer.h"
#include "SSDServerCore.h"

CPacketQueue::CPacketQueue() 
	: m_SyncObj(CServerCS()), m_iHead(0), m_iTail(0), m_iSize(0), m_iCapacity(0)
	, m_pPacket(nullptr)
{

}
CPacketQueue::~CPacketQueue() 
{
	DELETE_ARRPOINTER(m_pPacket)
}

const bool CPacketQueue::PushUserPacket(CServerUser* _pUser)
{
	LocalCS Lock(&m_SyncObj);

	if (nullptr == _pUser)
		return false;

	bool PushCheck = false;

	while (_pUser->ExistUserPacket()) 
	{
		PACKET UserPacket = _pUser->GetUserPacket();
		int iTail = (m_iTail + 1) % m_iCapacity;
		memcpy(&m_pPacket[iTail], &UserPacket, sizeof(PACKET));
		m_iTail = iTail;
		++m_iSize;
		PushCheck = true;
	}

	return PushCheck;
}
const DWORD CPacketQueue::ExtractionPacketSize(CServerUser* _pUser)
{
	if (nullptr == _pUser)
		return 0;

	DWORD PacketLen = 0;
	CopyMemory(&PacketLen, _pUser->m_RecvBuffer.m_Packet, sizeof(DWORD));
	return PacketLen;
}
const bool CPacketQueue::GetData(CServerUser* _pUser)
{
	LocalCS Lock(&m_SyncObj);

	while (true)
	{
		//���� ���̰� 4����Ʈ ���� ������ ����
		if (m_iRemainData < sizeof(DWORD))
			break;

		// ��Ŷ ���� ������
		int iPacketLen = 0;
		CopyMemory(&iPacketLen, m_PacketStream, sizeof(DWORD));

		//���� ���̰� ��Ŷ���̺��� ������ ����
		if (m_iRemainData < iPacketLen)
			break;

		//��Ŷť�� ������������ ����
		if (Full() == true)
			break;

		//��Ŷ���� ������ ����
		PACKET NewPacket;
		ZeroMemory(&NewPacket, sizeof(PACKET));

		memcpy(&NewPacket, m_PacketStream, iPacketLen);
		NewPacket.pUser = _pUser;

		//��Ŷť�� ����ֱ�
		int iTail = (m_iTail + 1) % m_iCapacity;
		memcpy(&m_pPacket[iTail], &NewPacket, sizeof(PACKET));
		m_iTail = iTail;
		++m_iSize;

		//��Ŷ ��Ʈ������ ��Ŷ��ŭ ������ �̵�
		m_iRemainData -= iPacketLen;
		MoveMemory(m_PacketStream, m_PacketStream + iPacketLen, m_iRemainData);
	}

	return true;
}