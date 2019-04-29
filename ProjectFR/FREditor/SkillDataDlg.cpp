// SkillDataDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "FREditor.h"
#include "SkillDataDlg.h"
#include "afxdialogex.h"
#include "SkillMgr.h"
#include <SoundPlayer.h>


// CSkillDataDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CSkillDataDlg, CDialogEx)

CSkillDataDlg::CSkillDataDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_SKILLDATADLG, pParent)
	, m_CurSkillNo(0)
	, m_strSkillTotalTime(_T("0.0"))
	, m_strCasterFixPosX(_T("0.0"))
	, m_strCasterFixPosY(_T("0.0"))
	, m_strPosPivotX(_T("0.0"))
	, m_strPosPivotY(_T("0.0"))
	, m_fSkillTotalTime(0.0f)
	, m_fCasterFixPosX(0.0f)
	, m_fCasterFixPosY(0.0f)
	, m_fPosPivotX(0.0f)
	, m_fPosPivotY(0.0f)
	, m_uiSkillCount(0)
	, m_uiSkillCost(0)
	, m_iDemageFontOption(0)
	, m_uiCurHitDataCount(0)
	, m_strHitStartTime(_T("0.0"))
	, m_uiHitOverlapCount(1)
	, m_uiDemageMag(0)
	, m_uiFixDemage(0)
	, m_strDemageFontMag(_T("0.0"))
	, m_strHitEffectPivotX(_T("0.0"))
	, m_strHitEffectPivotY(_T("0.0"))
	, m_strEffectImageMag(_T("1.0"))
	, m_strHitEffectSizeX(_T("0.0"))
	, m_strHitEffectSizeY(_T("0.0"))
	, m_fDemageFontMag(0.0f)
	, m_fHitEffectPivotX(0.0f)
	, m_fHitEffectPivotY(0.0f)
	, m_fEffectImageMag(1.0f)
	, m_fHitEffectSizeX(0.0f)
	, m_fHitEffectSizeY(0.0f)
	, m_iBuffValue(0)
	, m_uiBuffTurnCount(0)
	, m_CurSkillData(SKILLDATA())
	, m_CurCharNo(0)
	, m_SkillDataSpeaker(nullptr)
	, m_uiCurLevel(0)
	, m_CurLevelData(LEVELDATA())
{
	CEditGlobal::SkillDataDlg = this;
	
	m_vecCurLevelData.assign(5, LEVELDATA());
	//m_CurSkillCost.assign(5, 0);
	//m_vecCurHitData.clear();
	
}

CSkillDataDlg::~CSkillDataDlg()
{
}

void CSkillDataDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST2, m_SkillList);
	DDX_Text(pDX, IDC_SKILLDATA_SKILLNO, m_CurSkillNo);
	DDX_Control(pDX, IDC_SKILLDATA_CHARCOM, m_SkillCasterCombo);
	DDX_Control(pDX, IDC_SDTOTALTIMEEDIT, m_SkillTotalTime);
	DDX_Text(pDX, IDC_SDTOTALTIMEEDIT, m_strSkillTotalTime);
	DDX_Control(pDX, IDC_SKILLTYPECOM, m_SkillTypeCombo);
	DDX_Control(pDX, IDC_BUFFTYPECOM, m_BuffTypeCombo);
	DDX_Control(pDX, IDC_TEAMTARGETCOM, m_TeamTargetCombo);
	DDX_Control(pDX, IDC_ENEMYTARGETCOM, m_EnemyTargetCombo);
	DDX_Control(pDX, IDC_INITCHARPOSCOM, m_CasterInitPosCombo);
	DDX_Control(pDX, IDC_CHARFIXPOSXEDIT, m_CasterFixPosX);
	DDX_Text(pDX, IDC_CHARFIXPOSXEDIT, m_strCasterFixPosX);
	DDX_Control(pDX, IDC_CHARFIXPOSYEDIT, m_CasterFixPosY);
	DDX_Text(pDX, IDC_CHARFIXPOSYEDIT, m_strCasterFixPosY);
	DDX_Control(pDX, IDC_CHARPOSPIVOTXEDIT, m_PosPivotX);
	DDX_Text(pDX, IDC_CHARPOSPIVOTXEDIT, m_strPosPivotX);
	DDX_Control(pDX, IDC_CHARPOSPIVOTYEDIT, m_PosPivotY);
	DDX_Text(pDX, IDC_CHARPOSPIVOTYEDIT, m_strPosPivotY);
	DDX_Control(pDX, IDC_SKILLLEVELCOM, m_SkillLevelCombo);
	DDX_Control(pDX, IDC_SKILLCOUNTEDIT, m_SkillCount);
	DDX_Control(pDX, IDC_SKILLLCOSTCOM, m_SkillCostCombo);
	DDX_Control(pDX, IDC_SKILLCOSTEDIT, m_SkillCost);
	DDX_Text(pDX, IDC_SKILLCOUNTEDIT, m_uiSkillCount);
	DDX_Text(pDX, IDC_SKILLCOSTEDIT, m_uiSkillCost);
	DDX_Control(pDX, IDC_DEMAGESTYLECOM, m_DemageFontStyle);
	DDX_Control(pDX, IDC_OPTIONSLIDER, m_OptionSlider);
	DDX_Slider(pDX, IDC_OPTIONSLIDER, m_iDemageFontOption);
	DDX_Text(pDX, IDC_CURHITDATACOUNTEDIT, m_uiCurHitDataCount);
	DDX_Control(pDX, IDC_HITDATACOM, m_HitDataCombo);
	DDX_Control(pDX, IDC_HITSTARTTIMEEDIT, m_HitStartTime);
	DDX_Text(pDX, IDC_HITSTARTTIMEEDIT, m_strHitStartTime);
	DDX_Control(pDX, IDC_HITCOUNTEDIT, m_HitOverlapCount);
	DDX_Text(pDX, IDC_HITCOUNTEDIT, m_uiHitOverlapCount);
	DDX_Control(pDX, IDC_DEMAGEMAGEDIT, m_DemageMag);
	DDX_Text(pDX, IDC_DEMAGEMAGEDIT, m_uiDemageMag);
	DDX_Control(pDX, IDC_FIXDEMAGEEDIT, m_FixDemage);
	DDX_Text(pDX, IDC_FIXDEMAGEEDIT, m_uiFixDemage);
	DDX_Control(pDX, IDC_DEMAGEFONTMAGEDIT, m_DemageFontMag);
	DDX_Text(pDX, IDC_DEMAGEFONTMAGEDIT, m_strDemageFontMag);
	DDX_Control(pDX, IDC_SDHITEFFECTCOM, m_HitEffectCombo);
	DDX_Control(pDX, IDC_EFFECTPOSMODECOM, m_HitEffectPosModeCombo);
	DDX_Control(pDX, IDC_HITEFFECTPOSPIVOTXEDIT, m_HitEffectPivotX);
	DDX_Text(pDX, IDC_HITEFFECTPOSPIVOTXEDIT, m_strHitEffectPivotX);
	DDX_Control(pDX, IDC_HITEFFECTPOSPIVOTYEDIT, m_HitEffectPivotY);
	DDX_Text(pDX, IDC_HITEFFECTPOSPIVOTYEDIT, m_strHitEffectPivotY);
	DDX_Control(pDX, IDC_HITEFFEIMAGEMAGEDIT, m_EffectImageMag);
	DDX_Text(pDX, IDC_HITEFFEIMAGEMAGEDIT, m_strEffectImageMag);
	DDX_Control(pDX, IDC_HITEFFETSIZEXEDIT, m_HitEffectSizeX);
	DDX_Text(pDX, IDC_HITEFFETSIZEXEDIT, m_strHitEffectSizeX);
	DDX_Control(pDX, IDC_HITEFFETSIZEYEDIT, m_HitEffectSizeY);
	DDX_Text(pDX, IDC_HITEFFETSIZEYEDIT, m_strHitEffectSizeY);
	DDX_Control(pDX, IDC_HITSOUNDCOM, m_HitSoundCombo);
	DDX_Control(pDX, IDC_HITREACTIONCOM, m_HitReactionCombo);
	DDX_Control(pDX, IDC_BUFFDETAILTYPECOM, m_BuffDetailCombo);
	DDX_Control(pDX, IDC_BUFFVALUEEDIT, m_BuffValue);
	DDX_Text(pDX, IDC_BUFFVALUEEDIT, m_iBuffValue);
	DDX_Control(pDX, IDC_BUFFTURNCOUNTEDIT, m_BuffTurnCount);
	DDX_Text(pDX, IDC_BUFFTURNCOUNTEDIT, m_uiBuffTurnCount);
	DDX_Control(pDX, IDC_BUFFEFFECTCOM, m_BuffEffectCombo);
	DDX_Control(pDX, IDC_BUFFICONCOM, m_BuffIconCombo);
}


BEGIN_MESSAGE_MAP(CSkillDataDlg, CDialogEx)
	ON_WM_DESTROY()
	ON_WM_SHOWWINDOW()
	ON_LBN_SELCHANGE(IDC_LIST2, &CSkillDataDlg::OnLbnSelchangeList2)
	ON_EN_KILLFOCUS(IDC_SDTOTALTIMEEDIT, &CSkillDataDlg::OnEnKillfocusSdtotaltimeedit)
	ON_EN_KILLFOCUS(IDC_CHARFIXPOSXEDIT, &CSkillDataDlg::OnEnKillfocusCharfixposxedit)
	ON_EN_KILLFOCUS(IDC_CHARFIXPOSYEDIT, &CSkillDataDlg::OnEnKillfocusCharfixposyedit)
	ON_EN_KILLFOCUS(IDC_CHARPOSPIVOTXEDIT, &CSkillDataDlg::OnEnKillfocusCharpospivotxedit)
	ON_EN_KILLFOCUS(IDC_CHARPOSPIVOTYEDIT, &CSkillDataDlg::OnEnKillfocusCharpospivotyedit)
	ON_EN_KILLFOCUS(IDC_SKILLCOUNTEDIT, &CSkillDataDlg::OnEnKillfocusSkillcountedit)
	ON_EN_KILLFOCUS(IDC_SKILLCOSTEDIT, &CSkillDataDlg::OnEnKillfocusSkillcostedit)
	ON_EN_KILLFOCUS(IDC_HITSTARTTIMEEDIT, &CSkillDataDlg::OnEnKillfocusHitstarttimeedit)
	ON_EN_KILLFOCUS(IDC_HITCOUNTEDIT, &CSkillDataDlg::OnEnKillfocusHitcountedit)
	ON_EN_KILLFOCUS(IDC_DEMAGEMAGEDIT, &CSkillDataDlg::OnEnKillfocusDemagemagedit)
	ON_EN_KILLFOCUS(IDC_FIXDEMAGEEDIT, &CSkillDataDlg::OnEnKillfocusFixdemageedit)
	ON_EN_KILLFOCUS(IDC_DEMAGEFONTMAGEDIT, &CSkillDataDlg::OnEnKillfocusDemagefontmagedit)
	ON_EN_KILLFOCUS(IDC_HITEFFECTPOSPIVOTXEDIT, &CSkillDataDlg::OnEnKillfocusHiteffectpospivotxedit)
	ON_EN_KILLFOCUS(IDC_HITEFFECTPOSPIVOTYEDIT, &CSkillDataDlg::OnEnKillfocusHiteffectpospivotyedit)
	ON_EN_KILLFOCUS(IDC_HITEFFEIMAGEMAGEDIT, &CSkillDataDlg::OnEnKillfocusHiteffeimagemagedit)
	ON_EN_KILLFOCUS(IDC_HITEFFETSIZEXEDIT, &CSkillDataDlg::OnEnKillfocusHiteffetsizexedit)
	ON_EN_KILLFOCUS(IDC_HITEFFETSIZEYEDIT, &CSkillDataDlg::OnEnKillfocusHiteffetsizeyedit)
	ON_EN_KILLFOCUS(IDC_BUFFVALUEEDIT, &CSkillDataDlg::OnEnKillfocusBuffvalueedit)
	ON_EN_KILLFOCUS(IDC_BUFFTURNCOUNTEDIT, &CSkillDataDlg::OnEnKillfocusBuffturncountedit)
	ON_BN_CLICKED(IDC_SKILLDATACREATEBUTTON, &CSkillDataDlg::OnBnClickedSkilldatacreatebutton)
	ON_BN_CLICKED(IDC_SKILLDATADELETEBUTTON, &CSkillDataDlg::OnBnClickedSkilldatadeletebutton)
	ON_BN_CLICKED(IDC_SDSKILLLOADEBUTTON, &CSkillDataDlg::OnBnClickedSdskillloadebutton)
	ON_BN_CLICKED(IDC_SDSKILLSAVEEBUTTON, &CSkillDataDlg::OnBnClickedSdskillsaveebutton)
	ON_CBN_SELCHANGE(IDC_SKILLDATA_CHARCOM, &CSkillDataDlg::OnCbnSelchangeSkilldataCharcom)
	ON_CBN_SELCHANGE(IDC_BUFFTYPECOM, &CSkillDataDlg::OnCbnSelchangeBufftypecom)
	ON_CBN_SELCHANGE(IDC_TEAMTARGETCOM, &CSkillDataDlg::OnCbnSelchangeTeamtargetcom)
	ON_CBN_SELCHANGE(IDC_INITCHARPOSCOM, &CSkillDataDlg::OnCbnSelchangeInitcharposcom)
	ON_CBN_SELCHANGE(IDC_ENEMYTARGETCOM, &CSkillDataDlg::OnCbnSelchangeEnemytargetcom)
	ON_CBN_SELCHANGE(IDC_SKILLLEVELCOM, &CSkillDataDlg::OnCbnSelchangeSkilllevelcom)
	ON_CBN_SELCHANGE(IDC_SDHITEFFECTCOM, &CSkillDataDlg::OnCbnSelchangeSdhiteffectcom)
	ON_CBN_SELCHANGE(IDC_HITSOUNDCOM, &CSkillDataDlg::OnCbnSelchangeHitsoundcom)
	ON_BN_CLICKED(IDC_LEVELREGBUTTON, &CSkillDataDlg::OnBnClickedLevelregbutton)
	ON_BN_CLICKED(IDC_LEVELDELBUTTON, &CSkillDataDlg::OnBnClickedLeveldelbutton)
	ON_BN_CLICKED(IDC_COSTREGBUTTON, &CSkillDataDlg::OnBnClickedCostregbutton)
	ON_BN_CLICKED(IDC_COSTDELBUTTON, &CSkillDataDlg::OnBnClickedCostdelbutton)
	ON_BN_CLICKED(IDC_HITREGBUTTON, &CSkillDataDlg::OnBnClickedHitregbutton)
	ON_BN_CLICKED(IDC_HITDELBUTTON, &CSkillDataDlg::OnBnClickedHitdelbutton)
	ON_CBN_SELCHANGE(IDC_SKILLLCOSTCOM, &CSkillDataDlg::OnCbnSelchangeSkilllcostcom)
	ON_CBN_SELCHANGE(IDC_HITDATACOM, &CSkillDataDlg::OnCbnSelchangeHitdatacom)
	ON_CBN_SELCHANGE(IDC_DEMAGESTYLECOM, &CSkillDataDlg::OnCbnSelchangeDemagestylecom)
	ON_WM_HSCROLL()
	ON_CBN_SELCHANGE(IDC_SKILLTYPECOM, &CSkillDataDlg::OnCbnSelchangeSkilltypecom)
END_MESSAGE_MAP()


// CSkillDataDlg 메시지 처리기입니다.
void CSkillDataDlg::InitSkillDataDlg()
{
	for (size_t i = 0; i < CEditGlobal::CharacterName.size(); i++)
	{
		m_SkillCasterCombo.AddString(CEditGlobal::CharacterName[i]);
	}

	m_SkillCasterCombo.AddString(_T("Monster"));
	
	m_HitEffectCombo.ResetContent();

	CGameFile LoadFile(_T("EditData"), _T("EffectData.bin"));

	if (FALSE == LoadFile.Open(_T("rb")))
	{
		MessageBox(_T("EffectDataFile을 열지 못하였습니다."));
		return;
	}


	vector<EFFECTDATA> TempVec;
	TempVec.reserve(128);
	EFFECTDATA::LoadDataToFile(LoadFile.GetFile(), &TempVec);

	CString AddEffectString = _T("");
	m_HitEffectCombo.AddString(_T("  No Effect"));

	for (size_t i = 0; i < TempVec.size(); i++)
	{
		AddEffectString.Format(_T("%3d: %s"), TempVec[i].EffectIndex, TempVec[i].EffectName);
		m_HitEffectCombo.AddString(AddEffectString);
	}

	CActObject* Speaker = CActObject::CreateActObject(_T("SkillDataSpeaker"), CEditGlobal::SkillScene);
	m_SkillDataSpeaker = Speaker->CreateCom<CSoundPlayer>();


}
void CSkillDataDlg::InitCurSkillData()
{
	m_CurSkillData = SKILLDATA();

	m_vecCurLevelData.assign(5, LEVELDATA());

	
	InitAllCombo();
	FillSkillData();

	m_CurSkillData.CharacterSkill = false;
	m_fSkillTotalTime = 1.f;
	m_strSkillTotalTime.Format(_T("%.2f"), m_fSkillTotalTime);
	UpdateData(FALSE);

	m_SkillCasterCombo.EnableWindow(FALSE);
	m_SkillTotalTime.EnableWindow(FALSE);

	CEditGlobal::EditSkillMgr->CurCharacterOff();
	CEditGlobal::EditSkillMgr->SkillStandby(false);
	CEditGlobal::EditSkillMgr->MonTargetUnitOn();
	
	m_SkillCasterCombo.SetCurSel(m_SkillCasterCombo.GetCount() - 1);
	ClickCasterComboEvent();

	m_CurSkillData.SkillType = SKILLDATA::SKILLTYPE_ACTION;
	m_CurSkillData.SkillBuffType = SKILLDATA::BUFFTYPE_NONE;
	m_CurSkillData.TeamTargetType = SKILLDATA::TARGET_NONE;
	m_CurSkillData.EnemyTargetType = SKILLDATA::TARGET_SINGLE;
	m_CurSkillData.InitPosMode = SKILLDATA::INITPOS_NONE;

	m_SkillTypeCombo.SetCurSel(m_CurSkillData.SkillType);
	m_BuffTypeCombo.SetCurSel(m_CurSkillData.SkillBuffType);
	EnableBuffControl(FALSE);
	m_TeamTargetCombo.SetCurSel(m_CurSkillData.TeamTargetType);
	m_EnemyTargetCombo.SetCurSel(m_CurSkillData.EnemyTargetType);
	m_CasterInitPosCombo.SetCurSel(m_CurSkillData.InitPosMode);
	OnCbnSelchangeInitcharposcom();

	m_SkillLevelCombo.SetCurSel(m_SkillLevelCombo.GetCount() - 1);
	m_SkillCostCombo.SetCurSel(LEVELDATA::SKILLCOST_MP);
	SkillLevelClickEvent(m_SkillLevelCombo.GetCurSel());
	SkillCostClickEvent(m_SkillCostCombo.GetCurSel());


	m_DemageFontStyle.SetCurSel(0);
	DemageStyleClickEvent(m_DemageFontStyle.GetCurSel());

	m_HitDataCombo.SetCurSel(0);
	OnCbnSelchangeHitdatacom();

	m_HitEffectCombo.SetCurSel(0);
	HitEffectListClickEvent(m_HitEffectCombo.GetCurSel());

	m_HitSoundCombo.SetCurSel(0);
	m_HitReactionCombo.SetCurSel(1);
}
void CSkillDataDlg::ClickSkillListEvent(const int& _ListIndex) 
{
	CString ListString;

	UpdateData(TRUE);
	m_SkillList.GetText(_ListIndex, ListString);
	UpdateData(FALSE);

	int SkillNo = ListStringToSkillNo(ListString);

	SKILL Temp = CEditGlobal::EditSkillMgr->GetSkill(SkillNo);

	// SkillMgr에서 받은 스킬의 SkillData와 
	m_CurSkillData = Temp.SkillData;

	vector<LEVELDATA>* pLevel = CEditGlobal::EditSkillMgr->GetVecLevelData(SkillNo);

	if (nullptr == pLevel)
		return;

	m_vecCurLevelData.assign(pLevel->begin(), pLevel->end());


	if (0.0f != Temp.SkillActionScript.TotalSkillTime) 
	{
		UpdateData(TRUE);
		m_fSkillTotalTime = Temp.SkillActionScript.TotalSkillTime;
		UpdateData(FALSE);
	}

	FillSkillData();
	m_HitDataCombo.SetCurSel(0);
	m_HitEffectCombo.SetCurSel(0);
	EnableEffectControl(FALSE);
	m_HitSoundCombo.SetCurSel(0);
	m_HitReactionCombo.SetCurSel(1);

	bool Character = m_CurSkillData.CharacterSkill;
	
	if (true == Character) 
		m_SkillCasterCombo.SetCurSel(Temp.SkillActionScript.CharIndex);
	else 
		m_SkillCasterCombo.SetCurSel(m_SkillCasterCombo.GetCount() -1 );
	
	bool ScriptEmpty = Temp.SkillActionScript.isEmpty();


	m_SkillCasterCombo.EnableWindow(FALSE);
	m_SkillTotalTime.EnableWindow(FALSE);

	//if (true == ScriptEmpty)
	//{
	//	m_SkillCasterCombo.EnableWindow(TRUE);
	//	m_SkillTotalTime.EnableWindow(TRUE);
	//
	//}
	//else
	//{
	//	m_SkillCasterCombo.EnableWindow(FALSE);
	//	m_SkillTotalTime.EnableWindow(FALSE);
	//}
	//OnCbnSelchangeSkilldataCharcom();

	SetSkillTestUnit();

	CEditGlobal::EditSkillMgr->SetSkill(m_CurSkillNo);
}
void CSkillDataDlg::ClearSkillList() 
{
	m_SkillList.ResetContent();
	m_SkillList.AddString(_T("새로 만들기"));
}
void CSkillDataDlg::ClearHitDataList()
{
	int CurCount = m_HitDataCombo.GetCount();

	if (CurCount <= 1)
		return;

	for (size_t i = CurCount - 1; i >= 1; --i)
		m_HitDataCombo.DeleteString((UINT)i);
	

}
void CSkillDataDlg::FillSkillData()
{
	// 아무것도 지정안된 스킬 Data의 경우 값을 초기화값으로 셋팅한다.
	if (-1 == m_CurSkillData.SkillNo || -1 == m_CurSkillData.SkillType) 
	{
		UpdateData(TRUE);
		m_CurSkillNo = CEditGlobal::EditSkillMgr->SearchNextIndex();
		m_CurSkillData.SkillNo = m_CurSkillNo;
		for (size_t i = 0; i < LEVELDATA::MAXSKILLLEVEL; i++)
			m_vecCurLevelData[i].SkillNo = m_CurSkillNo;
		m_fSkillTotalTime = 0.0f;
		m_strSkillTotalTime.Format(_T("%.2f"), m_fSkillTotalTime);
		m_fCasterFixPosX = 0.0f;
		m_strCasterFixPosX.Format(_T("%.2f"), m_fCasterFixPosX);
		m_fCasterFixPosY = 0.0f;
		m_strCasterFixPosY.Format(_T("%.2f"), m_fCasterFixPosY);
		m_fPosPivotX = 0.0f;
		m_strPosPivotX.Format(_T("%.2f"), m_fPosPivotX);
		m_fPosPivotY = 0.0f;
		m_strPosPivotY.Format(_T("%.2f"), m_fPosPivotY);
		m_uiSkillCount = 0;
		m_uiSkillCost = 0;
		m_fHitStartTime = 0.0f;
		m_strHitStartTime.Format(_T("%.2f"), m_fHitStartTime);
		m_uiHitOverlapCount = 1;
		m_uiDemageMag = 0;
		m_uiFixDemage = 0;
		m_fDemageFontMag = 0.0f;
		m_strDemageFontMag.Format(_T("%.2f"), m_fDemageFontMag);
		m_fHitEffectPivotX = 0.0f;
		m_strHitEffectPivotX.Format(_T("%.2f"), m_fHitEffectPivotX);
		m_fHitEffectPivotY = 0.0f;
		m_strHitEffectPivotY.Format(_T("%.2f"), m_fHitEffectPivotY);
		m_fEffectImageMag = 1.0f;
		m_strEffectImageMag.Format(_T("%.2f"), m_fEffectImageMag);
		m_fHitEffectSizeX = 0.0f;
		m_strHitEffectSizeX.Format(_T("%.2f"), m_fHitEffectSizeX);
		m_fHitEffectSizeY = 0.0f;
		m_strHitEffectSizeY.Format(_T("%.2f"), m_fHitEffectSizeY);
		m_iBuffValue = 0;
		m_uiBuffTurnCount = 0;
		UpdateData(FALSE);
	}
	else 
	{
		UpdateData(TRUE);
		m_CurSkillNo = m_CurSkillData.SkillNo;
		m_strSkillTotalTime.Format(_T("%.2f"), m_fSkillTotalTime);
		m_fCasterFixPosX = m_CurSkillData.InitFixPos.x;
		m_strCasterFixPosX.Format(_T("%.2f"), m_fCasterFixPosX);
		m_fCasterFixPosY = m_CurSkillData.InitFixPos.y;
		m_strCasterFixPosY.Format(_T("%.2f"), m_fCasterFixPosY);
		m_fPosPivotX = m_CurSkillData.InitPosPivot.x;
		m_strPosPivotX.Format(_T("%.2f"), m_fPosPivotX);
		m_fPosPivotY = m_CurSkillData.InitPosPivot.y;
		m_strPosPivotY.Format(_T("%.2f"), m_fPosPivotY);
		m_SkillTypeCombo.SetCurSel(m_CurSkillData.SkillType);
		m_BuffTypeCombo.SetCurSel(m_CurSkillData.SkillBuffType);
		m_TeamTargetCombo.SetCurSel(m_CurSkillData.TeamTargetType);
		m_EnemyTargetCombo.SetCurSel(m_CurSkillData.EnemyTargetType);
		m_CasterInitPosCombo.SetCurSel(m_CurSkillData.InitPosMode);
		UpdateData(FALSE);
	
		if (NONEBUFF == m_CurSkillData.SkillBuffType)
			EnableBuffControl(FALSE);
		else
			EnableBuffControl(TRUE);

		if (SKILLDATA::INITPOS_FIXPOS != m_CurSkillData.InitPosMode)
			EnableFixPosControl(FALSE);
		else
			EnableFixPosControl(TRUE);
	}

	
}
void CSkillDataDlg::FillSkillDetailData(const int& _Level)
{
	if (_Level >= ALLLEVEL)
		return;

	m_uiCurLevel = _Level;
	m_CurLevelData = m_vecCurLevelData[m_uiCurLevel];

	UpdateHitDataList();

	
	UINT DemStyle = m_CurLevelData.DemageStyle;
	int Option = m_CurLevelData.Option;

	if (DemStyle < (UINT)m_DemageFontStyle.GetCount())
		m_DemageFontStyle.SetCurSel(DemStyle);

	m_OptionSlider.SetPos(Option);

	UpdateData(TRUE);
	m_uiSkillCount = m_CurLevelData.SkillCount;
	int CostType = m_SkillCostCombo.GetCurSel();
	if (CostType < LEVELDATA::MAXSKILLCOST)
		m_uiSkillCost = m_CurLevelData.SkillCost[CostType];
	m_uiCurHitDataCount = (UINT)m_CurLevelData.HitDataCount;
	UpdateData(FALSE);

	FillBuffData();
}
void CSkillDataDlg::FillBuffData()
{
	BUFFDATA Buff = m_CurLevelData.BuffData;
	
	if (-1 == Buff.BuffType)
		return;

	UpdateData(TRUE);
	if (Buff.BuffType < BUFFDETAIL)
		m_BuffDetailCombo.SetCurSel(Buff.BuffType);

	m_iBuffValue = (UINT)(Buff.BuffValue*100.0f);
	m_uiBuffTurnCount = Buff.BuffCount;
	
	if (MAXBUFFEFFECTINDEX > Buff.BuffEffectIndex)
		m_BuffEffectCombo.SetCurSel(Buff.BuffEffectIndex);
	
	if (MAXBUFFICON > Buff.BuffIconIndex)
		m_BuffIconCombo.SetCurSel(Buff.BuffIconIndex);

	UpdateData(FALSE);
}
void CSkillDataDlg::InitHitDataCtrl()
{
	UpdateData(TRUE);

	UpdateData(FALSE);
}
void CSkillDataDlg::InitAllCombo()
{
	InitRegCombo();
	InitTypeCombo();
}
void CSkillDataDlg::InitRegCombo()
{
	UpdateData(TRUE);
	m_HitDataCombo.ResetContent();
	m_HitDataCombo.AddString(_T(" New"));
	m_uiCurHitDataCount = 0;
	UpdateData(FALSE);
}
void CSkillDataDlg::InitTypeCombo()
{
	UpdateData(TRUE);
	m_SkillTypeCombo.SetCurSel(-1);
	m_BuffTypeCombo.SetCurSel(-1);
	m_TeamTargetCombo.SetCurSel(-1);
	m_EnemyTargetCombo.SetCurSel(-1);
	m_CasterInitPosCombo.SetCurSel(-1);
	m_SkillLevelCombo.SetCurSel(-1);
	m_SkillCostCombo.SetCurSel(-1);
	m_DemageFontStyle.SetCurSel(-1);
	m_OptionSlider.SetPos(0);
	m_HitEffectCombo.SetCurSel(-1);
	m_HitEffectPosModeCombo.SetCurSel(-1);
	m_HitSoundCombo.SetCurSel(-1);
	m_HitReactionCombo.SetCurSel(-1);
	m_BuffDetailCombo.SetCurSel(-1);
	m_BuffEffectCombo.SetCurSel(-1);
	m_BuffIconCombo.SetCurSel(-1);
	UpdateData(FALSE);


}
void CSkillDataDlg::ChangeEditEvent(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	unordered_map<UINT, EditSet>::iterator Find = m_mapEditSet.find(nID);

	// EditSetMap 에서 해당 Edit컨트롤을 찾지못할경우 함수 종료
	if (Find == m_mapEditSet.end())
		return;

	ChangeEditString(Find->second.EditCtrl, Find->second.EditString);

}
void CSkillDataDlg::ChangeEditString(CEdit* _Edit, CString* _String)
{
	if (nullptr == _Edit || nullptr == _String)
		return;


	int Sel = _Edit->GetSel();
	CEditGlobal::ChangeFloatString(_String);
	_Edit->SetSel(Sel);

}
void CSkillDataDlg::CheckTimeString(CEdit& _EditCtrl, CString& _TimeStr, float& _Time) 
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
void CSkillDataDlg::CheckFloatString(CEdit& _EditCtrl, CString& _TimeStr, float& _Value)
{
	if (nullptr == &_EditCtrl || nullptr == &_TimeStr || nullptr == &_Value)
		return;

	UpdateData(TRUE);
	float Value = CGameString::StringToFloat(_TimeStr.GetString());

	_Value = Value;
	_TimeStr.Format(_T("%.2f"), _Value);
	
	UpdateData(FALSE);
}
void CSkillDataDlg::CheckPosString(CEdit& _EditCtrl, CString& _PosStr, float& _Pos) 
{
	if (nullptr == &_EditCtrl || nullptr == &_PosStr || nullptr == &_Pos)
		return;

	UpdateData(TRUE);

	float Pos = CGameString::StringToFloat(_PosStr.GetString());

	_Pos = Pos;
	_PosStr.Format(_T("%.2f"), _Pos);

	UpdateData(FALSE);
}
BOOL CSkillDataDlg::EditBoxClickCheck(MSG* pMsg) 
{
	for (size_t i = 0; i < m_vecEdit.size(); i++)
	{
		if (pMsg->hwnd == m_vecEdit[i]->m_hWnd
			&& GetFocus()->m_hWnd != m_vecEdit[i]->m_hWnd)
		{
			m_vecEdit[i]->SetFocus();
			m_vecEdit[i]->SetSel(0, -1, true);
			return TRUE;
		}
	}
	return FALSE;
}
BOOL CSkillDataDlg::CheckButtonEvent(MSG* pMsg) 
{
	return TRUE;
}
void CSkillDataDlg::EnableBuffControl(const BOOL& _Mode)
{
	UpdateData(TRUE);
	m_BuffDetailCombo.EnableWindow(_Mode);
	m_BuffValue.EnableWindow(_Mode);
	m_BuffEffectCombo.EnableWindow(_Mode);
	m_BuffTurnCount.EnableWindow(_Mode);
	m_BuffIconCombo.EnableWindow(_Mode);
	m_iBuffValue = 0;
	m_uiBuffTurnCount = 0;
	UpdateData(FALSE);

}
void CSkillDataDlg::EnableEffectControl(const BOOL& _Mode)
{
	UpdateData(TRUE);

	if (FALSE == _Mode)
		m_HitEffectPosModeCombo.SetCurSel(-1);
	else
		m_HitEffectPosModeCombo.SetCurSel(0);
	
	m_HitEffectPosModeCombo.EnableWindow(_Mode);
	m_HitEffectPivotX.EnableWindow(_Mode);
	m_HitEffectPivotY.EnableWindow(_Mode);
	m_EffectImageMag.EnableWindow(_Mode);
	m_HitEffectSizeX.EnableWindow(_Mode);
	m_HitEffectSizeY.EnableWindow(_Mode);
	UpdateData(FALSE);
}
void CSkillDataDlg::EnableFixPosControl(const BOOL& _Mode)
{
	UpdateData(TRUE);
	m_CasterFixPosX.EnableWindow(_Mode);
	m_CasterFixPosY.EnableWindow(_Mode);
	UpdateData(FALSE);
}
void CSkillDataDlg::ClickCasterComboEvent()
{
	int CurIndex = m_SkillCasterCombo.GetCurSel();

	if (nullptr != CEditGlobal::EditSkillMgr && STATE_COMPLETE == CEditGlobal::EditSkillMgr->GetSkillMgrState()) 
	{
		if (CurIndex < MONSTER) 
		{
			CEditGlobal::EditSkillMgr->CharacterOn((CharacterIndex)CurIndex);
			CEditGlobal::EditSkillMgr->MonTargetUnitOff();
			m_CurSkillData.CharacterSkill = true;
		}
		else 
		{
			CEditGlobal::EditSkillMgr->MonTargetUnitOn();
			CEditGlobal::EditSkillMgr->CurCharacterOff();
			m_CurSkillData.CharacterSkill = false;
			if (m_fSkillTotalTime <= 0.f) 
			{
				m_fSkillTotalTime = 1.f;
				m_strSkillTotalTime.Format(_T("%.2f"), m_fSkillTotalTime);
				UpdateData(FALSE);
			}
			
		}
	}

	m_CurCharNo = CurIndex;
}
void CSkillDataDlg::UpdateSkillList()
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
				SkillName.Format(_T("%d) %s"), (*SkillVec)[i].SkillNo, (*SkillVec)[i].SkillName);
			

			m_SkillList.AddString(SkillName);
		}
	}
}
void CSkillDataDlg::UpdateHitDataList()
{
	ClearHitDataList();

	for (size_t i = 0; i < m_CurLevelData.HitDataCount; i++)
	{
		CString AddStr;
		AddStr.Format(_T("[%2d] : %.2f"), i, m_CurLevelData.HitData[i].StartTime);
		m_HitDataCombo.AddString(AddStr);
	}
}
const int CSkillDataDlg::ListStringToSkillNo(CString& _ListString) 
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
BOOL CSkillDataDlg::AddHitData(const HITDATA& _Data)
{
	if (nullptr == &_Data)
	{
		assert(nullptr);
		return FALSE;
	}

	if (false == m_CurLevelData.AddHitData(_Data))
	{
		MessageBox(_T("더 이상 HitData를 등록할 수 없습니다."));
		return FALSE;
	}

	UpdateData(TRUE);
	m_uiCurHitDataCount = m_CurLevelData.HitDataCount;
	UpdateData(FALSE);

	return TRUE;
}
BOOL CSkillDataDlg::ChangeHitData(const HITDATA& _Data,const int& _HitIndex)
{
	if (nullptr == &_Data || nullptr == &_HitIndex)
	{
		assert(nullptr);
		return FALSE;
	}

	if ((UINT)_HitIndex >= m_CurLevelData.HitDataCount)
	{
		MessageBox(_T("등록되지 않은 Index가 입력으로 들어왔습니다."));
		return FALSE;
	}

	m_CurLevelData.HitData[_HitIndex] = _Data;
	return TRUE;
}
BOOL CSkillDataDlg::SkillTypeComboClickEvent(const int& _Index)
{
	if (_Index >= SKILLDATA::MAXSKILLTYPE)
		return FALSE;

	m_CurSkillData.SkillType = _Index;

	return TRUE;
}
BOOL CSkillDataDlg::BuffTypeComboClickEvent(const int& _Index)
{
	if (_Index >= m_BuffTypeCombo.GetCount())
		return FALSE;

	m_BuffTypeCombo.SetCurSel(_Index);

	
	EnableBuffControl(TRUE);
	
	if ( NONEBUFF == _Index)
		EnableBuffControl(FALSE);
	
	return TRUE;
}
BOOL CSkillDataDlg::TeamTargetClickEvent(const int& _Index)
{
	if (_Index >= SKILLDATA::MAXTARGET)
		return FALSE;

	m_CurSkillData.TeamTargetType = _Index;

	return TRUE;
}
BOOL CSkillDataDlg::EnemyTargetClickEvent(const int& _Index)
{
	if (_Index >= SKILLDATA::MAXTARGET)
		return FALSE;

	m_CurSkillData.EnemyTargetType = _Index;
	return TRUE;
}
BOOL CSkillDataDlg::CasterInitPosModeClickEvent(const int& _Index)
{
	if (_Index >= SKILLDATA::MAXINITPOS)
		return FALSE;

	m_CurSkillData.InitPosMode = _Index;

	EnableFixPosControl(TRUE);

	if (SKILLDATA::INITPOS_FIXPOS != _Index) 
	{
		UpdateData(TRUE);
		m_fCasterFixPosX = 0.0f;
		m_strCasterFixPosX.Format(_T("%.2f"), m_fCasterFixPosX);
		m_fCasterFixPosY = 0.0f;
		m_strCasterFixPosY.Format(_T("%.2f"), m_fCasterFixPosY);
		UpdateData(FALSE);
		EnableFixPosControl(FALSE);
	}
	return TRUE;
}
BOOL CSkillDataDlg::SkillLevelClickEvent(const int& _Index)
{
	if (_Index >= m_SkillLevelCombo.GetCount())
		return FALSE;

	if (_Index < ALLLEVEL) 
	{
		FillSkillDetailData(_Index);
		CEditGlobal::EditSkillMgr->SetCurSkillLevel(_Index);
	}

	return TRUE;
}
BOOL CSkillDataDlg::SkillCostClickEvent(const int& _Index)
{
	if (_Index >= LEVELDATA::MAXSKILLCOST)
		return FALSE;

	UpdateData(TRUE);
	m_uiSkillCost = m_CurLevelData.SkillCost[_Index];
	UpdateData(FALSE);

	return TRUE;
}
BOOL CSkillDataDlg::DemageStyleClickEvent(const int& _Index)
{
	if (_Index >= MAXDEMAGESTYLE)
		return FALSE;

	m_CurLevelData.DemageStyle = _Index;

	return TRUE;
}
BOOL CSkillDataDlg::OptionSliderEvent(const int& _Option) 
{
	m_CurLevelData.Option = _Option;
	return TRUE;
}
BOOL CSkillDataDlg::HitDataListClickEvent(const int& _Index)
{

	if (m_CurLevelData.HitDataCount <= (UINT)_Index)
		return FALSE;

	HITDATA Data(m_CurLevelData.HitData[_Index]);

	UpdateData(TRUE);
	m_fHitStartTime = Data.StartTime;
	m_strHitStartTime.Format(_T("%.2f"), m_fHitStartTime);
	m_uiHitOverlapCount = Data.HitCount;
	m_uiDemageMag =(UINT)(Data.HitDemageMag * 100.0f);
	m_uiFixDemage = Data.FixHitDemage;
	m_fDemageFontMag = Data.DemageFontMag;
	m_strDemageFontMag.Format(_T("%.2f"), m_fDemageFontMag);
	UpdateData(FALSE);

	UINT EffectIndex = Data.HitEffectIndex;
	
	// Effect가 등록되지 않았다면 이팩트 관련 컨트롤을 다꺼라
	if (-1 == EffectIndex) 
	{
		m_HitEffectCombo.SetCurSel(0);
		EnableEffectControl(FALSE);
		
		UpdateData(TRUE);
		m_fHitEffectPivotX = 0.0f;
		m_strHitEffectPivotX.Format(_T("%.2f"), m_fHitEffectPivotX);
		m_fHitEffectPivotY = 0.0f;
		m_strHitEffectPivotY.Format(_T("%.2f"), m_fHitEffectPivotY);
		m_fEffectImageMag = 0.0f;
		m_strEffectImageMag.Format(_T("%.2f"), m_fEffectImageMag);
		m_fHitEffectSizeX = 0.0f;
		m_strHitEffectSizeX.Format(_T("%.2f"), m_fHitEffectSizeX);
		m_fHitEffectSizeY = 0.0f;
		m_strHitEffectSizeY.Format(_T("%.2f"), m_fHitEffectSizeY);
		UpdateData(FALSE);
	}
	else 
	{
		EnableEffectControl(TRUE);
		
		if (Data.PosOption < (UINT)m_HitEffectPosModeCombo.GetCount())
			m_HitEffectPosModeCombo.SetCurSel(Data.PosOption);

		if (EffectIndex < (UINT)m_HitEffectCombo.GetCount())
			m_HitEffectCombo.SetCurSel(EffectIndex + 1);

		UpdateData(TRUE);
		m_fHitEffectPivotX = Data.HitEffectPivot.x;
		m_strHitEffectPivotX.Format(_T("%.2f"), m_fHitEffectPivotX);
		m_fHitEffectPivotY = Data.HitEffectPivot.y;
		m_strHitEffectPivotY.Format(_T("%.2f"), m_fHitEffectPivotY);
		m_fEffectImageMag = Data.HitEffectImageMag;
		m_strEffectImageMag.Format(_T("%.2f"), m_fEffectImageMag);
		m_fHitEffectSizeX = Data.HitEffectCustomSize.x;
		m_strHitEffectSizeX.Format(_T("%.2f"), m_fHitEffectSizeX);
		m_fHitEffectSizeY = Data.HitEffectCustomSize.y;
		m_strHitEffectSizeY.Format(_T("%.2f"), m_fHitEffectSizeY);
		UpdateData(FALSE);
	}


	if (-1 == Data.HitSoundIndex)
		m_HitSoundCombo.SetCurSel(0);
	else
	for (UINT i = 0; i < (UINT)m_vecHitSoundIndex.size(); i++)
	{
		if (Data.HitSoundIndex == m_vecHitSoundIndex[i]) 
		{
			m_HitSoundCombo.SetCurSel(i + 1);
			break;
		}
	}

	if (Data.ReActionIndex < (UINT)m_HitReactionCombo.GetCount())
		m_HitReactionCombo.SetCurSel(Data.ReActionIndex + 1);
	else 
			m_HitReactionCombo.SetCurSel(0);

	return TRUE;
}
BOOL CSkillDataDlg::HitEffectListClickEvent(const int& _Index)
{
	// 만일 유효하지 않은 Index 클릭시 -1로 셋팅하고 
	// Effect를 출력하지 않은 것으로 간주하여 
	// Effect Window를 Enable 한다.

	if ( 0 == _Index) 
		EnableEffectControl(FALSE);
	else 
	{
		EnableEffectControl(TRUE);
		UpdateData(TRUE);
		if (0.0f >= m_fEffectImageMag)
			m_fEffectImageMag = 1.0f;
		UpdateData(FALSE);
	}

	return TRUE;
}
BOOL CSkillDataDlg::PlaySoundEvent(const int& _Index)
{
	if (m_vecHitSoundIndex.size() <= _Index)
		return FALSE;

	if (nullptr == m_SkillDataSpeaker)
		return FALSE;

	CString SoundName;
	SoundName.Format(_T("Hit%d"), m_vecHitSoundIndex[_Index]);
	m_SkillDataSpeaker->PlaySound(SoundName.GetString());
	return TRUE;
}
BOOL CSkillDataDlg::LevelDataRegEvent(const int& _Index) 
{
	if (_Index > ALLLEVEL)
		return FALSE;

	if (_Index < ALLLEVEL) 
	{
		UpdateData(TRUE);
		m_CurLevelData.SkillNo = m_CurSkillNo;
		UpdateData(FALSE);
		m_vecCurLevelData[_Index] = m_CurLevelData;
		int Level = _Index + 1;
		CString Msg;
		Msg.Format(_T("Skill Level%2d의 데이터를 갱신하였습니다"), Level);
		MessageBox(Msg.GetString());
	}
	else 
	{
		UpdateData(TRUE);
		m_CurLevelData.SkillNo = m_CurSkillNo;
		UpdateData(FALSE);

		for (size_t i = 0; i < ALLLEVEL; i++)
			m_vecCurLevelData[i] = m_CurLevelData;

		MessageBox(_T("모든  Level 데이터를 갱신하였습니다"));
	}

	return TRUE;
}
BOOL CSkillDataDlg::LevelDataDelEvent(const int& _Index) 
{
	if (_Index > ALLLEVEL)
		return FALSE;

	if (_Index < ALLLEVEL)
	{
		m_vecCurLevelData[_Index] = LEVELDATA();
		MessageBox(_T("Skill Level %2d의 데이터를 초기화하였습니다"));
	}
	else
	{
		for (size_t i = 0; i < ALLLEVEL; i++)
			m_vecCurLevelData[i] = LEVELDATA();

		MessageBox(_T("모든  Level 데이터를 초기화하였습니다"));
	}

	return TRUE;
}
BOOL CSkillDataDlg::SkillCostRegEvent(const int& _Index) 
{
	if (_Index >= LEVELDATA::MAXSKILLCOST)
		return FALSE;

	m_CurLevelData.SkillCost[_Index] = m_uiSkillCost;

	return TRUE;
}
BOOL CSkillDataDlg::SkillCostDelEvent(const int& _Index) 
{
	if (_Index >= LEVELDATA::MAXSKILLCOST)
		return FALSE;

	m_CurLevelData.SkillCost[_Index] = 0;

	UpdateData(TRUE);
	m_uiSkillCost = 0;
	UpdateData(FALSE);

	return TRUE;
}
BOOL CSkillDataDlg::HitDataRegEvent(const int& _Index)
{
	if ((UINT)_Index > m_CurLevelData.HitDataCount)
	{
		MessageBox(_T("유효하지 않는 Hit Data Index가 들어왔습니다."));
		return FALSE;
	}
	
	HITDATA Data = HITDATA();

	UpdateData(TRUE);
	Data.StartTime = m_fHitStartTime;
	Data.HitCount = m_uiHitOverlapCount;
	Data.HitDemageMag = (float)m_uiDemageMag * 0.01f;
	Data.FixHitDemage = m_uiFixDemage;
	Data.DemageFontMag = m_fDemageFontMag;
	
	int EffectNo = m_HitEffectCombo.GetCurSel();
	--EffectNo;
	
	if (EffectNo < 0)
		Data.InitHitEffect();
	else
	{
		Data.HitEffectIndex = EffectNo;
		Data.PosOption = m_HitEffectPosModeCombo.GetCurSel();
		Data.HitEffectPivot = Vec2{ m_fHitEffectPivotX,m_fHitEffectPivotY };
		Data.HitEffectImageMag = m_fEffectImageMag;
		if (0.0f != m_fHitEffectSizeX && 0.0f != m_fHitEffectSizeY)
			Data.HitEffectCustomSize = Vec2{ m_fHitEffectSizeX,m_fHitEffectSizeY };
	}
	int Index = m_HitSoundCombo.GetCurSel() - 1;

	if (Index < 0)
		Data.InitHitSound();
	else
		Data.SetHitSound(m_vecHitSoundIndex[Index]);
	
	Index = m_HitReactionCombo.GetCurSel() - 1 ;


	if (Index < 0)
		Data.InitReAction();
	else
		Data.SetHitReAction(Index);
	

	UpdateData(FALSE);

	if (0 == _Index)
		AddHitData(Data);
	else
		ChangeHitData(Data, _Index - 1);
	
	UpdateHitDataList();

	return TRUE;
}
BOOL CSkillDataDlg::HitDataDelEvent(const int& _Index)
{
	if (_Index == 0)
		return FALSE;


	UINT HitIndex = _Index - 1;

	if (HitIndex >= m_CurLevelData.HitDataCount)
		return FALSE;

	m_CurLevelData.DelHitData(HitIndex);

	UpdateHitDataList();

	m_HitDataCombo.SetCurSel(0);
	
	return TRUE;
}
BOOL CSkillDataDlg::CreateSkill()
{
	//CString Caster;
	//m_SkillCasterCombo.GetLBText(m_SkillCasterCombo.GetCurSel(),Caster);
	//
	//if (_T("Monster") != Caster)
	//	return FALSE;

	if (true == m_CurSkillData.CharacterSkill)
		return FALSE;


	BOOL Check = CEditGlobal::EditSkillMgr->CreateSkill(m_fSkillTotalTime,m_CurSkillData, &m_vecCurLevelData);

	if (TRUE == Check)
		InitCurSkillData();
	else
		MessageBox(_T("Monster Skill을 생성하지 못하였습니다."));
	
	UpdateSkillList();
	return Check;
}
BOOL CSkillDataDlg::ModifySkillData(const int& _ListIndex)
{
	CString SkillNoString;
	
	UpdateData(TRUE);
	m_SkillList.GetText(_ListIndex, SkillNoString);
	UpdateData(FALSE);

	int SkillNo = ListStringToSkillNo(SkillNoString);

	if (FALSE == CEditGlobal::EditSkillMgr->ChangeSkillData(SkillNo, m_CurSkillData,&m_vecCurLevelData))
	{
		MessageBox(_T("Skill Data & Level Data 변경에 실패하였습니다."));
		return FALSE;
	}
	else
		MessageBox(_T("Skill Data & Level Data 변경 하였습니다."));
	

	return TRUE;
}
BOOL CSkillDataDlg::DeleteSkill(const int& _ListIndex)
{
	CString SkillNoString;

	UpdateData(TRUE);
	m_SkillList.GetText(_ListIndex, SkillNoString);
	UpdateData(FALSE);

	int SkillNo = ListStringToSkillNo(SkillNoString);

	if (FALSE == CEditGlobal::EditSkillMgr->DeleteSkill(SkillNo))
	{
		MessageBox(_T("Skill을 삭제할 수 없습니다."));
		return FALSE;
	}

	m_SkillList.DeleteString(_ListIndex);
	m_CurSkillData = SKILLDATA();
	
	UpdateData(TRUE);
	m_CurSkillNo = CEditGlobal::EditSkillMgr->SearchNextIndex();
	UpdateData(FALSE);

	m_vecCurLevelData.assign(LEVELDATA::MAXSKILLLEVEL, LEVELDATA());

	InitAllCombo();

	return TRUE;
}
BOOL CSkillDataDlg::LoadSkillFile()
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
		MessageBox(_T("SkillLevelDataFile을 열지 못하였습니다."));
		return FALSE;
	}

	if (FALSE == CEditGlobal::EditSkillMgr->LoadSkillFile(SkillFile.GetFile(), LevelFile.GetFile()))
	{
		MessageBox(_T("Skill을 정상적으로 로드하지 못하였습니다."));
		return FALSE;
	}

	UpdateSkillList();
	MessageBox(_T("Skill을 정상적으로 로드했습니다."));
	return TRUE;
}
BOOL CSkillDataDlg::SaveSkillFile()
{
	CGameFile SkillFile(_T("EditData"), _T("Skill.bin"));
	CGameFile LevelFile(_T("EditData"), _T("LevelData.bin"));

	if (FALSE == SkillFile.Open(_T("wb")))
	{
		MessageBox(_T("SkillDataFile을 열지 못하였습니다."));
		return FALSE;
	}

	if (FALSE == LevelFile.Open(_T("wb")))
	{
		MessageBox(_T("SkillLevelDataFile을 열지 못하였습니다."));
		return FALSE;
	}

	if (FALSE == CEditGlobal::EditSkillMgr->SaveSkillFile(SkillFile.GetFile(), LevelFile.GetFile()))
	{
		MessageBox(_T("Skill을 정상적으로 저장하지 못하였습니다."));
		return FALSE;
	}


	MessageBox(_T("Skill을 정상적으로 저장했습니다."));
	return TRUE;
}

void CSkillDataDlg::SetSkillTestUnit()
{
	int CurIndex = m_SkillCasterCombo.GetCurSel();

	if (nullptr != CEditGlobal::EditSkillMgr && STATE_COMPLETE == CEditGlobal::EditSkillMgr->GetSkillMgrState())
	{
		if (CurIndex < MONSTER)
		{
			CEditGlobal::EditSkillMgr->CharacterOn((CharacterIndex)CurIndex);
			CEditGlobal::EditSkillMgr->MonTargetUnitOff();
		}
		else
		{
			CEditGlobal::EditSkillMgr->MonTargetUnitOn();
			CEditGlobal::EditSkillMgr->CurCharacterOff();
			if (m_fSkillTotalTime <= 0.f)
			{
				m_fSkillTotalTime = 1.f;
				m_strSkillTotalTime.Format(_T("%.2f"), m_fSkillTotalTime);
				UpdateData(FALSE);
			}

		}
	}
}
void CSkillDataDlg::OnOK()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	return;
	//CDialogEx::OnOK();
}


BOOL CSkillDataDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	m_SkillList.AddString(_T("새로 만들기"));
	m_SkillList.SetCurSel(0);

	m_SkillTypeCombo.AddString(_T("1. Action"));
	m_SkillTypeCombo.AddString(_T("2. Ability"));
	m_SkillTypeCombo.AddString(_T("3. Burst"));
	m_SkillTypeCombo.AddString(_T("4. Item"));


	m_BuffTypeCombo.AddString(_T("1. No BuffSkill"));
	m_BuffTypeCombo.AddString(_T("2. Team Buff"));
	m_BuffTypeCombo.AddString(_T("3. Enemy Buff"));


	m_TeamTargetCombo.AddString(_T("1. No Target"));
	m_TeamTargetCombo.AddString(_T("2. Single"));
	m_TeamTargetCombo.AddString(_T("3. Column"));
	m_TeamTargetCombo.AddString(_T("4. All"));
	m_TeamTargetCombo.AddString(_T("5. Random 1"));
	m_TeamTargetCombo.AddString(_T("6. Random 2"));
	m_TeamTargetCombo.AddString(_T("7. Random 3"));
	m_TeamTargetCombo.AddString(_T("8. Random 4"));


	m_EnemyTargetCombo.AddString(_T("1. No Target"));
	m_EnemyTargetCombo.AddString(_T("2. Single"));
	m_EnemyTargetCombo.AddString(_T("3. Column"));
	m_EnemyTargetCombo.AddString(_T("4. All"));
	m_EnemyTargetCombo.AddString(_T("5. Random 1"));
	m_EnemyTargetCombo.AddString(_T("6. Random 2"));
	m_EnemyTargetCombo.AddString(_T("7. Random 3"));
	m_EnemyTargetCombo.AddString(_T("8. Random 4"));
	


	m_CasterInitPosCombo.AddString(_T("1. TeamPos"));	// 제자리
	m_CasterInitPosCombo.AddString(_T("2. FixPos"));
	m_CasterInitPosCombo.AddString(_T("3. Target Pos"));
	m_CasterInitPosCombo.AddString(_T("4. Target Front"));
	m_CasterInitPosCombo.AddString(_T("5. Target Back"));

	m_SkillLevelCombo.AddString(_T(" Level 1"));
	m_SkillLevelCombo.AddString(_T(" Level 2"));
	m_SkillLevelCombo.AddString(_T(" Level 3"));
	m_SkillLevelCombo.AddString(_T(" Level 4"));
	m_SkillLevelCombo.AddString(_T(" Level 5"));
	m_SkillLevelCombo.AddString(_T(" All Level"));


	m_SkillCostCombo.AddString(_T("1. HP"));
	m_SkillCostCombo.AddString(_T("2. MP"));
	m_SkillCostCombo.AddString(_T("3. BURST"));

	m_DemageFontStyle.AddString(_T("1. 팽창형"));	//	확장되면서 사라짐
	m_DemageFontStyle.AddString(_T("1. 계단형"));	//  계단식

	m_HitEffectPosModeCombo.AddString(_T("1. Target"));
	m_HitEffectPosModeCombo.AddString(_T("2. Caster"));
	m_HitEffectPosModeCombo.AddString(_T("3. Target Middle"));


	m_HitDataCombo.AddString(_T(" New"));
	
	m_HitReactionCombo.AddString(_T("No Reaction"));
	m_HitReactionCombo.AddString(_T("1. 타격 필터"));		// Red
	m_HitReactionCombo.AddString(_T("2. 진동"));			// Vib
	m_HitReactionCombo.AddString(_T("3. 진동 타격 필터"));	// Red + Vib
	m_HitReactionCombo.AddString(_T("4. 콜드 필터"));		// Blue

	m_vecHitSoundIndex.clear();


	m_HitSoundCombo.AddString(_T(" No Sound"));
	m_HitSoundCombo.AddString(_T(" Hit 1"));
	m_HitSoundCombo.AddString(_T(" Hit 2"));
	m_HitSoundCombo.AddString(_T(" Hit 3"));
	m_HitSoundCombo.AddString(_T(" Hit 4"));
	m_HitSoundCombo.AddString(_T(" Hit 5"));
	m_HitSoundCombo.AddString(_T(" Hit 6"));
	m_HitSoundCombo.AddString(_T(" Hit 7"));
	m_HitSoundCombo.AddString(_T(" Hit 8"));
	m_HitSoundCombo.AddString(_T(" Hit 9"));
	m_HitSoundCombo.AddString(_T(" Sword 1"));
	m_HitSoundCombo.AddString(_T(" Sword 2"));
	m_HitSoundCombo.AddString(_T(" Sword 3"));
	m_HitSoundCombo.AddString(_T(" Sword 4"));
	m_HitSoundCombo.AddString(_T(" Electric 1"));
	m_HitSoundCombo.AddString(_T(" Electric 2"));
	m_HitSoundCombo.AddString(_T(" Electric 3"));
	m_HitSoundCombo.AddString(_T(" Electric 4"));
	m_HitSoundCombo.AddString(_T(" Explosion 1"));
	m_HitSoundCombo.AddString(_T(" Explosion 2"));
	m_HitSoundCombo.AddString(_T(" Flame 1"));

	m_vecHitSoundIndex.push_back(HIT1);
	m_vecHitSoundIndex.push_back(HIT2);
	m_vecHitSoundIndex.push_back(HIT3);
	m_vecHitSoundIndex.push_back(HIT4);
	m_vecHitSoundIndex.push_back(HIT5);
	m_vecHitSoundIndex.push_back(HIT6);
	m_vecHitSoundIndex.push_back(HIT7);
	m_vecHitSoundIndex.push_back(HIT8);
	m_vecHitSoundIndex.push_back(HIT9);
	m_vecHitSoundIndex.push_back(SWORDHIT1);
	m_vecHitSoundIndex.push_back(SWORDHIT2);
	m_vecHitSoundIndex.push_back(SWORDHIT3);
	m_vecHitSoundIndex.push_back(SWORDHIT4);
	m_vecHitSoundIndex.push_back(ELECTRICHIT1);
	m_vecHitSoundIndex.push_back(ELECTRICHIT2);
	m_vecHitSoundIndex.push_back(ELECTRICHIT3);
	m_vecHitSoundIndex.push_back(ELECTRICHIT4);
	m_vecHitSoundIndex.push_back(EXPLOSIONHIT1);
	m_vecHitSoundIndex.push_back(EXPLOSIONHIT2);
	m_vecHitSoundIndex.push_back(FLAMEHIT1);

	
	m_BuffDetailCombo.AddString(_T("1. HP"));
	m_BuffDetailCombo.AddString(_T("2. MP"));
	m_BuffDetailCombo.AddString(_T("3. 주 스텟"));
	m_BuffDetailCombo.AddString(_T("4. 체력 "));
	m_BuffDetailCombo.AddString(_T("5. 솜씨 "));
	m_BuffDetailCombo.AddString(_T("6. 지력 "));
	m_BuffDetailCombo.AddString(_T("7. 행운 "));
	m_BuffDetailCombo.AddString(_T("8. 데미지"));
	m_BuffDetailCombo.AddString(_T("9. 방어"));
	m_BuffDetailCombo.AddString(_T("10. 크리티컬"));
	m_BuffDetailCombo.AddString(_T("11. 크리티컬 데미지"));
	m_BuffDetailCombo.AddString(_T("12. 회피율"));
	m_BuffDetailCombo.AddString(_T("13. 행동력"));		// Unit TurnCount

	m_mapEditSet.clear();

	m_vecEdit.push_back(&m_SkillTotalTime);
	m_vecEdit.push_back(&m_CasterFixPosX);
	m_vecEdit.push_back(&m_CasterFixPosY);
	m_vecEdit.push_back(&m_PosPivotX);
	m_vecEdit.push_back(&m_PosPivotY);
	m_vecEdit.push_back(&m_SkillCount);
	m_vecEdit.push_back(&m_SkillCost);
	m_vecEdit.push_back(&m_HitStartTime);
	m_vecEdit.push_back(&m_HitOverlapCount);
	m_vecEdit.push_back(&m_DemageMag);
	m_vecEdit.push_back(&m_FixDemage);
	m_vecEdit.push_back(&m_DemageFontMag);
	m_vecEdit.push_back(&m_HitEffectPivotX);
	m_vecEdit.push_back(&m_HitEffectPivotY);
	m_vecEdit.push_back(&m_EffectImageMag);
	m_vecEdit.push_back(&m_HitEffectSizeX);
	m_vecEdit.push_back(&m_HitEffectSizeY);
	m_vecEdit.push_back(&m_BuffValue);
	m_vecEdit.push_back(&m_BuffTurnCount);
	

	m_mapEditSet.insert(unordered_map<UINT, EditSet>::value_type(unordered_map<UINT, EditSet>::value_type(m_SkillTotalTime.GetDlgCtrlID(), EditSet(&m_SkillTotalTime, &m_strSkillTotalTime, &m_fSkillTotalTime))));
	m_mapEditSet.insert(unordered_map<UINT, EditSet>::value_type(unordered_map<UINT, EditSet>::value_type(m_CasterFixPosX.GetDlgCtrlID(), EditSet(&m_CasterFixPosX, &m_strCasterFixPosX, &m_fCasterFixPosX))));
	m_mapEditSet.insert(unordered_map<UINT, EditSet>::value_type(unordered_map<UINT, EditSet>::value_type(m_CasterFixPosY.GetDlgCtrlID(), EditSet(&m_CasterFixPosY, &m_strCasterFixPosY, &m_fCasterFixPosY))));
	m_mapEditSet.insert(unordered_map<UINT, EditSet>::value_type(unordered_map<UINT, EditSet>::value_type(m_PosPivotX.GetDlgCtrlID(), EditSet(&m_PosPivotX, &m_strPosPivotX, &m_fPosPivotX))));
	m_mapEditSet.insert(unordered_map<UINT, EditSet>::value_type(unordered_map<UINT, EditSet>::value_type(m_PosPivotY.GetDlgCtrlID(), EditSet(&m_PosPivotY, &m_strPosPivotY, &m_fPosPivotY))));
	m_mapEditSet.insert(unordered_map<UINT, EditSet>::value_type(unordered_map<UINT, EditSet>::value_type(m_HitStartTime.GetDlgCtrlID(), EditSet(&m_HitStartTime, &m_strHitStartTime, &m_fHitStartTime))));
	m_mapEditSet.insert(unordered_map<UINT, EditSet>::value_type(unordered_map<UINT, EditSet>::value_type(m_DemageFontMag.GetDlgCtrlID(), EditSet(&m_DemageFontMag, &m_strDemageFontMag, &m_fDemageFontMag))));
	m_mapEditSet.insert(unordered_map<UINT, EditSet>::value_type(unordered_map<UINT, EditSet>::value_type(m_HitEffectPivotX.GetDlgCtrlID(), EditSet(&m_HitEffectPivotX, &m_strHitEffectPivotX, &m_fHitEffectPivotX))));
	m_mapEditSet.insert(unordered_map<UINT, EditSet>::value_type(unordered_map<UINT, EditSet>::value_type(m_HitEffectPivotY.GetDlgCtrlID(), EditSet(&m_HitEffectPivotY, &m_strHitEffectPivotY, &m_fHitEffectPivotY))));
	m_mapEditSet.insert(unordered_map<UINT, EditSet>::value_type(unordered_map<UINT, EditSet>::value_type(m_EffectImageMag.GetDlgCtrlID(), EditSet(&m_EffectImageMag, &m_strEffectImageMag, &m_fEffectImageMag))));
	m_mapEditSet.insert(unordered_map<UINT, EditSet>::value_type(unordered_map<UINT, EditSet>::value_type(m_HitEffectSizeX.GetDlgCtrlID(), EditSet(&m_HitEffectSizeX, &m_strHitEffectSizeX, &m_fHitEffectSizeX))));
	m_mapEditSet.insert(unordered_map<UINT, EditSet>::value_type(unordered_map<UINT, EditSet>::value_type(m_HitEffectSizeY.GetDlgCtrlID(), EditSet(&m_HitEffectSizeY, &m_strHitEffectSizeY, &m_fHitEffectSizeY))));

	UpdateData(TRUE);
	m_OptionSlider.SetRange(-10, 10);
	m_OptionSlider.SetRangeMin(-10);
	m_OptionSlider.SetRangeMax(10);
	m_OptionSlider.SetPos(0);
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


BOOL CSkillDataDlg::PreTranslateMessage(MSG* pMsg)
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

		if (TRUE == EditBoxClickCheck(pMsg))
		{
			return TRUE;
		}

	}
	return CDialogEx::PreTranslateMessage(pMsg);
}


void CSkillDataDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	m_mapEditSet.clear();
	m_vecEdit.clear();
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}


void CSkillDataDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if (TRUE == bShow)
	{
		if (FALSE == m_bInit)
		{
			InitSkillDataDlg();
			////UpdateData(TRUE);
			m_CurSkillNo = CEditGlobal::EditSkillMgr->SearchNextIndex();
			UpdateData(FALSE);
			m_bInit = TRUE;
		}
		UpdateSkillList();
	}
	
	
}


BOOL CSkillDataDlg::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	UpdateData(TRUE);
	ChangeEditEvent(nID, nCode, pExtra, pHandlerInfo);
	UpdateData(FALSE);
	return CDialogEx::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}


void CSkillDataDlg::OnLbnSelchangeList2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int Index = m_SkillList.GetCurSel();

	if (-1 == Index)
		return;

	if (Index == 0)
		InitCurSkillData();
	else
		ClickSkillListEvent(Index);
}

void CSkillDataDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (pScrollBar)
	{
		if (pScrollBar == (CScrollBar*)&m_OptionSlider)
		{
			// 슬라이더의 위치를 검사한다.
			int nPos = m_OptionSlider.GetPos();
			OptionSliderEvent(nPos);
		}
	}
	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CSkillDataDlg::OnEnKillfocusSdtotaltimeedit()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CheckTimeString(m_SkillTotalTime, m_strSkillTotalTime, m_fSkillTotalTime);
}


void CSkillDataDlg::OnEnKillfocusCharfixposxedit()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CheckPosString(m_CasterFixPosX, m_strCasterFixPosX, m_fCasterFixPosX);
	m_CurSkillData.InitFixPos.x = m_fCasterFixPosX;
}


void CSkillDataDlg::OnEnKillfocusCharfixposyedit()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CheckPosString(m_CasterFixPosY, m_strCasterFixPosY, m_fCasterFixPosY);
	m_CurSkillData.InitFixPos.y = m_fCasterFixPosY;
}


void CSkillDataDlg::OnEnKillfocusCharpospivotxedit()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CheckPosString(m_PosPivotX, m_strPosPivotX, m_fPosPivotX);
	m_CurSkillData.InitPosPivot.x = m_fPosPivotX;
}


void CSkillDataDlg::OnEnKillfocusCharpospivotyedit()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CheckPosString(m_PosPivotY, m_strPosPivotY, m_fPosPivotY);
	m_CurSkillData.InitPosPivot.y = m_fPosPivotY;
}


void CSkillDataDlg::OnEnKillfocusSkillcountedit()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	m_CurLevelData.SkillCount = m_uiSkillCount;
	UpdateData(FALSE);
}


void CSkillDataDlg::OnEnKillfocusSkillcostedit()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int a = 0;
}


void CSkillDataDlg::OnEnKillfocusHitstarttimeedit()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CheckTimeString(m_HitStartTime, m_strHitStartTime, m_fHitStartTime);
}


void CSkillDataDlg::OnEnKillfocusHitcountedit()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CSkillDataDlg::OnEnKillfocusDemagemagedit()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

}


void CSkillDataDlg::OnEnKillfocusFixdemageedit()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	
}


void CSkillDataDlg::OnEnKillfocusDemagefontmagedit()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CheckTimeString(m_DemageFontMag, m_strDemageFontMag, m_fDemageFontMag);
}


void CSkillDataDlg::OnEnKillfocusHiteffectpospivotxedit()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CheckPosString(m_HitEffectPivotX, m_strHitEffectPivotX, m_fHitEffectPivotX);
}


void CSkillDataDlg::OnEnKillfocusHiteffectpospivotyedit()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CheckPosString(m_HitEffectPivotY, m_strHitEffectPivotY, m_fHitEffectPivotY);
}


void CSkillDataDlg::OnEnKillfocusHiteffeimagemagedit()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//CheckTimeString(m_EffectImageMag, m_strEffectImageMag, m_fEffectImageMag);
	CheckFloatString(m_EffectImageMag, m_strEffectImageMag, m_fEffectImageMag);
}


void CSkillDataDlg::OnEnKillfocusHiteffetsizexedit()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CheckTimeString(m_HitEffectSizeX, m_strHitEffectSizeX, m_fHitEffectSizeX);
}


void CSkillDataDlg::OnEnKillfocusHiteffetsizeyedit()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CheckTimeString(m_HitEffectSizeY, m_strHitEffectSizeY, m_fHitEffectSizeY);
}


void CSkillDataDlg::OnEnKillfocusBuffvalueedit()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	
}


void CSkillDataDlg::OnEnKillfocusBuffturncountedit()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

}


void CSkillDataDlg::OnBnClickedSkilldatacreatebutton()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int ListIndex = m_SkillList.GetCurSel();

	if (-1 == ListIndex)
		return;

	if (ListIndex == 0)
		CreateSkill();
	else
		ModifySkillData(ListIndex);
}


void CSkillDataDlg::OnBnClickedSkilldatadeletebutton()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	
	int ListIndex = m_SkillList.GetCurSel();
	if (-1 == ListIndex)
		return;

	if (ListIndex != 0)
		DeleteSkill(ListIndex);
}


void CSkillDataDlg::OnBnClickedSdskillloadebutton()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	LoadSkillFile();
}


void CSkillDataDlg::OnBnClickedSdskillsaveebutton()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	SaveSkillFile();
}


void CSkillDataDlg::OnBnClickedLevelregbutton()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	LevelDataRegEvent(m_SkillLevelCombo.GetCurSel());
}


void CSkillDataDlg::OnBnClickedLeveldelbutton()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	LevelDataDelEvent(m_SkillLevelCombo.GetCurSel());
}


void CSkillDataDlg::OnBnClickedCostregbutton()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	SkillCostRegEvent(m_SkillCostCombo.GetCurSel());
}


void CSkillDataDlg::OnBnClickedCostdelbutton()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	SkillCostDelEvent(m_SkillCostCombo.GetCurSel());
}


void CSkillDataDlg::OnBnClickedHitregbutton()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	HitDataRegEvent(m_HitDataCombo.GetCurSel());
}


void CSkillDataDlg::OnBnClickedHitdelbutton()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	HitDataDelEvent(m_HitDataCombo.GetCurSel());
}


void CSkillDataDlg::OnCbnSelchangeSkilldataCharcom()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	ClickCasterComboEvent();
}

void CSkillDataDlg::OnCbnSelchangeSkilltypecom()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	SkillTypeComboClickEvent(m_SkillTypeCombo.GetCurSel());
}


void CSkillDataDlg::OnCbnSelchangeBufftypecom()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int Index = m_BuffTypeCombo.GetCurSel();

	if (Index >= m_BuffTypeCombo.GetCount())
		return;

	m_CurSkillData.SkillBuffType = Index;
	
	EnableBuffControl(TRUE);

	if (m_CurSkillData.SkillBuffType == NONEBUFF)
		EnableBuffControl(FALSE);
}


void CSkillDataDlg::OnCbnSelchangeTeamtargetcom()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	TeamTargetClickEvent(m_TeamTargetCombo.GetCurSel());
}

void CSkillDataDlg::OnCbnSelchangeEnemytargetcom()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	EnemyTargetClickEvent(m_EnemyTargetCombo.GetCurSel());
}

void CSkillDataDlg::OnCbnSelchangeInitcharposcom()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CasterInitPosModeClickEvent(m_CasterInitPosCombo.GetCurSel());
}

void CSkillDataDlg::OnCbnSelchangeSkilllevelcom()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	SkillLevelClickEvent(m_SkillLevelCombo.GetCurSel());
}

void CSkillDataDlg::OnCbnSelchangeSkilllcostcom()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	SkillCostClickEvent(m_SkillCostCombo.GetCurSel());
}
void CSkillDataDlg::OnCbnSelchangeHitdatacom()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int Index = m_HitDataCombo.GetCurSel() - 1;

	if (Index < 0)
		InitHitDataCtrl();
	else
		HitDataListClickEvent(Index);
}
void CSkillDataDlg::OnCbnSelchangeSdhiteffectcom()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	HitEffectListClickEvent(m_HitEffectCombo.GetCurSel());
}


void CSkillDataDlg::OnCbnSelchangeHitsoundcom()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int Index = m_HitSoundCombo.GetCurSel() - 1 ;
	if (Index >= 0)
		PlaySoundEvent(Index);

}

void CSkillDataDlg::OnCbnSelchangeDemagestylecom()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	DemageStyleClickEvent(m_DemageFontStyle.GetCurSel());

}





