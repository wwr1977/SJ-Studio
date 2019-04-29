// MultiSpriteDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "FREditor.h"
#include "MultiSpriteDlg.h"
#include "afxdialogex.h"
#include <GameSprite.h>
#include <GameMultiSprite.h>



// CMultiSpriteDlg 대화 상자입니다.
//
//void LoadingTexture(FolderData* _FolderData)
//{
//	tstring DirPath = CFilePathMgr::GetPath(_FolderData->FolderKey);
//	CEditGlobal::LoadingImage(DirPath, _FolderData);
//}



IMPLEMENT_DYNAMIC(CMultiSpriteDlg, CDialogEx)

CMultiSpriteDlg::CMultiSpriteDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_MULTISPRITE, pParent)
	, m_fStartPosX(0.0f)
	, m_fStartPosY(0.0f)
	, m_fSizeX(0.0f)
	, m_fSizeY(0.0f)
	, m_iRow(0)
	, m_iColm(0)
	, m_iFrameCount(0)
	, m_fPivotX(0.0f)
	, m_fPivotY(0.0f)
	, m_PivotX(_T("0.0"))
	, m_PivotY(_T("0.0"))
	, m_iPrevIndex(-1)
	//, m_iLastSelectIndex(-1)
	, m_bSpriteMode(TRUE)
	, m_bAnimationMode(FALSE)
	, m_LastSelectString(_T(""))
	, m_iStartFrame(_T(""))
{

}

CMultiSpriteDlg::~CMultiSpriteDlg()
{
}

void CMultiSpriteDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_FolderCtrl);
	DDX_Control(pDX, IDC_LIST1, m_ImageList);
	DDX_Text(pDX, IDC_STARTPOSX, m_fStartPosX);
	DDX_Text(pDX, IDC_STARTPOSY, m_fStartPosY);
	DDX_Text(pDX, IDC_SIZEX, m_fSizeX);
	DDX_Text(pDX, IDC_SIZEY, m_fSizeY);
	DDX_Text(pDX, IDC_ROW, m_iRow);
	DDX_Text(pDX, IDC_COLM, m_iColm);
	DDX_Text(pDX, IDC_FRAMECOUNT, m_iFrameCount);
	DDX_Text(pDX, IDC_PIVOTX, m_PivotX);
	DDX_Text(pDX, IDC_PIVOTY, m_PivotY);
	DDX_Control(pDX, IDC_SAVESPRITELIST, m_SaveSpriteKey);
	DDX_Control(pDX, IDC_SPRITEKEY2, m_AddSpriteName);
	DDX_Control(pDX, IDC_SPRITEKEY1, m_SpriteMainName);
	DDX_Check(pDX, IDC_SPRITECHECK, m_bSpriteMode);
	DDX_Check(pDX, IDC_ANICHECK, m_bAnimationMode);
	DDX_Control(pDX, IDC_SIZEX, m_SizeXCtrl);
	DDX_Control(pDX, IDC_SIZEY, m_SizeYCtrl);
	DDX_Control(pDX, IDC_ROW, m_RowCtrl);
	DDX_Control(pDX, IDC_COLM, m_ColmCtrl);
	DDX_Control(pDX, IDC_FRAMECOUNT, m_FrameCtrl);
	DDX_Control(pDX, IDC_STARTPOSX, m_StartPosXCtrl);
	DDX_Control(pDX, IDC_STARTPOSY, m_StartPosYCtrl);
	DDX_Control(pDX, IDC_PIVOTX, m_PivotXCtrl);
	DDX_Control(pDX, IDC_PIVOTY, m_PivotYCtrl);
	DDX_Control(pDX, IDC_STARTFRAME, m_StartFrameCtrl);
	DDX_Text(pDX, IDC_STARTFRAME, m_iStartFrame);
}


BEGIN_MESSAGE_MAP(CMultiSpriteDlg, CDialogEx)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CMultiSpriteDlg::OnCbnSelchangeCombo1)
	ON_LBN_SELCHANGE(IDC_LIST1, &CMultiSpriteDlg::OnLbnSelchangeList1)
	ON_BN_CLICKED(IDC_CREATEBUTTON, &CMultiSpriteDlg::OnBnClickedCreatebutton)
	ON_CBN_SELCHANGE(IDC_SPRITEKEY2, &CMultiSpriteDlg::OnCbnSelchangeSpritekey2)
	ON_BN_CLICKED(IDC_SPRITECHECK, &CMultiSpriteDlg::OnBnClickedSpritecheck)
	ON_BN_CLICKED(IDC_ANICHECK, &CMultiSpriteDlg::OnBnClickedAnicheck)
	ON_WM_SETCURSOR()
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


// CMultiSpriteDlg 메시지 처리기입니다.

void CMultiSpriteDlg::OnCbnSelchangeCombo1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	//int Index = m_FolderCtrl.GetCurSel();
	//if (Index >= FOLDERMAX) 
	//{
	//	return;
	//}
	//CString SelectForder;
	//m_FolderCtrl.GetLBText(Index, SelectForder);
	//tstring DirPath = CFilePathMgr::GetPath(SelectForder.GetString());

	//m_iPrevIndex = -1;
	//m_LastSelectString = _T("");

	//// 쓰레드를 이용하여 로딩을 해야할때
	//if (FALSE == CEditGlobal::vecFolderData[Index].CheckLoadingStart) 
	//{
	//	
	//	m_ImageList.ResetContent();
	//	
	//	// 사용가능한 쓰레드가 존재할때
	//	if (TRUE == CEditGlobal::CheckUseThread()) 
	//	{
	//		CThreadMgr::CreateGlobvalThread(LoadingTexture, &CEditGlobal::vecFolderData[Index]);
	//		CEditGlobal::vecFolderData[Index].CheckLoadingStart = TRUE;
	//		CString FolderKey = CEditGlobal::vecFolderData[Index].FolderKey.c_str();
	//		m_ImageList.AddString(_T("             ") + FolderKey + _T("  Loading  Start!"));	
	//	}
	//	// 사용가능한 쓰레드가 없을때
	//	else 
	//	{
	//		m_ImageList.AddString(_T("                       ...Waiting..."));
	//	}

	//	return;
	//}
	//
	//// 로딩이 완전히 종료되었을때
	//if (TRUE == CEditGlobal::vecFolderData[Index].CheckFinish)
	//{
	//	//UpdateImageList(DirPath);
	//	CEditGlobal::TexSpriteListUpdate(&m_ImageList, (FOLDER)Index);
	//}
	//// 로딩이 완전히 끝나지 않을때
	//else 
	//{
	//	m_ImageList.ResetContent();
	//	m_ImageList.AddString(_T("                          Loading....\t\t"));
	//}


}


BOOL CMultiSpriteDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	m_FolderCtrl.AddString(_T("Character"));
	m_FolderCtrl.AddString(_T("Monster"));
	m_FolderCtrl.AddString(_T("Effect"));
	m_FolderCtrl.AddString(_T("UI"));

	m_AddSpriteName.AddString(_T("Idle"));
	m_AddSpriteName.AddString(_T("Attack"));
	m_AddSpriteName.AddString(_T("LimitAttack"));
	m_AddSpriteName.AddString(_T("Win"));
	m_AddSpriteName.AddString(_T("Winbefore"));
	m_AddSpriteName.AddString(_T("Dying"));
	m_AddSpriteName.AddString(_T("Dead"));



	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CMultiSpriteDlg::OnLbnSelchangeList1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	int Index = m_ImageList.GetCurSel();
	
	if (m_iPrevIndex == Index) 
	{
		return;
	}
	CString ImageKey;
	m_ImageList.GetText(Index, ImageKey);
	// 리스트 목록에 누른 이미지 이름은 Texture전체 를 의미하는 _T를 붙어서 저장
	if (TRUE == CEditGlobal::EditResourceMgr->ExistGameSprite((_T("_Tex")+ImageKey).GetString())) 
	{
		CEditGlobal::EditMainCamera->SetZoomRatio(1.0f);
		CEditGlobal::InitAniSpriteModel();
		CEditGlobal::MTexCom->SetSprite((_T("_Tex") + ImageKey).GetString());
		Vec2 ImageSize = CEditGlobal::MTexCom->GetCurSpriteSize();
		CEditGlobal::MTexCom->SyncSpriteSize();
		CEditGlobal::MTexModel->On();
		UpdateData(TRUE);
		GetDlgItem(IDC_SPRITEKEY1)->SetWindowTextW(ImageKey);
		GetDlgItem(IDC_SPRITEKEY2)->SetWindowTextW(_T(""));
		m_SaveSpriteKey.ResetContent();
		m_SaveSpriteKey.AddString(ImageKey);
		InitEditBox(ImageSize);
		UpdateData(FALSE);
	}

	m_iPrevIndex = Index;
	m_LastSelectString = ImageKey;
}




void CMultiSpriteDlg::OnBnClickedCreatebutton()
{
	 //TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	return;
}

void CMultiSpriteDlg::OnOK()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	return;
}

BOOL CMultiSpriteDlg::SettingSpriteValue()
{
	CString TexSpriteKey;
	//GetDlgItemText(IDC_SPRITEKEY1, TexSpriteKey);
	int Index = m_ImageList.GetCurSel();
	if (-1 == Index) 
	{
		return FALSE;
	}
	m_ImageList.GetText(Index, TexSpriteKey);
	SPTR<CGameSprite> Tex = CEditGlobal::EditResourceMgr->FindGameSprite((_T("_Tex")+TexSpriteKey).GetString());

	if (nullptr == Tex) 
	{
		return FALSE;
	}

	m_fStartPosX = (float)GetDlgItemInt(IDC_STARTPOSX);
	if (FALSE == CEditGlobal::AvailableValue(m_fStartPosX, Tex->GetSpriteSize().x, _T("StartPos의 X가 적절치 않습니다.")))
	{
		GetDlgItem(IDC_STARTPOSX)->SetFocus();
		m_StartPosXCtrl.SetSel(0, -1);
		return FALSE;
	}

	m_fStartPosY = (float)GetDlgItemInt(IDC_STARTPOSY);
	if (FALSE == CEditGlobal::AvailableValue(m_fStartPosY, Tex->GetSpriteSize().x, _T("StartPos의 Y가 적절치 않습니다.")))
	{
		GetDlgItem(IDC_STARTPOSY)->SetFocus();
		m_StartPosYCtrl.SetSel(0, -1);
		return FALSE;
	}

	m_fSizeX = (float)GetDlgItemInt(IDC_SIZEX);
	if (FALSE == CEditGlobal::AvailableValue(m_fSizeX,Tex->GetSpriteSize().x - m_fStartPosX, _T("Size의 X가 적절치 않습니다.")))
	{
		GetDlgItem(IDC_SIZEX)->SetFocus();
		m_SizeXCtrl.SetSel(0, -1);
		return FALSE;
	}
	m_fSizeY = (float)GetDlgItemInt(IDC_SIZEY);
	if (FALSE == CEditGlobal::AvailableValue(m_fSizeY, Tex->GetSpriteSize().y - m_fStartPosY, _T("Size의 Y가 적절치 않습니다.")))
	{
		GetDlgItem(IDC_SIZEY)->SetFocus();
		m_SizeYCtrl.SetSel(0, -1);
		return FALSE;
	}

	m_iRow = GetDlgItemInt(IDC_ROW);
	if (FALSE == CEditGlobal::AvailableValue(m_iRow, _T("행의 값이 적절치 않습니다.")))
	{
		GetDlgItem(IDC_ROW)->SetFocus();
		m_RowCtrl.SetSel(0, -1);
		return FALSE;
	}
	m_iColm = GetDlgItemInt(IDC_COLM);
	if (FALSE == CEditGlobal::AvailableValue(m_iColm, _T("열의 값이 적절치 않습니다.")))
	{
		GetDlgItem(IDC_COLM)->SetFocus();
		m_ColmCtrl.SetSel(0, -1);
		return FALSE;
	}
	m_iFrameCount = GetDlgItemInt(IDC_FRAMECOUNT);
	if (FALSE == CEditGlobal::AvailableValue(m_iFrameCount,m_iRow * m_iColm ,_T("프레임 값이 적절치 않습니다.")))
	{
		GetDlgItem(IDC_FRAMECOUNT)->SetFocus();
		m_FrameCtrl.SetSel(0, -1);
		return FALSE;
	}

	if (m_PivotX == _T("-") || m_PivotX == _T(".")) 
	{
		m_PivotX == _T("0.0");
	}
	if (m_PivotY == _T("-") || m_PivotY == _T("."))
	{
		m_PivotY == _T("0.0");
	}

	m_fPivotX = CEditGlobal::ChangeFloatValue(m_PivotX);
	m_fPivotY = CEditGlobal::ChangeFloatValue(m_PivotY);
	
	return TRUE;
}

BOOL CMultiSpriteDlg::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	 //TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (FALSE == CEditGlobal::bEditInit) 
	{
		return CDialogEx::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
	}
	
	if (nID == IDC_SPRITEKEY1 || nID == IDC_SPRITEKEY2) 
	{
		UpdateData(TRUE);
		m_SaveSpriteKey.ResetContent();
		CString First, Second;
		GetDlgItemText(IDC_SPRITEKEY1,First);
		GetDlgItemText(IDC_SPRITEKEY2, Second);
		if (Second == _T("")) 
		{
			m_SaveSpriteKey.AddString(First);
		}
		else
		{
			m_SaveSpriteKey.AddString(First + Second);
		}
		UpdateData(FALSE);
	}
	if (nID == IDC_ROW || nID == IDC_COLM) 
	{
		int Row, Colm;
		UpdateData(TRUE);
		Row = GetDlgItemInt(IDC_ROW);
		Colm = GetDlgItemInt(IDC_COLM);
		m_iFrameCount = Row * Colm;
		UpdateData(FALSE);
	}
	if (nID == IDC_PIVOTX) 
	{
		int Sel = m_PivotXCtrl.GetSel();
		UpdateData(TRUE);
		CEditGlobal::ChangeFloatString(m_PivotX);
		UpdateData(FALSE);
		m_PivotXCtrl.SetSel(Sel);
	}
	if (nID == IDC_PIVOTY) 
	{
		int Sel = m_PivotYCtrl.GetSel();
		UpdateData(TRUE);
		CEditGlobal::ChangeFloatString(m_PivotY);
		UpdateData(FALSE);
		m_PivotYCtrl.SetSel(Sel);
	}
	return CDialogEx::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}


void CMultiSpriteDlg::OnCbnSelchangeSpritekey2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString First, Second;
	int Index = m_AddSpriteName.GetCurSel();
	m_AddSpriteName.GetLBText(Index, Second);

	UpdateData(TRUE);
	m_SaveSpriteKey.ResetContent();
	GetDlgItemText(IDC_SPRITEKEY1, First);
	if (Second == _T(""))
	{
		m_SaveSpriteKey.AddString(First);
	}
	else 
	{
		m_SaveSpriteKey.AddString(First + Second);
	}
	UpdateData(FALSE);
}


BOOL CMultiSpriteDlg::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	SetCursor(LoadCursor(NULL,IDC_ARROW));
	return TRUE;
	
}


void CMultiSpriteDlg::OnBnClickedSpritecheck()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// 만일 직전에 눌린 버튼이라면 이값은 FALSE(역이 적용된)을 출력
	BOOL ChangeAni = IsDlgButtonChecked(IDC_SPRITECHECK);
	UpdateData(TRUE);
	m_bSpriteMode = TRUE;
	m_bAnimationMode = FALSE;
	CheckDlgButton(IDC_SPRITECHECK, m_bSpriteMode);
	CheckDlgButton(IDC_ANICHECK, m_bAnimationMode);
	UpdateData(FALSE);
	//눌려졌던 버튼인경우 이 값이 FALSE이므로  이떄는 모델은 바꾸지않는다.
	if (TRUE == ChangeAni)
	{
		ChangeAniSpriteModel(m_LastSelectString);
	}
}


void CMultiSpriteDlg::OnBnClickedAnicheck()
{

	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	BOOL ChangeAni = IsDlgButtonChecked(IDC_ANICHECK);
	UpdateData(TRUE);
	m_bSpriteMode = FALSE;
	m_bAnimationMode = TRUE;
	CheckDlgButton(IDC_SPRITECHECK, m_bSpriteMode);
	CheckDlgButton(IDC_ANICHECK, m_bAnimationMode);
	UpdateData(FALSE);
	if (TRUE == ChangeAni)
	{
		ChangeAniSpriteModel(m_LastSelectString);
	}
}

void CMultiSpriteDlg::InitEditBox(const Vec2& _SpriteSize)
{
	UpdateData(TRUE);
	m_fStartPosX = 0.0f;
	m_fStartPosY = 0.0f;
	m_fSizeX = _SpriteSize.x;
	m_fSizeY = _SpriteSize.y;
	m_iRow = 1;
	m_iColm = 1;
	m_iFrameCount = 1;
	m_fPivotX = 0.0f;
	m_fPivotY = 0.0f;
	m_PivotX = _T("0.0");
	m_PivotY = _T("0.0");
	UpdateData(FALSE);
}

BOOL CMultiSpriteDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN) 
	{
		if (GetFocus()->m_hWnd == GetDlgItem(IDC_CREATEBUTTON)->m_hWnd)
		{
			CreateButtonEvent();
			
		}
		if (GetFocus()->m_hWnd == GetDlgItem(IDC_SAVEBUTTON)->m_hWnd)
		{
			SaveButtonEvent();
		}
		
	}
	
	if (pMsg->message == WM_KEYDOWN && pMsg->hwnd == GetDlgItem(IDC_LIST1)->m_hWnd)
	{
		
		// List가 포커싱되었을때 A~Z키가 입력되었으면 해당 키의 Event 무시
		if (0x41 <= pMsg->wParam && 0x5A >= pMsg->wParam) 
		{
			return TRUE;
		}
		
	}


	if (pMsg->message == WM_LBUTTONDOWN)
	{
		if (pMsg->hwnd== GetDlgItem(IDC_CREATEBUTTON)->m_hWnd)
		{
			CreateButtonEvent();
			return TRUE;
		}
		if (pMsg->hwnd == GetDlgItem(IDC_SAVEBUTTON)->m_hWnd)
		{
			SaveButtonEvent();
			return TRUE;
		}
		int nID = IDC_STARTPOSX;
		while (nID <= IDC_PIVOTY)
		{
			if (pMsg->hwnd == GetDlgItem(nID)->m_hWnd &&
				GetFocus()->m_hWnd != GetDlgItem(nID)->m_hWnd)
			{
				GetDlgItem(nID)->SetFocus();
				switch (nID)
				{
				case IDC_STARTPOSX: m_StartPosXCtrl.SetSel(0, -1, true); return TRUE;
				case IDC_STARTPOSY: m_StartPosYCtrl.SetSel(0, -1, true); return TRUE;
				case IDC_SIZEX:		m_SizeXCtrl.SetSel(0, -1, true); return TRUE;
				case IDC_SIZEY:		m_SizeYCtrl.SetSel(0, -1, true); return TRUE;
				case IDC_ROW:		m_RowCtrl.SetSel(0, -1, true); return TRUE;
				case IDC_COLM:		m_ColmCtrl.SetSel(0, -1, true); return TRUE;
				case IDC_FRAMECOUNT:m_FrameCtrl.SetSel(0, -1, true); return TRUE;
				case IDC_PIVOTX:	m_PivotXCtrl.SetSel(0, -1, true); return TRUE;
				case IDC_PIVOTY:	m_PivotYCtrl.SetSel(0, -1, true); return TRUE;
				default:
					return TRUE;
				}
			}
			else
			{
				++nID;
			}
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}
void CMultiSpriteDlg::CreateButtonEvent()
{
	
	if (FALSE == SettingSpriteValue())
	{
		return;
	}
	CEditGlobal::InitAniSpriteModel();

	CString	TexKey, SpriteKey;
	GetDlgItemText(IDC_SPRITEKEY1, TexKey);
	m_SaveSpriteKey.GetText(0, SpriteKey);

	CEditGlobal::EditResourceMgr->CreateEditGameSprite(
		TexKey.GetString()
		, SpriteKey.GetString()
		, { m_fStartPosX,m_fStartPosY }
	, { m_fSizeX,m_fSizeY });

	CGameMultiSprite* NewAniSprite = CEditGlobal::EditResourceMgr->CreateEditMultiSprite(
		TexKey.GetString()
		, SpriteKey.GetString()
		, { m_fStartPosX,m_fStartPosY }
		, { m_fSizeX,m_fSizeY }
		, m_iFrameCount
		, m_iRow
		, m_iColm
		, { m_fPivotX,m_fPivotY });

	NewAniSprite->SetSpriteType((ANISPRITETYPE)m_FolderCtrl.GetCurSel());


	CEditGlobal::MAniSpriteCom->SetSprite(SpriteKey.GetString());
	

	ANIDATA AniData(SpriteKey.GetString(), SpriteKey.GetString());
	AniData.SettingFullRage({ 0,m_iFrameCount - 1 });

	CEditGlobal::MBaseAniCom->CreateEditAnimation(AniData);
	CEditGlobal::MBaseAniCom->ChangeAni(SpriteKey.GetString());
	

	ChangeAniSpriteModel(SpriteKey);

	GetDlgItem(IDC_LIST1)->SetFocus();
}
void CMultiSpriteDlg::ChangeAniSpriteModel(const CString& _ChangeKey)
{
	CEditGlobal::InitAniSpriteModel();


	if (_T("") == _ChangeKey)
	{
		return;
	}

	if (TRUE == m_bSpriteMode) 
	{
		if (TRUE == CEditGlobal::EditResourceMgr->ExistGameMultiSprite(_ChangeKey.GetString()))
		{
			CEditGlobal::MAniSpriteCom->SetSprite(_ChangeKey.GetString());
			CEditGlobal::MAniSpriteModel->On();
			CEditGlobal::MAniSpriteCom->SyncSpriteSize();
			return;
		}
	}
	if (TRUE == m_bAnimationMode) 
	{
		if (TRUE == CEditGlobal::MBaseAniCom->ExistAnimation(_ChangeKey.GetString()))
		{
			CEditGlobal::MBaseAniCom->ChangeAni(_ChangeKey.GetString());
			CEditGlobal::MBaseAniModel->On();
			CEditGlobal::MBaseAniCom->CurAniReset();
			return;
		}
	}
	

}

void CMultiSpriteDlg::SaveButtonEvent()
{
	ANISPRITETYPE GroupIndex  = (ANISPRITETYPE)m_FolderCtrl.GetCurSel();

	if (MAXANISPRITETYPE <= GroupIndex)
	{
		return;
	}

	vector<MULTISPRITEDATA> VecSaveDataGroup;

	CEditGlobal::EditResourceMgr->GetMultiSpriteData(&VecSaveDataGroup, GroupIndex);
	
	
	CGameFile SaveFile = CGameFile(_T("Data"), CGameFile::vecSpriteDataFileName[(int)GroupIndex]);

	if (FALSE == SaveFile.Open(_T("wb"))) 
	{
		return;
	}

	
	for (size_t i = 0; i < VecSaveDataGroup.size(); i++)
	{
		fwrite(&VecSaveDataGroup[i], sizeof(VecSaveDataGroup[i]), 1, SaveFile.GetFile());
	}

	return;
}





void CMultiSpriteDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	
	if (FALSE == CEditGlobal::bEditInit) 
	{
		return;
	}
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if (FALSE == bShow) 
	{
		CEditGlobal::InitAniSpriteModel();
		m_iPrevIndex = -1;
		return;
	}
	if (TRUE == bShow)
	{
		ChangeAniSpriteModel(m_LastSelectString);
	}

	CDialogEx::OnShowWindow(bShow, nStatus);
}
