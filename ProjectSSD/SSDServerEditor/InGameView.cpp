// InGameView.cpp: 구현 파일
//

#include "stdafx.h"
#include "InGameView.h"



// InGameView

IMPLEMENT_DYNCREATE(InGameView, CView)

InGameView::InGameView()
{

}

InGameView::~InGameView()
{
}

BEGIN_MESSAGE_MAP(InGameView, CView)
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
