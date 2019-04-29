#include "Precom.h"
#include "ClientGlobalFunc.h"
#include<GameMultiSprite.h>
#include<GameSprite.h>
#include<GameSound.h>
#include"BattleUnit.h"
#include"TurnOrderUI.h"
#include"MenuSkillUI.h"


const bool operator>(const TileMapKey& _Left, const TileMapKey& _Right)
{
	return _Left.CompareValue > _Right.CompareValue;
}
const bool operator>=(const TileMapKey& _Left, const TileMapKey& _Right)
{
	return _Left.CompareValue >= _Right.CompareValue;
}
const bool operator<(const TileMapKey& _Left, const TileMapKey& _Right)
{
	return _Left.CompareValue < _Right.CompareValue;
}
const bool operator<=(const TileMapKey& _Left, const TileMapKey& _Right)
{
	return _Left.CompareValue <= _Right.CompareValue;
}
const bool operator==(const TileMapKey& _Left, const TileMapKey& _Right)
{
	return _Left.CompareValue == _Right.CompareValue;
}
const bool operator!=(const TileMapKey& _Left, const TileMapKey& _Right)
{
	return _Left.CompareValue != _Right.CompareValue;
}

tstring Idle(const tstring& _UnitName)
{
	return _UnitName + _T("_idle");
}
tstring Atk(const tstring& _UnitName)
{
	if (true == CClientGlobal::CheckDoubleAtkCharacter(_UnitName)) 
	{
		return _UnitName + _T("_atk1");
	}
	return _UnitName + _T("_atk");

}
tstring Atk2(const tstring& _UnitName) 
{
	if (true == CClientGlobal::CheckDoubleAtkCharacter(_UnitName))
	{
		return _UnitName + _T("_atk2");
	}
	return _UnitName + _T("_atk");
}
tstring DAtk(const tstring& _UnitName)
{
	if (true == CClientGlobal::CheckDoubleAtkCharacter(_UnitName))
	{
		return _UnitName + _T("_DoubleAtk");
	}
	return _UnitName + _T("_atk");

}
tstring Dead(const tstring& _UnitName)
{
	return _UnitName + _T("_dead");
}
tstring Dying(const tstring& _UnitName)
{
	return _UnitName + _T("_dying");
}
tstring Jump(const tstring& _UnitName)
{
	return _UnitName + _T("_jump");
}
tstring Move(const tstring& _UnitName)
{
	return _UnitName + _T("_move");
}
tstring Limit(const tstring& _UnitName)
{
	return _UnitName + _T("_limit_atk");
}
tstring MAtk(const tstring& _UnitName)
{
	return _UnitName + _T("_magic_atk");
}
tstring MStand(const tstring& _UnitName)
{
	return _UnitName + _T("_magic_standby");
}
tstring Stand(const tstring& _UnitName)
{
	return _UnitName + _T("_standby");
}
tstring Win(const tstring& _UnitName)
{
	return _UnitName + _T("_win");
}
tstring WinBe(const tstring& _UnitName)
{
	return _UnitName + _T("_win_before");
}
tstring Icon(const tstring& _UnitName)
{
	return _T("unit_icon_") + _UnitName;
}
tstring Frame(const tstring& _UnitName)
{
	return _T("Frame_") + _UnitName;
}
tstring Illust(const tstring& _UnitName)
{
	return _T("unit_ills_") + _UnitName;
}

ANIMATIONDATA* ANI(const tstring& _AniKey)
{
	return CClientGlobal::GetAniData(_AniKey);
}
ANIMATIONDATA* Ani_Idle(const tstring& _Name)
{
	return CClientGlobal::GetAniData(_Name + _T("_idle"));
}
ANIMATIONDATA* Ani_Atk(const tstring& _Name)
{
	return CClientGlobal::GetAniData(_Name + _T("_atk"));
}
ANIMATIONDATA* Ani_Atk1(const tstring& _Name)
{
	return CClientGlobal::GetAniData(_Name + _T("_atk1"));
}
ANIMATIONDATA* Ani_Atk2(const tstring& _Name)
{
	return CClientGlobal::GetAniData(_Name + _T("_atk2"));
}
ANIMATIONDATA* Ani_DAtk(const tstring& _Name)
{
	return CClientGlobal::GetAniData(_Name + _T("_DoubleAtk"));
}
ANIMATIONDATA* Ani_Limit(const tstring& _Name)
{
	return CClientGlobal::GetAniData(_Name + _T("_limit_atk"));
}
ANIMATIONDATA* Ani_Dead(const tstring& _Name)
{
	return CClientGlobal::GetAniData(_Name + _T("_dead"));
}
ANIMATIONDATA* Ani_Dying(const tstring& _Name)
{
	return CClientGlobal::GetAniData(_Name + _T("_dying"));
}
ANIMATIONDATA* Ani_Move(const tstring& _Name)
{
	return CClientGlobal::GetAniData(_Name + _T("_move"));
}
ANIMATIONDATA* Ani_Jump(const tstring& _Name)
{
	return CClientGlobal::GetAniData(_Name + _T("_jump"));
}
ANIMATIONDATA* Ani_MAtk(const tstring& _Name)
{
	return CClientGlobal::GetAniData(_Name + _T("_magic_atk"));
}
ANIMATIONDATA* Ani_MStand(const tstring& _Name)
{
	return CClientGlobal::GetAniData(_Name + _T("_magic_standby"));
}
ANIMATIONDATA* Ani_Stand(const tstring& _Name)
{
	return CClientGlobal::GetAniData(_Name + _T("_standby"));
}
ANIMATIONDATA* Ani_Win(const tstring& _Name)
{
	return CClientGlobal::GetAniData(_Name + _T("_win"));
}
ANIMATIONDATA* Ani_WinBe(const tstring& _Name)
{
	return CClientGlobal::GetAniData(_Name + _T("_win_before"));
}
POINT operator+(const POINT& _Left, const POINT& _Right)
{
	LONG X = _Left.x + _Right.x;
	LONG Y = _Left.y + _Right.y;
	return POINT{ X, Y };
}
POINT operator-(const POINT& _Left, const POINT& _Right)
{
	LONG X = _Left.x - _Right.x;
	LONG Y = _Left.y - _Right.y;
	return POINT{ X, Y };
}


void AllTextureLoading(void* _Arg)
{
	for (size_t i = 0; i < CClientGlobal::FolderName.size(); i++)
	{
		CClientGlobal::MainWindow->GetResourceMgr()->LoadTextureInFolder(CClientGlobal::FolderName[i]);
	}

}
void TexThreadLoading(tstring* _FolderKey) 
{
	if (nullptr == _FolderKey) 
	{
		return;
	}

	CClientGlobal::MainWindow->GetResourceMgr()->LoadTextureInFolder(*_FolderKey);
}
CGameSound* SoundLoad(FMOD::System* _Sys, const tstring& _SoundPath, unordered_map<tstring, SPTR<CGameSound>>& _TempMap) 
{
	if (nullptr == _Sys)
		return nullptr;

	if (nullptr == &_TempMap)
		return nullptr;

	tstring SoundKey = CFilePathMgr::PathToFileName(_SoundPath);
	
	unordered_map<tstring, SPTR<CGameSound>>::iterator Find = _TempMap.find(SoundKey);

	if (Find != _TempMap.end())
		return nullptr;

	CGameSound* NewSound = CGameSound::CreateGameSound(_Sys, _SoundPath);

	if (nullptr == NewSound)
	{
		return nullptr;
	}

	_TempMap.insert(unordered_map<tstring, SPTR<CGameSound>>::value_type(SoundKey, NewSound));

	return NewSound;
}
void SoundThreadLoading(CResourceMgr* _ResMgr)
{
	if (nullptr == _ResMgr) 
	{
		return;
	}

	tstring Path = CFilePathMgr::GetPath(_T("Sound"));

	if (_T("") == Path)
	{
		return;
	}


	_tfinddata_t Fd;

	FMOD::System* SoundSys = CResourceMgr::GetSoundSystem();
	unordered_map<tstring, SPTR<CGameSound>> TempMap;

	TempMap.clear();

	for (int Index = (int)LOADINGSOUND::WAV; Index < (int)LOADINGSOUND::MAXSOUND; ++Index)
	{
		tstring FilePath = Path + CFilePathMgr::SoundFile[Index];
		
		intptr_t handle = _tfindfirst(FilePath.c_str(), &Fd);

		if (handle == -1)
		{
			continue;
		}

		int Check = 0;

		do
		{
			tstring FileName = Fd.name;
			//_ResMgr->SoundLoad(Path +FileName,TempMap);
			SoundLoad(SoundSys, Path + FileName, TempMap);
			Check = _tfindnext(handle, &Fd);

		} while (Check != -1);
	}

	Path = CFilePathMgr::GetPath(_T("SkillSound"));

	if (_T("") == Path)
	{
		return;
	}

	for (int Index = (int)LOADINGSOUND::WAV; Index < (int)LOADINGSOUND::MAXSOUND; ++Index)
	{
		tstring FilePath = Path + CFilePathMgr::SoundFile[Index];

		intptr_t handle = _tfindfirst(FilePath.c_str(), &Fd);

		if (handle == -1)
		{
			continue;
		}

		int Check = 0;

		do
		{
			tstring FileName = Fd.name;
			//_ResMgr->SoundLoad(Path +FileName,TempMap);
			SoundLoad(SoundSys, Path + FileName, TempMap);
			Check = _tfindnext(handle, &Fd);

		} while (Check != -1);
	}


	Path = CFilePathMgr::GetPath(_T("Bgm"));

	if (_T("") == Path)
	{
		return;
	}

	for (int Index = (int)LOADINGSOUND::WAV; Index < (int)LOADINGSOUND::MAXSOUND; ++Index)
	{
		tstring FilePath = Path + CFilePathMgr::SoundFile[Index];

		intptr_t handle = _tfindfirst(FilePath.c_str(), &Fd);

		if (handle == -1)
		{
			continue;
		}

		int Check = 0;

		do
		{
			tstring FileName = Fd.name;
			//_ResMgr->SoundLoad(Path +FileName,TempMap);
			SoundLoad(SoundSys, Path + FileName, TempMap);
			Check = _tfindnext(handle, &Fd);

		} while (Check != -1);
	}

	CCriSectionObject<CResourceMgr>();
	_ResMgr->MergeSoundMap(TempMap);
}
void AniThreadLoading(void* _Arg)
{
	CClientGlobal::LoadAnimationData(ANI_CHARACTER);
	CClientGlobal::LoadAnimationData(ANI_MONSTER);
	CClientGlobal::LoadAnimationData(ANI_EFFECT);
}
void MultiDataThreadLoading(void* _Arg)
{
	CClientGlobal::LoadMultiSpriteData(ANI_CHARACTER);
	CClientGlobal::LoadMultiSpriteData(ANI_MONSTER);
	CClientGlobal::LoadMultiSpriteData(ANI_EFFECT);
}
void StatDataThreadLoading(void* _Arg) 
{
	CClientGlobal::LoadStatData();
}
void MonsterDataThreadLoading(void* _Arg) 
{
	CClientGlobal::LoadMonsterData();
}
void EffectDataThreadLoading(void* _Arg) 
{
	CClientGlobal::LoadEffectData();
}
void SkillDataThreadLoading(void* _Arg) 
{
	CClientGlobal::LoadSkillData();
}
void ItemDataThreadLoading(void* _Arg) 
{
	CClientGlobal::LoadItemData();
}
//void SplashImageRendering(void* _Arg)
//{
//	if (nullptr != CClientGlobal::SplashBuilder)
//		CClientGlobal::SplashBuilder->SplashImageRendering();
//
//	return;
//}
tstring GetAnimationName(const ANIMATIONNAME& _AniNameIndex, const tstring& _CharName) 
{
	if (false == CClientGlobal::CheckCharName(_CharName)) 
	{
		return _T("");
	}
	
	switch (_AniNameIndex)
	{
	case ANINAME_IDLE:
		return Idle(_CharName);
	case ANINAME_STAND:
		return Stand(_CharName);
	case ANINAME_MOVE:
		return Move(_CharName);
	case ANINAME_ATTACK:
		return Atk(_CharName);
	case ANINAME_LIMIT:
		return Limit(_CharName);
	case ANINAME_DEAD:
		return Dead(_CharName);
	case ANINAME_DYING:
		return Dying(_CharName);
	case ANINAME_MAGICATTACK:
		return MAtk(_CharName);
	case ANINAME_MAGICSTAND:
		return MStand(_CharName);
	case ANINAME_WIN:
		return Win(_CharName);
	case ANINAME_WIN_BEFORE:
		return WinBe(_CharName);
	case ANINAME_ATTACK2:
		return Atk2(_CharName);
	case ANINAME_DOUBLEATTACK:
		return DAtk(_CharName);
	}
	return _T("");
}
ANIMATIONNAME GetAniNameIndex(const tstring& _CharAniName) 
{
	size_t Index = _CharAniName.find_first_of(_T("_"));

	if (-1 == Index)
	{
		return MAXANINAME;
	}

	tstring AniName = _CharAniName;
	AniName.replace(0, Index + 1, _T(""));

	map<tstring,ANIMATIONNAME>::iterator Find = CClientGlobal::CharacterAniIndex.find(AniName);

	if (Find != CClientGlobal::CharacterAniIndex.end()) 
	{
		return MAXANINAME;
	}

	return Find->second;
}
tstring GetAnimationName(const int& _AniNameIndex, const tstring& _CharName) 
{
	return GetAnimationName((ANIMATIONNAME)_AniNameIndex, _CharName);
}
tstring GetAnimationName(const int& _AniNameIndex, const int& _CharIndex) 
{
	if ((int)CClientGlobal::CharacterName.size() <= _CharIndex) 
	{
		return _T("");
	}

	return GetAnimationName((ANIMATIONNAME)_AniNameIndex, CClientGlobal::CharacterName[_CharIndex].UnitName);
}
const float DefReductionRate(const int& _Def) 
{
	// Def 계수				
	// 0~50		: 0.9(-10%)	
	// 50~100	: 0.8(-20%)	
	// 100~150	: 0.7(-30%)	
	// 150~200	: 0.6(-40%)	
	// 200~		: 0.5(-50%)	

	if (_Def < 50)
		return 0.9f;

	if (_Def >= 50 && _Def < 100)
		return 0.8f;

	if (_Def >= 100 && _Def < 150)
		return 0.7f;

	if (_Def >= 150 && _Def < 200)
		return 0.6f;

	return 0.5f;
}
const float LevelBonusRate(const int& _LevelDiff) 
{
	// 레벨 차이 보너스 최대 1.5배

	if (_LevelDiff >= 25)
		return 1.5f;

	if (_LevelDiff >= 20 &&_LevelDiff < 25)
		return 1.4f;

	if (_LevelDiff >= 15 && _LevelDiff < 20)
		return 1.3f;

	if (_LevelDiff >= 10 && _LevelDiff < 15)
		return 1.2f;

	if (_LevelDiff >= 5 && _LevelDiff < 10)
		return 1.1f;
		
	return 1.0f;
}
bool operator==(const POINT& _Left, const POINT& _Right) 
{
	if (_Left.x != _Right.x)
		return false;

	if (_Left.y != _Right.y)
		return false;

	return  true;
}
const bool ChangeMasterMode()
{
	CClientGlobal::MasterMode = !CClientGlobal::MasterMode;

	bool Check;

	if (true == CClientGlobal::MasterMode)
		Check = CClientGlobal::MainWindow->ChangeWndTitle(_T("ProjectFR[MasterMode]"));
	else
		Check = CClientGlobal::MainWindow->ChangeWndTitle(_T("ProjectFR"));


#ifdef _DEBUG
	MessageBeep(MB_OK);
#else
#endif
	return Check;
}
const bool operator<(SPTR<CTurnOrderUI> _Left, SPTR<CTurnOrderUI> _Right) 
{
	if (nullptr == _Left || nullptr == _Right)
	{
		assert(nullptr);
		return false;
	}

	if (nullptr == _Left->GetBattleUnit() || nullptr == _Right->GetBattleUnit())
	{
		assert(nullptr);
		return false;
	}

	UINT Left = _Left->GetBattleUnit()->GetCurStat().Dex;
	UINT Right = _Right->GetBattleUnit()->GetCurStat().Dex;

	return Left < Right;

}
const bool operator>(SPTR<CTurnOrderUI> _Left, SPTR<CTurnOrderUI> _Right) 
{
	if (nullptr == _Left || nullptr == _Right)
	{
		assert(nullptr);
		return false;
	}

	if (nullptr == _Left->GetBattleUnit() || nullptr == _Right->GetBattleUnit())
	{
		assert(nullptr);
		return false;
	}

	UINT Left = _Left->GetBattleUnit()->GetCurStat().Dex;
	UINT Right = _Right->GetBattleUnit()->GetCurStat().Dex;

	return Left > Right;
}

void ITEM::operator=(const ITEM& _Other)
{
	vecMountableLineNo.clear();

	ItemNo = _Other.ItemNo;
	ItemIconIndex = _Other.ItemIconIndex;
	ItemType = _Other.ItemType;
	ItemCategory = _Other.ItemCategory;
	ItemStat = _Other.ItemStat;
	DemageRange = _Other.DemageRange;
	ItemDataBuf = _Other.ItemDataBuf;
	ItemKName = _Other.ItemKName;
	ItemDescription = _Other.ItemDescription;
	ItemPrice = _Other.ItemPrice;

	if (true == _Other.vecMountableLineNo.empty())
		return;

	vecMountableLineNo.assign(_Other.vecMountableLineNo.begin(), _Other.vecMountableLineNo.end());
}
const bool ITEM::CheckMountItem(const CHARACTERLINE& _LineNo)
{
	if (TYPE_EQUIPMENT != ItemType)
		return false; 

	if (true == vecMountableLineNo.empty())
		return true;

	for (size_t i = 0; i < vecMountableLineNo.size(); i++)
	{
		if (_LineNo == vecMountableLineNo[i])
			return true;
	}

	return false;
}
BOOL ITEM::LoadItemData(FILE* _ReadFile, std::list<ITEM>* _ItemList)
{
	if (nullptr == _ReadFile || nullptr == _ItemList)
		return FALSE;

	CHAR Buf[MAXSTRING];
	// 첫줄은 안내용 데이터이므로 빼놓는다.
	fgets(Buf, MAXSTRING, _ReadFile);

	while (!feof(_ReadFile))
	{
		fgets(Buf, MAXSTRING, _ReadFile);

		ITEM TempData(Buf);

		TempData.DecodeItemData();
		_ItemList->push_back(TempData);
	}

	return TRUE;
}
string ITEM::GetPriceString()
{
	return to_string(ItemPrice);
}
void ITEM::DecodeItemData()
{
	tstring DataString = ItemDataBuf;
	tstring Temp = _T("");

	size_t Start = 0;
	size_t End = 0;

	Start = DataString.find_first_of('[');
	End = DataString.find_first_of(']');

	// 1. 아이템 번호, 아이콘 번호
	Temp.assign(DataString, Start + 1, End - Start - 1);
	ItemNo = _wtoi(Temp.c_str());

	DataString.replace(Start, End - Start + 1, _T(""));

	Temp.assign(DataString, Start + 1, End - Start - 1);
	ItemIconIndex = _wtoi(Temp.c_str());

	DataString.replace(Start, End - Start + 1, _T(""));

	// 2 아이템 이름,아이템 설명
	Start = DataString.find_first_of('[');
	End = DataString.find_first_of(']');
	Temp.assign(DataString, Start + 1, End - Start - 1);
	DataString.replace(Start, End - Start + 1, _T(""));

	ItemKName = Temp;

	Start = DataString.find_first_of('[');
	End = DataString.find_first_of(']');
	Temp.assign(DataString, Start + 1, End - Start - 1);
	DataString.replace(Start, End - Start + 1, _T(""));

	ItemDescription = Temp;

	// 3. 아이템 타입 & 카테고리
	Start = DataString.find_first_of('[');
	End = DataString.find_first_of(']');
	Temp.assign(DataString, Start + 1, End - Start - 1);
	DataString.replace(Start, End - Start + 1, _T(""));

	if (_T("장비") == Temp)
		ItemType = TYPE_EQUIPMENT;
	else if (_T("소모") == Temp)
		ItemType = TYPE_CONSUMPTION;

	Start = DataString.find_first_of('[');
	End = DataString.find_first_of(']');
	Temp.assign(DataString, Start + 1, End - Start - 1);
	DataString.replace(Start, End - Start + 1, _T(""));

	if (_T("무기") == Temp)
		ItemCategory = CATEGORY_WEAPON;
	else if (_T("방어구") == Temp)
		ItemCategory = CATEGORY_ARMOR;
	else if (_T("장신구") == Temp || _T("악세사리") == Temp)
		ItemCategory = CATEGORY_ACCESSORIES;
	else if (_T("소모품") == Temp || _T("소모") == Temp)
		ItemCategory = CATEGORY_CONSUMPTION;
	else if (_T("기타") == Temp )
		ItemCategory = CATEGORY_ETC;


	Start = DataString.find_first_of('[');
	End = DataString.find_first_of(']');
	Temp.assign(DataString, Start + 1, End - Start - 1);
	DataString.replace(Start, End - Start + 1, _T(""));


	// 4. 착용 가능 직업군 설정
	vecMountableLineNo.clear();

	while (true)
	{
		tstring Temp2 = _T("");
		size_t LineNo = -1;

		Start = Temp.find_first_of(',');
		if (-1 == Start)
		{
			LineNo = CClientGlobal::FindCharacterLineNoForLineName(Temp);
			
			if (-1 != LineNo)
				vecMountableLineNo.push_back((CHARACTERLINE)LineNo);
			break;
		}
		else
		{
			Temp2.assign(Temp, 0, Start);

			LineNo = CClientGlobal::FindCharacterLineNoForLineName(Temp2);

			if (-1 != LineNo)
				vecMountableLineNo.push_back((CHARACTERLINE)LineNo);

			Temp.replace(0, Start + 1, _T(""));
		}
	}

	// 5 아이템 스탯
	Start = DataString.find_first_of('[');
	End = DataString.find_first_of(']');
	Temp.assign(DataString, Start + 1, End - Start - 1);
	DataString.replace(Start, End - Start + 1, _T(""));

	size_t Find = -1;
	tstring Str = _T("");
	int No = 0;

	for (size_t i = 0; i < MAXITEMSTAT; i++)
	{
		Find = Temp.find(',');

		if (-1 == Find)
		{
			Str.assign(Temp, 0, Temp.size());
			No = _wtoi(Str.c_str());
			ItemPrice = No;
			break;
		}

		Str.assign(Temp, 0, Find);
		No = _wtoi(Str.c_str());
		Temp.replace(0, Find + 1, _T(""));

		switch (i)
		{
		case ITEM::ITEM_STR:ItemStat.Str = No;	break;
		case ITEM::ITEM_DEX:ItemStat.Dex = No;	break;
		case ITEM::ITEM_INT:ItemStat.Int = No;	break;
		case ITEM::ITEM_LUK:ItemStat.Luck = No; break;
		case ITEM::ITEM_HP:ItemStat.Heath = No; break;
		case ITEM::ITEM_MP:ItemStat.Mana = No;	break;
		case ITEM::ITEM_DMG:ItemStat.Damage = No; break;
		case ITEM::ITEM_DMG_RANGE:DemageRange = No; break;
		case ITEM::ITEM_DEF:ItemStat.Defense = No; break;
		case ITEM::ITEM_CRI:ItemStat.Critical = No; break;
		case ITEM::ITEM_CRIDMG:ItemStat.CriticalDamage = No; break;
		case ITEM::ITEM_AVD:ItemStat.Avoidability = No; break;
		case ITEM::ITEM_EXP:ItemStat.Exp = No;	break;
		case ITEM::ITEM_LEVEL:ItemStat.Level = No;	break;
		case ITEM::ITEM_PRICE:ItemPrice = No;	break;
		}
	}

	
}

void CHARACTERDATA::operator=(const CHARACTERDATA& _Other)
{
	CharacterNo = _Other.CharacterNo;
	CharacterLv = _Other.CharacterLv;

	CurHp = _Other.CurHp;
	MaxHp = _Other.MaxHp;

	CurMp = _Other.CurMp;
	MaxMp = _Other.MaxMp;

	CurExp = _Other.CurExp;
	MaxExp = _Other.MaxExp;

	if (nullptr != _Other.EquipItem)
		memcpy_s(EquipItem, sizeof(size_t)*MAXITEMEQUIP, _Other.EquipItem, sizeof(size_t)*MAXITEMEQUIP);

	int a = 0;
}

CHARACTERDATA::CHARACTERDATA(const CHARACTERDATA& _Other)
	:CharacterNo(_Other.CharacterNo), CharacterLv(_Other.CharacterLv)
	, CurHp(_Other.CurHp), MaxHp(_Other.MaxHp), CurMp(_Other.CurMp)
	, MaxMp(_Other.MaxMp), CurExp(_Other.CurExp), MaxExp(_Other.MaxExp)
{
	if (nullptr != _Other.EquipItem)
		memcpy_s(EquipItem, sizeof(EquipItem), _Other.EquipItem, sizeof(_Other.EquipItem));
}


void NPCDATA::operator=(const NPCDATA& _Other)
{
	NpcName = _Other.NpcName;
	NpcKName = _Other.NpcKName;
	IdleType = _Other.IdleType;
	NpcPoster = _Other.NpcPoster;
}

NPCDATA::NPCDATA()
	:NpcName(_T("")), IdleType(MAXIDLETYPE)
	, NpcPoster(nullptr), NpcKName(_T(""))
{}
NPCDATA::NPCDATA(const tstring& _Name, const IDLETYPE& _Type)
	:NpcName(_Name), IdleType(_Type)
	, NpcPoster(nullptr), NpcKName(_T(""))
{}
NPCDATA::NPCDATA(const tstring& _Name, const tstring& _KName
	, const IDLETYPE& _Type)
	:NpcName(_Name), IdleType(_Type)
	, NpcPoster(nullptr), NpcKName(_KName)
{}
NPCDATA::NPCDATA(const NPCDATA& _Other)
	:NpcName(_Other.NpcName), IdleType(_Other.IdleType)
	, NpcPoster(_Other.NpcPoster), NpcKName(_Other.NpcKName)
{}
NPCDATA::~NPCDATA()
{
	NpcPoster = nullptr;
}
const VERTEX LerpVertax(VERTEX& _StartVtx, VERTEX& _EndVtx,const float& _Weight)
{
	Vec2 LerpTex = Vec2{0.f,0.f};
	Vec3 LerpPos = Vec3{ 0.f,0.f ,0.f};

	D3DXVec2Lerp(&LerpTex, &_StartVtx.vTexCoord, &_EndVtx.vTexCoord, _Weight);
	D3DXVec3Lerp(&LerpPos, &_StartVtx.vPos, &_EndVtx.vPos, _Weight);

	return VERTEX(LerpPos, LerpTex);
}
VERTEX operator*(const float& _Scala, VERTEX& _Vtx) 
{
	return _Vtx*_Scala;
}
bool CompareItemDataCategory(const ItemData& _Left, const ItemData& _Right)
{
	return _Left.ItemCategory < _Right.ItemCategory;
}
