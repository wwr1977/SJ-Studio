// StatDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "FREditor.h"
#include "StatDlg.h"
#include "afxdialogex.h"
#include "CharStatDlg.h"
#include "MonStatDlg.h"


// CStatDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CStatDlg, CDialogEx)

CStatDlg::CStatDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_STATDLG, pParent)
{
	CEditGlobal::StatDlg = this;
}

CStatDlg::~CStatDlg()
{
}

void CStatDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATTAB, m_StatTabCtrl);
}


BEGIN_MESSAGE_MAP(CStatDlg, CDialogEx)
	ON_WM_DESTROY()
	ON_NOTIFY(TCN_SELCHANGE, IDC_STATTAB, &CStatDlg::OnTcnSelchangeStattab)
	ON_WM_CREATE()
	ON_WM_GETMINMAXINFO()
	ON_WM_SETCURSOR()
END_MESSAGE_MAP()


// CStatDlg 메시지 처리기입니다.


BOOL CStatDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	CreateTab<CCharStatDlg>(IDD_CHARSTAT, _T("캐릭터"));
	CreateTab<CMonStatDlg>(IDD_MONSTAT, _T("몬스터"));

	ShowDlg(CHARACTER);

	CEditGlobal::StatDlg = this;
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CStatDlg::OnDestroy()
{
	for (size_t i = 0; i < m_vecStatDlg.size(); i++)
	{
		m_vecStatDlg[i]->SendMessage(WM_DESTROY);
		SAFE_DELETE(m_vecStatDlg[i]);
	}
	m_vecStatDlg.clear();

	CDialogEx::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}


void CStatDlg::OnTcnSelchangeStattab(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;

	STATINDEX Index = (STATINDEX)m_StatTabCtrl.GetCurSel();

	if (MAXSTATINDEX <= Index)
	{
		return;
	}

	ShowDlg(Index);

}


int CStatDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
	m_DlgRect = { 0,0,629,818 };

	return 0;
}


void CStatDlg::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	lpMMI->ptMaxTrackSize.x = m_DlgRect.right - m_DlgRect.left;
	lpMMI->ptMaxTrackSize.y = m_DlgRect.bottom - m_DlgRect.top;
	lpMMI->ptMinTrackSize.x = m_DlgRect.right - m_DlgRect.left;
	lpMMI->ptMinTrackSize.y = m_DlgRect.bottom - m_DlgRect.top;

	CDialogEx::OnGetMinMaxInfo(lpMMI);
}


BOOL CStatDlg::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	SetCursor(LoadCursor(NULL, IDC_ARROW));
	return TRUE;
}
