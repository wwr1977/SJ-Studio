// PacketDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "SSDServerEditor.h"
#include "PacketDlg.h"
#include "afxdialogex.h"


// CPacketDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CPacketDlg, CDialogEx)

CPacketDlg::CPacketDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_PACKETDLG, pParent)
{

}

CPacketDlg::~CPacketDlg()
{
}

void CPacketDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_PacketLogList);
}


BEGIN_MESSAGE_MAP(CPacketDlg, CDialogEx)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CPacketDlg �޽��� ó�����Դϴ�.


void CPacketDlg::OnClose()
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	return;
	CDialogEx::OnClose();
}


void CPacketDlg::OnOK()
{
	return;
}


BOOL CPacketDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.

	if (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE) 
		return TRUE;


	return CDialogEx::PreTranslateMessage(pMsg);
}
