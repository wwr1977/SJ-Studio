#pragma once
#include "afxcmn.h"


// CSkillDlg ��ȭ �����Դϴ�.

class CActionScriptDlg;
class CSkillDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSkillDlg)

	enum SKILLTABINDEX
	{
		ACTIONSCRIPTDLG,
		SKILLDATADLG,
		MAXSKILLTABINDEX
	}m_CurTabIndex;


public:
	CSkillDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CSkillDlg();

private:
	vector<CDialogEx*>		m_vecSubSkillDlg;
	RECT m_DlgRect;
	CTabCtrl m_SkillTabCtrl;

	template<typename T>
	CDialogEx* CreateTab(UINT ID, tstring _TabName)
	{
		T* NewDlg = new T();

		if (FALSE == NewDlg->Create(ID, &m_SkillTabCtrl))
		{
			AfxMessageBox(_T("�޴� DLG ����� ����"), MB_OK | MB_ICONEXCLAMATION);
			return nullptr;
		}

		m_SkillTabCtrl.InsertItem((int)m_vecSubSkillDlg.size(), _TabName.c_str());


		m_vecSubSkillDlg.push_back(NewDlg);
		return NewDlg;
}

	void ShowDlg(SKILLTABINDEX _ShowIndex)
	{
		if ((size_t)_ShowIndex < 0 || (size_t)_ShowIndex >= m_vecSubSkillDlg.size())
		{
			return;
		}
		if (m_CurTabIndex < MAXSKILLTABINDEX)
			m_vecSubSkillDlg[m_CurTabIndex]->ShowWindow(SW_HIDE);

		RECT Rc = {};

		m_SkillTabCtrl.GetClientRect(&Rc);
		m_SkillTabCtrl.AdjustRect(TRUE, &Rc);
		m_vecSubSkillDlg[_ShowIndex]->SetWindowPos(&m_SkillTabCtrl, Rc.left * -1 - 4, Rc.top * -1, Rc.right + 3 * Rc.left + 5, Rc.bottom + (int)(1.2f* Rc.top), SWP_NOZORDER);
		m_vecSubSkillDlg[_ShowIndex]->ShowWindow(SW_SHOW);
		m_CurTabIndex = _ShowIndex;
	}
// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SKILLDLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnDestroy();
	afx_msg int  OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
	afx_msg void OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
};
