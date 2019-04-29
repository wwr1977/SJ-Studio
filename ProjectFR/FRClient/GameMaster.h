#pragma once
#include <ObjBase.h>

class CTile;
class CMainMapMgr;
class CInventoryMgr;
class CScriptMgr;
class CEventMgr;
class CPlayer;
class CGameMaster : public CObjBase
{
public:
	enum BATTLECOUNT
	{
		BATTLE_THIEF,
		BATTLE_DARKCOMMANDER,
		BATTLE_BOSS,
		BATTLE_SLIME,
		MAXBATTLECOUNT,
	};

	enum TRIGERTYPE
	{
		MAPCHANGE_PLAYERTRIGER,
		MAPCHANGE_WORLDMAPTRIGER,
		MAXMAPCHANGETRIGER,
	}m_TrigerType;
private:
	CGameScene*						m_pMainScene;
	CSceneBuilder*					m_pMainSceneBuilder;

private:
	float							m_fAccTime;
	SPTR<CPlayer>					m_MainPlayer;
	SPTR<CFixRenderer>				m_SceneCurtain;

	// 모든 NPC는 GameMaster가 관리
	// GameMaster를 통해 NPC를 찾을 수 있다.
	vector<SPTR<class CNPCBase>>	m_vecNPC;

	SPTR<class CMainMenuMgr>		m_MainMenuMgr;
	CMainMapMgr*					m_pMapMgr;
	CInventoryMgr*					m_pInventoryMgr;
	CScriptMgr*						m_pScriptMgr;
	CEventMgr*						m_pEventMgr;
	// 모든 캐릭 그룹에서의 현재  그룹상에서의 Index 번호
	// Ex )  0 : 레인2  1: 레인5  2: 데몬 레인
	vector<size_t>					m_vecCurChar;
	size_t							m_CurTeam[MAXTEAM];


	vector<NPCDATA>						m_vecNPCData;

	function<void(void)>				m_pEventUpdateFunc;
	// 메인 씬에서 배틀 씬으로 넘어가기 전의
	// 퀘스트 스크립트 키와 퀘스트 카운터
	tstring								m_CurBattleQuestKey;
	UINT								m_CurBattleQuestCount;

	bool								m_bBattleSceneTriger;
	bool								m_bMainSceneStart;
	bool								m_bBattleWin;


	SPTR<class CQuestClearObject>		m_QuestClearObject;
	list<SPTR<class CEffect>>			m_listEffect;

	float								m_fAccWaitTime;
	
	bool								m_bMapChangeUpdate;
	bool								m_bMapChange;
	bool								m_bStoryCurtainWait;
	bool								m_bBaseNPCTriger;
	MAINMAPNAME							m_ChangeMap;
	SPTR<CTile>							m_ChangeTrigerTile;

	SPTR<class CStoryCurtain>			m_StoryCurtain;

	SPTR<class CStoreUI>				m_StoreUI;

	bool								m_bGameClear;
	SPTR<CMouseCursor>					m_MouseCursor;


	vector<bool>						m_vecCharacterLineOpen;

	SPTR<class CQuestRewardGuide>		m_QuestResultGuide;

public:
	void InitGameMaster();
	void SetMouseCursor(CMouseCursor* _MouseCursor);
	// 게임마스터 하위의 매니져들을 초기화 한다.
	void ResetGameMaster();
	void Update();
	void LastUpdate();
	void WakeUpGameMaster();

public:
	void PrevSceneCheck();

public:
	void SceneStartUpdate();
	void KeyCheck();
	void MapChangeUpdate();
	void StoryCurtainUpdate();
	void GameClearUpdate();

public:
	CActObject* CreateActObject(const tstring& _Name);
	CGameScene* GetMainScene();

public:
	void MapChangeEvent(SPTR<CTile> _Tile);
	void MapChangeEvent(SPTR<CTile> _Tile, const MAINMAPNAME& _MapNo);
	void ReleaseGameMaster();
	void InitCameraPos();
	void BattleSceneChangeEvent();

public:
	void SetCharacterTeam(const size_t& _TeamIndex, const size_t& _CharNo);
	void PopCharacterTeam(const size_t& _TeamIndex);
	//캐릭터가 현재 팀에 소속되었는지 검사
	const bool	CheckCurBelongTeam(const size_t& _CharNo);
	const size_t GetCurTeam(const size_t& _TeamIndex);
	const size_t GetCurCharacter(const CHARACTERLINE& _LineNo);
	const CHARACTERDATA GetCurCharData(const CHARACTERLINE& _LineNo);
	void MenuOffEvnet();

public:
	bool SceneEffect(DWORD _Flag, const float& _Time);
	// 지진 Data 셋팅
	const Vec2 GetSpaceSize(DWORD _Power);
	void SetEarthquakeData(const Vec2& _SpaceSize, const float& _PulseTime = 0.02f);
	// Fade Effect Data 셋팅
	void SetFadeData(const float& _FadeTime);
	void SetFadeData(const float& _FadeTime,const float& _Pow);
	// Zoom Effect Data 셋팅
	void SetZoomData(const Vec3& _ZoomPos, const float& _Near, const float& _ZoomTime);
	void SetZoomData(const Vec3& _ZoomPos, const Vec2& _ZoomRatio, const float& _ZoomTime);


public:
	const bool IsStortCurtainWait();
	SPTR<CNPCBase> GetNPC(const NPC& _Index);
	SPTR<CNPCBase> GetNPC(const tstring& _NPCName);
	SPTR<CGameSprite> GetNpcPoster(const NPC& _Index);
	SPTR<CGameSprite> GetNPCPoster(const tstring& _NPCName);
	const tstring GetNPCKName(const tstring& _NpcName);
	void EnableNPC(const NPC& _Index,const bool& _Enable = true);
	SPTR<CStoreUI> GetStoreUI();

public:
	void ChangeNPCDyingAni(const UINT& _NPCNo);
	void ChangeNPCDeadAni(const UINT& _NPCNo);
public:
	void ShowScriptEvent(const tstring& _NPCName,const bool& _bDialog = true);
	void ShowScript();

public:
	void MenuMgrEventOn(const bool& _On);
	void FinishScriptEvent();
	void FinishQuestEvent(const tstring& _QuestName);
	void FinishLastQuestEvent();

public:
	void EventTriger(const tstring& _NPCNo);
	void SetEventUpdateFunc(const tstring& _NPCNo);
	void FinishEventUpdate(const tstring& _NPCName);
	void BattleSceneChangeEvent(const tstring& _QuestKey, const UINT& _QuestCount);
	void SettingBattleMonster(const BATTLECOUNT& _Count);
	void CaptureScreen();
	
public:
	void BattleWin(const bool& _Win);
	void BattleWinEvent();

public:
	void EffectOn(const tstring& _EffectKey, const Vec3& _Pos,const bool& _CharBack = true);
	void ReleaseEffect();

public:
	void StoreOpenEvent(const STORETYPE& _Type);
	void StoreCloseEvent();

public:
	void InventoryEvent(const bool& _Open);
	SPTR<CMouseCursor> GetMouseCursor();
	
public:
	CEventMgr* GetEventMgr();


public:
	void MapSoundPauseEvent();

public:
	const bool IsOpenCharacterLine(const CharacterIndex& _CharIndex);
	const bool IsOpenCharacterLine(const tstring& _CharName);
	const bool IsOpenCharacterLine(const CHARACTERLINE& _LineNo);
	void OpenCharacterLine(const CHARACTERLINE& _LineNo, const bool& _Open = true);


public:
	CGameMaster();
	CGameMaster(CGameScene* _MainScene,CSceneBuilder* _MainBuilder);
	~CGameMaster();
};

