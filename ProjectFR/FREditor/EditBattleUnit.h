#pragma once
#include <Logic.h>

class CSkillMgr;
class CEditBattleUnit : public CLogic
{
	friend class CSkillMgr;

	static D3DXCOLOR FocusMonBackColor;
	// BattleUnit이 사용하는 TurnMgr 전용 포인터
	static CSkillMgr* pUnitSkillMgr;
	static void SetTurnMgr(CSkillMgr* _TurnMgr);



	SPTR<CAniRenderer>						m_AniRen;
	// 몬스터 전용 리액션 랜더러(.atk 의 0번째 Sprite 사용)
	SPTR<CFixRenderer>						m_MonReactionRen;


	float									m_fAlpha;
	SPTR<CStateMgr>							m_PairMgr;
	SPTR<CSoundPlayer>						m_UnitSound;

	tstring									m_UnitName;
	size_t									m_iCharIndex;
	int										m_iLevel;
	int										m_iTeamNumber;
	// 레벨이 변경되었을때를 감지하는 변수
	bool									m_bLevelUpdate;
	bool									m_bCharacter;


	// 스킬의 턴카운트  - 캐릭터의 턴 카운트 = 밀리는 턴
	int										m_iTurnCount;
	// Select Phase때 이미 행동이 정해져 있는지에 대한 여부
	bool									m_bSelectAction;
	// Select Phase때 선택된 스킬의 번호
	size_t									m_SelectSkillNo;
	size_t									m_SelectSkillLevel;
	// Select Phase에서 선택된 타겟그룹 정보
	TARGET									m_SelectTarget;

	// Action Phase때 행동이 끝났는 지에 대한 여부 
	bool									m_bEndAction;

	// Action Phase때 어떤 행동을 할지에 대한 값
	int										m_iCurActionIndex;


	vector<STAT>*							m_pUnitStatTable;
	STAT									m_CurUnitStat;
	STATPARAMETERDATA						m_UnitPara;

	UNITSTATE								m_CurUnitState;

	unordered_map<size_t, size_t>			m_mapUnitSkillData;

private:
	// 필터 적용 시간
	float									m_fFilterTime;
	// 필터 적용 이후 누적 시간(Check용)
	float									m_fAccFilterTime;
	// 필터를 몇번 적용할것인지 체크(반짝반짝 효과)
	int										m_iFilterCount;
	// 필터 적용이 완전히 종료되었는가를 체크
	bool									m_bFilterEnd;
	// 필터 색상 적용을 체크
	bool									m_bFilterOn;

	// 캐릭터 진통을 체크하기 위한 변수
	float									m_fVibTime;
	float									m_fAccVibTime;
	bool									m_bVibOn;

	// 캐릭터의 공격 및 스킬의 타겟이 되는 Unit의 포인터를 저장하는 변수
	list<SPTR<CEditBattleUnit>>					m_CurTargetUnit;

	// HitEffect 와 DemageFont가 뜨는 위치의 피봇값
	Vec3									m_UnitMiddlePivot;
	Vec3									m_DemageFontPivot;

	SPTR<CCollision>						m_UnitCol;

	// 체력이 0이되어 전투 불능 상태시 On이 되는 변수
	bool									m_bDead;

public:
	virtual void InitData(const tstring& _UnitName);
	virtual void InitData(const CharacterIndex _UnitIndex);

	// 캐릭터 or 몬스터로 배틀유닛을 생성
	void InitalizeCharacter();
	void InitalizeMonster();
	void LoadUseSkill();

	void Init();
	void Update();



	void SetDead(const bool& _Dead = true);
	bool isDead();
	bool IsCurAniEnd();
	bool ChangeAni(const ANIMATIONNAME& AniIndex);
	bool ChangeAni(const int& _Index);
	bool ChangeAni(const tstring& _AniKey);
	// State Change 되기 전에 한번 수행되는 함수
	void ChangeStateEvent();
	void ChangeState(int _Index);

	void SetTurnCount(const int& _TurnCount);


	void SetFilter(const D3DXCOLOR& _Color = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
	void FilterOff();
	void SetTeamNumber(const int& _TeamIndex);
	int GetTeamNumber();

	void Enter(SPTR<CCollision> _pThis, SPTR<CCollision> _pOtherCol);
	void Stay(SPTR<CCollision> _pThis, SPTR<CCollision> _pOtherCol);
	void Exit(SPTR<CCollision> _pThis, SPTR<CCollision> _pOtherCol);


public:
	int	GetCharIndex();
	tstring GetUnitName();
	int GetCurAniFrame();
	int GetCurAniLooping();
	int GetTurnCount();
	const Vec3 GetPos();
	// Vec2용 위치
	const Vec2 GetPosVec2();
	// 랜더링 피벗까지 포함된 포스
	SPTR<CSoundPlayer> GetUnitSoundPlayer();
	unordered_map<size_t, size_t>* GetUnitSkillDataMap();


public:
	STAT SetLevel(const int& _Level);
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
	// Action Phase에서 유닛의 스킬관련 정보를 모두 전송한뒤
	// 선택 스킬 관련된 모든 데이터를 초기화
	void InitSelectSkillData();

public:
	// 밀리는 턴을 계산하는 함수(스킬턴 카운트 - 캐릭터 턴 카운트)
	int CalTurnCount(const int& _SkillTurnCount);

public:
	// 선택한 Action이 활성화 가능한지 판단하는 여부
	bool ActionCheck();

public:
	void On();
	void On(const Vec3& _SelectPos);
	void Off();
	void SetPos(const Vec2& _Pos);
	void SetPos(const Vec3& _Pos);
	void SetIdle();
	void SetAction();
	void SetReAction();

	// 현재 상태가 리액션 상태일때 Idle상태로 변경해주는 함수
	void EndReAction();
	const UNITSTATE GetCurUnitState();
	void AniRenOn();
	void ReactionRenOn();
	void AniRenOff();
	void ReactionRenOff();


	void SelectActionIndex(int _ActionIndex);
	int GetCutActionIndex();

	bool IsCharacter();
	// Unit의 현 상태를 반영하는 함수,기본값은 true  
	bool IsEndAction();
	bool IsSelectAction();
	void EndAction(bool _EndAction = true);
	void SelectAction(bool _SelectAction = true);
	void SelectSkill(const size_t& _SkillNo);
	void SetSelectTarget(const size_t& _Target);
	void SetSelectTarget(const TARGET& _Target);

	const size_t GetCurSelectSkill();
	const size_t GetSelectSkill();
	const TARGET GetSelectTarget();


	// 유닛이 스킬 or 공격 시전시 타겟유닛의 포인터를 저장 & 초기화
	void ClearCurTarget();
	bool SetCurTarget(const SPTR<CEditBattleUnit> _TargetUnit);
	list<SPTR<CEditBattleUnit>>* GetCurTargetUnit();

public:
	// 유닛의 색상을 단색 계통으로 바꿀때 사용하는 함수
	bool SetFilter(const D3DXCOLOR& _Color, const float& _FilterTime, const int& _Count = 1);
	// 필터를 적용,해제 하는 함수
	void FilterUpdate();
	void SetVib(const float& _Time = LARGEFLOAT);
	void VibrationUpdate();

	bool ReActionEvent(const REACTION& _ReactionMode);
	bool ReActionEvent(const int& _ReactionMode);
	CActObject* CreateActObject(const tstring& _Name = _T(""));
	const STAT GetCurUnitStat();


public:
	// 실제 랜더링 위치 + (UnitSprite* ACTORIMAGEMAG)*0.5f = UnitMiddlePos
	const Vec3 GetUnitMiddlePos();
	// 실제 랜더링 위치 + (UnitSprite* ACTORIMAGEMAG)*1.0f = UnitFontPos
	const Vec3 GetUnitDemageFontPos();

public:
	// SkillIData Map 관련 함수
	const size_t GetTotalSkillCount();
	const size_t GetSkillLevel(const size_t& SkillNo);

public:
	// Collision 컴포넌트 관련 함수
	void CollisionComOff();
	void CollisionComOn();


public:
	const Vec2 GetUnitFrontPos();
	const Vec2 GetUnitBackPos();

	void TargetUnitEndReaction();

	void SetUnitImageMag(const float& _Mag);
public:
	CEditBattleUnit();
	~CEditBattleUnit();
};

