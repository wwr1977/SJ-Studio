#pragma once
#include<UserHeader.h>
enum BATTLEPHASE
{
	PHASE_STANDBY,
	PHASE_SELECT,
	PHASE_ROTATE,
	PHASE_ACTION,
	MAX_PHASE,
};

enum SKILLMGRSTATE 
{
	STATE_INIT,
	STATE_LOADING,
	STATE_COMPLETE,
	MAXSKILLMGRSTATE
};



enum ACTIONICON
{
	AICON_ATTCK,
	AICON_ITEM,
	AICON_BUFF,
	AICON_COMBATLIMIT,
	AICON_MAGICLIMIT,
	MAXACTIONICON
};

enum UNITSTATE
{
	BATTLE_IDLE,
	BATTLE_ACTION,
	BATTLE_REACTION,
};


#define MAXTEAM 5
#define MAXMONSTER 3

tstring Idle(const tstring& _UnitName);
tstring Atk(const tstring& _UnitName);
tstring Atk2(const tstring& _UnitName);
tstring DAtk(const tstring& _UnitName);
tstring Dead(const tstring& _UnitName);
tstring Dying(const tstring& _UnitName);
tstring Jump(const tstring& _UnitName);
tstring Move(const tstring& _UnitName);
tstring Limit(const tstring& _UnitName);
tstring MAtk(const tstring& _UnitName);
tstring MStand(const tstring& _UnitName);
tstring Stand(const tstring& _UnitName);
tstring Win(const tstring& _UnitName);
tstring WinBe(const tstring& _UnitName);
tstring Icon(const tstring& _UnitName);
tstring Frame(const tstring& _UnitName);
tstring Illust(const tstring& _UnitName);

ANIMATIONDATA* ANI(const tstring& _AniKey);
ANIMATIONDATA* Ani_Idle(const tstring& _Name);
ANIMATIONDATA* Ani_Atk(const tstring& _Name);
ANIMATIONDATA* Ani_Atk1(const tstring& _Name);
ANIMATIONDATA* Ani_Atk2(const tstring& _Name);
ANIMATIONDATA* Ani_DAtk(const tstring& _Name);
ANIMATIONDATA* Ani_Limit(const tstring& _Name);
ANIMATIONDATA* Ani_Dead(const tstring& _Name);
ANIMATIONDATA* Ani_Dying(const tstring& _Name);
ANIMATIONDATA* Ani_Move(const tstring& _Name);
ANIMATIONDATA* Ani_Jump(const tstring& _Name);
ANIMATIONDATA* Ani_MAtk(const tstring& _Name);
ANIMATIONDATA* Ani_MStand(const tstring& _Name);
ANIMATIONDATA* Ani_Stand(const tstring& _Name);
ANIMATIONDATA* Ani_Win(const tstring& _Name);
ANIMATIONDATA* Ani_WinBe(const tstring& _Name);

tstring GetAnimationName(const ANIMATIONNAME& _AniNameIndex, const tstring& _CharName);
ANIMATIONNAME GetAniNameIndex(const tstring& _CharAniName);
tstring GetAnimationName(const int& AniNameIndex, const tstring& _CharName);
tstring GetAnimationName(const int& AniNameIndex, const int& _CharIndex);

POINT operator+(const POINT& _Left, const POINT& _Right);
POINT operator-(const POINT& _Left, const POINT& _Right);

void LoadingTexture(FolderData* _FolderData);
void SkillMgrOn(void* _ptr);

bool operator==(const POINT& _Left, const POINT& _Right);
