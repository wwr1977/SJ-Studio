#pragma once
#include <ObjBase.h>

class CBackGround;
class CTurnOrderUI;
class CBattleUnit;
class CUnitStatusBar;
class CBattleSceneBuilder;
class CEffectMgr;
class CSkillUIMgr;
class CDemageFont;
class CTargetUI;
class CSkillUI;
class CTurnMgr : public CObjBase
{
	enum 
	{
		MAXDEMAGEFONTCOUNT = 100
	};
	
	enum EFFECTSCRIPT
	{
		QUEUE_SKILLEFFECT,
		QUEUE_SCENEEFFECT,
		MAXEFFECTQUEUE,
	};

	enum SKILLQUEUE_INDEX 
	{
		ACTSCRIPT_ANI,
		ACTSCRIPT_MOVE,
		ACTSCRIPT_SOUND,
		ACTSCRIPT_SKILLEFFECT,
		ACTSCRIPT_SCENEEFFECT,
		SKILLDATA_HITDATA,
		MAXSKILLQUEUEINDEX,
	};

	enum SKILLDATA_INDEX 
	{
		SKILLDATA_HIT,
		MAXSKILLDATAINDEX
	};


private:
	CGameScene*										m_pBattleScene;
	CBattleSceneBuilder*							m_pBattleSceneBuilder;
	SPTR<CActObject>								m_BattleCamera;
	SPTR<CCamera>									m_BattleCamCom;
	SPTR<CStateMgr>									m_StateMgr;
	CEffectMgr*										m_pEffectMgr;
	CSkillUIMgr*									m_pSkillUIMgr;
	vector<SPTR<CBattleUnit>>						m_vecCurTurnTarget;

	SPTR<CBackGround>								m_BattleBack;

	// 모든 전투 캐릭터 , 몬스터를 모은 저장소(Actor + Logic)
	vector<SPTR<CBattleUnit>>								m_vecCharacter;
	//map<tstring, SPTR<CBattleUnit>>							m_mapMonster;
	unordered_map<tstring, list<SPTR<CBattleUnit>>>			m_mapMonsterList;

	//모든 전투 캐릭터들의 Hp,Mp bar을 모은 저장소(Actor + Logic)
	//unordered_map<tstring,SPTR<CUnitStatusBar>>				m_mapStatusBar;
	unordered_map<tstring, list<SPTR<CUnitStatusBar>>>		m_mapStatusBarList;
	Vec2													m_CharStatusSize;
	Vec2													m_MonStatusSize;

	// Turn의 흐름을 표기하는 UI가 모두 모인 저장소
	//unordered_map<tstring, SPTR<CTurnOrderUI>>				m_mapTurnUI;
	unordered_map<tstring, list<SPTR<CTurnOrderUI>>>		m_mapTurnUIList;

	// 현재 전투에 참여한 캐릭터들
	vector<SPTR<CBattleUnit>>								m_vecCurBattleChar;
	UINT													m_CurCharCount;
	
	// 현재 전투에 참여한 몬스터
	vector<SPTR<CBattleUnit>>								m_vecCurBattleMon;
	UINT													m_CurMonCount;

	// Turn의 흐름을 표기하기 위한 UI (전투시 사용)
	list<SPTR<CTurnOrderUI>>								m_listCurTurnUI;

	// DamegeFont를 모아둔 저장소 , Queue
	vector<SPTR<CDemageFont>>								m_vecDemageFont;
	// 데미지 폰트를 뽑아오는 Queue
	list<SPTR<CDemageFont>>									m_DemageReadyList;
	// 시간이 다된 데미지 폰트가 돌아오는 Queue
	list<SPTR<CDemageFont>>									m_DemageReturnList;
	// 현재 pop된 DemageFont의 갯수 와 pop되었던 모든 DemageFont가 ReturnList돌아왓는지 체크하는 변수
	int														m_iCurOnDemageCount;
	bool													m_bAllReturn;
	/*bool													m_FontReturnCheck;
	float													m_fAccReturnCheckTime;
	float													m_fMaxReturn;*/
	


	// 전투 캐릭터들의 위치를 저장(0~4 번,총 5명의 자리)
	vector<Vec3>									m_vecCharacterPos;
	// 전투 캐릭터의 스테이터스 창의 위치(0~4 번, 총 5개의 자리)
	vector<Vec3>									m_vecCharStatusPos;
	// 전투 몬스터의 스테이터스 창의 위치(0~2 번, 총 3개의 자리)
	vector<Vec3>									m_vecMonStatusPos;

	// Turn UI의 위치가 저장되어있는 저장소 
	vector<Vec3>									m_vecTurnUIPos;
	// 캐릭터들의 턴을 표시하기 위한 Effect의 위치
	vector<Vec3>									m_vecSelectEffectPos;
	// 몬스터들의 위치할 장소를 저장
	vector<Vec3>									m_vecMonsterPos;


	// Turn UI의 크기를 Custom으로 지정하기 위해 생성한 변수
	Vec2											m_FirstFrameSize;
	Vec2											m_OtherFrameSize;

	// SkillBackBoard 위치와 크기
	Vec3											m_SkillBackBoardPos;
	Vec2											m_SkillBackBoardSize;

	// 스킬이 발동 되었을때 true 가 되는 변수
	// Select Phase에서 이 값을 체크한다.
	bool											m_bSkillTrigger;
	UINT											m_CurSkillLevel;


	// Skill 동작 관련된 모든 저장소와 Queue
	// Action Script 관련 저장소 & Queue
	
	// Skill1)  Unit의 애니메이션 관련 스크립트
	queue<ANIMATIONSCRIPT>							m_qAniScript;
	// 현재 큐에 들어있는 애니메이션 스크립트의 갯수
	int												m_iAniScriptCount;

	// Unit의 움직임 관련 스크립트
	queue<MOVEDATA>									m_qMoveScript;
	// 현재 큐에 들어있는 Move 스크립트의 갯수
	int												m_iMoveScriptCount;

	// 유닛의 대사 or 효과음 관련 저장소 & Queue
	vector<SoundData>								m_vecSoundScript;
	queue<tstring>									m_SoundQueue;

	// 스킬 이팩트 큐
	vector<SKILLEFFECTDATA>							m_vecSkillEffect;
	queue<SKILLEFFECTDATA>							m_qSkillEffect;

	// 카메라 특수효과 큐
	queue<SCENEEFFECTDATA>							m_qSceneEffect;
	// 현재 특수효과 큐에 들어있는 객체의 갯수
	int												m_iCurSceneEffectCount;
	
	// 스킬이 끝나는 시간
	float											m_fSkillActionTime;

	// Skill2) SkillData 와 Hit Data 관련 Data
	SKILLDATA										m_CurSkillData;
	LEVELDATA										m_CurSkillLevelData;
	list<HITDATA>									m_HitData;
	queue<HITDATA>									m_qHitData;
	
	// Attack or DoubleAttack 스킬이 동작중임을 나타내는 변수값
	bool											m_bLastHitCheck;
	UINT											m_LastHitIndex;
	
private:
	SPTR<CBattleUnit>								m_CurBattleUnit;
	// Rotate Phase에서 현재 턴의 Unit의 턴을 얼마나 밀지를 판단하는 변수
	// 0보다 작을경우 가장 뒤로 밀어버린다
	int												m_iRotateCount;
	float											m_fAccTime;
	
	//Vec2											m_InitPos;
	//int												m_InitPosMode;
	//size_t											m_InitPosIndex;

	size_t											m_InitPosMode;
	Vec2											m_InitFixPos;
	Vec2											m_InitPosPivot;




	// 현재 턴이 진행되는 캐릭터가 갖는 스킬 번호를 저장한다.
	// 이 정보는 SkillUIBoard가 사용 할것이므로 해당객체에게 포인터를 넘겨줄것이다.
	vector<vector<size_t>>							m_CurSkillIndexList;


	// 현재 선택된 스킬의 Index를 소지
	// -1은 선택이 안된 상태를 의미
	size_t											m_CurSelectSkill;
	TARGET											m_SelectTarget;
	// 현재 켜진 타겟 UI를 임시로 저장(몬스터에서 출력해제시 List안의 UI를 Off)
	list<SPTR<CTargetUI>>							m_CurActTargetUI;

	vector<queue<UINT>>								m_vecDemageQueue;
	

	UINT											m_CurBattleRound;

	float											m_fAccEndEventTime;
	UINT											m_BattleWinTotalExp;


	bool											m_bBGMMode;
	vector<BGMDATA>									m_vecBattleBgmData;
	UINT											m_CurBattleBgmIndex;

	SPTR<class CBattleResultWindow>					m_ResultWindow;
	SPTR<class CBattleEventObject>					m_EventObject;



public:
	CGameScene*			GetScene();
	CBattleSceneBuilder* GetBattleSceneBuilder();
	SPTR<CTurnOrderUI> GetCurOrderUI();
	CBattleUnit* GetCurUnit();
	CActObject* CreateActObject(const tstring& _Name = _T(""));
	//SPTR<CBattleUnit> GetBattleMonster(const int& _Index);
	SPTR<CBattleUnit> PopBattleMonster(const tstring& _MonsterName);
	SPTR<CBattleUnit> PopBattleMonster(const int& _Index);
	CEffectMgr* GetEffectMgr();
	const Vec3 GetCharacterPos(const int& _TeamNumber);
	const Vec3 GetCharacterPos(const size_t& _TeamNumber);
	const Vec3 GetMonsterPos(const int& _TeamNumber);
	const Vec3 GetMonsterPos(const size_t& _TeamNumber);
	SPTR<CBattleUnit> GetCharacter(const size_t& _CharNo);
	SPTR<CBattleUnit> GetCurCharacter(const size_t& _TeamNo);
	SPTR<CBattleUnit> GetCurMonster(const size_t& _TeamNo);
	const UINT GetCurCharCount();
	const UINT GetCurMonCount();
public:
	void Init(CGameScene* _BattleScene,CBattleSceneBuilder* _Builder);
	void SettingObjectPosition();
	
	void CreateBattleObject();
	void CreateBattleCharacter();
	void CreateBattleMonster();
	CBattleUnit* CreateBattleMonster(const tstring& _MonName);


public:
	//SPTR<CTurnOrderUI> FindTurnUI(const tstring& _UnitKey);

	bool PushTurnUI(SPTR<CTurnOrderUI> _TurnUI);
	// Rotate 카운트 만큼 UI를 뒤로 밀어주는 함수
	bool PushTurnUI(SPTR<CTurnOrderUI> _TurnUI, int _RotateCount);
	// Turn List의 가장 앞에 있는 UI를 Pop
	bool PopCurTurnUI();

	void InitList();
	void Update();
	
	void InitSkillUIMgr();

public:
	void ChangePhase(int _Index);
	// StandByPhase에서 사용할 작업(모든 오브젝트 Off & 전투 오브젝트 셋팅)
	void AllObjectOff();
	void CurActiveObjectOff();
	void SettingBattleObject();
	void UnitPairUIOn();
	// 리스트안의 Turn UI를 어떤 변수의 값을 비교하여(Dex 예상)
	// Sort를 진행한 뒤 그 순서로 TurnUI를 옮김
	void InitTurnUIPos();

	// Rotate Phase에서 사용할 변수 셋팅 or 획득
	void SetRotateCount(const int& _RotCount);
	int GetRotateCount();
	void PopDeadUnitUI();
	bool UpdateTurnUIPos();
	// 맨위의 Ui를 가장 아래로 내려버리는 UI 무브먼트
	bool DefaultRotateEvent();
	// m_iRotateCount만큼 최상위 Ui를 아래로 내리는 무브먼트
	bool ChangeRotateEvent();
	// 각 무브먼트 Event가 끝나면 마지막으로 위치조정을 하는 함수
	void SyncroUIPos();
	UINT RandomPushTurnUI(SPTR<CTurnOrderUI> _UI);
	void InitUILocation();
	void ClearAllDemageQueue();

	// Demage Font 관련 함수
	SPTR<CDemageFont> GetDemageFont(const int& _Number, const Vec3& _RenderPos, const size_t& NumberSpriteIndex);
	void PopDemageFont(const int& _Number, const Vec3& _RenderPos, const int& _Count = 1);
	void PopDemageFont(const int& _Number, const Vec3& _RenderPos, const int& _Count , const tstring& _NumberSpriteName);
	void PopDemageFont(const int& _Number, const Vec3& _RenderPos, const int& _Count, const size_t& NumberSpriteIndex);
	// RenderPos 가 폰트의 중심점으로 설정하는 함수
	void PopMiddleDemageFont(const int& _Number, const Vec3& _RenderPos, const UINT& _Count, const size_t& NumberSpriteIndex = DEMAGE_BLACK);
	

	void ReturnDemageFont(SPTR<CDemageFont> _Demage);
	bool IsFontAllReturn();
	// 리턴 리스트에 있는 DemageFont를 ReadList에 병합
	void MergeDemageList();

	const size_t GetReadyListSize();
	const size_t GetReturnListSize();

	// Skill 재생 관련 함수
	void ClearAllQueue();
	void ClearQueue(const SKILLQUEUE_INDEX& _Index);
	void ClearQueue(const int& _Index);
	bool SetSkill(const size_t& _Index);
	bool SetSkill(const size_t& _SkillNo, const size_t& _SkillLevel);
	void SetActionScript(ACTIONSCRIPT* _ActionScript);
	void SetActionScript(const size_t& _SkillNo);
	bool SetSkillData(const size_t& _SkillNo,const size_t& _SkillLevel);
	void SetBattleUnit(CBattleUnit* _Unit);
	void SetFirstBattleUnit();
	
	//bool SetSkillData(const size_t& _SkillNo);


	vector<SKILLEFFECTDATA>* GetSkillEffectVector();
	vector<SoundData>*		GetSoundVector();

	// ActionScript 관련 Vector,Queue, Data 출력
	queue<ANIMATIONSCRIPT>* GetAniQueue();
	queue<MOVEDATA>*		GetMoveQueue();
	queue<tstring>*			GetSoundQueue();
	queue<SKILLEFFECTDATA>*	GetSkillEffectQueue();
	queue<SCENEEFFECTDATA>*	GetSceneEffectQueue();

	int* GetAnimationCount();
	int* GetMoveCount();
	int* GetSceneEffectCount();
	float* GetSkillActionTime();
	
	/*
	Vec2* GetInitPos();
	int* GetInitPosMode();
	size_t* GetInitPosIndex();
	*/

	size_t* GetInitPosMode();
	Vec2* GetInitPos();
	Vec2* GetInitPosPivot();
	

	// SkillData 관련 Vector(or List),Queue, Data 출력
	SKILLDATA* GetSkillData();
	LEVELDATA* GetSkillLevelData();
	list<HITDATA>* GetHitDataList();
	queue<HITDATA>* GetHitDataQueue();

	vector<SPTR<CBattleUnit>>* GetCurTargetVector();
	vector<vector<size_t>>* GetSkillIndexList();

	const UINT GetCurSkillNo();
	const UINT GetCurSkillLv();


	void ClearAllSkillList();
	void PushSkillIndex(const size_t& _SkillType, const size_t& _SkillIndex);


	//스킬 선택& 타겟설정 관련된 함수
	bool IsSelectSkill();
	void SelectSkillUI(const size_t& _SkillIndex);
	void UnSelectSkill();

	// 캐릭터 or 몬스터에 마우스가 충돌시 해당 충돌 유닛의 정보를 전송후
	// 현재 스킬에 맞는 타겟을 설정하고 TargetUI를 On 하는 함수
	void CheckSkillTarget(const bool& _bChar, const size_t& _TeamIndex);
	void TeamTargetUIOn(const size_t& _TeamIndex);
	void MonsterTargetUIOn(const size_t& _TeamIndex);
	void CurTargetUIOn();
	void CurTargetUIOff();


	// 스킬이 선택된 상태에서 스킬타겟을 클릭하였을때 
	// 스킬이 시전되었음을 알리고 
	// 스킬카운터에 맞추워 스킬을 작동시키도록 명령
	void TriggerSkill();
	bool isSkillTrigger();
	void SetSkillTrigger(const bool& _Trigger);
	// 스킬작동이 걸렷을때 유닛에게 자신이 공격해야할 대상을 알려줌
	void SettingUnitTarget();
	// 임의 타겟 설정 이후 출력은 타겟으로 설정된 유닛의 갯수
	const size_t SettingRandomTarget();
	const size_t SingleRandomTarget();
	const size_t ForwardTarget();
	const size_t RearTarget();
	const size_t AllTarget();             
	const size_t SingleRandomTarget(SPTR<CBattleUnit> _Unit);
	const size_t ForwardTarget(SPTR<CBattleUnit> _Unit);
	const size_t RearTarget(SPTR<CBattleUnit> _Unit);
	const size_t AllTarget(SPTR<CBattleUnit> _Unit);
	// SkillUIMgr 관련 함수

public:
	void SkillBoardOn();
	void SkillBoardOff();
	SPTR<CSkillUI> GetSkillUI(const size_t& _SkillNo);
	void SetSkillUILevel(const size_t& _SkillNo, const size_t& _SkillLevel);
public:
	// UnitCollision 관련 함수
	void CurUnitCollisionOn();
	void CurUnitCollisionOff();

	//void TargetUnitEndReaction();
	void ChangeCurUnit();
	void ReleaseTurnMgr();

	const bool CheckSkillCost();
	void CalSkillCost();

	const bool IsLastHitCheckAction();
	const bool DetectionLastHit(const UINT& _CurHitDataIndex);

	void PushDemage(const UINT& _QueueIndex, const UINT& _Demage);
	const UINT PopDemage(const UINT& _Index);

	
	void BattleUnitDeadEvent(UINT& _AccExp);
	const BATTLEPHASE GetNextPhaseToCheckPhase();

	bool IsCharacterBattleWin();
	void CharacterWinEvent(); 
	void CharacterLoseEvent();
	
	void TrigerResultWindow();
	void ResultWindowOff();
	const bool CheckChangeScene();

	void TrigerSceneChangeEvent();
	const bool IsSceneChangeEventEnd();

	void InitBattleRound();
	void RelocationNextMonster();
	void RelocationEnd();
	const bool FinishRelocation();
	

	// 캐릭터 진영에서 열단위로 현재 캐릭터가 생존하고 있는지 여부를 묻는다.
	// 몇명이 생존중인지 알려준다.
	const UINT GetLiveUnitCount(const UNITCOLUMN& _Op);
 

	vector<SPTR<CBattleUnit>>* GetCurBattleCharVec();
	vector<SPTR<CBattleUnit>>* GetCurBattleMonVec();
public:
	// 전투 라운드가 변경됨으로써 이전의 라운드에서 스킬을 선택한 유닛의
	// 타겟을 재설정 하는 작업
	void CurCharTargetOverride();

public:
	void ChangeStateDeadCharacter();
	void CharacterLevelUpEvent();


public:
	void SetCurBattleBgmIndex(const UINT& _BgmIndex);
	void PlayBgm();
	void BgmOff();
	void BgmPause(const bool& _Pause);
	void ReverseBgmPlay();
	void CurUIOff();

public:
	void GlobalCharacterDataUpdate(const bool& _bWin);

public:
	void BattleStartEvent();

public:
	void ChangeBackGround(const UINT& _BackGround);

public:
	CTurnMgr();
	~CTurnMgr();
};

