#pragma once

//enum CharacterIndex
//{
//	DemonRain, Rain2, Rain5, Vargas,	// DoubleAtk 가능
//	Chizuru1, Chizuru3, CupidArtemios, CupidLuna1, CupidLuna3, Dean, Lid, Ling, Sakura, Zyrus1, Zyrus3,	// DoubleAtk 불가능
//	MAXBATTLECHARACTER,
//};
enum STORETYPE
{
	WEAPON_STORE,
	ARMOR_STORE,
	ACCESSORIES_STORE,
	MAXSTORE,
};

enum QUESTTYPE
{
	QUEST_STORY,
	QUEST_EVENT,
	QUEST_ETC,
	MAXQUESTTYPE,
};

enum NPC
{
	NPC_GIRLS,
	NPC_ELDER,
	NPC_LID,
	NPC_DESTROYER,
	NPC_BERAMODE,
	NPC_CARC,
	NPC_MERUS,
	NPC_MAMON,
	NPC_GOLBEZ,
	MAXNPC
};

typedef enum POSITIONUIRENDERINGVERSION
{
	ROOTMENEVER,
	BATTLEPOSITIONVER,
	MAXRENDERINGVERSION
}UIRENDERINGVERSION;

enum FONTPIVOTOPTION
{
	LEFTSTART,
	RIGHTSTART,
	MAXFONTPIVOTOPTION
};

enum ITEMEQUIP
{
	ITEM_WEAPON,
	ITEM_ARMOR,
	ITEM_ACCESSORIES1,
	ITEM_ACCESSORIES2,
	MAXITEMEQUIP,
};


struct MapKeyPair
{
	UINT	PrevMapNo;
	UINT	CurMapNo;

	void operator=(const MapKeyPair& _Other)
	{
		PrevMapNo = _Other.PrevMapNo;
		CurMapNo = _Other.CurMapNo;
	}

	MapKeyPair() :PrevMapNo(-1), CurMapNo(-1) {}
	MapKeyPair(const UINT& _Prev, const UINT& _Cur) :PrevMapNo(_Prev), CurMapNo(_Cur) {}
	MapKeyPair(const MapKeyPair& _Other) :PrevMapNo(_Other.PrevMapNo), CurMapNo(_Other.CurMapNo) {}
	~MapKeyPair() {}
};

union TileMapKey
{
	MapKeyPair Key;
	LONG CompareValue;

	void operator=(const MapKeyPair& _Key)
	{
		Key = _Key;
	}

	bool operator>(const TileMapKey& _Other)
	{
		return CompareValue > _Other.CompareValue;
	}
	bool operator>=(const TileMapKey& _Other)
	{
		return CompareValue >= _Other.CompareValue;
	}
	bool operator<(const TileMapKey& _Other)
	{
		return CompareValue < _Other.CompareValue;
	}
	bool operator<=(const TileMapKey& _Other)
	{
		return CompareValue <= _Other.CompareValue;
	}

	bool operator==(const TileMapKey& _Other)
	{
		return CompareValue == _Other.CompareValue;
	}
	bool operator!=(const TileMapKey& _Other)
	{
		return CompareValue != _Other.CompareValue;
	}
	void operator=(const TileMapKey& _Other) 
	{
		Key = _Other.Key;
	}

	TileMapKey()
	{
		Key.PrevMapNo = -1;
		Key.CurMapNo = -1;
	}

	TileMapKey(const UINT& _Prev, const UINT& _Cur)
	{
		Key.PrevMapNo = _Prev;
		Key.CurMapNo = _Cur;
	}
	TileMapKey(const TileMapKey& _Other)
	{
		Key = _Other.Key;
	}
	TileMapKey(const MapKeyPair& _Pair)
	{
		Key = _Pair;
	}
	~TileMapKey() {}
};

const bool operator>(const TileMapKey& _Left, const TileMapKey& _Right);
const bool operator>=(const TileMapKey& _Left, const TileMapKey& _Right);
const bool operator<(const TileMapKey& _Left, const TileMapKey& _Right);
const bool operator<=(const TileMapKey& _Left, const TileMapKey& _Right);
const bool operator==(const TileMapKey& _Left, const TileMapKey& _Right);
const bool operator!=(const TileMapKey& _Left, const TileMapKey& _Right);

struct BGMDATA 
{
	tstring				BgmName;
	float				Volume;

	void operator=(const BGMDATA& _Other) 
	{
		BgmName = _Other.BgmName;
		Volume = _Other.Volume;
	}

	BGMDATA() :BgmName(_T("")), Volume(0.f){}
	BGMDATA(const tstring& _BgmName,const float& _Volume) :BgmName(_BgmName), Volume(_Volume) {}
	BGMDATA(const BGMDATA& _Other) :BgmName(_Other.BgmName), Volume(_Other.Volume) {}
	~BGMDATA() {}
};

struct UNITNAME 
{
	tstring UnitName;
	tstring UnitKName;
	tstring UnitNickName;	// 별칭

	void operator=(const UNITNAME& _Other) 
	{
		UnitName = _Other.UnitName;
		UnitKName = _Other.UnitKName;
		UnitNickName = _Other.UnitNickName;
	}

	UNITNAME() :UnitName(_T("")), UnitKName(_T("")), UnitNickName(_T("")) {}
	UNITNAME(const tstring& _Name, const tstring& _KName,const tstring& _NName) :UnitName(_Name), UnitKName(_KName) , UnitNickName(_NName){}
	UNITNAME(const UNITNAME& _Other) :UnitName(_Other.UnitName), UnitKName(_Other.UnitKName) ,UnitNickName(_Other.UnitNickName){}
	~UNITNAME() {}

};



struct CHARACTERDATA
{
	size_t				CharacterNo;
	size_t				CharacterLv;
	
	size_t				CurHp;
	size_t				MaxHp;
	
	size_t				CurMp;
	size_t				MaxMp;

	size_t				CurExp;
	size_t				MaxExp;

	size_t				EquipItem[MAXITEMEQUIP];


public:
	void operator=(const CHARACTERDATA& _Other);


	CHARACTERDATA() 
		:CharacterNo(-1), CharacterLv(0), CurHp(0), MaxHp(0)
	, CurMp(0), MaxMp(0), CurExp(0), MaxExp(0)
	{
		for (size_t i = 0; i < MAXITEMEQUIP; i++)
			EquipItem[i] = -1;
	}
	CHARACTERDATA(const size_t& _No, const size_t& _Lv) 
		:CharacterNo(_No), CharacterLv(_Lv), CurHp(0), MaxHp(0)
		, CurMp(0), MaxMp(0), CurExp(0), MaxExp(0)
	{
		for (size_t i = 0; i < MAXITEMEQUIP; i++)
			EquipItem[i] = -1;
	}

	CHARACTERDATA(const CHARACTERDATA& _Other);
	
	~CHARACTERDATA() {}
};

struct CHARACTERLINEDATA
{
	bool								LineOpen;
	vector<CHARACTERDATA>				LineChar;


	void operator=(const CHARACTERLINEDATA& _Other)
	{
		LineOpen = _Other.LineOpen;
		LineChar.clear();
		LineChar.assign(_Other.LineChar.begin(), _Other.LineChar.end());
	}

public:
	CHARACTERLINEDATA():LineOpen(true) { LineChar.clear();	}
	CHARACTERLINEDATA(vector<CHARACTERDATA>* _CharLine,const bool Open = true) :LineOpen(Open)
	{
		LineChar.clear();
		LineChar.assign(_CharLine->begin(), _CharLine->end());
	}
	CHARACTERLINEDATA(const CHARACTERLINEDATA& _Other)
		:LineOpen(_Other.LineOpen)
	{
		LineChar.clear();
		LineChar.assign(_Other.LineChar.begin(), _Other.LineChar.end());
	}
	~CHARACTERLINEDATA() { LineChar.clear();}

};
enum IDLETYPE
{
	FIXIDLE,		// 기본 동작이 고정된 이미지
	ANIIDLE,		// 기본 동작이 애니메이션
	MAXIDLETYPE,
};

struct NPCDATA 
{
	tstring							NpcName;
	tstring							NpcKName;
	SPTR<CGameSprite> 				NpcPoster;
	IDLETYPE						IdleType;
	

	void operator=(const NPCDATA& _Other);

	NPCDATA();
	NPCDATA(const tstring& _Name, const IDLETYPE& _Type);
	NPCDATA(const tstring& _Name, const tstring& _KName, const IDLETYPE& _Type);
	NPCDATA(const NPCDATA& _Other);
	~NPCDATA();
};

enum ITEMTYPE
{
	TYPE_EQUIPMENT,
	TYPE_CONSUMPTION,
	MAXITEMTYPE,
};

enum ITEMCATEGORY
{
	CATEGORY_WEAPON,
	CATEGORY_ARMOR,
	CATEGORY_ACCESSORIES,
	CATEGORY_CONSUMPTION,
	CATEGORY_ETC,
	MAXITEMCATEGORY
};

struct ITEM
{
	enum ITEMSTAT
	{
		ITEM_STR,
		ITEM_DEX,
		ITEM_INT,
		ITEM_LUK,
		ITEM_HP,
		ITEM_MP,
		ITEM_DMG,
		ITEM_DMG_RANGE,
		ITEM_DEF,
		ITEM_CRI,
		ITEM_CRIDMG,
		ITEM_AVD,
		ITEM_EXP,
		ITEM_LEVEL,
		ITEM_PRICE,
		MAXITEMSTAT,
	};
	UINT						ItemNo;
	std::tstring				ItemKName;
	std::tstring				ItemDescription;
	UINT						ItemIconIndex;
	ITEMTYPE					ItemType;
	ITEMCATEGORY				ItemCategory;

	// 장비 아이템 관련 정보
	vector<CHARACTERLINE>		vecMountableLineNo;		// 장착 가능한 직업군 (비어 있을경우 모두 장착 가능)
														// 아이템 스탯내의 레벨은 장착레벨을 의미한다.
	STAT						ItemStat;
	int							DemageRange;			//(10 = 90% ~ 110%)

	// 소모품 아이템 관련 정보
	UINT						ItemPrice;

	//파싱 데이터 임시 버퍼
	std::tstring				ItemDataBuf;
public:
	void operator=(const ITEM& _Other);
	const bool CheckMountItem(const CHARACTERLINE& _LineNo);
	static BOOL LoadItemData(FILE* _File, std::list<ITEM>* _ItemList);
	string GetPriceString();
public:
	void DecodeItemData();

	ITEM() :ItemNo(-1), ItemIconIndex(-1), ItemType(MAXITEMTYPE)
		, ItemCategory(MAXITEMCATEGORY)
		, ItemStat(STAT())
		, DemageRange(0), ItemDataBuf(_T("")), ItemKName(_T(""))
		, ItemDescription(_T("")), ItemPrice(0)
	{}
	ITEM(CHAR* _DataString)
		:ItemNo(-1), ItemIconIndex(-1), ItemType(MAXITEMTYPE)
		, ItemCategory(MAXITEMCATEGORY)
		, ItemStat(STAT())
		, DemageRange(0), ItemDataBuf(_T("")), ItemKName(_T(""))
		, ItemDescription(_T("")), ItemPrice(0)
	{
		ItemDataBuf = CA2W(_DataString);
	}
	ITEM(const ITEM& _Other)
		:ItemNo(_Other.ItemNo), ItemIconIndex(_Other.ItemIconIndex), ItemType(_Other.ItemType)
		, ItemCategory(_Other.ItemCategory)
		, ItemStat(_Other.ItemStat), DemageRange(_Other.DemageRange)
		, ItemDataBuf(_Other.ItemDataBuf)
		, ItemKName(_Other.ItemKName)
		, ItemDescription(_Other.ItemDescription)
		, ItemPrice(_Other.ItemPrice)
	{
		if (false == _Other.vecMountableLineNo.empty())
			vecMountableLineNo.assign(_Other.vecMountableLineNo.begin(), _Other.vecMountableLineNo.end());
	}
	~ITEM() {}
};


enum WORLDUISTATE
{
	UI_NOACTIVE,
	UI_NOFOCUS,
	UI_FOCUS,
	MAXWORLDUISTATE,
};
enum BATTLEPHASE
{
	PHASE_STANDBY,
	PHASE_SELECT,
	PHASE_ROTATE,
	PHASE_ACTION,
	PHASE_CHECK,
	PHASE_RELOCATION,
	PHASE_BATTLEEND,
	MAX_PHASE,
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
	BATTLE_DYING,
	BATTLE_DEAD,
	BATTLE_WIN,
	BATTLE_CHARAPPEAR,
	BATTLE_MONDEAD = 3,
	BATTLE_MONAPPEAR =4,
	MAXUNITSTATE
};
enum PLAYERSTATE
{
	PLAYER_IDLE,
	PLAYER_WALK,
	PLAYER_RUN,
	MAXPLAYERSTATE,
};


enum UNITCOLUMN
{
	CHARACTER_FRONT,
	CHARACTER_REAR,
	MONSTER_FRONT,
	MONSTER_REAR,
	MAXUNITCOLUMN
};

struct ItemData
{
	size_t				ItemNo;			// 아이템 번호
	size_t				ItemCount;		// 아이템 갯수
	size_t				ItemCategory;	// 아이템 카테고리
public:
	void operator=(const ItemData& _Other)
	{
		ItemNo = _Other.ItemNo;
		ItemCount = _Other.ItemCount;
		ItemCategory = _Other.ItemCategory;
	}

public:
	ItemData() :ItemNo(-1), ItemCount(0) , ItemCategory(MAXITEMCATEGORY){}
	ItemData(const size_t& _ItemNo, const size_t& _ItemCount,const size_t _ItemCategory)
		:ItemNo(_ItemNo), ItemCount(_ItemCount),ItemCategory(_ItemCategory)
	{}
	ItemData(const ItemData& _Other)
		:ItemNo(_Other.ItemNo), ItemCount(_Other.ItemCount)
		,ItemCategory(_Other.ItemCategory)
	{}
	~ItemData() {}
};

bool CompareItemDataCategory(const ItemData& _Left, const ItemData& _Right);

#define MAXTEAM 5
#define MAXMONSTER 3
#define MAXROUND 3

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

CGameSound* SoundLoad(const FMOD::System* _Sys, const tstring& _SoundPath,unordered_map<tstring,SPTR<CGameSound>>& _TempMap);
void AllTextureLoading(void* _Arg);
void TexThreadLoading(tstring* _FolderKey);
void SoundThreadLoading(CResourceMgr* _ResMgr);
void AniThreadLoading(void* _Arg);
void MultiDataThreadLoading(void* _Arg);
void StatDataThreadLoading(void* _Arg);
void MonsterDataThreadLoading(void* _Arg);
void EffectDataThreadLoading(void* _Arg);
void SkillDataThreadLoading(void* _Arg);
void ItemDataThreadLoading(void* _Arg);
//void SplashImageRendering(void* _Arg);

const float DefReductionRate(const int& _Def);
const float LevelBonusRate(const int& _LevelDiff);

bool operator==(const POINT& _Left, const POINT& _Right);

const bool ChangeMasterMode();

const bool operator<(SPTR<class CTurnOrderUI> _Left, SPTR<class CTurnOrderUI> _Right);
const bool operator>(SPTR<class CTurnOrderUI> _Left, SPTR<class CTurnOrderUI> _Right);

//bool CompareMenuSkillUI(SPTR<class CMenuSkillUI> _Left, SPTR<CMenuSkillUI> _Right);

const VERTEX LerpVertax(VERTEX& _StartVtx, VERTEX& _EndVtx, const float& _Weight);
VERTEX operator*(const float& _Scala, VERTEX& _Vtx);


