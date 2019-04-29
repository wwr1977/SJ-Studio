#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CAniSpriteDlg ��ȭ �����Դϴ�.

class CAniSpriteDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CAniSpriteDlg)

public:
	CAniSpriteDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CAniSpriteDlg();

// ��ȭ ���� �������Դϴ�.
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
	// ���� MultiSpriteDlg�� Ȱ��ȭ �Ǿ����� ���θ� Ȯ���ϴ� ����
	BOOL					m_bDlgOn;
	// SpriteList�� Ŭ�������� Sprite or Loop Animation���� ������� �Ǵ�
	BOOL					m_bAnimationMode;

public:
	BOOL IsDlgOn();

	template<typename T>
	CDialogEx* CreateTab(UINT ID, tstring _TabName)
	{
		T* NewDlg = new T();

		if (FALSE == NewDlg->Create(ID, &m_ListTabCtrl))
		{
			AfxMessageBox(_T("�޴� DLG ����� ����"), MB_OK | MB_ICONEXCLAMATION);
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
	// Row,Colm,Frame ī���Ͱ� ���� �������� �׵��� ��갪�� �����ϱ��� 
	// �ӽ� �������
	int		m_iTempFrameCount;
	
	CEdit m_PivotXCtrl;
	CString m_PivotX;
	float	m_fPivotX;
	
	CEdit m_PivotYCtrl;
	CString m_PivotY;
	float m_fPivotY;


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	// User Custom Function
public:													
	// ���� �޺� �ڽ��� �̺�Ʈ�� �߻��Ǿ��� �� Texture List�� Update�ϴ� �Լ�
	void UpdateTextureList();
	
	// ���� �޺� �ڽ��� �̺�Ʈ�� �߻��Ǿ��� �� MultiSprite List�� Update�ϴ� �Լ�
	void UpdateMultiSpriteList();
	
	// ����Ʈ �ڽ��� ���ڿ��� ���õǾ����� �۵��ϴ� �Լ�
	//(Ŭ���� ���ڿ��� �̸������ϴ� ���� �������ϱ� ����)
	BOOL SelectTextureList(const CString& _TexKey);
	// Texture List�� ������� Editâ �ʱ�ȭ 
	void InitTexData(const Vec2& _TexSize);
	BOOL SelectMultiSpriteList(const CString& _SpriteKey);
	// Sprite List�� ������� SpriteData Editâ �ʱ�ȭ
	BOOL InitSpriteData(const CString& _SpriteKey = _T(""));

	// Sence ���� ChangeAniMode Ű�� ���������� �۵��ϴ� �Լ�
	void ChangeModeEvent();

	// ��Ƽ ��������Ʈ�� ��������� EditBox�� ���� �˻��ϴ� �Լ�
	BOOL SettingSpriteValue();

	// ��ư �̺�Ʈ �Լ�
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
