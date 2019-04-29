// DxView.cpp : ���� �����Դϴ�.
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


// CDxView �׸����Դϴ�.

void CDxView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: ���⿡ �׸��� �ڵ带 �߰��մϴ�.
}


// CDxView �����Դϴ�.

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


// CDxView �޽��� ó�����Դϴ�.


BOOL CDxView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	// ���� �ڷ� ������(ī�޶� ������ ����, �̹����� Ŀ���δ�, ī�޶� ��ü ȭ���� �ø���)
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
	// ���� ������ ������(ī�޶�� �ڷ� ����)
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
