#pragma once
#include "afxcmn.h"


// CRootDlg ��ȭ �����Դϴ�.

class CRootDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CRootDlg)


public:
	enum TABINDEX
	{
		CHARACTER,
		MAP,
		STAT,
		MAXTAB
	} m_RootCurTab;

private:
	bool		m_bLock;
	RECT		m_DlgRect;
	
	bool		m_bAddStatDlg;
	

public:
	void Lock() { m_bLock = true; }
	void UnLock() { m_bLock = false; }

public:
	CRootDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CRootDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ROOTDLG };
#endif


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()

public:	
	vector<CDialogEx*>		m_vecDlg;
	CTabCtrl				m_RootTabCtrl;

	template<typename T>
	CDialogEx* CreateTab(UINT ID, tstring _TabName) 
	{
		T* NewDlg = new T();

		if (FALSE == NewDlg->Create(ID, &m_RootTabCtrl))
		{
			AfxMessageBox(_T("�޴� DLG ����� ����"), MB_OK | MB_ICONEXCLAMATION);
			return nullptr;
		}

		m_RootTabCtrl.InsertItem((int)m_vecDlg.size(), _TabName.c_str());

		m_vecDlg.push_back(NewDlg);
		return NewDlg;
	}

	void ShowDlg(TABINDEX _ShowIndex) 
	{
		if ((size_t)_ShowIndex < 0 || (size_t)_ShowIndex >= m_vecDlg.size())
		{
			return;
		}
		m_vecDlg[m_RootCurTab]->ShowWindow(SW_HIDE);

		RECT Rc = {};
		
		m_RootTabCtrl.GetClientRect(&Rc);
		m_RootTabCtrl.AdjustRect(TRUE, &Rc);
		m_vecDlg[_ShowIndex]->SetWindowPos(&m_RootTabCtrl, Rc.left * -1, Rc.top * -1, Rc.right - Rc.left - Rc.left * -4, Rc.bottom - Rc.top - Rc.top * -4, SWP_NOZORDER);
		m_vecDlg[_ShowIndex]->ShowWindow(SW_SHOW);
		m_RootCurTab = _ShowIndex;	
	}

public:
	void AddStatDlg();
	void ShowSkillDlg();

	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);
	virtual void OnOK();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnMouseHWheel(UINT nFlags, short zDelta, CPoint pt);
};
