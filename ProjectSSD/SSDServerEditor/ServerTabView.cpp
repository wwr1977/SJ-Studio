// ServerTabView.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "SSDServerEditor.h"
#include "ServerTabView.h"


// CServerTabView

IMPLEMENT_DYNCREATE(CServerTabView, CView)

CServerTabView::CServerTabView()
{

}

CServerTabView::~CServerTabView()
{
}

BEGIN_MESSAGE_MAP(CServerTabView, CView)
	ON_WM_CREATE()
END_MESSAGE_MAP()


// CServerTabView �׸����Դϴ�.

void CServerTabView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: ���⿡ �׸��� �ڵ带 �߰��մϴ�.
}


// CServerTabView �����Դϴ�.

#ifdef _DEBUG
void CServerTabView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CServerTabView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// CServerTabView �޽��� ó�����Դϴ�.


int CServerTabView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  ���⿡ Ư��ȭ�� �ۼ� �ڵ带 �߰��մϴ�.

	if (FALSE == m_RootTabDlg.Create(IDD_SERVERTABDLG, this))
	{
		AfxMessageBox(_T("��Ʈ �� DLG ����� ����"), MB_OK | MB_ICONEXCLAMATION);
		return -1;
	}

	RECT Rc = { 0,0 };
	this->GetClientRect(&Rc);
	m_RootTabDlg.SetWindowPos(this, Rc.left - 2, Rc.top, Rc.right - 4 * Rc.left, Rc.bottom + 10, SWP_NOZORDER);
	m_RootTabDlg.ShowWindow(SW_SHOW);

	return 0;
}
