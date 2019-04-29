#pragma once
#include <Logic.h>
class CQuestRewardGuide : public CLogic
{
	static Vec2							RewardIconSize;

	enum GUIDEMODE
	{
		COLLEAGUEMODE,
		ITEMMODE,
		MAXGUIDEMODE
	};

private:
	SPTR<CFixRenderer>					m_GuideBack;
	SPTR<CFixRenderer>					m_IconRen;
	SPTR<CFontRenderer>					m_ResultFont;
	float								m_fAccTime;


public:
	void Init();
	void Update();

public:
	void StandBy();
	void SetGuideMode(const CharacterIndex& _CharNo);
	void SetGuideMode(const CHARACTERLINE& _LineNo);
	void SetAlpha(const float& _Alpha);

public:
	CQuestRewardGuide();
	~CQuestRewardGuide();
};

