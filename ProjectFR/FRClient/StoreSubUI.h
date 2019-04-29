#pragma once
#include <Logic.h>
class CStoreSubUI : public CLogic
{
public:
	enum 
	{
		PURCHASE_TAB,
		SALE_TAB,
		CANCEL_TAB,
		MAXSUBSTORETAB,
	};
private:
	class CStoreUI*							m_pStoreUI;
	SPTR<CFixRenderer>						m_UIBackRen;
	vector<SPTR<class CMenuTab>>			m_vecSubUITab;
	vector<SPTR<class CMenuTab>>			m_ActiveTab;
	size_t									m_CurActiveIndex;


public:
	void Init();
	void Update();

public:
	bool PrevTab();
	bool NextTab();

public:
	void SetStoreUI(CStoreUI* _StoreUI);
	void AllTabActive(const bool& _Active);
	void AllTabSelectSet(const bool& _Select);
	void StandBy(const bool& _Purchase);
	void SelectTabEvent();

public:
	CStoreSubUI();
	~CStoreSubUI();
};

