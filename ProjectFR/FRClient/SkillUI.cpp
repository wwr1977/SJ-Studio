#include "Precom.h"
#include "SkillUI.h"
#include <GameSprite.h>
#include <GameMultiSprite.h>
#include "TurnMgr.h"
#include "BattleUnit.h"

SPTR<CGameMultiSprite> CSkillUI::SkillMainIconSprite = nullptr;
D3DXCOLOR CSkillUI::SelectBackColor = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
D3DXCOLOR CSkillUI::FocusBackColor = D3DXCOLOR(1.0f,0.5f,0.0f,1.0f);
D3DXCOLOR CSkillUI::NoneFocusBackColor = D3DXCOLOR(0.9f, 0.9f, 0.9f, 1.0f);
D3DXCOLOR CSkillUI::CoverColor = D3DXCOLOR(0.05f, 0.05f, 0.05f, 1.0f);

bool CSkillUI::m_bInit = false;
CTurnMgr* CSkillUI::pSkillUITurnMgr = nullptr;
CSkillUI* CSkillUI::pCurSelectUl = nullptr;

CSkillUI::CSkillUI()
	:m_SkillUIData(SKILLUIDATA()), m_SkillUIFrame(nullptr)
	, m_SkillIcon(nullptr), m_SubSkillIcon(nullptr)
	, m_SkillNameRenderer(nullptr), m_SkillUIBack(nullptr)
	, m_SkillUICol(nullptr), m_bSelect(false)
	, m_CurSkillLevel(0), m_SkillLevelRen(nullptr)
	, m_CurUIState(0), m_SkillUICover(nullptr)
	, m_CurSkillCaster(nullptr)
	, m_fAccTime(0.0f), m_IconPivot(Vec2{0.0f,0.0f})
{
}


CSkillUI::~CSkillUI()
{
}
void CSkillUI::SetTurnMgr(CTurnMgr* _TurnMgr)
{
	if (nullptr == _TurnMgr)
		return;

	pSkillUITurnMgr = _TurnMgr;
}
void CSkillUI::InitData(const SKILLUIDATA& _Data)
{
	if (false == m_bInit)
	{
		m_bInit = true;
		SkillMainIconSprite = COMRESMGR->FindMultiSprite(_T("SkillMainIcon"));
	}

	m_SkillUIData = _Data;

	
	m_SkillUIBack = ACTOR->CreateCom<CFixRenderer>(RENDATA(LAYER_UI, 0.9f, true));
	m_SkillUIBack->SetSprite(_T("SkillUIBack"));
	m_SkillUIBack->MaterialOn();
	m_SkillUIBack->SetMaterial(NoneFocusBackColor);
	m_SkillUIBack->SetCustomSize(Vec2{ 202.0f,56.0f });

	m_SkillUIFrame = ACTOR->CreateCom<CFixRenderer>(RENDATA(LAYER_UI, 1.0f, true));
	m_SkillUIFrame->SetSprite(_T("SkillUIFrame"));
	m_SkillUIFrame->SetCustomSize(Vec2{ 195.0f,50.0f });

	m_SkillIcon = ACTOR->CreateCom<CFixRenderer>(RENDATA(LAYER_UI, 1.0f, true));
	

	SPTR<CGameSprite> IconSprite = SkillMainIconSprite->GetSprite(m_SkillUIData.SkillIconIndex);

	if(nullptr != IconSprite)
		m_SkillIcon->SetSprite(IconSprite);

	m_SkillIcon->SetCustomSize(Vec2{ 35.0f, 35.0f });
	m_IconPivot = Vec2{ -72.0f, 2.0f };
	m_SkillIcon->SetRenPivot(m_IconPivot);

	m_SkillNameRenderer = ACTOR->CreateCom<CFontRenderer>(RENDATA(LAYER_UI, 1.0f, true));
	m_SkillNameRenderer->SetFontStyle(_T("스킬타이틀"));
	m_SkillNameRenderer->SetFontMag(2.3f);
	m_SkillNameRenderer->SetString(m_SkillUIData.SkillKName);
	m_SkillNameRenderer->SetLeftStartPivot(POINT{ -48 , 2 });

	m_SkillLevelRen = ACTOR->CreateCom<CFontRenderer>(RENDATA(LAYER_UI, 1.0f, true));
	m_SkillLevelRen->SetFontStyle(_T("UI레벨폰트"));
	m_SkillLevelRen->SetFontMag(2.3f);
	m_SkillLevelRen->SetFontColor(D3DCOLOR_ARGB(255, 200, 200, 0));
	m_SkillLevelRen->SetLeftStartPivot(POINT{ 23 , -21 });


	m_SkillUICol = ACTOR->CreateCom<CCollision>(COLDATA(_T("UI")));
	m_SkillUICol->SetColSize(Vec2{ 199.0f, 50.0f });
	m_SkillUICol->SetEnter(this, &CSkillUI::Enter);
	m_SkillUICol->SetStay(this, &CSkillUI::Stay);
	m_SkillUICol->SetExit(this, &CSkillUI::Exit);

	m_SkillUICover = ACTOR->CreateCom<CFixRenderer>(RENDATA(LAYER_UI, 0.8f, true));
	m_SkillUICover->SetSprite(_T("SkillUIBack"));
	m_SkillUICover->MaterialOn();
	m_SkillUICover->SetMaterial(CoverColor);
	m_SkillUICover->SetCustomSize(Vec2{ 202.0f,56.0f });
	m_SkillUICover->Off();


	if (nullptr == pSkillUITurnMgr)
		return;

	if (m_SkillUIData.SkillNo >= CClientGlobal::vecSkill.size())
		return;

	UINT CharNo = CClientGlobal::vecSkill[m_SkillUIData.SkillNo].SkillActionScript.CharIndex;

	m_CurSkillCaster = pSkillUITurnMgr->GetCharacter(CharNo);
}
bool CSkillUI::EqualSkillName(const tstring& _SkillName)
{
	if (0 == m_SkillUIData.SkillName.compare(_SkillName))
		return true;
	
	return false;
}
bool CSkillUI::EqualSkillKName(const tstring& _SkillKName) 
{
	if (0 == m_SkillUIData.SkillKName.compare(_SkillKName))
		return true;
	
	return false;
}
void CSkillUI::SkillUIOn()
{
	ACTOR->On();
}
void CSkillUI::SkillUIOff()
{
	ACTOR->Off();
	m_bSelect = false;
	m_SkillUIBack->SetMaterial(NoneFocusBackColor);
}
void CSkillUI::SetPos(const Vec2& _Pos)
{
	TRANS->SetPos(_Pos);
}
void CSkillUI::SetPos(const Vec3& _Pos)
{
	TRANS->SetPos(_Pos);
}
void CSkillUI::SetCurCaster(SPTR<CBattleUnit> _Unit)
{
	if (nullptr == _Unit)
		return;

	m_CurSkillCaster = _Unit;
}
void CSkillUI::Update()
{
	if (INABLE_EVENT != m_CurUIState)
		return;


	if (m_fAccTime >= 0.7f) 
	{
		Vec2 Pivot = Vec2{ 0.0f,0.0f };
	
		m_SkillUIBack->SetRenPivot(Pivot);
		m_SkillUIFrame->SetRenPivot(Pivot);
		m_SkillIcon->SetRenPivot(Pivot + m_IconPivot);
		m_SkillUICover->SetRenPivot(Pivot);
		m_SkillNameRenderer->SetRenPivot(Pivot);
		m_SkillLevelRen->SetRenPivot(Pivot);
		//m_SubSkillIcon->SetRenPivot(Pivot);

		m_fAccTime = 0.0f;
		m_CurUIState = SKILL_INABLE;
		return;
	}

	NotSelectEvent();


}
void CSkillUI::Enter(SPTR<CCollision> _pThis, SPTR<CCollision> _pOtherCol) 
{
	if (SKILL_AVAILABLE != m_CurUIState)
		return;

	if (false == m_bSelect)
		m_SkillUIBack->SetMaterial(FocusBackColor);
}
void CSkillUI::Stay(SPTR<CCollision> _pThis, SPTR<CCollision> _pOtherCol) 
{
	if (INABLE_EVENT == m_CurUIState)
		return;

	if (true == KEYDOWN("LMouse")) 
	{
		if (SKILL_AVAILABLE == m_CurUIState)
		{
			SkillUIClickEvent();

			if (nullptr != pSkillUITurnMgr)
				pSkillUITurnMgr->SelectSkillUI(m_SkillUIData.SkillNo);
		}
		else
			SetSkillUIState(INABLE_EVENT);
	}
}
void CSkillUI::Exit(SPTR<CCollision> _pThis, SPTR<CCollision> _pOtherCol) 
{
	if (SKILL_AVAILABLE != m_CurUIState)
		return;

	if(false == m_bSelect)
		m_SkillUIBack->SetMaterial(NoneFocusBackColor);
}
void CSkillUI::SkillUIClickEvent()
{
	// 이미 눌렀던 버튼일 경우에는 클릭이벤트를 무시한다.
	if (true == m_bSelect)
		return;

	if (nullptr != pCurSelectUl)
	{
		pCurSelectUl->m_SkillUIBack->SetMaterial(NoneFocusBackColor);
		pCurSelectUl->m_bSelect = false;
	}

	pCurSelectUl = this;

	m_bSelect = true;
	m_SkillUIBack->SetMaterial(SelectBackColor);
}
void CSkillUI::SetCurSkillLevel(const size_t& _SkillLevel)
{
	m_CurSkillLevel = _SkillLevel;

	TCHAR Level[10] = _T("");
	
	if (_SkillLevel < SKILLDATA::MAXSKILLLEVEL)
	{
		_stprintf_s(Level, _T("Lv %2d"), (int)m_CurSkillLevel + 1);
		
		if(nullptr != m_SkillLevelRen)
			m_SkillLevelRen->SetString(Level);
	}
}
void CSkillUI::SetSkillUIState(const SKILLUISTATE& _State)
{
	if (_State >= MAXSKILLUISTATE)
		return;

	m_CurUIState = _State;

	switch (m_CurUIState)
	{
	case CSkillUI::SKILL_AVAILABLE:
		m_SkillUICover->Off();
		break;
	case CSkillUI::SKILL_INABLE:
		m_SkillUICover->On();
		break;
	case CSkillUI::INABLE_EVENT:
		break;
	}
	return;

}
void CSkillUI::CheckUIAvailable()
{
	if (nullptr == m_CurSkillCaster || m_CurSkillLevel >= LEVELDATA::MAXSKILLLEVEL)
		return;

	if (true == CClientGlobal::MasterMode)
	{
		SetSkillUIState(SKILL_AVAILABLE);
		return;
	}

	if (true == m_CurSkillCaster->CheckSkillAvailable(CClientGlobal::vecSkillLevelData[m_SkillUIData.SkillNo][m_CurSkillLevel]))
		SetSkillUIState(SKILL_AVAILABLE);
	else
		SetSkillUIState(SKILL_INABLE);

}
void CSkillUI::NotSelectEvent()
{
	int Prev = (int)(m_fAccTime / 0.05f);
	m_fAccTime += DELTATIME;
	int Cur = (int)(m_fAccTime / 0.05f);

	if (Prev == Cur)
		return;


	Vec2 Pivot = Vec2{ CSJRandom::RandomFloat(-5.0f,5.0f),0.0f };
	m_SkillUIBack->SetRenPivot(Pivot);
	m_SkillUIFrame->SetRenPivot(Pivot);
	m_SkillIcon->SetRenPivot(Pivot + m_IconPivot);
	m_SkillUICover->SetRenPivot(Pivot);
	m_SkillNameRenderer->SetRenPivot(Pivot);
	m_SkillLevelRen->SetRenPivot(Pivot);
	//m_SubSkillIcon->SetRenPivot(Pivot);

}