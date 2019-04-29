#pragma once
#include "afxwin.h"


// CEffectDlg ��ȭ �����Դϴ�.

class CEffectDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CEffectDlg)
	enum
	{
		EFFECTANIMATION,
		SOUND,
		EFFECT
	};

	enum LAYERINDEX
	{
		MONBACK,
		// �÷��̾� �� == ���� �� 
		PLAYERBACK,
		PLAYERFRONT,
		MAXLAYERINDEX
	} m_iCurLayer;

public:
	CEffectDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CEffectDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_EFFECT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	//���̾�αװ� ó�� Ȱ��ȭ �Ǿ����� �ʿ��� �ؽ��ĸ� �ε� 
	bool				 m_bInit;
	// ���̾�αװ� Ȱ��ȭ �� �� ó������ �޺��ڽ��� �������� 
	// ��Ƽ��������Ʈ�� �ε��ϱ� ���� ����(��Ƽ��������Ʈ ���� ��� X,1ȸ �ε�)
	bool				 m_bMultiSpriteInit;

	// ���̾�αװ� ó�� Ȱ��ȭ �Ǿ��ų� 
	// AnimationDlg���� Save�۾��� ������ ���
	// �𵨿� ����� �ִϸ��̼��� �������� �ϱ� ���� ����
	bool				 m_bModelSetting;
	vector<CEdit*>		 m_vecEditBox;


	CListBox m_EffectListCtrl;
	int		 m_iPrevIndex;
	// ����Ʈ�� ����� ����� �����ϴ� �޺� �ڽ�
	CComboBox m_ListModeCom;
	int		  m_iListMode;

	// ���� ������ ����Ʈ �̸��� ����ϴ� ����Ʈ
	CListBox m_EffectAniNameCtrl;
	// ����Ʈ �̸�
	CEdit m_EffectNameCtrl;
	CString m_EffectName;

	// ����Ʈ�� ������ üũ�ϴ� ����
	BOOL m_bLoop;
	BOOL m_bBlackBack;
	BOOL m_bFixPos;
	

	CListBox			m_CurSoundCtrl;
	CComboBox			m_SoundListComCtrl;
	list<SoundData>		m_listSoundData;
	CEdit				m_SoundStartTimeCtrl;
	CString				m_SoundStartTime;
	float				m_fSoundStartTime;

	CEdit m_LifeTimeCtrl;
	CString m_LifeTime;
	float	m_fLifeTime;

	CEdit m_CustomSizeXCtrl;
	int m_iCustomSizeX;
	

	CEdit m_CustomSizeYCtrl;
	int m_iCustomSizeY;


	CEdit m_ImageMagCtrl;
	CString m_ImageMag;
	float	m_fImageMag;

	// ������ ���� ����ϴ� �޺� �ڽ�
	CComboBox m_LayerComCtrl;

	CEdit m_RenderStartTimeCtrl;
	CString m_RenderStartTime;
	float	m_fRenderStartTime;

	size_t m_EffectNumber;


	void InitEffectDlg();
	void CompleteEffectSetting();
	// ����Ʈ ���̾�α׿� �ʿ��� ���ҽ��� �޸𸮿� ����ȵȰ�� �ε��ϴ� �Լ�
	void LoadEffectTexture();
	BOOL LoadEffectMultiSprite();
	void LoadEffectAnimation();


	// ����Ʈ �޺� �ڽ��� �������� �����Ʈ�� ����Ʈ�ڽ��� ������Ʈ �ϴ� �Լ�
	void UpdateEffectAniList();
	void UpdateSoundList();
	void UpdateEffectList();
	void UpdateList(const int& _Index);
	
	// ����Ʈ�� Item�� ���õǾ����� � ���Ŀ� ���� �Լ�
	void SelectEffectAniList();
	void SelectSoundList();
	void SelectEffectList();

	void InitEffectData(const EFFECTDATA& _EffectData = EFFECTDATA());

	void SettingEffectModel();

	// ButtonŬ�� �̺�Ʈ ó�� �Լ�
	BOOL SoundPlayEvent();
	BOOL SoundAddButtonEvent();
	BOOL SoundDeleteButtonEvent();
	BOOL SoundTimeButtonEvent();
	BOOL CreateButtonEvent();
	BOOL DeleteButtonEvent();
	BOOL DataLoadButtonEvent();
	BOOL DataSaveButtonEvent();

	// ����Ʈ�� �Ŵ����� �������� �����;��� ���� �ùٸ�ġ �Ǵ��ϴ� �Լ�
	BOOL CheckEffectDataValue();
	// ������ �ڽ��� Ŭ���� ��� ��ü ������ �ϱ� ���� �Լ�
	BOOL EditBoxClickCheck(MSG* pMsg);


	virtual void OnOK();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnCbnSelchangeEffectlistcom();
	afx_msg void OnLbnSelchangeEffectlist();
	afx_msg void OnEnKillfocusStarttime();
	afx_msg void OnEnKillfocusEffectlifetime();
	afx_msg void OnEnKillfocusImagemag();
	afx_msg void OnEnKillfocusImagetime();
	afx_msg void OnCbnSelchangeSoundcombo();
	afx_msg void OnCbnSelchangeLayercombo();
};
