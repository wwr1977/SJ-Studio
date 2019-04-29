#pragma once
#include <Logic.h>

class CSkillLevelUpUI : public CLogic
{
public:
	enum RENDERINGMODE
	{
		SKILL_LEVEL_UP_QUERY,			// 스킬 승급 여부를 묻는 UI상태
		SKILL_LEVEL_UP_NOTICE,			// 스킬 승급이 완료되었음을 알려주는 상태
	}m_CurMode;

	enum SELECTTAB
	{
		LEVELUP,
		NOTLEVELUP,
		MAXSELECTTAB,
	};

	enum QUERY_STRING
	{
		SKILLNAME_STRING,
		SKILLLEVEL_STRING,
		SKILLDEMAGE_STRING,
		SKILLCOST1_STRING,
		SKILLCOST2_STRING,
		SKILLCOST3_STRING,
		MAXQUERYSTRING,
	};


private:
	class CCharacterSkillMenu*				m_pSkillMenu;
	SPTR<CFixRenderer>					m_LevelUpQueryBackRen;
	SPTR<CFixRenderer>					m_LevelUpQueryFrameRen;
	Vec2								m_QueryIconPivot;

	vector<tstring>						m_vecQueryRenderString;
	vector<SPTR<class CMenuTab>>		m_vecQuerySelectTab;
	int									m_CurSelectTab;

	SPTR<CFixRenderer>					m_LevelUpUIBackRen;
	SPTR<CFixRenderer>					m_LevelUpUIFrameRen;
	SPTR<CFontRenderer>					m_LevelUpFontRen;
	SPTR<CFixRenderer>					m_SkillIcon;
	Vec2								m_NoticeIconPivot;
	vector<tstring>						m_NoticeRenderString;


	
	SPTR<CFixRenderer>					m_SkillIconRen;
	SPTR<CSoundPlayer>					m_LevelUpEffectSound;
	bool								m_bKeyCheck;
	size_t								m_CurSkillNo;
	float								m_fAccTime;
public:
	void Init();
	void InitData(void* _SkillMenu);
	void Update();
	void PostUpdate();
	void UIFontRender();

public:
	void QueryUpdate();
	void NoticeUpdate();
	void LevelUpQueryRender();
	void LevelUpNoticeRender();

public:
	void ChangeUIMode(const RENDERINGMODE& _Mode);
	void SetLevelUpSkill(const size_t& _SkillNo);
	void LevelUpEvent();

public:
	void UIOn();
	void UIOff();

public:
	CSkillLevelUpUI();
	~CSkillLevelUpUI();
};

