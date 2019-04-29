#pragma once
#include "afxwin.h"


// CAnimationList 대화 상자입니다.

class CAnimationList : public CDialogEx
{
	DECLARE_DYNAMIC(CAnimationList)

public:
	CAnimationList(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CAnimationList();

public:
	CListBox m_AniListCtrl;
	int		 m_iPrevIndex;
	int		 m_iLastSelectIndex;
	CString  m_LastSelectString;


// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ANIMATIONLIST };
#endif
public:
	void ShowListEvent();
	void UpdateAnimationList(ANISPRITETYPE _Type);
	CString GetCurSelectString();
	BOOL EraseString(const CString& _EraseString = _T(""));

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
	virtual void OnOK();
public:
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	virtual BOOL OnInitDialog();
	afx_msg void OnLbnSelchangeList1();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
