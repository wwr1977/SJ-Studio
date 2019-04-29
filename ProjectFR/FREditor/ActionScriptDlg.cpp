// ActionScriptDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "FREditor.h"
#include "ActionScriptDlg.h"
#include "afxdialogex.h"
#include "SkillMgr.h"
#include "EditEffect.h"
#include "EditEffectMgr.h"

// CActionScriptDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CActionScriptDlg, CDialogEx)

CActionScriptDlg::CActionScriptDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_ACTIONSCRIPT, pParent)
	, m_CurSkillNo(0), m_bInit(FALSE)
	, m_PrevCharNo(MAXBATTLECHARACTER)
	, m_strTotalTime(_T("0.0"))
	, m_strAniStartTime(_T("0.0"))
	, m_strSoundStartTime(_T("0.0"))
	, m_CurActionScript(ACTIONSCRIPT())
	, m_fTotalTime(0.0f),m_fAniStartTime(0.0f), m_fSoundStartTime(0.0f)
	, m_strMoveStartTime(_T("0.0"))
	, m_strMoveTime(_T("0.0"))
	, m_strHideTime(_T("0.0"))
	, m_strMoveStartX(_T("0.0"))
	, m_strMoveStartY(_T("0.0"))
	, m_strMoveEndX(_T("0.0"))
	, m_strMoveEndY(_T("0.0"))
	, m_fMoveStartTime(0.0f), m_fMoveTime(0.0f), m_fHideTime(0.0f)
	, m_fMoveStartX(0.0f), m_fMoveStartY(0.0f)
	, m_fMoveEndX(0.0f), m_fMoveEndY(0.0f)
	, m_strEffectInitPosX(_T("0.0"))
	, m_strEffectInitPosY(_T("0.0"))
	, m_strEffectStartTime(_T("0.0"))
	, m_strEffectSizeX(_T("0.0"))
	, m_strEffectSizeY(_T("0.0"))
	, m_strEffectMag(_T("1.0"))
	, m_strEMoveStartTime(_T("0.0"))
	, m_strEMoveTime(_T("0.0"))
	, m_strEHideTime(_T("0.0"))
	, m_strEMoveStartX(_T("0.0"))
	, m_strEMoveStartY(_T("0.0"))
	, m_strEMoveEndX(_T("0.0"))
	, m_strEMoveEndY(_T("0.0"))
	, m_fEffectInitPosX(0.0f), m_fEffectInitPosY(0.0f), m_fEffectStartTime(0.0f)
	, m_fEffectSizeX(0.0f), m_fEffectSizeY(0.0f), m_fEffectMag(1.0f)
	, m_fEMoveStartTime(0.0f), m_fEMoveTime(0.0f), m_fEHideTime(0.0f)
	, m_fEMoveStartX(0.0f), m_fEMoveStartY(0.0f)
	, m_fEMoveEndX(0.0f), m_fEMoveEndY(0.0f)
	, m_strCamEffectStartTime(_T("0.0"))
	, m_strCamEffectTime(_T("0.0"))
	, m_strCamSubTime(_T("0.0"))
	, m_strInitZoomPosX(_T("0.0"))
	, m_strInitZoomPosY(_T("0.0"))
	, m_strZoomNear(_T("0.0"))
	, m_strZoomFar(_T("0.0"))
	, m_fCamEffectStartTime(0.0f), m_fCamEffectTime(0.0f)
	, m_fCamSubTime(0.0f), m_fInitZoomPosX(0.0f), m_fInitZoomPosY(0.0f)
	, m_fZoomNear(0.0f), m_fZoomFar(0.0f)
	, m_CurEffectNo(0), m_SkillSpeaker(nullptr)
{
	CEditGlobal::ActionScriptDlg = this;
}

CActionScriptDlg::~CActionScriptDlg()
{
}

void CActionScriptDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_ACTSKILLNO, m_CurSkillNo);
	DDX_Control(pDX, IDC_COMBO1, m_SkillCharCom);
	DDX_Control(pDX, IDC_EDIT2, m_TotalTimeEdit);
	DDX_Text(pDX, IDC_EDIT2, m_strTotalTime);
	DDX_Control(pDX, IDC_ANIREGCOMBO, m_AniRegCom);
	DDX_Control(pDX, IDC_ANICOMBO, m_AniTypeCom);
	DDX_Control(pDX, IDC_SOUNDREGCOMBO, m_SoundRegCom);
	DDX_Control(pDX, IDC_SOUNDCOMBO, m_SoundNameCom);
	DDX_Control(pDX, IDC_ANISTARTTIME, m_AniStartTimeEdit);
	DDX_Text(pDX, IDC_ANISTARTTIME, m_strAniStartTime);
	DDX_Control(pDX, IDC_SOUNDSTARTTIME, m_SoundStartTimeEdit);
	DDX_Text(pDX, IDC_SOUNDSTARTTIME, m_strSoundStartTime);
	DDX_Control(pDX, IDC_MOVEREGCOMBO, m_MoveRegCom);
	DDX_Control(pDX, IDC_MOVECOMBO, m_MoveTypeCom);
	DDX_Control(pDX, IDC_CMOVESTARTTIME, m_MoveStartTimeEdit);
	DDX_Text(pDX, IDC_CMOVESTARTTIME, m_strMoveStartTime);
	DDX_Control(pDX, IDC_CMOVEENDTIME, m_MoveTimeEdit);
	DDX_Text(pDX, IDC_CMOVEENDTIME, m_strMoveTime);
	DDX_Control(pDX, IDC_CMOVEHIDETIME, m_HideTimeEdit);
	DDX_Text(pDX, IDC_CMOVEHIDETIME, m_strHideTime);
	DDX_Control(pDX, IDC_CMOVESTARTPOSX, m_MoveStartXEdit);
	DDX_Text(pDX, IDC_CMOVESTARTPOSX, m_strMoveStartX);
	DDX_Control(pDX, IDC_CMOVESTARTPOSY, m_MoveStartYEdit);
	DDX_Text(pDX, IDC_CMOVESTARTPOSY, m_strMoveStartY);
	DDX_Control(pDX, IDC_CMOVEENDPOSX, m_MoveEndXEdit);
	DDX_Text(pDX, IDC_CMOVEENDPOSX, m_strMoveEndX);
	DDX_Control(pDX, IDC_CMOVEENDPOSY, m_MoveEndYEdit);
	DDX_Text(pDX, IDC_CMOVEENDPOSY, m_strMoveEndY);
	DDX_Control(pDX, IDC_ACTSKILLEFFECTREGCOMBO, m_SkillEffectRegCom);
	DDX_Control(pDX, IDC_ACTSKILLEFFECTCOMBO, m_SkillEffectNoCom);
	DDX_Control(pDX, IDC_ACTSKILLEFFECTPOSCOMBO, m_EffectPosTypeCom);
	DDX_Control(pDX, IDC_ACTSKILLEFFECTPOSX, m_EffectInitPosXEdit);
	DDX_Control(pDX, IDC_ACTSKILLEFFECTPOSY, m_EffectInitPosYEdit);
	DDX_Text(pDX, IDC_ACTSKILLEFFECTPOSX, m_strEffectInitPosX);
	DDX_Text(pDX, IDC_ACTSKILLEFFECTPOSY, m_strEffectInitPosY);
	DDX_Control(pDX, IDC_ACTSKILLEFFECTSTARTTIME, m_EffectStartTimeEdit);
	DDX_Text(pDX, IDC_ACTSKILLEFFECTSTARTTIME, m_strEffectStartTime);
	DDX_Control(pDX, IDC_ACTSKILLEFFECTSIZEX, m_EffectSizeXEdit);
	DDX_Text(pDX, IDC_ACTSKILLEFFECTSIZEX, m_strEffectSizeX);
	DDX_Control(pDX, IDC_ACTSKILLEFFECTSIZEY, m_EffectSizeYEdit);
	DDX_Text(pDX, IDC_ACTSKILLEFFECTSIZEY, m_strEffectSizeY);
	DDX_Control(pDX, IDC_ACTSKILLEFFECTMAG, m_EffectMagEdit);
	DDX_Text(pDX, IDC_ACTSKILLEFFECTMAG, m_strEffectMag);
	DDX_Control(pDX, IDC_ACTSKILLEFFECTMOVECOMBO, m_EffectMoveTypeCom);
	DDX_Control(pDX, IDC_ACTSKILLEFFECTMOVESTARTTIME, m_EMoveStartTimeEdit);
	DDX_Text(pDX, IDC_ACTSKILLEFFECTMOVESTARTTIME, m_strEMoveStartTime);
	DDX_Control(pDX, IDC_ACTSKILLEFFECTMOVETIME, m_EMoveTimeEdit);
	DDX_Text(pDX, IDC_ACTSKILLEFFECTMOVETIME, m_strEMoveTime);
	DDX_Control(pDX, IDC_ACTSKILLEFFECTHIDETIME, m_EHideTimeEdit);
	DDX_Text(pDX, IDC_ACTSKILLEFFECTHIDETIME, m_strEHideTime);
	DDX_Control(pDX, IDC_ACTSKILLEFFECTMOVESTARTPOSX, m_EMoveStartXEdit);
	DDX_Text(pDX, IDC_ACTSKILLEFFECTMOVESTARTPOSX, m_strEMoveStartX);
	DDX_Control(pDX, IDC_ACTSKILLEFFECTMOVESTARTPOSY, m_EMoveStartYEdit);
	DDX_Text(pDX, IDC_ACTSKILLEFFECTMOVESTARTPOSY, m_strEMoveStartY);
	DDX_Control(pDX, IDC_ACTSKILLEFFECTMOVEENDPOSX, m_EMoveEndXEdit);
	DDX_Text(pDX, IDC_ACTSKILLEFFECTMOVEENDPOSX, m_strEMoveEndX);
	DDX_Control(pDX, IDC_ACTSKILLEFFECTMOVEENDPOSX2, m_EMoveEndYEdit);
	DDX_Text(pDX, IDC_ACTSKILLEFFECTMOVEENDPOSX2, m_strEMoveEndY);
	DDX_Control(pDX, IDC_SCENEEFFECTREGCOMBO, m_CamEffectRegCom);
	DDX_Control(pDX, IDC_SCENEEFFECTCOMBO, m_CamEffectTypeCom);
	DDX_Control(pDX, IDC_DETAILSCENEEFFECTCOMBO, m_DetailTypeCom);
	DDX_Control(pDX, IDC_SCENEEFFECTSTARTTIME, m_CamEffectStartTime);
	DDX_Text(pDX, IDC_SCENEEFFECTSTARTTIME, m_strCamEffectStartTime);
	DDX_Control(pDX, IDC_SCENEEFFECTACTTIME, m_CamEffectTime);
	DDX_Text(pDX, IDC_SCENEEFFECTACTTIME, m_strCamEffectTime);
	DDX_Control(pDX, IDC_SCENEEFFECTSUBTIME, m_CamSubTime);
	DDX_Text(pDX, IDC_SCENEEFFECTSUBTIME, m_strCamSubTime);
	DDX_Control(pDX, IDC_ZOOMPOSCOMBO, m_ZoomPosTypeCom);
	DDX_Control(pDX, IDC_ZOOMPOSX, m_InitZoomPosX);
	DDX_Text(pDX, IDC_ZOOMPOSX, m_strInitZoomPosX);
	DDX_Control(pDX, IDC_ZOOMPOSX2, m_InitZoomPosY);
	DDX_Text(pDX, IDC_ZOOMPOSX2, m_strInitZoomPosY);
	DDX_Control(pDX, IDC_ZOOMNEAR, m_ZoomNear);
	DDX_Text(pDX, IDC_ZOOMNEAR, m_strZoomNear);
	DDX_Control(pDX, IDC_ZOOMFAR, m_ZoomFar);
	DDX_Text(pDX, IDC_ZOOMFAR, m_strZoomFar);
	DDX_Control(pDX, IDC_LIST1, m_ActSkillList);
}


BEGIN_MESSAGE_MAP(CActionScriptDlg, CDialogEx)
	ON_WM_DESTROY()
	ON_WM_SHOWWINDOW()
	ON_CBN_SELCHANGE(IDC_COMBO1, &CActionScriptDlg::OnCbnSelchangeCombo1)
	ON_EN_KILLFOCUS(IDC_EDIT2, &CActionScriptDlg::OnEnKillfocusEdit2)
	ON_EN_KILLFOCUS(IDC_ANISTARTTIME, &CActionScriptDlg::OnEnKillfocusAnistarttime)
	ON_EN_KILLFOCUS(IDC_SOUNDSTARTTIME, &CActionScriptDlg::OnEnKillfocusSoundstarttime)
	ON_CBN_SELCHANGE(IDC_ANIREGCOMBO, &CActionScriptDlg::OnCbnSelchangeAniregcombo)
	ON_BN_CLICKED(IDC_ANIREGBUTTON, &CActionScriptDlg::OnBnClickedAniregbutton)
	ON_BN_CLICKED(IDC_ANIDELBUTTON, &CActionScriptDlg::OnBnClickedAnidelbutton)
	ON_BN_CLICKED(IDC_SOUNDREGBUTTON, &CActionScriptDlg::OnBnClickedSoundregbutton)
	ON_BN_CLICKED(IDC_ANISOUNDDELBUTTON, &CActionScriptDlg::OnBnClickedAnisounddelbutton)
	ON_CBN_SELCHANGE(IDC_SOUNDREGCOMBO, &CActionScriptDlg::OnCbnSelchangeSoundregcombo)
	ON_EN_KILLFOCUS(IDC_CMOVESTARTTIME, &CActionScriptDlg::OnEnKillfocusCmovestarttime)
	ON_EN_KILLFOCUS(IDC_CMOVEENDTIME, &CActionScriptDlg::OnEnKillfocusCmoveendtime)
	ON_EN_KILLFOCUS(IDC_CMOVEHIDETIME, &CActionScriptDlg::OnEnKillfocusCmovehidetime)
	ON_EN_KILLFOCUS(IDC_CMOVESTARTPOSX, &CActionScriptDlg::OnEnKillfocusCmovestartposx)
	ON_EN_KILLFOCUS(IDC_CMOVESTARTPOSY, &CActionScriptDlg::OnEnKillfocusCmovestartposy)
	ON_EN_KILLFOCUS(IDC_CMOVEENDPOSX, &CActionScriptDlg::OnEnKillfocusCmoveendposx)
	ON_EN_KILLFOCUS(IDC_CMOVEENDPOSY, &CActionScriptDlg::OnEnKillfocusCmoveendposy)
	ON_BN_CLICKED(IDC_MOVEREGBUTTON, &CActionScriptDlg::OnBnClickedMoveregbutton)
	ON_BN_CLICKED(IDC_MOVEDELBUTTON, &CActionScriptDlg::OnBnClickedMovedelbutton)
	ON_CBN_SELCHANGE(IDC_MOVEREGCOMBO, &CActionScriptDlg::OnCbnSelchangeMoveregcombo)
	ON_CBN_SELCHANGE(IDC_MOVECOMBO, &CActionScriptDlg::OnCbnSelchangeMovecombo)
	ON_EN_KILLFOCUS(IDC_ACTSKILLEFFECTPOSX, &CActionScriptDlg::OnEnKillfocusActskilleffectposx)
	ON_EN_KILLFOCUS(IDC_ACTSKILLEFFECTPOSY, &CActionScriptDlg::OnEnKillfocusActskilleffectposy)
	ON_EN_KILLFOCUS(IDC_ACTSKILLEFFECTSTARTTIME, &CActionScriptDlg::OnEnKillfocusActskilleffectstarttime)
	ON_EN_KILLFOCUS(IDC_ACTSKILLEFFECTSIZEX, &CActionScriptDlg::OnEnKillfocusActskilleffectsizex)
	ON_EN_KILLFOCUS(IDC_ACTSKILLEFFECTSIZEY, &CActionScriptDlg::OnEnKillfocusActskilleffectsizey)
	ON_EN_KILLFOCUS(IDC_ACTSKILLEFFECTMAG, &CActionScriptDlg::OnEnKillfocusActskilleffectmag)
	ON_EN_KILLFOCUS(IDC_ACTSKILLEFFECTMOVESTARTTIME, &CActionScriptDlg::OnEnKillfocusActskilleffectmovestarttime)
	ON_EN_KILLFOCUS(IDC_ACTSKILLEFFECTMOVETIME, &CActionScriptDlg::OnEnKillfocusActskilleffectmovetime)
	ON_EN_KILLFOCUS(IDC_ACTSKILLEFFECTHIDETIME, &CActionScriptDlg::OnEnKillfocusActskilleffecthidetime)
	ON_EN_KILLFOCUS(IDC_ACTSKILLEFFECTMOVESTARTPOSX, &CActionScriptDlg::OnEnKillfocusActskilleffectmovestartposx)
	ON_EN_KILLFOCUS(IDC_ACTSKILLEFFECTMOVESTARTPOSY, &CActionScriptDlg::OnEnKillfocusActskilleffectmovestartposy)
	ON_EN_KILLFOCUS(IDC_ACTSKILLEFFECTMOVEENDPOSX, &CActionScriptDlg::OnEnKillfocusActskilleffectmoveendposx)
	ON_EN_KILLFOCUS(IDC_ACTSKILLEFFECTMOVEENDPOSX2, &CActionScriptDlg::OnEnKillfocusActskilleffectmoveendposx2)
	ON_CBN_SELCHANGE(IDC_ACTSKILLEFFECTMOVECOMBO, &CActionScriptDlg::OnCbnSelchangeActskilleffectmovecombo)
	ON_CBN_SELCHANGE(IDC_ACTSKILLEFFECTREGCOMBO, &CActionScriptDlg::OnCbnSelchangeActskilleffectregcombo)
	ON_BN_CLICKED(IDC_ACTSKILLEFFECTREGBUTTON, &CActionScriptDlg::OnBnClickedActskilleffectregbutton)
	ON_BN_CLICKED(IDC_ACTSKILLEFFECTDELBUTTON, &CActionScriptDlg::OnBnClickedActskilleffectdelbutton)
	ON_EN_KILLFOCUS(IDC_SCENEEFFECTSTARTTIME, &CActionScriptDlg::OnEnKillfocusSceneeffectstarttime)
	ON_EN_KILLFOCUS(IDC_SCENEEFFECTACTTIME, &CActionScriptDlg::OnEnKillfocusSceneeffectacttime)
	ON_EN_KILLFOCUS(IDC_SCENEEFFECTSUBTIME, &CActionScriptDlg::OnEnKillfocusSceneeffectsubtime)
	ON_EN_KILLFOCUS(IDC_ZOOMPOSX, &CActionScriptDlg::OnEnKillfocusZoomposx)
	ON_EN_KILLFOCUS(IDC_ZOOMPOSX2, &CActionScriptDlg::OnEnKillfocusZoomposx2)
	ON_EN_KILLFOCUS(IDC_ZOOMNEAR, &CActionScriptDlg::OnEnKillfocusZoomnear)
	ON_EN_KILLFOCUS(IDC_ZOOMFAR, &CActionScriptDlg::OnEnKillfocusZoomfar)
	ON_BN_CLICKED(IDC_CAMEFFECTREGBUTTON, &CActionScriptDlg::OnBnClickedCameffectregbutton)
	ON_BN_CLICKED(IDC_CAMEFFECTDELBUTTON, &CActionScriptDlg::OnBnClickedCameffectdelbutton)
	ON_CBN_SELCHANGE(IDC_SCENEEFFECTCOMBO, &CActionScriptDlg::OnCbnSelchangeSceneeffectcombo)
	ON_CBN_SELCHANGE(IDC_SCENEEFFECTREGCOMBO, &CActionScriptDlg::OnCbnSelchangeSceneeffectregcombo)
	ON_CBN_SELCHANGE(IDC_SOUNDCOMBO, &CActionScriptDlg::OnCbnSelchangeSoundcombo)
	ON_CBN_SELCHANGE(IDC_ACTSKILLEFFECTCOMBO, &CActionScriptDlg::OnCbnSelchangeActskilleffectcombo)
	ON_BN_CLICKED(IDC_ACTCREATEBUTTON, &CActionScriptDlg::OnBnClickedActcreatebutton)
	ON_BN_CLICKED(IDC_ACTDELETEBUTTON, &CActionScriptDlg::OnBnClickedActdeletebutton)
	ON_BN_CLICKED(IDC_ACTLOADBUTTON, &CActionScriptDlg::OnBnClickedActloadbutton)
	ON_BN_CLICKED(IDC_ACTSAVEBUTTON, &CActionScriptDlg::OnBnClickedActsavebutton)
	ON_LBN_SELCHANGE(IDC_LIST1, &CActionScriptDlg::OnLbnSelchangeList1)
END_MESSAGE_MAP()


// CActionScriptDlg 메시지 처리기입니다.
void CActionScriptDlg::InitActionScriptDlg()
{
	for (size_t i = 0; i < CEditGlobal::CharacterName.size(); i++)
	{
		m_SkillCharCom.AddString(CEditGlobal::CharacterName[i]);
	}

	m_SkillCharCom.EnableWindow(FALSE);
	m_SoundNameCom.EnableWindow(FALSE);
	CEditGlobal::SkillSoundLoading(m_vecSkillSound);


	for (size_t i = 0; i < m_vecSkillSound.size(); i++)
	{
		CString AddString = _T("");
		AddString.Format(_T("%d: %s"), i, m_vecSkillSound[i].c_str());
		m_SoundNameCom.AddString(AddString);
	}

	CActObject* Speaker = CActObject::CreateActObject(_T("MgrSpeaker"), CEditGlobal::SkillScene);
	m_SkillSpeaker = Speaker->CreateCom<CSoundPlayer>();

}
void CActionScriptDlg::SkillNoComboUpdate()
{
	m_SkillEffectNoCom.ResetContent();

	CGameFile LoadFile(_T("EditData"), _T("EffectData.bin"));

	if (FALSE == LoadFile.Open(_T("rb")))
	{
		MessageBox(_T("EffectDataFile을 열지 못하였습니다."));
		return ;
	}


	vector<EFFECTDATA> TempVec;
	TempVec.reserve(128);
	EFFECTDATA::LoadDataToFile(LoadFile.GetFile(), &TempVec);

	CString AddEffectString = _T("");
	
	for (size_t i = 0; i < TempVec.size(); i++)
	{
		AddEffectString.Format(_T("%3d: %s"), TempVec[i].EffectIndex, TempVec[i].EffectName);
		m_SkillEffectNoCom.AddString(AddEffectString);
	}
}
void CActionScriptDlg::InitCurActionScript()
{	
	if (nullptr == CEditGlobal::EditSkillMgr)
		return;

	UpdateData(TRUE);
	m_CurSkillNo = CEditGlobal::EditSkillMgr->SearchNextIndex();
	UpdateData(FALSE);
	m_CurActionScript = ACTIONSCRIPT();
	m_CurActionScript.SkillNo = m_CurSkillNo;
	m_fTotalTime = m_CurActionScript.TotalSkillTime;
	UpdateData(TRUE);
	m_strTotalTime.Format(_T("%.2f"), m_fTotalTime);
	UpdateData(FALSE);
	InitAllComboBox();

	CEditGlobal::EditSkillMgr->CurCharacterOff();
	CEditGlobal::EditSkillMgr->SkillStandby(false);
	m_SkillCharCom.EnableWindow(TRUE);
}
void CActionScriptDlg::ClickSkillListEvent(const int& _ListIndex)
{
	CString ListString;
	
	UpdateData(TRUE);
	m_ActSkillList.GetText(_ListIndex, ListString);
	UpdateData(FALSE);
	
	int SkillNo = ListStringToSkillNo(ListString);

	SKILL Temp = CEditGlobal::EditSkillMgr->GetSkill(SkillNo);

	/*if (true == Temp.SkillActionScript.isEmpty())
		return;
*/
	m_CurActionScript = Temp.SkillActionScript;
	InitAllComboBox();
	FillScriptData();
	UpdateData(TRUE);
	m_CurSkillNo = m_CurActionScript.SkillNo;
	UpdateData(FALSE);
	m_fTotalTime = m_CurActionScript.TotalSkillTime;
	UpdateData(TRUE);
	m_strTotalTime.Format(_T("%.2f"), m_fTotalTime);
	UpdateData(FALSE);
	if (true == Temp.SkillData.CharacterSkill)
		m_SkillCharCom.SetCurSel(m_CurActionScript.CharIndex);
	else
		m_SkillCharCom.SetCurSel(-1);
	OnCbnSelchangeCombo1();
	CEditGlobal::EditSkillMgr->SetSkill(m_CurSkillNo, 0);
	
	UpdateData(FALSE);
}
void CActionScriptDlg::ClearSkillList()
{
	m_ActSkillList.ResetContent();
	m_ActSkillList.AddString(_T("새로 만들기"));
}
void CActionScriptDlg::FillScriptData()
{
	for (UINT i = 0; i < m_CurActionScript.AniCount; ++i)
	{
		int AniIndex = m_CurActionScript.AniSctipt[i].AnimationIndex;
		float Time = m_CurActionScript.AniSctipt[i].StartTime;

		CString AddRegString = _T("");
		AddRegString.Format(_T("%2d: %.2f"), AniIndex, Time);
		m_AniRegCom.AddString(AddRegString);
	}

	for (UINT i = 0; i < m_CurActionScript.MoveCount; ++i)
	{
		int MoveIndex = ChnageMoveDataToIndex(m_CurActionScript.CharMoveData[i]);
		float Time = m_CurActionScript.CharMoveData[i].MoveStartTime;

		CString AddRegString = _T("");
		AddRegString.Format(_T("%2d: %.2f"), MoveIndex, Time);
		m_MoveRegCom.AddString(AddRegString);
	}

	for (UINT i = 0; i < m_CurActionScript.CharSoundCount; ++i)
	{
		int SoundIndex = ChangeSoundNameToIndex(m_CurActionScript.CharSound[i].SoundName);
		float Time = m_CurActionScript.CharSound[i].PlayStartTime;

		CString AddRegString = _T("");
		AddRegString.Format(_T("%2d: %.2f"), SoundIndex, Time);
		m_SoundRegCom.AddString(AddRegString);
	}

	for (UINT i = 0; i < m_CurActionScript.EffectCount; i++)
	{
		int EffectNo = m_CurActionScript.SkillEffect[i].EffectIndex;
		float Time = m_CurActionScript.SkillEffect[i].StartTime;

		CString AddRegString = _T("");
		AddRegString.Format(_T("%2d: %.2f"), EffectNo, Time);
		m_SkillEffectRegCom.AddString(AddRegString);
	}

	for (UINT i = 0; i < m_CurActionScript.SceneEffectCount; i++)
	{
		int Type, Detail, ZoomMode;
		ChangeCamEffectDataToIndex(m_CurActionScript.SceneEffectData[i], Type, Detail, ZoomMode);
		float Time = m_CurActionScript.SkillEffect[i].StartTime;

		CString AddRegString = _T("");
		AddRegString.Format(_T("(%d,%d): %.2f"), Type, Detail, Time);
		m_CamEffectRegCom.AddString(AddRegString);
	}

}
void CActionScriptDlg::InitAllComboBox()
{
	InitRegComboBox();
	InitComboBox();
}
void CActionScriptDlg::InitRegComboBox()
{
	UpdateData(TRUE);
	ClearAnimationRegCombo();
	ClearSoundRegCombo();
	ClearMoveRegCombo();
	ClearSkillEffectRegCombo();
	ClearCamEffectRegCombo();

	m_AniRegCom.SetCurSel(0);
	m_SoundRegCom.SetCurSel(0);
	m_MoveRegCom.SetCurSel(0);
	m_SkillEffectRegCom.SetCurSel(0);
	m_CamEffectRegCom.SetCurSel(0);
	UpdateData(FALSE);
}
void CActionScriptDlg::InitComboBox()
{
	UpdateData(TRUE);
	m_SkillCharCom.SetCurSel(-1);
	m_PrevCharNo = MAXBATTLECHARACTER;
	m_AniTypeCom.ResetContent();
	m_AniTypeCom.SetCurSel(-1);
	m_SoundNameCom.SetCurSel(-1);
	m_MoveTypeCom.SetCurSel(-1);
	m_SkillEffectNoCom.SetCurSel(-1);
	m_EffectPosTypeCom.SetCurSel(-1);
	m_EffectMoveTypeCom.SetCurSel(-1);
	m_CamEffectTypeCom.SetCurSel(-1);
	m_DetailTypeCom.SetCurSel(-1);
	m_ZoomPosTypeCom.SetCurSel(-1);
	UpdateData(FALSE);
}
const int CActionScriptDlg::ChangeSoundNameToIndex(const tstring& _SoundName)
{
	if (nullptr == &_SoundName)
		return -1;

	for (int i = 0; i < (int)m_vecSkillSound.size(); i++)
	{
		if (_SoundName == m_vecSkillSound[i])
			return i;
	}
	return -1;

}
const int CActionScriptDlg::ChnageMoveDataToIndex(const MOVEDATA& _MoveData)
{
	if (nullptr == &_MoveData)
		return MAXMOVEDATATYPE;

	if (true == _MoveData.bSetTeamPos)
		return TEAMPOS;

	if (true == _MoveData.bSetInitPos)
		return INITPOS;
	
	if (_MoveData.MoveMode == MOVEDATA::MOVEMODE_MOVE)
		return MOVE;
	else if (_MoveData.MoveMode == MOVEDATA::MOVEMODE_HIDE)
		return HIDE;
	else if (_MoveData.MoveMode == MOVEDATA::MOVEMODE_SHOW)
		return SHOW;

	return MAXMOVEDATATYPE;
}
void CActionScriptDlg::UpdateAniTypeCombo(const int& _CharIndex)
{
	// 더블어택 가능 캐릭터의 타입은 2개더 많다.

	UpdateData(TRUE);
	m_AniTypeCom.ResetContent();

	if (_CharIndex <= CharacterIndex::Vargas) 
	{
		for (size_t i = 0; i < m_vecAnimationName.size(); i++)
			m_AniTypeCom.AddString(m_vecAnimationName[i]);
	}
	else 
	{
		for (size_t i = 0; i <= NODOUBLEATK; i++)
			m_AniTypeCom.AddString(m_vecAnimationName[i]);
	}
	UpdateData(FALSE);

}
void CActionScriptDlg::ChangeEditEvent(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	unordered_map<UINT, EditSet>::iterator Find = m_mapEditSet.find(nID);

	// EditSetMap 에서 해당 Edit컨트롤을 찾지못할경우 함수 종료
	if (Find == m_mapEditSet.end())
		return;
	
	ChangeEditString(Find->second.EditCtrl, Find->second.EditString);
	
}
void CActionScriptDlg::ChangeEditString(CEdit* _Edit, CString* _String)
{
	if (nullptr == _Edit || nullptr == _String)
		return;

	
	int Sel = _Edit->GetSel();
	CEditGlobal::ChangeFloatString(_String);
	_Edit->SetSel(Sel);
	
}
void CActionScriptDlg::CheckTimeString(CEdit& _EditCtrl, CString& _TimeStr, float& _Time)
{
	if (nullptr == &_EditCtrl || nullptr == &_TimeStr || nullptr == &_Time)
		return;

	UpdateData(TRUE);
	float Time = CGameString::StringToFloat(_TimeStr.GetString());

	if (Time < 0.0f)
	{
		MessageBox(_T("입력한 값이 음수 입니다."));
		_EditCtrl.SetFocus();
		_EditCtrl.SetSel(0, -1, true);
		SendMessage(_EditCtrl.GetDlgCtrlID(), WM_KILLFOCUS, NULL);
		return;
	}
	else
	{
		_Time = Time;
		_TimeStr.Format(_T("%.2f"), _Time);
	}
	UpdateData(FALSE);
}
void CActionScriptDlg::CheckFloatString(CEdit& _EditCtrl, CString& _TimeStr, float& _Value)
{
	if (nullptr == &_EditCtrl || nullptr == &_TimeStr || nullptr == &_Value)
		return;

	UpdateData(TRUE);
	float Time = CGameString::StringToFloat(_TimeStr.GetString());

	_Value = Time;
	_TimeStr.Format(_T("%.2f"), _Value);
	
	UpdateData(FALSE);
}
void CActionScriptDlg::CheckPosString(CEdit& _EditCtrl, CString& _PosStr, float& _Pos)
{
	if (nullptr == &_EditCtrl || nullptr == &_PosStr || nullptr == &_Pos)
		return;

	UpdateData(TRUE);

	float Pos = CGameString::StringToFloat(_PosStr.GetString());

	_Pos = Pos;
	_PosStr.Format(_T("%.2f"), _Pos);
	
	UpdateData(FALSE);
}
BOOL CActionScriptDlg::EditBoxClickCheck(MSG* pMsg)
{

	unordered_map<UINT, EditSet>::iterator Start = m_mapEditSet.begin();
	unordered_map<UINT, EditSet>::iterator End = m_mapEditSet.end();

	for ( ; Start!= End; ++Start)
	{
		if (pMsg->hwnd == Start->second.EditCtrl->m_hWnd
			&& GetFocus()->m_hWnd != Start->second.EditCtrl->m_hWnd)
		{
			Start->second.EditCtrl->SetFocus();
			Start->second.EditCtrl->SetSel(0, -1, true);
			return TRUE;
		}
	}
	return FALSE;
}
BOOL CActionScriptDlg::CheckButtonEvent(MSG* pMsg)
{	
	if (pMsg->hwnd == GetDlgItem(IDC_ANIREGBUTTON)->m_hWnd) 
	{
		OnBnClickedAniregbutton();
		return TRUE;
	}
	if (pMsg->hwnd == GetDlgItem(IDC_ANIDELBUTTON)->m_hWnd) 
	{
		OnBnClickedAnidelbutton();
		return TRUE;
	}
	if (pMsg->hwnd == GetDlgItem(IDC_SOUNDREGBUTTON)->m_hWnd)
	{
		OnBnClickedSoundregbutton();
		return TRUE;
	}
	if (pMsg->hwnd == GetDlgItem(IDC_SOUNDDELBUTTON)->m_hWnd) 
	{
		OnBnClickedAnisounddelbutton();
		return TRUE;
	}
	if (pMsg->hwnd == GetDlgItem(IDC_MOVEREGBUTTON)->m_hWnd)
	{
		OnBnClickedMoveregbutton();
		return TRUE;
	}
	if (pMsg->hwnd == GetDlgItem(IDC_MOVEDELBUTTON)->m_hWnd)
	{
		OnBnClickedMovedelbutton();
		return TRUE;
	}
	if (pMsg->hwnd == GetDlgItem(IDC_ACTSKILLEFFECTREGBUTTON)->m_hWnd) 
	{
		OnBnClickedActskilleffectregbutton();
		return TRUE;
	}
	if (pMsg->hwnd == GetDlgItem(IDC_ACTSKILLEFFECTDELBUTTON)->m_hWnd)
	{
		OnBnClickedActskilleffectdelbutton();
		return TRUE;
	}
	if (pMsg->hwnd == GetDlgItem(IDC_CAMEFFECTREGBUTTON)->m_hWnd)
	{
		OnBnClickedCameffectregbutton();
		return TRUE;
	}
	if (pMsg->hwnd == GetDlgItem(IDC_CAMEFFECTDELBUTTON)->m_hWnd)
	{
		OnBnClickedCameffectdelbutton();
		return TRUE;
	}
	if (pMsg->hwnd == GetDlgItem(IDC_ACTCREATEBUTTON)->m_hWnd)
	{
		OnBnClickedActcreatebutton();
		return TRUE;
	}
	if (pMsg->hwnd == GetDlgItem(IDC_ACTDELETEBUTTON)->m_hWnd)
	{
		OnBnClickedActdeletebutton();
		return TRUE;
	}
	if (pMsg->hwnd == GetDlgItem(IDC_ACTLOADBUTTON)->m_hWnd)
	{
		OnBnClickedActloadbutton();
		return TRUE;
	}
	if (pMsg->hwnd == GetDlgItem(IDC_ACTSAVEBUTTON)->m_hWnd)
	{
		OnBnClickedActsavebutton();
		return TRUE;
	}


	return FALSE;
}
const int& CActionScriptDlg::GetCurEffectNo()
{
	return m_CurEffectNo;
}
const CActionScriptDlg::MOVETYPE CActionScriptDlg::GetTypeIndex(const MOVEDATA& _MoveData)
{
	if (nullptr == &_MoveData)
		return MAXMOVEDATATYPE;

	if (_MoveData.bSetTeamPos)
		return TEAMPOS;

	if (_MoveData.bSetInitPos)
		return INITPOS;

	switch (_MoveData.MoveMode)
	{
	case MOVEDATA::MOVEMODE_MOVE:
		return MOVE;
	case MOVEDATA::MOVEMODE_HIDE:
		return HIDE;
	case MOVEDATA::MOVEMODE_SHOW:
		return SHOW;
	}
	return MAXMOVEDATATYPE;
}
CComboBox* CActionScriptDlg::GetSkillCharCombo()
{
	return &m_SkillCharCom;
}
CComboBox* CActionScriptDlg::GetSoundNameCombo()
{
	return &m_SoundNameCom;
}
void CActionScriptDlg::SoundNameComEvent(const int& _NameIndex)
{
	if (_NameIndex >= m_vecSkillSound.size())
		return;

	if (nullptr != m_SkillSpeaker)
		m_SkillSpeaker->PlaySound(m_vecSkillSound[_NameIndex]);
}
void CActionScriptDlg::EffectTypeComEvent(const int& _EffectNo)
{
	m_CurEffectNo = _EffectNo;

	/*if (nullptr != CEditGlobal::EffectMgr)
		CEditGlobal::EffectMgr->EffectOn(_EffectNo);*/
}
void CActionScriptDlg::AniRegComEvent(const int& _RegIndex)
{
	UpdateData(TRUE);
	if (_RegIndex < 0 )
	{
		m_AniRegCom.SetCurSel(0);
		m_fAniStartTime = 0.0f;
		m_strAniStartTime.Format(_T("%.2f"), m_fAniStartTime);
	}
	else if((int)m_CurActionScript.AniCount > _RegIndex)
	{
		int AniIndex = m_CurActionScript.AniSctipt[_RegIndex].AnimationIndex;

		if (AniIndex >= m_AniTypeCom.GetCount())
			return;

		float StartTime = m_CurActionScript.AniSctipt[_RegIndex].StartTime;

		m_fAniStartTime = StartTime;
		m_strAniStartTime.Format(_T("%.2f"), m_fAniStartTime);
		m_AniTypeCom.SetCurSel(AniIndex);
	}
	UpdateData(FALSE);
}
void CActionScriptDlg::SoundRegComEvent(const int& _RegIndex)
{
	UpdateData(TRUE);
	if (_RegIndex < 0)
	{
		m_SoundRegCom.SetCurSel(0);
		m_fSoundStartTime = 0.0f;
		m_strSoundStartTime.Format(_T("%.2f"), m_fSoundStartTime);
	}
	else if ((int)m_CurActionScript.CharSoundCount > _RegIndex)
	{
		float StartTime = m_CurActionScript.CharSound[_RegIndex].PlayStartTime;

		tstring SoundName = m_CurActionScript.CharSound[_RegIndex].SoundName;

		UINT SoundNameIndex = -1;

		for (int i = 0; i < (int)m_vecSkillSound.size(); ++i)
		{
			if (m_CurActionScript.CharSound[_RegIndex].SoundName == m_vecSkillSound[i])
			{
				SoundNameIndex = i;
				break;
			}
		}

		if (SoundNameIndex >= (UINT)m_SoundNameCom.GetCount())
			return;

		m_fSoundStartTime = StartTime;
		m_strSoundStartTime.Format(_T("%.2f"), m_fSoundStartTime);
		m_SoundNameCom.SetCurSel(SoundNameIndex);
	}
	UpdateData(FALSE);

}
void CActionScriptDlg::MoveRegComEvent(const int& _RegIndex)
{

	UpdateData(TRUE);
	if (_RegIndex < 0)
	{
		m_MoveRegCom.SetCurSel(0);
		m_fMoveStartTime = 0.0f;
		m_strMoveStartTime.Format(_T("%.2f"), m_fMoveStartTime);
		m_fMoveTime = 0.0f;
		m_strMoveTime.Format(_T("%.2f"), m_fMoveTime);
		m_fHideTime = 0.0f;
		m_strHideTime.Format(_T("%.2f"), m_fHideTime);
		m_fMoveStartX = 0.0f;
		m_strMoveStartX.Format(_T("%.2f"), m_fMoveStartX);
		m_fMoveStartY = 0.0f;
		m_strMoveStartY.Format(_T("%.2f"), m_fMoveStartY);
		m_fMoveEndX = 0.0f;
		m_strMoveEndX.Format(_T("%.2f"), m_fMoveEndX);
		m_fMoveEndY = 0.0f;
		m_strMoveEndY.Format(_T("%.2f"), m_fMoveEndY);
	}
	else if ((int)m_CurActionScript.MoveCount > _RegIndex)
	{
		MOVEDATA MoveData(m_CurActionScript.CharMoveData[_RegIndex]);

		MOVETYPE TypeIndex = GetTypeIndex(MoveData);

		if (TypeIndex < MAXMOVEDATATYPE)
			m_MoveTypeCom.SetCurSel(TypeIndex);

		MoveTypeComEvent(TypeIndex);

		m_fMoveStartTime = MoveData.MoveStartTime;
		m_strMoveStartTime.Format(_T("%.2f"), m_fMoveStartTime);
		m_fMoveTime = MoveData.MoveTime;
		m_strMoveTime.Format(_T("%.2f"), m_fMoveTime);
		m_fHideTime = MoveData.HideTime;
		m_strHideTime.Format(_T("%.2f"), m_fHideTime);
		m_fMoveStartX = MoveData.MoveStartPos.x;
		m_strMoveStartX.Format(_T("%.2f"), m_fMoveStartX);
		m_fMoveStartY = MoveData.MoveStartPos.y;
		m_strMoveStartY.Format(_T("%.2f"), m_fMoveStartY);
		m_fMoveEndX = MoveData.MoveEndPos.x;
		m_strMoveEndX.Format(_T("%.2f"), m_fMoveEndX);
		m_fMoveEndY = MoveData.MoveEndPos.y;
		m_strMoveEndY.Format(_T("%.2f"), m_fMoveEndY);
	}
	UpdateData(FALSE);
}
void CActionScriptDlg::MoveTypeComEvent(const int& _ComboIndex)
{
	// 같은 타입을 선택한 경우 이벤트처리를 무시한다.
	if (_ComboIndex < 0 || _ComboIndex >= m_MoveTypeCom.GetCount())
		return;


	// 무브데이터와 관련된 컨트롤중 ComboInex에 영향을 받는 모든 컨트롤은 켜둔다.
	m_MoveTimeEdit.EnableWindow(TRUE);
	m_HideTimeEdit.EnableWindow(TRUE);
	m_MoveStartXEdit.EnableWindow(TRUE);
	m_MoveStartYEdit.EnableWindow(TRUE);
	m_MoveEndXEdit.EnableWindow(TRUE);
	m_MoveEndYEdit.EnableWindow(TRUE);

	switch ((MOVETYPE)_ComboIndex)
	{
	case CActionScriptDlg::MOVE: 
	{
		m_HideTimeEdit.EnableWindow(FALSE);
	}
	break;
	case CActionScriptDlg::TEAMPOS:
	case CActionScriptDlg::INITPOS: 
	{
		m_MoveTimeEdit.EnableWindow(FALSE);
		m_HideTimeEdit.EnableWindow(FALSE);
		m_MoveStartXEdit.EnableWindow(FALSE);
		m_MoveStartYEdit.EnableWindow(FALSE);
		m_MoveEndXEdit.EnableWindow(FALSE);
		m_MoveEndYEdit.EnableWindow(FALSE);
	}
	break;
	case CActionScriptDlg::HIDE:
	case CActionScriptDlg::SHOW: 
	{
		m_MoveTimeEdit.EnableWindow(FALSE);
		m_MoveStartXEdit.EnableWindow(FALSE);
		m_MoveStartYEdit.EnableWindow(FALSE);
		m_MoveEndXEdit.EnableWindow(FALSE);
		m_MoveEndYEdit.EnableWindow(FALSE);
	}
		break;
	}

}
void CActionScriptDlg::SkillEffectRegComEvent(const int& _RegIndex)
{
	UpdateData(TRUE);
	if (_RegIndex < 0)
	{
		m_SkillEffectRegCom.SetCurSel(0);
		m_fEffectInitPosX = 0.0f;
		m_strEffectInitPosX.Format(_T("%.2f"), m_fEffectInitPosX);
		m_fEffectInitPosY = 0.0f;
		m_strEffectInitPosY.Format(_T("%.2f"), m_fEffectInitPosY);
		m_fEffectStartTime = 0.0f;
		m_strEffectStartTime.Format(_T("%.2f"), m_fEffectStartTime);
		m_fEffectSizeX = 0.0f;
		m_strEffectSizeX.Format(_T("%.2f"), m_fEffectSizeX);
		m_fEffectSizeY = 0.0f;
		m_strEffectSizeY.Format(_T("%.2f"), m_fEffectSizeY);
		m_fEffectMag = 1.0f;
		m_strEffectMag.Format(_T("%.2f"), m_fEffectMag);
		m_fEMoveStartTime = 0.0f;
		m_strEMoveStartTime.Format(_T("%.2f"), m_fEMoveStartTime);
		m_fEMoveTime = 0.0f;
		m_strEMoveTime.Format(_T("%.2f"), m_fEMoveTime);
		m_fEHideTime = 0.0f;
		m_strEHideTime.Format(_T("%.2f"), m_fEHideTime);
		m_fEMoveStartX = 0.0f;
		m_strEMoveStartX.Format(_T("%.2f"), m_fEMoveStartX);
		m_fEMoveStartY = 0.0f;
		m_strEMoveStartY.Format(_T("%.2f"), m_fEMoveStartY);
		m_fEMoveEndX = 0.0f;
		m_strEMoveEndX.Format(_T("%.2f"), m_fEMoveEndX);
		m_fEMoveEndY = 0.0f;
		m_strEMoveEndY.Format(_T("%.2f"), m_fEMoveEndY);
	}
	else if ((int)m_CurActionScript.EffectCount > _RegIndex)
	{
		SKILLEFFECTDATA Data(m_CurActionScript.SkillEffect[_RegIndex]);

		if (Data.EffectIndex < (UINT)m_SkillEffectNoCom.GetCount())
			m_SkillEffectNoCom.SetCurSel(Data.EffectIndex);

		int PosType = Data.InitPosMode;

		if (PosType < m_EffectPosTypeCom.GetCount())
			m_EffectPosTypeCom.SetCurSel(PosType);

		int MoveType = Data.MoveData.MoveMode;

		//MoveTime이 0인 경우 움직이지 않는 이펙트이다.
		if (MoveType == MOVEDATA::MOVEMODE_MOVE && Data.MoveData.MoveTime <= 0.0f)
		{
			m_EffectMoveTypeCom.SetCurSel(0);
			EffectMoveTypeComEvent(0);
		}
		else if (MoveType + 1 < m_EffectMoveTypeCom.GetCount())
		{
			m_EffectMoveTypeCom.SetCurSel(MoveType + 1);
			EffectMoveTypeComEvent(MoveType + 1);
		}

		m_fEffectInitPosX = Data.InitPos.x;
		m_strEffectInitPosX.Format(_T("%.2f"), m_fEffectInitPosX);
		m_fEffectInitPosY = Data.InitPos.y;
		m_strEffectInitPosY.Format(_T("%.2f"), m_fEffectInitPosY);
		m_fEffectStartTime = Data.StartTime;
		m_strEffectStartTime.Format(_T("%.2f"), m_fEffectStartTime);
		m_fEffectSizeX = Data.EffectCustomSize.x;
		m_strEffectSizeX.Format(_T("%.2f"), m_fEffectSizeX);
		m_fEffectSizeY = Data.EffectCustomSize.y;
		m_strEffectSizeY.Format(_T("%.2f"), m_fEffectSizeY);
		m_fEffectMag = Data.EffectImageMag;
		m_strEffectMag.Format(_T("%.2f"), m_fEffectMag);
		m_fEMoveStartTime = Data.MoveData.MoveStartTime;
		m_strEMoveStartTime.Format(_T("%.2f"), m_fEMoveStartTime);
		m_fEMoveTime = Data.MoveData.MoveTime;
		m_strEMoveTime.Format(_T("%.2f"), m_fEMoveTime);
		m_fEHideTime = Data.MoveData.HideTime;
		m_strEHideTime.Format(_T("%.2f"), m_fEHideTime);
		m_fEMoveStartX = Data.MoveData.MoveStartPos.x;
		m_strEMoveStartX.Format(_T("%.2f"), m_fEMoveStartX);
		m_fEMoveStartY = Data.MoveData.MoveStartPos.y;
		m_strEMoveStartY.Format(_T("%.2f"), m_fEMoveStartY);
		m_fEMoveEndX = Data.MoveData.MoveEndPos.x;
		m_strEMoveEndX.Format(_T("%.2f"), m_fEMoveEndX);
		m_fEMoveEndY = Data.MoveData.MoveEndPos.y;
		m_strEMoveEndY.Format(_T("%.2f"), m_fEMoveEndY);
	}
	UpdateData(FALSE);
}
void CActionScriptDlg::EffectMoveTypeComEvent(const int& _ComboIndex) 
{
	if (_ComboIndex < 0 || _ComboIndex >= m_EffectMoveTypeCom.GetCount())
		return;
	
	m_EMoveStartTimeEdit.EnableWindow(FALSE);
	m_EMoveTimeEdit.EnableWindow(FALSE);
	m_EHideTimeEdit.EnableWindow(FALSE);
	m_EMoveStartXEdit.EnableWindow(FALSE);
	m_EMoveStartYEdit.EnableWindow(FALSE);
	m_EMoveEndXEdit.EnableWindow(FALSE);
	m_EMoveEndYEdit.EnableWindow(FALSE);

	switch (_ComboIndex)
	{
	case EMOVE :
	{
		m_EMoveStartTimeEdit.EnableWindow(TRUE);
		m_EMoveTimeEdit.EnableWindow(TRUE);
		m_EMoveStartXEdit.EnableWindow(TRUE);
		m_EMoveStartYEdit.EnableWindow(TRUE);
		m_EMoveEndXEdit.EnableWindow(TRUE);
		m_EMoveEndYEdit.EnableWindow(TRUE);
	}
	break;
	case EHIDE:
	case ESHOW:
	{
		m_EMoveStartTimeEdit.EnableWindow(TRUE);
		m_EHideTimeEdit.EnableWindow(TRUE);
	}
	break;
	}
}

void CActionScriptDlg::CamEffectRegComEvent(const int& _RegIndex)
{
	UpdateData(TRUE);
	if (_RegIndex < 0) 
	{
		m_CamEffectRegCom.SetCurSel(0);
		m_fCamEffectStartTime = 0.0f;
		m_strCamEffectStartTime.Format(_T("%.2f"),m_fCamEffectStartTime);
		m_fCamEffectTime = 0.0f;
		m_strCamEffectTime.Format(_T("%.2f"), m_fCamEffectTime);
		m_fCamSubTime = 0.0f;
		m_strCamSubTime.Format(_T("%.2f"), m_fCamSubTime);
		m_fInitZoomPosX = 0.0f;
		m_strInitZoomPosX.Format(_T("%.2f"), m_fInitZoomPosX);
		m_fInitZoomPosY = 0.0f;
		m_strInitZoomPosY.Format(_T("%.2f"), m_fInitZoomPosY);
		m_fZoomNear = 0.0f;
		m_strZoomNear.Format(_T("%.2f"), m_fZoomNear);
		m_fZoomFar = 0.0f;
		m_strZoomFar.Format(_T("%.2f"), m_fZoomFar);
	}
	else if (m_CurActionScript.SceneEffectCount > (UINT)_RegIndex)
	{
		SCENEEFFECTDATA Data = m_CurActionScript.SceneEffectData[_RegIndex];
		int Type = 0;
		int Detail = 0;
		int ZoomPosType = -1;

		if (FALSE == ChangeCamEffectDataToIndex(Data, Type, Detail, ZoomPosType))
		{
			MessageBox(_T("특수효과 데이터가 깨져 있습니다."));
			return;
		}

		if (Type < m_CamEffectTypeCom.GetCount())
		{
			m_CamEffectTypeCom.SetCurSel(Type);
			CamEffectTypeComEvent(Type);
		}

		if (Detail < m_DetailTypeCom.GetCount())
			m_DetailTypeCom.SetCurSel(Detail);

		if (-1 != ZoomPosType)
		{
			if (ZoomPosType < m_ZoomPosTypeCom.GetCount())
				m_ZoomPosTypeCom.SetCurSel(ZoomPosType);
		}

		m_fCamEffectStartTime = Data.SceneEffectStartTime;
		m_strCamEffectStartTime.Format(_T("%.2f"), m_fCamEffectStartTime);
		m_fCamEffectTime = Data.SceneEffectTime;
		m_strCamEffectTime.Format(_T("%.2f"), m_fCamEffectTime);
		m_fCamSubTime = Data.SubTime;
		m_strCamSubTime.Format(_T("%.2f"), m_fCamSubTime);
		m_fInitZoomPosX = Data.ZoomPos.x;
		m_strInitZoomPosX.Format(_T("%.2f"), m_fInitZoomPosX);
		m_fInitZoomPosY = Data.ZoomPos.y;
		m_strInitZoomPosY.Format(_T("%.2f"), m_fInitZoomPosY);
		
		float Near = min(Data.ZoomRatio.x, Data.ZoomRatio.y);
		float Far = max(Data.ZoomRatio.x, Data.ZoomRatio.y);

		m_fZoomNear = Near;
		m_strZoomNear.Format(_T("%.2f"), m_fZoomNear);
		m_fZoomFar = Far;
		m_strZoomFar.Format(_T("%.2f"), m_fZoomFar);
	}
	UpdateData(FALSE);
}
void CActionScriptDlg::CamEffectTypeComEvent(const int& _ComboIndex)
{
	if (_ComboIndex < 0 || _ComboIndex >= m_CamEffectTypeCom.GetCount())
		return;

	m_DetailTypeCom.ResetContent();

	m_CamSubTime.EnableWindow(TRUE);
	m_InitZoomPosX.EnableWindow(TRUE);
	m_InitZoomPosY.EnableWindow(TRUE);
	m_ZoomNear.EnableWindow(TRUE);
	m_ZoomFar.EnableWindow(TRUE);
	m_ZoomPosTypeCom.EnableWindow(TRUE);


	switch (_ComboIndex)
	{
	case CActionScriptDlg::EARTH_XAXIS:
	case CActionScriptDlg::EARTH_YAXIS:
	case CActionScriptDlg::EARTH_RECT:
	case CActionScriptDlg::EARTH_CIRCLE:
		return SetEarthDetailType();
	case CActionScriptDlg::BLACKOUT_FLASH:
		return SetBlackOutDetailType();
	case CActionScriptDlg::FADE_IN:
	case CActionScriptDlg::FADE_OUT:
		return SetFadeEffectDetailType();
	case CActionScriptDlg::ZOOMEFFECT:
		return SetZoomEffectDetailType();
	}
	return;
}
void CActionScriptDlg::SetEarthDetailType()
{
	EnableNotUseCtrl(EARTH);

	for (size_t i = 0; i < m_vecCamDetailType[EARTH].size(); i++)
	{
		m_DetailTypeCom.AddString(m_vecCamDetailType[EARTH][i]);
	}
}
void CActionScriptDlg::SetBlackOutDetailType()
{
	EnableNotUseCtrl(BLACKOUT);
	for (size_t i = 0; i < m_vecCamDetailType[BLACKOUT].size(); i++)
	{
		m_DetailTypeCom.AddString(m_vecCamDetailType[BLACKOUT][i]);
	}
}
void CActionScriptDlg::SetFadeEffectDetailType()
{
	EnableNotUseCtrl(FADE);
	for (size_t i = 0; i < m_vecCamDetailType[FADE].size(); i++)
	{
		m_DetailTypeCom.AddString(m_vecCamDetailType[FADE][i]);
	}
}
void CActionScriptDlg::SetZoomEffectDetailType() 
{
	EnableNotUseCtrl(ZOOM);
	for (size_t i = 0; i < m_vecCamDetailType[ZOOM].size(); i++)
	{
		m_DetailTypeCom.AddString(m_vecCamDetailType[ZOOM][i]);
	}
	
	m_ZoomPosTypeCom.SetCurSel(0);
}
void CActionScriptDlg::EnableNotUseCtrl(const int& _CamEffect)
{
	if (nullptr == &_CamEffect)
		return;

	switch (_CamEffect)
	{
	case EARTH:
	case BLACKOUT:
	{
		m_CamSubTime.EnableWindow(FALSE);
		m_InitZoomPosX.EnableWindow(FALSE);
		m_InitZoomPosY.EnableWindow(FALSE);
		m_ZoomNear.EnableWindow(FALSE);
		m_ZoomFar.EnableWindow(FALSE);
		m_ZoomPosTypeCom.EnableWindow(FALSE);

		UpdateData(TRUE);
		m_fCamSubTime = 0.0f;
		m_strCamSubTime.Format(_T("%.2f"), m_fCamSubTime);
		m_fEffectInitPosX = 0.0f;
		m_strEffectInitPosX.Format(_T("%.2f"), m_fEffectInitPosX);
		m_fEffectInitPosY = 0.0f;
		m_strEffectInitPosX.Format(_T("%.2f"), m_fEffectInitPosY);
		m_fZoomNear = 0.0f;
		m_strZoomNear.Format(_T("%.2f"), m_fZoomNear);
		m_fZoomFar = 0.0f;
		m_strZoomNear.Format(_T("%.2f"), m_fZoomFar);
		UpdateData(FALSE);
	}
		return;
	case FADE:
	{
		m_InitZoomPosX.EnableWindow(FALSE);
		m_InitZoomPosY.EnableWindow(FALSE);
		m_ZoomNear.EnableWindow(FALSE);
		m_ZoomFar.EnableWindow(FALSE);
		m_ZoomPosTypeCom.EnableWindow(FALSE);

		UpdateData(TRUE);
		m_fEffectInitPosX = 0.0f;
		m_strEffectInitPosX.Format(_T("%.2f"), m_fEffectInitPosX);
		m_fEffectInitPosY = 0.0f;
		m_strEffectInitPosX.Format(_T("%.2f"), m_fEffectInitPosY);
		m_fZoomNear = 0.0f;
		m_strZoomNear.Format(_T("%.2f"), m_fZoomNear);
		m_fZoomFar = 0.0f;
		m_strZoomNear.Format(_T("%.2f"), m_fZoomFar);
		UpdateData(FALSE);
	}
		return;
	case ZOOM:
		return;
	}

	return;
}
void CActionScriptDlg::ClearAnimationRegCombo()
{
	while (1 < m_AniRegCom.GetCount()) 
	{
		m_AniRegCom.DeleteString(1);
	}
}
void CActionScriptDlg::ClearSoundRegCombo()
{

	while (1 < m_SoundRegCom.GetCount())
	{
		m_SoundRegCom.DeleteString(1);
	}

}
void CActionScriptDlg::ClearMoveRegCombo()
{
	while (1 < m_MoveRegCom.GetCount())
	{
		m_MoveRegCom.DeleteString(1);
	}

}
void CActionScriptDlg::ClearSkillEffectRegCombo()
{
	while (1 < m_SkillEffectRegCom.GetCount())
	{
		m_SkillEffectRegCom.DeleteString(1);
	}

}
void CActionScriptDlg::ClearCamEffectRegCombo()
{
	for (UINT i = (UINT)m_CamEffectRegCom.GetCount() - 1 ; i >= 1; --i)
	{
		m_CamEffectRegCom.DeleteString(i);
	}

	if (1 != m_CamEffectRegCom.GetCount())
		assert(nullptr);
}
BOOL CActionScriptDlg::AnimationAddEvent()
{
	if (m_CurActionScript.AniCount >= ACTIONSCRIPT::MAXANISCRIPTCOUNT) 
	{
		MessageBox(_T("더 이상 애니메이션을 지정할 수 없습니다."), _T("ActionScriptDlg"), MB_ICONWARNING);
		//SendMessageA(GetDlgCtrlID(NOFOCUS)->m_hWnd, WM_SETFOCUS, NULL, NULL);
		return FALSE;
	}

	int AniIndex = m_AniTypeCom.GetCurSel();

	if (AniIndex < 0 || AniIndex >= m_AniTypeCom.GetCount())
		return FALSE;

	if (m_fAniStartTime < 0)
		return FALSE;

	ANIMATIONSCRIPT  AddAniData((ANIMATIONNAME)AniIndex, m_fAniStartTime);
	if (true == m_CurActionScript.AddAniScript(AddAniData)) 
	{
		CString AddRegString = _T("");
		AddRegString.Format(_T("%2d: %.2f"), AniIndex, m_fAniStartTime);
		int Index = m_AniRegCom.AddString(AddRegString);
		m_AniRegCom.SetCurSel(0);
	}
	
	return TRUE;
}
BOOL CActionScriptDlg::AnimationModifyEvent(const int& _ModIndex)
{
	if (_ModIndex < 0 || (int)m_CurActionScript.AniCount <= _ModIndex)
		return FALSE;

	int AniIndex = m_AniTypeCom.GetCurSel();

	if (AniIndex < 0 || AniIndex >= m_AniTypeCom.GetCount())
		return FALSE;

	if (m_fAniStartTime < 0)
		return FALSE;

	m_CurActionScript.AniSctipt[_ModIndex].AnimationIndex = AniIndex;
	m_CurActionScript.AniSctipt[_ModIndex].StartTime = m_fAniStartTime;

	
	ClearAnimationRegCombo();

	for (size_t i = 0; i < m_CurActionScript.AniCount; ++i)
	{
		int  AniType = m_CurActionScript.AniSctipt[i].AnimationIndex;
		float StartTime = m_CurActionScript.AniSctipt[i].StartTime;
		CString AddRegString = _T("");

		AddRegString.Format(_T("%2d: %.2f"), AniType, StartTime);
		m_AniRegCom.AddString(AddRegString);
	}
	m_AniRegCom.SetCurSel(_ModIndex + 1);

	return TRUE;
}
BOOL CActionScriptDlg::AnimationDeleteEvent(const int& _DelIndex)
{
	if (_DelIndex >= (int)m_CurActionScript.AniCount)
	{
		MessageBox(_T("애니메이션을 제거할 수 없습니다."), _T("ActionScriptDlg"), MB_ICONWARNING);
		return FALSE;
	}

	// 성공적으로 제거를 했으면 등록 콤보박스의 내용을 수정한다.

	if (true == m_CurActionScript.DeleteAniScript(_DelIndex))
	{
		m_AniRegCom.DeleteString(_DelIndex + 1);
	}

	AniRegComEvent(NOREGINDEX);
	return TRUE;
}
BOOL CActionScriptDlg::SoundAddEvent()
{
	if (m_CurActionScript.CharSoundCount >= ACTIONSCRIPT::MAXCHARSOUNDCOUNT)
	{
		MessageBox(_T("더 이상 음성 & 효과음을 지정할 수 없습니다."), _T("ActionScriptDlg"), MB_ICONWARNING);
		//SendMessageA(GetDlgCtrlID(NOFOCUS)->m_hWnd, WM_SETFOCUS, NULL, NULL);
		return FALSE;
	}

	int SoundNameIndex = m_SoundNameCom.GetCurSel();

	if (SoundNameIndex < 0 || SoundNameIndex >= m_SoundNameCom.GetCount() || SoundNameIndex >= m_vecSkillSound.size())
		return FALSE;

	if (m_fSoundStartTime < 0)
		return FALSE;

	
	SOUNDSAVEDATA  AddSoundData(m_vecSkillSound[SoundNameIndex], m_fSoundStartTime);
	if (true == m_CurActionScript.AddCharSoundData(AddSoundData)) 
	{
		CString AddRegString = _T("");
		AddRegString.Format(_T("%2d: %.2f"), SoundNameIndex, m_fSoundStartTime);
		int Index = m_SoundRegCom.AddString(AddRegString);
		m_SoundRegCom.SetCurSel(0);
	}

	return TRUE;
}
BOOL CActionScriptDlg::SoundModifyEvent(const int& _ModIndex)
{
	if (_ModIndex < 0 || (int)m_CurActionScript.CharSoundCount <= _ModIndex)
		return FALSE;

	int SoundNameIndex = m_SoundNameCom.GetCurSel();

	if (SoundNameIndex < 0 || SoundNameIndex >= m_SoundNameCom.GetCount() || SoundNameIndex >= m_vecSkillSound.size())
		return FALSE;

	if (m_fSoundStartTime < 0)
		return FALSE;

	m_CurActionScript.CharSound[_ModIndex].SetSoundName(m_vecSkillSound[SoundNameIndex]);
	m_CurActionScript.CharSound[_ModIndex].PlayStartTime = m_fSoundStartTime;
	
	ClearSoundRegCombo();

	for (size_t i = 0; i < m_CurActionScript.CharSoundCount; ++i)
	{
		int SoundIndex = ChangeSoundNameToIndex(m_CurActionScript.CharSound[i].SoundName);
		float StartTime = m_CurActionScript.CharSound[i].PlayStartTime;

		CString AddRegString = _T("");
		AddRegString.Format(_T("%2d: %.2f"), SoundIndex, StartTime);
		m_SoundRegCom.AddString(AddRegString);
	}
	m_SoundRegCom.SetCurSel(_ModIndex + 1);
	return TRUE;
}
BOOL CActionScriptDlg::SoundDeleteEvent(const int& _DelIndex)
{
	if (_DelIndex >= (int)m_CurActionScript.CharSoundCount)
	{
		MessageBox(_T("음성 & 효과음을 제거할 수 없습니다."), _T("ActionScriptDlg"), MB_ICONWARNING);
		return FALSE;
	}

	// 성공적으로 제거를 했으면 등록 콤보박스의 내용을 수정한다.

	if (true == m_CurActionScript.DeleteCharSoundData(_DelIndex))
	{
		m_SoundRegCom.DeleteString(_DelIndex + 1);
	}

	SoundRegComEvent(NOREGINDEX);
	return TRUE;
}
BOOL CActionScriptDlg::MoveAddEvent() 
{
	if (m_CurActionScript.MoveCount >= ACTIONSCRIPT::MAXMOVECOUNT)
	{
		MessageBox(_T("더 이상 MoveData를 지정할 수 없습니다."), _T("ActionScriptDlg"), MB_ICONWARNING);
		return FALSE;
	}

	MOVEDATA AddMoveData = MOVEDATA();

	int TypeIndex = m_MoveTypeCom.GetCurSel();

	switch (TypeIndex)
	{
	case CActionScriptDlg::MOVE: 
	{
		if (m_fMoveTime < 0.0f)
			return FALSE;
		AddMoveData.SetMoveData(m_fMoveStartTime, m_fMoveTime, Vec2{ m_fMoveStartX,m_fMoveStartY }, Vec2{ m_fMoveEndX,m_fMoveEndY });
	}
	break;
	case CActionScriptDlg::TEAMPOS: 
		AddMoveData.SetTeamPos(m_fMoveStartTime);
		break;
	case CActionScriptDlg::INITPOS:
		AddMoveData.SetInitPos(m_fMoveStartTime);
		break;
	case CActionScriptDlg::HIDE: 
	{
		if (m_fHideTime < 0.0f)
			return FALSE;
		AddMoveData.SetHideData(m_fMoveStartTime, m_fHideTime);
	}
	break;
	case CActionScriptDlg::SHOW: 
	{
		if (m_fHideTime < 0.0f)
			return FALSE;
		AddMoveData.SetShowData(m_fMoveStartTime, m_fHideTime);
	}
	break;
	}

	if (true == m_CurActionScript.AddCharMoveData(AddMoveData)) 
	{
		CString AddRegString = _T("");
		AddRegString.Format(_T("%2d: %.2f"), TypeIndex, m_fMoveStartTime);
	
		int Index = m_MoveRegCom.AddString(AddRegString);
		m_MoveRegCom.SetCurSel(0);
	}

	return TRUE;
}
BOOL CActionScriptDlg::MoveModifyEvent(const int& _ModIndex)
{
	if (_ModIndex < 0 || (int)m_CurActionScript.MoveCount <= _ModIndex)
		return FALSE;

	int TypeIndex = m_MoveTypeCom.GetCurSel();

	switch (TypeIndex)
	{
	case CActionScriptDlg::MOVE:
	{
		if (m_fMoveStartTime < 0 || m_fMoveTime <= 0.0f)
			return FALSE;
		m_CurActionScript.CharMoveData[_ModIndex].SetMoveData(m_fMoveStartTime, m_fMoveTime, Vec2{ m_fMoveStartX,m_fMoveStartY }, Vec2{ m_fMoveEndX,m_fMoveEndY });
	}
	break;
	case CActionScriptDlg::TEAMPOS:
		m_CurActionScript.CharMoveData[_ModIndex].SetTeamPos(m_fMoveStartTime);
		break;
	case CActionScriptDlg::INITPOS:
		m_CurActionScript.CharMoveData[_ModIndex].SetInitPos(m_fMoveStartTime);
		break;
	case CActionScriptDlg::HIDE:
	{
		if (m_fMoveStartTime < 0 || m_fHideTime <= 0.0f)
			return FALSE;
		m_CurActionScript.CharMoveData[_ModIndex].SetHideData(m_fMoveStartTime, m_fHideTime);
	}
	break;
	case CActionScriptDlg::SHOW:
	{
		if (m_fMoveStartTime < 0 || m_fHideTime <= 0.0f)
			return FALSE;
		m_CurActionScript.CharMoveData[_ModIndex].SetShowData(m_fMoveStartTime, m_fHideTime);
	}
	break;
	}

	ClearMoveRegCombo();

	for (int  i = 0; i < (int)m_CurActionScript.MoveCount; i++)
	{
		int MoveIndex = ChnageMoveDataToIndex(m_CurActionScript.CharMoveData[i]);
		float StartTime = m_CurActionScript.CharMoveData[i].MoveStartTime;

		CString AddRegString = _T("");
		AddRegString.Format(_T("%2d: %.2f"), MoveIndex, StartTime);
		m_MoveRegCom.AddString(AddRegString);
	}

	m_MoveRegCom.SetCurSel(_ModIndex + 1);
	return TRUE;
}
BOOL CActionScriptDlg::MoveDeleteEvent(const int& _DelIndex)
{
	if (_DelIndex >= (int)m_CurActionScript.MoveCount )
	{
		MessageBox(_T("MoveData를 제거할 수 없습니다."), _T("ActionScriptDlg"), MB_ICONWARNING);
		return FALSE;
	}

	// 성공적으로 제거를 했으면 등록 콤보박스의 내용을 수정한다.

	if (true == m_CurActionScript.DeleteCharMoveData(_DelIndex))
	{
		m_MoveRegCom.DeleteString(_DelIndex + 1 );
	}

	MoveRegComEvent(NOREGINDEX);
	return TRUE;

}
BOOL CActionScriptDlg::SkillEffectAddEvent()
{
	if (m_CurActionScript.EffectCount >= ACTIONSCRIPT::MAXEFFECTCOUNT)
	{
		MessageBox(_T("더 이상 스킬 이펙트를 지정할 수 없습니다."), _T("ActionScriptDlg"), MB_ICONWARNING);
		return FALSE;
	}

	// 시간 변수가 0 이하일경우 데이터 생성 X
	if (m_fEffectStartTime < 0 )
		return FALSE;

	SKILLEFFECTDATA TempData;
	int EffectNo = m_SkillEffectNoCom.GetCurSel();

	if (EffectNo >= m_SkillEffectNoCom.GetCount())
		return FALSE;

	int InitPosType = m_EffectPosTypeCom.GetCurSel();

	if (InitPosType >= m_EffectPosTypeCom.GetCount())
		return FALSE;


	int EffectMoveType = m_EffectMoveTypeCom.GetCurSel();

	if (EffectMoveType >= m_EffectMoveTypeCom.GetCount())
		return FALSE;

	TempData.EffectIndex = EffectNo;

	switch (InitPosType)
	{
	case SKILLEFFECTDATA::INITPOS_CUSTOM:
		TempData.SetInitPos(Vec2{ m_fEffectInitPosX,m_fEffectInitPosY });
		break;
	case SKILLEFFECTDATA::INITPOS_CASTERPOS:
		TempData.SetInitPos(InitPosType, Vec2{ m_fEffectInitPosX,m_fEffectInitPosY });
	break;
	}

	TempData.StartTime = m_fEffectStartTime;

	if (m_fEffectSizeX > 0 && m_fEffectSizeY > 0)
		TempData.EffectCustomSize = Vec2{ m_fEffectSizeX ,m_fEffectSizeY };
	
	TempData.EffectImageMag = m_fEffectMag;

	switch (EffectMoveType)
	{
	case EMOVE:
		if (0 >= m_fEMoveTime)
		{
			MessageBox(_T("MoveTime이 0 입니다."), _T("ActionScriptDlg"), MB_ICONWARNING);
			return FALSE;
		}
		TempData.MoveData.SetMoveData(m_fEMoveStartTime, m_fEMoveTime, Vec2{ m_fEMoveStartX,m_fEMoveStartY }, Vec2{ m_fEMoveEndX,m_fEMoveEndY });
		break;
	case EHIDE:
		if (0 >= m_fEHideTime)
		{
			MessageBox(_T("HideTime이 0 입니다."), _T("ActionScriptDlg"), MB_ICONWARNING);
			return FALSE;
		}
		TempData.MoveData.SetHideData(m_fEMoveStartTime, m_fEHideTime);
		break;
	case ESHOW:
		if (0 >= m_fEHideTime) 
		{
			MessageBox(_T("HideTime이 0 입니다."), _T("ActionScriptDlg"), MB_ICONWARNING);
			return FALSE;
		}
		TempData.MoveData.SetShowData(m_fEMoveStartTime, m_fEHideTime);
		break;
	}

	if (true == m_CurActionScript.AddSkillEffectData(TempData) )
	{
		CString AddRegString = _T("");
		AddRegString.Format(_T("%2d: %.2f"), TempData.EffectIndex, TempData.StartTime);

		int Index = m_SkillEffectRegCom.AddString(AddRegString);
		m_SkillEffectRegCom.SetCurSel(0);

		return TRUE;
	}

	// 추가에 실패하였으므로 FALSE를 리턴
	return FALSE;
}
BOOL CActionScriptDlg::SkillEffectModifyEvent(const int& _ModIndex)
{
	if (_ModIndex < 0 || (int)m_CurActionScript.EffectCount <= _ModIndex)
		return FALSE;

	if (m_fEffectStartTime < 0)
		return FALSE;

	int EffectNo = m_SkillEffectNoCom.GetCurSel();

	if (EffectNo >= m_SkillEffectNoCom.GetCount())
		return FALSE;

	int InitPosType = m_EffectPosTypeCom.GetCurSel();

	if (InitPosType >= m_EffectPosTypeCom.GetCount())
		return FALSE;


	int EffectMoveType = m_EffectMoveTypeCom.GetCurSel();

	if (EffectMoveType >= m_EffectMoveTypeCom.GetCount())
		return FALSE;

	SKILLEFFECTDATA TempData;
	TempData.EffectIndex = EffectNo;

	switch (InitPosType)
	{
	case SKILLEFFECTDATA::INITPOS_CUSTOM:
		TempData.SetInitPos(Vec2{ m_fEffectInitPosX,m_fEffectInitPosY });
		break;
	case SKILLEFFECTDATA::INITPOS_CASTERPOS:
		TempData.SetInitPos(InitPosType, Vec2{ m_fEffectInitPosX,m_fEffectInitPosY });
		break;
	}

	TempData.StartTime = m_fEffectStartTime;

	if (m_fEffectSizeX > 0 && m_fEffectSizeY > 0)
		TempData.EffectCustomSize = Vec2{ m_fEffectSizeX ,m_fEffectSizeY };

	TempData.EffectImageMag = m_fEffectMag;

	switch (EffectMoveType)
	{
	case EMOVE:
		if (0 >= m_fEMoveTime)
		{
			MessageBox(_T("MoveTime이 0보다 작습니다."), _T("ActionScriptDlg"), MB_ICONWARNING);
			return FALSE;
		}
		TempData.MoveData.SetMoveData(m_fEMoveStartTime, m_fEMoveTime, Vec2{ m_fEMoveStartX,m_fEMoveStartY }, Vec2{ m_fEMoveEndX,m_fEMoveEndY });
		break;
	case EHIDE:
		if (0 >= m_fEHideTime)
		{
			MessageBox(_T("HideTime이 0보다 작습니다."), _T("ActionScriptDlg"), MB_ICONWARNING);
			return FALSE;
		}
		TempData.MoveData.SetHideData(m_fEMoveStartTime, m_fEHideTime);
		break;
	case ESHOW:
		if (0 >= m_fEHideTime)
		{
			MessageBox(_T("HideTime이 0보다 작습니다."), _T("ActionScriptDlg"), MB_ICONWARNING);
			return FALSE;
		}
		TempData.MoveData.SetShowData(m_fEMoveStartTime, m_fEHideTime);
		break;
	}

	m_CurActionScript.SkillEffect[_ModIndex] = TempData;

	ClearSkillEffectRegCombo();
	for (int i = 0; i < (int)m_CurActionScript.EffectCount; i++)
	{
		int EffectNo = m_CurActionScript.SkillEffect[i].EffectIndex;
		float StartTime = m_CurActionScript.SkillEffect[i].StartTime;
		CString AddRegString = _T("");
		AddRegString.Format(_T("%2d: %.2f"), EffectNo, StartTime);

		int Index = m_SkillEffectRegCom.AddString(AddRegString);
	}

	m_SkillEffectRegCom.SetCurSel(_ModIndex + 1);
	return TRUE;
}
BOOL CActionScriptDlg::SkillEffectDeleteEvent(const int& _DelIndex)
{
	if (_DelIndex >= (int)m_CurActionScript.EffectCount)
	{
		MessageBox(_T("스킬 이펙트를 제거할 수 없습니다."), _T("ActionScriptDlg"), MB_ICONWARNING);
		return FALSE;
	}

	// 성공적으로 제거를 했으면 등록 콤보박스의 내용을 수정한다.

	if (true == m_CurActionScript.DeleteSkillEffectData(_DelIndex))
	{
		m_SkillEffectRegCom.DeleteString(_DelIndex + 1);
	}
	
	SkillEffectRegComEvent(NOREGINDEX);
	return TRUE;

}

BOOL CActionScriptDlg::CamEffectAddEvent() 
{
	if (m_CurActionScript.SceneEffectCount >= ACTIONSCRIPT::MAXSCENEEFFECTCOUNT) 
	{
		MessageBox(_T("더 이상 특수효과를 지정할 수 없습니다."), _T("ActionScriptDlg"), MB_ICONWARNING);
		return FALSE;
	}

	int CamEffectType = m_CamEffectTypeCom.GetCurSel();
	SCENEEFFECTDATA Temp;
	BOOL Check = FALSE;

	switch (CamEffectType)
	{
	case CActionScriptDlg::EARTH_XAXIS:
	case CActionScriptDlg::EARTH_YAXIS:
	case CActionScriptDlg::EARTH_RECT:
	case CActionScriptDlg::EARTH_CIRCLE: 
	{
		Check = CreateEarthEffect(Temp);
	}
	break;
	case CActionScriptDlg::BLACKOUT_FLASH: 
	{
		Check = CreateBlackOutEffect(Temp);
	}
	break;
	case CActionScriptDlg::FADE_IN:
	case CActionScriptDlg::FADE_OUT: 
	{
		Check = CreateFadeEffect(Temp);
	}
	break;	
	case CActionScriptDlg::ZOOMEFFECT:
	{
		Check = CreateZoomEffect(Temp);
	}
	break;
	default: 
	{
		MessageBox(_T("특수효과를 지정 할 수 없습니다."), _T("ActionScriptDlg"), MB_ICONWARNING);
		return FALSE;
	}
	}

	if (Check == FALSE) 
	{
		MessageBox(_T("특수효과를 생성 할 수 없습니다."), _T("ActionScriptDlg"), MB_ICONWARNING);
		return FALSE;
	}

	if (true == m_CurActionScript.AddSceneEffectData(Temp))
	{
		int CamEffect = m_CamEffectTypeCom.GetCurSel();
		int Detail = m_DetailTypeCom.GetCurSel();

		CString RegString = _T("");
		RegString.Format(_T("(%d,%d): %.2f"), CamEffect, Detail, m_fCamEffectStartTime);

		int Index = m_CamEffectRegCom.AddString(RegString);
		m_CamEffectRegCom.SetCurSel(0);
		return TRUE;
	}

	return FALSE;
}
BOOL CActionScriptDlg::CamEffectModifyEvent(const int& _ModIndex) 
{
	if (_ModIndex >= (int)m_CurActionScript.SceneEffectCount) 
	{
		MessageBox(_T("특수효과를 수정 할 수 없습니다."), _T("ActionScriptDlg"), MB_ICONWARNING);
		return FALSE;
	}


	int CamEffectType = m_CamEffectTypeCom.GetCurSel();
	SCENEEFFECTDATA Temp;
	BOOL Check = FALSE;

	switch (CamEffectType)
	{
	case CActionScriptDlg::EARTH_XAXIS:
	case CActionScriptDlg::EARTH_YAXIS:
	case CActionScriptDlg::EARTH_RECT:
	case CActionScriptDlg::EARTH_CIRCLE:
	{
		Check = CreateEarthEffect(Temp);
	}
	break;
	case CActionScriptDlg::BLACKOUT_FLASH:
	{
		Check = CreateBlackOutEffect(Temp);
	}
	break;
	case CActionScriptDlg::FADE_IN:
	case CActionScriptDlg::FADE_OUT:
	{
		Check = CreateFadeEffect(Temp);
	}
	break;
	case CActionScriptDlg::ZOOMEFFECT:
	{
		Check = CreateZoomEffect(Temp);
	}
	break;
	default:
	{
		MessageBox(_T("특수효과를 지정 할 수 없습니다."), _T("ActionScriptDlg"), MB_ICONWARNING);
		return FALSE;
	}
	}

	if (Check == FALSE)
	{
		MessageBox(_T("특수효과를 생성 할 수 없습니다."), _T("ActionScriptDlg"), MB_ICONWARNING);
		return FALSE;
	}

	m_CurActionScript.SceneEffectData[_ModIndex] = Temp;

	ClearCamEffectRegCombo();
	
	for (size_t i = 0; i < m_CurActionScript.SceneEffectCount; i++)
	{
		SCENEEFFECTDATA Data = m_CurActionScript.SceneEffectData[i];
		int Type = 0;
		int Detail = 0;
		int ZoomType = -1;

		if (FALSE == ChangeCamEffectDataToIndex(Data, Type, Detail, ZoomType))
			continue;

		CString AddRegString = _T("");
		AddRegString.Format(_T("(%d,%d): %.2f"), Type, Detail, Data.SceneEffectStartTime);
		m_CamEffectRegCom.AddString(AddRegString);

	}

	if (m_CamEffectRegCom.GetCount() != m_CurActionScript.SceneEffectCount + 1)
	{
		assert(nullptr);
		return FALSE;
	}

	m_CamEffectRegCom.SetCurSel(_ModIndex + 1);

	return TRUE;
}
BOOL CActionScriptDlg::CamEffectDeleteEfent(const int& _DelIndex) 
{
	if (_DelIndex >= (int)m_CurActionScript.SceneEffectCount)
	{
		MessageBox(_T("특수효과를 제거 할 수 없습니다."), _T("ActionScriptDlg"), MB_ICONWARNING);
		return FALSE;
	}

	if (true == m_CurActionScript.DeleteSceneEffectData(_DelIndex)) 
	{
		m_CamEffectRegCom.DeleteString(_DelIndex + 1);
		CamEffectRegComEvent(NOREGINDEX);
		return TRUE;
	}

	return FALSE;
}

BOOL CActionScriptDlg::CreateEarthEffect(SCENEEFFECTDATA& _Data)
{
	if (nullptr == &_Data)
		return FALSE;

	DWORD BaseFlag = SCENE_EFFECT_EARTHQUAKE;

	int EarthType = m_CamEffectTypeCom.GetCurSel();
	int Detail = m_DetailTypeCom.GetCurSel();

	// 만일 디테일 콤보 박스의 문자열 숫자가 벡터에 담긴 지진 디테일 문자열의
	// 갯수가 맞지 않은 경우 데이터를 생성하지 않는다.
	if (m_DetailTypeCom.GetCount() != MAXEARTHDETAIL)
		return FALSE;


	DWORD SpaceFlag = 0;

	switch (EarthType)
	{
	case CActionScriptDlg::EARTH_XAXIS:
		SpaceFlag = EARTHQUAKE_LEFTRIGHT;
		break;
	case CActionScriptDlg::EARTH_YAXIS:
		SpaceFlag = EARTHQUAKE_UPDOWN;
		break;
	case CActionScriptDlg::EARTH_RECT:
		SpaceFlag = EARTHQUAKE_RECTSPACE;
		break;
	case CActionScriptDlg::EARTH_CIRCLE:
		SpaceFlag = EARTHQUAKE_CIRCLESPACE;
	break;
	// 만일 현재 특수 효과 콤보박스의 현재 셀이 지진관련이 아닐경우 
	// 데이터를 생성하지 않는다.
	default:
		return FALSE;
	}

	DWORD SubType = 0;
	Vec3 EarthSpaceSize = Vec3{ 0.0f,0.0f,0.0f };
	bool Check = false;

	switch (Detail)
	{
	case CActionScriptDlg::CONST_STRONG:
	{
		Detail = EARTHQUAKE_CONSISTENT;
		Check = _Data.SetEarthEffect(BaseFlag | SpaceFlag | Detail, m_fCamEffectStartTime, m_fCamEffectTime, m_vecEarthSpaceSize[STRONG]);
	}
	break;
	case CActionScriptDlg::CONST_NORMAL:
	{
		Detail = EARTHQUAKE_CONSISTENT;
		Check = _Data.SetEarthEffect(BaseFlag | SpaceFlag | Detail, m_fCamEffectStartTime, m_fCamEffectTime, m_vecEarthSpaceSize[NORMAL]);
	}
	break;
	case CActionScriptDlg::CONST_WEAK:
	{
		Detail = EARTHQUAKE_CONSISTENT;
		Check = _Data.SetEarthEffect(BaseFlag | SpaceFlag | Detail, m_fCamEffectStartTime, m_fCamEffectTime, m_vecEarthSpaceSize[WEAK]);
	}
	break;
	case CActionScriptDlg::INC_STRONG:
	{
		Detail = EARTHQUAKE_INCREASE;
		Check = _Data.SetEarthEffect(BaseFlag |SpaceFlag | Detail, m_fCamEffectStartTime, m_fCamEffectTime, m_vecEarthSpaceSize[STRONG]);
	}
		break;
	case CActionScriptDlg::INC_NORMAL:
	{
		Detail = EARTHQUAKE_INCREASE;
		Check = _Data.SetEarthEffect(BaseFlag | SpaceFlag | Detail, m_fCamEffectStartTime, m_fCamEffectTime, m_vecEarthSpaceSize[NORMAL]);
	}
		break;
	case CActionScriptDlg::INC_WEAK:
	{
		Detail = EARTHQUAKE_INCREASE;
		Check = _Data.SetEarthEffect(BaseFlag | SpaceFlag | Detail, m_fCamEffectStartTime, m_fCamEffectTime, m_vecEarthSpaceSize[WEAK]);
	}
		break;
	case CActionScriptDlg::DEC_STRONG:
	{
		Detail = EARTHQUAKE_DECREASE;
		Check = _Data.SetEarthEffect(BaseFlag | SpaceFlag | Detail, m_fCamEffectStartTime, m_fCamEffectTime, m_vecEarthSpaceSize[STRONG]);
	}
		break;
	case CActionScriptDlg::DEC_NORMAL:
	{
		Detail = EARTHQUAKE_DECREASE;
		Check = _Data.SetEarthEffect(BaseFlag | SpaceFlag | Detail, m_fCamEffectStartTime, m_fCamEffectTime, m_vecEarthSpaceSize[NORMAL]);
	}
	break;
	case CActionScriptDlg::DEC_WEAK:
	{
		Detail = EARTHQUAKE_DECREASE;
		Check = _Data.SetEarthEffect(BaseFlag | SpaceFlag | Detail, m_fCamEffectStartTime, m_fCamEffectTime, m_vecEarthSpaceSize[WEAK]);
	}
	break;
	}

	if (Check == false)
		return FALSE;

	return TRUE;
}
BOOL CActionScriptDlg::CreateBlackOutEffect(SCENEEFFECTDATA& _Data)
{
	if (nullptr == &_Data)
		return FALSE;

	int DetailIndex = m_DetailTypeCom.GetCurSel();

	if (DetailIndex >= m_vecCamDetailType[BLACKOUT].size())
		return FALSE;

	DWORD Flag = 0;
	bool Check = false;

	if (DetailIndex == 0)
		Flag = SCENE_EFFECT_BLACKOUT;
	else
		Flag = SCENE_EFFECT_FLASH;

	
	Check = _Data.SetBlackOutEffect(Flag, m_fCamEffectStartTime, m_fCamEffectTime);

	return (BOOL)Check;
}
BOOL CActionScriptDlg::CreateFadeEffect(SCENEEFFECTDATA& _Data)
{
	if (nullptr == &_Data)
		return FALSE;

	int CamIndex = m_CamEffectTypeCom.GetCurSel();
	int Detail = m_DetailTypeCom.GetCurSel();

	DWORD BaseFlag = 0;

	if (CamIndex == FADE_IN)
		BaseFlag = SCENE_EFFECT_FADE_IN;
	else if (CamIndex == FADE_OUT)
		BaseFlag = SCENE_EFFECT_FADE_OUT;
	else
		return FALSE;

	switch (Detail)
	{
	case FADECOLOR_BLACK:
		BaseFlag = BaseFlag | FADE_COLOR_BLACK;
		break;
	case FADECOLOR_WHITE:
		BaseFlag = BaseFlag | FADE_COLOR_WHITE;
		break;
	case FADECOLOR_RED:
		BaseFlag = BaseFlag | FADE_COLOR_RED;
		break;
	case FADECOLOR_GREEN:
		BaseFlag = BaseFlag | FADE_COLOR_GREEN;
		break;
	case FADECOLOR_BLUE:
		BaseFlag = BaseFlag | FADE_COLOR_BLUE;
		break;
	default :
		return FALSE;
	}

	return (BOOL)_Data.SetFadeEffect(BaseFlag, m_fCamEffectStartTime, m_fCamEffectTime, m_fCamSubTime);

}
BOOL CActionScriptDlg::CreateZoomEffect(SCENEEFFECTDATA& _Data)
{
	if (nullptr == &_Data)
		return FALSE;

	int Detail = m_DetailTypeCom.GetCurSel();
	DWORD Flag = 0;
	
	if (Detail == 0)
		Flag = SCENE_EFFECT_ZOOM_IN;
	else if (Detail == 1)
		Flag = SCENE_EFFECT_ZOOM_OUT;
	else
		return FALSE;

	UINT ZoomPosMode =  (UINT)m_ZoomPosTypeCom.GetCurSel();
	return (BOOL)_Data.SetZoomEffect(Flag, m_fCamEffectStartTime, m_fCamEffectTime, m_fCamSubTime, ZoomPosMode, Vec3{ m_fInitZoomPosX,m_fInitZoomPosY,1.0f }, Vec2{m_fZoomNear,m_fZoomFar});
}

BOOL CActionScriptDlg::ChangeCamEffectDataToIndex(const SCENEEFFECTDATA& _Data,  int& _Type,  int& _Detail,  int& _ZoomPosType)
{
	if (nullptr == &_Data || nullptr == &_Type || nullptr == &_Detail || nullptr == &_ZoomPosType)
		return FALSE;

	DWORD Flag = _Data.SceneEffectFlag & 0xFF000000L;
	BOOL check = FALSE;

	switch (Flag)
	{
	case SCENE_EFFECT_EARTHQUAKE:
		check = ChangeEarthDataToIndex(_Data,_Type, _Detail);
		break;
	case SCENE_EFFECT_BLACKOUT:
	case SCENE_EFFECT_FLASH:
		check = ChangeBlackOutDataToIndex(_Data, _Type, _Detail);
		break;
	case SCENE_EFFECT_FADE_IN:
	case SCENE_EFFECT_FADE_OUT:
		check = ChangeFadeDataToIndex(_Data, _Type, _Detail);
		break;
	case SCENE_EFFECT_ZOOM_IN:
	case SCENE_EFFECT_ZOOM_OUT:
		check = ChangeZoomDataToIndex(_Data, _Type, _Detail, _ZoomPosType);
		break;
	}


	return check;
}
BOOL CActionScriptDlg::ChangeEarthDataToIndex(const SCENEEFFECTDATA& _Data, int& _Type, int& _Detail)
{
	if (nullptr == &_Data || nullptr == &_Type || nullptr == &_Detail)
		return FALSE;

	DWORD SpaceFlag = _Data.SceneEffectFlag & 0x00FF0000L;
	DWORD PowerFlag = _Data.SceneEffectFlag & 0x0000FF00L;

	switch (SpaceFlag)
	{
	case EARTHQUAKE_UPDOWN:
		_Type = EARTH_YAXIS;
		break;
	case EARTHQUAKE_LEFTRIGHT:
		_Type = EARTH_XAXIS;
		break;
	case EARTHQUAKE_CIRCLESPACE:
		_Type = EARTH_CIRCLE;
		break;
	case EARTHQUAKE_RECTSPACE:
		_Type = EARTH_RECT;
		break;
	default:
		return FALSE;
	}

	switch (PowerFlag)
	{
	case EARTHQUAKE_CONSISTENT:
	{
		for (int i = 0; i < MAXEARTHSPACESIZE; i++)
		{
			if (m_vecEarthSpaceSize[i] == _Data.Earthquake_SpaceSize) 
			{
				_Detail = CONST_STRONG + i;
				break;
			}
		}
	}
		break;
	case EARTHQUAKE_INCREASE:
	{
		for (int i = 0; i < MAXEARTHSPACESIZE; i++)
		{
			if (m_vecEarthSpaceSize[i] == _Data.Earthquake_SpaceSize)
			{
				_Detail = INC_STRONG + i;
				break;
			}
		}
	}
		break;
	case EARTHQUAKE_DECREASE:
	{
		for (int i = 0; i < MAXEARTHSPACESIZE; i++)
		{
			if (m_vecEarthSpaceSize[i] == _Data.Earthquake_SpaceSize)
			{
				_Detail = DEC_STRONG + i;
				break;
			}
		}
	}
		break;
	default:
		_Detail = 0;
	}

	return TRUE;
}
BOOL CActionScriptDlg::ChangeBlackOutDataToIndex(const SCENEEFFECTDATA& _Data, int& _Type, int& _Detail)
{
	if (nullptr == &_Data || nullptr == &_Type || nullptr == &_Detail)
		return FALSE;

	DWORD Flag = _Data.SceneEffectFlag & 0xFF000000L;

	_Type = BLACKOUT_FLASH;


	switch (Flag)
	{
	case SCENE_EFFECT_BLACKOUT:
		_Detail = 0;
		break;
	case SCENE_EFFECT_FLASH:
		_Detail = 1;
		break;
	default:
		return FALSE;
	}

	return TRUE;
}
BOOL CActionScriptDlg::ChangeFadeDataToIndex(const SCENEEFFECTDATA& _Data, int& _Type, int& _Detail)
{
	if (nullptr == &_Data || nullptr == &_Type || nullptr == &_Detail)
		return FALSE;

	DWORD BaseFlag = _Data.SceneEffectFlag & 0xFF000000L;
	DWORD ColorFlag = _Data.SceneEffectFlag & 0x0000FF00L;

	switch (BaseFlag)
	{
	case SCENE_EFFECT_FADE_IN:
		_Type = FADE_IN;
		break;
	case SCENE_EFFECT_FADE_OUT:
		_Type = FADE_OUT;
		break;
	default:
		return FALSE;
	}

	switch (ColorFlag)
	{
	case FADE_COLOR_BLACK:
		_Detail = FADECOLOR_BLACK;
		break;
	case FADE_COLOR_WHITE:
		_Detail = FADECOLOR_WHITE;
		break;
	case FADE_COLOR_RED:
		_Detail = FADECOLOR_RED;
		break;
	case FADE_COLOR_GREEN:
		_Detail = FADECOLOR_GREEN;
		break;
	case FADE_COLOR_BLUE:
		_Detail = FADECOLOR_BLUE;
		break;
	default:
		return FALSE;
	}

	return TRUE;
}
BOOL CActionScriptDlg::ChangeZoomDataToIndex(const SCENEEFFECTDATA& _Data, int& _Type, int& _Detail, int& _ZoomPosType)
{
	if (nullptr == &_Data || nullptr == &_Type || nullptr == &_Detail || nullptr == &_ZoomPosType)
		return FALSE;

	_Type = ZOOMEFFECT;
	 
	DWORD Flag = _Data.SceneEffectFlag & 0xFF000000L;

	switch (Flag)
	{
	case SCENE_EFFECT_ZOOM_IN:
		_Detail = 0;
		break;
	case SCENE_EFFECT_ZOOM_OUT:
		_Detail = 1;
		break;
	default:
		return FALSE;
	}

	if (_Data.ZoomPosMode == SCENEEFFECTDATA::ZOOMPOS_UNIT)
		_ZoomPosType = 0;
	else if (_Data.ZoomPosMode == SCENEEFFECTDATA::ZOOMPOS_CUSTOM)
		_ZoomPosType = 1;
	else
		return FALSE;

	return TRUE;

}

const int CActionScriptDlg::ChangeTypeIndexToVecIndex(const int& _Type)
{
	if (nullptr == &_Type)
		return -1;

	switch (_Type)
	{
	case CActionScriptDlg::EARTH_XAXIS:
	case CActionScriptDlg::EARTH_YAXIS:
	case CActionScriptDlg::EARTH_RECT:
	case CActionScriptDlg::EARTH_CIRCLE:
		return EARTH;
	case CActionScriptDlg::BLACKOUT_FLASH:
		return BLACKOUT;
	case CActionScriptDlg::FADE_IN:
	case CActionScriptDlg::FADE_OUT:
		return FADE;
	case CActionScriptDlg::ZOOMEFFECT:
		return ZOOM;
	}

	return -1;
}
void CActionScriptDlg::UpdateSkillList()
{
	vector<SKILL>* SkillVec = CEditGlobal::EditSkillMgr->GetSkillVector();

	if (nullptr == SkillVec)
		return;

	ClearSkillList();

	for (size_t i = 0; i < SkillVec->size(); i++)
	{
		if (-1 != (*SkillVec)[i].SkillNo)
		{

			CString SkillName = (*SkillVec)[i].SkillName;
			// 임시 이름이 아닌경우
			
			if (false == (*SkillVec)[i].SkillData.CharacterSkill)
				SkillName.Format(_T("%d) %s"), (*SkillVec)[i].SkillNo, (*SkillVec)[i].SkillName);
			else if (-1 == SkillName.Find(_T("_Skill")))
			{
				SkillName.Format(_T("%d) %s"), (*SkillVec)[i].SkillNo, (*SkillVec)[i].SkillName);
			}

			m_ActSkillList.AddString(SkillName);
		}
	}

}
const int CActionScriptDlg::ListStringToSkillNo(CString& _ListString)
{
	if (nullptr == &_ListString)
		return -1;


	int Index = _ListString.Find(_T(")"));

	if (-1 == Index)
	{
		assert(nullptr);
		return -1;
	}

	_ListString = _ListString.Left(Index);

	int SkillNo = CEditGlobal::ChangeIntValue(_ListString);
	return SkillNo;
}
BOOL CActionScriptDlg::CreateSkill()
{
	if (true == m_CurActionScript.isEmpty()) 
	{
		MessageBox(_T("현재 ActionScript가 비어있습니다."));
		return FALSE;
	}

	if (0 >= m_CurActionScript.TotalSkillTime)
	{
		MessageBox(_T("Skill Total Time이 적용불가능한 값입니다."));
		return FALSE;
	}

	BOOL Check = CEditGlobal::EditSkillMgr->CreateSkill(m_CurActionScript);

	if (TRUE == Check) 
	{
		UpdateSkillList();

		m_CurActionScript = ACTIONSCRIPT();
		UpdateData(TRUE);
		m_CurActionScript.SkillNo = CEditGlobal::EditSkillMgr->SearchNextIndex();
		m_fTotalTime = m_CurActionScript.TotalSkillTime;
		m_strTotalTime.Format(_T("%.2f"), m_fTotalTime);
		InitAllComboBox();
		UpdateData(FALSE);
	}
	else 
	{
		MessageBox(_T("Skill을 생성하지 못하였습니다."));
	}

	return Check;
}
BOOL CActionScriptDlg::ModifyActionScript(const int& _ListIndex)
{
	CString SkillNoString;
	UpdateData(TRUE);
	m_ActSkillList.GetText(_ListIndex, SkillNoString);
	UpdateData(FALSE);

	int SkillNo = ListStringToSkillNo(SkillNoString);
		
	if (FALSE == CEditGlobal::EditSkillMgr->ChangeActionScript(SkillNo, m_CurActionScript)) 
	{
		MessageBox(_T("Action Script 수정에 실패하였습니다."));
		return FALSE;
	}
	else 
	{
		MessageBox(_T("Action Script 수정 하였습니다."));
	}

	
	return TRUE;
}
BOOL CActionScriptDlg::DeleteSkill(const int& _ListIndex)
{
	CString SkillNoString;

	UpdateData(TRUE);
	m_ActSkillList.GetText(_ListIndex, SkillNoString);
	UpdateData(FALSE);

	int SkillNo = ListStringToSkillNo(SkillNoString);

	if (FALSE == CEditGlobal::EditSkillMgr->DeleteSkill(SkillNo)) 
	{
		MessageBox(_T("Skill을 삭제할 수 없습니다."));
		return FALSE;
	}

	m_ActSkillList.DeleteString(_ListIndex);
	m_CurActionScript = ACTIONSCRIPT();
	m_CurActionScript.SkillNo = CEditGlobal::EditSkillMgr->SearchNextIndex();

	InitAllComboBox();
	return TRUE;
}
BOOL CActionScriptDlg::LoadSkillFile()
{
	CGameFile SkillFile(_T("EditData"), _T("Skill.bin"));
	CGameFile LevelFile(_T("EditData"), _T("LevelData.bin"));

	if (FALSE == SkillFile.Open(_T("rb")))
	{
		MessageBox(_T("SkillDataFile을 열지 못하였습니다."));
		return FALSE;
	}

	if (FALSE == LevelFile.Open(_T("rb")))
	{
		MessageBox(_T("LevelDataFile을 열지 못하였습니다."));
		return FALSE;
	}



	if (FALSE == CEditGlobal::EditSkillMgr->LoadSkillFile(SkillFile.GetFile(),LevelFile.GetFile())) 
	{
		MessageBox(_T("Skill을 정상적으로 로드하지 못하였습니다."));
		return FALSE;
	}

	UpdateSkillList();
	MessageBox(_T("Skill을 정상적으로 로드했습니다."));
	return TRUE;
}
BOOL CActionScriptDlg::SaveSkillFile()
{
	CGameFile SkillFile(_T("EditData"), _T("Skill.bin"));

	if (FALSE == SkillFile.Open(_T("wb")))
	{
		MessageBox(_T("SkillDataFile을 열지 못하였습니다."));
		return FALSE;
	}

	if(FALSE == CEditGlobal::EditSkillMgr->SaveSkillFile(SkillFile.GetFile()))
	{
		MessageBox(_T("Skill을 정상적으로 저장하지 못하였습니다."));
		return FALSE;
	}


	MessageBox(_T("Skill을 정상적으로 저장했습니다."));
	return TRUE;
}
void CActionScriptDlg::OnOK()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	return;
	//CDialogEx::OnOK();
}


BOOL CActionScriptDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	// EditGlobal의 초기화가 선행 안되도 되는 초기화 로직
	m_ActSkillList.AddString(_T("새로 만들기"));
	m_ActSkillList.SetCurSel(0);

	m_vecAnimationName.push_back(_T("0 : Idle"));
	m_vecAnimationName.push_back(_T("1 : Stand"));
	m_vecAnimationName.push_back(_T("2 : Move"));
	m_vecAnimationName.push_back(_T("3 : Attack"));
	m_vecAnimationName.push_back(_T("4 : LimitAttack"));
	m_vecAnimationName.push_back(_T("5 : Dead"));
	m_vecAnimationName.push_back(_T("6 : Dying"));
	m_vecAnimationName.push_back(_T("7 : MagicAttack"));
	m_vecAnimationName.push_back(_T("8 : MagicStand"));
	m_vecAnimationName.push_back(_T("9 : Win"));
	m_vecAnimationName.push_back(_T("10 : Win_Before"));
	m_vecAnimationName.push_back(_T("11 : Attack2"));
	m_vecAnimationName.push_back(_T("12 : DoubleAttack"));

	m_MoveTypeCom.AddString(_T("0: Move"));
	m_MoveTypeCom.AddString(_T("1: TeamPos"));
	m_MoveTypeCom.AddString(_T("2: InitPos"));
	m_MoveTypeCom.AddString(_T("3: Hide"));
	m_MoveTypeCom.AddString(_T("4: Show"));

	m_EffectPosTypeCom.AddString(_T("Custom"));
	m_EffectPosTypeCom.AddString(_T("Caster"));

	m_EffectMoveTypeCom.AddString(_T("0: No Move"));
	m_EffectMoveTypeCom.AddString(_T("1: Move"));
	m_EffectMoveTypeCom.AddString(_T("2: Hide"));
	m_EffectMoveTypeCom.AddString(_T("3: Show"));

	m_AniRegCom.AddString(_T("새로 만들기"));
	m_AniRegCom.SetCurSel(0);
	m_SoundRegCom.AddString(_T("새로 만들기"));
	m_SoundRegCom.SetCurSel(0);
	m_MoveRegCom.AddString(_T("새로 만들기"));
	m_MoveRegCom.SetCurSel(0);
	m_SkillEffectRegCom.AddString(_T("새로 만들기"));
	m_SkillEffectRegCom.SetCurSel(0);
	m_CamEffectRegCom.AddString(_T("새로 만들기"));
	m_CamEffectRegCom.SetCurSel(0);

	m_CamEffectTypeCom.AddString(_T("0: X축 지진"));
	m_CamEffectTypeCom.AddString(_T("1: Y축 지진"));
	m_CamEffectTypeCom.AddString(_T("2: 사각 지진"));
	m_CamEffectTypeCom.AddString(_T("3: 원 지진"));
	m_CamEffectTypeCom.AddString(_T("4: 암전 & 섬광"));
	m_CamEffectTypeCom.AddString(_T("5: 페이드 인"));
	m_CamEffectTypeCom.AddString(_T("6: 페이드 아웃"));
	m_CamEffectTypeCom.AddString(_T("7: 줌 효과"));


	m_vecCamDetailType.clear();
	vector<CString> TempVec;
	TempVec.clear();

	TempVec.push_back(_T("0: 유지(강)"));
	TempVec.push_back(_T("1: 유지(중)"));
	TempVec.push_back(_T("2: 유지(약)"));
	TempVec.push_back(_T("3: 증폭(강)"));
	TempVec.push_back(_T("4: 증폭(중)"));
	TempVec.push_back(_T("5: 증폭(약)"));
	TempVec.push_back(_T("6: 감쇄(강)"));
	TempVec.push_back(_T("7: 감쇄(중)"));
	TempVec.push_back(_T("8: 감쇄(약)"));
	m_vecCamDetailType.push_back(TempVec);

	TempVec.clear();
	TempVec.push_back(_T("0: 암전"));
	TempVec.push_back(_T("1: 섬광"));
	m_vecCamDetailType.push_back(TempVec);

	TempVec.clear();
	TempVec.push_back(_T("0: 검은색"));
	TempVec.push_back(_T("1: 하얀색"));
	TempVec.push_back(_T("2: 빨간색"));
	TempVec.push_back(_T("3: 초록색"));
	TempVec.push_back(_T("4: 파란색"));
	m_vecCamDetailType.push_back(TempVec);

	TempVec.clear();
	TempVec.push_back(_T("0: 줌 인"));
	TempVec.push_back(_T("1: 줌 아웃"));
	m_vecCamDetailType.push_back(TempVec);

	m_ZoomPosTypeCom.AddString(_T("Unit"));
	m_ZoomPosTypeCom.AddString(_T("Custom"));

	m_vecEarthSpaceSize.push_back(Vec2{ 70.0f,70.0f });
	m_vecEarthSpaceSize.push_back(Vec2{ 40.0f,40.0f });
	m_vecEarthSpaceSize.push_back(Vec2{ 25.0f,25.0f });

	m_mapEditSet.insert(unordered_map<UINT, EditSet>::value_type(m_TotalTimeEdit.GetDlgCtrlID(),EditSet(&m_TotalTimeEdit,&m_strTotalTime,&m_fTotalTime)));
	m_mapEditSet.insert(unordered_map<UINT, EditSet>::value_type(m_AniStartTimeEdit.GetDlgCtrlID(), EditSet(&m_AniStartTimeEdit, &m_strAniStartTime, &m_fAniStartTime)));
	m_mapEditSet.insert(unordered_map<UINT, EditSet>::value_type(m_SoundStartTimeEdit.GetDlgCtrlID(), EditSet(&m_SoundStartTimeEdit, &m_strSoundStartTime, &m_fSoundStartTime)));
	// MoveData 
	m_mapEditSet.insert(unordered_map<UINT, EditSet>::value_type(m_MoveStartTimeEdit.GetDlgCtrlID(), EditSet(&m_MoveStartTimeEdit, &m_strMoveStartTime, &m_fMoveStartTime)));
	m_mapEditSet.insert(unordered_map<UINT, EditSet>::value_type(m_MoveTimeEdit.GetDlgCtrlID(), EditSet(&m_MoveTimeEdit, &m_strMoveTime, &m_fMoveTime)));
	m_mapEditSet.insert(unordered_map<UINT, EditSet>::value_type(m_HideTimeEdit.GetDlgCtrlID(), EditSet(&m_HideTimeEdit, &m_strHideTime, &m_fHideTime)));
	m_mapEditSet.insert(unordered_map<UINT, EditSet>::value_type(m_MoveStartXEdit.GetDlgCtrlID(), EditSet(&m_MoveStartXEdit, &m_strMoveStartX, &m_fMoveStartX)));
	m_mapEditSet.insert(unordered_map<UINT, EditSet>::value_type(m_MoveStartYEdit.GetDlgCtrlID(), EditSet(&m_MoveStartYEdit, &m_strMoveStartY, &m_fMoveStartY)));
	m_mapEditSet.insert(unordered_map<UINT, EditSet>::value_type(m_MoveEndXEdit.GetDlgCtrlID(), EditSet(&m_MoveEndXEdit, &m_strMoveEndX, &m_fMoveEndX)));
	m_mapEditSet.insert(unordered_map<UINT, EditSet>::value_type(m_MoveEndYEdit.GetDlgCtrlID(), EditSet(&m_MoveEndYEdit, &m_strMoveEndY, &m_fMoveEndY)));
	// SkillEffect
	m_mapEditSet.insert(unordered_map<UINT, EditSet>::value_type(m_EffectInitPosXEdit.GetDlgCtrlID(), EditSet(&m_EffectInitPosXEdit, &m_strEffectInitPosX, &m_fEffectInitPosX)));
	m_mapEditSet.insert(unordered_map<UINT, EditSet>::value_type(m_EffectInitPosYEdit.GetDlgCtrlID(), EditSet(&m_EffectInitPosYEdit, &m_strEffectInitPosY, &m_fEffectInitPosY)));
	m_mapEditSet.insert(unordered_map<UINT, EditSet>::value_type(m_EffectStartTimeEdit.GetDlgCtrlID(), EditSet(&m_EffectStartTimeEdit, &m_strEffectStartTime, &m_fEffectStartTime)));
	m_mapEditSet.insert(unordered_map<UINT, EditSet>::value_type(m_EffectSizeXEdit.GetDlgCtrlID(), EditSet(&m_EffectSizeXEdit, &m_strEffectSizeX, &m_fEffectSizeX)));
	m_mapEditSet.insert(unordered_map<UINT, EditSet>::value_type(m_EffectSizeYEdit.GetDlgCtrlID(), EditSet(&m_EffectSizeYEdit, &m_strEffectSizeY, &m_fEffectSizeY)));
	m_mapEditSet.insert(unordered_map<UINT, EditSet>::value_type(m_EffectMagEdit.GetDlgCtrlID(), EditSet(&m_EffectMagEdit, &m_strEffectMag, &m_fEffectMag)));
	m_mapEditSet.insert(unordered_map<UINT, EditSet>::value_type(m_EMoveStartTimeEdit.GetDlgCtrlID(), EditSet(&m_EMoveStartTimeEdit, &m_strEMoveStartTime, &m_fEMoveStartTime)));
	m_mapEditSet.insert(unordered_map<UINT, EditSet>::value_type(m_EMoveTimeEdit.GetDlgCtrlID(), EditSet(&m_EMoveTimeEdit, &m_strEMoveTime, &m_fEMoveTime)));
	m_mapEditSet.insert(unordered_map<UINT, EditSet>::value_type(m_EHideTimeEdit.GetDlgCtrlID(), EditSet(&m_EHideTimeEdit, &m_strEHideTime, &m_fEHideTime)));
	m_mapEditSet.insert(unordered_map<UINT, EditSet>::value_type(m_EMoveStartXEdit.GetDlgCtrlID(), EditSet(&m_EMoveStartXEdit, &m_strEMoveStartX, &m_fEMoveStartX)));
	m_mapEditSet.insert(unordered_map<UINT, EditSet>::value_type(m_EMoveStartYEdit.GetDlgCtrlID(), EditSet(&m_EMoveStartYEdit, &m_strEMoveStartY, &m_fEMoveStartY)));
	m_mapEditSet.insert(unordered_map<UINT, EditSet>::value_type(m_EMoveEndXEdit.GetDlgCtrlID(), EditSet(&m_EMoveEndXEdit, &m_strEMoveEndX, &m_fEMoveEndX)));
	m_mapEditSet.insert(unordered_map<UINT, EditSet>::value_type(m_EMoveEndYEdit.GetDlgCtrlID(), EditSet(&m_EMoveEndYEdit, &m_strEMoveEndY, &m_fEMoveEndY)));
	// Scene(Camera) Effect 
	m_mapEditSet.insert(unordered_map<UINT, EditSet>::value_type(m_CamEffectStartTime.GetDlgCtrlID(), EditSet(&m_CamEffectStartTime, &m_strCamEffectStartTime, &m_fCamEffectStartTime)));
	m_mapEditSet.insert(unordered_map<UINT, EditSet>::value_type(m_CamEffectTime.GetDlgCtrlID(), EditSet(&m_CamEffectTime, &m_strCamEffectTime, &m_fCamEffectTime)));
	m_mapEditSet.insert(unordered_map<UINT, EditSet>::value_type(m_CamSubTime.GetDlgCtrlID(), EditSet(&m_CamSubTime, &m_strCamSubTime, &m_fCamSubTime)));
	m_mapEditSet.insert(unordered_map<UINT, EditSet>::value_type(m_InitZoomPosX.GetDlgCtrlID(), EditSet(&m_InitZoomPosX, &m_strInitZoomPosX, &m_fInitZoomPosX)));
	m_mapEditSet.insert(unordered_map<UINT, EditSet>::value_type(m_InitZoomPosY.GetDlgCtrlID(), EditSet(&m_InitZoomPosY, &m_strInitZoomPosY, &m_fInitZoomPosY)));
	m_mapEditSet.insert(unordered_map<UINT, EditSet>::value_type(m_ZoomNear.GetDlgCtrlID(), EditSet(&m_ZoomNear, &m_strZoomNear, &m_fZoomNear)));
	m_mapEditSet.insert(unordered_map<UINT, EditSet>::value_type(m_ZoomFar.GetDlgCtrlID(), EditSet(&m_ZoomFar, &m_strZoomFar, &m_fZoomFar)));


	m_vecEditCtrl.push_back(&m_TotalTimeEdit);
	m_vecEditCtrl.push_back(&m_AniStartTimeEdit);
	m_vecEditCtrl.push_back(&m_SoundStartTimeEdit);
	m_vecEditCtrl.push_back(&m_MoveStartTimeEdit);
	m_vecEditCtrl.push_back(&m_MoveTimeEdit);
	m_vecEditCtrl.push_back(&m_HideTimeEdit);
	m_vecEditCtrl.push_back(&m_MoveStartXEdit);
	m_vecEditCtrl.push_back(&m_MoveStartYEdit);
	m_vecEditCtrl.push_back(&m_MoveEndXEdit);
	m_vecEditCtrl.push_back(&m_MoveEndYEdit);
	

	m_EffectPosTypeCom.SetCurSel(0);
	m_EffectMoveTypeCom.SetCurSel(0);
	EffectMoveTypeComEvent(0);

	

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


BOOL CActionScriptDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	{
		if (TRUE == CheckButtonEvent(pMsg))
			return TRUE;
	}

	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE) {
			// VK_RETURN  : Enter Key
			// VK_ESCAPE  : ESC key

			return TRUE;
		}
	}

	if (pMsg->message == WM_LBUTTONDOWN)
	{
		SendMessageA(this->m_hWnd, WM_SETFOCUS, NULL, NULL);
	
		/*if (TRUE == CheckButtonEvent(pMsg))
			return TRUE;
		*/

		if (TRUE == EditBoxClickCheck(pMsg))
		{
			return TRUE;
		}

	}
	return CDialogEx::PreTranslateMessage(pMsg);
}


void CActionScriptDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	m_vecAnimationName.clear();
	m_vecSkillSound.clear();
	m_vecEditCtrl.clear();
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}


void CActionScriptDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if (TRUE == bShow)
	{
		if (FALSE == m_bInit)
		{
			InitActionScriptDlg();
			UpdateData(TRUE);
			m_CurActionScript.SkillNo = CEditGlobal::EditSkillMgr->SearchNextIndex();
			UpdateData(FALSE);
			m_bInit = TRUE;
		}

		SkillNoComboUpdate();
		UpdateSkillList();
	}
}


void CActionScriptDlg::OnCbnSelchangeCombo1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int CurIndex = m_SkillCharCom.GetCurSel();


	if (CurIndex != -1 && CurIndex == m_PrevCharNo)
		return;


	if (nullptr != CEditGlobal::EditSkillMgr && STATE_COMPLETE == CEditGlobal::EditSkillMgr->GetSkillMgrState())
	{
		if (CurIndex == -1)
		{
			CEditGlobal::EditSkillMgr->MonTargetUnitOn();
			CEditGlobal::EditSkillMgr->CurCharacterOff();
			m_SkillCharCom.EnableWindow(FALSE);
			m_CurActionScript.CharIndex = -1;
		}
		else if(CurIndex < MAXBATTLECHARACTER)
		{
			CEditGlobal::EditSkillMgr->CharacterOn((CharacterIndex)CurIndex);
			CEditGlobal::EditSkillMgr->MonTargetUnitOff();
			if (MAXBATTLECHARACTER == m_PrevCharNo || (CEditGlobal::isDoubleAttackChar(CurIndex) != CEditGlobal::isDoubleAttackChar(m_PrevCharNo)))
				UpdateAniTypeCombo(CurIndex);
			m_SkillCharCom.EnableWindow(TRUE);
			m_CurActionScript.CharIndex = CurIndex;
		}
	}


	m_PrevCharNo = CurIndex;
}


BOOL CActionScriptDlg::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	
	UpdateData(TRUE);
	ChangeEditEvent(nID, nCode, pExtra, pHandlerInfo);
	UpdateData(FALSE);

	return CDialogEx::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

void CActionScriptDlg::OnLbnSelchangeList1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int Index = m_ActSkillList.GetCurSel();
	
	if (-1 == Index)
		return;

	if (Index == 0)
		InitCurActionScript();
	else
		ClickSkillListEvent(Index);
}

void CActionScriptDlg::OnEnKillfocusEdit2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CheckTimeString(m_TotalTimeEdit, m_strTotalTime, m_fTotalTime);
	m_CurActionScript.TotalSkillTime = m_fTotalTime;

}


void CActionScriptDlg::OnEnKillfocusAnistarttime()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CheckTimeString(m_AniStartTimeEdit, m_strAniStartTime, m_fAniStartTime);
}


void CActionScriptDlg::OnEnKillfocusSoundstarttime()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CheckTimeString(m_SoundStartTimeEdit, m_strSoundStartTime, m_fSoundStartTime);
}


void CActionScriptDlg::OnEnKillfocusCmovestarttime()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CheckTimeString(m_MoveStartTimeEdit, m_strMoveStartTime, m_fMoveStartTime);

}


void CActionScriptDlg::OnEnKillfocusCmoveendtime()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CheckTimeString(m_MoveTimeEdit, m_strMoveTime, m_fMoveTime);

}


void CActionScriptDlg::OnEnKillfocusCmovehidetime()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CheckTimeString(m_HideTimeEdit, m_strHideTime, m_fHideTime);
}


void CActionScriptDlg::OnEnKillfocusCmovestartposx()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CheckPosString(m_MoveStartXEdit, m_strMoveStartX, m_fMoveStartX);
}


void CActionScriptDlg::OnEnKillfocusCmovestartposy()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CheckPosString(m_MoveStartYEdit, m_strMoveStartY, m_fMoveStartY);
}


void CActionScriptDlg::OnEnKillfocusCmoveendposx()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CheckPosString(m_MoveEndXEdit, m_strMoveEndX, m_fMoveEndX);
}


void CActionScriptDlg::OnEnKillfocusCmoveendposy()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CheckPosString(m_MoveEndXEdit, m_strMoveEndY, m_fMoveEndY);
}

void CActionScriptDlg::OnEnKillfocusActskilleffectposx()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CheckPosString(m_EffectInitPosXEdit, m_strEffectInitPosX, m_fEffectInitPosX);
}


void CActionScriptDlg::OnEnKillfocusActskilleffectposy()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CheckPosString(m_EffectInitPosYEdit, m_strEffectInitPosY, m_fEffectInitPosY);
}


void CActionScriptDlg::OnEnKillfocusActskilleffectstarttime()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CheckTimeString(m_EffectStartTimeEdit, m_strEffectStartTime, m_fEffectStartTime);
}


void CActionScriptDlg::OnEnKillfocusActskilleffectsizex()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CheckPosString(m_EffectSizeXEdit, m_strEffectSizeX, m_fEffectSizeX);
}


void CActionScriptDlg::OnEnKillfocusActskilleffectsizey()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CheckPosString(m_EffectSizeYEdit, m_strEffectSizeY, m_fEffectSizeY);
}


void CActionScriptDlg::OnEnKillfocusActskilleffectmag()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CheckFloatString(m_EffectMagEdit, m_strEffectMag, m_fEffectMag);
}


void CActionScriptDlg::OnEnKillfocusActskilleffectmovestarttime()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CheckTimeString(m_EMoveStartTimeEdit, m_strEMoveStartTime, m_fEMoveStartTime);
}


void CActionScriptDlg::OnEnKillfocusActskilleffectmovetime()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CheckTimeString(m_EMoveTimeEdit, m_strEMoveTime, m_fEMoveTime);
}


void CActionScriptDlg::OnEnKillfocusActskilleffecthidetime()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CheckTimeString(m_EHideTimeEdit, m_strEHideTime, m_fEHideTime);
}


void CActionScriptDlg::OnEnKillfocusActskilleffectmovestartposx()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CheckPosString(m_EMoveStartXEdit, m_strEMoveStartX, m_fEMoveStartX);
}


void CActionScriptDlg::OnEnKillfocusActskilleffectmovestartposy()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CheckPosString(m_EMoveStartYEdit, m_strEMoveStartY, m_fEMoveStartY);
}


void CActionScriptDlg::OnEnKillfocusActskilleffectmoveendposx()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CheckPosString(m_EMoveEndXEdit, m_strEMoveEndX, m_fEMoveEndX);
}


void CActionScriptDlg::OnEnKillfocusActskilleffectmoveendposx2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CheckPosString(m_EMoveEndYEdit, m_strEMoveEndY, m_fEMoveEndY);
}

void CActionScriptDlg::OnEnKillfocusSceneeffectstarttime()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CheckTimeString(m_CamEffectStartTime, m_strCamEffectStartTime, m_fCamEffectStartTime);
}


void CActionScriptDlg::OnEnKillfocusSceneeffectacttime()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CheckTimeString(m_CamEffectTime, m_strCamEffectTime, m_fCamEffectTime);
}


void CActionScriptDlg::OnEnKillfocusSceneeffectsubtime()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CheckTimeString(m_CamSubTime, m_strCamSubTime, m_fCamSubTime);
}


void CActionScriptDlg::OnEnKillfocusZoomposx()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CheckPosString(m_InitZoomPosX, m_strInitZoomPosX, m_fInitZoomPosX);
}


void CActionScriptDlg::OnEnKillfocusZoomposx2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CheckPosString(m_InitZoomPosY, m_strInitZoomPosY, m_fInitZoomPosY);
}


void CActionScriptDlg::OnEnKillfocusZoomnear()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CheckPosString(m_ZoomNear, m_strZoomNear, m_fZoomNear);
}


void CActionScriptDlg::OnEnKillfocusZoomfar()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CheckPosString(m_ZoomFar, m_strZoomFar, m_fZoomFar);
}


void CActionScriptDlg::OnCbnSelchangeAniregcombo()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	AniRegComEvent(m_AniRegCom.GetCurSel() - 1 );
}
void CActionScriptDlg::OnCbnSelchangeSoundregcombo()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	SoundRegComEvent(m_SoundRegCom.GetCurSel() - 1);
}
void CActionScriptDlg::OnCbnSelchangeSoundcombo()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	SoundNameComEvent(m_SoundNameCom.GetCurSel());
}
void CActionScriptDlg::OnCbnSelchangeMoveregcombo()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	MoveRegComEvent(m_MoveRegCom.GetCurSel() - 1);
}
void CActionScriptDlg::OnCbnSelchangeMovecombo()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	MoveTypeComEvent(m_MoveTypeCom.GetCurSel());
}
void CActionScriptDlg::OnCbnSelchangeActskilleffectregcombo()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	SkillEffectRegComEvent(m_SkillEffectRegCom.GetCurSel() - 1);
}

void CActionScriptDlg::OnCbnSelchangeActskilleffectmovecombo()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	EffectMoveTypeComEvent(m_EffectMoveTypeCom.GetCurSel());
}
void CActionScriptDlg::OnCbnSelchangeActskilleffectcombo()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	EffectTypeComEvent(m_SkillEffectNoCom.GetCurSel());
}

void CActionScriptDlg::OnCbnSelchangeSceneeffectregcombo()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CamEffectRegComEvent(m_CamEffectRegCom.GetCurSel() - 1);

}

void CActionScriptDlg::OnCbnSelchangeSceneeffectcombo()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CamEffectTypeComEvent(m_CamEffectTypeCom.GetCurSel());
}


void CActionScriptDlg::OnBnClickedAniregbutton()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int Index = m_AniRegCom.GetCurSel();

	// RegComboBox의 현재 Index 가 0인 경우 새로만들기 수행
	if (0 == Index)
		AnimationAddEvent();
	else
		AnimationModifyEvent(Index - 1);
}



void CActionScriptDlg::OnBnClickedAnidelbutton()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int Index = m_AniRegCom.GetCurSel();

	// RegComboBox의 현재 Index 가 0인 경우 Delete 무시
	if (0 != Index)
		AnimationDeleteEvent(Index - 1);
}


void CActionScriptDlg::OnBnClickedSoundregbutton()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int Index = m_SoundRegCom.GetCurSel();

	// RegComboBox의 현재 Index 가 0인 경우 새로만들기 수행
	if (0 == Index)
		SoundAddEvent();
	else
		SoundModifyEvent(Index - 1);
	
}


void CActionScriptDlg::OnBnClickedAnisounddelbutton()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int Index = m_SoundRegCom.GetCurSel();

	// RegComboBox의 현재 Index 가 0인 경우 Delete 무시
	if (0 != Index)
		SoundDeleteEvent(Index - 1);
	
}

void CActionScriptDlg::OnBnClickedMoveregbutton()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int Index = m_MoveRegCom.GetCurSel();

	// RegComboBox의 현재 Index 가 0인 경우 새로만들기 수행
	if (0 == Index)
		MoveAddEvent();
	else
		MoveModifyEvent(Index - 1);
	
}


void CActionScriptDlg::OnBnClickedMovedelbutton()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int Index = m_MoveRegCom.GetCurSel();

	// RegComboBox의 현재 Index 가 0인 경우 Delete 무시
	if (0 != Index)
		MoveDeleteEvent(Index - 1);
		
}
void CActionScriptDlg::OnBnClickedActskilleffectregbutton()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int Index = m_SkillEffectRegCom.GetCurSel();

	// RegComboBox의 현재 Index 가 0인 경우 새로만들기 수행
	if (0 == Index)
		SkillEffectAddEvent();
	else
		SkillEffectModifyEvent(Index - 1);
	
}


void CActionScriptDlg::OnBnClickedActskilleffectdelbutton()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int Index = m_SkillEffectRegCom.GetCurSel();

	// RegComboBox의 현재 Index 가 0인 경우 Delete 무시
	if (0 != Index)
		SkillEffectDeleteEvent(Index - 1);
	
}

void CActionScriptDlg::OnBnClickedCameffectregbutton()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int  Index = m_CamEffectRegCom.GetCurSel();

	if (Index == 0)
		CamEffectAddEvent();
	else
		CamEffectModifyEvent(Index - 1);
}


void CActionScriptDlg::OnBnClickedCameffectdelbutton()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int  Index = m_CamEffectRegCom.GetCurSel();

	if (Index > 0)
		CamEffectDeleteEfent(Index - 1);
}
void CActionScriptDlg::OnBnClickedActcreatebutton()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int ListIndex = m_ActSkillList.GetCurSel();

	if (-1 == ListIndex)
		return;

	if (ListIndex == 0)
		CreateSkill();
	else
		ModifyActionScript(ListIndex);
}

void CActionScriptDlg::OnBnClickedActdeletebutton()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int ListIndex = m_ActSkillList.GetCurSel();
	if (-1 == ListIndex)
		return;

	if (ListIndex != 0)
		DeleteSkill(ListIndex);

}

void CActionScriptDlg::OnBnClickedActloadbutton()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	LoadSkillFile();
}


void CActionScriptDlg::OnBnClickedActsavebutton()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	SaveSkillFile();
}


