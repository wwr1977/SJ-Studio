#include "stdafx.h"
#include "FixSpWnd.h"


CFixSpWnd::CFixSpWnd()
{
}


CFixSpWnd::~CFixSpWnd()
{
}
BEGIN_MESSAGE_MAP(CFixSpWnd, CSplitterWnd)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_SETCURSOR()
END_MESSAGE_MAP()


void CFixSpWnd::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	if (false == m_bLock)
	{
		CSplitterWnd::OnLButtonDown(nFlags, point);
	}
}


void CFixSpWnd::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	if (false == m_bLock)
	{
		CSplitterWnd::OnMouseMove(nFlags, point);
	}
}


BOOL CFixSpWnd::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	if (false == m_bLock)
	{
		return CSplitterWnd::OnSetCursor(pWnd, nHitTest, message);
	}
	return TRUE;
}


BOOL CFixSpWnd::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	m_cySplitter = 0;

	m_cxBorder = 0;
	m_cyBorder = 0;

	m_cxSplitterGap = 1;
	m_cySplitterGap = 1;
	return CSplitterWnd::PreCreateWindow(cs);
}




