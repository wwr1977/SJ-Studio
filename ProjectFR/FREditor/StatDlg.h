#pragma once
#include "afxcmn.h"


// CStatDlg 대화 상자입니다.

class CStatDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CStatDlg)

public:
	CStatDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CStatDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_STATDLG };
#endif


public:
	enum STATINDEX
	{
		CHARACTER,
		MONSTER,
		MAXSTATINDEX,
	}m_StatCurTab;

	vector<CDialogEx*>				m_vecStatDlg;
	RECT m_DlgRect;
	CTabCtrl m_StatTabCtrl;
	

public:
	template<typename T>
	CDialogEx* CreateTab(UINT ID, tstring _TabName)
	{
		T* NewDlg = new T();

		if (FALSE == NewDlg->Create(ID, &m_StatTabCtrl))
		{
			AfxMessageBox(_T("메뉴 DLG 만들기 오류"), MB_OK | MB_ICONEXCLAMATION);
			return nullptr;
		}

		m_StatTabCtrl.InsertItem((int)m_vecStatDlg.size(), _TabName.c_str());

		m_vecStatDlg.push_back(NewDlg);
		return NewDlg;
	}

	void ShowDlg(STATINDEX _ShowIndex)
	{
		if ((size_t)_ShowIndex < 0 || (size_t)_ShowIndex >= m_vecStatDlg.size())
		{
			return;
		}

		m_vecStatDlg[m_StatCurTab]->ShowWindow(SW_HIDE);

		RECT Rc = {};

		m_StatTabCtrl.GetClientRect(&Rc);
		m_StatTabCtrl.AdjustRect(TRUE, &Rc);
		m_vecStatDlg[_ShowIndex]->SetWindowPos(&m_StatTabCtrl, Rc.left * -1 - 4, Rc.top * -1, Rc.right + 3 * Rc.left + 5, Rc.bottom + (int)(1.2f* Rc.top), SWP_NOZORDER);
		m_vecStatDlg[_ShowIndex]->ShowWindow(SW_SHOW);
		m_StatCurTab = _ShowIndex;
	}


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnTcnSelchangeStattab(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
};
