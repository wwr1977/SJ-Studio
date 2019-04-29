#pragma once
#include<Logic.h>
class CStoryCurtain : public CLogic
{
public:
	enum STORYEVENT
	{
		STORY_START,
		STORY_END,
		MAXSTORY,
	}m_CurUpdate;

private:
	SPTR<CFixRenderer>					m_UpRen;
	SPTR<CFixRenderer>					m_DownRen;
	float								m_fAccTime;
	const float							m_fUpdateTime;
	bool								m_bCurtainUpdate;

	Vec2								m_UpStartPos;
	Vec2								m_UpEndPos;
	Vec2								m_DownStartPos;
	Vec2								m_DownEndPos;

public:
	void Init();
	void Update();

public:
	void StortStartUpdate();
	void StortEndUpdate();
	void StoryEvent(const bool& _Start);


public:
	void SetCurtain(const bool& _EndPos);
	void InvalidateUpdate();
public:
	const bool IsCurtainUpdate();
	const STORYEVENT GetCurEvent();

public:
	void RenderEnable(const bool& _Enable);
public:
	CStoryCurtain();
	~CStoryCurtain();
};

