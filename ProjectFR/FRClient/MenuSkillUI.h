#pragma once
#include <Logic.h>

class CMenuSkillUI : public CLogic
{
	friend class CCharacterSkillMenu;

	static SPTR<CGameMultiSprite> MenuSkillMainIconSprite;
	static D3DXCOLOR SelectColor;
	static D3DXCOLOR NoneFocusColor;
	static D3DXCOLOR UICoverColor;

public:
	static void InitMenuSkillUI(SPTR<CGameMultiSprite> _IconSprite);
	static bool CompareMenuSkillUI(SPTR<class CMenuSkillUI> _Left, SPTR<CMenuSkillUI> _Right);

private:
	SPTR<CFixRenderer>			m_SkillUIBack;
	SPTR<CFixRenderer>			m_SkillUIFrame;
	SPTR<CFixRenderer>			m_SkillIcon;
	SPTR<CFixRenderer>			m_SubSkillIcon;
	SPTR<CFixRenderer>			m_SkillUICover;
	SPTR<CFontRenderer>			m_SkillNameRenderer;
	SPTR<CFontRenderer>			m_SkillLevelRen;
	Vec2						m_IconPivot;

	CharacterIndex				m_CasterNo;
	SKILLUIDATA					m_SkillUIData;
	bool						m_bSelect;

	SKILLDATA::SKILLTYPE		m_SkillType;

public:
	void Init();
	void InitData(const SKILLUIDATA& _Data);
	void Update();


public:
	void SetPos(const Vec3& _Pos);
	void SetLevel(const size_t& _Lv);
	const CharacterIndex GetCasterNo();
	const SKILLDATA::SKILLTYPE GetSkillType();
	const UINT GetSkillNo();

public:
	void SkillUIOn();
	void SkillUIOn(const Vec3& _Pos);
	void SkillUIOff();
	void SelectUI(const bool& _Select);


public:
	CMenuSkillUI();
	~CMenuSkillUI();
};

