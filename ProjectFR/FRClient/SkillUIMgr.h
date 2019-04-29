#pragma once
#include <ObjBase.h>

class CTurnMgr;
class CSkillUI;
class CSkillUIBoard;
class CSkillUITab;
class CSkillUIMgr : public CObjBase
{
	enum SKILLNAME
	{
		SKILL_NAME,
		SKILL_KNAME,
		MAXSKILLNAME
	};

	
private:
	CGameScene*								m_PairScene;
	CTurnMgr*								m_pTurnMgr;
	// ��ų UI 
	vector<SPTR<CSkillUI>>					m_vecSkillUI;
	// ��ų UI�� �ߴ� ���(SkillBoard)
	SPTR<CSkillUIBoard>						m_SkillUIBoard;



public:
	void SettingScene(CGameScene* _Scene);
	void SettingTurnMgr(CTurnMgr* _TurnMgr);

public:
	SPTR<CSkillUI> operator[](const size_t& _SkillNo);
	SPTR<CSkillUI> FindSkillUI(const size_t& _SkillNo);
	bool ExistSkillUI(const tstring& _SkillName,const SKILLNAME& _Mode);
	CSkillUI* CreateSkillUI(const size_t& _SkillNo);
	void LoadSkillUIData();

	SPTR<CSkillUIBoard> GetSkillBackBoard();
	
	void SkillBoardOn();
	void SkillBoardOff();
	
public:
	CSkillUIMgr();
	~CSkillUIMgr();
};

