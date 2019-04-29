#pragma once
#include "afxwin.h"


// CActionScriptDlg ��ȭ �����Դϴ�.

class CActionScriptDlg : public CDialogEx
{


	enum 
	{
		DOUBLEATK = 12,
		NODOUBLEATK = 10,
		NOFOCUS = 59649,
		NOREGINDEX = -1,
		AMONSTER = MAXBATTLECHARACTER,
	};

	enum MOVETYPE
	{
		MOVE,
		TEAMPOS,
		INITPOS,
		HIDE,
		SHOW,
		MAXMOVEDATATYPE,
	};

	enum EMOVETYPE 
	{
		NOMOVE,
		EMOVE,
		EHIDE,
		ESHOW,
		MAXEMOVETYPE,
	};

	// m_vecCamDetailType �� ������
	enum CAMEFFECT
	{
		EARTH,
		BLACKOUT,
		FADE,
		ZOOM,
		MAXCAMEFFECT
	};

	// m_CamEffectTypeCom �� ������
	enum CAMEFFECTTYPE
	{
		EARTH_XAXIS,
		EARTH_YAXIS,
		EARTH_RECT,
		EARTH_CIRCLE,
		BLACKOUT_FLASH,
		FADE_IN,
		FADE_OUT,
		ZOOMEFFECT,
		MAXCAMEFFECTTYPE
	};



	enum EARTHDETAIL
	{
		CONST_STRONG,
		CONST_NORMAL,
		CONST_WEAK,
		INC_STRONG,
		INC_NORMAL,
		INC_WEAK,
		DEC_STRONG,
		DEC_NORMAL,
		DEC_WEAK,
		MAXEARTHDETAIL
	};

	enum EARTHSPACESIZE
	{
		STRONG,
		NORMAL,
		WEAK,
		MAXEARTHSPACESIZE
	};

	enum FADECOLOR
	{
		FADECOLOR_BLACK,
		FADECOLOR_WHITE,
		FADECOLOR_RED,
		FADECOLOR_GREEN,
		FADECOLOR_BLUE,
		MAXFADECOLOR,
	};
	DECLARE_DYNAMIC(CActionScriptDlg)

public:
	CActionScriptDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CActionScriptDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ACTIONSCRIPT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()

	BOOL						m_bInit;
	vector<CEdit*>				m_vecEditCtrl;

	// nID�� Ű���ϴ� Edit��Ʈ�Ѱ� ���õ� ��� ���� �������� ����
	unordered_map<UINT,EditSet>	m_mapEditSet;

	vector<CString>				m_vecAnimationName;
	vector<tstring>				m_vecSkillSound;

	vector<vector<CString>>		m_vecCamDetailType;
	vector<Vec2>				m_vecEarthSpaceSize;
	
	// �׼� ��ũ��Ʈ �⺻����(�⺻���� , �ִϸ��̼� , ����)
	UINT						m_CurSkillNo;
	
	CEdit						m_TotalTimeEdit;
	CString						m_strTotalTime;
	float						m_fTotalTime;

	// ���������� �����Ϸ��� �ϴ� ������
	// �� ��� or ���� �̺�Ʈ�� ���������� �����̵ȴ�.
	// ���Ŀ� �׼� ��ũ��Ʈ�� �����Ҷ� �� ������ �����͸� �״�� �����Ұ��̴�.
	ActionScript				m_CurActionScript;

	// SkillList 
	CListBox					m_ActSkillList;

	// ĳ���� & �ִϸ��̼� ���� ��Ʈ�Ѱ� �Է� ����
	CComboBox					m_SkillCharCom;
	UINT						m_PrevCharNo;
	CComboBox					m_AniRegCom;
	CComboBox					m_AniTypeCom;
	
	CEdit						m_AniStartTimeEdit;
	CString						m_strAniStartTime;
	float						m_fAniStartTime;
	

	// ���� ���� ��Ʈ�Ѱ� �Է� ����
	CComboBox					m_SoundRegCom;
	CComboBox					m_SoundNameCom;

	CEdit						m_SoundStartTimeEdit;
	CString						m_strSoundStartTime;
	float						m_fSoundStartTime;

	// Move ������ ���� ��Ʈ�Ѱ� �Է� ����
	CComboBox					m_MoveRegCom;
	CComboBox					m_MoveTypeCom;


	CEdit						m_MoveStartTimeEdit;
	CString						m_strMoveStartTime;
	float						m_fMoveStartTime;

	CEdit						m_MoveTimeEdit;
	CString						m_strMoveTime;
	float						m_fMoveTime;
	
	CEdit						m_HideTimeEdit;
	CString						m_strHideTime;
	float						m_fHideTime;

	
	CEdit						m_MoveStartXEdit;
	CString						m_strMoveStartX;
	float						m_fMoveStartX;

	CEdit						m_MoveStartYEdit;
	CString						m_strMoveStartY;
	float						m_fMoveStartY;

	CEdit						m_MoveEndXEdit;
	CString						m_strMoveEndX;
	float						m_fMoveEndX;

	CEdit						m_MoveEndYEdit;
	CString						m_strMoveEndY;
	float						m_fMoveEndY;
	
	// SkillEffect ���� ��Ʈ�Ѱ� ����
	CComboBox					m_SkillEffectRegCom;
	CComboBox					m_SkillEffectNoCom;
	int							m_CurEffectNo;
	CComboBox					m_EffectPosTypeCom;

	CEdit						m_EffectInitPosXEdit;
	CString						m_strEffectInitPosX;
	float						m_fEffectInitPosX;

	CEdit						m_EffectInitPosYEdit;
	CString						m_strEffectInitPosY;
	float						m_fEffectInitPosY;

	CEdit						m_EffectStartTimeEdit;
	CString						m_strEffectStartTime;
	float						m_fEffectStartTime;


	CEdit						m_EffectSizeXEdit;
	CString						m_strEffectSizeX;
	float						m_fEffectSizeX;

	CEdit						m_EffectSizeYEdit;
	CString						m_strEffectSizeY;
	float						m_fEffectSizeY;

	CEdit						m_EffectMagEdit;
	CString						m_strEffectMag;
	float						m_fEffectMag;

	CComboBox					m_EffectMoveTypeCom;

	CEdit						m_EMoveStartTimeEdit;
	CString						m_strEMoveStartTime;
	float						m_fEMoveStartTime;

	CEdit						m_EMoveTimeEdit;
	CString						m_strEMoveTime;
	float						m_fEMoveTime;

	CEdit						m_EHideTimeEdit;
	CString						m_strEHideTime;
	float						m_fEHideTime;

	CEdit						m_EMoveStartXEdit;
	CString						m_strEMoveStartX;
	float						m_fEMoveStartX;

	CEdit						m_EMoveStartYEdit;
	CString						m_strEMoveStartY;
	float						m_fEMoveStartY;

	CEdit						m_EMoveEndXEdit;
	CString						m_strEMoveEndX;
	float						m_fEMoveEndX;

	CEdit						m_EMoveEndYEdit;
	CString						m_strEMoveEndY;
	float						m_fEMoveEndY;

	// Scene(Camera Effect) ���� ��Ʈ�Ѱ� ������
	CComboBox					m_CamEffectRegCom;
	CComboBox					m_CamEffectTypeCom;
	CComboBox					m_DetailTypeCom;

	CEdit						m_CamEffectStartTime;
	CString						m_strCamEffectStartTime;
	float						m_fCamEffectStartTime;

	CEdit						m_CamEffectTime;
	CString						m_strCamEffectTime;
	float						m_fCamEffectTime;

	CEdit						m_CamSubTime;
	CString						m_strCamSubTime;
	float						m_fCamSubTime;

	CComboBox					m_ZoomPosTypeCom;

	CEdit						m_InitZoomPosX;
	CString						m_strInitZoomPosX;
	float						m_fInitZoomPosX;

	CEdit						m_InitZoomPosY;
	CString						m_strInitZoomPosY;
	float						m_fInitZoomPosY;

	CEdit						m_ZoomNear;
	CString						m_strZoomNear;
	float						m_fZoomNear;

	CEdit						m_ZoomFar;
	CString						m_strZoomFar;
	float						m_fZoomFar;

	SPTR<CSoundPlayer>			m_SkillSpeaker;

public:
	// ��Ʈ�ѹ� ���� �ʱ�ȭ(EditGlobal�� �ʱ�ȭ �������� �۾�)
	void InitActionScriptDlg();
	// ActionScript�� Show �ɶ����� EffectNoCombo Box�� 
	// Effect File�� �о ������Ʈ ����
	void SkillNoComboUpdate();

	void InitCurActionScript();
	void ClickSkillListEvent(const int& _ListIndex);
	void ClearSkillList();
	// ���� ActionScript�� ��ϵ� ������ Reg ComboBox�� ���
	void FillScriptData();

	// reg combo box �� ������ ��� �޺��ڽ��� ������ �ʱ�ȭ
	void InitAllComboBox();
	void InitRegComboBox();
	void InitComboBox();

	// ���� �̸��� ���� Index�� ��ȯ�ϴ� �Լ�
	const int ChangeSoundNameToIndex(const tstring& _SoundName);
	// MoveData -> Character MoveType Index�� ��ȯ
	const int ChnageMoveDataToIndex(const MOVEDATA& _MoveData);

	void UpdateAniTypeCombo(const int& _CharIndex);
	
	// �����Ϳ� Ű�� �������� �۵��Ǵ� �Լ�
	void ChangeEditEvent(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	// Ű�� �������� Time or Pos Value���� Ȯ���ϰ� �ƴѰ�� �ѹ��ϴ� �Լ�
	void ChangeEditString(CEdit* _Edit, CString* _String);
	// �����Ͱ� ��Ŀ�� �ƿ� �Ǿ����� ����� ���ڿ��� Ÿ���� ������ �Ǵ��ϴ� �Լ�
	void CheckTimeString(CEdit& _EditCtrl,CString& _TimeStr,float& _Time);
	void CheckFloatString(CEdit& _EditCtrl, CString& _TimeStr, float& _Value);
	void CheckPosString(CEdit& _EditCtrl, CString& _PosStr, float& _Pos);
	// ������ �ڽ��� Ŭ���� ��� ��ü ������ �ϱ� ���� �Լ�
	BOOL EditBoxClickCheck(MSG* pMsg);
	// ��� ��ư�� Ŭ���Ǿ����� üũ�ϴ� �Լ�
	BOOL CheckButtonEvent(MSG* pMsg);
	const int& GetCurEffectNo();

	
	// MoveRegCom�� ���õǾ����� �ش� MoveData�� TypeComIndex�� ���
	const MOVETYPE GetTypeIndex(const MOVEDATA& _MoveData);

	CComboBox* GetSkillCharCombo();
	CComboBox* GetSoundNameCombo();

	void SoundNameComEvent(const int& _NameIndex);
	void EffectTypeComEvent(const int& _EffectNo);

	// ��� RegComboBox�� �̺�Ʈ �Լ�
	// �ش� �޺� �ڽ��� ����� ���� �Ҽ� ������ ��Ʈ�ѿ� ��ü ����.
	void AniRegComEvent(const int& _RegIndex);
	void SoundRegComEvent(const int& _RegIndex);
	void MoveRegComEvent(const int& _RegIndex);
	void SkillEffectRegComEvent(const int& _RegIndex);
	void MoveTypeComEvent(const int& _ComboIndex);
	void EffectMoveTypeComEvent(const int& _ComboIndex);

	void CamEffectRegComEvent(const int& _RegIndex);
	void CamEffectTypeComEvent(const int& _ComboIndex);
	void SetEarthDetailType();
	void SetBlackOutDetailType();
	void SetFadeEffectDetailType();
	void SetZoomEffectDetailType();
	
	void EnableNotUseCtrl(const int& _CamEffect);

	// ���� Reg�� ���� ���� Index 0 �� ������ ������ ����
	void ClearAnimationRegCombo();
	void ClearSoundRegCombo();
	void ClearMoveRegCombo();
	void ClearSkillEffectRegCombo();
	void ClearCamEffectRegCombo();


	// ��� ��ư �̺�Ʈ �Լ�
	BOOL AnimationAddEvent();
	BOOL AnimationModifyEvent(const int& _ModIndex);
	BOOL AnimationDeleteEvent(const int& _DelIndex);
	

	BOOL SoundAddEvent();
	BOOL SoundModifyEvent(const int& _ModIndex);
	BOOL SoundDeleteEvent(const int& _DelIndex);

	BOOL MoveAddEvent();
	BOOL MoveModifyEvent(const int& _ModIndex);
	BOOL MoveDeleteEvent(const int& _DelIndex);
	
	BOOL SkillEffectAddEvent();
	BOOL SkillEffectModifyEvent(const int& _ModIndex);
	BOOL SkillEffectDeleteEvent(const int& _DelIndex);

	BOOL CamEffectAddEvent();
	BOOL CamEffectModifyEvent(const int& _ModIndex);
	BOOL CamEffectDeleteEfent(const int& _DelIndex);

	BOOL CreateEarthEffect(SCENEEFFECTDATA& _Data);
	BOOL CreateBlackOutEffect(SCENEEFFECTDATA& _Data);
	BOOL CreateFadeEffect(SCENEEFFECTDATA& _Data);
	BOOL CreateZoomEffect(SCENEEFFECTDATA& _Data);

	BOOL ChangeCamEffectDataToIndex(const SCENEEFFECTDATA& _Data,int& _Type, int& _Detail,int& _ZoomPosType);
	BOOL ChangeEarthDataToIndex(const SCENEEFFECTDATA& _Data, int& _Type, int& _Detail);
	BOOL ChangeBlackOutDataToIndex(const SCENEEFFECTDATA& _Data, int& _Type, int& _Detail);
	BOOL ChangeFadeDataToIndex(const SCENEEFFECTDATA& _Data, int& _Type, int& _Detail);
	BOOL ChangeZoomDataToIndex(const SCENEEFFECTDATA& _Data, int& _Type, int& _Detail,int& _ZoomPosType);
	// m_CamEffectTypeCom�� Index -> m_vecCamDetailType�� �ε����� ��ȯ�ϴ� �Լ�
	const int ChangeTypeIndexToVecIndex(const int& _Type);


	void UpdateSkillList();
	const int ListStringToSkillNo(CString& _ListString);

	// ����,����,�ε�,���̺� ��ư �̺�Ʈ
	BOOL CreateSkill();
	BOOL ModifyActionScript(const int& _ListIndex);
	BOOL DeleteSkill(const int& _ListIndex);

	// �ε�, ���̺��� ��� SkillMgr���� ���� �����͸� ���Ͽ� �����Ѵ�.
	BOOL LoadSkillFile();
	BOOL SaveSkillFile();

public:
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	afx_msg void OnDestroy();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnCbnSelchangeCombo1();

	// SKill List Click Msg
	afx_msg void OnLbnSelchangeList1();

	// EditBox  KillFocus Msg
	afx_msg void OnEnKillfocusEdit2();
	afx_msg void OnEnKillfocusAnistarttime();
	afx_msg void OnEnKillfocusSoundstarttime();
	// MoveData KillFocus Msg
	afx_msg void OnEnKillfocusCmovestarttime();
	afx_msg void OnEnKillfocusCmoveendtime();
	afx_msg void OnEnKillfocusCmovehidetime();
	afx_msg void OnEnKillfocusCmovestartposx();
	afx_msg void OnEnKillfocusCmovestartposy();
	afx_msg void OnEnKillfocusCmoveendposx();
	afx_msg void OnEnKillfocusCmoveendposy();
	// SkillEffect KillFocus Msg
	afx_msg void OnEnKillfocusActskilleffectposx();
	afx_msg void OnEnKillfocusActskilleffectposy();
	afx_msg void OnEnKillfocusActskilleffectstarttime();
	afx_msg void OnEnKillfocusActskilleffectsizex();
	afx_msg void OnEnKillfocusActskilleffectsizey();
	afx_msg void OnEnKillfocusActskilleffectmag();
	afx_msg void OnEnKillfocusActskilleffectmovestarttime();
	afx_msg void OnEnKillfocusActskilleffectmovetime();
	afx_msg void OnEnKillfocusActskilleffecthidetime();
	afx_msg void OnEnKillfocusActskilleffectmovestartposx();
	afx_msg void OnEnKillfocusActskilleffectmovestartposy();
	afx_msg void OnEnKillfocusActskilleffectmoveendposx();
	afx_msg void OnEnKillfocusActskilleffectmoveendposx2();
	// Scene(Camera) Effect KillFocus Msg
	afx_msg void OnEnKillfocusSceneeffectstarttime();
	afx_msg void OnEnKillfocusSceneeffectacttime();
	afx_msg void OnEnKillfocusSceneeffectsubtime();
	afx_msg void OnEnKillfocusZoomposx();
	afx_msg void OnEnKillfocusZoomposx2();
	afx_msg void OnEnKillfocusZoomnear();
	afx_msg void OnEnKillfocusZoomfar();

	// RegComboBox & TypeComboBox Select Msg
	afx_msg void OnCbnSelchangeAniregcombo();
	afx_msg void OnCbnSelchangeSoundregcombo();
	afx_msg void OnCbnSelchangeSoundcombo();
	afx_msg void OnCbnSelchangeMoveregcombo();
	afx_msg void OnCbnSelchangeMovecombo();
	afx_msg void OnCbnSelchangeActskilleffectregcombo();
	afx_msg void OnCbnSelchangeActskilleffectmovecombo();
	afx_msg void OnCbnSelchangeActskilleffectcombo();
	afx_msg void OnCbnSelchangeSceneeffectregcombo();
	afx_msg void OnCbnSelchangeSceneeffectcombo();

	// Button Click Msg
	afx_msg void OnBnClickedAniregbutton();
	afx_msg void OnBnClickedAnidelbutton();
	afx_msg void OnBnClickedSoundregbutton();
	afx_msg void OnBnClickedAnisounddelbutton();
	afx_msg void OnBnClickedMoveregbutton();
	afx_msg void OnBnClickedMovedelbutton();
	afx_msg void OnBnClickedActskilleffectregbutton();
	afx_msg void OnBnClickedActskilleffectdelbutton();
	afx_msg void OnBnClickedCameffectregbutton();
	afx_msg void OnBnClickedCameffectdelbutton();
	afx_msg void OnBnClickedActcreatebutton();
	afx_msg void OnBnClickedActdeletebutton();
	afx_msg void OnBnClickedActloadbutton();
	afx_msg void OnBnClickedActsavebutton();
};
