// MultiSpriteListDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "FREditor.h"
#include "MultiSpriteListDlg.h"
#include "afxdialogex.h"
#include"AniSpriteDlg.h"


// CMultiSpriteListDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CMultiSpriteListDlg, CDialogEx)

CMultiSpriteListDlg::CMultiSpriteListDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_MULTISPRITELIST, pParent)
	, m_iPrevIndex(-1)
{

}

CMultiSpriteListDlg::~CMultiSpriteListDlg()
{
}

void CMultiSpriteListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MULTISPRITELIST, m_ListCtrl);
}


BEGIN_MESSAGE_MAP(CMultiSpriteListDlg, CDialogEx)
	ON_WM_SETCURSOR()
	ON_LBN_SELCHANGE(IDC_MULTISPRITELIST, &CMultiSpriteListDlg::OnLbnSelchangeMultispritelist)
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()

//////////////////////////////////////////////////			User Custom Function	////////////////////////////////////////
void CMultiSpriteListDlg::ResetList(const CString& _InitString /*= _T("")*/)
{
	m_ListCtrl.ResetContent();

	if (_T("") != _InitString)
	{
		m_ListCtrl.AddString(_InitString);
	}

	return;
}
void CMultiSpriteListDlg::UpdateList(ANISPRITETYPE _AniSpriteType)
{
	CEditGlobal::AniSpriteListUpdate(&m_ListCtrl, _AniSpriteType);

	if (0 == m_ListCtrl.GetCount()) 
	{
		m_ListCtrl.AddString(_T("                              Empty..."));
	}
}

CString CMultiSpriteListDlg::GetLastSelectString()
{
	return m_LastSelectString;
}
CString CMultiSpriteListDlg::GetCurSelectString()
{
	int StringIndex = m_ListCtrl.GetCurSel();
	if (-1 == StringIndex) 
	{
		return _T("");
	}
	CString CurSelectString; 
	m_ListCtrl.GetText(StringIndex, CurSelectString);
	return CurSelectString;
}

BOOL CMultiSpriteListDlg::EraseString(const CString& _EraseString /*= _T("")*/)
{
	//디폴트작동은 현재 선택된 문자열을 제거
	if (_T("") == _EraseString) 
	{
		int CurSel = m_ListCtrl.GetCurSel();
		m_ListCtrl.DeleteString(CurSel);
		return TRUE;
	}

	int EraseIndex = m_ListCtrl.FindString(0,_EraseString);

	if (-1 != EraseIndex)
	{
		m_ListCtrl.DeleteString(EraseIndex);
		return TRUE;
	}
	return FALSE;
}


// CMultiSpriteListDlg 메시지 처리기입니다.
void CMultiSpriteListDlg::OnOK()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	
	return;
}


BOOL CMultiSpriteListDlg::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	SetCursor(LoadCursor(NULL, IDC_ARROW));
	return TRUE;
}


BOOL CMultiSpriteListDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (pMsg->message == WM_KEYDOWN) {
		if (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE) {
			// VK_RETURN  : Enter Key
			// VK_ESCAPE  : ESC key

			return TRUE;
		}
	}
	if (pMsg->message == WM_KEYDOWN && pMsg->hwnd == GetDlgItem(IDC_MULTISPRITELIST)->m_hWnd)
	{
		// List가 포커싱되었을때 A~Z키가 입력되었으면 해당 키의 Event 무시
		if (0x41 <= pMsg->wParam && 0x5A >= pMsg->wParam)
		{
			return TRUE;
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}


BOOL CMultiSpriteListDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	CEditGlobal::MultiSpriteList = this;

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CMultiSpriteListDlg::OnLbnSelchangeMultispritelist()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int Index = m_ListCtrl.GetCurSel();

	if (m_iPrevIndex == Index)
	{
		return;
	}

	CString ImageKey;
	m_ListCtrl.GetText(Index, ImageKey);

	CEditGlobal::AniSpriteDlg->SelectMultiSpriteList(ImageKey);

	m_iPrevIndex = Index;
	m_LastSelectString = ImageKey;
}


void CMultiSpriteListDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if (TRUE == bShow)
	{
		m_iPrevIndex = -1;
	}
}
