#pragma once
#include "MenuBase.h"
class CBattlePositionMenu : public CMenuBase
{
	enum TABINDEX
	{
		POSITION_TAB,
		CHARACTER_TAB,
		MAXTABINDEX
	};

	enum PAGEEVENT
	{
		PAGE_UP_EVENT,
		PAGE_DOWN_EVENT,
		MAXPAGEEVENT,
	};

	SPTR<CFixRenderer>						m_MainBackRen;
	SPTR<CFixRenderer>						m_SubBackRen;
	SPTR<CLineRenderer>						m_SplitterLineRen;
	
	vector<SPTR<class CMenuTab>>			m_PositionTab;
	int										m_PositionTabIndex;

	vector<Vec3>							m_vecUIPosition;
	vector<SPTR<class CBattlePositionUI>>	m_vecCurPositionUI;

	vector<CHARACTERDATA>					m_vecCharacterData;

	TABINDEX								m_CurTab;


	SPTR<CMenuTab>							m_NoPositionTab;
	// 모든 캐릭터 탭
	vector<SPTR<CMenuTab>>					m_CharacterTab;
	// 현재 포커싱된 포지션에 배치가 가능한 캐릭터 탭
	vector<SPTR<CMenuTab>>					m_ActiveCharTab;
	int 									m_CurCharacterPage;
	int										m_CurCharacterTabIndex;
	vector<Vec3>							m_vecCharTabPos;
	SPTR<CAniRenderer>						m_NoPositionGuide;
	Vec2									m_GuidePivot;

public:
	void Init();
	void Update();
	void MenuOnEvent();

public:
	void PositionSelectUpdate();
	void CharacterSelectUpdate();
	void AllActiveTabOff();
	void CharacterListUpdate();
	void ReturnPositionSelect();

public:
	// 현재 캐릭터 직군 1명만 배치가능
	void NormalModeOn();
	// 전체 캐릭터 직군 배치 가능
	void MasterModeOn();


public:
	void PrevMenuTab();
	void NextMenuTab();
	void PrevCharTab();
	void NextCharTab();
	const bool CheckChangePage(const PAGEEVENT& _Event);
	void ChangePage(const PAGEEVENT& _Event);
	void PopCurPositionCharacter();
	void ChangeCharacterPositionEvent();

public:
	CBattlePositionMenu();
	~CBattlePositionMenu();
};

