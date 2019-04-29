#pragma once
#include"Precom.h"


class CGameMaster;
class CMouseCursor;
class CPlayer;
class CTileMap;
class CClientGlobal
{
	enum 
	{
		READCOUNT = 10
	};
private:
	static ANIMATIONDATA* FindAnimationData(const tstring& _AnimationDataKey);


public:
	enum SceneIndex 
	{
		LOADINGSCENE,
		BATTLESCENE,
		MAINSCENE,
		TITLESCENE,
		ENDINGSCENE,
	};

	static SceneIndex											PrevScene;
	static vector<tstring>										FolderName;
	
	static Mat													CurSceneProj;
	static Mat													CurSceneView;
	// 줌인이 적용되지 않은 행렬
	static Mat													CurNoneZoomProj;
	// 카메라 행렬이 적용되지 않은 행렬
	static Mat													NoneCameraView;
	//static SPTR<CCamera>										MainCamera;

	static vector<tstring>										SceneName;
	//static vector<tstring>									CharacterName;
	static vector<UNITNAME>										CharacterName;
	static set<tstring>											MonsterName;
	static int													AdvanceTeam[MAXTEAM];
	
	static UINT													MonsterTeam[MAXROUND][MAXMONSTER];
	static vector<vector<tstring>>								strMonsterTeam;
	static UINT													CurBattleEndRound;

public:
	static SPTR<CGameWindow>									MainWindow;
	static map<tstring, ANIMATIONDATA*>							mapAnimationData;
	static map<tstring, MULTISPRITEDATA*>						MultiSpriteData;
	static SPTR<CActObject>										MouseObeject;

	static map<tstring, vector<STAT>>							CharacterStatTable;
	static map<tstring, STATPARAMETERDATA>						CharacterParameter;
	static map<tstring,ANIMATIONNAME>							CharacterAniIndex;

	// Character 초기 스텟
	//static  map<tstring, STAT>									InitCharacterStat;

	// 나중에 Skill Vector로 한번에 저장
	static vector<SKILL>										vecSkill;
	static vector<UINT>											vecSkillMaxExp;
	static vector<ACTIONSCRIPT>									vecActionScript;
	static vector<SKILLUIDATA>									vecSkillUIData;

	static vector<SKILLDATA>									vecSkillData;
	static vector<vector<LEVELDATA>>							vecSkillLevelData;
	
	static vector<EFFECTDATA>									EffectData;
	static map<tstring, size_t>									EffectIndexMap;

	// 몬스터 데이터 
	static unordered_map<tstring, MONSTERDATA>					MonsterDataMap;


	// 스킬 효과음 or 타격음
	static vector<tstring>										SkillHitSound;
	// DemageFont SpriteName 
	static vector<tstring>										DemageFont;


	static class CTurnMgr*										TurnMgr;

	// MainScene Object
	static CGameMaster*											GameMaster;
	static class CInventoryMgr*									InventoryMgr;
	static SPTR<CPlayer>										MainPlayer;
	static SPTR<CTileMap>										CurTileMap;
	
	// 캐릭터 분류
	static vector<vector<CHARACTERDATA>>						CharacterLine;
	

	static SPTR<CGameMultiSprite>								SkillIconMultiSprite;
	static SPTR<CGameMultiSprite>								ItemIconMultiSprite;

	static vector<ITEM>											vecItem;
	// 치트키 모드
	static bool													MasterMode;

	static unordered_map<tstring, Sigmoid>						SigmoidMap;
	static vector<UINT>											ExpTable;

	static class CEndingSceneBuilder*							EndingBuilder;
	static class CSplashSceneBuilder*							SplashBuilder;
	
	// 스토리 전투일때 패배한경우
	// 게임을 종료 시킨다.
	static bool													StoryBattle;

public:
	static BOOL ExistAnimationData(const tstring& _AnimationDataKey);
	static ANIMATIONDATA* GetAniData(const tstring& _AnimationDataKey);
	static EFFECTDATA GetEffectData(const tstring& _EffectKey);
	static BOOL LoadAnimationData(ANISPRITETYPE _Index);
	static BOOL LoadMultiSpriteData(ANISPRITETYPE _Index);
	static BOOL LoadMonsterName();
	static BOOL LoadStatData();
	static BOOL LoadMonsterData();
	static BOOL LoadEffectData();
	static BOOL LoadSkillData();
	static BOOL LoadItemData();
	static size_t GetEffectIndex(const tstring& _EffectName);
	static const tstring GetHitSound(const size_t& _SoundIndex);
	static const tstring GetDemageFont(const size_t& _DemageFontIndex);
 
	
public:
	static bool CheckDoubleAtkCharacter(const tstring& _Name);
	static bool CheckCharName(const tstring& _Name);
	static bool IsMonsterName(const tstring& _MonName);
	
public:
	static void Init();
	static void ReleaseGlobalData();
	
	static void CreateTestSkill();

	static void ClearStrMonsterTeam();
	static const float GetSigmoidValue(const tstring& _FuncName,const float& _Para);
	
	static const size_t FindCharacterLineNo(const CharacterIndex& _CharIndex);
	static const size_t FindCharacterLineNoForLineName(const tstring& _LineName);
	static const size_t FindLineIndex(const CharacterIndex& _CharIndex);
	static const size_t GetCurCharacterLevel(const CharacterIndex& _CharIndex);
	static const CHARACTERDATA GetCurCharData(const CharacterIndex& _CharIndex);
	static const STAT GetCurCharacterStat(const CharacterIndex& _CharIndex,const size_t& _Lv);
	static const bool ChangeEquipItem(const CharacterIndex& _CharIndex, const ITEMEQUIP& SlotNo,const size_t& _ItemNo);
public:
	static const bool CheckSkillLevelUp(const size_t& _SkillNo);

private:
	CClientGlobal() {}
	~CClientGlobal() {}
};


