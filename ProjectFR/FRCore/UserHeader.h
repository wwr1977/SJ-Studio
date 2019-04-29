#pragma once
#include<Windows.h>
#include<stdio.h>
#include<io.h>
#include<stdlib.h>
#include<tchar.h>
#include<math.h>
#include<random>
#include<string>
#include<typeinfo>

#include <algorithm>
#include <functional>
#include <atlstr.h>
#include <assert.h>

#ifdef WIN32
#pragma comment(lib, "fmod_vc.lib")
#else
#pragma comment(lib, "fmod64_vc.lib")
#endif

#include<fmod.hpp>

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

#include<d3d9.h>
#include<d3dx9.h>


#include<map>
#include<set>
#include<unordered_map>
#include<unordered_set>
#include<list>
#include<stack>
#include<queue>
#include<stack>

#include"SPTR.h"

#include<crtdbg.h>

#include "UserDefine.h"
#include "UserEnum.h"
#include "UserTypedef.h"
#include "UserMacro.h"
#include "UserTemplate.h"
#include "UserGlobalVar.h"
#include "UserStruct.h"

using namespace std;
