#pragma once
#include "afxwin.h"


// CMultiSpriteDlg 대화 상자입니다.

class CMultiSpriteDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMultiSpriteDlg)


public:
	CMultiSpriteDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CMultiSpriteDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MULTISPRITE };
#endif

private:
	CComboBox			m_FolderCtrl;
	CListBox			m_ImageList;
	
	float				m_fStartPosX;
	CEdit				m_StartPosXCtrl;
	
	float				m_fStartPosY;
	CEdit				m_StartPosYCtrl;
	
	float				m_fSizeX;
	CEdit				m_SizeXCtrl;
	
	float				m_fSizeY;
	CEdit				m_SizeYCtrl;
	
	int					m_iRow;
	CEdit				m_RowCtrl;
	
	int					m_iColm;
	CEdit				m_ColmCtrl;
	
	int					m_iFrameCount;
	CEdit				m_FrameCtrl;
	
	float				m_fPivotX;
	CString				m_PivotX;
	CEdit				m_PivotXCtrl;

	float				m_fPivotY;
	CString				m_PivotY;
	CEdit				m_PivotYCtrl;

	CListBox			m_SaveSpriteKey;
	int					m_iPrevIndex;

	CComboBox			m_AddSpriteName;
	CEdit				m_SpriteMainName;
	BOOL				m_bSpriteMode;
	BOOL				m_bAnimationMode;
	
	//int				m_iLastSelectIndex;
	CString				m_LastSelectString;
public:
	void InitEditBox(const Vec2& _SpriteSize);
	void CreateButtonEvent();
	void SaveButtonEvent();
	void ChangeAniSpriteModel(const CString& _ChangeKey);
	BOOL SettingSpriteValue();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	afx_msg void OnCbnSelchangeCombo1();
	afx_msg void OnLbnSelchangeList1();
	afx_msg void OnBnClickedCreatebutton();
	afx_msg void OnCbnSelchangeSpritekey2();
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnBnClickedSpritecheck();
	afx_msg void OnBnClickedAnicheck();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	CEdit m_StartFrameCtrl;
	CString m_iStartFrame;
};

