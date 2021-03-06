#pragma once
#include "afxwin.h"


// EditDlg 대화 상자

class EditDlg : public CDialogEx
{
	DECLARE_DYNAMIC(EditDlg)

public:
	EditDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~EditDlg();

	// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_EDITDLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	void CreateLightDlg();
	void DeleteLightDlg();
	void CreateMatDlg();
	void DeleteMatDlg();
	void CreateFilterDlg();
	void DeleteFilterDlg();
	void CreateNaviDlg();
	void DeleteNaviDlg();
	void CreateFogDlg();
	void DeleteFogDlg();

public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnEnChangePosx();
	afx_msg void OnEnChangeScalex();
	afx_msg void OnEnChangeRotx();
	afx_msg void OnEnChangePosy();
	afx_msg void OnEnChangeScaley();
	afx_msg void OnEnChangeRoty();
	afx_msg void OnEnChangePosz();
	afx_msg void OnEnChangeScalez();
	afx_msg void OnEnChangeRotz();
	virtual BOOL OnInitDialog();

	void SetActorTransValue();
	void SetRenderTransValue();
	void SetCollTransValue();

	afx_msg void OnEnChangePosx2();
	afx_msg void OnEnChangePosy2();
	afx_msg void OnEnChangePosz2();
	afx_msg void OnEnChangeScalex2();
	afx_msg void OnEnChangeScaley2();
	afx_msg void OnEnChangeScalez2();
	afx_msg void OnEnChangePosx3();
	afx_msg void OnEnChangePosy3();
	afx_msg void OnEnChangePosz3();
	afx_msg void OnEnChangeScalex3();
	afx_msg void OnEnChangeScaley3();
	afx_msg void OnEnChangeScalez3();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	void ChangeCollPivotX(float _x);
	void ChangeCollPivotY(float _y);
	void ChangeCollPivotZ(float _z);
	void ChangeCollSizeX(float _x);
	void ChangeCollSizeY(float _y);
	void ChangeCollSizeZ(float _z);

	void ChangePosX();
	void ChangePosY();
	void ChangePosZ();
	void ChangeRotX();
	void ChangeRotY();
	void ChangeRotZ();
	//void ChangeRenPosX();
	//void ChangeRenPosY();
	//void ChangeRenPosZ();
	//void ChangeColPosX();
	//void ChangeColPosY();
	//void ChangeColPosZ();

	void ChangeActorTrans();

	CEdit m_Posx;





	afx_msg void OnDestroy();
};

