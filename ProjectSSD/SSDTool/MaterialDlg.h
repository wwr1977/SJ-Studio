#pragma once


// MaterialDlg 대화 상자

#include "SSDToolGlobal.h"

class MaterialDlg : public CDialogEx
{
	DECLARE_DYNAMIC(MaterialDlg)

public:
	MaterialDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~MaterialDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MATDLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	virtual BOOL OnInitDialog();

	void SetMaterialValue(FbxData* _Data, int _Subset, bool _AllCheck = false);

	afx_msg void OnEnChangePath1();
	afx_msg void OnEnChangeName1();
	afx_msg void OnEnChangePath2();
	afx_msg void OnEnChangeName2();
	afx_msg void OnEnChangePath3();
	afx_msg void OnEnChangeName3();
	afx_msg void OnEnChangePath4();
	afx_msg void OnEnChangeName4();
	afx_msg void OnCbnSelchangeIndex();
	CComboBox m_IndexList;
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton6();
	afx_msg void OnBnClickedButton8();
	afx_msg void OnBnClickedButton1();
};
