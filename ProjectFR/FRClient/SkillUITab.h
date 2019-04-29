#pragma once
#include <Logic.h>

class CTurnMgr;
class CSkillUIBoard;
class CSkillUITab : public CLogic
{
	friend class CTurnMgr;

	static D3DXCOLOR				NoneFocusTabColor;
	static D3DXCOLOR				FocusTabColor;
	static D3DXCOLOR				NoneFocusFrameColor;
	static D3DXCOLOR				FocusFrameColor;
	static DWORD					NoneFocusFontColor;
	static DWORD					FocusFontColor;
	static CSkillUIBoard*			pSkillUIBoard;
	static CTurnMgr*				pSkillTabTurnMgr;

	static void SetTurnMgr(CTurnMgr* _Mgr);

public:
	static void SetSkillUIBoard(CSkillUIBoard* _pBord);

private:
	size_t							m_SkillType;
	SPTR<CFixRenderer>				m_TabFrameRen;
	SPTR<CFixRenderer>				m_TabRen;
	SPTR<CFontRenderer>				m_TabFontRen;
	SPTR<CCollision>				m_TabCol;
	bool							m_bSelect;


public:
	void Init();
	void Update();
	void DebugRender();

public:
	void Enter(SPTR<CCollision> _pThis, SPTR<CCollision> _pOtherCol);
	void Stay(SPTR<CCollision> _pThis, SPTR<CCollision> _pOtherCol);
	void Exit(SPTR<CCollision> _pThis, SPTR<CCollision> _pOtherCol);

public:
	// 탭버튼이 클릭되었을때 현재 보드안에있는 스킬 UI를 Off하고 
	// 선택된 스킬 Type의 보유 스킬을 On하도록 Board에게 신호를 준다.
	void TabClickEvent();
	void SelectTab();
	void UnSelectTab();
	void SetSkillType(const size_t& _Type);
	void SetTabFontString();
	void SetPos(const Vec2& _Pos);
	void SetPos(const Vec3& _Pos);
	void SetPivot(const Vec2& _Pivot);
	const Vec2 GetSize();
	const Vec3 GetSize3();
	void UIOff();
	void UIOn();
	const bool IsSelect();

public:
	CSkillUITab();
	~CSkillUITab();
};

