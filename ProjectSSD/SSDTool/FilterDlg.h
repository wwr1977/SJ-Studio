#pragma once

#include <string>
// FilterDlg 대화 상자

class FilterDlg : public CDialogEx
{
	DECLARE_DYNAMIC(FilterDlg)

public:
	FilterDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~FilterDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FILTER };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

public:
	void SetCurBloomInfo();
	void SetNewBloomInfo();
	void SetCurGaussianInfo();
	void SetNewGaussianInfo();
	std::wstring GetWinChar(float _Value);

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	CComboBox BufferSizeList;
	afx_msg void OnEnChangeSigma();
	afx_msg void OnEnChangeRange();
	afx_msg void OnEnChangeAmount();
	afx_msg void OnEnChangeMultiple();
	afx_msg void OnCbnSelchangeCombo1();
	virtual BOOL OnInitDialog();
	afx_msg void OnEnChangeLux();
	afx_msg void OnEnChangeLuy();
	afx_msg void OnEnChangeLuz();
	afx_msg void OnEnChangeSigma2();
	afx_msg void OnEnChangeRange2();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CSliderCtrl m_RangeSlider;
	afx_msg void OnNMCustomdrawRangeslider(NMHDR *pNMHDR, LRESULT *pResult);
	CButton m_GauOn;
	BOOL m_GauOnValue;
	CButton m_BloomOn;
	BOOL m_BloomOnValue;
	afx_msg void OnBnClickedOn();
	afx_msg void OnBnClickedOn2();
	afx_msg void OnBnClickedOff2();
	afx_msg void OnBnClickedOff();
};
