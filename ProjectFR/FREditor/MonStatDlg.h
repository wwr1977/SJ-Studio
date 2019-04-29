#pragma once


// CMonStatDlg 대화 상자입니다.

class CMonStatDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMonStatDlg)

public:
	CMonStatDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CMonStatDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MONSTAT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
};
