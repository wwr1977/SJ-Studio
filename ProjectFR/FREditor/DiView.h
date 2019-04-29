#pragma once
#include"RootDlg.h"

// CDiView ���Դϴ�.

class CDiView : public CView
{
	DECLARE_DYNCREATE(CDiView)

protected:
	CDiView();           // ���� ����⿡ ���Ǵ� protected �������Դϴ�.
	virtual ~CDiView();

public:
	virtual void OnDraw(CDC* pDC);      // �� �並 �׸��� ���� �����ǵǾ����ϴ�.
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif
	CRootDlg			m_EditDlg;

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
};


