// ControlDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "SSDServerEditor.h"
#include "ControlDlg.h"
#include "afxdialogex.h"


// CControlDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CControlDlg, CDialogEx)

CControlDlg::CControlDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_CONTROLDLG, pParent)
{

}

CControlDlg::~CControlDlg()
{
}

void CControlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CControlDlg, CDialogEx)
END_MESSAGE_MAP()


// CControlDlg �޽��� ó�����Դϴ�.


BOOL CControlDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	if (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)
		return TRUE;

	return CDialogEx::PreTranslateMessage(pMsg);
}
