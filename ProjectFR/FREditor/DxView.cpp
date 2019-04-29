// DxView.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "FREditor.h"
#include "DxView.h"
#include<Camera.h>


// CDxView

IMPLEMENT_DYNCREATE(CDxView, CView)

CDxView::CDxView()
{

}

CDxView::~CDxView()
{
}

BEGIN_MESSAGE_MAP(CDxView, CView)
//	ON_WM_MOUSEWHEEL()
END_MESSAGE_MAP()


// CDxView 그리기입니다.

void CDxView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: 여기에 그리기 코드를 추가합니다.
}


// CDxView 진단입니다.

#ifdef _DEBUG
void CDxView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CDxView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// CDxView 메시지 처리기입니다.


BOOL CDxView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	// 휠을 뒤로 돌리면(카메라가 앞으로 간다, 이미지가 커보인다, 카메라 전체 화면을 늘린다)
	float CurCameraRatio = CEditGlobal::EditMainCamera->GetZoomRatio();

	if (0 > zDelta) 
	{
		if (CurCameraRatio <= 0.15f) 
		{
			CEditGlobal::EditMainCamera->SetZoomRatio(0.15f);
		}
		else
		{
			CEditGlobal::EditMainCamera->SetZoomRatio(CurCameraRatio - 0.05f);
		}
	}
	// 휠을 앞으로 돌리면(카메라는 뒤로 간다)
	else 
	{
		if (CurCameraRatio >= 2.0f)
		{
			CEditGlobal::EditMainCamera->SetZoomRatio(2.0f);
		}
		else 
		{
			CEditGlobal::EditMainCamera->SetZoomRatio(CurCameraRatio + 0.05f);
		}
	}
	return CView::OnMouseWheel(nFlags, zDelta, pt);
}
