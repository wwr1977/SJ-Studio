#pragma once


// CMonStatDlg ��ȭ �����Դϴ�.

class CMonStatDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMonStatDlg)

public:
	CMonStatDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CMonStatDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MONSTAT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
};
