#pragma once



#ifdef X32
#pragma comment(lib, "fmod_vc.lib")
#else
#pragma comment(lib, "fmod64_vc.lib")
#endif

#include<fmod.hpp>

#ifdef X32
#ifdef _DEBUG
#pragma comment(lib, "FRCoreD.lib")
#else
#pragma comment(lib, "FRCoreR.lib")
#endif
#endif

#ifdef X64 
#ifdef _DEBUG
#pragma comment(lib, "FRCoreDx64.lib")
#else
#pragma comment(lib, "FRCoreRx64.lib")
#endif
#endif

#include<UserHeader.h>

