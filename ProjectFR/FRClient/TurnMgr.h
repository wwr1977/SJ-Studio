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

	// ��� ���� ĳ���� , ���͸� ���� �����(Actor + Logic)
	vector<SPTR<CBattleUnit>>								m_vecCharacter;
	//map<tstring, SPTR<CBattleUnit>>							m_mapMonster;
	unordered_map<tstring, list<SPTR<CBattleUnit>>>			m_mapMonsterList;

	//��� ���� ĳ���͵��� Hp,Mp bar�� ���� �����(Actor + Logic)
	//unordered_map<tstring,SPTR<CUnitStatusBar>>				m_mapStatusBar;
	unordered_map<tstring, list<SPTR<CUnitStatusBar>>>		m_mapStatusBarList;
	Vec2													m_CharStatusSize;
	Vec2													m_MonStatusSize;

	// Turn�� �帧�� ǥ���ϴ� UI�� ��� ���� �����
	//unordered_map<tstring, SPTR<CTurnOrderUI>>				m_mapTurnUI;
	unordered_map<tstring, list<SPTR<CTurnOrderUI>>>		m_mapTurnUIList;

	// ���� ������ ������ ĳ���͵�
	vector<SPTR<CBattleUnit>>								m_vecCurBattleChar;
	UINT													m_CurCharCount;
	
	// ���� ������ ������ ����
	vector<SPTR<CBattleUnit>>								m_vecCurBattleMon;
	UINT													m_CurMonCount;

	// Turn�� �帧�� ǥ���ϱ� ���� UI (������ ���)
	list<SPTR<CTurnOrderUI>>								m_listCurTurnUI;

	// DamegeFont�� ��Ƶ� ����� , Queue
	vector<SPTR<CDemageFont>>								m_vecDemageFont;
	// ������ ��Ʈ�� �̾ƿ��� Queue
	list<SPTR<CDemageFont>>									m_DemageReadyList;
	// �ð��� �ٵ� ������ ��Ʈ�� ���ƿ��� Queue
	list<SPTR<CDemageFont>>									m_DemageReturnList;
	// ���� pop�� DemageFont�� ���� �� pop�Ǿ��� ��� DemageFont�� ReturnList���ƿӴ��� üũ�ϴ� ����
	int														m_iCurOnDemageCount;
	bool													m_bAllReturn;
	/*bool													m_FontReturnCheck;
	float													m_fAccReturnCheckTime;
	float													m_fMaxReturn;*/
	


	// ���� ĳ���͵��� ��ġ�� ����(0~4 ��,�� 5���� �ڸ�)
	vector<Vec3>									m_vecCharacterPos;
	// ���� ĳ������ �������ͽ� â�� ��ġ(0~4 ��, �� 5���� �ڸ�)
	vector<Vec3>									m_vecCharStatusPos;
	// ���� ������ �������ͽ� â�� ��ġ(0~2 ��, �� 3���� �ڸ�)
	vector<Vec3>									m_vecMonStatusPos;

	// Turn UI�� ��ġ�� ����Ǿ��ִ� ����� 
	vector<Vec3>									m_vecTurnUIPos;
	// ĳ���͵��� ���� ǥ���ϱ� ���� Effect�� ��ġ
	vector<Vec3>									m_vecSelectEffectPos;
	// ���͵��� ��ġ�� ��Ҹ� ����
	vector<Vec3>									m_vecMonsterPos;


	// Turn UI�� ũ�⸦ Custom���� �����ϱ� ���� ������ ����
	Vec2											m_FirstFrameSize;
	Vec2											m_OtherFrameSize;

	// SkillBackBoard ��ġ�� ũ��
	Vec3											m_SkillBackBoardPos;
	Vec2											m_SkillBackBoardSize;

	// ��ų�� �ߵ� �Ǿ����� true �� �Ǵ� ����
	// Select Phase���� �� ���� üũ�Ѵ�.
	bool											m_bSkillTrigger;
	UINT											m_CurSkillLevel;


	// Skill ���� ���õ� ��� ����ҿ� Queue
	// Action Script ���� ����� & Queue
	
	// Skill1)  Unit�� �ִϸ��̼� ���� ��ũ��Ʈ
	queue<ANIMATIONSCRIPT>							m_qAniScript;
	// ���� ť�� ����ִ� �ִϸ��̼� ��ũ��Ʈ�� ����
	int												m_iAniScriptCount;

	// Unit�� ������ ���� ��ũ��Ʈ
	queue<MOVEDATA>									m_qMoveScript;
	// ���� ť�� ����ִ� Move ��ũ��Ʈ�� ����
	int												m_iMoveScriptCount;

	// ������ ��� or ȿ���� ���� ����� & Queue
	vector<SoundData>								m_vecSoundScript;
	queue<tstring>									m_SoundQueue;

	// ��ų ����Ʈ ť
	vector<SKILLEFFECTDATA>							m_vecSkillEffect;
	queue<SKILLEFFECTDATA>							m_qSkillEffect;

	// ī�޶� Ư��ȿ�� ť
	queue<SCENEEFFECTDATA>							m_qSceneEffect;
	// ���� Ư��ȿ�� ť�� ����ִ� ��ü�� ����
	int												m_iCurSceneEffectCount;
	
	// ��ų�� ������ �ð�
	float											m_fSkillActionTime;

	// Skill2) SkillData �� Hit Data ���� Data
	SKILLDATA										m_CurSkillData;
	LEVELDATA										m_CurSkillLevelData;
	list<HITDATA>									m_HitData;
	queue<HITDATA>									m_qHitData;
	
	// Attack or DoubleAttack ��ų�� ���������� ��Ÿ���� ������
	bool											m_bLastHitCheck;
	UINT											m_LastHitIndex;
	
private:
	SPTR<CBattleUnit>								m_CurBattleUnit;
	// Rotate Phase���� ���� ���� Unit�� ���� �󸶳� ������ �Ǵ��ϴ� ����
	// 0���� ������� ���� �ڷ� �о������
	int												m_iRotateCount;
	float											m_fAccTime;
	
	//Vec2											m_InitPos;
	//int												m_InitPosMode;
	//size_t											m_InitPosIndex;

	size_t											m_InitPosMode;
	Vec2											m_InitFixPos;
	Vec2											m_InitPosPivot;




	// ���� ���� ����Ǵ� ĳ���Ͱ� ���� ��ų ��ȣ�� �����Ѵ�.
	// �� ������ SkillUIBoard�� ��� �Ұ��̹Ƿ� �ش簴ü���� �����͸� �Ѱ��ٰ��̴�.
	vector<vector<size_t>>							m_CurSkillIndexList;


	// ���� ���õ� ��ų�� Index�� ����
	// -1�� ������ �ȵ� ���¸� �ǹ�
	size_t											m_CurSelectSkill;
	TARGET											m_SelectTarget;
	// ���� ���� Ÿ�� UI�� �ӽ÷� ����(���Ϳ��� ��������� List���� UI�� Off)
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
	// Rotate ī��Ʈ ��ŭ UI�� �ڷ� �о��ִ� �Լ�
	bool PushTurnUI(SPTR<CTurnOrderUI> _TurnUI, int _RotateCount);
	// Turn List�� ���� �տ� �ִ� UI�� Pop
	bool PopCurTurnUI();

	void InitList();
	void Update();
	
	void InitSkillUIMgr();

public:
	void ChangePhase(int _Index);
	// StandByPhase���� ����� �۾�(��� ������Ʈ Off & ���� ������Ʈ ����)
	void AllObjectOff();
	void CurActiveObjectOff();
	void SettingBattleObject();
	void UnitPairUIOn();
	// ����Ʈ���� Turn UI�� � ������ ���� ���Ͽ�(Dex ����)
	// Sort�� ������ �� �� ������ TurnUI�� �ű�
	void InitTurnUIPos();

	// Rotate Phase���� ����� ���� ���� or ȹ��
	void SetRotateCount(const int& _RotCount);
	int GetRotateCount();
	void PopDeadUnitUI();
	bool UpdateTurnUIPos();
	// ������ Ui�� ���� �Ʒ��� ���������� UI �����Ʈ
	bool DefaultRotateEvent();
	// m_iRotateCount��ŭ �ֻ��� Ui�� �Ʒ��� ������ �����Ʈ
	bool ChangeRotateEvent();
	// �� �����Ʈ Event�� ������ ���������� ��ġ������ �ϴ� �Լ�
	void SyncroUIPos();
	UINT RandomPushTurnUI(SPTR<CTurnOrderUI> _UI);
	void InitUILocation();
	void ClearAllDemageQueue();

	// Demage Font ���� �Լ�
	SPTR<CDemageFont> GetDemageFont(const int& _Number, const Vec3& _RenderPos, const size_t& NumberSpriteIndex);
	void PopDemageFont(const int& _Number, const Vec3& _RenderPos, const int& _Count = 1);
	void PopDemageFont(const int& _Number, const Vec3& _RenderPos, const int& _Count , const tstring& _NumberSpriteName);
	void PopDemageFont(const int& _Number, const Vec3& _RenderPos, const int& _Count, const size_t& NumberSpriteIndex);
	// RenderPos �� ��Ʈ�� �߽������� �����ϴ� �Լ�
	void PopMiddleDemageFont(const int& _Number, const Vec3& _RenderPos, const UINT& _Count, const size_t& NumberSpriteIndex = DEMAGE_BLACK);
	

	void ReturnDemageFont(SPTR<CDemageFont> _Demage);
	bool IsFontAllReturn();
	// ���� ����Ʈ�� �ִ� DemageFont�� ReadList�� ����
	void MergeDemageList();

	const size_t GetReadyListSize();
	const size_t GetReturnListSize();

	// Skill ��� ���� �Լ�
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

	// ActionScript ���� Vector,Queue, Data ���
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
	

	// SkillData ���� Vector(or List),Queue, Data ���
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


	//��ų ����& Ÿ�ټ��� ���õ� �Լ�
	bool IsSelectSkill();
	void SelectSkillUI(const size_t& _SkillIndex);
	void UnSelectSkill();

	// ĳ���� or ���Ϳ� ���콺�� �浹�� �ش� �浹 ������ ������ ������
	// ���� ��ų�� �´� Ÿ���� �����ϰ� TargetUI�� On �ϴ� �Լ�
	void CheckSkillTarget(const bool& _bChar, const size_t& _TeamIndex);
	void TeamTargetUIOn(const size_t& _TeamIndex);
	void MonsterTargetUIOn(const size_t& _TeamIndex);
	void CurTargetUIOn();
	void CurTargetUIOff();


	// ��ų�� ���õ� ���¿��� ��ųŸ���� Ŭ���Ͽ����� 
	// ��ų�� �����Ǿ����� �˸��� 
	// ��ųī���Ϳ� ���߿� ��ų�� �۵���Ű���� ���
	void TriggerSkill();
	bool isSkillTrigger();
	void SetSkillTrigger(const bool& _Trigger);
	// ��ų�۵��� �ɷ����� ���ֿ��� �ڽ��� �����ؾ��� ����� �˷���
	void SettingUnitTarget();
	// ���� Ÿ�� ���� ���� ����� Ÿ������ ������ ������ ����
	const size_t SettingRandomTarget();
	const size_t SingleRandomTarget();
	const size_t ForwardTarget();
	const size_t RearTarget();
	const size_t AllTarget();             
	const size_t SingleRandomTarget(SPTR<CBattleUnit> _Unit);
	const size_t ForwardTarget(SPTR<CBattleUnit> _Unit);
	const size_t RearTarget(SPTR<CBattleUnit> _Unit);
	const size_t AllTarget(SPTR<CBattleUnit> _Unit);
	// SkillUIMgr ���� �Լ�

public:
	void SkillBoardOn();
	void SkillBoardOff();
	SPTR<CSkillUI> GetSkillUI(const size_t& _SkillNo);
	void SetSkillUILevel(const size_t& _SkillNo, const size_t& _SkillLevel);
public:
	// UnitCollision ���� �Լ�
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
	

	// ĳ���� �������� �������� ���� ĳ���Ͱ� �����ϰ� �ִ��� ���θ� ���´�.
	// ����� ���������� �˷��ش�.
	const UINT GetLiveUnitCount(const UNITCOLUMN& _Op);
 

	vector<SPTR<CBattleUnit>>* GetCurBattleCharVec();
	vector<SPTR<CBattleUnit>>* GetCurBattleMonVec();
public:
	// ���� ���尡 ��������ν� ������ ���忡�� ��ų�� ������ ������
	// Ÿ���� �缳�� �ϴ� �۾�
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

