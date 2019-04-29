// DiView.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "FREditor.h"
#include "DiView.h"


// CDiView

IMPLEMENT_DYNCREATE(CDiView, CView)

CDiView::CDiView()
{

}

CDiView::~CDiView()
{
}

BEGIN_MESSAGE_MAP(CDiView, CView)
END_MESSAGE_MAP()


// CDiView �׸����Դϴ�.

void CDiView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: ���⿡ �׸��� �ڵ带 �߰��մϴ�.
}


// CDiView �����Դϴ�.

#ifdef _DEBUG
void CDiView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CDiView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// CDiView �޽��� ó�����Դϴ�.


BOOL CDiView::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	CView::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
	if (FALSE == m_EditDlg.Create(IDD_ROOTDLG, this))
	{
		AfxMessageBox(_T("�޴� DLG ����� ����"), MB_OK | MB_ICONEXCLAMATION);
		return -1;
	}

	RECT Rc = { 0,0 };
	this->GetClientRect(&Rc);
	//Rc.left = -11; Rc.top = -11;
	m_EditDlg.SetWindowPos(this, Rc.left, Rc.top , Rc.right- 4* Rc.left, Rc.bottom- Rc.top, SWP_NOZORDER);

	m_EditDlg.ShowWindow(SW_SHOW);
	
	return TRUE;
}
