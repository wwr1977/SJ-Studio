#pragma once

#define SOUNDNAMELENGTH 64
#define EFFECTNAMELENGTH 64
#define MAXKEYLENGTH 128
#define MAXSTRING 256
#define LARGEFLOAT	100000000.0f
#define MAXLEVEL 100



#define DELTATIME CTimerMgr::DeltaTime()

#define RESMGR GetResourceMgr()

// GameCom ���� ��ũ��
#define ACTOR CGameCom::ParentActObject()
#define TRANS CGameCom::TransForm()
#define COMRESMGR CGameCom::GetComResourceMgr()
#define COMDEBUGMGR CGameCom::GetComDebugMgr()
#define COMDEV	CGameCom::GetComDevice()

// ���� ����
#define KEYDOWN(NAME) CKeyMgr::GetPush(_T(##NAME##))
// ������ �ִ���
#define KEYPRESS(NAME) CKeyMgr::GetKey(_T(##NAME##))
// Ű�� ���ȴ������� Ȯ��
#define KEY(NAME) CKeyMgr::GetKeyOrPush(_T(##NAME##))
// Ű�� �������� ������°�
#define KEYUP(NAME) CKeyMgr::GetUp(_T(##NAME##))
// Ű�� �ι�° ���� �����ΰ�
#define KEYDOUBLE(NAME) CKeyMgr::GetDouble(_T(##NAME##))

