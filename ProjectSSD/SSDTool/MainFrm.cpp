
// MainFrm.cpp: CMainFrame 클래스의 구현
//

#include "stdafx.h"
#include "SSDTool.h"

#include "MainFrm.h"
#include "InGameView.h"
#include "ListView.h"
#include "EditView.h"
#include "ControlView.h"
#include "ToolCoreBuilder.h"

#include <SSDCore.h>
#include <KeyMgr.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
	ON_WM_SETCURSOR()
END_MESSAGE_MAP()

// CMainFrame 생성/소멸

CMainFrame::CMainFrame() noexcept
{
	// TODO: 여기에 멤버 초기화 코드를 추가합니다.
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_Sp.CreateStatic(this, 1, 3, WS_CHILD | WS_VISIBLE);

	RECT Size;
	this->GetClientRect(&Size);

	m_SubSp.CreateStatic(&m_Sp, 2, 1, WS_CHILD | WS_VISIBLE | WS_BORDER, m_Sp.IdFromRowCol(0, 2));

	m_SubSp.CreateView(0, 0, RUNTIME_CLASS(InGameView), SIZE{ 1280, 678 }, nullptr);
	InGameView* NewView = (InGameView*)m_SubSp.GetPane(0, 0);
	Core::CoreInit<CToolCoreBuilder>(lpCreateStruct->hInstance, L"SSD Tool", NewView->m_hWnd);

	m_SubSp.CreateView(1, 0, RUNTIME_CLASS(ControlView), SIZE{ 1280, 678 }, nullptr);
	m_Sp.CreateView(0, 0, RUNTIME_CLASS(ListView), SIZE{ 270, Size.bottom }, nullptr);
	m_Sp.CreateView(0, 1, RUNTIME_CLASS(EditView), SIZE{ 330, Size.bottom }, nullptr);


	m_Sp.ModifyStyleEx(WS_EX_CLIENTEDGE, 0, SWP_FRAMECHANGED);
	

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if (!CFrameWnd::PreCreateWindow(cs))
		return FALSE;
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	cs.style = WS_OVERLAPPED | WS_CAPTION | FWS_ADDTOTITLE
		| WS_SYSMENU;

	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.lpszClass = AfxRegisterWndClass(0);

	cs.cx = 1900;
	cs.cy = 1020;
	cs.x = 10;
	cs.y = 0;

	return TRUE;
}

// CMainFrame 진단

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}
#endif //_DEBUG


// CMainFrame 메시지 처리기

void CMainFrame::OnSetFocus(CWnd* /*pOldWnd*/)
{
	// 뷰 창으로 포커스를 이동합니다.
	//m_wndView.SetFocus();
}

BOOL CMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	// 뷰에서 첫째 크랙이 해당 명령에 나타나도록 합니다.
	//if (m_wndView.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
		//return TRUE;



	// 그렇지 않으면 기본 처리합니다.
	return CFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}



BOOL CMainFrame::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	switch (pMsg->message)
	{
	case WM_DESTROY:
	case WM_CLOSE:
	case WM_QUIT:
		Core::QuitCore(true);
	default:
		break;
	}

	return CFrameWnd::PreTranslateMessage(pMsg);
}


BOOL CMainFrame::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	return CFrameWnd::OnSetCursor(pWnd, nHitTest, message);
}
