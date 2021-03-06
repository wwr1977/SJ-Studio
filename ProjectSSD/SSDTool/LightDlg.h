#pragma once
#include <string>

// LightDlg 대화 상자

class LightDlg : public CDialogEx
{
	DECLARE_DYNAMIC(LightDlg)

public:
	LightDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~LightDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LIGHT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

	//LPCTSTR Key;

public:
	void SetCurLightInfo();
	void SetNewLightInfo(bool _VolumeCheck);
	std::wstring GetWinChar(float _Value);

	void ResetLightValue();

public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedRadio1();
	BOOL m_VolumeMeshCheck;
	afx_msg void OnEnChangeColorr();
	afx_msg void OnEnChangeColorg();
	afx_msg void OnEnChangeColorb();
	afx_msg void OnEnChangeColora();
	afx_msg void OnEnChangeDirx();
	afx_msg void OnEnChangeDiry();
	afx_msg void OnEnChangeDirz();
	afx_msg void OnEnChangeAtenw();
	afx_msg void OnEnChangeAtenz();
	afx_msg void OnEnChangeAteny();
	afx_msg void OnEnChangeAtenix();
	afx_msg void OnEnChangeSpecpower();
	afx_msg void OnEnChangeAmbipower();
	afx_msg void OnCbnSelchangeVolumemesh();
	afx_msg void OnCbnSelchangeLighttype();
	afx_msg void OnEnChangePower();
	afx_msg void OnEnChangeRange();
	CComboBox m_VolumeList;
	CComboBox m_TypeList;
	CString m_VolumeMeshType;
	CString m_LightTypeValue;
	afx_msg void OnCbnSetfocusVolumemesh();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnEnChangePosx3();
	afx_msg void OnEnChangePosy3();
	afx_msg void OnEnChangePosz3();
	afx_msg void OnEnChangeScalex3();
	afx_msg void OnEnChangeScaley3();
	afx_msg void OnEnChangeScalez3();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CSliderCtrl m_RSlider;
	CSliderCtrl m_GSlider;
	CSliderCtrl m_BSlider;
	CSliderCtrl m_ASlider;
	afx_msg void OnNMCustomdrawSlider1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSlider2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSlider3(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSlider4(NMHDR *pNMHDR, LRESULT *pResult);
	CSliderCtrl m_DistanceSlider;
	CSliderCtrl m_ConstantSlider;
	CSliderCtrl m_LinearSlider;
	CSliderCtrl m_QuadraticSlider;
	afx_msg void OnNMCustomdrawSlider5(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSlider6(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSlider7(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSlider8(NMHDR *pNMHDR, LRESULT *pResult);

	void SpotLightDirUpdate();

public:
	static float Rgbtofloat(int _Rgb);
	static int floattoRgb(float _Value);

	static float InttoFloat(int _Int);
	static int FloattoInt(float _Float);
};
