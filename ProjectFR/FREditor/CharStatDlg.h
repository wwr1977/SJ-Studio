#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CCharStatDlg ��ȭ �����Դϴ�.

class CCharStatDlg : public CDialogEx
{
	enum 
	{
		READCOUNT = 10
	};
	enum PARA
	{
		HPPARA,
		MPPARA,
		STRPARA,
		DEXPARA,
		INTPARA,
		LUCKPARA,
		DAMAGEPARA,
		DEFPARA,
		CRIPARA,
		CRIDAMAGEPARA,
		AVOIDPARA,
		MAXPARA,
	};

	DECLARE_DYNAMIC(CCharStatDlg)

public:
	CCharStatDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CCharStatDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CHARSTAT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	bool		 m_bInit;
	int			 m_iPrevIndex;
	CListBox	 m_CharacterListCtrl;
	CComboBox	 m_ComboCtrl;
	MAINSTAT	 m_MainStat;

	CEdit m_StrCtrl;
	CString m_Str;
	float m_fStr;

	CEdit m_DexCtrl;
	CString m_Dex;
	float m_fDex;

	CEdit m_IntCtrl;
	CString m_Int;
	float m_fInt;

	CEdit m_LukCtrl;
	CString m_Luk;
	float m_fLuk;

	CEdit m_DamageCtrl;
	CString m_Damage;
	float m_fDamage;

	CEdit m_DefCtrl;
	CString m_Def;
	float m_fDef;

	CEdit m_HpCtrl;
	CString m_Hp;
	float m_fHp;

	CEdit m_MpCtrl;
	CString m_Mp;
	float m_fMp;

	CEdit m_CriticalCtrl;
	int m_iCritical;
	float m_fCritical;

	CEdit m_CriDamageCtrl;
	int m_iCriDamage;
	float m_fCriDamage;

	CEdit m_DamageRangeCtrl;
	int m_iDamageRange;

	CSliderCtrl m_LevelCtrl;
	int m_iLevel;

	CEdit m_AvdCtrl;
	int m_iAvd;
	float m_fAvd;

	void SettingUnitName();
	// ����� �ȵ� ĳ���͸� Ŭ���ϰų� ó�� â�� ������� ������ â�� ��ġ�� �ʱ�ȭ�ϴ� �۾�
	void InitStatData(const STATPARAMETERDATA& _InitData = STATPARAMETERDATA());
	// 8���� �⺻ ���� EditBox�� ��Ŀ�� �ƿ������ ���� ��ġ�� Check�ϴ� �Լ� 
	void CheckParameterData(CString& _BaseData,float& _ApplyData);
	// ����Ʈ â�� �������� ȭ���� �Ķ���� â�� Ŭ���� ������ ������ �ݿ�
	void ChangeCharacterEvent();
	// ���̺� �ش� ĳ������ �������̺��� �������� ������� Level 1 �� ���� �ݿ�
	void SettingInitData(const STAT& _BaseStat);
	// ���̺� �ش� ĳ������ �������̺��� �����ϴ� ��� ���̺����� �ݿ�
	void SettingTableData(vector<STAT>* _Table);

	void CreateButtonEvent();
	void DeleteButtonEvent();
	void DataLoadButtonEvent();
	void DataSaveButtonEvent();

	// DataRenderer���� �� ���������� �����ö� ���
	int GetDetailStat(const PARA& _Para);
	int GetDetailStat(const int& _Para);

	virtual BOOL OnInitDialog();
	afx_msg void OnLbnSelchangeStatcharlist();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnCbnSelchangeMainstat();
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnEnKillfocusStr();
	afx_msg void OnEnKillfocusDex();
	afx_msg void OnEnKillfocusInt();
	afx_msg void OnEnKillfocusLuk();
	afx_msg void OnEnKillfocusDemage();
	afx_msg void OnEnKillfocusDef();
	afx_msg void OnEnKillfocusHp();
	afx_msg void OnEnKillfocusMp();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnEnKillfocusCritical();
	afx_msg void OnEnKillfocusCriticaldemage();
	afx_msg void OnEnKillfocusDemagerange();
	afx_msg void OnEnKillfocusAvoidability();
};
