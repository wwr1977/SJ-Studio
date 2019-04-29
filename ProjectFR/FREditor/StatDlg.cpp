// StatDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "FREditor.h"
#include "StatDlg.h"
#include "afxdialogex.h"
#include "CharStatDlg.h"
#include "MonStatDlg.h"


// CStatDlg ��ȭ �����Դϴ�.

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


// CStatDlg �޽��� ó�����Դϴ�.


BOOL CStatDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	CreateTab<CCharStatDlg>(IDD_CHARSTAT, _T("ĳ����"));
	CreateTab<CMonStatDlg>(IDD_MONSTAT, _T("����"));

	ShowDlg(CHARACTER);

	CEditGlobal::StatDlg = this;
	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
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

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
}


void CStatDlg::OnTcnSelchangeStattab(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
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

	// TODO:  ���⿡ Ư��ȭ�� �ۼ� �ڵ带 �߰��մϴ�.
	m_DlgRect = { 0,0,629,818 };

	return 0;
}


void CStatDlg::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	lpMMI->ptMaxTrackSize.x = m_DlgRect.right - m_DlgRect.left;
	lpMMI->ptMaxTrackSize.y = m_DlgRect.bottom - m_DlgRect.top;
	lpMMI->ptMinTrackSize.x = m_DlgRect.right - m_DlgRect.left;
	lpMMI->ptMinTrackSize.y = m_DlgRect.bottom - m_DlgRect.top;

	CDialogEx::OnGetMinMaxInfo(lpMMI);
}


BOOL CStatDlg::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	SetCursor(LoadCursor(NULL, IDC_ARROW));
	return TRUE;
}
