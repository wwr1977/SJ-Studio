// MonStatDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "FREditor.h"
#include "MonStatDlg.h"
#include "afxdialogex.h"


// CMonStatDlg 대화 상자입니다.

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


// CMonStatDlg 메시지 처리기입니다.


BOOL CMonStatDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	CEditGlobal::MonStatDlg = this;
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}
