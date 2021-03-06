#pragma once


// FogDlg 대화 상자

class FogDlg : public CDialogEx
{
	DECLARE_DYNAMIC(FogDlg)

public:
	FogDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~FogDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FOGDLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnEnChangeColorr();
	afx_msg void OnEnChangeColorg();
	afx_msg void OnEnChangeColorb();
	afx_msg void OnEnChangeColora();
	afx_msg void OnNMCustomdrawSlider1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSlider2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSlider3(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSlider4(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnChangeColorr2();
	afx_msg void OnEnChangeColorg2();
	afx_msg void OnEnChangeColorb2();
	afx_msg void OnEnChangeColora3();
	afx_msg void OnNMCustomdrawSlider5(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSlider6(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSlider7(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSlider8(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnChangeStartdepth();
	afx_msg void OnEnChangeDensity();
	afx_msg void OnEnChangeFalloff();
	afx_msg void OnEnChangeSundirx();
	afx_msg void OnEnChangeSundiry();
	afx_msg void OnEnChangeSundirz();
	virtual BOOL OnInitDialog();
	CSliderCtrl m_FogColorR;
	CSliderCtrl m_FogColorG;
	CSliderCtrl m_FogColorB;
	CSliderCtrl m_FogColorA;
	CSliderCtrl m_HighLightColorR;
	CSliderCtrl m_HighLightColorG;
	CSliderCtrl m_HighLightColorB;
	CSliderCtrl m_HighLightColorA;


	void SetCurFog();
};
