// RootDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "SSDServerEditor.h"
#include "RootDlg.h"
#include "afxdialogex.h"
#include "PacketDlg.h"
#include <DXMacro.h>


// CRootDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CRootDlg, CDialogEx)

CRootDlg::CRootDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_SERVERTABDLG, pParent)
{

}

CRootDlg::~CRootDlg()
{
}

void CRootDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ROOTTAB, m_RootTabCtrl);
}


BEGIN_MESSAGE_MAP(CRootDlg, CDialogEx)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CRootDlg �޽��� ó�����Դϴ�.


BOOL CRootDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	CreateTab<CPacketDlg>(IDD_PACKETDLG, L"��Ŷ �α�");

	ShowDlg(PACKETLOG);
	SetBackgroundColor(RGB(0, 0, 0));
	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}


void CRootDlg::OnDestroy()
{
	for (size_t i = 0; i < m_vecDlg.size(); i++)
	{
		m_vecDlg[i]->SendMessage(WM_DESTROY);
		SAFE_DELETE(m_vecDlg[i]);
	}
	m_vecDlg.clear();


	CDialogEx::OnDestroy();

}


BOOL CRootDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	if (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)
		return TRUE;


	return CDialogEx::PreTranslateMessage(pMsg);
}
