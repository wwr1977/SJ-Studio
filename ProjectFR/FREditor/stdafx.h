
// stdafx.h : ���� ��������� ���� ��������� �ʴ�
// ǥ�� �ý��� ���� ���� �� ������Ʈ ���� ���� ������ 
// ��� �ִ� ���� �����Դϴ�.

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // ���� ������ �ʴ� ������ Windows ������� �����մϴ�.
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // �Ϻ� CString �����ڴ� ��������� ����˴ϴ�.

// MFC�� ���� �κа� ���� ������ ��� �޽����� ���� ����⸦ �����մϴ�.
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC �ٽ� �� ǥ�� ���� ����Դϴ�.
#include <afxext.h>         // MFC Ȯ���Դϴ�.





#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // Internet Explorer 4 ���� ��Ʈ�ѿ� ���� MFC �����Դϴ�.
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // Windows ���� ��Ʈ�ѿ� ���� MFC �����Դϴ�.
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // MFC�� ���� �� ��Ʈ�� ���� ����









#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif



#include <ResolutionMgr.h>
#include <TimerMgr.h>
#include <FilePathMgr.h>
#include <KeyMgr.h>
#include <CollisionFunc.h>
#include <GameString.h>
#include <SJMath.h>
#include <SJRandom.h>

#include<ObjBase.h>
#include<DX9Core.h>

#include <WindowMgr.h>
#include<GameWindow.h>
#include<GameScene.h>
#include<GDevice.h>
#include<ResourceMgr.h>
#include<DebugMgr.h>
#include<ThreadMgr.h>
#include<GameFont.h>
#include<GameFile.h>

#include<AniRenderer.h>
#include<FixRenderer.h>
#include<LineRenderer.h>


#include<BaseState.h>
#include<StateMgr.h>

#include<ActObject.h>
#include<TransForm.h>
#include<Logic.h>
#include<Renderer.h>
#include<Collision.h>
#include<SoundPlayer.h>
#include<Camera.h>
#include<PersCamera.h>

#include"EditGlobal.h"
#include"EditGlobalFunc.h"

