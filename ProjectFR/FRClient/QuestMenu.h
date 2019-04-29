#pragma once
#include "MenuBase.h"
class CQuestMenu : public CMenuBase
{
	enum 
	{
		MAXQUESTTAB = 16,
	};

	enum 
	{
		QUESTNAME,
		QUESTLIST,
		QUESTDETAIL,
		QUESTREWARD,
		MAXQUESTMENUTEXT,
	};

	enum 
	{
		MAXMISSION = 10,
	};
private:
	SPTR<CFixRenderer>					m_QuestTitleBackRen;
	SPTR<CFixRenderer>					m_QuestBackRen;

	vector<SPTR<class CQuestTab>>		m_vecQuestTab;
	vector<SPTR<CQuestTab>>				m_vecActiveQuestTab;
	vector<Vec3>						m_vecQuestTabPos;
	size_t								m_CurQuestIndex;


	vector<SPTR<CFontRenderer>>			m_vecMenuFont;
	SPTR<CScriptRenderer>				m_QuestContentsFont;
	SPTR<CScriptRenderer>				m_QuestRewardFont;
	vector<SPTR<CScriptRenderer>>		m_vecQuestMission;


public:
	void Init();
	void Update();
	void MenuOnEvent();
	

public:
	const bool CheckPrevQuest();
	const bool CheckNextQuest();
	void ChangeQuestPage();

public:
	void PushActiveQuest();
	void ClearActiveQuestTab();
public:
	void AllQuestTabOff();

public:
	CQuestMenu();
	~CQuestMenu();
};

