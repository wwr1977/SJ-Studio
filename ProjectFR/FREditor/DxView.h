#pragma once


// CDxView ���Դϴ�.

class CDxView : public CView
{
	DECLARE_DYNCREATE(CDxView)

protected:
	CDxView();           // ���� ����⿡ ���Ǵ� protected �������Դϴ�.
	virtual ~CDxView();

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
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
};


