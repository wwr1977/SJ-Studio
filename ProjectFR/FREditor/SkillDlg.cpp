// SkillDlg.cpp : 구현 파일입니다.
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


// CSkillDlg 대화 상자입니다.

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


// CSkillDlg 메시지 처리기입니다.


void CSkillDlg::OnDestroy()
{
	for (size_t i = 0; i < m_vecSubSkillDlg.size(); i++)
	{
		m_vecSubSkillDlg[i]->SendMessage(WM_DESTROY);
		SAFE_DELETE(m_vecSubSkillDlg[i]);
	}

	m_vecSubSkillDlg.clear();

	CDialogEx::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}


int CSkillDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
	m_DlgRect = { lpCreateStruct->x,lpCreateStruct->y,lpCreateStruct->cx,lpCreateStruct->cy };


	return 0;
}


void CSkillDlg::OnOK()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	
	return;
	//CDialogEx::OnOK();
}


BOOL CSkillDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	CreateTab<CActionScriptDlg>(IDD_ACTIONSCRIPT, _T("액션 스크립트"));
	CreateTab<CSkillDataDlg>(IDD_SKILLDATADLG, _T("스킬 데이터"));


	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CSkillDlg::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	lpMMI->ptMaxTrackSize.x = m_DlgRect.right - m_DlgRect.left;
	lpMMI->ptMaxTrackSize.y = m_DlgRect.bottom - m_DlgRect.top;
	lpMMI->ptMinTrackSize.x = m_DlgRect.right - m_DlgRect.left;
	lpMMI->ptMinTrackSize.y = m_DlgRect.bottom - m_DlgRect.top;


	CDialogEx::OnGetMinMaxInfo(lpMMI);
}


void CSkillDlg::OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
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
		
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if (TRUE == bShow)
	{
		// 처음 스킬탭이 눌렸을때만 작동
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
