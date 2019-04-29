#pragma once

#include <SDKDDKVer.h>

#define WIN32_LEAN_AND_MEAN             // ���� ������ �ʴ� ������ Windows ������� �����մϴ�.
// Windows ��� ����:
#include <windows.h>

// C ��Ÿ�� ��� �����Դϴ�.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <atlimage.h>

#include<gdiplus.h>
using namespace Gdiplus;

#pragma comment(lib,"Gdiplus.lib")

#include"ObjBase.h"
#include"DX9Core.h"


#include "ResolutionMgr.h"
#include "TimerMgr.h"
#include "FilePathMgr.h"
#include "KeyMgr.h"
#include "CollisionFunc.h"
#include "GameString.h"
#include "SJMath.h"
#include "SJRandom.h"
#include "GameFile.h"

#include "WindowMgr.h"
#include "GameWindow.h"
#include "SplashWindow.h"
#include"GameScene.h"
#include"GDevice.h"
#include"ResourceMgr.h"
#include"DebugMgr.h"
#include"ThreadMgr.h"

#include"AniRenderer.h"
#include"FixRenderer.h"
#include"FontRenderer.h"
#include"LineRenderer.h"
#include"ScriptRenderer.h"

#include"BaseState.h"
#include"StateMgr.h"

#include"GameCom.h"
#include"ActObject.h"
#include"TransForm.h"
#include"Logic.h"
#include"Renderer.h"
#include"Collision.h"
#include"SoundPlayer.h"
#include"Camera.h"
#include"PersCamera.h"
#include"Light.h"

bool RendereYSort(SPTR<CRenderer> _Left, SPTR<CRenderer> _Right);
