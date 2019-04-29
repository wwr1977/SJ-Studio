#include "Precom.h"
#include "StoreSubUI.h"
#include "StoreUI.h"
#include "MenuTab.h"
#include "InventoryMgr.h"

CStoreSubUI::CStoreSubUI()
	:m_UIBackRen(nullptr), m_pStoreUI(nullptr)
	, m_CurActiveIndex(0)
{
	m_vecSubUITab.clear();
	m_ActiveTab.clear();
}


CStoreSubUI::~CStoreSubUI()
{
	m_vecSubUITab.clear();
	m_ActiveTab.clear();
}
void CStoreSubUI::Init() 
{
	m_UIBackRen = ACTOR->CreateCom<CFixRenderer>(RENDATA(LAYER_UI, 1.f, true));
	m_UIBackRen->SetSprite(_T("ColorBase"));
	m_UIBackRen->SetCustomSize(Vec2{100.f,70.f});
	m_UIBackRen->SetMaterial(D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.f));
	m_UIBackRen->MaterialOn();
	

	for (size_t j = 0; j < MAXSUBSTORETAB; ++j)
	{
		m_vecSubUITab.push_back(ACTOR->CreateCom<CMenuTab>());
		m_vecSubUITab[j]->SetTabStyle(_T("큰UI레벨폰트"));
		m_vecSubUITab[j]->SetTabSize(Vec2{ 80.f,25.f });
		m_vecSubUITab[j]->SelectTab(false);
		m_vecSubUITab[j]->MenuTabOff();
	}

	m_vecSubUITab[0]->SetTabName(_T("구매하기"));
	m_vecSubUITab[1]->SetTabName(_T("판매하기"));
	m_vecSubUITab[2]->SetTabName(_T("취소"));

	ACTOR->Off();
	this->Off();
}
void CStoreSubUI::Update() 
{
	if (TRUE == KEYDOWN("DOWN")) 
	{
		NextTab();
		return;
	}
	if (TRUE == KEYDOWN("UP"))
	{
		PrevTab();
		return;
	}

	if (TRUE == KEYDOWN("Enter"))
	{
		SelectTabEvent();
		return;
	}

	if (TRUE == KEYDOWN("MenuKey"))
	{
		ACTOR->Off();
		m_pStoreUI->SetKeyCheck(true);
		return;
	}

}

bool CStoreSubUI::PrevTab()
{
	if (0 >= m_CurActiveIndex)
		return false;

	if (nullptr != m_ActiveTab[m_CurActiveIndex])
		m_ActiveTab[m_CurActiveIndex--]->SelectTab(false);

	if (nullptr != m_ActiveTab[m_CurActiveIndex])
		m_ActiveTab[m_CurActiveIndex]->SelectTab(true);

	return true;
}
bool CStoreSubUI::NextTab() 
{
	if (m_ActiveTab.size() <= m_CurActiveIndex + 1)
		return false;

	if (nullptr != m_ActiveTab[m_CurActiveIndex])
		m_ActiveTab[m_CurActiveIndex++]->SelectTab(false);

	if (nullptr != m_ActiveTab[m_CurActiveIndex])
		m_ActiveTab[m_CurActiveIndex]->SelectTab(true);

	return true;
}
void CStoreSubUI::SetStoreUI(CStoreUI* _StoreUI)
{
	if (nullptr == _StoreUI)
		return;

	m_pStoreUI = _StoreUI;
}
void CStoreSubUI::AllTabActive(const bool& _Active)
{
	for (size_t i = 0; i < m_vecSubUITab.size(); i++)
	{
		if (_Active)
			m_vecSubUITab[_Active]->MenuTabOn();
		else 
			m_vecSubUITab[_Active]->MenuTabOff();
	}
}
void CStoreSubUI::AllTabSelectSet(const bool& _Select)
{
	for (size_t i = 0; i < m_vecSubUITab.size(); i++)
	{
		if (nullptr != m_vecSubUITab[i])
			m_vecSubUITab[i]->SelectTab(_Select);
	}

}

void CStoreSubUI::StandBy(const bool& _Purchase)
{
	m_CurActiveIndex = 0;

	m_ActiveTab.clear();
	AllTabActive(false);
	AllTabSelectSet(false);

	if (_Purchase) 
	{
		m_ActiveTab.push_back(m_vecSubUITab[PURCHASE_TAB]);
		m_ActiveTab.push_back(m_vecSubUITab[CANCEL_TAB]);
	}
	else 
	{
		m_ActiveTab.push_back(m_vecSubUITab[SALE_TAB]);
		m_ActiveTab.push_back(m_vecSubUITab[CANCEL_TAB]);
	}

	for (size_t i = 0; i < m_ActiveTab.size(); i++) 
	{
		m_ActiveTab[i]->SetPivot(Vec3{ 0.f,7.5f + -30.f*(float)i,0.f });
		m_ActiveTab[i]->MenuTabOn();
	}
	

	m_ActiveTab[m_CurActiveIndex]->SelectTab(true);
	
	ACTOR->On();
}
void CStoreSubUI::SelectTabEvent()
{
	if (m_CurActiveIndex >= m_ActiveTab.size())
		return;

	if (m_vecSubUITab[PURCHASE_TAB] == m_ActiveTab[m_CurActiveIndex]) 
		m_pStoreUI->PurchaseItemEvent();
	else if (m_vecSubUITab[SALE_TAB] == m_ActiveTab[m_CurActiveIndex])
		m_pStoreUI->SaleItemEvent();
	else if (m_vecSubUITab[CANCEL_TAB] == m_ActiveTab[m_CurActiveIndex])
		m_pStoreUI->SetKeyCheck(true);
	
	ACTOR->Off();
}