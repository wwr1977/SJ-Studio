// CharacterDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "FREditor.h"
#include "CharacterDlg.h"
#include "afxdialogex.h"
#include "MultiSpriteDlg.h"				// 수정 전
#include "AniSpriteDlg.h"				// 수정 후
#include "AnimationDlg.h"
#include "EffectDlg.h"
#include "SkillDlg.h"

// CCharacterDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CCharacterDlg, CDialogEx)

CCharacterDlg::CCharacterDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_CHARACTER, pParent)
{
	CEditGlobal::CharacterDlg = this;
}

CCharacterDlg::~CCharacterDlg()
{
}

void CCharacterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_CharTabCtrl);
}


BEGIN_MESSAGE_MAP(CCharacterDlg, CDialogEx)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CCharacterDlg::OnTcnSelchangeTab1)
	ON_WM_CREATE()
	ON_WM_GETMINMAXINFO()
	ON_WM_DESTROY()
	ON_WM_SETCURSOR()
END_MESSAGE_MAP()


// CCharacterDlg 메시지 처리기입니다.


void CCharacterDlg::OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;

	TABINDEX Index =(TABINDEX)m_CharTabCtrl.GetCurSel();

	ShowDlg(Index);
}


int CCharacterDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
	m_DlgRect = { lpCreateStruct->x,lpCreateStruct->y,lpCreateStruct->cx,lpCreateStruct->cy };
	
	return 0;
}


void CCharacterDlg::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	lpMMI->ptMaxTrackSize.x = m_DlgRect.right - m_DlgRect.left;
	lpMMI->ptMaxTrackSize.y = m_DlgRect.bottom - m_DlgRect.top;
	lpMMI->ptMinTrackSize.x = m_DlgRect.right - m_DlgRect.left;
	lpMMI->ptMinTrackSize.y = m_DlgRect.bottom - m_DlgRect.top;

	CDialogEx::OnGetMinMaxInfo(lpMMI);
}


BOOL CCharacterDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	//CreateTab<CMultiSpriteDlg>(IDD_MULTISPRITE, _T("멀티스프라이트"));
	CreateTab<CAniSpriteDlg>(IDD_NEWMULTISPRITE, _T("멀티스프라이트"));
	CreateTab<CAnimationDlg>(IDD_ANIMATION, _T("애니메이션"));
	CreateTab<CEffectDlg>(IDD_EFFECT, _T("이팩트"));
	CreateTab<CSkillDlg>(IDD_SKILLDLG, _T("스킬"));

	ShowDlg(MULTISPRITE);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CCharacterDlg::OnDestroy()
{
	for (size_t i = 0; i < m_vecCharDlg.size(); i++)
	{
		m_vecCharDlg[i]->SendMessage(WM_DESTROY);
		SAFE_DELETE(m_vecCharDlg[i]);
	}
	m_vecCharDlg.clear();

	CDialogEx::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.

}


BOOL CCharacterDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (pMsg->message == WM_KEYDOWN) {
		if (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE) {
			// VK_RETURN  : Enter Key
			// VK_ESCAPE  : ESC key

			return TRUE;
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}


BOOL CCharacterDlg::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	SetCursor(LoadCursor(NULL, IDC_ARROW));
	return TRUE;
	//return CDialogEx::OnSetCursor(pWnd, nHitTest, message);
}
