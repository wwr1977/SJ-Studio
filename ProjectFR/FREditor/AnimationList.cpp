// AnimationList.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "FREditor.h"
#include "AnimationList.h"
#include "afxdialogex.h"
#include"AnimationDlg.h"


// CAnimationList ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CAnimationList, CDialogEx)

CAnimationList::CAnimationList(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_ANIMATIONLIST, pParent)
	, m_iPrevIndex(-1)
	, m_iLastSelectIndex(-1)
{

}

CAnimationList::~CAnimationList()
{
}

void CAnimationList::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_AniListCtrl);
}


BEGIN_MESSAGE_MAP(CAnimationList, CDialogEx)
	ON_WM_SETCURSOR()
	ON_WM_SHOWWINDOW()
	ON_LBN_SELCHANGE(IDC_LIST1, &CAnimationList::OnLbnSelchangeList1)
END_MESSAGE_MAP()


// CAnimationList �޽��� ó�����Դϴ�.


void CAnimationList::OnOK()
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	return;
}


BOOL CAnimationList::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	SetCursor(LoadCursor(NULL, IDC_ARROW));
	return TRUE;
}

void CAnimationList::ShowListEvent()
{
	CEditGlobal::InitAniSpriteModel();
	m_AniListCtrl.ResetContent();
	ANISPRITETYPE Index = CEditGlobal::AnimationDlg->GetCurTypeIndex();
	
	UpdateAnimationList(Index);
	
}

void CAnimationList::OnShowWindow(BOOL bShow, UINT nStatus)
{
	if (TRUE == bShow) 
	{
		ShowListEvent();
	}

	CDialogEx::OnShowWindow(bShow, nStatus);

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
}


BOOL CAnimationList::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	CEditGlobal::AnimationList = this;
	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}


void CAnimationList::OnLbnSelchangeList1()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	int Index = m_AniListCtrl.GetCurSel();

	if (m_iPrevIndex == Index)
	{
		return;
	}
	CString AnimationKey;
	m_AniListCtrl.GetText(Index, AnimationKey);
	CEditGlobal::AnimationDlg->AnimationSelectEvent(AnimationKey);

	m_iPrevIndex = Index;
	m_LastSelectString = AnimationKey;
}
void CAnimationList::UpdateAnimationList(ANISPRITETYPE _Type)
{
	m_iPrevIndex = -1;
	m_AniListCtrl.ResetContent();

	CEditGlobal::AnimationListUpdate(&m_AniListCtrl,CEditGlobal::AAnimationCom, _Type);


	m_iLastSelectIndex = m_AniListCtrl.FindString(0, m_LastSelectString);
	
	if (-1 == m_iLastSelectIndex)
	{
		return;
	}

	m_AniListCtrl.SetCurSel(m_iLastSelectIndex);

	CEditGlobal::AAnimationCom->ChangeAni(m_LastSelectString.GetString());
	CEditGlobal::AAnimationModel->On();
	CEditGlobal::AnimationDlg->InitAnimationData(m_LastSelectString);
}

BOOL CAnimationList::PreTranslateMessage(MSG* pMsg)
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
CString CAnimationList::GetCurSelectString()
{
	int  SelectIndex = m_AniListCtrl.GetCurSel();

	if (-1 == SelectIndex) 
	{
		return _T("");
	}
	CString SelectString;
	m_AniListCtrl.GetText(SelectIndex, SelectString);
	return SelectString;
}
BOOL CAnimationList::EraseString(const CString& _EraseString /*= _T("")*/)
{
	//����Ʈ�۵��� ���� ���õ� ���ڿ��� ����
	if (_T("") == _EraseString)
	{
		int CurSel = m_AniListCtrl.GetCurSel();
		m_AniListCtrl.DeleteString(CurSel);
		return TRUE;
	}

	int EraseIndex = m_AniListCtrl.FindString(0, _EraseString);

	if (-1 != EraseIndex)
	{
		m_AniListCtrl.DeleteString(EraseIndex);
		return TRUE;
	}

	return FALSE;
}