#include "Precom.h"
#include "Inventory.h"
#include "InventoryMgr.h"
#include "InvenSlot.h"
#include "MenuTab.h"
#include "GameMaster.h"
#include "MouseCursor.h"
#include "InvenItemGuide.h"

CInventory::CInventory()
	:m_pInventoryMgr(nullptr), m_InventoryBack(nullptr)
	, m_ItemTypeBackRen(nullptr), m_InvenGoldRen(nullptr)
	, m_InvenGoldFontRen(nullptr), m_CurSlotIndex(0)
	, m_CurInvenType(WEAPON_INVEN), m_InventoryCol(nullptr)
	, m_HoldingPivot(Vec3{ 0.f,0.f,0.f })
	, m_MainMouseCursor(nullptr), m_bHolding(false)
	, m_ItemGuide(nullptr)
	, m_GuideXPivot(430.f)
	, m_WndSize(Vec2{0.f,0.f})
{
	m_InvenTypeTab.clear();
	m_vecInvenSlot.clear();
	m_ActiveInvenSlot.clear();
	m_InvenSlotPivot.clear();
	m_vecInvenLineRen.clear();
	CInvenSlot::SetInventory(this);
}


CInventory::~CInventory()
{
	m_MainMouseCursor = nullptr;
	m_ItemGuide = nullptr;
	m_InvenTypeTab.clear();
	m_vecInvenSlot.clear();
	m_ActiveInvenSlot.clear();
	m_InvenSlotPivot.clear();
	m_vecInvenLineRen.clear();
	
}
void CInventory::SetInventoryMgr(CInventoryMgr* _pInvenMgr)
{
	if (nullptr == _pInvenMgr)
		return;

	m_pInventoryMgr = _pInvenMgr;
}
void CInventory::SetMouseCursor(SPTR<CMouseCursor> _Cursor)
{
	if (nullptr == _Cursor)
		return;

	m_MainMouseCursor = _Cursor;
}
void CInventory::Init() 
{
	m_WndSize = Vec2{ (float)CClientGlobal::MainWindow->WndSize().x, (float)CClientGlobal::MainWindow->WndSize().y };
	float X = m_WndSize.x;
	float Y = m_WndSize.y;

	m_InvenEndPoint.push_back(Vec3{ -235.f,332.f,7.f });
	m_InvenEndPoint.push_back(Vec3{ 235.f,332.f,7.f });
	m_InvenEndPoint.push_back(Vec3{ 235.f,-348.f,7.f });
	m_InvenEndPoint.push_back(Vec3{ -235.f,-348.f,7.f });

	m_ItemTypeBackRen = ACTOR->CreateCom<CFixRenderer>(RENDATA(LAYER_UIBACK, 0.75f, true));
	m_ItemTypeBackRen->SetSprite(_T("ColorBase"));
	m_ItemTypeBackRen->SetCustomSize(Vec2{ 470.f,64.f });
	m_ItemTypeBackRen->SetRenPivot(Vec2{ 0.f, 300.f });
	m_ItemTypeBackRen->SetMaterial(D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.f));
	m_ItemTypeBackRen->MaterialOn();

	m_InventoryBack = ACTOR->CreateCom<CFixRenderer>(RENDATA(LAYER_UIBACK, 0.75f, true));
	m_InventoryBack->SetSprite(_T("ColorBase"));
	m_InventoryBack->SetCustomSize(Vec2{ 470.f,600.f });
	m_InventoryBack->SetRenPivot(Vec2{ 0.f,Y*-0.06f });
	m_InventoryBack->SetMaterial(D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.f));
	m_InventoryBack->MaterialOn();

	for (size_t j = 0; j < MAXINVEN; ++j)
	{
		m_InvenTypeTab.push_back(ACTOR->CreateCom<CMenuTab>());
		m_InvenTypeTab[j]->SetTabStyle(_T("큰UI레벨폰트"));
		m_InvenTypeTab[j]->SetTabSize(Vec2{ 80.f,25.f });
		m_InvenTypeTab[j]->SetPivot(Vec3{ -182.f + 90.f*(float)j, 290.f ,0.f});
		m_InvenTypeTab[j]->SetFontPivot(Vec3{ -182.f + 90.f*(float)j, 291.5f ,0.f });
		//m_InvenTypeTab[j]->SetSelectColor(D3DXCOLOR(0.9f, 0.85f, 0.1f, 1.f));
	}
	m_InvenTypeTab[WEAPON_INVEN]->SelectTab(true);
	m_InvenTypeTab[WEAPON_INVEN]->SetTabName(_T("무기"));
	m_InvenTypeTab[ARMOR_INVEN]->SetTabName(_T("방어구"));
	m_InvenTypeTab[ACCESSORIES_INVEN]->SetTabName(_T("장신구"));
	m_InvenTypeTab[CONSUMPTION_INVEN]->SetTabName(_T("소모품"));
	m_InvenTypeTab[ETC_INVEN]->SetTabName(_T("기타"));
	

	m_InvenGoldFontRen = ACTOR->CreateCom<CFontRenderer>(RENDATA(LAYER_UI, 1.f, true));
	m_InvenGoldFontRen->SetFontStyle(_T("볼드굴림"));
	m_InvenGoldFontRen->SetString(_T("소지 골드 "));
	m_InvenGoldFontRen->SetLeftStartPivot(POINT{ -210, -320});

	m_InvenGoldRen = ACTOR->CreateCom<CFontRenderer>(RENDATA(LAYER_UI, 1.f, true));
	m_InvenGoldRen->SetFontStyle(_T("큰UI레벨폰트"));
	m_InvenGoldRen->SetRightStartPivot(POINT{ 210, -320});
	m_InvenGoldRen->SetFontColor(D3DCOLOR_ARGB(255, 255, 135, 0));
	
	for (size_t i = 0; i < MAXINVENSLOT; i++)
		m_InvenSlotPivot.push_back(Vec3{ 0.f,220.f - 40.f*(float)i,0.f });
	


	for (size_t i = 0; i < CClientGlobal::vecItem.size(); i++)
	{
		m_vecInvenSlot.push_back(CClientGlobal::GameMaster->CreateActObject(_T("ItemSlot"))->CreateCom<CInvenSlot>());
		m_vecInvenSlot[i]->SetInvenSlot(i);
		m_vecInvenSlot[i]->ActorOff();
	}


	//m_Test = ACTOR->CreateCom<CFixRenderer>(RENDATA(LAYER_UIBACK, 0.75f, true));
	//m_Test->SetSprite(_T("ColorBase"));
	//m_Test->SetCustomSize(Vec2{ 470.f,680.f });
	//m_Test->SetRenPivot(Vec2{ 0.f, -8.f });
	//m_Test->SetMaterial(D3DXCOLOR(0.9f, 0.9f, 0.9f, 1.f));
	//m_Test->MaterialOn();
	//

	for (size_t i = 0; i < MAXLINECOUNT; i++)
	{
		m_vecInvenLineRen.push_back(ACTOR->CreateCom<CLineRenderer>(RENDATA(LAYER_UI, 1.f, true)));
		m_vecInvenLineRen[i]->SetLineThick(2.f);
		m_vecInvenLineRen[i]->SetColor(D3DXCOLOR(1.f, 0.7f, 0.3f, 1.f));
		m_vecInvenLineRen[i]->Off();
	}

	m_InventoryCol = ACTOR->CreateCom<CCollision>(COLDATA(_T("Inventory")));
	m_InventoryCol->SetColType(CT_RECT);
	m_InventoryCol->SetColSize(Vec2{ 470.f,100.f });
	m_InventoryCol->SetPivotPos(Vec2{ 0.f, 300.f });
	m_InventoryCol->ColDebugRenActive(false);

	m_InventoryCol->SetEnter(this, &CInventory::InvenEnter);
	m_InventoryCol->SetStay(this, &CInventory::InvenStay);
	m_InventoryCol->SetExit(this, &CInventory::InvenExit);


	m_ItemGuide = CActObject::CreateActObject(_T("ItemGuide"), ACTOR->GetScene())->CreateCom<CInvenItemGuide>();
	m_ItemGuide->ActorOff();


	SetInvenPos(Vec3{ X*0.24f ,0.f,7.f });



	this->Off();
	ACTOR->Off();
}
void CInventory::Update()
{
	if (CKeyMgr::GetWheelEvent())
	{
		int Sign = WheelEvent(CKeyMgr::GetWheelSign());

		if (0 != Sign)
			return ChangeInvenSlotList(Sign);
	}

	if (TRUE == KEYDOWN("DOWN") || TRUE == KEYDOWN("S_Key"))
	{
		int Sign = WheelEvent(-1);

		if (0 != Sign)
			return ChangeInvenSlotList(Sign);
	}

	if (TRUE == KEYDOWN("UP") || TRUE == KEYDOWN("W_Key"))
	{
		int Sign = WheelEvent(1);

		if (0 != Sign)
			return ChangeInvenSlotList(Sign);
	}

	
	if (TRUE == KEYDOWN("RIGHT") || TRUE == KEYDOWN("D_Key"))
	{
		if (ETC_INVEN <= m_CurInvenType)
			return;

		if(nullptr != m_InvenTypeTab[m_CurInvenType])
			m_InvenTypeTab[m_CurInvenType]->SelectTab(false);
		
		m_CurInvenType = (INVENTYPE)(m_CurInvenType + 1 );
		
		if (nullptr != m_InvenTypeTab[m_CurInvenType])
			m_InvenTypeTab[m_CurInvenType]->SelectTab(true);

		UpdateActiveInvenSlot();
		InitInvenSlotDeque();
		return;
	}
		
	if (TRUE == KEYDOWN("LEFT") || TRUE == KEYDOWN("A_Key"))
	{
		if (WEAPON_INVEN >= m_CurInvenType)
			return;

		if (nullptr != m_InvenTypeTab[m_CurInvenType])
			m_InvenTypeTab[m_CurInvenType]->SelectTab(false);

		m_CurInvenType = (INVENTYPE)(m_CurInvenType - 1);

		if (nullptr != m_InvenTypeTab[m_CurInvenType])
			m_InvenTypeTab[m_CurInvenType]->SelectTab(true);

		UpdateActiveInvenSlot();
		InitInvenSlotDeque();
		return;
	}


	if (TRUE == KEYDOWN("InvenKey") || TRUE == KEYDOWN("MenuKey"))
	{
		CloseInventory();
		return;
	}	
	

	if (m_bHolding && m_MainMouseCursor->IsMouseMove()) 
	{
		Vec3 Dir = Vec3{ m_MainMouseCursor->GetMoveDir().x,m_MainMouseCursor->GetMoveDir().y ,0.f };
		SetInvenPos(TRANS->GetPos() + Dir);
	}
}
const int CInventory::WheelEvent(const int& _Sign)
{
	if (m_ActiveInvenSlot.empty() || m_InvenSlotDeque.empty())
		return 0;

	// 휠을 뒤로 돌린다.
	if (_Sign < 0)
	{
		if (m_InvenSlotDeque.back() == m_ActiveInvenSlot.back())
			return 0;

		return 1;
	}
	// 휠을 앞으로 돌린다.
	else
	{
		if (m_InvenSlotDeque.front() == m_ActiveInvenSlot.front())
			return 0;

		return -1;
	}
}
void CInventory::ChangeInvenSlotList(const int& _Sign)
{
	if (0 < _Sign) 
	{
		m_InvenSlotDeque.front()->ActorOff();
		m_InvenSlotDeque.pop_front();
		++m_CurSlotIndex;
		m_InvenSlotDeque.push_back(m_ActiveInvenSlot[m_CurSlotIndex + MAXINVENSLOT - 1 ]);
		m_InvenSlotDeque.back()->ActorOn();
	}
	else if(0 > _Sign)
	{
		m_InvenSlotDeque.back()->ActorOff();
		m_InvenSlotDeque.pop_back();
		m_InvenSlotDeque.push_front(m_ActiveInvenSlot[--m_CurSlotIndex]);
		m_InvenSlotDeque.front()->ActorOn();
	}
	
	Vec3 Pos = TRANS->GetPos();

	for (size_t i = 0; i < m_InvenSlotDeque.size(); i++)
		m_InvenSlotDeque[i]->SetActorPos(Pos + m_InvenSlotPivot[i]);

	
}
void CInventory::UpdateInvenGold()
{
	size_t Gold = m_pInventoryMgr->GetGold();

	m_InvenGoldRen->SetString(to_wstring(Gold) + _T(" G"));
}
void CInventory::UpdateActiveInvenSlot()
{
	//m_ActiveInvenSlot.clear();

	//for (size_t i = 0; i < m_vecInvenSlot.size(); i++)
	//{
	//	if(nullptr == m_vecInvenSlot[i])
	//		continue;
	//	
	//	if (0 != m_vecInvenSlot[i]->UpdateInvenItemCount())
	//		m_ActiveInvenSlot.push_back(m_vecInvenSlot[i]);
	//}

	m_ActiveInvenSlot.clear();

	for (size_t i = 0; i < m_vecInvenSlot.size(); i++)
	{
		if (nullptr == m_vecInvenSlot[i])
			continue;

		size_t ItemNo = m_vecInvenSlot[i]->GetItemNo();
		ITEMCATEGORY Cat = CClientGlobal::vecItem[ItemNo].ItemCategory;

		if (0 != m_vecInvenSlot[i]->UpdateInvenItemCount() && Cat == m_CurInvenType)
			m_ActiveInvenSlot.push_back(m_vecInvenSlot[i]);
	}
}
void CInventory::InitInvenSlotDeque()
{
	Vec3 Pos = TRANS->GetPos();

	if (!m_InvenSlotDeque.empty()) 
	{
		for (size_t i = 0; i < m_InvenSlotDeque.size(); i++)
		{
			if (nullptr != m_InvenSlotDeque[i])
				m_InvenSlotDeque[i]->ActorOff();
		}
		m_InvenSlotDeque.clear();
	}

	for (size_t i = 0; i < min(MAXINVENSLOT, m_ActiveInvenSlot.size()); i++)
		m_InvenSlotDeque.push_back(m_ActiveInvenSlot[i]);

	for (size_t i = 0; i < m_InvenSlotDeque.size(); i++)
	{
		m_InvenSlotDeque[i]->SetActorPos(Pos + m_InvenSlotPivot[i]);
		m_InvenSlotDeque[i]->ActorOn();
	}

}
void CInventory::SyncroInvenSlotPos()
{
	Vec3 Pos = TRANS->GetPos();

	for (size_t i = 0; i < m_InvenSlotDeque.size(); i++)
		m_InvenSlotDeque[i]->SetActorPos(Pos + m_InvenSlotPivot[i]);
	
}
void CInventory::StandBy()
{
	m_CurSlotIndex = 0;
	ACTOR->On();
	UpdateInvenGold();
	
	Vec3 Pos = TRANS->GetPos();
	float WndX =  (float)CClientGlobal::MainWindow->WndSize().x;
	float WndY = (float)CClientGlobal::MainWindow->WndSize().y;

	if (abs(Pos.x) >= abs(WndX*0.5f) || abs(Pos.y) >= abs(WndY*0.5f))
		SetInvenPos(Vec3{ WndX*0.24f ,0.f,7.f });
	

	UpdateActiveInvenSlot();
	InitInvenSlotDeque();

	
}
void CInventory::CloseInventory()
{
	for (size_t i = 0; i < m_InvenSlotDeque.size(); i++)
	{
		if (nullptr != m_InvenSlotDeque[i])
			m_InvenSlotDeque[i]->ActorOff();
	}

	m_InvenSlotDeque.clear();
	m_ActiveInvenSlot.clear();

	ItemGuideOffEvent();
	m_pInventoryMgr->CloseInventoryEvent();
	ACTOR->Off();
}
void CInventory::InvenEnter(SPTR<CCollision> _This, SPTR<CCollision> _Other) 
{
	ActiveFocusingLine(true);
}
void CInventory::InvenStay(SPTR<CCollision> _This, SPTR<CCollision> _Other) 
{
	if (!m_bHolding && TRUE == KEYDOWN("LMouse"))
	{
		m_bHolding = true;
		SelectFoucusinLine(true);
		return;
	}

	if (m_bHolding && TRUE == KEYUP("LMouse"))
	{
		m_bHolding = false;
		SelectFoucusinLine(false);
	}
}
void CInventory::InvenExit(SPTR<CCollision> _This, SPTR<CCollision> _Other) 
{
	ActiveFocusingLine(false);
}
void CInventory::ActiveFocusingLine(const bool& _Active)
{
	for (size_t i = 0; i < m_vecInvenLineRen.size(); i++)
	{
		if (nullptr != m_vecInvenLineRen[i]) 
		{
			m_vecInvenLineRen[i]->Active(_Active);
		}
	}
}
void CInventory::SelectFoucusinLine(const bool& _Select)
{
	if (_Select)
	{
		for (size_t i = 0; i < m_vecInvenLineRen.size(); i++)
		{
			if (nullptr != m_vecInvenLineRen[i])
				m_vecInvenLineRen[i]->SetColor(D3DXCOLOR(1.f,0.f,0.f,1.f));
		}
	}
	else 
	{
		for (size_t i = 0; i < m_vecInvenLineRen.size(); i++)
		{
			if (nullptr != m_vecInvenLineRen[i])
				m_vecInvenLineRen[i]->SetColor(D3DXCOLOR(1.f, 0.7f, 0.3f, 1.f));
		}
	}
}
void CInventory::SetInvenPos(const Vec3& _Pos)
{
	Vec3 Pos = _Pos;

	ACTOR->SetPos(_Pos);

	for (size_t i = 0; i < MAXLINECOUNT; i++)
		m_vecInvenLineRen[i]->SettingLineData(Pos + m_InvenEndPoint[i], Pos + m_InvenEndPoint[(i + 1) % MAXLINECOUNT]);
	

	SyncroInvenSlotPos();
}
void CInventory::SlotClickEvent(const size_t& _ItemNo, const bool& _ClickLeft)
{
	if (nullptr == m_ItemGuide || m_ItemGuide->IsActorOn())
		return;

	m_ItemGuide->ActorOn();
	m_ItemGuide->SetItemData(_ItemNo);

	// 아이템 정보창은 슬롯이 눌린 상대적 위치를 반영한다.
	// 만일 반영된 위치에 출력시 정보창이 짤린다면 그와 반대방향으로
	// 위치하게된다.

	Vec3 Pos = TRANS->GetPos();
	 
	if (_ClickLeft) 
	{
		if ( Pos.x - m_GuideXPivot >= m_WndSize.x*-0.5f + 190.f)
			m_ItemGuide->SetActorPos(Pos + Vec3{ -m_GuideXPivot ,3.f,0.f});
		else
			m_ItemGuide->SetActorPos(Pos + Vec3{ m_GuideXPivot ,3.f,0.f });
	}
	else 
	{
		if ( Pos.x + m_GuideXPivot <= m_WndSize.x*0.5f - 190.f)
			m_ItemGuide->SetActorPos(Pos + Vec3{ m_GuideXPivot ,3.f,0.f });
		else
			m_ItemGuide->SetActorPos(Pos + Vec3{ -m_GuideXPivot ,3.f,0.f });
	}
	
}
void CInventory::ItemGuideOffEvent()
{
	if (nullptr == m_ItemGuide || !m_ItemGuide->IsActorOn())
		return;

	m_ItemGuide->ActorOff();
}