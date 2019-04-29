#pragma once

#ifdef WIN32
#pragma comment(lib, "fmod_vc.lib")
#else
#pragma comment(lib, "fmod64_vc.lib")
#endif

#include<fmod.hpp>
