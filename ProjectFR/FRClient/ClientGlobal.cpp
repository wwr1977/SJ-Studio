#include "Precom.h"
#include "ClientGlobal.h"
#include "NumberUI.h"
#include "Player.h"
#include "TileMap.h"
#include "InventoryMgr.h"
#include <GameMultiSprite.h>

Mat								CClientGlobal::CurSceneProj;
Mat								CClientGlobal::CurSceneView;
// 줌인이 적용되지 않은 Proj 행렬
Mat								CClientGlobal::CurNoneZoomProj;
// 카메라 행렬이 적용되지 않은 행렬
Mat								CClientGlobal::NoneCameraView;
//SPTR<CCamera>					CClientGlobal::MainCamera = nullptr;


CClientGlobal::SceneIndex		CClientGlobal::PrevScene = LOADINGSCENE;
vector<tstring>					CClientGlobal::FolderName;
vector<tstring>					CClientGlobal::SceneName;
//vector<tstring>					CClientGlobal::CharacterName;
vector<UNITNAME>				CClientGlobal::CharacterName;
set<tstring>					CClientGlobal::MonsterName;

SPTR<CGameWindow>				CClientGlobal::MainWindow = nullptr;
map<tstring, ANIMATIONDATA*>	CClientGlobal::mapAnimationData;
map<tstring, MULTISPRITEDATA*>	CClientGlobal::MultiSpriteData;
SPTR<CActObject>				CClientGlobal::MouseObeject = nullptr;
int								CClientGlobal::AdvanceTeam[MAXTEAM];


UINT							CClientGlobal::MonsterTeam[MAXROUND][MAXMONSTER];
vector<vector<tstring>>			CClientGlobal::strMonsterTeam;
UINT							CClientGlobal::CurBattleEndRound = -1;

map<tstring, vector<STAT>>		CClientGlobal::CharacterStatTable;
map<tstring, STATPARAMETERDATA>	CClientGlobal::CharacterParameter;
map<tstring, ANIMATIONNAME>		CClientGlobal::CharacterAniIndex;

vector<EFFECTDATA>				CClientGlobal::EffectData;
map<tstring, size_t>			CClientGlobal::EffectIndexMap;

unordered_map<tstring, MONSTERDATA>	CClientGlobal::MonsterDataMap;

vector<SKILL>					CClientGlobal::vecSkill;
vector<UINT>					CClientGlobal::vecSkillMaxExp;
vector<ACTIONSCRIPT>			CClientGlobal::vecActionScript;
vector<SKILLUIDATA>				CClientGlobal::vecSkillUIData;

vector<SKILLDATA>				CClientGlobal::vecSkillData;
vector<vector<LEVELDATA>>		CClientGlobal::vecSkillLevelData;


vector<tstring>					CClientGlobal::SkillHitSound;
vector<tstring>					CClientGlobal::DemageFont;

vector<vector<CHARACTERDATA>>	CClientGlobal::CharacterLine;


CTurnMgr*						CClientGlobal::TurnMgr = nullptr;

CGameMaster*					CClientGlobal::GameMaster = nullptr;
CInventoryMgr*					CClientGlobal::InventoryMgr = nullptr;
SPTR<CPlayer>					CClientGlobal::MainPlayer = nullptr;
SPTR<CTileMap>					CClientGlobal::CurTileMap = nullptr;

vector<ITEM>					CClientGlobal::vecItem;

SPTR<CGameMultiSprite>			CClientGlobal::SkillIconMultiSprite = nullptr;
SPTR<CGameMultiSprite>			CClientGlobal::ItemIconMultiSprite = nullptr;

bool							CClientGlobal::MasterMode = false;
vector<UINT>					CClientGlobal::ExpTable;

unordered_map<tstring, Sigmoid>	CClientGlobal::SigmoidMap;

CEndingSceneBuilder*			CClientGlobal::EndingBuilder = nullptr;
CSplashSceneBuilder*			CClientGlobal::SplashBuilder = nullptr;


bool							CClientGlobal::StoryBattle = false;

ANIMATIONDATA* CClientGlobal::FindAnimationData(const tstring& _AnimationDataKey)
{
	return FINDMAP_PTR<ANIMATIONDATA>(_AnimationDataKey, mapAnimationData);

}
BOOL CClientGlobal::ExistAnimationData(const tstring& _AnimationDataKey)
{
	map<tstring, ANIMATIONDATA*>::iterator FindIter = mapAnimationData.find(_AnimationDataKey);

	
	if (FindIter == mapAnimationData.end()) 
	{
		return FALSE;
	}
	return TRUE;
}
void CClientGlobal::Init()
{
	VERTEX LeftUp = VERTEX({ -0.5f, 0.5f, 1.0f }, { 0.0f, 0.0f });
	VERTEX RightUp = VERTEX({ 0.5f, 0.5f, 1.0f }, { 1.0f,0.0f });
	VERTEX RightDown = VERTEX({ 0.5f, -0.5f, 1.0f }, { 1.0f,1.0f });
	VERTEX LeftDown = VERTEX({ -0.5f, -0.5f, 1.0f }, { 0.0f,1.0f });

	int a = 0;

	CharacterName.push_back(UNITNAME(_T("DemonRain"),_T("데몬 레인"),_T("마신의 재림")));
	CharacterName.push_back(UNITNAME(_T("Rain2"),_T("레인"), _T("방랑 용병")));
	CharacterName.push_back(UNITNAME(_T("Rain5"), _T("레인"), _T("여명의 기사 단장")));
	CharacterName.push_back(UNITNAME(_T("Vargas"), _T("베가스"), _T("홍염의 분노")));
	CharacterName.push_back(UNITNAME(_T("Chizuru1"), _T("시즈루"), _T("검객")));
	CharacterName.push_back(UNITNAME(_T("Chizuru3"), _T("시즈루"), _T("검성")));
	CharacterName.push_back(UNITNAME(_T("CupidArtemios"),_T("아르테미오스"), _T("질풍의 눈")));
	CharacterName.push_back(UNITNAME(_T("CupidLuna1"),_T("루나"), _T("궁수")));
	CharacterName.push_back(UNITNAME(_T("CupidLuna3"), _T("루나"), _T("폭풍의 눈")));
	CharacterName.push_back(UNITNAME(_T("Dean"), _T("딘"), _T("파멸의 기사")));
	CharacterName.push_back(UNITNAME(_T("Lid"), _T("리드"), _T("비공정 정비공")));
	CharacterName.push_back(UNITNAME(_T("Ling"), _T("링"), _T("천년 무희")));
	CharacterName.push_back(UNITNAME(_T("Sakura"), _T("사쿠라"), _T("창월의 공주")));
	CharacterName.push_back(UNITNAME(_T("Zyrus1"), _T("자이러스"), _T("마도사")));
	CharacterName.push_back(UNITNAME(_T("Zyrus3"), _T("자이러스"), _T("대 현자")));

	vector<CHARACTERDATA> TempLine;

	TempLine.push_back(CHARACTERDATA(Rain2,10));
	TempLine.push_back(CHARACTERDATA(Rain5, 50));
	TempLine.push_back(CHARACTERDATA(DemonRain, 50));
	CharacterLine.push_back(TempLine);
	
	// test
	//CharacterLine[0][0].EquipItem[ITEM_WEAPON] = 0;
	//CharacterLine[0][0].EquipItem[ITEM_ARMOR] = 9;
	//CharacterLine[0][0].EquipItem[ITEM_ACCESSORIES1] = 20;
	//CharacterLine[0][0].EquipItem[ITEM_ACCESSORIES2] = 21;

	TempLine.clear();

	TempLine.push_back(CHARACTERDATA(Chizuru1, 10));
	TempLine.push_back(CHARACTERDATA(Chizuru3, 50));
	CharacterLine.push_back(TempLine);
	TempLine.clear();

	TempLine.push_back(CHARACTERDATA(CupidLuna1, 10));
	TempLine.push_back(CHARACTERDATA(CupidLuna3, 50));
	CharacterLine.push_back(TempLine);
	
	TempLine.clear();

	TempLine.push_back(CHARACTERDATA(Zyrus1, 10));
	TempLine.push_back(CHARACTERDATA(Zyrus3, 50));
	
	CharacterLine.push_back(TempLine);
	TempLine.clear();

	TempLine.push_back(CHARACTERDATA(Lid, 50));
	CharacterLine.push_back(TempLine);
	TempLine.clear();

	TempLine.push_back(CHARACTERDATA(Ling, 50));
	CharacterLine.push_back(TempLine);
	TempLine.clear();

	TempLine.push_back(CHARACTERDATA(Sakura, 50));
	CharacterLine.push_back(TempLine);
	TempLine.clear();

	TempLine.push_back(CHARACTERDATA(Vargas, 50));
	CharacterLine.push_back(TempLine);
	TempLine.clear();

	TempLine.push_back(CHARACTERDATA(Dean, 50));
	CharacterLine.push_back(TempLine);
	TempLine.clear();

	TempLine.push_back(CHARACTERDATA(CupidArtemios, 50));
	CharacterLine.push_back(TempLine);
	TempLine.clear();



	for (int i = 0; i < MAXTEAM; i++)
		AdvanceTeam[i] = 0;
	

	D3DXMatrixIdentity(&NoneCameraView);

	CRenderer::RendererInit(CClientGlobal::MainWindow->GetLPDevice());

	FolderName.push_back(_T("Character"));
	FolderName.push_back(_T("Monster"));
	FolderName.push_back(_T("Effect"));
	FolderName.push_back(_T("UI"));
	FolderName.push_back(_T("Map"));

	EffectData.reserve(64);

	CNumberUI::InitFontNumberData();

	CharacterAniIndex.clear();
	CharacterAniIndex.insert(map<tstring, ANIMATIONNAME>::value_type(_T("_idle"),ANINAME_IDLE));
	CharacterAniIndex.insert(map<tstring, ANIMATIONNAME>::value_type(_T("_standby"),ANINAME_STAND));
	CharacterAniIndex.insert(map<tstring, ANIMATIONNAME>::value_type(_T("_move") , ANINAME_MOVE));
	CharacterAniIndex.insert(map<tstring, ANIMATIONNAME>::value_type(_T("_atk"), ANINAME_ATTACK));
	CharacterAniIndex.insert(map<tstring, ANIMATIONNAME>::value_type(_T("_atk1"), ANINAME_ATTACK));
	CharacterAniIndex.insert(map<tstring, ANIMATIONNAME>::value_type(_T("_limit_atk"), ANINAME_LIMIT));
	CharacterAniIndex.insert(map<tstring, ANIMATIONNAME>::value_type(_T("_dead"), ANINAME_DEAD));
	CharacterAniIndex.insert(map<tstring, ANIMATIONNAME>::value_type(_T("_dying"), ANINAME_DYING));
	CharacterAniIndex.insert(map<tstring, ANIMATIONNAME>::value_type(_T("_magic_atk"), ANINAME_MAGICATTACK));
	CharacterAniIndex.insert(map<tstring, ANIMATIONNAME>::value_type(_T("_magic_standby"), ANINAME_MAGICSTAND));
	CharacterAniIndex.insert(map<tstring, ANIMATIONNAME>::value_type(_T("_win"), ANINAME_WIN));
	CharacterAniIndex.insert(map<tstring, ANIMATIONNAME>::value_type(_T("_win_before"), ANINAME_WIN_BEFORE));
	CharacterAniIndex.insert(map<tstring, ANIMATIONNAME>::value_type(_T("_atk2"), ANINAME_ATTACK2));
	CharacterAniIndex.insert(map<tstring, ANIMATIONNAME>::value_type(_T("_DoubleAtk"), ANINAME_DOUBLEATTACK));

	int Count = 0;

	TCHAR HitSound[MAXKEYLENGTH] = _T("");
	
	for (; Count < MAXHITSOUND; ++Count)
	{
		_stprintf_s(HitSound, _T("Hit%d"), Count);
		SkillHitSound.push_back(HitSound);
	}

	DemageFont.push_back(_T("Number_Black"));
	DemageFont.push_back(_T("Number_White"));
	DemageFont.push_back(_T("Number_Sp"));
	DemageFont.push_back(_T("Number_Red"));
	DemageFont.push_back(_T("Number_Blue"));
	DemageFont.push_back(_T("Number_Yellow"));

	//memset(MonsterTeam, -1, sizeof(UINT)*MAXROUND*MAXMONSTER);

	vector<tstring> Temp = vector<tstring>();
	Temp.assign(MAXMONSTER, _T(""));

	CClientGlobal::strMonsterTeam.assign(MAXROUND, Temp);

	CClientGlobal::SigmoidMap.insert(unordered_map<tstring, Sigmoid>::value_type(_T("DefFunc"), Sigmoid(0.75f, 0.1f, 0.0275f)));

	float A = CClientGlobal::GetSigmoidValue(_T("DefFunc"), 100.0f);


	UINT De = 0;
	UINT AccExp = 80;

	for (UINT i = 0; i < MAXLEVEL; i++)
	{
		if (0 == i % 5)
			De += 10 + (2* (i/5));
		
		AccExp = AccExp + De;
		CClientGlobal::ExpTable.push_back(AccExp);
	}
	CClientGlobal::ExpTable.push_back(0);


	//Skill Level Up 요구 경험치
	vecSkillMaxExp.push_back(10);		// 1 -> 2	
	vecSkillMaxExp.push_back(15);		// 2 -> 3
	vecSkillMaxExp.push_back(20);		// 3 -> 4
	vecSkillMaxExp.push_back(40);		// 4 -> 5
}
ANIMATIONDATA* CClientGlobal::GetAniData(const tstring& _AnimationDataKey)
{
	ANIMATIONDATA* AnimationData = FindAnimationData(_AnimationDataKey);

	if (nullptr == AnimationData) 
	{
		return nullptr;
	}

	return AnimationData;

}
EFFECTDATA CClientGlobal::GetEffectData(const tstring& _EffectKey)
{
	for (size_t i = 0; i < EffectData.size(); i++)
	{
		if (_EffectKey == EffectData[i].EffectName)
			return  EffectData[i];
	}

	return EFFECTDATA();
}
BOOL CClientGlobal::LoadAnimationData(ANISPRITETYPE _Index)
{
	if (MAXANISPRITETYPE <= _Index) 
	{
		return FALSE;
	}

	CGameFile RFile = CGameFile(_T("Data"), CGameFile::vecAniDataFileName[_Index]);

	if (FALSE == RFile.Open(_T("rb")))
	{
		return FALSE;
	}

	map<tstring, ANIMATIONDATA*> TempMap;

	ANIMATIONDATA::ReadAnimationData(RFile.GetFile(), &TempMap);

	if (0 >= TempMap.size()) 
	{
		return FALSE;
	}

	CCriSectionObject<CClientGlobal>();
	mapAnimationData.insert(TempMap.begin(), TempMap.end());
	TempMap.clear();
	return TRUE;
}

BOOL CClientGlobal::LoadMultiSpriteData(ANISPRITETYPE _Index) 
{
	if (MAXANISPRITETYPE <= _Index)
	{
		return FALSE;
	}

	CGameFile RFile = CGameFile(_T("Data"), CGameFile::vecSpriteDataFileName[_Index]);

	if (FALSE == RFile.Open(_T("rb")))
	{
		return FALSE;
	}

	map<tstring, MULTISPRITEDATA*> TempMap = map<tstring, MULTISPRITEDATA*>();

	MULTISPRITEDATA::ReadMultiSpriteData(RFile.GetFile(), &TempMap);

	if (0 >= TempMap.size())
	{
		return FALSE;
	}

	CCriSectionObject<CClientGlobal>();
	MultiSpriteData.insert(TempMap.begin(), TempMap.end());
	TempMap.clear();

	return TRUE;
}


void CClientGlobal::ReleaseGlobalData()
{
	SAFE_DELETE_MAP(mapAnimationData);
	SAFE_DELETE_MAP(MultiSpriteData);
	CTileMap::ReleaseMainSceneCamera();
}

bool CClientGlobal::CheckDoubleAtkCharacter(const tstring& _Name)
{
	for (size_t i = 0; i < CharacterName.size(); i++)
	{
		if (_Name == CharacterName[i].UnitName) 
		{
			if (i <= Vargas) 
			{
				return true;
			}
			else 
			{
				return false;
			}
		}
	}

	return false;
}
BOOL CClientGlobal::LoadMonsterName()
{
	tstring Path = CFilePathMgr::GetPath(_T("Monster"));

	_tfinddata_t Fd;

	for (int Index = (int)LOADINGFILE::PNG; Index < (int)LOADINGFILE::FILEMAX; ++Index)
	{
		tstring FilePath = Path + CFilePathMgr::ImageFile[Index];
#ifdef X32
		long handle = _tfindfirst(FilePath.c_str(), &Fd);
#else
		intptr_t handle = _tfindfirst(FilePath.c_str(), &Fd);
#endif

		if (handle == -1)
		{
			return FALSE;
		}

		int Check = 0;

		do
		{
			tstring FileName = Fd.name;
			size_t Index = FileName.find(_T("_atk.png"));
			
			if (-1 != Index) 
			{
				FileName.replace(Index, 8, _T(""));
				MonsterName.insert(FileName);
			}
			Check = _tfindnext(handle, &Fd);

		} while (Check != -1);
	}

	
	return TRUE;
}
bool CClientGlobal::IsMonsterName(const tstring& _MonName)
{
	set<tstring>::iterator Find = MonsterName.find(_MonName);

	if (Find != MonsterName.end()) 
	{
		return true;
	}

	return false;

}
BOOL CClientGlobal::LoadStatData()
{
	CGameFile LoadFile = CGameFile(_T("Data"), _T("StatTable.bin"));

	if (FALSE == LoadFile.Open(_T("rb")))
	{
		assert(nullptr);
		return FALSE;
	}

	size_t LoadCount;
	STATPARAMETERDATA LoadParaData;

	while (0 < (LoadCount = fread_s(&LoadParaData, sizeof(LoadParaData), sizeof(STATPARAMETERDATA), 1, LoadFile.GetFile())))
	{
		vector<STAT> TempTable = vector<STAT>();

		STAT StatBuf[READCOUNT];

		for (size_t i = 0; i < MAXLEVEL / READCOUNT; i++)
		{
			fread_s(&StatBuf, sizeof(StatBuf), sizeof(STAT), READCOUNT, LoadFile.GetFile());

			for (size_t j = 0; j < READCOUNT; ++j)
			{
				TempTable.push_back(StatBuf[j]);
			}
		}

		map<tstring, STATPARAMETERDATA>::iterator ParaFind = CClientGlobal::CharacterParameter.find(LoadParaData.UnitName);

		if (ParaFind == CClientGlobal::CharacterParameter.end())
		{
			CClientGlobal::CharacterParameter.insert(map<tstring, STATPARAMETERDATA>::value_type(LoadParaData.UnitName, LoadParaData));
		}
		else
		{
			ParaFind->second = LoadParaData;
		}

		map<tstring, vector<STAT>>::iterator StatFind = CClientGlobal::CharacterStatTable.find(LoadParaData.UnitName);

		if (StatFind == CClientGlobal::CharacterStatTable.end())
		{
			CClientGlobal::CharacterStatTable.insert(map<tstring, vector<STAT>>::value_type(LoadParaData.UnitName, TempTable));
		}
		else
		{
			StatFind->second = TempTable;
		}

	}

	return TRUE;
}
BOOL CClientGlobal::LoadMonsterData()
{
	CGameFile LoadFile = CGameFile(_T("Data"), _T("MonsterScript.txt"));

	if (FALSE == LoadFile.Open(_T("rb")))
	{
		assert(nullptr);
		return FALSE;
	}

	list<MONSTERDATA> TempList;
	TempList.clear();

	MONSTERDATA::LoadMonsterScriptFile(LoadFile.GetFile(), &TempList);

	list<MONSTERDATA>::iterator Start = TempList.begin();
	list<MONSTERDATA>::iterator End = TempList.end();
	for (; Start!= End; ++Start)
		CClientGlobal::MonsterDataMap.insert(unordered_map<tstring, MONSTERDATA>::value_type(Start->MonsterName, (*Start)));
	
	return TRUE;
}
BOOL CClientGlobal::LoadEffectData()
{
	CGameFile LoadFile(_T("Data"), _T("EffectData.bin"));

	if (FALSE == LoadFile.Open(_T("rb")))
	{
		return FALSE;
	}

	EFFECTDATA::LoadDataToFile(LoadFile.GetFile(), &EffectData);
	
	return TRUE;
}
BOOL CClientGlobal::LoadSkillData()
{
	CGameFile SkillFile(_T("Data"), _T("Skill.bin"));
	CGameFile LevelFile(_T("Data"), _T("LevelData.bin"));
	CGameFile UIFile(_T("Data"), _T("SkillUIData.bin"));


	if (FALSE == SkillFile.Open(_T("rb")))
	{
		return FALSE;
	}

	size_t LoadCount = -1;
	SKILLSAVEDATA SkillData = SKILLSAVEDATA();
	vecSkill.clear();

	list<SKILLSAVEDATA> SkillList;
	SkillList.clear();
	UINT MaxSkillNo = 0;

	while (LoadCount = fread_s(&SkillData, sizeof(SkillData), sizeof(SKILLSAVEDATA), 1, SkillFile.GetFile()))
	{
		SkillList.push_back(SkillData);
		MaxSkillNo = max(MaxSkillNo, SkillData.ActionScript.SkillNo);
	}

	vecSkill.assign(MaxSkillNo + 1, SKILL());

	vector<LEVELDATA> EmpLevelData;
	
	EmpLevelData.assign(LEVELDATA::MAXSKILLLEVEL, LEVELDATA());
	vecSkillLevelData.assign(MaxSkillNo + 1, EmpLevelData);

	list<SKILLSAVEDATA>::iterator Start = SkillList.begin();
	list<SKILLSAVEDATA>::iterator End = SkillList.end();

	for (; Start != End; ++Start)
	{
		UINT No = Start->ActionScript.SkillNo;

		if (No >= vecSkill.size())
			continue;

		vecSkill[No].SkillActionScript = Start->ActionScript;
		vecSkill[No].SkillData = Start->SkillData;
		vecSkill[No].SkillNo = No;

		for (size_t i = 0; i < LEVELDATA::MAXSKILLLEVEL; i++)
			vecSkillLevelData[No][i].SkillNo = No;


		// 스킬 임시 이름 지정
		UINT CharIndex = vecSkill[No].SkillActionScript.CharIndex;

		if (CharIndex < CClientGlobal::CharacterName.size())
		{
			CString TempName;
			TempName.Format(_T("%d) %s_Skill"), No, CClientGlobal::CharacterName[CharIndex]);
			vecSkill[No].SetSkillName(TempName.GetString());
		}
	}

	// SkillLevelData Load + SkillUIData Load Logic 추가
	if (FALSE == LevelFile.Open(_T("rb")))
	{
		return FALSE;
	}

	LoadCount = -1;
	list<SAVELEVELDATA> TempLevelList;
	TempLevelList.clear();

	SAVELEVELDATA  BufLevelData = SAVELEVELDATA();

	while (LoadCount = BufLevelData.LoadDataFile(LevelFile.GetFile()))
		TempLevelList.push_back(BufLevelData);

	list<SAVELEVELDATA>::iterator LStart = TempLevelList.begin();
	list<SAVELEVELDATA>::iterator LEnd = TempLevelList.end();

	for (; LStart != LEnd; ++LStart)
	{
		UINT No = LStart->SkillNo;

		if (No >= vecSkillLevelData.size())
			continue;

		for (size_t i = 0; i < LEVELDATA::MAXSKILLLEVEL; i++)
			vecSkillLevelData[No][i] = LStart->LevelData[i];

	}

	if (FALSE == UIFile.Open(_T("rt")))
	{
		return FALSE;
	}


	list<SKILLUIDATA> TempList;
	TempList.clear();

	SKILLUIDATA::LoadTextFile(UIFile.GetFile(), &TempList);

	list<SKILLUIDATA>::iterator UIStart = TempList.begin();
	list<SKILLUIDATA>::iterator UIEnd = TempList.end();

	for (; UIStart != UIEnd; ++UIStart)
	{
		UINT No = UIStart->SkillNo;

		if (No >= vecSkill.size() || -1 == vecSkill[No].SkillNo)
			continue;

		vecSkill[No].SkillUIData = *UIStart;
		vecSkill[No].SetSkillName(UIStart->SkillName);
	}

	return TRUE;
}
BOOL CClientGlobal::LoadItemData()
{
	CGameFile ItemFile(_T("Data"), _T("ItemData.txt"));

	if (FALSE == ItemFile.Open(_T("rt")))
		return FALSE;
	
	list<ITEM> TempList;
	TempList.clear();

	if (FALSE == ITEM::LoadItemData(ItemFile.GetFile(), &TempList))
	{
		assert(nullptr);
		return FALSE;
	}

	size_t MaxIndex = 0;

	list<ITEM>::iterator Start = TempList.begin();
	list<ITEM>::iterator End = TempList.end();

	for (;Start!= End ;++Start)
	{
		if (MaxIndex < Start->ItemNo)
			MaxIndex = Start->ItemNo;
	}

	CClientGlobal::vecItem.assign(MaxIndex + 1, ITEM());

	Start = TempList.begin();
	End = TempList.end();


	for (; Start != End; ++Start)
	{
		if (CClientGlobal::vecItem.size() <= Start->ItemNo)
			continue;

		CClientGlobal::vecItem[Start->ItemNo] = (*Start);
	}

	return TRUE;
}
bool CClientGlobal::CheckCharName(const tstring& _Name)
{
	for (size_t i = 0; i < CharacterName.size(); i++)
	{
		if (CharacterName[i].UnitName == _Name)
		{
			return true;
		}
	}

	return false;
}
size_t CClientGlobal::GetEffectIndex(const tstring& _EffectName)
{
	map<tstring, size_t>::iterator Find = EffectIndexMap.find(_EffectName);

	if (Find == EffectIndexMap.end()) 
	{
		return -1;
	}

	return Find->second;
}
const tstring CClientGlobal::GetHitSound(const size_t& _SoundIndex)
{
	if (_SoundIndex >= SkillHitSound.size()) 
	{
		return _T("");
	}

	return SkillHitSound[_SoundIndex];
}
const tstring CClientGlobal::GetDemageFont(const size_t& _DemageFontIndex)
{
	if (_DemageFontIndex >= DemageFont.size())
	{
		return _T("");
	}

	return DemageFont[_DemageFontIndex];
}
void CClientGlobal::CreateTestSkill()
{
	// Test ActionScropt 제작
	ActionScript Test = ActionScript();
	
	Test.AddAniScript(ANIMATIONSCRIPT(ANINAME_WIN_BEFORE, 0.4f));
	Test.AddAniScript(ANIMATIONSCRIPT(ANINAME_MOVE, 1.9f));
	Test.AddAniScript(ANIMATIONSCRIPT(ANINAME_IDLE, 5.0f));

	Test.SkillNo = 0;
	Test.CharIndex = DemonRain;
	//Test.SetInitPos(ACTIONSCRIPT::INITPOS_MONSTER, 0, Vec2{ 0.0f,0.0f });
	Test.TotalSkillTime = 7.3f;

	SCENEEFFECTDATA SceneEffect = SCENEEFFECTDATA();
	SceneEffect.SceneEffectFlag = SCENE_EFFECT_ZOOM_IN;
	SceneEffect.SceneEffectStartTime = 0.0f;
	SceneEffect.SceneEffectTime = 3.0f;
	SceneEffect.ZoomRatio = Vec2{ 1.0f,0.3f };
	SceneEffect.SubTime = 0.4f;

	Test.AddSceneEffectData(SceneEffect);

	SceneEffect.SceneEffectFlag = SCENE_EFFECT_ZOOM_OUT;
	SceneEffect.SceneEffectStartTime = 2.0f;
	SceneEffect.SceneEffectTime = 0.8f;
	SceneEffect.ZoomRatio = Vec2{ 0.3f,1.0f };
	SceneEffect.SubTime = 0.7f;

	Test.AddSceneEffectData(SceneEffect);

	MOVEDATA MoveData = MOVEDATA();
	MoveData.MoveMode = MOVEDATA::MOVEMODE_HIDE;
	MoveData.MoveTime = 0.4f;
	MoveData.HideTime = 0.3f;
	MoveData.MoveStartTime = 2.0f;
	Test.AddCharMoveData(MoveData);

	MoveData.MoveMode = MOVEDATA::MOVEMODE_SHOW;
	MoveData.MoveTime = 0.3f;
	MoveData.HideTime = 0.2f;
	MoveData.MoveStartTime = 6.4f;
	Test.AddCharMoveData(MoveData);

	MoveData.MoveStartTime = 1.9f;
	MoveData.MoveTime = 0.5f;
	MoveData.MoveMode = MOVEDATA::MOVEMODE_MOVE;
	MoveData.MoveStartPos = Vec2{ 0.0f,0.0f };
	MoveData.MoveEndPos = Vec2{ -170.0f,0.0f };
	Test.AddCharMoveData(MoveData);

	// Character를 스킬 시전전 위치 or 스킬 시전후 위치로 이동하고 싶다면
	// 스킬시전전 : SetTeamPos(Time) ,스킬 시전후 : SetInitPos(Time) 맴버함수를 호출하면 된다.
	// 처리는 ActionPhase의 MoveUpdate에서 처리한다.
	MoveData.SetTeamPos(4.0f);
	Test.AddCharMoveData(MoveData);

	SKILLEFFECTDATA EffectData = SKILLEFFECTDATA();
	EffectData.EffectIndex = (UINT)CClientGlobal::GetEffectIndex(_T("DemonBlade"));
	EffectData.SetInitPos(Vec2{ -250.0f,0.0f });
	EffectData.MoveData.MoveStartTime = 3.0f;
	EffectData.MoveData.MoveStartPos = Vec2{0.0f,0.0f};
	EffectData.MoveData.MoveEndPos = Vec2{ 0.0f,0.0f };
	EffectData.MoveData.MoveMode = MOVEDATA::MOVEMODE_MOVE;
	EffectData.MoveData.MoveTime = 1.0f;
	EffectData.EffectImageMag = 2.5f;
	EffectData.StartTime = 2.7f;
	Test.AddSkillEffectData(EffectData);

	// Spare Effect 테스트
	//EffectData.MoveData.MoveEndPos = Vec2{ 200.0f,0.0f };
	//Test.AddSkillEffectData(EffectData);

	//EffectData.MoveData.MoveEndPos = Vec2{ 0.0f,200.0f };
	//Test.AddSkillEffectData(EffectData);

	//EffectData.MoveData.MoveEndPos = Vec2{ 0.0f,-200.0f };
	//Test.AddSkillEffectData(EffectData);

	//EffectData.MoveData.MoveEndPos = Vec2{ -200.0f,0.0f };
	//Test.AddSkillEffectData(EffectData);

	//EffectData.MoveData.MoveEndPos = Vec2{ 200.0f,200.0f };
	//Test.AddSkillEffectData(EffectData);

	//EffectData.MoveData.MoveEndPos = Vec2{ 200.0f,-200.0f };
	//Test.AddSkillEffectData(EffectData);

	//EffectData.MoveData.MoveEndPos = Vec2{ -200.0f,200.0f };
	//Test.AddSkillEffectData(EffectData);

	//EffectData.MoveData.MoveEndPos = Vec2{ -200.0f,-200.0f };
	//Test.AddSkillEffectData(EffectData);

	//EffectData.EffectIndex = CClientGlobal::GetEffectIndex(_T("ChargingRedBlade"));
	//EffectData.SetInitPos(SKILLEFFECTDATA::INITPOS_CASTERPOS, Vec2{ 0.0f,10.0f });
	//EffectData.EffectImageMag = 2.0f;
	//EffectData.StartTime = 1.0f;
	//Test.AddSkillEffectData(EffectData);

	SOUNDSAVEDATA SoundData = SOUNDSAVEDATA();
	SoundData.SetSoundName(_T("voice.demonAvenger.M.56"));
	SoundData.PlayStartTime = 0.0f;
	Test.AddCharSoundData(SoundData);

	vecActionScript.push_back(Test);

	SKILLDATA TData = SKILLDATA();
	TData.SkillNo = 0;
	TData.CharacterSkill = true;
	TData.EnemyTargetType = SKILLDATA::TARGET_ALL;
	TData.TeamTargetType = SKILLDATA::TARGET_NONE;
	TData.SkillType = SKILLDATA::SKILLTYPE_BURSTSKILL;

	TData.InitPosMode = SKILLDATA::INITPOS_FIXPOS;
	TData.InitFixPos = Vec2{ 0.0f,0.0f };
	TData.InitPosPivot = Vec2{ 0.0f,0.0f };

	LEVELDATA LData = LEVELDATA();
	LData.SkillNo = 0;
	LData.SkillCount = 1;
	LData.SetSkillCost(LEVELDATA::SKILLCOST_HP,0);
	LData.SetSkillCost(LEVELDATA::SKILLCOST_MP,300);
	LData.SetSkillCost(LEVELDATA::SKILLCOST_BURST,100);

	LData.DemageStyle = DEMAGESTYLE_STAIRS;
	LData.Option = -5;

	HITDATA HData = HITDATA();
	HData.HitCount = 1;

	HData.HitDemageMag = 10.0f;
	HData.FixHitDemage = 0;
	
	//HData.HitEffectIndex = 8;
	//HData.HitEffectImageMag = 2.0f;
	//HData.PosOption = HITDATA::POS_TARGET;

	HData.SetHitSound(SWORDHIT4);
	HData.SetHitReAction(REACTION_DEFAULT);
	HData.HitEffectIndex = -1;

	HData.StartTime = 4.9f;
	LData.AddHitData(HData);
	
	HData.StartTime = 5.05f;
	LData.AddHitData(HData);

	HData.StartTime = 5.2f;
	LData.AddHitData(HData);

	HData.StartTime = 5.35f;
	LData.AddHitData(HData);

	HData.StartTime = 5.5f;
	LData.AddHitData(HData);

	HData.StartTime = 5.65f;
	LData.AddHitData(HData);

	CClientGlobal::vecSkillLevelData.push_back(vector<LEVELDATA>());

	for (size_t i = 0; i < SKILLDATA::MAXSKILLLEVEL; ++i)
	{
		for (size_t j = 0; j < LData.HitDataCount; ++j)
		{
			LData.HitData[j].HitDemageMag = 10.0f*(i + 1);
		}
				
		vecSkillLevelData[0].push_back(LData);
	}

	//CClientGlobal::vecSkillData.push_back(TData);

	SKILLUIDATA UIData; 
	UIData.SkillNo = 0;
	UIData.SkillIconIndex = 75;
	UIData.SkillName = _T("ExtremeBlade");
	UIData.SkillKName = _T("익스트림 블레이드");
	UIData.SkillDescription = _T("분노의 힘을 최대로 끌어올려 진정한 악마로 각성한 뒤 적군 전체에 피해를 입힌다.");

	UIData.SetSkillSaveData();
	UIData.ClearData();
	UIData.ApplySaveData();
	vecSkillUIData.push_back(UIData);

	SKILL TestSkill = SKILL(UIData.SkillNo,UIData.SkillName, Test, TData ,UIData);

	vecSkill.push_back(TestSkill);

	//SKILL EmpSkill;
	//EmpSkill.SkillNo = 1;
	//EmpSkill.SkillUIData.SkillIconIndex = 1;
	//EmpSkill.SkillUIData.SkillNo = 1;
	//vecSkill.push_back(EmpSkill);
	//EmpSkill.SkillNo = 2;
	//EmpSkill.SkillUIData.SkillIconIndex = 2;
	//EmpSkill.SkillUIData.SkillNo = 2;
	//vecSkill.push_back(EmpSkill);
	//EmpSkill.SkillNo = 3;
	//EmpSkill.SkillUIData.SkillIconIndex = 3;
	//EmpSkill.SkillUIData.SkillNo = 3;
	//vecSkill.push_back(EmpSkill);
	//EmpSkill.SkillNo = 4;
	//EmpSkill.SkillUIData.SkillIconIndex = 4;
	//EmpSkill.SkillUIData.SkillNo = 4;
	//vecSkill.push_back(EmpSkill);
	//EmpSkill.SkillNo = 5;
	//EmpSkill.SkillUIData.SkillIconIndex = 5;
	//EmpSkill.SkillUIData.SkillNo = 5;
	//vecSkill.push_back(EmpSkill);
	//EmpSkill.SkillNo = 6;
	//EmpSkill.SkillUIData.SkillIconIndex = 6;
	//EmpSkill.SkillUIData.SkillNo = 6;
	//vecSkill.push_back(EmpSkill);
}
void CClientGlobal::ClearStrMonsterTeam()
{
	for (size_t i = 0; i < CClientGlobal::strMonsterTeam.size(); i++)
		CClientGlobal::strMonsterTeam[i].assign(MAXMONSTER, _T(""));
	
}
const float CClientGlobal::GetSigmoidValue(const tstring& _FuncName, const float& _Para)
{
	unordered_map<tstring, Sigmoid>::iterator Find = CClientGlobal::SigmoidMap.find(_FuncName);

	if (Find == CClientGlobal::SigmoidMap.end())
		return 0.0f;

	return Find->second(_Para);
}
const size_t CClientGlobal::FindCharacterLineNo(const CharacterIndex& _CharIndex)
{
	for (size_t j = 0; j < CharacterLine.size(); ++j)
	{
		for (size_t i = 0; i < CharacterLine[j].size(); i++)
		{
			if (_CharIndex == CharacterLine[j][i].CharacterNo)
				return j;
		}
	}

	assert(nullptr);
	return -1;
}
const size_t CClientGlobal::FindCharacterLineNoForLineName(const tstring& _LineName)
{
	if (_T("레인") == _LineName)
		return RAIN;
	if (_T("시즈루") == _LineName)
		return CHIZURU;
	if (_T("루나") == _LineName)
		return LUNA;
	if (_T("자이러스") == _LineName)
		return ZYRUS;
	if (_T("리드") == _LineName)
		return LID;
	if (_T("링") == _LineName)
		return LING;
	if (_T("사쿠라") == _LineName)
		return SAKURA;
	if (_T("베가스") == _LineName)
		return VARGAS;
	if (_T("딘") == _LineName)
		return DEAN;
	if (_T("아르테미오스") == _LineName)
		return ARTEMIOS;

	return -1;
}
const size_t CClientGlobal::FindLineIndex(const CharacterIndex& _CharIndex) 
{
	for (size_t j = 0; j < CharacterLine.size(); ++j)
	{
		for (size_t i = 0; i < CharacterLine[j].size(); i++)
		{
			if (_CharIndex == CharacterLine[j][i].CharacterNo)
				return i;
		}
	}

	assert(nullptr);
	return -1;
}
const size_t CClientGlobal::GetCurCharacterLevel(const CharacterIndex& _CharIndex)
{
	for (size_t j = 0; j < CharacterLine.size(); ++j)
	{
		for (size_t i = 0; i < CharacterLine[j].size(); i++)
		{
			if (_CharIndex == CharacterLine[j][i].CharacterNo)
				return  CharacterLine[j][i].CharacterLv;
		}
	}

	assert(nullptr);
	return -1;
}
const CHARACTERDATA CClientGlobal::GetCurCharData(const CharacterIndex& _CharIndex)
{
	for (size_t j = 0; j < CharacterLine.size(); ++j)
	{
		for (size_t i = 0; i < CharacterLine[j].size(); i++)
		{
			if (_CharIndex == CharacterLine[j][i].CharacterNo)
				return  CharacterLine[j][i];
		}
	}

	assert(nullptr);
	return CHARACTERDATA();
}
const STAT CClientGlobal::GetCurCharacterStat(const CharacterIndex& _CharIndex, const size_t& _Lv)
{
	if (_CharIndex >= MAXBATTLECHARACTER )
		return STAT();

	tstring CharacterName = CClientGlobal::CharacterName[_CharIndex].UnitName;
	map<tstring, vector<STAT>>::iterator Find = CClientGlobal::CharacterStatTable.find(CharacterName);

	if (Find == CClientGlobal::CharacterStatTable.end() || Find->second.size() <= _Lv)
		return STAT();

	return Find->second[_Lv];
}
const bool CClientGlobal::ChangeEquipItem(const CharacterIndex& _CharIndex, const ITEMEQUIP& SlotNo, const size_t& _ItemNo)
{
	if (SlotNo >= MAXITEMEQUIP)
		return false;

	for (size_t j = 0; j < CharacterLine.size(); ++j)
	{
		for (size_t i = 0; i < CharacterLine[j].size(); i++)
		{
			if (_CharIndex == CharacterLine[j][i].CharacterNo)
			{
				size_t CurNo = CharacterLine[j][i].EquipItem[SlotNo];
				
				if (CurNo == _ItemNo)
					return false;
				else 
				{
					ITEM CurItem, ChangeItem;
					
					if (CurNo < vecItem.size()) 
					{
						CurItem = vecItem[CurNo];
						InventoryMgr->ReleaseEquipItem(CurNo);
					}

					if (_ItemNo < vecItem.size()) 
					{
						ChangeItem = vecItem[_ItemNo];
						InventoryMgr->MountEquipItem(_ItemNo);
					}
					

					STAT ItemDiffStat = ChangeItem.ItemStat - CurItem.ItemStat;
					float CurRatio = 1.f;

					if (0 != ItemDiffStat.Heath) 
					{
						CurRatio = (float)CharacterLine[j][i].CurHp / (float)CharacterLine[j][i].MaxHp;
						CharacterLine[j][i].MaxHp = max(0, (int)CharacterLine[j][i].MaxHp + ItemDiffStat.Heath);
						CharacterLine[j][i].CurHp = min(CharacterLine[j][i].MaxHp,(size_t)ceilf(CharacterLine[j][i].MaxHp * CurRatio));

					}

					if (0 != ItemDiffStat.Mana) 
					{
						CurRatio = (float)CharacterLine[j][i].CurMp / (float)CharacterLine[j][i].MaxMp;
						CharacterLine[j][i].MaxMp = max(0, CharacterLine[j][i].MaxMp + ItemDiffStat.Mana);
						CharacterLine[j][i].CurMp = min(CharacterLine[j][i].MaxMp, (size_t)ceilf(CharacterLine[j][i].MaxMp * CurRatio));
					}

				
					CharacterLine[j][i].EquipItem[SlotNo] = _ItemNo;
					
					return true;
				}
			}
		}
	}

	return false;
}
const bool CClientGlobal::CheckSkillLevelUp(const size_t& _SkillNo)
{
	if (_SkillNo >= CClientGlobal::vecSkill.size())
		return false;

	if (false == CClientGlobal::vecSkill[_SkillNo].SkillData.CharacterSkill)
		return false;

	size_t CurLv = CClientGlobal::vecSkill[_SkillNo].CurSkillLevel;
	
	if (vecSkillMaxExp[CurLv] > CClientGlobal::vecSkill[_SkillNo].CurSkillExp)
		return false;

	return true;
}