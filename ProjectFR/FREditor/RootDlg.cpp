// RootDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "FREditor.h"
#include "RootDlg.h"
#include "afxdialogex.h"
#include "CharacterDlg.h"
#include "StatDlg.h"
#include "MapDlg.h"
#include "SkillDlg.h"
#include "SkillMgr.h"


// CRootDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CRootDlg, CDialogEx)

CRootDlg::CRootDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_ROOTDLG, pParent)
	, m_bAddStatDlg(false)
{
	CEditGlobal::RootDlg = this;
}

CRootDlg::~CRootDlg()
{
}

void CRootDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_RootTabCtrl);
}


BEGIN_MESSAGE_MAP(CRootDlg, CDialogEx)
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_GETMINMAXINFO()
	ON_WM_CREATE()
	ON_WM_SETCURSOR()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CRootDlg::OnTcnSelchangeTab1)
	ON_WM_SHOWWINDOW()
	ON_WM_MOUSEHWHEEL()
END_MESSAGE_MAP()


// CRootDlg 메시지 처리기입니다.



void CRootDlg::AddStatDlg()
{
	CreateTab<CStatDlg>(IDD_STATDLG, _T("스탯"));
	ShowDlg(CHARACTER);
	m_bAddStatDlg = true;
}
void CRootDlg::ShowSkillDlg()
{
	this->SendMessage(WM_SHOWWINDOW);
	CEditGlobal::SkillDlg->SendMessage(WM_SHOWWINDOW);
	CEditGlobal::MainWindow->ChangeScene(_T("SkillScene"));
	m_RootTabCtrl.SetCurSel(CHARACTER);
}
BOOL CRootDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	CreateTab<CCharacterDlg>(IDD_CHARACTER, _T("캐릭터"));
	CreateTab<CMapDlg>(IDD_MAPDLG, _T("맵"));
	//CreateTab<CStatDlg>(IDD_STATDLG, _T("스탯"));

	ShowDlg(CHARACTER);
	



	SetBackgroundColor(RGB(0, 0, 0));
	Lock();
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CRootDlg::OnDestroy()
{
	for (size_t i = 0; i < m_vecDlg.size(); i++)
	{
		m_vecDlg[i]->SendMessage(WM_DESTROY);
		SAFE_DELETE(m_vecDlg[i]);
	}
	m_vecDlg.clear();

	CEditGlobal::GlobalRelease();

	CDialogEx::OnDestroy();


	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}


BOOL CRootDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CRect rect;
	GetClientRect(rect);
	pDC->FillSolidRect(rect, RGB(0, 0, 0));

	return CDialogEx::OnEraseBkgnd(pDC);
}


void CRootDlg::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	lpMMI->ptMaxTrackSize.x = m_DlgRect.right - m_DlgRect.left;
	lpMMI->ptMaxTrackSize.y = m_DlgRect.bottom - m_DlgRect.top;
	lpMMI->ptMinTrackSize.x = m_DlgRect.right - m_DlgRect.left;
	lpMMI->ptMinTrackSize.y = m_DlgRect.bottom - m_DlgRect.top;

	CDialogEx::OnGetMinMaxInfo(lpMMI);
}




int CRootDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
	m_DlgRect = { lpCreateStruct->x,lpCreateStruct->y,lpCreateStruct->cx,lpCreateStruct->cy };

	return 0;
}




BOOL CRootDlg::PreTranslateMessage(MSG* pMsg)
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


BOOL CRootDlg::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	SetCursor(LoadCursor(NULL, IDC_ARROW));
	return TRUE;
	//return CDialogEx::OnSetCursor(pWnd, nHitTest, message);
}


void CRootDlg::OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;

	TABINDEX Index = (TABINDEX)m_RootTabCtrl.GetCurSel();

	if (Index >= MAXTAB) 
	{
		return;
	}

	ShowDlg(Index);

	switch (Index)
	{
	case CRootDlg::CHARACTER:
	{
		CEditGlobal::MainWindow->ChangeScene(_T("SkillScene"));
	}
		break;
	case CRootDlg::STAT:
	{
		CEditGlobal::MainWindow->ChangeScene(_T("StatScene"));
	}
	break;
	case CRootDlg::MAP:
	{
		CEditGlobal::MainWindow->ChangeScene(_T("MapScene"));
	}
		break;
	default:
		break;
	}
}


void CRootDlg::OnOK()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (false == m_bAddStatDlg)
		AddStatDlg();
	
	return;
	//CDialogEx::OnOK();
}


void CRootDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if (nullptr == CEditGlobal::EditSkillMgr)
		return;

	if (false == m_bAddStatDlg && STATE_COMPLETE ==  CEditGlobal::EditSkillMgr->GetSkillMgrState())
		AddStatDlg();

}


void CRootDlg::OnMouseHWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// 이 기능을 사용하려면 Windows Vista 이상이 있어야 합니다.
	// _WIN32_WINNT 기호는 0x0600보다 크거나 같아야 합니다.
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	int a = 0;
	CDialogEx::OnMouseHWheel(nFlags, zDelta, pt);
}
