#pragma once
#include <Logic.h>
class CInventory : public CLogic
{
	enum INVENTYPE
	{
		WEAPON_INVEN,
		ARMOR_INVEN,
		ACCESSORIES_INVEN,
		CONSUMPTION_INVEN,
		ETC_INVEN,
		MAXINVEN,
	}m_CurInvenType;

	enum
	{
		ENDPOINT_LT,
		ENDPOINT_RT,
		ENDPOINT_RB,
		ENDPOINT_LB,
		MAXENDPOINT,
		MAXLINECOUNT = 4,
	};

	enum 
	{
		MAXINVENSLOT = 13,
	};
private:
	class CInventoryMgr*				m_pInventoryMgr;
	SPTR<CMouseCursor>					m_MainMouseCursor;
	SPTR<CFixRenderer>					m_ItemTypeBackRen;
	SPTR<CFixRenderer>					m_InventoryBack;

	SPTR<CFontRenderer>					m_InvenGoldFontRen;
	SPTR<CFontRenderer>					m_InvenGoldRen;

	vector<SPTR<class CMenuTab>>		m_InvenTypeTab;

	// ��� �κ��丮 ����
	vector<SPTR<class CInvenSlot>>		m_vecInvenSlot;
	// ���� �κ��丮�ȿ� �ִ� �������� �κ�����
	vector<SPTR<class CInvenSlot>>		m_ActiveInvenSlot;
	// ���� �κ��丮���� ������ ���� �κ� ����
	deque<SPTR<class CInvenSlot>>		m_InvenSlotDeque;

	vector<Vec3>						m_InvenSlotPivot;
	//	Ȱ��ȭ�� ���� ���� �ִ� �����߿� ���� ���� �ִ� ������ �ε���
	size_t								m_CurSlotIndex;

	SPTR<CCollision>					m_InventoryCol;
	vector<Vec3>						m_InvenEndPoint;
	vector<SPTR<CLineRenderer>>			m_vecInvenLineRen;

	SPTR<CFixRenderer>					m_Test;

	bool								m_bHolding;
	Vec3								m_HoldingPivot;

	SPTR<class CInvenItemGuide>			m_ItemGuide;
	float								m_GuideXPivot;
	Vec2								m_WndSize;
public:
	void SetInventoryMgr(CInventoryMgr* _pInvenMgr);
	void SetMouseCursor(SPTR<CMouseCursor> _Cursor);
public:
	void Init();
	void Update();

public:
	const int WheelEvent(const int& _Sign);
	void ChangeInvenSlotList(const int& _Sign);
	void UpdateInvenGold();

public:
	void UpdateActiveInvenSlot();
	void InitInvenSlotDeque();
	void SyncroInvenSlotPos();

public:
	void StandBy();
	void CloseInventory();

public:
	void InvenEnter(SPTR<CCollision> _This, SPTR<CCollision> _Other);
	void InvenStay(SPTR<CCollision> _This, SPTR<CCollision> _Other);
	void InvenExit(SPTR<CCollision> _This, SPTR<CCollision> _Other);
	void ActiveFocusingLine(const bool& _Active);
	void SelectFoucusinLine(const bool& _Select);
public:
	void SetInvenPos(const Vec3& _Pos);

public:
	void SlotClickEvent(const size_t& _ItemNo,const bool& _ClickLeft);
	void ItemGuideOffEvent();

public:
	CInventory();
	~CInventory();
};

