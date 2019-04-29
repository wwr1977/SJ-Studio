// AniSpriteList.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "FREditor.h"
#include "AniSpriteList.h"
#include "afxdialogex.h"
#include"AnimationDlg.h"
#include<GameMultiSprite.h>

// CAniSpriteList 대화 상자입니다.

IMPLEMENT_DYNAMIC(CAniSpriteList, CDialogEx)

CAniSpriteList::CAniSpriteList(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_ANISPRITELIST, pParent),m_iPrevIndex(-1)
	, m_iLastSelectIndex(-1)
	, m_LastSelectString(_T(""))
{

}

CAniSpriteList::~CAniSpriteList()
{
}

void CAniSpriteList::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_SpriteListCtrl);
}


BEGIN_MESSAGE_MAP(CAniSpriteList, CDialogEx)
	ON_WM_SETCURSOR()
	ON_WM_SHOWWINDOW()
	ON_LBN_SELCHANGE(IDC_LIST1, &CAniSpriteList::OnLbnSelchangeList1)
END_MESSAGE_MAP()


// CAniSpriteList 메시지 처리기입니다.


void CAniSpriteList::OnOK()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	return;
}


BOOL CAniSpriteList::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	SetCursor(LoadCursor(NULL, IDC_ARROW));
	return TRUE;
}


void CAniSpriteList::OnShowWindow(BOOL bShow, UINT nStatus)
{
	
	if (FALSE == CEditGlobal::bEditInit) 
	{
		return;
	}
	
	if (TRUE == bShow) 
	{
		ShowListEvent();
	}


	CDialogEx::OnShowWindow(bShow, nStatus);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}
void CAniSpriteList::ShowListEvent()
{
	CEditGlobal::InitAnimationModel();

	m_SpriteListCtrl.ResetContent();
	ANISPRITETYPE Index = CEditGlobal::AnimationDlg->GetCurTypeIndex();
	UpdateAniSpriteList(Index);

}

BOOL CAniSpriteList::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	CEditGlobal::AniSpriteList = this;
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CAniSpriteList::OnLbnSelchangeList1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int Index = m_SpriteListCtrl.GetCurSel();

	if (m_iPrevIndex == Index)
	{
		return;
	}
	CString AniSpriteKey;
	m_SpriteListCtrl.GetText(Index, AniSpriteKey);
	CEditGlobal::AnimationDlg->AniSpriteSelectEvent(AniSpriteKey);

	m_iPrevIndex = Index;
	m_SpriteListCtrl.GetText(Index,m_LastSelectString);
}
void CAniSpriteList::UpdateAniSpriteList(ANISPRITETYPE _Type)
{
	m_iPrevIndex = -1;
	m_SpriteListCtrl.ResetContent();

	CEditGlobal::AniSpriteListUpdate(&m_SpriteListCtrl, _Type);
	

	m_iLastSelectIndex = m_SpriteListCtrl.FindString(0, m_LastSelectString);
	if (-1 == m_iLastSelectIndex) 
	{
		return;
	}
	
	m_SpriteListCtrl.SetCurSel(m_iLastSelectIndex);
	
	CEditGlobal::MBaseAniCom->ChangeAni(m_LastSelectString.GetString());
	CEditGlobal::MBaseAniModel->On();

}
void CAniSpriteList::ChangeSprinteType()
{
	m_LastSelectString = _T("");
	m_iPrevIndex = -1;
}

BOOL CAniSpriteList::PreTranslateMessage(MSG* pMsg)
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
