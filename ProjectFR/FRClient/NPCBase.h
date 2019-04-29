#pragma once
#include <Logic.h>
class CNPCBase : public CLogic
{
	static vector<tstring>		IdleDir;
public:
	static void InitNpcBase();
public:

	enum IDLEDIR
	{
		IDLE_DOWN,
		IDLE_UP,
		IDLE_LEFT,
		IDLE_RIGHT,
		MAXIDLEDIR,
	}m_DefaultDir;

	enum HIDETYPE
	{
		HIDE_DEFAULT,
		HIDE_VIBRATION,
		HIDE_EFFECT,
		HIDE_DEADANI,
		MAXHIDETYPE
	}m_CurHideType;

	enum NPCTYPE
	{
		DIALOG_NPC,
		WEAPON_STORE_NPC,
		ARMOR_STORE_NPC,
		ACCESSORIES_STORE_NPC,
		MAXSTORENPC
	}m_CurNPCType;

	static bool bScriptEvent;
private:
	friend class CScriptMgr;
	friend class CGameMaster;

protected:
	tstring						m_NpcName;
	tstring						m_NpcKName;
	IDLETYPE					m_IdleType;
	SPTR<CFixRenderer>			m_FixNpcRen;
	SPTR<CAniRenderer>			m_AniNpcRen;
	
	// 플레이어에게만 반응하는 충돌체
	SPTR<CCollision>			m_PlayerPushCol;
	SPTR<CCollision>			m_ScriptCol;

	vector<SPTR<CGameSprite>>	m_vecFixIdleSprite;
	float						m_fAccTime;

	bool						m_bHide;
	bool						m_bHideEventEnd;
	bool						m_bEffectPlay;
public:
	void Init();
	void InitData(const NPCDATA& _Data);
	void Update();
	void LastUpdate();
	void CreatePlayerCollision(const COLFIGURE& _Figure,const COLTYPE& _ColType);
	void CreateScriptCollision(const COLFIGURE& _Figure, const COLTYPE& _ColType);
	virtual void CreateSubCollision(const COLFIGURE& _Figure, const COLTYPE& _ColType);


public:
	void HideUpdate();
	void DefaultHideUpdate();
	void VibrationHideUpdate();
	void EffectHideUpdate();
	void DeadHideUpdate();

public:
	void PushEnter(SPTR<CCollision> _pThis, SPTR<CCollision> _pOtherCol);
	void PushStay(SPTR<CCollision> _pThis, SPTR<CCollision> _pOtherCol);
	void PushExit(SPTR<CCollision> _pThis, SPTR<CCollision> _pOtherCol);

	void ScriptEnter(SPTR<CCollision> _pThis, SPTR<CCollision> _pOtherCol);
	void ScriptStay(SPTR<CCollision> _pThis, SPTR<CCollision> _pOtherCol);
	void ScriptExit(SPTR<CCollision> _pThis, SPTR<CCollision> _pOtherCol);


public:
	void SetStoreNPC(const NPCTYPE& _NPCType);
	void SetAniIntervalTime(const float& _Time);
	void SettingFixNpc();
	void SettingAniNpc();
	void SetAlpha(const float& _Alpha);
	void SetRenPivot(const Vec2& _Pivot);
	void SetHideType(const HIDETYPE& _Type);
	void SetDead_Dying_NPC();

public:
	void ChangeAni_Dying();
	void ChangeAni_Dead();

public:
	void Reset();

public:
	const bool IsHide();
	const bool IsHideEventEnd();

public:
	void InitDefaultDir();
	void SetDefaultDir(const IDLEDIR& _Dir);
	void ChangeIdleDir(const IDLEDIR& _Dir);
	const IDLEDIR CalNPCDir();

public:
	void NPCHideEvent();
	void HideEventEnd();

public:
	CNPCBase();
	~CNPCBase();
};

