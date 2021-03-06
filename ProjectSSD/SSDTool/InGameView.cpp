// InGameView.cpp: 구현 파일
//

#include "stdafx.h"
#include "SSDTool.h"
#include "InGameView.h"
#include <KeyMgr.h>
#include "EditDlg.h"
#include "SSDToolGlobal.h"
#include "LightDlg.h"

// InGameView

IMPLEMENT_DYNCREATE(InGameView, CView)

InGameView::InGameView()
{

}

InGameView::~InGameView()
{
}

BEGIN_MESSAGE_MAP(InGameView, CView)
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()


// InGameView 그리기

void InGameView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: 여기에 그리기 코드를 추가합니다.
}


// InGameView 진단

#ifdef _DEBUG
void InGameView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void InGameView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// InGameView 메시지 처리기

void InGameView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CView::OnMouseMove(nFlags, point);

	if (nullptr != SSDToolGlobal::ToolEditDlg)
	{
		SSDToolGlobal::ToolEditDlg->ChangeActorTrans();
	}
	if (nullptr != SSDToolGlobal::ToolLightDlg)
	{
		//SSDToolGlobal::ToolLightDlg->SpotLightDirUpdate();
	}
}
