#pragma once
#include "afxwin.h"


// CMultiSpriteListDlg 대화 상자입니다.

class CMultiSpriteListDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMultiSpriteListDlg)

public:
	CMultiSpriteListDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CMultiSpriteListDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MULTISPRITELIST };
#endif
public:
	CListBox m_ListCtrl;
	// 아무런 이벤트도 작동하지 않게 하기 위해서
	int					m_iPrevIndex;
	// 가장 마지막에 클릭한 List의 String
	CString				m_LastSelectString;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	
//	User Custom Function
public:
	// 현재 리스트를 초기화 하고 초기화 문자열만을 출력하는 함수
	void ResetList(const CString& _InitString = _T(""));
	// 리스트 목록을 업데이트 하는 함수(인수로 AniSpriteType)
	void UpdateList(ANISPRITETYPE _AniSpriteType);
	// 마지막으로 선택한 String을 출력
	CString GetLastSelectString();
	// 현재 선택된 문자열을 출력
	CString GetCurSelectString();
	
	// 제거 버튼이 눌려졌을때 리스트에서 해당 문자열을 지우는 함수
	BOOL EraseString(const CString& _EraseString = _T(""));

	DECLARE_MESSAGE_MAP()
public:
	virtual void OnOK();
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
	afx_msg void OnLbnSelchangeMultispritelist();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
};
