// TextureListDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "FREditor.h"
#include "TextureListDlg.h"
#include "afxdialogex.h"
#include"AniSpriteDlg.h"


// CTextureListDlg ��ȭ �����Դϴ�.

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

// CTextureListDlg �޽��� ó�����Դϴ�.





void CTextureListDlg::OnOK()
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.

	return ;
}


BOOL CTextureListDlg::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	SetCursor(LoadCursor(NULL, IDC_ARROW));
	return TRUE;
}


BOOL CTextureListDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	if (pMsg->message == WM_KEYDOWN) {
		if (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE) {
			// VK_RETURN  : Enter Key
			// VK_ESCAPE  : ESC key

			return TRUE;
		}
	}
	if (pMsg->message == WM_KEYDOWN && pMsg->hwnd == GetDlgItem(IDC_TEXTURELIST)->m_hWnd)
	{
		// List�� ��Ŀ�̵Ǿ����� A~ZŰ�� �ԷµǾ����� �ش� Ű�� Event ����
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

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	CEditGlobal::TextureList = this;

	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}


void CTextureListDlg::OnLbnSelchangeTexturelist()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	int Index = m_ListCtrl.GetCurSel();

	if (m_iPrevIndex == Index)
	{
		return;
	}

	CString ImageKey;
	m_ListCtrl.GetText(Index, ImageKey);

	// ����Ʈ ��Ͽ� ���� �̹��� �̸��� Texture��ü �� �ǹ��ϴ� _T�� �پ ����
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
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
}
