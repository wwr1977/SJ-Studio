// CharacterDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "FREditor.h"
#include "CharacterDlg.h"
#include "afxdialogex.h"
#include "MultiSpriteDlg.h"				// ���� ��
#include "AniSpriteDlg.h"				// ���� ��
#include "AnimationDlg.h"
#include "EffectDlg.h"
#include "SkillDlg.h"

// CCharacterDlg ��ȭ �����Դϴ�.

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


// CCharacterDlg �޽��� ó�����Դϴ�.


void CCharacterDlg::OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	*pResult = 0;

	TABINDEX Index =(TABINDEX)m_CharTabCtrl.GetCurSel();

	ShowDlg(Index);
}


int CCharacterDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  ���⿡ Ư��ȭ�� �ۼ� �ڵ带 �߰��մϴ�.
	m_DlgRect = { lpCreateStruct->x,lpCreateStruct->y,lpCreateStruct->cx,lpCreateStruct->cy };
	
	return 0;
}


void CCharacterDlg::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	lpMMI->ptMaxTrackSize.x = m_DlgRect.right - m_DlgRect.left;
	lpMMI->ptMaxTrackSize.y = m_DlgRect.bottom - m_DlgRect.top;
	lpMMI->ptMinTrackSize.x = m_DlgRect.right - m_DlgRect.left;
	lpMMI->ptMinTrackSize.y = m_DlgRect.bottom - m_DlgRect.top;

	CDialogEx::OnGetMinMaxInfo(lpMMI);
}


BOOL CCharacterDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	//CreateTab<CMultiSpriteDlg>(IDD_MULTISPRITE, _T("��Ƽ��������Ʈ"));
	CreateTab<CAniSpriteDlg>(IDD_NEWMULTISPRITE, _T("��Ƽ��������Ʈ"));
	CreateTab<CAnimationDlg>(IDD_ANIMATION, _T("�ִϸ��̼�"));
	CreateTab<CEffectDlg>(IDD_EFFECT, _T("����Ʈ"));
	CreateTab<CSkillDlg>(IDD_SKILLDLG, _T("��ų"));

	ShowDlg(MULTISPRITE);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
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

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.

}


BOOL CCharacterDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
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
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	SetCursor(LoadCursor(NULL, IDC_ARROW));
	return TRUE;
	//return CDialogEx::OnSetCursor(pWnd, nHitTest, message);
}
