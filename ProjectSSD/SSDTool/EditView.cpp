// EditView.cpp: 구현 파일
//

#include "stdafx.h"
#include "SSDTool.h"
#include "EditView.h"
#include "SSDToolGlobal.h"

// EditView

IMPLEMENT_DYNCREATE(EditView, CView)

EditView::EditView()
{

}

EditView::~EditView()
{
}

BEGIN_MESSAGE_MAP(EditView, CView)
	ON_WM_CREATE()
END_MESSAGE_MAP()


// EditView 그리기

void EditView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: 여기에 그리기 코드를 추가합니다.
}


// EditView 진단

#ifdef _DEBUG
void EditView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void EditView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// EditView 메시지 처리기



int EditView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.


	if (FALSE == m_EditDlg.Create(IDD_EDITDLG, this))
	{
		AfxMessageBox(_T("에디터 DLG 만들기 오류"), MB_OK | MB_ICONEXCLAMATION);
		return -1;
	}

	RECT Rc = { 0,0 };
	this->GetClientRect(&Rc);
	m_EditDlg.SetWindowPos(this, Rc.left - 2, Rc.top, Rc.right - 4 * Rc.left, Rc.bottom + 10, SWP_NOZORDER);
	//m_EditDlg.SetWindowPos(this, 300 - 2, 0, 600, 961, SWP_NOZORDER);
	m_EditDlg.ShowWindow(SW_SHOW);

	SSDToolGlobal::ToolEditView = this;
	return 0;
}
