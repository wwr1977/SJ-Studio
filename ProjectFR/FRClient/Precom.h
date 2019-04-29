#pragma once

#include <SDKDDKVer.h>

#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용은 Windows 헤더에서 제외합니다.
// Windows 헤더 파일:
#include <Windows.h>

// C 런타임 헤더 파일입니다.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <atlimage.h>


#include <ResolutionMgr.h>
#include <TimerMgr.h>
#include <FilePathMgr.h>
#include <KeyMgr.h>
#include <CollisionFunc.h>
#include <SJMath.h>
#include <SJRandom.h>
#include <GameFile.h>

#include<ObjBase.h>
#include<DX9Core.h>

#include <WindowMgr.h>
#include<GameWindow.h>
#include<SplashWindow.h>
#include<GameScene.h>
#include<GDevice.h>
#include<ResourceMgr.h>
#include<DebugMgr.h>
#include<ThreadMgr.h>
#include<GameFont.h>



#include<AniRenderer.h>
#include<FixRenderer.h>
#include<FontRenderer.h>
#include<LineRenderer.h>
#include<ScriptRenderer.h>

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
#include<Light.h>


#include"ClientGlobalFunc.h"
#include"ClientGlobal.h"

