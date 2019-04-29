#include "Precom.h"
#include "StoreUI.h"
#include "StoreSubUI.h"
#include "MenuTab.h"
#include "ItemTab.h"
#include "GameMaster.h"
#include "InventoryMgr.h"



CStoreUI::CStoreUI()
	:m_GoldFrameRen(nullptr), m_StoreTabFrameRen(nullptr)
	, m_ItemListFrameRen(nullptr), m_ItemDetailFrameRen(nullptr)
	, m_CurPage(0), m_CurItemIndex(0), m_CurStoreIndex(0)
	, m_CurSelectTab(SELECT_STORE_TAB), m_CurStoreType(WEAPON_STORE)
	, m_InventoryGold(0), m_InvenGoldRen(nullptr)
	, m_ItemDetailFontRen(nullptr), m_StoreSubUI(nullptr)
	, m_bKeyCheck(true)
{
}


CStoreUI::~CStoreUI()
{
	m_StoreTabPivot.clear();
	m_StoreTabString.clear();
	m_ItemTabPivot.clear();

	m_vecStoreMenuTab.clear();
	m_ActiveStoreTab.clear();

	m_vecItemTab.clear();
	m_ActiveItemTab.clear();
	m_StoreSubUI = nullptr;
}
void CStoreUI::Init() 
{
	Vec2 WndSize = Vec2{ (float)CClientGlobal::MainWindow->WndSize().x, (float)CClientGlobal::MainWindow->WndSize().y };
	float X = WndSize.x;
	float Y = WndSize.y;

	m_GoldFrameRen = ACTOR->CreateCom<CFixRenderer>(RENDATA(LAYER_UIBACK, 0.75f, true));
	m_GoldFrameRen->SetSprite(_T("ColorBase"));
	m_GoldFrameRen->SetCustomSize(Vec2{ 450.f,64.f });
	m_GoldFrameRen->SetRenPivot(Vec2{ X*-0.2f, Y*-0.41f});
	m_GoldFrameRen->SetMaterial(D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.f));
	m_GoldFrameRen->MaterialOn();

	m_InvenGoldRen = ACTOR->CreateCom<CFontRenderer>(RENDATA(LAYER_UI, 1.f, true));
	m_InvenGoldRen->SetFontStyle(_T("큰볼드굴림"));
	m_InvenGoldRen->SetRightStartPivot(POINT{ (LONG)(X*-0.05f), (LONG)(Y*-0.425f) });


	m_StoreTabFrameRen = ACTOR->CreateCom<CFixRenderer>(RENDATA(LAYER_UIBACK, 0.75f, true));
	m_StoreTabFrameRen->SetSprite(_T("ColorBase"));
	m_StoreTabFrameRen->SetCustomSize(Vec2{ 470.f,64.f });
	m_StoreTabFrameRen->SetRenPivot(Vec2{ X*0.24f, Y*0.4f });
	m_StoreTabFrameRen->SetMaterial(D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.f));
	m_StoreTabFrameRen->MaterialOn();

	m_ItemListFrameRen = ACTOR->CreateCom<CFixRenderer>(RENDATA(LAYER_UIBACK, 0.75f, true));
	m_ItemListFrameRen->SetSprite(_T("ColorBase"));
	m_ItemListFrameRen->SetCustomSize(Vec2{ 470.f,400.f });
	m_ItemListFrameRen->SetRenPivot(Vec2{ X*0.24f, Y*0.083f });
	m_ItemListFrameRen->SetMaterial(D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.f));
	m_ItemListFrameRen->MaterialOn();

	
	m_ItemDetailFrameRen = ACTOR->CreateCom<CFixRenderer>(RENDATA(LAYER_UIBACK, 0.75f, true));
	m_ItemDetailFrameRen->SetSprite(_T("ColorBase"));
	m_ItemDetailFrameRen->SetCustomSize(Vec2{ 470.f,200.f });
	m_ItemDetailFrameRen->SetRenPivot(Vec2{ X*0.24f, Y*-0.32f });
	m_ItemDetailFrameRen->SetMaterial(D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.f));
	m_ItemDetailFrameRen->MaterialOn();

	m_ItemDetailFontRen = ACTOR->CreateCom<CScriptRenderer>(RENDATA(LAYER_UI, 1.f, true));
	m_ItemDetailFontRen->SetFontStyle(_T("큰UI레벨폰트"));
	m_ItemDetailFontRen->SetLineInterval(30);
	m_ItemDetailFontRen->SetLeftStartPivot(POINT{ (LONG)(X*0.09f), (LONG)(Y*-0.25f) });

	m_StoreTabString.push_back(_T("무기"));
	m_StoreTabString.push_back(_T("방어구"));
	m_StoreTabString.push_back(_T("장신구"));
	m_StoreTabString.push_back(_T("기타"));
	m_StoreTabString.push_back(_T("판매"));


	for (size_t i = 0; i < MAXSTORETAB; i++)
		m_StoreTabPivot.push_back(Vec3{ X*(0.11f +0.065f*(float)i ), Y*0.39f ,0.f });

	for (size_t i = 0; i < MAXITEMTAB; i++)
		m_ItemTabPivot.push_back(Vec3{ X*0.24f, Y*(0.31f - 0.05f*(float)i) ,7.f });
	

	for (size_t j = 0; j < MAXSTORETAB; ++j)
	{
		m_vecStoreMenuTab.push_back(ACTOR->CreateCom<CMenuTab>());
		m_vecStoreMenuTab[j]->SetTabStyle(_T("큰UI레벨폰트"));
		m_vecStoreMenuTab[j]->SetTabSize(Vec2{ 80.f,25.f });
		m_vecStoreMenuTab[j]->SetTabName(m_StoreTabString[j]);
		m_vecStoreMenuTab[j]->SetPivot(m_StoreTabPivot[j]);
		m_vecStoreMenuTab[j]->SelectTab(false);
	}

	for (size_t i = 0; i < CClientGlobal::vecItem.size(); i++)
	{
		m_vecItemTab.push_back(CActObject::CreateActObject(_T("ItemTab"), ACTOR->GetScene())->CreateCom<CItemTab>(CItemTab::PURCHASE_MODE));
		m_vecItemTab[i]->SetItemTab(i);
		m_vecItemTab[i]->ActorOff();
	}

	m_StoreSubUI = CActObject::CreateActObject(_T("ItemTab"), ACTOR->GetScene())->CreateCom<CStoreSubUI>();
	m_StoreSubUI->SetStoreUI(this);

	StandBy(WEAPON_STORE);
}
void CStoreUI::Update()
{
	if (m_bKeyCheck) 
		StoreUpdate();
	else 
		m_StoreSubUI->Update();
	
	//if (!m_bKeyCheck)
	//	return;

	//if (m_bPurchaseEvent)
	//{
	//	m_bPurchaseEvent = false;
	//	return;
	//}

	//if (TRUE == KEYDOWN("LEFT"))
	//{
	//	if (PrevStoreTab())
	//		return ChangeItemListEvent();
	//}

	//if (TRUE == KEYDOWN("RIGHT"))
	//{
	//	if (NextStoreTab())
	//		return ChangeItemListEvent();
	//}

	//if (TRUE == KEYDOWN("UP"))
	//{
	//	if (PrevItemTab())
	//		return UpdateItemDetail();
	//}

	//if (TRUE == KEYDOWN("DOWN"))
	//{
	//	if (NextItemTab())
	//		return UpdateItemDetail();
	//}

	/*switch (m_CurSelectTab)
	{
	case CStoreUI::SELECT_STORE_TAB:
		StoreTabUpdate();
		break;
	case CStoreUI::SELECT_ITEM_TAB:
		ItemTabUpdate();
		break;
	}
	*/
}

void CStoreUI::StoreUpdate()
{
	if (TRUE == KEYDOWN("LEFT"))
	{
		if (PrevStoreTab())
			return ChangeItemListEvent();
	}

	if (TRUE == KEYDOWN("RIGHT"))
	{
		if (NextStoreTab())
			return ChangeItemListEvent();
	}

	if (TRUE == KEYDOWN("UP"))
	{
		if (PrevItemTab())
			return UpdateItemDetail();
	}

	if (TRUE == KEYDOWN("DOWN"))
	{
		if (NextItemTab())
			return UpdateItemDetail();
	}

	if (TRUE == KEYDOWN("Enter"))
	{
		//구매확인창을 띄운다.
		SelectItemTabEvent();
		return;
	}

	if (TRUE == KEYDOWN("MenuKey"))
	{
		CClientGlobal::GameMaster->StoreCloseEvent();
		return;
	}

}
void CStoreUI::UpdateInvenGold()
{
	m_InventoryGold = CClientGlobal::InventoryMgr->GetGold();
	m_InvenGoldRen->SetString(to_wstring(m_InventoryGold) + _T(" G"));

}
void CStoreUI::UpdateItemDetail()
{
	m_ItemDetailFontRen->SetScriptString(_T(""));
	
	size_t ItemNo = m_ActiveItemTab[m_CurPage][m_CurItemIndex]->GetItemNo();
	
	if (ItemNo >= CClientGlobal::vecItem.size())
		return;
	
	m_ItemDetailFontRen->SetScriptString(CClientGlobal::vecItem[ItemNo].ItemDescription);
}
void CStoreUI::StoreTabUpdate() 
{
	if (TRUE == KEYDOWN("LEFT")) 
	{
		if (PrevStoreTab())
			ChangeItemListEvent();
		return;
	}

	if (TRUE == KEYDOWN("RIGHT")) 
	{
		if (NextStoreTab())
			ChangeItemListEvent();
		return;
	}


	if (TRUE == KEYDOWN("DOWN") || TRUE == KEYDOWN("Enter"))
	{
		if (!m_ActiveItemTab.empty()) 
		{
			m_CurSelectTab = SELECT_ITEM_TAB;
			m_ActiveItemTab[m_CurPage][m_CurItemIndex]->SelectTab(true);
			m_ItemDetailFontRen->On();
			UpdateItemDetail();
		}
		return;
	}
}
void CStoreUI::ItemTabUpdate() 
{
	if (TRUE == KEYDOWN("UP"))
	{
		if(PrevItemTab())
			UpdateItemDetail();
		return;
	}

	if (TRUE == KEYDOWN("DOWN"))
	{
		if(NextItemTab())
			UpdateItemDetail();
		return;
	}

	if (TRUE == KEYDOWN("Enter")) 
	{
		//구매확인창을 띄운다.
		SelectItemTabEvent();
	}

	if (TRUE == KEYDOWN("MenuKey")) 
	{
		m_ActiveItemTab[m_CurPage][m_CurItemIndex]->SelectTab(false);
		m_CurSelectTab = SELECT_STORE_TAB;
		m_CurItemIndex = 0;
		m_ItemDetailFontRen->Off();
		return;
	}
}
void CStoreUI::StandBy(const STORETYPE& _StoreType)
{
	m_bKeyCheck = true;
	SettingStore(_StoreType);
	m_CurStoreIndex = 0;
	m_CurPage = 0;
	m_CurItemIndex = 0;
	m_ActiveStoreTab[m_CurStoreIndex]->SelectTab(true);
	UpdateInvenGold();

	//m_CurSelectTab = SELECT_STORE_TAB;

	ChangeItemListEvent();
}
bool CStoreUI::PrevStoreTab()
{
	if (0 == m_CurStoreIndex )
		return false;

	if (nullptr != m_ActiveStoreTab[m_CurStoreIndex])
		m_ActiveStoreTab[m_CurStoreIndex--]->SelectTab(false);

	if (nullptr != m_ActiveStoreTab[m_CurStoreIndex])
		m_ActiveStoreTab[m_CurStoreIndex]->SelectTab(true);

	return true;
	//ChangeItemListEvent();
}
bool CStoreUI::NextStoreTab()
{
	if (m_ActiveStoreTab.size() <= m_CurStoreIndex + 1)
		return false;

	if (nullptr != m_ActiveStoreTab[m_CurStoreIndex])
		m_ActiveStoreTab[m_CurStoreIndex++]->SelectTab(false);

	if (nullptr != m_ActiveStoreTab[m_CurStoreIndex])
		m_ActiveStoreTab[m_CurStoreIndex]->SelectTab(true);

	return true;
	//ChangeItemListEvent();
}
bool CStoreUI::PrevItemTab()
{
	if (0 == m_CurItemIndex) 
		return ChangeItemListPage(false);
		
	if (nullptr != m_ActiveItemTab[m_CurPage][m_CurItemIndex]) 
		m_ActiveItemTab[m_CurPage][m_CurItemIndex--]->SelectTab(false);
	
	if (nullptr != m_ActiveItemTab[m_CurPage][m_CurItemIndex])
		m_ActiveItemTab[m_CurPage][m_CurItemIndex]->SelectTab(true);

	return true;
}
bool CStoreUI::NextItemTab()
{
	if ( MAXITEMTAB - 1 == m_CurItemIndex)
		return ChangeItemListPage(true);

	if (m_ActiveItemTab.size() <= m_CurPage 
		|| nullptr == m_ActiveItemTab[m_CurPage][m_CurItemIndex + 1])
		return false;
	
	if (nullptr != m_ActiveItemTab[m_CurPage][m_CurItemIndex])
		m_ActiveItemTab[m_CurPage][m_CurItemIndex++]->SelectTab(false);

	if (nullptr != m_ActiveItemTab[m_CurPage][m_CurItemIndex])
		m_ActiveItemTab[m_CurPage][m_CurItemIndex]->SelectTab(true);
	return true;
}
bool CStoreUI::ChangeItemListPage(const bool& _NextPage)
{
	if (_NextPage) 
	{
		if (m_CurPage + 1 >= m_ActiveItemTab.size())
			return false;

		for (size_t i = 0; i < m_ActiveItemTab[m_CurPage].size(); i++)
		{
			if (nullptr != m_ActiveItemTab[m_CurPage][i])
			{
				m_ActiveItemTab[m_CurPage][i]->SelectTab(false);
				m_ActiveItemTab[m_CurPage][i]->ActorOff();
			}
		}

		++m_CurPage;

		for (size_t i = 0; i < m_ActiveItemTab[m_CurPage].size(); i++)
		{
			if (nullptr == m_ActiveItemTab[m_CurPage][i])
				break;
			
			m_ActiveItemTab[m_CurPage][i]->SetActorPos(m_ItemTabPivot[i]);
			m_ActiveItemTab[m_CurPage][i]->ActorOn();
		}
		m_CurItemIndex = 0;
		m_ActiveItemTab[m_CurPage][m_CurItemIndex]->SelectTab(true);
	}
	else 
	{
		if (0  >= m_CurPage)
			return false;

		for (size_t i = 0; i < m_ActiveItemTab[m_CurPage].size(); i++)
		{
			if (nullptr != m_ActiveItemTab[m_CurPage][i])
			{
				m_ActiveItemTab[m_CurPage][i]->SelectTab(false);
				m_ActiveItemTab[m_CurPage][i]->ActorOff();
			}
		}

		--m_CurPage;

		for (size_t i = 0; i < m_ActiveItemTab[m_CurPage].size(); i++)
		{
			if (nullptr == m_ActiveItemTab[m_CurPage][i])
				break;

			m_ActiveItemTab[m_CurPage][i]->SetActorPos(m_ItemTabPivot[i]);
			m_ActiveItemTab[m_CurPage][i]->ActorOn();
		}
		m_CurItemIndex = MAXITEMTAB -1 ;
		m_ActiveItemTab[m_CurPage][m_CurItemIndex]->SelectTab(true);

	}

	return true;
}
void CStoreUI::AllStoreTabActive(const bool& _Active)
{
	for (size_t i = 0; i < m_vecStoreMenuTab.size(); i++)
	{
		if (nullptr != m_vecStoreMenuTab[i])
		{
			m_vecStoreMenuTab[i]->SelectTab(false);
			if (true == _Active)
				m_vecStoreMenuTab[i]->MenuTabOn();
			else
				m_vecStoreMenuTab[i]->MenuTabOff();
		}
	}
}
void CStoreUI::AllItemTabActive(const bool& _Active)
{
	for (size_t i = 0; i < m_vecItemTab.size(); i++)
	{
		if (nullptr != m_vecItemTab[i])
		{
			m_vecItemTab[i]->SelectTab(false);
			m_vecItemTab[i]->ActorActive(_Active);
		}
	}
}
void CStoreUI::SettingStore(const STORETYPE& _StoreType)
{
	if (_StoreType >= MAXSTORE)
		return;

	m_CurStoreType = _StoreType;

	AllStoreTabActive(false);
	AllItemTabActive(false);

	m_ActiveStoreTab.clear();

	switch (m_CurStoreType)
	{
	case WEAPON_STORE: 
	{
		m_ActiveStoreTab.push_back(m_vecStoreMenuTab[WEAPON_TAB]);
		//m_ActiveStoreTab.push_back(m_vecStoreMenuTab[ARMOR_TAB]);
		//m_ActiveStoreTab.push_back(m_vecStoreMenuTab[ACCESSORIES_TAB]);
		//m_ActiveStoreTab.push_back(m_vecStoreMenuTab[ETC_TAB]);
		m_ActiveStoreTab.push_back(m_vecStoreMenuTab[SALE_TAB]);
	}
	break;
	case ARMOR_STORE:
	{
		m_ActiveStoreTab.push_back(m_vecStoreMenuTab[ARMOR_TAB]);
		m_ActiveStoreTab.push_back(m_vecStoreMenuTab[SALE_TAB]);
	}
	break;
	case ACCESSORIES_STORE: 
	{
		m_ActiveStoreTab.push_back(m_vecStoreMenuTab[ACCESSORIES_TAB]);
		m_ActiveStoreTab.push_back(m_vecStoreMenuTab[ETC_TAB]);
		m_ActiveStoreTab.push_back(m_vecStoreMenuTab[SALE_TAB]);
	}
	break;
	}

	for (size_t i = 0; i < m_ActiveStoreTab.size(); i++) 
	{
		m_ActiveStoreTab[i]->MenuTabOn();
		m_ActiveStoreTab[i]->SelectTab(false);
		m_ActiveStoreTab[i]->SetPivot(m_StoreTabPivot[i]);
	}
	
}
void CStoreUI::ChangeItemListEvent()
{
	if (nullptr == m_ActiveStoreTab[m_CurStoreIndex])
		return;

	for (size_t i = 0; i < m_vecStoreMenuTab.size(); i++)
	{
		if (m_vecStoreMenuTab[i] == m_ActiveStoreTab[m_CurStoreIndex])
		{
			AllItemTabActive(false);
			ChangeItemList((STORETABINDEX)i);
			return;
		}
	}

}
void CStoreUI::SelectItemTabEvent()
{
	bool Purchase = true;

	if (m_vecStoreMenuTab[SALE_TAB] == m_ActiveStoreTab[m_CurStoreIndex])
		Purchase = false;

	if (m_ActiveItemTab.size() <= m_CurPage || nullptr == m_ActiveItemTab[m_CurPage][m_CurItemIndex])
		return;


	m_StoreSubUI->StandBy(Purchase);
	m_StoreSubUI->SetActorPos(m_ItemTabPivot[m_CurItemIndex] + Vec3{50.f,-40.f,0.f});
	m_bKeyCheck = false;
}
void CStoreUI::PurchaseItemEvent()
{
	if (m_CurPage >= m_ActiveItemTab.size() ||
		nullptr == m_ActiveItemTab[m_CurPage][m_CurItemIndex])
		return;


	CClientGlobal::InventoryMgr->PurchaseItem(m_ActiveItemTab[m_CurPage][m_CurItemIndex]->GetItemNo(), 1);
	m_bKeyCheck = true;
	UpdateInvenGold();
}
void CStoreUI::SaleItemEvent() 
{
	if (m_CurPage >= m_ActiveItemTab.size() ||
		nullptr == m_ActiveItemTab[m_CurPage][m_CurItemIndex])
		return;


	CClientGlobal::InventoryMgr->SaleItem(m_ActiveItemTab[m_CurPage][m_CurItemIndex]->GetItemNo(), 1);
	m_bKeyCheck = true;
	UpdateInvenGold();
	m_ActiveItemTab[m_CurPage][m_CurItemIndex]->UpdateItemCount();
}
void CStoreUI::SetKeyCheck(const bool& _Check)
{
	m_bKeyCheck = _Check;
	
}
void CStoreUI::ChangeItemList(const  STORETABINDEX& _Index) 
{
	if (_Index >= MAXSTORETAB)
		return;

	for (size_t i = 0; i < m_ActiveItemTab.size(); i++)
		m_ActiveItemTab[i].clear();
	
	m_ActiveItemTab.clear();

	list<ItemData> TempList;
	TempList.clear();

	switch (_Index)
	{
	case CStoreUI::WEAPON_TAB:
		CClientGlobal::InventoryMgr->RequestStoreItemList(&TempList, CATEGORY_WEAPON);
		break;
	case CStoreUI::ARMOR_TAB:
		CClientGlobal::InventoryMgr->RequestStoreItemList(&TempList, CATEGORY_ARMOR);
		break;
	case CStoreUI::ACCESSORIES_TAB:
		CClientGlobal::InventoryMgr->RequestStoreItemList(&TempList, CATEGORY_ACCESSORIES);
		break;
	case CStoreUI::ETC_TAB:
		CClientGlobal::InventoryMgr->RequestStoreItemList(&TempList, CATEGORY_ETC);
		break;
	case CStoreUI::SALE_TAB:
		CClientGlobal::InventoryMgr->RequestInventoryItemList(&TempList);
		break;
	}

	PushActiveItemTab(&TempList, _Index);
	
	/*list<ItemData>::iterator Start = TempList.begin();
	list<ItemData>::iterator End = TempList.end();

	size_t Count = 0;
	size_t Page = 0;

	for (; Start != End; ++Start)
	{
		if (Start->ItemNo < m_vecItemTab.size())
		{
			if (0 == Count)
			{
				vector<SPTR<CItemTab>> NewVec;
				NewVec.assign(MAXITEMTAB, nullptr);
				m_ActiveItemTab.push_back(NewVec);
			}

		
				m_vecItemTab[Start->ItemNo]->ChangeTabMode(CItemTab::PURCHASE_MODE);

			m_ActiveItemTab[Page][Count++] = m_vecItemTab[Start->ItemNo];
			if (Count >= MAXITEMTAB)
			{
				++Page;
				Count = 0;
			}
		}
	}*/

	m_CurPage = 0;
	m_CurItemIndex = 0;

	if (m_ActiveItemTab.empty())
		return;

	for (size_t i = 0; i < m_ActiveItemTab[m_CurPage].size(); i++)
	{
		if (nullptr == m_ActiveItemTab[m_CurPage][i])
			break;

		m_ActiveItemTab[m_CurPage][i]->ActorOn();
		m_ActiveItemTab[m_CurPage][i]->SetActorPos(m_ItemTabPivot[i]);
	}
	
	m_ActiveItemTab[m_CurPage][m_CurItemIndex]->SelectTab(true);
	size_t No = m_ActiveItemTab[m_CurPage][m_CurItemIndex]->GetItemNo();
	m_ItemDetailFontRen->SetScriptString(CClientGlobal::vecItem[No].ItemDescription);

}
void CStoreUI::PushActiveItemTab(list<ItemData>* _ItemList, const STORETABINDEX& _Index)
{
	if (nullptr == _ItemList)
		return;
	
	list<ItemData>::iterator Start = _ItemList->begin();
	list<ItemData>::iterator End = _ItemList->end();

	size_t Count = 0;
	size_t Page = 0;

	// 현재 탭이 판매탭일경우 아이템 탭을 판매탭 모드로 변경
	// 현재 아이템 Data의 Count가 0인경우 Active시키지 않는다. 
	if (SALE_TAB == _Index) 
	{
		for (; Start != End; ++Start)
		{
			if (Start->ItemNo < m_vecItemTab.size())
			{
				if (0 == Count)
				{
					vector<SPTR<CItemTab>> NewVec;
					NewVec.assign(MAXITEMTAB, nullptr);
					m_ActiveItemTab.push_back(NewVec);
				}
				
				if (0 >= Start->ItemCount)
					continue;

				m_vecItemTab[Start->ItemNo]->SetCurItemCount(Start->ItemCount);
				m_vecItemTab[Start->ItemNo]->ChangeTabMode(CItemTab::SALE_MODE);
				m_ActiveItemTab[Page][Count++] = m_vecItemTab[Start->ItemNo];
				
				if (Count >= MAXITEMTAB)
				{
					++Page;
					Count = 0;
				}
			}
		}
	}
	else 
	{
		for (; Start != End; ++Start)
		{
			if (Start->ItemNo < m_vecItemTab.size())
			{
				if (0 == Count)
				{
					vector<SPTR<CItemTab>> NewVec;
					NewVec.assign(MAXITEMTAB, nullptr);
					m_ActiveItemTab.push_back(NewVec);
				}

				m_vecItemTab[Start->ItemNo]->ChangeTabMode(CItemTab::PURCHASE_MODE);
				m_ActiveItemTab[Page][Count++] = m_vecItemTab[Start->ItemNo];
				if (Count >= MAXITEMTAB)
				{
					++Page;
					Count = 0;
				}
			}
		}

	}

}
void CStoreUI::OpenStoreUI()
{
	ACTOR->On();
}
void CStoreUI::CloseStoreUI()
{
	AllItemTabActive(false);
	ACTOR->Off();
}