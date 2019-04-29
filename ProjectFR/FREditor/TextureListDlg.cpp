// TextureListDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "FREditor.h"
#include "TextureListDlg.h"
#include "afxdialogex.h"
#include"AniSpriteDlg.h"


// CTextureListDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CTextureListDlg, CDialogEx)

CTextureListDlg::CTextureListDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_TEXTURELIST, pParent)
	, m_iPrevIndex(-1)
{

}

CTextureListDlg::~CTextureListDlg()
{
}

void CTextureListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TEXTURELIST, m_ListCtrl);
}


BEGIN_MESSAGE_MAP(CTextureListDlg, CDialogEx)
	ON_WM_SETCURSOR()
	ON_LBN_SELCHANGE(IDC_TEXTURELIST, &CTextureListDlg::OnLbnSelchangeTexturelist)
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()
//////////////////////////////////////////////////			User Custom Function	////////////////////////////////////////
CString CTextureListDlg::GetLastSelectString()
{
	return m_LastSelectString;
}
void CTextureListDlg::ResetList(const CString& _InitString /*= _T("")*/)
{
	m_ListCtrl.ResetContent();

	if (_T("") != _InitString) 
	{
		m_ListCtrl.AddString(_InitString);
	}

	return;
}
void CTextureListDlg::UpdateList(FOLDER _FolderIndex)
{
	CEditGlobal::TexSpriteListUpdate(&m_ListCtrl, _FolderIndex);

	if (0 == m_ListCtrl.GetCount())
	{
		m_ListCtrl.AddString(_T("                              Empty..."));
	}
}

CString CTextureListDlg::GetCurSelectString()
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

// CTextureListDlg 메시지 처리기입니다.





void CTextureListDlg::OnOK()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	return ;
}


BOOL CTextureListDlg::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	SetCursor(LoadCursor(NULL, IDC_ARROW));
	return TRUE;
}


BOOL CTextureListDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (pMsg->message == WM_KEYDOWN) {
		if (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE) {
			// VK_RETURN  : Enter Key
			// VK_ESCAPE  : ESC key

			return TRUE;
		}
	}
	if (pMsg->message == WM_KEYDOWN && pMsg->hwnd == GetDlgItem(IDC_TEXTURELIST)->m_hWnd)
	{
		// List가 포커싱되었을때 A~Z키가 입력되었으면 해당 키의 Event 무시
		if (0x41 <= pMsg->wParam && 0x5A >= pMsg->wParam)
		{
			return TRUE;
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}


BOOL CTextureListDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	CEditGlobal::TextureList = this;

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CTextureListDlg::OnLbnSelchangeTexturelist()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int Index = m_ListCtrl.GetCurSel();

	if (m_iPrevIndex == Index)
	{
		return;
	}

	CString ImageKey;
	m_ListCtrl.GetText(Index, ImageKey);

	// 리스트 목록에 누른 이미지 이름은 Texture전체 를 의미하는 _T를 붙어서 저장
	CEditGlobal::AniSpriteDlg->SelectTextureList(ImageKey);

	if (0 == m_ListCtrl.GetCount())
	{
		m_ListCtrl.AddString(_T("                              Empty..."));
	}

	m_iPrevIndex = Index;
	m_LastSelectString = ImageKey;

}


void CTextureListDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	if (TRUE == bShow) 
	{
		m_iPrevIndex = -1;
	}
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}
