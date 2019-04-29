// SkillDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "FREditor.h"
#include "SkillDlg.h"
#include "afxdialogex.h"
#include "ActionScriptDlg.h"
#include "SkillDataDlg.h"
#include "SkillMgr.h"
#include "EffectDlg.h"
#include "RootDlg.h"


// CSkillDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CSkillDlg, CDialogEx)

CSkillDlg::CSkillDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_SKILLDLG, pParent)
	, m_DlgRect(RECT{0,0}), m_CurTabIndex(MAXSKILLTABINDEX)
{
	CEditGlobal::SkillDlg = this;
}

CSkillDlg::~CSkillDlg()
{
}

void CSkillDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_SkillTabCtrl);
}


BEGIN_MESSAGE_MAP(CSkillDlg, CDialogEx)
	ON_WM_DESTROY()
	ON_WM_CREATE()
	ON_WM_GETMINMAXINFO()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CSkillDlg::OnTcnSelchangeTab1)
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


// CSkillDlg �޽��� ó�����Դϴ�.


void CSkillDlg::OnDestroy()
{
	for (size_t i = 0; i < m_vecSubSkillDlg.size(); i++)
	{
		m_vecSubSkillDlg[i]->SendMessage(WM_DESTROY);
		SAFE_DELETE(m_vecSubSkillDlg[i]);
	}

	m_vecSubSkillDlg.clear();

	CDialogEx::OnDestroy();

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
}


int CSkillDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  ���⿡ Ư��ȭ�� �ۼ� �ڵ带 �߰��մϴ�.
	m_DlgRect = { lpCreateStruct->x,lpCreateStruct->y,lpCreateStruct->cx,lpCreateStruct->cy };


	return 0;
}


void CSkillDlg::OnOK()
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	
	return;
	//CDialogEx::OnOK();
}


BOOL CSkillDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	CreateTab<CActionScriptDlg>(IDD_ACTIONSCRIPT, _T("�׼� ��ũ��Ʈ"));
	CreateTab<CSkillDataDlg>(IDD_SKILLDATADLG, _T("��ų ������"));


	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}


void CSkillDlg::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	lpMMI->ptMaxTrackSize.x = m_DlgRect.right - m_DlgRect.left;
	lpMMI->ptMaxTrackSize.y = m_DlgRect.bottom - m_DlgRect.top;
	lpMMI->ptMinTrackSize.x = m_DlgRect.right - m_DlgRect.left;
	lpMMI->ptMinTrackSize.y = m_DlgRect.bottom - m_DlgRect.top;


	CDialogEx::OnGetMinMaxInfo(lpMMI);
}


void CSkillDlg::OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	*pResult = 0;

	SKILLTABINDEX Index = (SKILLTABINDEX)m_SkillTabCtrl.GetCurSel();

	ShowDlg(Index);
}


void CSkillDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	if (true == CEditGlobal::EditSkillMgr->CreateBattleUnit())
	{
		CEditGlobal::EditSkillMgr->MgrOn();
		return;
	}
		
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	if (TRUE == bShow)
	{
		// ó�� ��ų���� ���������� �۵�
		if (MAXSKILLTABINDEX == m_CurTabIndex)
		{
			ShowDlg(ACTIONSCRIPTDLG);

			CEditGlobal::EffectDlg->InitEffectDlg();
			CEditGlobal::EffectDlg->DataLoadButtonEvent();

			if (nullptr != CEditGlobal::EditSkillMgr)
				CThreadMgr::CreateGlobvalThread(SkillMgrOn);
		}
		else 
		{
			CEditGlobal::EditSkillMgr->MgrOn();
		}
	}
	else 
	{
		CEditGlobal::EditSkillMgr->MgrOff();
	}
}
