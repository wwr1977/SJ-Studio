#pragma once
#include "afxwin.h"


// CTextureListDlg ��ȭ �����Դϴ�.

class CTextureListDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CTextureListDlg)

public:
	CTextureListDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CTextureListDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TEXTURELIST };
#endif

public:
	CListBox			m_ListCtrl;
	// ������ ���õ� List Index(���� ����Ʈ Index�� Ŭ���߰ų� list�� ��Ʈ�� �̿��� list�ڽ��� Ŭ��������)
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
	// ����Ʈ ����� ������Ʈ �ϴ� �Լ�(�μ��� ���� �ε����� ������)
	void UpdateList(FOLDER _FolderIndex);
	// ���������� ������ String�� ���
	CString GetLastSelectString();
	// ���� ���õ� ���ڿ��� ���
	CString GetCurSelectString();
	DECLARE_MESSAGE_MAP()

	virtual void OnOK();
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
	afx_msg void OnLbnSelchangeTexturelist();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
};
