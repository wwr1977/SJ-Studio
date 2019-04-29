#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CCharStatDlg 대화 상자입니다.

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
	CCharStatDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CCharStatDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CHARSTAT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

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
	// 등록이 안된 캐릭터를 클릭하거나 처음 창을 띄웠을때 에디터 창의 수치를 초기화하는 작업
	void InitStatData(const STATPARAMETERDATA& _InitData = STATPARAMETERDATA());
	// 8개의 기본 정보 EditBox가 포커스 아웃됬을때 안의 수치를 Check하는 함수 
	void CheckParameterData(CString& _BaseData,float& _ApplyData);
	// 리스트 창을 눌렀을때 화면의 파라미터 창에 클릭한 유닛의 정보를 반영
	void ChangeCharacterEvent();
	// 테이블에 해당 캐릭터의 스탯테이블이 존재하지 않을경우 Level 1 의 정보 반영
	void SettingInitData(const STAT& _BaseStat);
	// 테이블에 해당 캐릭터의 스탯테이블이 존재하는 경우 테이블정보 반영
	void SettingTableData(vector<STAT>* _Table);

	void CreateButtonEvent();
	void DeleteButtonEvent();
	void DataLoadButtonEvent();
	void DataSaveButtonEvent();

	// DataRenderer에서 상세 스텟정보를 가져올때 사용
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
