#pragma once
#define PACKET_SIZE 1024
#define PACKET_PAYLOAD 1024 
#define PACKET_DOUDLESIZE 2048
#define STREAM_SIZE 8192

#define EOR 0xdededede

enum HEADER_FLAG
{
	FLAG_PING  = 1,
	FLAG_PONG,
	FLAG_CONNECT,
	FLAG_DISCONNECT,
	FLAG_ENTER_ROOM,
	FLAG_ENTER_ROOM_RANDOM,
	FLAG_ENTER_TEST_ROOM = FLAG_ENTER_ROOM_RANDOM,	//  ���� ������ ���� �׽�Ʈ�� ������ ����
	FLAG_ENTER_LOBBY_ROOM,							//  ���� ���� ���� �� ����
	FLAG_ENTER_ROOM_SUCCESS,						//	ó�� ���� ����(���� ���� ��ġ�� ����)�� ������������ �÷��� ��
	FLAG_ENTER_ROOM_FAIL,							//  ���� ���� ��ġ�� �����Ѱ��
	FLAG_LOBBY_WAITING,								//  ���� �κ񿡼� ��� ���϶�
	FLAG_GAME_PLAYING,								//  ������ ���� ���忡�� �÷��� ���϶�
	FLAG_SCENE_BUILD_COMPLETE,						//  Ŭ���̾�Ʈ�� �����κ��� ��Ŷ�� ���� �� �մ� ���°� �Ǿ����� �˸��� �÷��� ��
	FLAG_REQUEST_ROOM_DATA,							//  �뿡 ������ �������� ������ �����͸� �䱸�ϴ� �÷��� ��
	FLAG_GAME_EXIT_USER,							//  ���ӿ��� �ٸ� ������ �������
	FLAG_REQUEST_ROOM_LIST,							//  ���� Ȱ��ȭ ���� �������� ��û
	FLAG_PLAYING_RULE_SIGNAL,						//  �ΰ��� ���� �� Ŭ���̾�Ʈ�� Rule ��ü���� �����ϴ� ����
	MAX_HEADER_FLAG,
	FLAG_LOGICDATA =128,
	FLAG_DATA_ROOM_WORLD,
	FLAG_DATA_SYNCRO_WORLD,							// ������ �÷��̾���� ���� ���¸� ����ȭ ��Ű�� ������
	FLAG_DATA_ENTER_SURVIVOR,
	FLAG_DATA_ENTER_KILLER,
	FLAG_DATA_TRANSLATE,
	FLAG_DATA_COLLISION,
	FLAG_DATA_LOCALPLAYER_STATE,
	FLAG_DATA_BROADCATE_REMOTE,
	FLAG_DATA_ENTER_TEST,
	FLAG_DATA_ROOMENTER_USER,
	FLAG_DATA_READY,
	FLAG_DATA_READY_CANCLE,
	FLAG_DATA_ROOM_EXIT,
	FLAG_DATA_BUILD_GAMEWORLD,
	FLAG_DATA_WORLD_OBJECT,
	FLAG_DATA_WORLD_BUILD_COMPLETE,
	FLAG_DATA_RESCUEPLAYER,
	FLAG_DATA_HEALINGPLAYER,
	FLAG_DATA_SELFHEALING,
	FLAG_DATA_SUMMONSPLAYER,
	FLAG_DATA_HOOKPLAYER,
	FLAG_DATA_DISSOLVEPLAYER,
	FLAG_DATA_GANERATOR,
	FLAG_DATA_TRAP,
	FLAG_DATA_GANERATOR_COMPLETE,
	FLAG_DATA_HEALING_COMPLETE,
	FLAG_DATA_SELFHEALING_COMPLETE,
	FLAG_DATA_ACCHOOK,
	FLAG_DATA_RESCUED_COMPETE,
	FLAG_DATA_DEAD,
	FLAG_DATA_DEAD_OUT,
	FLAG_DATA_ESCAPE,
	FLAG_DATA_GAME_END,
	MAX_DATA_TYPE_FLAG
};


typedef struct _tagPACKETHEADER
{
	DWORD					Size;
	WORD					HeaderFlag;
	WORD					DataType;
}PacketHeader,PHeader,Header;


typedef struct _tagPACKET
{
	PacketHeader			Header;
	char					Packet[PACKET_SIZE];
	class CServerUser*		pUser;


	void operator=(const _tagPACKET& _Other) 
	{
		Header.Size = _Other.Header.Size;
		Header.HeaderFlag = _Other.Header.HeaderFlag;
		Header.DataType = _Other.Header.DataType;

		CopyMemory(Packet, _Other.Packet, sizeof(Packet));
	}
}PACKET;



template<typename DATA>
const DATA ExtractionData(const PACKET& _Pack)
{
	DATA ReturnData;

	CopyMemory(&ReturnData, _Pack.Packet, sizeof(DATA));
	return ReturnData;
}

// ���� ���� ��Ŷ�� ������ ���� �Լ�
template<typename DATA>
const DATA ExtractionData(const PACKET& _Pack, const size_t& _Size)
{
	DATA ReturnData;

	CopyMemory(&ReturnData, _Pack.Packet, _Size);
	return ReturnData;
}


