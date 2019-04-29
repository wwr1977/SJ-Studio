#pragma once
#include "afxcmn.h"


// CCharacterDlg 대화 상자입니다.

class CCharacterDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CCharacterDlg)
	enum TABINDEX 
	{
		MULTISPRITE,
		ANIMATION,
		EFFECT,
		SKILL,
		CHARACTER
	} m_CharCurIndex;

public:
	CCharacterDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CCharacterDlg();

private:
	vector<CDialogEx*>		m_vecCharDlg;
	RECT m_DlgRect;
	CTabCtrl m_CharTabCtrl;



// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CHARACTER };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);
	

	template<typename T>
	CDialogEx* CreateTab(UINT ID, tstring _TabName)
	{
		T* NewDlg = new T();

		if (FALSE == NewDlg->Create(ID, &m_CharTabCtrl))
		{
			AfxMessageBox(_T("메뉴 DLG 만들기 오류"), MB_OK | MB_ICONEXCLAMATION);
			return nullptr;
		}

		m_CharTabCtrl.InsertItem((int)m_vecCharDlg.size(), _TabName.c_str());


		m_vecCharDlg.push_back(NewDlg);
		return NewDlg;
	}

	void ShowDlg(TABINDEX _ShowIndex)
	{
		if ((size_t)_ShowIndex < 0 || (size_t)_ShowIndex >= m_vecCharDlg.size())
		{
			return;
		}
		m_vecCharDlg[m_CharCurIndex]->ShowWindow(SW_HIDE);

		RECT Rc = {};

		m_CharTabCtrl.GetClientRect(&Rc);
		m_CharTabCtrl.AdjustRect(TRUE, &Rc);
		m_vecCharDlg[_ShowIndex]->SetWindowPos(&m_CharTabCtrl, Rc.left * -1 -4, Rc.top * -1, Rc.right + 3 * Rc.left + 5 , Rc.bottom +  (int)(1.2f* Rc.top) , SWP_NOZORDER);
		m_vecCharDlg[_ShowIndex]->ShowWindow(SW_SHOW);
		m_CharCurIndex = _ShowIndex;
	}


	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
};
