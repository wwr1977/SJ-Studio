// MultiSpriteListDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "FREditor.h"
#include "MultiSpriteListDlg.h"
#include "afxdialogex.h"
#include"AniSpriteDlg.h"


// CMultiSpriteListDlg ��ȭ �����Դϴ�.

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
	//����Ʈ�۵��� ���� ���õ� ���ڿ��� ����
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


// CMultiSpriteListDlg �޽��� ó�����Դϴ�.
void CMultiSpriteListDlg::OnOK()
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	
	return;
}


BOOL CMultiSpriteListDlg::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	SetCursor(LoadCursor(NULL, IDC_ARROW));
	return TRUE;
}


BOOL CMultiSpriteListDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	if (pMsg->message == WM_KEYDOWN) {
		if (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE) {
			// VK_RETURN  : Enter Key
			// VK_ESCAPE  : ESC key

			return TRUE;
		}
	}
	if (pMsg->message == WM_KEYDOWN && pMsg->hwnd == GetDlgItem(IDC_MULTISPRITELIST)->m_hWnd)
	{
		// List�� ��Ŀ�̵Ǿ����� A~ZŰ�� �ԷµǾ����� �ش� Ű�� Event ����
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

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	CEditGlobal::MultiSpriteList = this;

	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}


void CMultiSpriteListDlg::OnLbnSelchangeMultispritelist()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
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

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	if (TRUE == bShow)
	{
		m_iPrevIndex = -1;
	}
}
