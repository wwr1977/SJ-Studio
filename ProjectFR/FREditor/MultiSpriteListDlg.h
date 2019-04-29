#pragma once
#include "afxwin.h"


// CMultiSpriteListDlg ��ȭ �����Դϴ�.

class CMultiSpriteListDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMultiSpriteListDlg)

public:
	CMultiSpriteListDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CMultiSpriteListDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MULTISPRITELIST };
#endif
public:
	CListBox m_ListCtrl;
	// �ƹ��� �̺�Ʈ�� �۵����� �ʰ� �ϱ� ���ؼ�
	int					m_iPrevIndex;
	// ���� �������� Ŭ���� List�� String
	CString				m_LastSelectString;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.
	
//	User Custom Function
public:
	// ���� ����Ʈ�� �ʱ�ȭ �ϰ� �ʱ�ȭ ���ڿ����� ����ϴ� �Լ�
	void ResetList(const CString& _InitString = _T(""));
	// ����Ʈ ����� ������Ʈ �ϴ� �Լ�(�μ��� AniSpriteType)
	void UpdateList(ANISPRITETYPE _AniSpriteType);
	// ���������� ������ String�� ���
	CString GetLastSelectString();
	// ���� ���õ� ���ڿ��� ���
	CString GetCurSelectString();
	
	// ���� ��ư�� ���������� ����Ʈ���� �ش� ���ڿ��� ����� �Լ�
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
