#pragma once
#include <Logic.h>

class CTurnMgr;
class CSkillUIMgr;
class CBattleUnit;
class CSkillUI : public CLogic
{
	enum SKILLUISTATE
	{
		SKILL_AVAILABLE,
		SKILL_INABLE,
		INABLE_EVENT,		// CLICK_NO ���¿��� UI�� Ŭ�������� �¿�� �����̴� ����� �ϴµ����� ����
		MAXSKILLUISTATE,
	};

	friend class CTurnMgr;

	static bool m_bInit;
	static SPTR<CGameMultiSprite> SkillMainIconSprite;
	static D3DXCOLOR SelectBackColor;
	static D3DXCOLOR FocusBackColor;
	static D3DXCOLOR NoneFocusBackColor;
	static D3DXCOLOR CoverColor;

	static CTurnMgr* pSkillUITurnMgr; 
	static CSkillUI* pCurSelectUl;


	static void SetTurnMgr(CTurnMgr* _TurnMgr);



private:
	SPTR<CFixRenderer>			m_SkillUIBack;
	SPTR<CFixRenderer>			m_SkillUIFrame;
	SPTR<CFixRenderer>			m_SkillIcon;
	SPTR<CFixRenderer>			m_SubSkillIcon;
	SPTR<CFixRenderer>			m_SkillUICover;
	SPTR<CFontRenderer>			m_SkillNameRenderer;
	SPTR<CFontRenderer>			m_SkillLevelRen;
	SPTR<CCollision>			m_SkillUICol;

	SKILLUIDATA					m_SkillUIData;
	bool						m_bSelect;

	// FontRenderer���Դ� �̰��� +1 �� ���� �Ѱ��ش�
	// �� ���� ������( 0 ~ 4 ) , ���� �������� ���� ���� (1 ~ 5)
	size_t						m_CurSkillLevel;

	UINT						m_CurUIState;
	SPTR<CBattleUnit>			m_CurSkillCaster;
	float						m_fAccTime;

	Vec2						m_IconPivot;

public:
	void Update();
	void Enter(SPTR<CCollision> _pThis, SPTR<CCollision> _pOtherCol);
	void Stay(SPTR<CCollision> _pThis, SPTR<CCollision> _pOtherCol);
	void Exit(SPTR<CCollision> _pThis, SPTR<CCollision> _pOtherCol);

public:
	void InitData(const SKILLUIDATA& _Data);
	bool EqualSkillName(const tstring& _SkillName);
	bool EqualSkillKName(const tstring& _SkillKName);

public:
	void SkillUIOn();
	void SkillUIOff();
	void SetPos(const Vec2& _Pos);
	void SetPos(const Vec3& _Pos);
	void SetCurCaster(SPTR<CBattleUnit> _Unit);

public:
	void SkillUIClickEvent();
	void SetCurSkillLevel(const size_t& _SkillLevel);

	void SetSkillUIState(const SKILLUISTATE& _State);

	void CheckUIAvailable();
	void NotSelectEvent();
public:
	CSkillUI();
	~CSkillUI();
};

