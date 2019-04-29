#pragma once
#include <Logic.h>

class CBattleUnit;
class CTurnOrderUI :public CLogic
{
public:
	enum TURNUIEVENT
	{
		EVENT_NO,
		EVENT_DEAD,
		// ���� �߰��Ǿ� ��ġ �������Ұ��(�������� ���� or ĳ���� ����)
		EVENT_RELOCATION,
		// ������ �־��� UI�� ��ġ ������ �� ���
		MAXTURNUIEVENT
	};

private:
	static SPTR<CGameMultiSprite>				m_ActionIconSprite;
	static bool									m_bSetSprite;


public:
	static void SetActionIcon(SPTR<CGameMultiSprite> _MultiSprite);

private:
	SPTR<CFixRenderer>					m_FrameRen;
	SPTR<CFixRenderer>					m_ActionIconBack;
	SPTR<CAniRenderer>					m_ActionIcon;

	SPTR<CFixRenderer>					m_ActionIconRen;
	CBattleUnit*						m_pPairUnit;
	
	// �� �������� ĳ���� or ���� ���� �Ǵ��ϴ� �Ұ�(true == ���� , false == ����)
	
	bool								m_bUnit;
	bool								m_bEntryUI;
	TURNUIEVENT							m_CurEvent;
	float								m_fAccTime;


	Vec3								m_EventStartPos;
	Vec3								m_EventEndPos;

	//bool								m_bReload
public:
	void Init();
	void Update();
	void FontRender();

public:
	void SetUnit(CBattleUnit* _pUnit);
	void Off();
	void On();
	void ActionIconOn(const int& _IconIndex);
	void ActionIconOff();
	CBattleUnit* GetBattleUnit();
	tstring GetUnitName();
	const TURNUIEVENT GetCurEvent();

	void SetPos(const Vec3& _Pos);
	void SetUISize(const Vec2& _Size);
	void SetUIAlpha(const float& _Alpha);
	void SetActionIcon(const int& _IconIndex);
	void EntryUI(const bool& _Entry);
	const bool IsEntryUI();
	void DeadUpdate();
	void RelocationUpdate();

	void StandBy();
public:
	bool		IsUnit();
	const bool	IsUnitDead();
	const bool	IsPopUI();

public:
	void MonsterDeadEvent();
	void RelocationEvent(const Vec3& _EndPos);
	
	const bool CompareDex(SPTR<CTurnOrderUI> _Other);

public:
	const bool operator>(SPTR<CTurnOrderUI> _Other);
	const bool operator<(SPTR<CTurnOrderUI> _Other);

public:
	CTurnOrderUI();
	~CTurnOrderUI();
};

