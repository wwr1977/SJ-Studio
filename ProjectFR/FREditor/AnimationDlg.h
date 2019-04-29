#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CAnimationDlg 대화 상자입니다.

class CAnimationDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CAnimationDlg)

	enum TABINDEX
	{
		ANISPRITELIST,
		ANIMATIONLIST,
		MAXANILIST
	} 
	m_CurAniListIndex;


	vector<CDialogEx*>		m_vecListDlg;
	RECT					m_DlgRect;
	CTabCtrl				m_ListTabCtrl;
	
public:
	CAnimationDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CAnimationDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ANIMATION };
#endif

	template<typename T>
	CDialogEx* CreateTab(UINT ID, tstring _TabName)
	{
		T* NewDlg = new T();

		if (FALSE == NewDlg->Create(ID, &m_ListTabCtrl))
		{
			AfxMessageBox(_T("메뉴 DLG 만들기 오류"), MB_OK | MB_ICONEXCLAMATION);
			return nullptr;
		}

		m_ListTabCtrl.InsertItem((int)m_vecListDlg.size(), _TabName.c_str());


		m_vecListDlg.push_back(NewDlg);
		return NewDlg;
	}

	void ShowDlg(TABINDEX _ShowIndex)
	{
		if ((size_t)_ShowIndex < 0 || (size_t)_ShowIndex >= m_vecListDlg.size())
		{
			return;
		}
		m_vecListDlg[m_CurAniListIndex]->ShowWindow(SW_HIDE);

		RECT Rc = {};

		m_ListTabCtrl.GetClientRect(&Rc);
		m_ListTabCtrl.AdjustRect(TRUE, &Rc);
		m_vecListDlg[_ShowIndex]->SetWindowPos(&m_ListTabCtrl, Rc.left * -1 -5, Rc.top * -1, Rc.right + 7 + 3 * Rc.left, Rc.bottom + (int)(1.2f* Rc.top), SWP_NOZORDER);
		m_vecListDlg[_ShowIndex]->ShowWindow(SW_SHOW);
		m_CurAniListIndex = _ShowIndex;
	}

public:
	CComboBox	m_TypeComCtrl;
	CString		m_CurType;
	
	CListBox m_AniSpriteCtrl;
	CEdit	m_AniNameCtrl;
	CString m_RegAniName;
	
	BOOL	m_bBaseMode;
	BOOL	m_bCycleMode;
	BOOL	m_bLoopMode;

	CEdit	m_FullStartCtrl;
	int		m_iFullStart;
	
	CEdit	m_FullEndCtrl;
	int		m_iFullEnd;
	
	CEdit	m_RepeatStartCtrl;
	int		m_iRepeatStart;
	
	CEdit	m_RepeatEndCtrl;
	int		m_iRepeatEnd;

	CEdit	m_AniCountCtrl;
	int		m_iAniCount;

	CEdit	m_FullIntervalCtrl;
	CString m_FullInterval;
	float	m_fFullInterval;

	CEdit	m_RepeatIntervalCtrl;
	CString m_RepeatInterval;
	float	m_fRepeatInterval;

	BOOL	m_bAnimationDlgOn;
	BOOL	m_bAnimationMode;
	
	CListBox m_AniFrameCtrl;
	CString m_AniFrameCount;

	CString m_LastSelectAniSprite;
	CString m_LastSelectAnimation;
public:
	BOOL IsDlgOn();
	void ChangeAniMode();
	void AniSpriteSelectEvent(const CString& _UpdateString);
	BOOL InitSpriteData(const CString& _UpdataString);
	BOOL InitAnimationData(const CString& _UpdateString);
	void AnimationSelectEvent(const CString& _UpdateString);
	void ChangeSpriteModel(const CString& _ChangeKey);
	BOOL ChangeAnimationModel(const CString& _ChangeKey);
	void UpdateCurAniList();
	ANISPRITETYPE GetCurTypeIndex();
	BOOL SettingAnimationValue();
	void RegisterBaseAni();
	void RegisterCycleAni();
	void RegisterLoopAni();
	void ChangeSpriteType();
	void AniRegisterEvent();
	BOOL AnimationDeleteEvent();
	BOOL AnimationLoadEvent();
	BOOL AnimationSaveEvent();

	

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);


	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnCbnSelchangeTypecombo();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnBnClickedBasecheck();
	afx_msg void OnBnClickedCyclecheck();
	afx_msg void OnBnClickedLoopcheck();
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);

};
