#pragma once

#ifdef X32
#pragma comment(lib, "fmod_vc.lib")
#else
#pragma comment(lib, "fmod64_vc.lib")
#endif

#include<fmod.hpp>


#pragma comment(lib , "d3d9.lib")
#pragma comment(lib , "d3dx9.lib")

#include<d3d9.h>
#include<d3dx9.h>

#ifdef X32 
#ifdef _DEBUG
#pragma comment(lib, "FRCoreD.lib")
#else
#pragma comment(lib, "FRCoreR.lib")
#endif
#else 
#ifdef _DEBUG
#pragma comment(lib, "FRCoreDx64.lib")
#else
#pragma comment(lib, "FRCoreRx64.lib")
#endif
#endif


#include <SJ_SplashWindow.h>
#ifdef X32 
#ifdef _DEBUG
#pragma comment(lib, "SJ_SplashWindowD.lib")
#else
#pragma comment(lib, "SJ_SplashWindow.lib")
#endif
#else 
#ifdef _DEBUG
#pragma comment(lib, "SJ_SplashWindowDx64.lib")
#else
#pragma comment(lib, "SJ_SplashWindowx64.lib")
#endif
#endif
