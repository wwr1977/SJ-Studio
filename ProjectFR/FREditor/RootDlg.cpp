// RootDlg.cpp : ���� �����Դϴ�.
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


// CRootDlg ��ȭ �����Դϴ�.

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


// CRootDlg �޽��� ó�����Դϴ�.



void CRootDlg::AddStatDlg()
{
	CreateTab<CStatDlg>(IDD_STATDLG, _T("����"));
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

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	CreateTab<CCharacterDlg>(IDD_CHARACTER, _T("ĳ����"));
	CreateTab<CMapDlg>(IDD_MAPDLG, _T("��"));
	//CreateTab<CStatDlg>(IDD_STATDLG, _T("����"));

	ShowDlg(CHARACTER);
	



	SetBackgroundColor(RGB(0, 0, 0));
	Lock();
	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
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


	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
}


BOOL CRootDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	CRect rect;
	GetClientRect(rect);
	pDC->FillSolidRect(rect, RGB(0, 0, 0));

	return CDialogEx::OnEraseBkgnd(pDC);
}


void CRootDlg::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

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

	// TODO:  ���⿡ Ư��ȭ�� �ۼ� �ڵ带 �߰��մϴ�.
	m_DlgRect = { lpCreateStruct->x,lpCreateStruct->y,lpCreateStruct->cx,lpCreateStruct->cy };

	return 0;
}




BOOL CRootDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
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
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	SetCursor(LoadCursor(NULL, IDC_ARROW));
	return TRUE;
	//return CDialogEx::OnSetCursor(pWnd, nHitTest, message);
}


void CRootDlg::OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
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
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	if (false == m_bAddStatDlg)
		AddStatDlg();
	
	return;
	//CDialogEx::OnOK();
}


void CRootDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	if (nullptr == CEditGlobal::EditSkillMgr)
		return;

	if (false == m_bAddStatDlg && STATE_COMPLETE ==  CEditGlobal::EditSkillMgr->GetSkillMgrState())
		AddStatDlg();

}


void CRootDlg::OnMouseHWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// �� ����� ����Ϸ��� Windows Vista �̻��� �־�� �մϴ�.
	// _WIN32_WINNT ��ȣ�� 0x0600���� ũ�ų� ���ƾ� �մϴ�.
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	int a = 0;
	CDialogEx::OnMouseHWheel(nFlags, zDelta, pt);
}
