#pragma once


enum RENDERSTYLE 
{
	SPRITESIZE,
	ACTSIZE,
};

enum ANISTYLE
{
	ANI_DEFAULT,
	ANI_COUNTCYCLE_REPEAT,		// 지정한 애니메이션 수만큼 전체 프레임 회전 수행후 지정된 인덱스 Range로 반복재생
	ANI_LOOP,
	MAXANISTYLE
};


enum COLTYPE 
{
	CT_RECT,
	CT_CIRCLE,
	CT_POINT,
	CT_OBB,
	CT_MAX,
};


enum COLMODE
{
	CM_ACTOR,
	CM_CUSTOM,
};

enum ANISPRITETYPE 
{
	  ANI_CHARACTER
	, ANI_MONSTER
	, ANI_EFFECT
	, ANI_ETC
	, MAXANISPRITETYPE

};

enum FOLDER
{
	CHARACTER,
	MONSTER,
	EFFECT,
	UI,
	MAP,
	FOLDERMAX,
	UNSPECIFIED 
};

enum LOADINGFILE
{
	PNG,
	JPG,
	FILEMAX
};



enum LOADINGSOUND 
{
	WAV,
	MP3,
	MP4,
	MAXSOUND,

};

enum RENDERLAYER 
{
	LAYER_BACKGROUND,
	LAYER_MAPOBJECT,
	LAYER_MONBACK,
	LAYER_MONSTER,
	LAYER_BACKEFFECT,
	LAYER_PLAYER,
	LAYER_FRONTEFFECT,
	LAYER_UIBACK,
	LAYER_UI,
	LAYER_MOUSE,
	LAYER_CAMERASHIELD,
	MAX_LAYER
};

enum PROJECTIONMODE
{
	ORTHOGONAL,		// 직교 투영
	PERSPECTIVE,	// 원근 투영
};

enum AXIS
{
	TANGENT,
	BITANGENT,
	NORMAL,
};

enum MAINSTAT 
{
	STRENGTH,
	DEXERITY,
	INTELIGENCE,
	LUCK,
	MAXMAINSTAT,
};

enum REACTION 
{
	REACTION_DEFAULT,
	REACTION_VIBRATION,
	REACTION_RED_VIB,
	REACTION_COLD,
	MAXREACTION
};

enum ANIMATIONNAME
{
	ANINAME_IDLE,
	ANINAME_STAND,
	ANINAME_MOVE,
	ANINAME_ATTACK,
	ANINAME_LIMIT,
	ANINAME_DEAD,
	ANINAME_DYING,
	ANINAME_MAGICATTACK,
	ANINAME_MAGICSTAND,
	ANINAME_WIN,
	ANINAME_WIN_BEFORE,
	ANINAME_ATTACK2,
	ANINAME_DOUBLEATTACK,
	MAXANINAME
};

enum SCENEEFFECT 
{
	SE_EARTHQUAKE,
	SE_BLACK_OUT,
	SE_FLASH,
	SE_FADE_IN,
	SE_FADE_OUT,
	SE_ZOOM_IN,
	SE_ZOOM_OUT,
	MAXSCENEEFFECT,
};

enum HITSOUND
{
	SWORDHIT1,
	SWORDHIT2,
	SWORDHIT3,
	SWORDHIT4,
	HIT1,
	HIT2,
	ELECTRICHIT1,
	HIT3,
	HIT4,
	HIT5,
	HIT6,
	HIT7,
	ELECTRICHIT2,
	FLAMEHIT1,
	HIT8,
	EXPLOSIONHIT1,
	ELECTRICHIT3,
	EXPLOSIONHIT2,
	HIT9,
	ELECTRICHIT4,
	MAXHITSOUND ,
};

enum DEMAGEFONT 
{
	DEMAGE_BLACK,
	DEMAGE_WHITE,
	DEMAGE_SP,
	DEMAGE_RED,
	DEMAGE_PHY_CRITICAL = 3,
	DEMAGE_BLUE,
	DEMAGE_MAG_CRITICAL = 4,
	DEMAGE_YELLOW = 5,
	MAXDEMAGEFONT,
};

enum TARGET
{
	TARGET_SINGLE,
	TARGET_FORWARDCOLUMN,		// 앞 줄
	TARGET_REARCOLUMN,			// 뒷 줄
	TARGET_ALL,
	MAXTARGET
};

enum CharacterIndex
{
	DemonRain, Rain2, Rain5, Vargas,	// DoubleAtk 가능
	Chizuru1, Chizuru3, CupidArtemios, CupidLuna1, CupidLuna3, Dean, Lid, Ling, Sakura, Zyrus1, Zyrus3,	// DoubleAtk 불가능
	MAXBATTLECHARACTER,
};

enum CHARACTERLINE 
{
	RAIN,
	CHIZURU,
	LUNA,
	ZYRUS,
	LID,
	LING,
	SAKURA,
	VARGAS,
	DEAN,
	ARTEMIOS,
	MAXCHARACTERLINE,
};

//Demage Font 가 어떻게 랜더링 될지 옵션값
enum DEMAGE_STYLE 
{
	DEMAGESTYLE_EXPANSION,		// 팽창 + 알파감소
	DEMAGESTYLE_STAIRS,			// 계단식
	MAXDEMAGESTYLE,
};


enum CAMERAMODE 
{
	CAM_MODE_FIX,
	CAM_MODE_TRACKING,			//	타겟이 무조건 중심에 오도록 위치를 설정하는 모드
	CAM_MODE_AREA_TRACKING,		//  일정 영역내에 벗어난경우 카메라위치를 조정하는 모드
	MAXCAMERAMODE,
};

enum BGMSOUND 
{
	BATTLE0_SOUND,				// 일반 전투 BGM
	BATTLE1_SOUND,
	BATTLE2_SOUND,
	BATTLE3_SOUND,
	VICTORY_SOUND,
	MAXBATTLE_SOUND,
};


enum TILEOPTION
{
	STEPTILE,
	STAIRTILE,
	STEPSTAIRTILE,
	MAPCHANGETILE,
	WORLDMAPTILE,					// 월드맵을 띄워주는 타일
	NOENTERTILE,
	MAXTILETOPTION,
};


enum MAINMAPNAME
{
	TOWN,
	RUINS,
	FOREST,
	DEVILCASTLE,
	WEANPONSTORE,
	ITEMSTORE,
	MAXMAINMAPNAME,
};

