#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CSkillDataDlg 대화 상자입니다.

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
	CSkillDataDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CSkillDataDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SKILLDATADLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

	BOOL							m_bInit;
	unordered_map<UINT, EditSet>	m_mapEditSet;
	vector<CEdit*>					m_vecEdit;

	// Editor에서는 사운드를 같은 종류별로 묶어서 콤보박스에 적용
	// 해당 콤보박스의 인덱스를 집어넣어서 실제 HitSound Index를 출력
	// 만일 HitSound를 새롭게 갱신한다면 이 vector에도 적용을해야한다.
	vector<UINT>					m_vecHitSoundIndex;

	SKILLDATA						m_CurSkillData;
	vector<LEVELDATA>				m_vecCurLevelData;
	LEVELDATA						m_CurLevelData;
	UINT							m_uiCurLevel;

	UINT							m_CurCharNo;
	SPTR<CSoundPlayer>				m_SkillDataSpeaker;
	// SkillData 컨트롤 & 변수
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

	// Skill Level Data 기본 컨트롤 & 변수
	CComboBox						m_SkillLevelCombo;
	CComboBox						m_SkillCostCombo;

	CEdit							m_SkillCount;
	UINT							m_uiSkillCount;

	CEdit							m_SkillCost;
	UINT							m_uiSkillCost;
	
	// Demage Font Style 과 옵션 값
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

	// 컨트롤및 모델을 초기화(EditGlobal의 초기화 선행이후 작업)
	void InitSkillDataDlg();

	void InitCurSkillData();
	void ClickSkillListEvent(const int& _ListIndex);
	void ClearSkillList();
	void ClearHitDataList();

	void FillSkillData();
	// Hit or Buff Data를 Dlg Control에 채워준다.
	void FillSkillDetailData(const int& _Level);
	void FillBuffData();

	// Hit Data List의 New를 누르게 되면
	// HitData 관련 컨트롤의 값을 초기화 시키는 작업
	void InitHitDataCtrl();

	void InitAllCombo();
	void InitRegCombo();
	void InitTypeCombo();

	// 에디터에 키가 눌렷을때 작동되는 함수
	void ChangeEditEvent(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	// 키가 눌렷을때 Time or Pos Value인지 확인하고 아닌경우 롤백하는 함수
	void ChangeEditString(CEdit* _Edit, CString* _String);
	// 에디터가 포커스 아웃 되었을때 적용된 문자열이 타당한 값인지 판단하는 함수
	void CheckTimeString(CEdit& _EditCtrl, CString& _TimeStr, float& _Time);
	void CheckFloatString(CEdit& _EditCtrl, CString& _TimeStr, float& _Value);
	void CheckPosString(CEdit& _EditCtrl, CString& _PosStr, float& _Pos);
	// 에디터 박스가 클릭된 경우 전체 선택을 하기 위한 함수
	BOOL EditBoxClickCheck(MSG* pMsg);
	// 모든 버튼이 클릭되었음을 체크하는 함수
	BOOL CheckButtonEvent(MSG* pMsg);
	void EnableBuffControl(const BOOL& _Mode);
	void EnableEffectControl(const BOOL& _Mode);
	void EnableFixPosControl(const BOOL& _Mode);

	void ClickCasterComboEvent();

	void UpdateSkillList();
	void UpdateHitDataList();
	const int ListStringToSkillNo(CString& _ListString);

	// HitData 생성 & 변경 함수
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
	

	// Monster Skill 생성 전용함수 (몬스터 스킬의 첫생성은 SkillDataDlg에서 수행)
	BOOL CreateSkill();
	BOOL ModifySkillData(const int& _ListIndex);
	BOOL DeleteSkill(const int& _ListIndex);

	// 로드, 세이브의 경우 SkillMgr안의 현재 데이터를 파일에 저장한다.
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
