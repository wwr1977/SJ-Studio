#pragma once
#include "afxwin.h"


// CTextureListDlg 대화 상자입니다.

class CTextureListDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CTextureListDlg)

public:
	CTextureListDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CTextureListDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TEXTURELIST };
#endif

public:
	CListBox			m_ListCtrl;
	// 이전에 선택된 List Index(같은 리스트 Index를 클릭했거나 list의 스트링 이외의 list박스를 클릭햇을때)
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
	// 리스트 목록을 업데이트 하는 함수(인수로 폴더 인덱스를 던진다)
	void UpdateList(FOLDER _FolderIndex);
	// 마지막으로 선택한 String을 출력
	CString GetLastSelectString();
	// 현재 선택된 문자열을 출력
	CString GetCurSelectString();
	DECLARE_MESSAGE_MAP()

	virtual void OnOK();
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
	afx_msg void OnLbnSelchangeTexturelist();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
};
