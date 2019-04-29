#pragma once
#include <Logic.h>



class CUnitFont;
class CTargetUI;
class CUnitStatusBar;
class CCharSelectEffect;
class CTurnOrderUI;
class CTurnMgr;
class CBattleUnit : public CLogic
{
	friend class CTurnMgr;

	static D3DXCOLOR FocusMonBackColor;
	// BattleUnit�� ����ϴ� TurnMgr ���� ������
	static CTurnMgr* pUnitTurnMgr;
	static void SetTurnMgr(CTurnMgr* _TurnMgr);
	

private:
	CUnitStatusBar*							m_PairStatus;
	CTurnOrderUI*							m_PairTurnUI;
	class CCharResultUI*					m_PairResultUI;
	class CBossGauge*						m_BossGauge;

private:
	SPTR<CCharSelectEffect>					m_SelectEffect;
	SPTR<CUnitFont>							m_PairUnitFont;
	
	SPTR<CAniRenderer>						m_AniRen;
	// ���� ���� ���׼� ������(.atk �� 0��° Sprite ���)
	SPTR<CFixRenderer>						m_MonReactionRen;
	
	
	float									m_fAlpha;
	SPTR<CStateMgr>							m_PairMgr;
	SPTR<CSoundPlayer>						m_UnitSound;

	//tstring									m_UnitName;
	UNITNAME								m_UnitName;
	size_t									m_iCharIndex;
	int										m_iLevel;
	int										m_iTeamNumber;
	// ������ ����Ǿ������� �����ϴ� ����
	bool									m_bLevelUpdate;
	bool									m_bCharacter;
	bool									m_bCurBattle;
	
	
	// ��ų�� ��ī��Ʈ  - ĳ������ �� ī��Ʈ = �и��� ��
	int										m_iTurnCount;
	// Select Phase�� �̹� �ൿ�� ������ �ִ����� ���� ����
	bool									m_bSelectAction;
	// Select Phase�� ���õ� ��ų�� ��ȣ
	size_t									m_SelectSkillNo;
	size_t									m_SelectSkillLevel;
	// Select Phase���� ���õ� Ÿ�ٱ׷� ����
	TARGET									m_SelectTarget;

	// Action Phase�� �ൿ�� ������ ���� ���� ���� 
	bool									m_bEndAction;

	// Action Phase�� � �ൿ�� ������ ���� ��
	int										m_iCurActionIndex;
	

	vector<STAT>*							m_pUnitStatTable;
	STAT									m_CurUnitStat;	
	UINT									m_CurUnitStatus[LEVELDATA::MAXSKILLCOST];
	STAT									m_BaseStat;
	STAT									m_ItemStat;
	STATPARAMETERDATA						m_UnitPara;

	UNITSTATE								m_CurUnitState;
	
	// �ڱⰡ ������ ��ų�� Index�� vector�� ����
	//vector<size_t>						m_vecSkillIndex;
	//vector<UNITSKILLDATA>					m_vecUnitSkillData;
	unordered_map<size_t, size_t>			m_mapUnitSkillData;

private:
	// ���� ���� �ð�
	float									m_fFilterTime;
	// ���� ���� ���� ���� �ð�(Check��)
	float									m_fAccFilterTime;
	// ���͸� ��� �����Ұ����� üũ(��¦��¦ ȿ��)
	int										m_iFilterCount;
	// ���� ������ ������ ����Ǿ��°��� üũ
	bool									m_bFilterEnd;
	// ���� ���� ������ üũ
	bool									m_bFilterOn;

	// ĳ���� ������ üũ�ϱ� ���� ����
	float									m_fVibTime;
	float									m_fAccVibTime;
	bool									m_bVibOn;

	// ĳ������ ���� �� ��ų�� Ÿ���� �Ǵ� Unit�� �����͸� �����ϴ� ����
	list<SPTR<CBattleUnit>>					m_CurTargetUnit;

	// HitEffect �� DemageFont�� �ߴ� ��ġ�� �Ǻ���
	Vec3									m_UnitMiddlePivot;
	Vec3									m_DemageFontPivot;

	SPTR<CCollision>						m_UnitCol;

	// Target UI(������ ���콺�� ���ý� �ٴڿ� �ߴ� UI)
	SPTR<CTargetUI>							m_PairTargetUI;

	// ü���� 0�̵Ǿ� ���� �Ҵ� ���½� On�� �Ǵ� ����
	bool									m_bDead;

	SPTR<CLineRenderer>						m_UnitPosRen;
	
	// ���尡 ����Ǿ ���� ���忡 ������ Ÿ���� ��� ��ȿȭ �Ǿ����� �˸���.
	bool									m_bTargetOverride;


	vector<UINT>							m_vecMonsterSkillNo;
	bool									m_bCalExp;

	size_t									m_CharacterLine;
	size_t									m_LineIndex;

	bool									m_bBoss;
public:
	virtual void InitData(const tstring& _UnitName);
	virtual void InitData(const CharacterIndex _UnitIndex);

	// ĳ���� or ���ͷ� ��Ʋ������ ����
	void InitalizeCharacter();
	void InitalizeMonster();
	void LoadUseSkill();

	void Init();
	void Update();
	void PostUpdate();
	void DebugRender();



	void SetDead(const bool& _Dead = true);
	bool isDead();
	bool IsCurAniEnd();
	bool IsBossMonster();
	bool ChangeAni(const ANIMATIONNAME& AniIndex);
	bool ChangeAni(const int& _Index);
	bool ChangeAni(const tstring& _AniKey);
	// State Change �Ǳ� ���� �ѹ� ����Ǵ� �Լ�
	void ChangeStateEvent();
	void ChangeState(int _Index);
	void SetStatusBar(CUnitStatusBar* _Pair);
	void SetTurnUI(CTurnOrderUI* _Pair);
	void SetResultUI(CCharResultUI* _Pair);
	void SetTurnCount(const int& _TurnCount);
	void SetCurExp(const int& _Exp);
	void SelectEffectOn();
	void SelectEffectOff();
	

	void SetFilter(const D3DXCOLOR& _Color = D3DXCOLOR(1.0f,0.0f,0.0f,1.0f));
	void FilterOff();
	void SetTeamNumber(const int& _TeamIndex);
	int GetTeamNumber();
	
	void Enter(SPTR<CCollision> _pThis, SPTR<CCollision> _pOtherCol);
	void Stay(SPTR<CCollision> _pThis, SPTR<CCollision> _pOtherCol);
	void Exit(SPTR<CCollision> _pThis, SPTR<CCollision> _pOtherCol);
	

public:
	SPTR<CTargetUI>	GetTargetUI();
	int	GetCharIndex();
	tstring GetUnitName();
	tstring GetUnitKName();
	int GetCurAniFrame();
	int GetCurAniLooping();
	int GetTurnCount();
	const Vec3 GetPos();
	// Vec2�� ��ġ
	const Vec2 GetPosVec2();
	// ������ �ǹ����� ���Ե� ����
	CTurnOrderUI* GetPairTurnUI();
	CUnitStatusBar* GetPairStatusBar();
	CCharResultUI* GetPairResultUI();
	CBossGauge* GetPairBossGauge();
	SPTR<CSoundPlayer> GetUnitSoundPlayer();
	unordered_map<size_t, size_t>* GetUnitSkillDataMap();
	

public:
	STAT SetLevel(const int& _Level);
	STAT LevelUp(const int& _Up = 1);
	int	 GetLevel();

	const tstring GetLevelString();
	bool IsUpdateLevel();
	void UpdateLevel();
	STAT GetCurStat();
	STATPARAMETERDATA GetStatParameter();
	void SetRenPivot(const Vec2& _Pivot);
	void SetAlpha(const float& _AlphaRatio);
	void SetAlpha();
	bool PlaySound(const tstring& _SoundName);
	// Action Phase���� ������ ��ų���� ������ ��� �����ѵ�
	// ���� ��ų ���õ� ��� �����͸� �ʱ�ȭ
	void InitSelectSkillData();
	const float GetCriticalMag();
public:
	// �и��� ���� ����ϴ� �Լ�(��ų�� ī��Ʈ - ĳ���� �� ī��Ʈ)
	int CalTurnCount(const int& _SkillTurnCount);

public:
	// ������ Action�� Ȱ��ȭ �������� �Ǵ��ϴ� ����
	bool ActionCheck();

public:
	void On();
	void On(const Vec3& _SelectPos);
	void UnitOn(const Vec3& _SelectPos = Vec3{0.f,0.f,0.f});
	void PairUIOn();
	
	void Off();
	void UnitOff();
	
	void SetPos(const Vec2& _Pos);
	void SetPos(const Vec3& _Pos);
	void SetIdle();
	void SetAction();
	void SetReAction();
	void SetAppear();
	void SetDying();
	void SetDead();
	void SetInitLevel();


	// ���� ���°� ���׼� �����϶� Idle���·� �������ִ� �Լ�
	void EndReAction();
	const UNITSTATE GetCurUnitState();
	void AniRenOn();
	void ReactionRenOn();
	void AniRenOff();
	void ReactionRenOff();


	void SelectActionIndex(int _ActionIndex);
	int GetCutActionIndex();
	
	bool IsCharacter();
	const bool IsCurBattle();
	void CurBattle(const bool& _Battle);
	// Unit�� �� ���¸� �ݿ��ϴ� �Լ�,�⺻���� true  
	bool IsEndAction();
	bool IsSelectAction();
	void EndAction(bool _EndAction = true);
	void SelectAction(bool _SelectAction =true);
	void SelectSkill(const size_t& _SkillNo);
	void SelectSkill(const size_t& _SkillNo,const size_t& _SkillLevel);
	void SetSelectTarget(const size_t& _Target);
	void SetSelectTarget(const TARGET& _Target);

	const size_t GetCurSelectSkill();
	const size_t GetSelectSkill();
	const TARGET GetSelectTarget();


	// ������ ��ų or ���� ������ Ÿ�������� �����͸� ���� & �ʱ�ȭ
	void ClearCurTarget();
	bool SetCurTarget(const SPTR<CBattleUnit> _TargetUnit);
	list<SPTR<CBattleUnit>>* GetCurTargetUnit();

public:
	// ������ ������ �ܻ� �������� �ٲܶ� ����ϴ� �Լ�
	bool SetFilter(const D3DXCOLOR& _Color, const float& _FilterTime,const int& _Count = 1);
	// ���͸� ����,���� �ϴ� �Լ�
	void FilterUpdate();
	void SetVib(const float& _Time = LARGEFLOAT);
	void VibrationUpdate();

	bool ReActionEvent(const REACTION& _ReactionMode);
	bool ReActionEvent(const int& _ReactionMode);
	CActObject* CreateActObject(const tstring& _Name = _T(""));
	const STAT GetCurUnitStat();


	bool CheckSkillAvailable(const LEVELDATA& _Data);
	// Action ������ ��ų �ڽ�Ʈ�� �����ϰ� ��ų �ߵ�
	void CalSkillCost(const LEVELDATA& _Data);
public:
	// ���� ������ ��ġ + (UnitSprite* ACTORIMAGEMAG)*0.5f = UnitMiddlePos
	const Vec3 GetUnitMiddlePos();
	// ���� ������ ��ġ + (UnitSprite* ACTORIMAGEMAG)*1.0f = UnitFontPos
	const Vec3 GetUnitDemageFontPos();

public:
	// SkillIData Map ���� �Լ�
	const size_t GetTotalSkillCount();
	const size_t GetSkillLevel(const size_t& SkillNo);

public:
	// Collision ������Ʈ ���� �Լ�
	void CollisionComOff();
	void CollisionComOn();

public:
	const Vec2 GetUnitFrontPos();
	const Vec2 GetUnitBackPos();

	void TargetUnitEndReaction();


	// ������ ������ ȸ���ϰ� �ڱ� �Ӹ����� ��Ʈ�� ����.
	void LastAttackEvent();
	void DemagedEvent(const UINT& _Demage);
	const UINT GetCurHp();
	const UINT GetCurMp();
	const UINT GetMaxHp();
	const UINT GetMaxMp();

	void StandBy();
private:
	void StandByCharacter();
	void StandByMonster();
	// Debug ����
public:
	void SyncroDebugLine();
	void UnitPosLineOn();
	void UnitPosLineOff();
	void ReverseLineRen();

	void InitAppear();

public:
	void TargetOverride(const bool _Override);
	const bool IsTargetOverrideByChangeRound();


	void SettingMonsterData();
	const UINT GetRandomSkill();
	const size_t CurRandomSkillNo();
	const size_t CurRandomSkillLv();
	const UINT CurRandomSkillCount();

	const bool CompareDex(SPTR<CBattleUnit> _Other);
	const UINT GetExp();
	const UINT GetExpEvent(const UINT& _Exp);


	const UINT GetCurUnitExp();
	void PairUIOff();

public:
	void CharacterDataUpdate(const bool& _bWin);

public:
	CBattleUnit();
	virtual ~CBattleUnit();
};

