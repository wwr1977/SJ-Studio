#include "Precom.h"
#include "SkillLevelUpUI.h"
#include "MenuTab.h"
#include "CharacterSkillMenu.h"
#include "GameMaster.h"
#include <GameMultiSprite.h>


CSkillLevelUpUI::CSkillLevelUpUI()
	:m_LevelUpQueryBackRen(nullptr), m_LevelUpQueryFrameRen(nullptr)
	,m_LevelUpUIBackRen(nullptr), m_LevelUpUIFrameRen(nullptr)
	, m_SkillIconRen(nullptr)
	, m_QueryIconPivot(Vec2{ 0.f,100.f }), m_NoticeIconPivot(Vec2{0.f,0.f})
	, m_CurSelectTab(0), m_bKeyCheck(true)
	, m_CurSkillNo(-1), m_fAccTime(0.f)
	, m_LevelUpEffectSound(nullptr)
	, m_LevelUpFontRen(nullptr)
	, m_pSkillMenu(nullptr)
{
	m_vecQueryRenderString.clear();
	m_NoticeRenderString.clear();
	m_vecQuerySelectTab.clear();
}


CSkillLevelUpUI::~CSkillLevelUpUI()
{
	m_vecQueryRenderString.clear();
	m_NoticeRenderString.clear();
	m_vecQuerySelectTab.clear();
}
void CSkillLevelUpUI::Init() 
{
	m_LevelUpQueryBackRen = ACTOR->CreateCom<CFixRenderer>(RENDATA(LAYER_UI, 0.5f, true));
	m_LevelUpQueryBackRen->SetSprite(_T("ColorBase"));
	m_LevelUpQueryBackRen->SetMaterial(D3DXCOLOR(0.f, 0.f, 0.f, 0.5f));
	m_LevelUpQueryBackRen->SetCustomSize(Vec2{ 354.f,432.f });
	m_LevelUpQueryBackRen->SetRenPivot(Vec2{ 0.f,-10.f });
	m_LevelUpQueryBackRen->MaterialOn();

	m_LevelUpQueryFrameRen = ACTOR->CreateCom<CFixRenderer>(RENDATA(LAYER_UI, 0.97f, true));
	m_LevelUpQueryFrameRen->SetSprite(_T("SkillLevelUpGuideFrame"));

	m_LevelUpUIBackRen = ACTOR->CreateCom<CFixRenderer>(RENDATA(LAYER_UI, 0.5f, true));
	m_LevelUpUIBackRen->SetSprite(_T("SkillLevelUpBack"));

	m_LevelUpUIFrameRen = ACTOR->CreateCom<CFixRenderer>(RENDATA(LAYER_UI, 0.9f, true));
	m_LevelUpUIFrameRen->SetSprite(_T("SkillLevelUpFrame"));

	m_SkillIconRen = ACTOR->CreateCom<CFixRenderer>(RENDATA(LAYER_UI, 0.95f, true));
	m_SkillIconRen->SetCustomSize(Vec2{ 80.f,80.f });

	m_LevelUpFontRen = ACTOR->CreateCom<CFontRenderer>(RENDATA(LAYER_UI, 0.95f, true));
	m_LevelUpFontRen->SetFontStyle(_T("Å«º¼µå±¼¸²"));
	m_LevelUpFontRen->SetString(_T("½ºÅ³ ½Â±Þ"));
	m_LevelUpFontRen->SetMiddleStartPivot(Vec3{ 0.f,170.0f,0.f });
	m_LevelUpFontRen->SetFontColor(D3DCOLOR_ARGB(255, 220, 220, 30));
	
	m_vecQueryRenderString.assign(MAXQUERYSTRING, _T(""));
	
	for (size_t i = 0; i < m_vecQueryRenderString.size(); i++)
		m_vecQueryRenderString.reserve(127);
			
	m_vecQuerySelectTab.push_back(ACTOR->CreateCom<CMenuTab>());
	m_vecQuerySelectTab.push_back(ACTOR->CreateCom<CMenuTab>());

	
	m_vecQuerySelectTab[LEVELUP]->SetTabStyle(_T("½ºÅ³½Â±ÞÅÇ"));
	m_vecQuerySelectTab[LEVELUP]->SetTabSize(Vec2{ 100.f,30.f });
	m_vecQuerySelectTab[LEVELUP]->SetTabName(_T("½Â±Þ"));
	m_vecQuerySelectTab[LEVELUP]->SetPivot(Vec3{-75.f,-200.f,0.f});
	m_vecQuerySelectTab[LEVELUP]->SetSelectColor(D3DXCOLOR(1.f,1.f,1.f,0.8f));
	m_vecQuerySelectTab[LEVELUP]->SelectTab(true);
	m_CurSelectTab = LEVELUP;

	m_vecQuerySelectTab[NOTLEVELUP]->SetTabStyle(_T("½ºÅ³½Â±ÞÅÇ"));
	m_vecQuerySelectTab[NOTLEVELUP]->SetTabSize(Vec2{ 100.f,30.f });
	m_vecQuerySelectTab[NOTLEVELUP]->SetTabName(_T("´ÙÀ½¿¡"));
	m_vecQuerySelectTab[NOTLEVELUP]->SetPivot(Vec3{ 75.f,-200.f,0.f });
	m_vecQuerySelectTab[NOTLEVELUP]->SetSelectColor(D3DXCOLOR(1.f, 1.f, 1.f, 0.7f));

	m_LevelUpEffectSound = ACTOR->CreateCom<CSoundPlayer>();
	

	ChangeUIMode(SKILL_LEVEL_UP_QUERY);
	//SetLevelUpSkill(0);

	UIOff();
}
void CSkillLevelUpUI::InitData(void* _SkillMenu)
{
	if (nullptr == _SkillMenu)
		return;

	m_pSkillMenu = (CCharacterSkillMenu*)_SkillMenu;
}
void CSkillLevelUpUI::Update() 
{
	if (false == m_bKeyCheck)
		return;

	switch (m_CurMode)
	{
	case CSkillLevelUpUI::SKILL_LEVEL_UP_QUERY:
		QueryUpdate();
		return;
	case CSkillLevelUpUI::SKILL_LEVEL_UP_NOTICE:
		NoticeUpdate();
		return;

	}
}
void CSkillLevelUpUI::UIFontRender()
{
	switch (m_CurMode)
	{
	case CSkillLevelUpUI::SKILL_LEVEL_UP_QUERY:
		LevelUpQueryRender();
		return ;
	case CSkillLevelUpUI::SKILL_LEVEL_UP_NOTICE:
		LevelUpNoticeRender();
		return;
	}
}
void CSkillLevelUpUI::PostUpdate()
{
	if (false == m_bKeyCheck) 
	{
		if (m_fAccTime >= 1.f)
			m_bKeyCheck = true;

		m_fAccTime += DELTATIME;
	}

}
void CSkillLevelUpUI::QueryUpdate() 
{
	if (m_CurSelectTab >= MAXSELECTTAB) 
	{
		assert(nullptr);
		return;
	}

	if (true == KEYDOWN("LEFT")) 
	{
		if (LEVELUP == m_CurSelectTab)
			return;
		else if(NOTLEVELUP == m_CurSelectTab)
		{
			if (nullptr != m_vecQuerySelectTab[m_CurSelectTab])
				m_vecQuerySelectTab[m_CurSelectTab--]->SelectTab(false);

			if (nullptr != m_vecQuerySelectTab[m_CurSelectTab])
				m_vecQuerySelectTab[m_CurSelectTab]->SelectTab(true);
		}
		
	}
	else if (true == KEYDOWN("RIGHT")) 
	{
		if (NOTLEVELUP == m_CurSelectTab)
			return;
		else if (LEVELUP == m_CurSelectTab)
		{
			if (nullptr != m_vecQuerySelectTab[m_CurSelectTab])
				m_vecQuerySelectTab[m_CurSelectTab++]->SelectTab(false);

			if (nullptr != m_vecQuerySelectTab[m_CurSelectTab])
				m_vecQuerySelectTab[m_CurSelectTab]->SelectTab(true);
		}
	}
	else if (true == KEYDOWN("Enter")) 
	{
		if (LEVELUP == m_CurSelectTab) 
		{
			LevelUpEvent();
			ChangeUIMode(SKILL_LEVEL_UP_NOTICE);
		}
		else 
		{
			UIOff();
			if (nullptr != m_pSkillMenu) 
			{
				m_pSkillMenu->CurSkillLevelUpEvent(false);
				m_pSkillMenu->HideSkillMenu(false);
			}
		}
	}
}
void CSkillLevelUpUI::NoticeUpdate() 
{
	if (true == KEYDOWN("Enter"))
	{
		UIOff();
		if (nullptr != m_pSkillMenu) 
		{
			m_pSkillMenu->CurSkillLevelUpEvent(true);
			m_pSkillMenu->HideSkillMenu(false);
		}
	}
}
void CSkillLevelUpUI::LevelUpQueryRender() 
{
	COMRESMGR->DrawFont(_T("Å«º¼µå±¼¸²"), m_vecQueryRenderString[SKILLNAME_STRING], Vec2{0.f,170.f}, 1.f, D3DCOLOR_ARGB(255, 255, 255, 255));
	COMRESMGR->DrawFont(_T("Å«UI·¹º§ÆùÆ®"), m_vecQueryRenderString[SKILLLEVEL_STRING], Vec2{ 0.f , 20.f}, 1.f, D3DCOLOR_ARGB(255, 255, 255, 255));
	COMRESMGR->DrawFont(_T("º¼µå±¼¸²"), m_vecQueryRenderString[SKILLDEMAGE_STRING], Vec2{ 0.f ,-30.f }, 1.f, D3DCOLOR_ARGB(255, 255, 255, 255));

	for (size_t i = SKILLCOST1_STRING; i <= SKILLCOST3_STRING; i++)
	{
		if (_T("") == m_vecQueryRenderString[i])
			break;

		COMRESMGR->DrawFont(_T("º¼µå±¼¸²"), m_vecQueryRenderString[i], Vec2{ 0.f , 20.f -25.f*i }, 1.f, D3DCOLOR_ARGB(255, 255, 255, 255));
	}
}
void CSkillLevelUpUI::LevelUpNoticeRender() 
{
	COMRESMGR->DrawFont(_T("Å«º¼µå±¼¸²"), m_vecQueryRenderString[SKILLNAME_STRING], Vec2{ 0.f,90.f }, 1.f, D3DCOLOR_ARGB(255, 255, 255, 255));
	if(_T("Level Max") == m_vecQueryRenderString[SKILLLEVEL_STRING])
		COMRESMGR->DrawFont(_T("Å«º¼µå±¼¸²"), m_vecQueryRenderString[SKILLLEVEL_STRING], Vec2{ 0.f , -120.f }, 1.f, D3DCOLOR_ARGB(255, 255, 30, 30));
	else
		COMRESMGR->DrawFont(_T("Å«º¼µå±¼¸²"), m_vecQueryRenderString[SKILLLEVEL_STRING], Vec2{ 0.f , -120.f }, 1.f, D3DCOLOR_ARGB(255, 255, 255, 255));
}
void CSkillLevelUpUI::ChangeUIMode(const RENDERINGMODE& _Mode)
{
	switch (_Mode)
	{
	case CSkillLevelUpUI::SKILL_LEVEL_UP_QUERY: 
	{
		m_CurMode = _Mode;
		m_LevelUpQueryBackRen->On();
		m_LevelUpQueryFrameRen->On();
		m_LevelUpUIBackRen->Off();
		m_LevelUpUIFrameRen->Off();
		m_SkillIconRen->SetRenPivot(m_QueryIconPivot);
		m_LevelUpFontRen->Off();

		for (size_t i = 0; i < m_vecQuerySelectTab.size(); i++)
		{
			if (nullptr != m_vecQuerySelectTab[i])
				m_vecQuerySelectTab[i]->MenuTabOn();
		}
		
		m_vecQuerySelectTab[LEVELUP]->SelectTab(true);
		m_CurSelectTab = 0;
		m_vecQuerySelectTab[NOTLEVELUP]->SelectTab(false);
		m_bKeyCheck = false;
		m_fAccTime = 0.9f;
	}
		break;
	case CSkillLevelUpUI::SKILL_LEVEL_UP_NOTICE: 
	{
		m_CurMode = _Mode;
		m_LevelUpQueryBackRen->Off();
		m_LevelUpQueryFrameRen->Off();
		m_LevelUpUIBackRen->On();
		m_LevelUpUIFrameRen->On();
		m_LevelUpFontRen->On();
		m_SkillIconRen->SetRenPivot(m_NoticeIconPivot);
		
		for (size_t i = 0; i < m_vecQuerySelectTab.size(); i++)
		{
			if (nullptr != m_vecQuerySelectTab[i])
				m_vecQuerySelectTab[i]->MenuTabOff();
		}

	}
		break;
	}
}
void CSkillLevelUpUI::SetLevelUpSkill(const size_t& _SkillNo)
{
	if (_SkillNo >= CClientGlobal::vecSkill.size())
		return;

	m_CurSkillNo = _SkillNo;
	size_t IconIndex = CClientGlobal::vecSkill[_SkillNo].SkillUIData.SkillIconIndex;
	size_t CurSkillLv = CClientGlobal::vecSkill[_SkillNo].CurSkillLevel;

	LEVELDATA CurLvData = CClientGlobal::vecSkillLevelData[_SkillNo][CurSkillLv];
	LEVELDATA NextLvData;

	if (CurSkillLv < 4) 
		NextLvData = CClientGlobal::vecSkillLevelData[_SkillNo][CurSkillLv + 1];
		
	if (nullptr != CClientGlobal::SkillIconMultiSprite )
		m_SkillIconRen->SetSprite(CClientGlobal::SkillIconMultiSprite->GetSprite(IconIndex));

	TCHAR Buf[MAXKEYLENGTH] = _T("");

	m_vecQueryRenderString[SKILLNAME_STRING] = CClientGlobal::vecSkill[_SkillNo].SkillUIData.SkillKName;

	if (3 <= CurSkillLv)
		m_vecQueryRenderString[SKILLLEVEL_STRING] = _T("Level Max");
	else 
	{
		_stprintf_s(Buf, sizeof(Buf) / sizeof(TCHAR) ,_T("Level %u"), CClientGlobal::vecSkill[_SkillNo].CurSkillLevel + 2 );
		m_vecQueryRenderString[SKILLLEVEL_STRING] = Buf;
	}
	
	int CurDem = 0, UpDem = 0;

	for (size_t i = 0; i < CurLvData.HitDataCount; i++)
	{
		if (0 != CurLvData.HitData[i].HitDemageMag)
			CurDem += (int)ceilf(CurLvData.HitData[i].HitDemageMag * 100.f);
	}

	for (size_t i = 0; i < NextLvData.HitDataCount; i++)
	{
		if (0 != NextLvData.HitData[i].HitDemageMag)
			UpDem += (int)ceilf(NextLvData.HitData[i].HitDemageMag * 100.f);
	}

	if (UpDem == CurDem) 
	{
		_stprintf_s(Buf, sizeof(Buf) / sizeof(TCHAR), _T("µ¥¹ÌÁö : %d%%(-)"), UpDem);
		m_vecQueryRenderString[SKILLDEMAGE_STRING] = Buf;
	}
	else 
	{
		_stprintf_s(Buf, sizeof(Buf) / sizeof(TCHAR), _T("µ¥¹ÌÁö : %d%%(+%d%%)"), UpDem, UpDem - CurDem);
		m_vecQueryRenderString[SKILLDEMAGE_STRING] = Buf;
	}


	UINT Start = SKILLCOST1_STRING;
	int CurCost = 0, NextCost = 0;

	NextCost = NextLvData.SkillCost[LEVELDATA::SKILLCOST_BURST];
	CurCost = CurLvData.SkillCost[LEVELDATA::SKILLCOST_BURST];

	if (0 != CurCost && 0 != NextCost)
	{
		if (CurCost == NextCost) 
		{
			_stprintf_s(Buf, sizeof(Buf) / sizeof(TCHAR), _T("Burst : %3d(-)"), NextCost);
			m_vecQueryRenderString[Start++] = Buf;		
		}
		else 
		{
			_stprintf_s(Buf, sizeof(Buf) / sizeof(TCHAR), _T("Burst : %3d(%d)"), NextCost, NextCost - CurCost);
			m_vecQueryRenderString[Start++] = Buf;
		}
	}
	
	NextCost = NextLvData.SkillCost[LEVELDATA::SKILLCOST_HP];
	CurCost = CurLvData.SkillCost[LEVELDATA::SKILLCOST_HP];

	if (0 != CurCost && 0 != NextCost)
	{
		if (CurCost == NextCost)
		{
			_stprintf_s(Buf, sizeof(Buf) / sizeof(TCHAR), _T("HP : %3d(-)"), NextCost);
			m_vecQueryRenderString[Start++] = Buf;
		}
		else
		{
			_stprintf_s(Buf, sizeof(Buf) / sizeof(TCHAR), _T("HP : %3d(%d)"), NextCost, NextCost - CurCost);
			m_vecQueryRenderString[Start++] = Buf;
		}
	}

	NextCost = NextLvData.SkillCost[LEVELDATA::SKILLCOST_MP];
	CurCost = CurLvData.SkillCost[LEVELDATA::SKILLCOST_MP];

	if (0 != CurCost && 0 != NextCost)
	{
		if (CurCost == NextCost)
		{
			_stprintf_s(Buf, sizeof(Buf) / sizeof(TCHAR), _T("MP : %3d(-)"), NextCost);
			m_vecQueryRenderString[Start++] = Buf;
		}
		else
		{
			_stprintf_s(Buf, sizeof(Buf) / sizeof(TCHAR), _T("MP : %3d(%d)"), NextCost, NextCost - CurCost);
			m_vecQueryRenderString[Start++] = Buf;
		}
	}
}
void CSkillLevelUpUI::LevelUpEvent()
{
	CClientGlobal::vecSkill[m_CurSkillNo].CurSkillLevel = CClientGlobal::vecSkill[m_CurSkillNo].CurSkillLevel + 1;
	CClientGlobal::vecSkill[m_CurSkillNo].CurSkillExp = 0;
	
	CClientGlobal::GameMaster->SceneEffect(SCENE_EFFECT_FADE_IN | FADE_COLOR_WHITE, 1.5f);
	CClientGlobal::GameMaster->SetFadeData(1.4f,3.f);

	m_bKeyCheck = false;
	m_fAccTime = 0.f;

	if (nullptr != m_LevelUpEffectSound)
		m_LevelUpEffectSound->PlaySound(_T("LevelUp"));
}
void CSkillLevelUpUI::UIOn()
{
	m_bKeyCheck = true;
	m_fAccTime = 0.f;
	ACTOR->On();
}
void CSkillLevelUpUI::UIOff()
{
	ACTOR->Off();
}