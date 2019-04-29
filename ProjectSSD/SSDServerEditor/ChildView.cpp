
// ChildView.cpp : CChildView Ŭ������ ����
//
#include "stdafx.h"
#include "SSDServerEditor.h"
#include "ChildView.h"
#include "GlobalServerViewer.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CChildView

CChildView::CChildView()
{
}

CChildView::~CChildView()
{
}


BEGIN_MESSAGE_MAP(CChildView, CWnd)
	ON_WM_PAINT()
	ON_COMMAND(ID_LOGIC_CONNECT, &CChildView::OnLogicConnect)
	ON_COMMAND(ID_LOGIC_DISCONNECT, &CChildView::OnLogicDisconnect)
END_MESSAGE_MAP()



// CChildView �޽��� ó����

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(NULL, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW+1), NULL);

	return TRUE;
}

void CChildView::OnPaint() 
{
	CPaintDC dc(this); // �׸��⸦ ���� ����̽� ���ؽ�Ʈ�Դϴ�.
	
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	
	// �׸��� �޽����� ���ؼ��� CWnd::OnPaint()�� ȣ������ ���ʽÿ�.
}



void CChildView::OnLogicConnect()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	if (true == GlobalServerViewer::ConnectToLogicServer())
	{
		AfxMessageBox(_T("���� ���� ���� ����"), MB_OK | MB_ICONEXCLAMATION);
	}
	else 
	{
		AfxMessageBox(_T("���� ���� ���� ����"), MB_OK | MB_ICONEXCLAMATION);
	}

}


void CChildView::OnLogicDisconnect()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.

	if (nullptr == GlobalServerViewer::pLogicClient)
	{
		AfxMessageBox(_T("���� ������ ����Ǿ����� �ʽ��ϴ�."), MB_OK | MB_ICONEXCLAMATION);
	}

	if ( true == GlobalServerViewer::DisConnectToLogicServer())
	{
		AfxMessageBox(_T("���� ���� ���� ���� ����"), MB_OK | MB_ICONEXCLAMATION);
	}
	else
	{
		AfxMessageBox(_T("���� ���� ���� ���� ����"), MB_OK | MB_ICONEXCLAMATION);
	}
	
}
