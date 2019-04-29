#pragma once

#define SOUNDNAMELENGTH 64
#define EFFECTNAMELENGTH 64
#define MAXKEYLENGTH 128
#define MAXSTRING 256
#define LARGEFLOAT	100000000.0f
#define MAXLEVEL 100



#define DELTATIME CTimerMgr::DeltaTime()

#define RESMGR GetResourceMgr()

// GameCom 전용 매크로
#define ACTOR CGameCom::ParentActObject()
#define TRANS CGameCom::TransForm()
#define COMRESMGR CGameCom::GetComResourceMgr()
#define COMDEBUGMGR CGameCom::GetComDebugMgr()
#define COMDEV	CGameCom::GetComDevice()

// 눌린 순간
#define KEYDOWN(NAME) CKeyMgr::GetPush(_T(##NAME##))
// 눌리고 있는지
#define KEYPRESS(NAME) CKeyMgr::GetKey(_T(##NAME##))
// 키가 눌렸는지만을 확인
#define KEY(NAME) CKeyMgr::GetKeyOrPush(_T(##NAME##))
// 키가 눌려졌다 띄어졌는가
#define KEYUP(NAME) CKeyMgr::GetUp(_T(##NAME##))
// 키가 두번째 눌린 순간인가
#define KEYDOUBLE(NAME) CKeyMgr::GetDouble(_T(##NAME##))

