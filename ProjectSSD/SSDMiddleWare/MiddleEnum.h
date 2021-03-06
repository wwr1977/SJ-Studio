#pragma once

// 현재 인트로 씬의 진행 단계
enum INTRO_STEP
{
	INTRO_LOADING,
	INTRO_WAIT_CONNECT,
	INTRO_CONNECT_COMPLETE,
	INTRO_SELECT_ROOM,
	INTRO_WAIT_ROOM_ENTER,
	INTRO_WAIT_ROOM_BUILD,
	MAX_INTRO_STEP,
};

// 현재 게임씬의 로딩 단계
enum ASYNC_LOADING_STEP
{
	ASYNC_WAIT,
	ASYNC_RESOURCE_LOAD,
	ASYNC_PREFAB_LOAD,
	ASYNC_MAP_LOAD,
	ASYNC_PLAYER_CREATE,
	ASYNC_LOAD_COMPLETE,
	MAX_ASYNC_LOADING_STEP
};

enum PLAYER_ROLE_TYPE
{
	PLAYER_SURVIVOR = 1,
	PLAYER_KILLER,
	PLAYER_TEST,
	MAX_PLAYER_ROLE_TYPE,
};

enum PLAYER_NETWORK_TYPE
{
	LOCAL_PLAYER,						//	자신이 조종하는 플레이어 객체 타입(본인 캐릭터)
	REMOTE_PLAYER,						//  서버의 데이터로 움직이는 플레이어 객체 타입(다른 사람 캐릭터)
	MAX_PLAYER_NETWORK_TYPE,
};

enum PLAYER_STATE 
{
	PLAYER_INIT,
	PLAYER_IDLE,
	PLAYER_WALK,
	PLAYER_RUN,
	PLAYER_ATTACK,
	PLAYER_DAMAGED,
	PLAYER_REPIRE,
	PLAYER_HEALING,
	PLAYER_GROGGY,
	PLAYER_SUMMONS,
	PLAYER_GENBREAK,
	PLAYER_SELFHEALING,
	PLAYER_HOOK,
	PLAYER_RESCUE,
	PLAYER_RESCUED,
	PLAYER_DEAD,
	MAX_PLAYER_STATE
};

enum COMMAND_FLAG 
{
	COMMAND_LOBBY_PACKET_PROCESSING,
	COMMAND_PACKET_PROCESSING ,
	COMMAND_GAME_PROCESSING,
	COMMAND_EXIT_USER_PROCESSING,
	MAX_COMMAND_FLAG,
};

enum LOG_FLAG 
{
	LOG_PRINT_CONSOL	= 0x01, 
	LOG_PRINT_FILE		= 0x02,
	MAX_LOG_FLAG,
};

enum LOBBY_UI_TYPE 
{
	ROOM_NUMBER_SIGNATURE,
	ROOM_NUMBER,
	MAX_ROOM_NUMBER_UI,
	ROOM_ACCESS_USER = 0,
	ROOM_USER_SLASH,
	ROOM_MAXACCESS_USER,
	MAX_USER_COUNT_UI,
	ROOM_BUTTON_READY = 0,
	ROOM_BUTTON_EXIT,
	MAX_ROOM_BUTTON_UI,
};

enum TRAP_FLAG 
{
	TRAP_ACTIVE,
	TRAP_STOP,
	TRAP_RECOVERY,
	MAX_TRAP_FLAG
};