#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CAniSpriteDlg 대화 상자입니다.

class CAniSpriteDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CAniSpriteDlg)

public:
	CAniSpriteDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CAniSpriteDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_NEWMULTISPRITE };
#endif

	enum TABINDEX
	{
		TEXTURELIST,
		ANISPRITELIST,
		MAXMULTISPRITELIST
	} m_CurListIndex;

	

	vector<CDialogEx*>		m_vecListDlg;
	RECT					m_DlgRect;
	CTabCtrl				m_ListTabCtrl;
	// 현재 MultiSpriteDlg가 활성화 되었는지 여부를 확인하는 변수
	BOOL					m_bDlgOn;
	// SpriteList를 클릭했을때 Sprite or Loop Animation으로 출력할지 판단
	BOOL					m_bAnimationMode;

public:
	BOOL IsDlgOn();

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
		m_vecListDlg[m_CurListIndex]->ShowWindow(SW_HIDE);

		RECT Rc = {};

		m_ListTabCtrl.GetClientRect(&Rc);
		m_ListTabCtrl.AdjustRect(TRUE, &Rc);
		m_vecListDlg[_ShowIndex]->SetWindowPos(&m_ListTabCtrl, Rc.left * -1 - 5, Rc.top * -1, Rc.right + 7 + 3 * Rc.left, Rc.bottom + (int)(1.2f* Rc.top), SWP_NOZORDER);
		m_vecListDlg[_ShowIndex]->ShowWindow(SW_SHOW);
		m_CurListIndex = _ShowIndex;
	}


public:
	CComboBox m_FolderComCtrl;

	CListBox m_TextureNameCtrl;
	CString m_SelectTexName;

	CEdit m_SpriteNameCtrl;
	CString m_SpriteName;

	CEdit m_StartPosXCtrl;
	float m_fStartPosX;

	CEdit m_StartPosYCtrl;
	float m_fStartPosY;

	CEdit m_SizeXCtrl;
	float m_fSizeX;

	CEdit m_SizeYCtrl;
	float m_fSizeY;

	CEdit m_RowCtrl;
	int m_iRow;
	
	CEdit m_ColmCtrl;
	int m_iColm;
	
	CEdit m_StartFrameCtrl;
	int m_iStartFrame;
	
	CEdit	m_FrameCountCtrl;
	int		m_iFrameCount;
	// Row,Colm,Frame 카운터가 새로 쓰여질때 그들의 계산값을 적용하기전 
	// 임시 저장장소
	int		m_iTempFrameCount;
	
	CEdit m_PivotXCtrl;
	CString m_PivotX;
	float	m_fPivotX;
	
	CEdit m_PivotYCtrl;
	CString m_PivotY;
	float m_fPivotY;


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	// User Custom Function
public:													
	// 폴더 콤보 박스의 이벤트가 발생되었을 때 Texture List를 Update하는 함수
	void UpdateTextureList();
	
	// 폴더 콤보 박스의 이벤트가 발생되었을 때 MultiSprite List를 Update하는 함수
	void UpdateMultiSpriteList();
	
	// 리스트 박스의 문자열이 선택되었을때 작동하는 함수
	//(클릭한 문자열을 이름으로하는 모델을 랜더링하기 위해)
	BOOL SelectTextureList(const CString& _TexKey);
	// Texture List가 눌린경우 Edit창 초기화 
	void InitTexData(const Vec2& _TexSize);
	BOOL SelectMultiSpriteList(const CString& _SpriteKey);
	// Sprite List가 눌린경우 SpriteData Edit창 초기화
	BOOL InitSpriteData(const CString& _SpriteKey = _T(""));

	// Sence 에서 ChangeAniMode 키가 눌려졌을때 작동하는 함수
	void ChangeModeEvent();

	// 멀티 스프라이트를 만들기전에 EditBox의 값을 검사하는 함수
	BOOL SettingSpriteValue();

	// 버튼 이벤트 함수
	BOOL CreateButtonEvent();
	BOOL DeleteButtonEvent();
	BOOL DataLoadButtonEvent();
	BOOL DataSaveButtonEvent();

	

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnDestroy();
	afx_msg void OnCbnSelchangeFoldercom();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnTcnSelchangeMultispritetab(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnChangeStartframe();
	afx_msg void OnEnChangeXcount();
	afx_msg void OnEnChangeYcount();
	afx_msg void OnEnChangeNewframecount();
};
