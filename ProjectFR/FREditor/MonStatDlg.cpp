// MonStatDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "FREditor.h"
#include "MonStatDlg.h"
#include "afxdialogex.h"


// CMonStatDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CMonStatDlg, CDialogEx)

CMonStatDlg::CMonStatDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_MONSTAT, pParent)
{

}

CMonStatDlg::~CMonStatDlg()
{
}

void CMonStatDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CMonStatDlg, CDialogEx)
END_MESSAGE_MAP()


// CMonStatDlg �޽��� ó�����Դϴ�.


BOOL CMonStatDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.

	CEditGlobal::MonStatDlg = this;
	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}
