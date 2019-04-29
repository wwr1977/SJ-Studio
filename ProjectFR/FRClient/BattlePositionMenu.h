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
	// ��� ĳ���� ��
	vector<SPTR<CMenuTab>>					m_CharacterTab;
	// ���� ��Ŀ�̵� �����ǿ� ��ġ�� ������ ĳ���� ��
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
	// ���� ĳ���� ���� 1�� ��ġ����
	void NormalModeOn();
	// ��ü ĳ���� ���� ��ġ ����
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

