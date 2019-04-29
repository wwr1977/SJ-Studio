#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CSkillDataDlg ��ȭ �����Դϴ�.

class CSkillDataDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSkillDataDlg)

public:
	enum
	{
		MONSTER = MAXBATTLECHARACTER,
		ALLLEVEL = SKILLDATA::MAXSKILLLEVEL,
		MAXSKILLLV = LEVELDATA::MAXSKILLLEVEL,
		NONEBUFF = 0,
		MAXBUFFEFFECTINDEX = 0,
		MAXBUFFICON = 0,
		BUFFDETAIL = BUFFDATA::MAXBUFFTYPE,
	};

public:
	CSkillDataDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CSkillDataDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SKILLDATADLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()

	BOOL							m_bInit;
	unordered_map<UINT, EditSet>	m_mapEditSet;
	vector<CEdit*>					m_vecEdit;

	// Editor������ ���带 ���� �������� ��� �޺��ڽ��� ����
	// �ش� �޺��ڽ��� �ε����� ����־ ���� HitSound Index�� ���
	// ���� HitSound�� ���Ӱ� �����Ѵٸ� �� vector���� �������ؾ��Ѵ�.
	vector<UINT>					m_vecHitSoundIndex;

	SKILLDATA						m_CurSkillData;
	vector<LEVELDATA>				m_vecCurLevelData;
	LEVELDATA						m_CurLevelData;
	UINT							m_uiCurLevel;

	UINT							m_CurCharNo;
	SPTR<CSoundPlayer>				m_SkillDataSpeaker;
	// SkillData ��Ʈ�� & ����
	CListBox						m_SkillList;

	UINT							m_CurSkillNo;

	CComboBox						m_SkillCasterCombo;

	CEdit							m_SkillTotalTime;
	CString							m_strSkillTotalTime;
	float							m_fSkillTotalTime;

	CComboBox						m_SkillTypeCombo;
	CComboBox						m_BuffTypeCombo;

	CComboBox						m_TeamTargetCombo;
	CComboBox						m_EnemyTargetCombo;

	CComboBox						m_CasterInitPosCombo;

	CEdit							m_CasterFixPosX;
	CString							m_strCasterFixPosX;
	float							m_fCasterFixPosX;

	CEdit							m_CasterFixPosY;
	CString							m_strCasterFixPosY;
	float							m_fCasterFixPosY;

	CEdit							m_PosPivotX;
	CString							m_strPosPivotX;
	float							m_fPosPivotX;

	CEdit							m_PosPivotY;
	CString							m_strPosPivotY;
	float							m_fPosPivotY;

	// Skill Level Data �⺻ ��Ʈ�� & ����
	CComboBox						m_SkillLevelCombo;
	CComboBox						m_SkillCostCombo;

	CEdit							m_SkillCount;
	UINT							m_uiSkillCount;

	CEdit							m_SkillCost;
	UINT							m_uiSkillCost;
	
	// Demage Font Style �� �ɼ� ��
	CComboBox						m_DemageFontStyle;
	CSliderCtrl						m_OptionSlider;
	int								m_iDemageFontOption;


	// LevelData Detail Data (Hit)

	UINT							m_uiCurHitDataCount;
	CComboBox						m_HitDataCombo;

	CEdit							m_HitStartTime;
	CString							m_strHitStartTime;
	float							m_fHitStartTime;

	CEdit							m_HitOverlapCount;
	UINT							m_uiHitOverlapCount;
	
	CEdit							m_DemageMag;
	UINT							m_uiDemageMag;

	CEdit							m_FixDemage;
	UINT							m_uiFixDemage;

	CEdit							m_DemageFontMag;
	CString							m_strDemageFontMag;
	float							m_fDemageFontMag;

	CComboBox						m_HitEffectCombo;
	CComboBox						m_HitEffectPosModeCombo;

	CEdit							m_HitEffectPivotX;
	CString							m_strHitEffectPivotX;
	float							m_fHitEffectPivotX;

	CEdit							m_HitEffectPivotY;
	CString							m_strHitEffectPivotY;
	float							m_fHitEffectPivotY;

	CEdit							m_EffectImageMag;
	CString							m_strEffectImageMag;
	float							m_fEffectImageMag;

	CEdit							m_HitEffectSizeX;
	CString							m_strHitEffectSizeX;
	float							m_fHitEffectSizeX;

	CEdit							m_HitEffectSizeY;
	CString							m_strHitEffectSizeY;
	float							m_fHitEffectSizeY;

	CComboBox						m_HitSoundCombo;
	CComboBox						m_HitReactionCombo;


	// LevelData Detail Data (Buff)
	CComboBox						m_BuffDetailCombo;

	CEdit							m_BuffValue;
	int								m_iBuffValue;

	CEdit							m_BuffTurnCount;
	UINT							m_uiBuffTurnCount;

	CComboBox						m_BuffEffectCombo;
	CComboBox						m_BuffIconCombo;

	// ��Ʈ�ѹ� ���� �ʱ�ȭ(EditGlobal�� �ʱ�ȭ �������� �۾�)
	void InitSkillDataDlg();

	void InitCurSkillData();
	void ClickSkillListEvent(const int& _ListIndex);
	void ClearSkillList();
	void ClearHitDataList();

	void FillSkillData();
	// Hit or Buff Data�� Dlg Control�� ä���ش�.
	void FillSkillDetailData(const int& _Level);
	void FillBuffData();

	// Hit Data List�� New�� ������ �Ǹ�
	// HitData ���� ��Ʈ���� ���� �ʱ�ȭ ��Ű�� �۾�
	void InitHitDataCtrl();

	void InitAllCombo();
	void InitRegCombo();
	void InitTypeCombo();

	// �����Ϳ� Ű�� �������� �۵��Ǵ� �Լ�
	void ChangeEditEvent(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	// Ű�� �������� Time or Pos Value���� Ȯ���ϰ� �ƴѰ�� �ѹ��ϴ� �Լ�
	void ChangeEditString(CEdit* _Edit, CString* _String);
	// �����Ͱ� ��Ŀ�� �ƿ� �Ǿ����� ����� ���ڿ��� Ÿ���� ������ �Ǵ��ϴ� �Լ�
	void CheckTimeString(CEdit& _EditCtrl, CString& _TimeStr, float& _Time);
	void CheckFloatString(CEdit& _EditCtrl, CString& _TimeStr, float& _Value);
	void CheckPosString(CEdit& _EditCtrl, CString& _PosStr, float& _Pos);
	// ������ �ڽ��� Ŭ���� ��� ��ü ������ �ϱ� ���� �Լ�
	BOOL EditBoxClickCheck(MSG* pMsg);
	// ��� ��ư�� Ŭ���Ǿ����� üũ�ϴ� �Լ�
	BOOL CheckButtonEvent(MSG* pMsg);
	void EnableBuffControl(const BOOL& _Mode);
	void EnableEffectControl(const BOOL& _Mode);
	void EnableFixPosControl(const BOOL& _Mode);

	void ClickCasterComboEvent();

	void UpdateSkillList();
	void UpdateHitDataList();
	const int ListStringToSkillNo(CString& _ListString);

	// HitData ���� & ���� �Լ�
	BOOL AddHitData(const HITDATA& _Data);
	BOOL ChangeHitData(const HITDATA& _Data,const int& _HitIndex);

	// ComboBox Click Event
	BOOL SkillTypeComboClickEvent(const int& _Index);
	BOOL BuffTypeComboClickEvent(const int& _Index);
	BOOL TeamTargetClickEvent(const int& _Index);
	BOOL EnemyTargetClickEvent(const int& _Index);
	BOOL CasterInitPosModeClickEvent(const int& _Index);
	BOOL SkillLevelClickEvent(const int& _Index);
	BOOL SkillCostClickEvent(const int& _Index);
	BOOL DemageStyleClickEvent(const int& _Index);
	BOOL OptionSliderEvent(const int& _Option);
	BOOL HitDataListClickEvent(const int& _Index);
	BOOL HitEffectListClickEvent(const int& _Index);
	BOOL PlaySoundEvent(const int& _Index);

	// Button Event Fucntion
	BOOL LevelDataRegEvent(const int& _Index);
	BOOL LevelDataDelEvent(const int& _Index);
	BOOL SkillCostRegEvent(const int& _Index);
	BOOL SkillCostDelEvent(const int& _Index);
	BOOL HitDataRegEvent(const int& _Index);
	BOOL HitDataDelEvent(const int& _Index);
	

	// Monster Skill ���� �����Լ� (���� ��ų�� ù������ SkillDataDlg���� ����)
	BOOL CreateSkill();
	BOOL ModifySkillData(const int& _ListIndex);
	BOOL DeleteSkill(const int& _ListIndex);

	// �ε�, ���̺��� ��� SkillMgr���� ���� �����͸� ���Ͽ� �����Ѵ�.
	BOOL LoadSkillFile();
	BOOL SaveSkillFile();

	void SetSkillTestUnit();

public:
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	afx_msg void OnDestroy();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnLbnSelchangeList2();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	
	// SkillData KillFocus Msg
	afx_msg void OnEnKillfocusSdtotaltimeedit();
	afx_msg void OnEnKillfocusCharfixposxedit();
	afx_msg void OnEnKillfocusCharfixposyedit();
	afx_msg void OnEnKillfocusCharpospivotxedit();
	afx_msg void OnEnKillfocusCharpospivotyedit();

	// Base LevelData KillFocus Msg
	afx_msg void OnEnKillfocusSkillcountedit();
	afx_msg void OnEnKillfocusSkillcostedit();

	// Hit Data KillFocus Msg
	afx_msg void OnEnKillfocusHitstarttimeedit();
	afx_msg void OnEnKillfocusHitcountedit();
	afx_msg void OnEnKillfocusDemagemagedit();
	afx_msg void OnEnKillfocusFixdemageedit();
	afx_msg void OnEnKillfocusDemagefontmagedit();
	afx_msg void OnEnKillfocusHiteffectpospivotxedit();
	afx_msg void OnEnKillfocusHiteffectpospivotyedit();
	afx_msg void OnEnKillfocusHiteffeimagemagedit();
	afx_msg void OnEnKillfocusHiteffetsizexedit();
	afx_msg void OnEnKillfocusHiteffetsizeyedit();

	// Buff Data KillFocus Msg
	afx_msg void OnEnKillfocusBuffvalueedit();
	afx_msg void OnEnKillfocusBuffturncountedit();

	// Button Click Msg
	afx_msg void OnBnClickedSkilldatacreatebutton();
	afx_msg void OnBnClickedSkilldatadeletebutton();
	afx_msg void OnBnClickedSdskillloadebutton();
	afx_msg void OnBnClickedSdskillsaveebutton();
	afx_msg void OnBnClickedLevelregbutton();
	afx_msg void OnBnClickedLeveldelbutton();
	afx_msg void OnBnClickedCostregbutton();
	afx_msg void OnBnClickedCostdelbutton();
	afx_msg void OnBnClickedHitregbutton();
	afx_msg void OnBnClickedHitdelbutton();

	// Reg & Type Combo Box Click Msg
	afx_msg void OnCbnSelchangeSkilldataCharcom();
	afx_msg void OnCbnSelchangeSkilltypecom();
	afx_msg void OnCbnSelchangeBufftypecom();
	afx_msg void OnCbnSelchangeTeamtargetcom();
	afx_msg void OnCbnSelchangeEnemytargetcom();
	afx_msg void OnCbnSelchangeInitcharposcom();
	afx_msg void OnCbnSelchangeSkilllevelcom();
	afx_msg void OnCbnSelchangeSkilllcostcom();
	afx_msg void OnCbnSelchangeHitdatacom();
	afx_msg void OnCbnSelchangeSdhiteffectcom();
	afx_msg void OnCbnSelchangeHitsoundcom();
	afx_msg void OnCbnSelchangeDemagestylecom();

};
