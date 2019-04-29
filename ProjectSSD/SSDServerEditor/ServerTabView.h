#pragma once
#include "RootDlg.h"

// CServerTabView ���Դϴ�.

class CServerTabView : public CView
{
	DECLARE_DYNCREATE(CServerTabView)

private:
	CRootDlg				m_RootTabDlg;

protected:
	CServerTabView();           // ���� ����⿡ ���Ǵ� protected �������Դϴ�.
	virtual ~CServerTabView();

public:
	virtual void OnDraw(CDC* pDC);      // �� �並 �׸��� ���� �����ǵǾ����ϴ�.
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};


