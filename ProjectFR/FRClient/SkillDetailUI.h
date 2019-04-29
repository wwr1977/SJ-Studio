#pragma once
#include <Logic.h>
class CSkillDetailUI : public CLogic
{
	enum 
	{
		DETAIL_SKILLNAME,
		DETAIL_SKILLLEVEL,
		DETAIL_SKILLDEMAGE,
		DETAIL_SKILLTARGET,
		DETAIL_COST1,
		DETAIL_COST2,
		DETAIL_COST3,
		MAXDETAILSTRING,
	};

	static SPTR<CGameMultiSprite> SkillDetailMainIconSprite;
public:
	static void InitDetailSkillUI(SPTR<CGameMultiSprite> _IconSprite);

	SPTR<CFixRenderer>					m_UIBack;
	SPTR<CFixRenderer>					m_UIMainBack;
	SPTR<CFixRenderer>					m_SkillIconRen;
	SPTR<CFixRenderer>					m_SkillTypeRen;
	SKILLUIDATA							m_CurSkillData;
	
	vector<tstring>						m_DetailString;
	SPTR<class CStdGauge>				m_SkillExpGauge;

	SPTR<CScriptRenderer>				m_SkillDetailRen;
public:
	void Init();
	void Update();
	void UIFontRender();

public:
	void SetSkillDetail(const size_t& _SkillNo);
	void SetPos(const Vec3& _Pos);
public:
	void UIOn();
	void UIOff();

public:
	CSkillDetailUI();
	~CSkillDetailUI();
};

