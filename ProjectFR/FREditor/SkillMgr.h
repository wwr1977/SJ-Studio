#pragma once
#include <ObjBase.h>



//class CGameScene;
class CEditBattleUnit;
class CEditEffect;
class CEditDemageFont;
class CEditDemageFontGroup;
class CSkillMgr : public CObjBase
{
	enum 
	{
		INITSKILLCOUNT = 100,
		MAXDEMAGEFONTCOUNT = 100,
	};

	enum PROGRESSSTATE
	{
		STATE_NONEPROGRESS,
		STATE_KEYCHECK,
		STATE_SKILLPROGRESS,
		STATE_OBJECTWAIT,
	};

private:
	CGameScene*												m_pSkillScene;
	CSceneBuilder*											m_pSkillSceneBuilder;

	// Dlg에서 새로 만들기 리스트를 누르면 스페이키를 눌러도 현재 스킬이 작동이 안되도록
	bool													m_bSkillStandby;

	// Skill관련 Dlg에서 스킬 사용하라고 신호를 주면 true
	SKILLMGRSTATE											m_SkillMgrState;
	// SkillMgr의 Update 내부 프로세스 로직에 대한 상태값
	UINT													m_ProgressState;

	float													m_fAccTime;
	
	HANDLE													m_ThreadHandle[FOLDERMAX];

	vector<SPTR<CEditBattleUnit>>							m_vecCharacter;
	vector<SPTR<CEditBattleUnit>>							m_vecMonster;

	SPTR<CEditBattleUnit>									m_MonTargetUnit;
	SPTR<CSoundPlayer>										m_SkillMgrSpeaker;

	bool													m_bCreateUnit;
	Vec3													m_CharacterPos;
	Vec3													m_MonsterPos;
	
	SPTR<CEditBattleUnit>									m_CurActionUnit;
	CharacterIndex											m_CurCharIndex;

	UINT													m_CurMonsterIndex;
	tstring													m_InitMonName;

	// Demage Font 
	vector<SPTR<CEditDemageFont>>								m_vecDemageFont;
	// 데미지 폰트를 뽑아오는 Queue
	list<SPTR<CEditDemageFont>>									m_DemageReadyList;
	// 시간이 다된 데미지 폰트가 돌아오는 Queue
	list<SPTR<CEditDemageFont>>									m_DemageReturnList;
	int															m_iCurOnDemageCount;
	bool														m_bAllReturn;
	
	vector<SPTR<CEditDemageFontGroup>>							m_CurDemageGroup;


	vector<SPTR<CEditBattleUnit>>								m_vecCurTurnTarget;

	// Skill 구동 관련 변수
private:
	vector<SKILL>									m_vecSkill;
	vector<vector<LEVELDATA>>						m_vecSkillLevelData;
	                    
	SKILL											m_CurSkill;
	UINT											m_CurSkillLevel;
	//Action Script
	queue<ANIMATIONSCRIPT>							m_qAniScript;
	int												m_iAniScriptCount;
	ANIMATIONSCRIPT									m_CurAniScript;
	
	queue<MOVEDATA>									m_qMoveScript;
	int												m_iMoveScriptCount;
	MOVEDATA										m_CurMoveScript;
	MOVEDATA										m_CurHideScript;

	vector<SoundData>								m_vecSoundScript;
	queue<tstring>									m_SoundQueue;

	// 스킬 이팩트 큐
	vector<SKILLEFFECTDATA>							m_vecSkillEffect;
	queue<SKILLEFFECTDATA>							m_qSkillEffect;
	vector<SPTR<CEditEffect>>						m_pCurEffect;


	queue<SCENEEFFECTDATA>							m_qSceneEffect;
	int												m_iCurSceneEffectCount;

	// 스킬이 끝나는 시간
	float											m_fSkillActionTime;
	
	// SkillData 관련 변수
	SKILLDATA										m_CurSkillData;
	LEVELDATA										m_CurSkillLevelData;
	list<HITDATA>									m_HitData;
	queue<HITDATA>									m_qHitData;

	UINT											m_InitPosMode;
	Vec2											m_InitFixPos;
	Vec2											m_InitPosPivot;
	

	bool											m_bCharTurn;
	//Skill 구동 함수                      
public:
	// Set
	void ClearAllQueue();
	void SetTarget(SPTR<CEditBattleUnit> Target);
	void SetCurSkillLevel(const UINT& _Level);
	bool SetSkill(const size_t& _SkillNo);
	bool SetSkill(const size_t& _SkillNo, const size_t& _SkillLevel);
	void SetActionScript(ACTIONSCRIPT* _ActionScript);
	void SetActionScript(const size_t& _SkillNo);
	bool SetSkillData(const size_t& _SkillNo, const size_t& _SkillLevel);
	// 스크립트 큐에서 시간이 지난 스크립트를 활성화
	void ScriptUpdate();
	// 스크립트 업데이트에서 활성화된 이펙트를 업데이트
	void ActionUpdate();
	// 스크립트 데이터 세부 업데이트 
	void HideUpdate();
	void MoveUpdate();
	void SkillEffectUpdate();
	void EffectMoveUpdate(const size_t& _EffectIndex);
	void EffectHideUpdate(const size_t& _EffectIndex);

	// SkillData 업데이트 함수

	// 스킬 데이터 관련된 데이터를 타이밍에 맞추워 활성화
	void SkillDataTimingCheck();
	// 스킬데이터 업데이트에서 활성화된 Hit 데이터를 업데이트
	void SkillDataUpdate();
	
	void HitDataUpdate(const HITDATA& _HitData);

	void CreateDemageGroup(const bool& _bChar);

public:
	const bool IsSkillProgress();
	const bool CreateAllUnit();
	const SKILLMGRSTATE GetSkillMgrState();
	vector<SKILL>* GetSkillVector();
	vector<LEVELDATA>* GetVecLevelData(const int& _SkillLevel);
	const UINT GetCurSkillNo();
	const size_t GetSkillCount();
	const SKILL GetSkill(const int& _SkillNo);
	void InitSkillMgr();
	void ThreadTextureLoading();
	void SerialTextureLoading();
	void CreateAniSprite();
	void AnimationDataLoaing();
	void CraeteEffect();
	bool CreateBattleUnit();
	bool CreateDemageFont();
	void SkillStandby(const bool _Standby);
	const bool IsSkillStandby();
	
	
	void MgrOn();
	void MgrOff();

	void Update();
	
	void KeyCheck();
	void SkillProcess();
	const bool CheckSkillEnd();
	void CharSkillProcess();
	void MonSkillProcess();
	void ObjectWait();

	void CurCharacterOn();
	void CurCharacterOff();
	void MonTargetUnitOn();
	void MonTargetUnitOff();
	void NextMonster();
	void PrevMonster();

	void PlaySkillSound(const tstring& _SoundName);
	CActObject* CreateActObject(const tstring& _ObjName);
public:
	void CharacterOn(const CharacterIndex& _CharIndex);
	void CharacterOn(const int& _CharIndex);
	void InitMonsterOn();


public:
	const UINT GetCurSkillLevel();
	const UINT SearchNextIndex();
	BOOL CreateSkill(const ACTIONSCRIPT& _Script);
	BOOL CreateSkill(const float& _TotalTime, const SKILLDATA& _SkillData, vector<LEVELDATA>* _LevelData);
	BOOL DeleteSkill(const int& _SkillNo);
	BOOL ChangeActionScript(const int& _SkillNo, const ACTIONSCRIPT& _Script);
	BOOL ChangeSkillData(const int& _SkillNo, const SKILLDATA& _SkillData, vector<LEVELDATA>* _LevelData);
	BOOL LoadSkillFile(FILE* _ReadFile);
	BOOL LoadSkillFile(FILE* _SkillFile, FILE* _LevelFile);
	BOOL SaveSkillFile(FILE* _WriteFile);
	BOOL SaveSkillFile(FILE* _SkillFile, FILE* _LevelFile);

	//DemageFont 관련
	SPTR<CEditDemageFont> GetDemageFont(const int& _Number, const Vec3& _RenderPos, const size_t& NumberSpriteIndex);
	void ReturnDemageFont(SPTR<CEditDemageFont> _Demage);
	bool IsFontAllReturn();
	// 리턴 리스트에 있는 DemageFont를 ReadList에 병합
	void MergeDemageList();

	const size_t GetReadyListSize();
	const size_t GetReturnListSize();

	void SetInitCharPos();

public:
	CSkillMgr();
	CSkillMgr(CGameScene* _Scene, CSceneBuilder* _Builder);
	~CSkillMgr();
};

