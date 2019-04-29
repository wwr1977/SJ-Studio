#pragma once
#include <Logic.h>
class CQuestClearObject : public CLogic
{
private:
	class CEventMgr*				m_pEventMgr;
	SPTR<CFixRenderer>				m_BackRen;
	SPTR<CFixRenderer>				m_BackFrameRen;
	SPTR<CFontRenderer>				m_QuestTitleRen;
	SPTR<CSoundPlayer>				m_EffectSoundPlayer;
	tstring							m_ClearQuestTitle;
	float							m_fAccTime;


	bool							m_bLastQuestClear;

public:
	void SetEventMgr(CEventMgr* _Mgr);

public:
	void Init();
	void Update();

public:
	void StandBy(const tstring& _QuestTitle);

	const bool IsLastQuest() { return m_bLastQuestClear; }
public:
	CQuestClearObject();
	~CQuestClearObject();
};

