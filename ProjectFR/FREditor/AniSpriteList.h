#pragma once
#include "afxwin.h"


// CAniSpriteList ��ȭ �����Դϴ�.


class CAniSpriteList : public CDialogEx
{
	DECLARE_DYNAMIC(CAniSpriteList)

public:
	CAniSpriteList(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CAniSpriteList();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ANISPRITELIST };
#endif
public:
	CListBox m_SpriteListCtrl;
	int		m_iPrevIndex;
	int		m_iLastSelectIndex;
	CString m_LastSelectString;

public:
	void ChangeSprinteType();
	void ShowListEvent();
	void UpdateAniSpriteList(ANISPRITETYPE _Type);
	

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
	virtual void OnOK();
public:
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	virtual BOOL OnInitDialog();
	afx_msg void OnLbnSelchangeList1();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
