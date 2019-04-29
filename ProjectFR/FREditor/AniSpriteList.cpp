// AniSpriteList.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "FREditor.h"
#include "AniSpriteList.h"
#include "afxdialogex.h"
#include"AnimationDlg.h"
#include<GameMultiSprite.h>

// CAniSpriteList ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CAniSpriteList, CDialogEx)

CAniSpriteList::CAniSpriteList(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_ANISPRITELIST, pParent),m_iPrevIndex(-1)
	, m_iLastSelectIndex(-1)
	, m_LastSelectString(_T(""))
{

}

CAniSpriteList::~CAniSpriteList()
{
}

void CAniSpriteList::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_SpriteListCtrl);
}


BEGIN_MESSAGE_MAP(CAniSpriteList, CDialogEx)
	ON_WM_SETCURSOR()
	ON_WM_SHOWWINDOW()
	ON_LBN_SELCHANGE(IDC_LIST1, &CAniSpriteList::OnLbnSelchangeList1)
END_MESSAGE_MAP()


// CAniSpriteList �޽��� ó�����Դϴ�.


void CAniSpriteList::OnOK()
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	return;
}


BOOL CAniSpriteList::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	SetCursor(LoadCursor(NULL, IDC_ARROW));
	return TRUE;
}


void CAniSpriteList::OnShowWindow(BOOL bShow, UINT nStatus)
{
	
	if (FALSE == CEditGlobal::bEditInit) 
	{
		return;
	}
	
	if (TRUE == bShow) 
	{
		ShowListEvent();
	}


	CDialogEx::OnShowWindow(bShow, nStatus);

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
}
void CAniSpriteList::ShowListEvent()
{
	CEditGlobal::InitAnimationModel();

	m_SpriteListCtrl.ResetContent();
	ANISPRITETYPE Index = CEditGlobal::AnimationDlg->GetCurTypeIndex();
	UpdateAniSpriteList(Index);

}

BOOL CAniSpriteList::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	CEditGlobal::AniSpriteList = this;
	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}


void CAniSpriteList::OnLbnSelchangeList1()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	int Index = m_SpriteListCtrl.GetCurSel();

	if (m_iPrevIndex == Index)
	{
		return;
	}
	CString AniSpriteKey;
	m_SpriteListCtrl.GetText(Index, AniSpriteKey);
	CEditGlobal::AnimationDlg->AniSpriteSelectEvent(AniSpriteKey);

	m_iPrevIndex = Index;
	m_SpriteListCtrl.GetText(Index,m_LastSelectString);
}
void CAniSpriteList::UpdateAniSpriteList(ANISPRITETYPE _Type)
{
	m_iPrevIndex = -1;
	m_SpriteListCtrl.ResetContent();

	CEditGlobal::AniSpriteListUpdate(&m_SpriteListCtrl, _Type);
	

	m_iLastSelectIndex = m_SpriteListCtrl.FindString(0, m_LastSelectString);
	if (-1 == m_iLastSelectIndex) 
	{
		return;
	}
	
	m_SpriteListCtrl.SetCurSel(m_iLastSelectIndex);
	
	CEditGlobal::MBaseAniCom->ChangeAni(m_LastSelectString.GetString());
	CEditGlobal::MBaseAniModel->On();

}
void CAniSpriteList::ChangeSprinteType()
{
	m_LastSelectString = _T("");
	m_iPrevIndex = -1;
}

BOOL CAniSpriteList::PreTranslateMessage(MSG* pMsg)
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
