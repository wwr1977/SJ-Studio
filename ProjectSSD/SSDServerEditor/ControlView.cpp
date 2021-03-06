// ControlView.cpp: 구현 파일
//

#include "stdafx.h"
#include "SSDServerEditor.h"
#include "ControlView.h"


// ControlView

IMPLEMENT_DYNCREATE(ControlView, CView)

ControlView::ControlView()
{

}

ControlView::~ControlView()
{
}

BEGIN_MESSAGE_MAP(ControlView, CView)
	ON_WM_CREATE()
END_MESSAGE_MAP()


// ControlView 그리기

void ControlView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: 여기에 그리기 코드를 추가합니다.
}


// ControlView 진단

#ifdef _DEBUG
void ControlView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void ControlView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// ControlView 메시지 처리기


int ControlView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.

	if (FALSE == m_ConDlg.Create(IDD_CONTROLDLG, this))
	{
		AfxMessageBox(_T("컨트롤 DLG 만들기 오류"), MB_OK | MB_ICONEXCLAMATION);
		return -1;
	}

	RECT Rc = { 0,0 };
	this->GetClientRect(&Rc);
	m_ConDlg.SetWindowPos(this, Rc.left - 2, Rc.top, Rc.right - 4 * Rc.left, Rc.bottom + 10, SWP_NOZORDER);
	m_ConDlg.ShowWindow(SW_SHOW);


	return 0;
}
