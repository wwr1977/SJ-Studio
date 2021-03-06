// ListView.cpp: 구현 파일
//

#include "stdafx.h"
#include "SSDTool.h"
#include "ListView.h"


// ListView

IMPLEMENT_DYNCREATE(ListView, CView)

ListView::ListView()
{

}

ListView::~ListView()
{
}

BEGIN_MESSAGE_MAP(ListView, CView)
	ON_WM_LBUTTONDOWN()
	ON_WM_CREATE()
END_MESSAGE_MAP()


// ListView 그리기

void ListView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: 여기에 그리기 코드를 추가합니다.
}


// ListView 진단

#ifdef _DEBUG
void ListView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void ListView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// ListView 메시지 처리기


int ListView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
	if (FALSE == m_SceneDlg.Create(IDD_SCENELIST, this))
	{
		AfxMessageBox(_T("씬 DLG 만들기 오류"), MB_OK | MB_ICONEXCLAMATION);
		return -1;
	}

	if (FALSE == m_ObjectDlg.Create(IDD_OBJECTDLG, this))
	{
		AfxMessageBox(_T("오브젝트 DLG 만들기 오류"), MB_OK | MB_ICONEXCLAMATION);
		return -1;
	}

	RECT Rc = { 0,0 };
	this->GetClientRect(&Rc);
	long X = Rc.right - Rc.left;
	long Y = (Rc.bottom - Rc.top) / 3;

	m_SceneDlg.SetWindowPos(this, 0, 0, 300, 400, SWP_NOZORDER);
	m_ObjectDlg.SetWindowPos(this, 0, 400, 300, Rc.bottom, SWP_NOZORDER);

	m_SceneDlg.ShowWindow(SW_SHOW);
	m_ObjectDlg.ShowWindow(SW_SHOW);
	return 0;
}
