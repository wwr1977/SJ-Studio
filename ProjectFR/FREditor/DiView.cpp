// DiView.cpp : 구현 파일입니다.
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


// CDiView 그리기입니다.

void CDiView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: 여기에 그리기 코드를 추가합니다.
}


// CDiView 진단입니다.

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


// CDiView 메시지 처리기입니다.


BOOL CDiView::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	CView::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
	if (FALSE == m_EditDlg.Create(IDD_ROOTDLG, this))
	{
		AfxMessageBox(_T("메뉴 DLG 만들기 오류"), MB_OK | MB_ICONEXCLAMATION);
		return -1;
	}

	RECT Rc = { 0,0 };
	this->GetClientRect(&Rc);
	//Rc.left = -11; Rc.top = -11;
	m_EditDlg.SetWindowPos(this, Rc.left, Rc.top , Rc.right- 4* Rc.left, Rc.bottom- Rc.top, SWP_NOZORDER);

	m_EditDlg.ShowWindow(SW_SHOW);
	
	return TRUE;
}
