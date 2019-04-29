// AnimationList.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "FREditor.h"
#include "AnimationList.h"
#include "afxdialogex.h"
#include"AnimationDlg.h"


// CAnimationList 대화 상자입니다.

IMPLEMENT_DYNAMIC(CAnimationList, CDialogEx)

CAnimationList::CAnimationList(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_ANIMATIONLIST, pParent)
	, m_iPrevIndex(-1)
	, m_iLastSelectIndex(-1)
{

}

CAnimationList::~CAnimationList()
{
}

void CAnimationList::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_AniListCtrl);
}


BEGIN_MESSAGE_MAP(CAnimationList, CDialogEx)
	ON_WM_SETCURSOR()
	ON_WM_SHOWWINDOW()
	ON_LBN_SELCHANGE(IDC_LIST1, &CAnimationList::OnLbnSelchangeList1)
END_MESSAGE_MAP()


// CAnimationList 메시지 처리기입니다.


void CAnimationList::OnOK()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	return;
}


BOOL CAnimationList::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	SetCursor(LoadCursor(NULL, IDC_ARROW));
	return TRUE;
}

void CAnimationList::ShowListEvent()
{
	CEditGlobal::InitAniSpriteModel();
	m_AniListCtrl.ResetContent();
	ANISPRITETYPE Index = CEditGlobal::AnimationDlg->GetCurTypeIndex();
	
	UpdateAnimationList(Index);
	
}

void CAnimationList::OnShowWindow(BOOL bShow, UINT nStatus)
{
	if (TRUE == bShow) 
	{
		ShowListEvent();
	}

	CDialogEx::OnShowWindow(bShow, nStatus);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}


BOOL CAnimationList::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	CEditGlobal::AnimationList = this;
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CAnimationList::OnLbnSelchangeList1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int Index = m_AniListCtrl.GetCurSel();

	if (m_iPrevIndex == Index)
	{
		return;
	}
	CString AnimationKey;
	m_AniListCtrl.GetText(Index, AnimationKey);
	CEditGlobal::AnimationDlg->AnimationSelectEvent(AnimationKey);

	m_iPrevIndex = Index;
	m_LastSelectString = AnimationKey;
}
void CAnimationList::UpdateAnimationList(ANISPRITETYPE _Type)
{
	m_iPrevIndex = -1;
	m_AniListCtrl.ResetContent();

	CEditGlobal::AnimationListUpdate(&m_AniListCtrl,CEditGlobal::AAnimationCom, _Type);


	m_iLastSelectIndex = m_AniListCtrl.FindString(0, m_LastSelectString);
	
	if (-1 == m_iLastSelectIndex)
	{
		return;
	}

	m_AniListCtrl.SetCurSel(m_iLastSelectIndex);

	CEditGlobal::AAnimationCom->ChangeAni(m_LastSelectString.GetString());
	CEditGlobal::AAnimationModel->On();
	CEditGlobal::AnimationDlg->InitAnimationData(m_LastSelectString);
}

BOOL CAnimationList::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (pMsg->message == WM_KEYDOWN) {
		if (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE) {
			// VK_RETURN  : Enter Key
			// VK_ESCAPE  : ESC key

			return TRUE;
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}
CString CAnimationList::GetCurSelectString()
{
	int  SelectIndex = m_AniListCtrl.GetCurSel();

	if (-1 == SelectIndex) 
	{
		return _T("");
	}
	CString SelectString;
	m_AniListCtrl.GetText(SelectIndex, SelectString);
	return SelectString;
}
BOOL CAnimationList::EraseString(const CString& _EraseString /*= _T("")*/)
{
	//디폴트작동은 현재 선택된 문자열을 제거
	if (_T("") == _EraseString)
	{
		int CurSel = m_AniListCtrl.GetCurSel();
		m_AniListCtrl.DeleteString(CurSel);
		return TRUE;
	}

	int EraseIndex = m_AniListCtrl.FindString(0, _EraseString);

	if (-1 != EraseIndex)
	{
		m_AniListCtrl.DeleteString(EraseIndex);
		return TRUE;
	}

	return FALSE;
}