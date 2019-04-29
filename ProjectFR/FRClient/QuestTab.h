#pragma once
#include "MenuTab.h"
class CQuestTab : public CMenuTab
{
private:
	SPTR<CFixRenderer>				m_QuestTypeRen;
	size_t							m_QuestNo;
	tstring							m_QuestKey;
	QUESTTYPE						m_QuestType;

public:
	void Init();

public:
	void SetQuestNo(const size_t& _QuestNo);


public:
	const tstring GetQuestKey();
	const QUESTTYPE GetQuestType();
public:
	CQuestTab();
	~CQuestTab();
};

