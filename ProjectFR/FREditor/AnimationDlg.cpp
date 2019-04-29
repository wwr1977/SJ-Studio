// AnimationDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "FREditor.h"
#include "AnimationDlg.h"
#include "afxdialogex.h"
#include <GameMultiSprite.h>
#include "AniSpriteList.h"
#include "AnimationList.h"
#include "MultiSpriteDlg.h"
#include "EffectDlg.h"


// CAnimationDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CAnimationDlg, CDialogEx)

CAnimationDlg::CAnimationDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_ANIMATION, pParent)
	, m_CurType(_T(""))
	, m_RegAniName(_T(""))
	, m_bBaseMode(TRUE)
	, m_bCycleMode(FALSE)
	, m_bLoopMode(FALSE)
	, m_iFullStart(0)
	, m_iFullEnd(0)
	, m_iRepeatStart(0)
	, m_iRepeatEnd(0)
	, m_iAniCount(0)
	, m_FullInterval(_T(""))
	, m_RepeatInterval(_T(""))
	, m_bAnimationMode(TRUE)
	, m_bAnimationDlgOn(FALSE)
	, m_AniFrameCount(_T(""))
	, m_LastSelectAniSprite(_T(""))
	, m_LastSelectAnimation(_T(""))
{
}

CAnimationDlg::~CAnimationDlg()
{
}

void CAnimationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_ListTabCtrl);
	DDX_Control(pDX, IDC_TYPECOMBO, m_TypeComCtrl);
	DDX_CBString(pDX, IDC_TYPECOMBO, m_CurType);
	DDX_Control(pDX, IDC_ANIMATIONEDIT, m_AniNameCtrl);
	DDX_Text(pDX, IDC_ANIMATIONEDIT, m_RegAniName);
	DDX_Check(pDX, IDC_BASECHECK, m_bBaseMode);
	DDX_Check(pDX, IDC_CYCLECHECK, m_bCycleMode);
	DDX_Check(pDX, IDC_LOOPCHECK, m_bLoopMode);
	DDX_Control(pDX, IDC_FULLSTART, m_FullStartCtrl);
	DDX_Text(pDX, IDC_FULLSTART, m_iFullStart);
	DDX_Control(pDX, IDC_FULLEND, m_FullEndCtrl);
	DDX_Control(pDX, IDC_REPEATSTART, m_RepeatStartCtrl);
	DDX_Text(pDX, IDC_FULLEND, m_iFullEnd);
	DDX_Text(pDX, IDC_REPEATSTART, m_iRepeatStart);
	DDX_Control(pDX, IDC_REPEATEND, m_RepeatEndCtrl);
	DDX_Text(pDX, IDC_REPEATEND, m_iRepeatEnd);
	DDX_Control(pDX, IDC_ANIMATIONCOUNT, m_AniCountCtrl);
	DDX_Text(pDX, IDC_ANIMATIONCOUNT, m_iAniCount);
	DDX_Control(pDX, IDC_FBASEINTERVAL, m_FullIntervalCtrl);
	DDX_Text(pDX, IDC_FBASEINTERVAL, m_FullInterval);
	DDX_Control(pDX, IDC_RBASEINTERVAL, m_RepeatIntervalCtrl);
	DDX_Text(pDX, IDC_RBASEINTERVAL, m_RepeatInterval);
	DDX_Control(pDX, IDC_ANISPRITENAME, m_AniSpriteCtrl);
	DDX_Control(pDX, IDC_ANIFRAMELIST, m_AniFrameCtrl);
}


BEGIN_MESSAGE_MAP(CAnimationDlg, CDialogEx)
	ON_WM_SETCURSOR()
	ON_WM_DESTROY()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CAnimationDlg::OnTcnSelchangeTab1)
	ON_CBN_SELCHANGE(IDC_TYPECOMBO, &CAnimationDlg::OnCbnSelchangeTypecombo)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_BASECHECK, &CAnimationDlg::OnBnClickedBasecheck)
	ON_BN_CLICKED(IDC_CYCLECHECK, &CAnimationDlg::OnBnClickedCyclecheck)
	ON_BN_CLICKED(IDC_LOOPCHECK, &CAnimationDlg::OnBnClickedLoopcheck)
END_MESSAGE_MAP()


// CAnimationDlg 메시지 처리기입니다.


BOOL CAnimationDlg::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	SetCursor(LoadCursor(NULL, IDC_ARROW));
	return TRUE;
	//return CDialogEx::OnSetCursor(pWnd, nHitTest, message);
}


BOOL CAnimationDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	CEditGlobal::AnimationDlg = this;

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	m_TypeComCtrl.AddString(_T("Character"));
	m_TypeComCtrl.AddString(_T("Monster"));
	m_TypeComCtrl.AddString(_T("Effect"));
	m_TypeComCtrl.AddString(_T("Etc"));
	m_TypeComCtrl.SetCurSel(0);

	CreateTab<CAniSpriteList>(IDD_ANISPRITELIST, _T("Multi Sprite"));
	CreateTab<CAnimationList>(IDD_ANIMATIONLIST, _T("Animation"));

	ShowDlg(ANISPRITELIST);

	
	m_RepeatStartCtrl.SetReadOnly(TRUE);
	m_RepeatEndCtrl.SetReadOnly(TRUE);
	m_AniCountCtrl.SetReadOnly(TRUE);
	m_RepeatIntervalCtrl.SetReadOnly(TRUE);


	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CAnimationDlg::OnDestroy()
{
	for (size_t i = 0; i < m_vecListDlg.size(); i++)
	{
		m_vecListDlg[i]->SendMessage(WM_DESTROY);
		SAFE_DELETE(m_vecListDlg[i]);
	}
	m_vecListDlg.clear();

	CDialogEx::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}


void CAnimationDlg::OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	TABINDEX Index = (TABINDEX)m_ListTabCtrl.GetCurSel();

	ShowDlg(Index);
	

	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;
}


BOOL CAnimationDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	{
		if (GetFocus()->m_hWnd == GetDlgItem(IDC_REGISTERBUTTON)->m_hWnd)
		{
			AniRegisterEvent();

		}
		if (GetFocus()->m_hWnd == GetDlgItem(IDC_ANIDELETEBUTTON)->m_hWnd)
		{
			AnimationDeleteEvent();
		}
		if (GetFocus()->m_hWnd == GetDlgItem(IDC_ANIMATIONLOADNUTTON)->m_hWnd)
		{
			AnimationLoadEvent();
		}
		if (GetFocus()->m_hWnd == GetDlgItem(IDC_REGISTERBUTTON)->m_hWnd)
		{
			AniRegisterEvent();

		}
		if (GetFocus()->m_hWnd == GetDlgItem(IDC_ALLSAVEBUTTON)->m_hWnd)
		{
			//애니메이션 세이브 버튼 이벤트
			AnimationSaveEvent();
		}
	}

	if (pMsg->message == WM_KEYDOWN) {
		if (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE) {
			// VK_RETURN  : Enter Key
			// VK_ESCAPE  : ESC key

			return TRUE;
		}
	}
	if (pMsg->message == WM_LBUTTONDOWN)
	{
		if (pMsg->hwnd == GetDlgItem(IDC_REGISTERBUTTON)->m_hWnd)
		{
			AniRegisterEvent();
			return TRUE;
		}
		if (pMsg->hwnd == GetDlgItem(IDC_ANIDELETEBUTTON)->m_hWnd)
		{
			AnimationDeleteEvent();
			return TRUE;
		}
		if (pMsg->hwnd == GetDlgItem(IDC_ANIMATIONLOADNUTTON)->m_hWnd)
		{
			AnimationLoadEvent();
			return TRUE;
		}
		if (pMsg->hwnd == GetDlgItem(IDC_ALLSAVEBUTTON)->m_hWnd)
		{
			//애니메이션 세이브 버튼 이벤트
			AnimationSaveEvent();
			return TRUE;
		}
		int nID = IDC_ANIMATIONEDIT;
		while (nID <= IDC_RBASEINTERVAL)
		{
			if (pMsg->hwnd == GetDlgItem(nID)->m_hWnd &&
				GetFocus()->m_hWnd != GetDlgItem(nID)->m_hWnd)
			{
				GetDlgItem(nID)->SetFocus();
				switch (nID)
				{
				case IDC_ANIMATIONEDIT: m_AniNameCtrl.SetSel(0, -1, true); return TRUE;
				case IDC_FULLSTART:		m_FullStartCtrl.SetSel(0, -1, true); return TRUE;
				case IDC_FULLEND:		m_FullEndCtrl.SetSel(0, -1, true); return TRUE;
				case IDC_REPEATSTART:	m_RepeatStartCtrl.SetSel(0, -1, true); return TRUE;
				case IDC_REPEATEND:		m_RepeatEndCtrl.SetSel(0, -1, true); return TRUE;
				case IDC_ANIMATIONCOUNT:m_AniCountCtrl.SetSel(0, -1, true); return TRUE;
				case IDC_FBASEINTERVAL:	m_FullIntervalCtrl.SetSel(0, -1, true); return TRUE;
				case IDC_RBASEINTERVAL:	m_RepeatIntervalCtrl.SetSel(0, -1, true); return TRUE;
				default:
					break;
				}
			}
			else
			{
				++nID;
			}
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}
void CAnimationDlg::AniSpriteSelectEvent(const CString& _UpdateString)
{
	
	UpdateData(TRUE);
	BOOL isUpdate =InitSpriteData(_UpdateString);
	UpdateData(FALSE);
	
	if (FALSE == isUpdate) 
	{
		return;
	}

	m_AniSpriteCtrl.ResetContent();
	m_AniSpriteCtrl.AddString(_UpdateString);
	
	ChangeSpriteModel(_UpdateString);
	m_LastSelectAniSprite = _UpdateString;
}
void CAnimationDlg::AnimationSelectEvent(const CString& _UpdateString)
{
	BOOL CheckChange = ChangeAnimationModel(_UpdateString);

	if (FALSE == CheckChange) 
	{
		return;
	}
	
	UpdateData(TRUE);
	InitAnimationData(_UpdateString);
	UpdateData(FALSE);
}

void CAnimationDlg::OnCbnSelchangeTypecombo()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	int TypeIndex = m_TypeComCtrl.GetCurSel();
	m_TypeComCtrl.SetCurSel(TypeIndex);

	if (ANI_ETC <= (ANISPRITETYPE)TypeIndex)
	{
		CEditGlobal::AniSpriteList->UpdateAniSpriteList(ANI_ETC);
	}
	else 
	{
		ChangeSpriteType();
		CEditGlobal::AniSpriteList->ChangeSprinteType();
		CEditGlobal::AniSpriteList->UpdateAniSpriteList((ANISPRITETYPE)TypeIndex);
	}

}


void CAnimationDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if (FALSE == bShow) 
	{
		m_bAnimationDlgOn = FALSE;
		CEditGlobal::InitAnimationModel();
	}
	else 
	{
		m_bAnimationDlgOn = TRUE;
		UpdateCurAniList();
		if (TRUE == CEditGlobal::AAnimationCom->IsSettingAni())
		{
			CEditGlobal::AAnimationModel->On();
		}
	}
	CDialogEx::OnShowWindow(bShow, nStatus);

}
void CAnimationDlg::ChangeSpriteModel(const CString& _ChangeKey)
{
	CEditGlobal::InitAniSpriteModel();

	if (FALSE == m_bAnimationMode)
	{
		if (TRUE == CEditGlobal::EditResourceMgr->ExistGameSprite(_ChangeKey.GetString()))
		{
			CEditGlobal::MBaseAniModel->Off();
			CEditGlobal::MAniSpriteCom->SetSprite(_ChangeKey.GetString());
			CEditGlobal::MAniSpriteCom->SyncSpriteSize();
			CEditGlobal::MAniSpriteModel->On();
		}
	}
	else
	{
		if (TRUE == CEditGlobal::MBaseAniCom->ExistAnimation(_ChangeKey.GetString()))
		{
			CEditGlobal::MAniSpriteModel->Off();
			CEditGlobal::MBaseAniCom->ChangeAni(_ChangeKey.GetString());
			CEditGlobal::MBaseAniModel->On();
		}
	}
	
}
BOOL  CAnimationDlg::ChangeAnimationModel(const CString& _ChangeKey)
{
	if (TRUE == CEditGlobal::AAnimationCom->ExistAnimation(_ChangeKey.GetString())) 
	{
		CEditGlobal::AAnimationModel->Off();
		CEditGlobal::AAnimationCom->ChangeAni(_ChangeKey.GetString());
		CEditGlobal::AAnimationModel->On();
		m_LastSelectAnimation = _ChangeKey;
		return TRUE;
	}
	return FALSE;
}
BOOL CAnimationDlg::IsDlgOn()
{
	return m_bAnimationDlgOn;
}
void CAnimationDlg::ChangeAniMode()
{
	if (ANISPRITELIST == m_CurAniListIndex)
	{
		m_bAnimationMode = !m_bAnimationMode;
		CString ChangeKey;
		m_AniSpriteCtrl.GetText(0, ChangeKey);
		ChangeSpriteModel(ChangeKey);
	}
	

	return;
}


void CAnimationDlg::OnBnClickedBasecheck()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	
	if (ANISPRITELIST == m_CurAniListIndex)
	{
		SPTR<CGameMultiSprite> Find = CEditGlobal::EditResourceMgr->FindMultiSprite(m_LastSelectAniSprite.GetString());
		if (nullptr != Find)
		{
			m_iRepeatStart = Find->GetSpriteLastIndex();
			m_iRepeatEnd = Find->GetSpriteLastIndex();
			m_iAniCount = 1;
		}
	}
	else 
	{
		ANIDATA* AniData = CEditGlobal::AAnimationCom->GetCurAniData();

		
		m_iRepeatStart = AniData->FullRange.End;
		m_iRepeatEnd = AniData->FullRange.End;
		m_iAniCount = 1;
		
		
	}
		m_bBaseMode = TRUE;
		m_bCycleMode = FALSE;
		m_bLoopMode = FALSE;
		CheckDlgButton(IDC_BASECHECK, TRUE);
		CheckDlgButton(IDC_CYCLECHECK, FALSE);
		CheckDlgButton(IDC_LOOPCHECK, FALSE);
		UpdateData(FALSE);
	
	
	m_RepeatStartCtrl.SetReadOnly(TRUE);
	m_RepeatEndCtrl.SetReadOnly(TRUE);
	m_AniCountCtrl.SetReadOnly(TRUE);
	m_RepeatIntervalCtrl.SetReadOnly(TRUE);
	
}


void CAnimationDlg::OnBnClickedCyclecheck()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//SPTR<CGameMultiSprite> Find = CEditGlobal::EditResourceMgr->FindMultiSprite(m_LastSelectAniSprite.GetString());

	UpdateData(TRUE);
	if (ANISPRITELIST == m_CurAniListIndex)
	{
		m_iRepeatStart = m_iFullStart;
		m_iRepeatEnd = m_iFullEnd;
		m_iAniCount = 1;
	}
	else 
	{
		ANIDATA* AniData = CEditGlobal::AAnimationCom->GetCurAniData();

		if (nullptr != AniData)
		{
			m_iRepeatStart = AniData->RepeatRange.Start;
			m_iRepeatEnd = AniData->RepeatRange.End;
			m_iAniCount = AniData->AnimationCount;
		}
	}
	m_bBaseMode = FALSE;
	m_bCycleMode = TRUE;
	m_bLoopMode = FALSE;
	CheckDlgButton(IDC_BASECHECK, FALSE);
	CheckDlgButton(IDC_CYCLECHECK, TRUE);
	CheckDlgButton(IDC_LOOPCHECK, FALSE);
	UpdateData(FALSE);


	m_RepeatStartCtrl.SetReadOnly(FALSE);
	m_RepeatEndCtrl.SetReadOnly(FALSE);
	m_AniCountCtrl.SetReadOnly(FALSE);
	m_RepeatIntervalCtrl.SetReadOnly(FALSE);
}


void CAnimationDlg::OnBnClickedLoopcheck()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	if (ANISPRITELIST == m_CurAniListIndex)
	{
		m_iRepeatStart = m_iFullStart;
		m_iRepeatEnd = m_iFullEnd;
		m_iAniCount = 1;
	}
	else 
	{
		ANIDATA* AniData = CEditGlobal::AAnimationCom->GetCurAniData();

		if (nullptr != AniData)
		{
			m_iRepeatStart = AniData->RepeatRange.Start;
			m_iRepeatEnd = AniData->RepeatRange.End;
			m_iAniCount = AniData->AnimationCount;
		}
	}
	m_bBaseMode = FALSE;
	m_bCycleMode = FALSE;
	m_bLoopMode = TRUE;
	CheckDlgButton(IDC_BASECHECK, FALSE);
	CheckDlgButton(IDC_CYCLECHECK, m_bCycleMode);
	CheckDlgButton(IDC_LOOPCHECK, m_bLoopMode);
	UpdateData(FALSE);

	
	m_RepeatStartCtrl.SetReadOnly(TRUE);
	m_RepeatEndCtrl.SetReadOnly(TRUE);
	m_AniCountCtrl.SetReadOnly(TRUE);
	m_RepeatIntervalCtrl.SetReadOnly(TRUE);
}
ANISPRITETYPE CAnimationDlg::GetCurTypeIndex()
{
	int TypeIndex = m_TypeComCtrl.GetCurSel();

	if (ANI_ETC <= (ANISPRITETYPE)TypeIndex)
	{
		return ANI_ETC;
	}
	else
	{
		return 	(ANISPRITETYPE)TypeIndex;
	}
}
void CAnimationDlg::UpdateCurAniList()
{
	int TypeIndex = m_TypeComCtrl.GetCurSel();

	if (ANI_ETC <= (ANISPRITETYPE)TypeIndex)
	{
		TypeIndex = ANI_ETC;
	}
	
	if (ANISPRITELIST == m_CurAniListIndex) 
	{
		CEditGlobal::AniSpriteList->UpdateAniSpriteList((ANISPRITETYPE)TypeIndex);
	}
	else 
	{
		CEditGlobal::AnimationList->UpdateAnimationList((ANISPRITETYPE)TypeIndex);
	}
}
BOOL CAnimationDlg::InitSpriteData(const CString& _UpdateString)
{
	SPTR<CGameMultiSprite> FindAniSprite = CEditGlobal::EditResourceMgr->FindMultiSprite(_UpdateString.GetString());
	
	if (nullptr == FindAniSprite)
	{
		return FALSE;
	}
	m_RegAniName = _UpdateString;
	m_AniFrameCtrl.ResetContent();
	m_AniFrameCount.Format(_T("%d"), (int)FindAniSprite->GetSpriteCount());
	m_AniFrameCtrl.AddString(m_AniFrameCount);

	int Start, End;
	Start = 0;
	End = (int)FindAniSprite->GetSpriteLastIndex();

	m_iFullStart = Start;
	m_iFullEnd = End;
	m_iAniCount = 1;

	if (TRUE == m_bBaseMode) 
	{
		m_iRepeatStart = End;
		m_iRepeatEnd = End;
	}
	if (TRUE == m_bCycleMode || TRUE == m_bLoopMode) 
	{
		m_iRepeatStart = 0;
		m_iRepeatEnd = End;
	}
	
	return TRUE;
}
BOOL CAnimationDlg::InitAnimationData(const CString& _UpdateString)
{
	if (FALSE == CEditGlobal::AAnimationCom->ExistAnimation(_UpdateString.GetString())) 
	{
		return FALSE;
	}
	
	ANIDATA* CurAniData = CEditGlobal::AAnimationCom->GetCurAniData();
	SPTR<CGameMultiSprite> FindSprite = CEditGlobal::EditResourceMgr->FindMultiSprite(CurAniData->AniSpriteName);

	m_RegAniName = CurAniData->AnimationName.c_str();
	m_AniFrameCtrl.ResetContent();
	m_AniFrameCount.Format(_T("%d"), (int)FindSprite->GetSpriteCount());
	m_AniFrameCtrl.AddString(m_AniFrameCount);
	
	m_AniSpriteCtrl.ResetContent();
	m_AniSpriteCtrl.AddString(CurAniData->AniSpriteName.c_str());
	
	m_iFullStart = CurAniData->FullRange.Start;
	m_iFullEnd = CurAniData->FullRange.End;
	m_iRepeatStart = CurAniData->RepeatRange.Start;
	m_iRepeatEnd = CurAniData->RepeatRange.End;
	
	m_iAniCount = CurAniData->AnimationCount;
	
	
	m_FullInterval.Format(_T("%f"), CurAniData->DefaultInterval);
	
	m_RepeatInterval.Format(_T("%f"), CurAniData->DefaultRepeatInterval);

	return TRUE;
	
}
BOOL CAnimationDlg::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (nID == IDC_FULLEND) 
	{
		if (TRUE == m_bBaseMode)
		{
			UpdateData(TRUE);
			m_iRepeatStart = m_iFullEnd;
			m_iRepeatEnd = m_iFullEnd;
			UpdateData(FALSE);
		}
	}
	
	if (nID == IDC_FBASEINTERVAL)
	{
		int Sel = m_FullIntervalCtrl.GetSel();
		UpdateData(TRUE);
		CEditGlobal::ChangeFloatString(m_FullInterval);
		UpdateData(FALSE);
		m_FullIntervalCtrl.SetSel(Sel);
	}
	if (nID == IDC_RBASEINTERVAL)
	{
		int Sel = m_RepeatIntervalCtrl.GetSel();
		UpdateData(TRUE);
		CEditGlobal::ChangeFloatString(m_RepeatInterval);
		UpdateData(FALSE);
		m_RepeatIntervalCtrl.SetSel(Sel);
	}

	return CDialogEx::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}
BOOL CAnimationDlg::SettingAnimationValue()
{
	CString AniSpriteKey;
	GetDlgItemText(IDC_ANIMATIONEDIT, AniSpriteKey);
	
	if (FALSE == CEditGlobal::EditResourceMgr->ExistGameMultiSprite(AniSpriteKey.GetString()))
	{
		return FALSE;
	}

	m_iFullStart =GetDlgItemInt(IDC_FULLSTART);
	if (FALSE == CEditGlobal::AvailableValue(m_iFullStart, _T("FullStartFrame 값이 적절치 않습니다.")))
	{
		GetDlgItem(IDC_FULLSTART)->SetFocus();
		m_FullStartCtrl.SetSel(0, -1);
		return FALSE;
	}

	int EndFrame = CEditGlobal::ChangeIntValue(m_AniFrameCount.GetString()) -1;

	m_iFullEnd = GetDlgItemInt(IDC_FULLEND);
	
	if (FALSE == CEditGlobal::AvailableValue(m_iFullEnd, EndFrame, _T("FullEndFrame 값이 적절치 않습니다.")))
	{
		GetDlgItem(IDC_FULLEND)->SetFocus();
		m_FullEndCtrl.SetSel(0, -1);
		return FALSE;
	}

	m_fFullInterval = CEditGlobal::ChangeFloatValue(m_FullInterval);
	if (m_fFullInterval <= 0.0f)
	{
		m_fFullInterval = 0.01f;
	}

	if (TRUE == m_bBaseMode || TRUE == m_bLoopMode) 
	{
		return TRUE;
	}

	m_iRepeatStart = GetDlgItemInt(IDC_REPEATSTART);
	if (FALSE == CEditGlobal::AvailableValue(m_iRepeatStart, _T("RepeatStartFrame 값이 적절치 않습니다.")))
	{
		GetDlgItem(IDC_REPEATSTART)->SetFocus();
		m_RepeatStartCtrl.SetSel(0, -1);
		return FALSE;
	}
	
	m_iRepeatEnd = GetDlgItemInt(IDC_REPEATEND);
	if (FALSE == CEditGlobal::AvailableValue(m_iRepeatEnd, EndFrame,_T("RepeatEndFrame 값이 적절치 않습니다.")))
	{
		GetDlgItem(IDC_REPEATEND)->SetFocus();
		m_RepeatEndCtrl.SetSel(0, -1);
		return FALSE;
	}

	m_iAniCount = GetDlgItemInt(IDC_ANIMATIONCOUNT);
	if (FALSE == CEditGlobal::AvailableValue(m_iAniCount,_T("Animation Count 값이 적절치 않습니다.")))
	{
		GetDlgItem(IDC_ANIMATIONCOUNT)->SetFocus();
		m_AniCountCtrl.SetSel(0, -1);
		return FALSE;
	}

	m_fRepeatInterval = CEditGlobal::ChangeFloatValue(m_RepeatInterval);
	if (m_fRepeatInterval <= 0.0f) 
	{
		m_fRepeatInterval = 0.05f;
	}

	return TRUE;
}
void CAnimationDlg::AniRegisterEvent()
{
	if (FALSE == SettingAnimationValue()) 
	{
		return;
	}

	if (TRUE == m_bBaseMode) 
	{
		RegisterBaseAni();
		return;
	}
	if (TRUE == m_bCycleMode) 
	{
		RegisterCycleAni();
		return;
	}
	if (TRUE == m_bLoopMode) 
	{
		RegisterLoopAni();
		return;
	}
}
void CAnimationDlg::ChangeSpriteType()
{
	m_AniSpriteCtrl.ResetContent();
	UpdateData(TRUE);
	m_RegAniName = _T("");
	m_iAniCount = 0;
	m_iFullStart = 0;
	m_iFullEnd = 0;
	m_iRepeatStart = 0;
	m_iRepeatEnd = 0;
	m_FullInterval = _T("");
	m_RepeatInterval = _T("");
	UpdateData(FALSE);
}
void CAnimationDlg::RegisterBaseAni() 
{
	CString AniSprite;
	m_AniSpriteCtrl.GetText(0, AniSprite);
	ANIDATA AniData = ANIDATA(AniSprite.GetString(),m_RegAniName.GetString());
	AniData.SettingFullRage({ m_iFullStart,m_iFullEnd }, m_fFullInterval);
	AniData.SettingRepeatRage({ m_iRepeatStart,m_iRepeatEnd }, 30.0f);
	AniData.Loop = false;
	CEditGlobal::AAnimationCom->CreateEditAnimation(AniData);
	CEditGlobal::AAnimationCom->ChangeAni(AniData.AnimationName);
	CEditGlobal::AAnimationCom->SetCurAniStyle(ANI_DEFAULT);
	//CEditGlobal::InitAniSpriteModel();
	//CEditGlobal::AAnimationModel->On();
}
void CAnimationDlg::RegisterCycleAni() 
{
	CString AniSprite;
	m_AniSpriteCtrl.GetText(0, AniSprite);
	ANIDATA AniData = ANIDATA(AniSprite.GetString(), m_RegAniName.GetString());
	AniData.SettingFullRage({ m_iFullStart,m_iFullEnd }, m_fFullInterval);
	AniData.SettingRepeatRage({ m_iRepeatStart,m_iRepeatEnd }, m_fRepeatInterval);
	AniData.Loop = false;
	AniData.AnimationCount = m_iAniCount;
	CEditGlobal::AAnimationCom->CreateEditAnimation(AniData);
	CEditGlobal::AAnimationCom->ChangeAni(AniData.AnimationName);
	CEditGlobal::AAnimationCom->SetCurAniStyle(ANI_COUNTCYCLE_REPEAT);
}
void CAnimationDlg::RegisterLoopAni()
{
	CString AniSprite;
	m_AniSpriteCtrl.GetText(0, AniSprite);
	ANIDATA AniData = ANIDATA(AniSprite.GetString(), m_RegAniName.GetString());
	AniData.SettingFullRage({ m_iFullStart,m_iFullEnd }, m_fFullInterval);
	AniData.Loop = true;
	CEditGlobal::AAnimationCom->CreateEditAnimation(AniData);
	CEditGlobal::AAnimationCom->ChangeAni(AniData.AnimationName);
	CEditGlobal::AAnimationCom->SetCurAniStyle(ANI_LOOP);
}

BOOL CAnimationDlg::AnimationSaveEvent()
{
	if (ANIMATIONLIST != m_CurAniListIndex) 
	{
		MessageBox(_T("Animation List를 활성화 해주세요."));
		return FALSE;
	}

	ANISPRITETYPE GroupIndex = (ANISPRITETYPE)m_TypeComCtrl.GetCurSel();

	// 애니타입이 선택이 안된경우 종료
	if (-1 == GroupIndex) 
	{
		return FALSE;
	}

	if (ANI_ETC <= GroupIndex)
	{
		GroupIndex = ANI_ETC;
	}

	if (FALSE == CEditGlobal::AAnimationCom->SaveAnimationData(GroupIndex)) 
	{
		MessageBox(CEditGlobal::vecSaveDataFileName[GroupIndex] + _T("(Animation Data)에 데이터 저장에 실패하였습니다."), _T("MultiSpriteDlg"), MB_ICONERROR);
	}

	MessageBox(CEditGlobal::vecSaveDataFileName[GroupIndex] + _T("(Animation Data)에 데이터 저장이 성공적으로 이루워 졌습니다."));
	
	if (nullptr != CEditGlobal::EffectDlg) 
	{
		CEditGlobal::EffectDlg->SettingEffectModel();
	}
	return TRUE;
}
BOOL CAnimationDlg::AnimationDeleteEvent()
{
	if (ANIMATIONLIST != m_CurAniListIndex) 
	{
		MessageBox(_T("Animation List를 활성화 해주세요."));
		return FALSE;
	}

	CString CurSelectString = CEditGlobal::AnimationList->GetCurSelectString();

	if (_T("") == CurSelectString) 
	{
		MessageBox(_T("제거할 Animation을 선택해주세요"), _T("AnimationDlg"), MB_ICONERROR);
		return FALSE;
	}

	CEditGlobal::AnimationList->EraseString();

	CEditGlobal::AAnimationCom->EraseAnimation(CurSelectString.GetString());
	CEditGlobal::AAnimationModel->Off();
	
	return TRUE;
}
BOOL CAnimationDlg::AnimationLoadEvent()
{
	int LoadDataIndex = m_TypeComCtrl.GetCurSel();

	if (0 > LoadDataIndex) 
	{
		MessageBox(_T("로드할 애니메이션 그룹을 선택하십시오!"), _T("AnimationDlg"), MB_ICONERROR);
		return FALSE;

	}

	if (ANI_ETC <= LoadDataIndex)
	{
		LoadDataIndex = ANI_ETC;
	}

	CGameFile RFile = CGameFile(_T("EditData"), CGameFile::vecAniDataFileName[LoadDataIndex]);

	if (FALSE == RFile.Open(_T("rb")))
	{
		MessageBox(CEditGlobal::vecSaveDataFileName[LoadDataIndex] + _T("을 열지 못하였습니다."), _T("MultiSpriteDlg"), MB_ICONERROR);
		return FALSE;
	}

	map<tstring, ANIMATIONDATA> TempMap;

	ANIMATIONDATA::ReadAnimationData(RFile.GetFile(), &TempMap);

	if (0 >= TempMap.size()) 
	{
		MessageBox(_T("데이터 파일이 비어있습니다!"), _T("AnimationDlg"), MB_ICONERROR);
		return FALSE;
	}

	map<tstring, ANIMATIONDATA>::iterator Iter = TempMap.begin();

	int RegAniCount = 0;

	for (;Iter != TempMap.end(); ++Iter)
	{
		CAniRenderer::CAnimation* NewAni;
		if (nullptr != (NewAni =  CEditGlobal::AAnimationCom->CreateEditAnimation(Iter->second.GetAniData())))
		{
			if ((int)MAXANISTYLE == Iter->second.AniStyle) 
			{
				int a = 0;
			}
			NewAni->SetAniStyle((ANISTYLE)Iter->second.AniStyle);
			++RegAniCount;
		}
	}

	if (RegAniCount == TempMap.size()) 
	{
		MessageBox(CEditGlobal::vecSaveDataFileName[LoadDataIndex] + _T("의 데이터를 성공적으로 로드 하였습니다."));
		return TRUE;
	}
	
	MessageBox(CEditGlobal::vecSaveDataFileName[LoadDataIndex] + _T("의 데이터 일부분을 Model에 적용하지 못했습니다."));
	return TRUE;
}