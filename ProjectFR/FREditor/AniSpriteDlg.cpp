// AniSpriteDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "FREditor.h"
#include "AniSpriteDlg.h"
#include "afxdialogex.h"
#include "TextureListDlg.h"
#include "MultiSpriteListDlg.h"
#include <GameMultiSprite.h>



// CAniSpriteDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CAniSpriteDlg, CDialogEx)

CAniSpriteDlg::CAniSpriteDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_NEWMULTISPRITE, pParent)
	, m_SelectTexName(_T(""))
	, m_SpriteName(_T(""))
	, m_fStartPosX(0.0f)
	, m_fStartPosY(0.0f)
	, m_fSizeX(0.0f)
	, m_fSizeY(0.0f)
	, m_iRow(0)
	, m_iColm(0)
	, m_iStartFrame(0)
	, m_iFrameCount(0)
	, m_PivotX(_T("0.0"))
	, m_PivotY(_T("0.0"))
	, m_fPivotX(0.0f)
	, m_fPivotY(0.0f)
	, m_bAnimationMode(TRUE)
	, m_bDlgOn(FALSE)
	, m_iTempFrameCount(0)
{

}

CAniSpriteDlg::~CAniSpriteDlg()
{
}

void CAniSpriteDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_FOLDERCOM, m_FolderComCtrl);
	DDX_Control(pDX, IDC_TEXTURELIST, m_TextureNameCtrl);
	DDX_LBString(pDX, IDC_TEXTURELIST, m_SelectTexName);
	DDX_Control(pDX, IDC_MULTISPRITENAME, m_SpriteNameCtrl);
	DDX_Text(pDX, IDC_MULTISPRITENAME, m_SpriteName);
	DDX_Control(pDX, IDC_NEWSTARTPOSX, m_StartPosXCtrl);
	DDX_Text(pDX, IDC_NEWSTARTPOSX, m_fStartPosX);
	DDX_Control(pDX, IDC_NEWSTARTPOSY, m_StartPosYCtrl);
	DDX_Text(pDX, IDC_NEWSTARTPOSY, m_fStartPosY);
	DDX_Control(pDX, IDC_NEWSIZEX, m_SizeXCtrl);
	DDX_Text(pDX, IDC_NEWSIZEX, m_fSizeX);
	DDX_Control(pDX, IDC_NEWSIZEY, m_SizeYCtrl);
	DDX_Text(pDX, IDC_NEWSIZEY, m_fSizeY);
	DDX_Control(pDX, IDC_XCOUNT, m_RowCtrl);
	DDX_Text(pDX, IDC_XCOUNT, m_iRow);
	DDX_Control(pDX, IDC_YCOUNT, m_ColmCtrl);
	DDX_Text(pDX, IDC_YCOUNT, m_iColm);
	DDX_Control(pDX, IDC_STARTFRAME, m_StartFrameCtrl);
	DDX_Text(pDX, IDC_STARTFRAME, m_iStartFrame);
	DDX_Control(pDX, IDC_NEWFRAMECOUNT, m_FrameCountCtrl);
	DDX_Text(pDX, IDC_NEWFRAMECOUNT, m_iFrameCount);
	DDX_Control(pDX, IDC_NEWPIVOTX, m_PivotXCtrl);
	DDX_Text(pDX, IDC_NEWPIVOTX, m_PivotX);
	DDX_Control(pDX, IDC_NEWPIVOTY, m_PivotYCtrl);
	DDX_Text(pDX, IDC_NEWPIVOTY, m_PivotY);
	DDX_Control(pDX, IDC_MULTISPRITETAB, m_ListTabCtrl);
}


BEGIN_MESSAGE_MAP(CAniSpriteDlg, CDialogEx)
	ON_WM_SETCURSOR()
	ON_WM_DESTROY()
	ON_CBN_SELCHANGE(IDC_FOLDERCOM, &CAniSpriteDlg::OnCbnSelchangeFoldercom)
	ON_WM_SHOWWINDOW()
	ON_NOTIFY(TCN_SELCHANGE, IDC_MULTISPRITETAB, &CAniSpriteDlg::OnTcnSelchangeMultispritetab)
	ON_EN_CHANGE(IDC_STARTFRAME, &CAniSpriteDlg::OnEnChangeStartframe)
	ON_EN_CHANGE(IDC_XCOUNT, &CAniSpriteDlg::OnEnChangeXcount)
	ON_EN_CHANGE(IDC_YCOUNT, &CAniSpriteDlg::OnEnChangeYcount)
	ON_EN_CHANGE(IDC_NEWFRAMECOUNT, &CAniSpriteDlg::OnEnChangeNewframecount)
END_MESSAGE_MAP()

//////////////////////////////////////////////////			User Custom Function	////////////////////////////////////////

// MultiSpriteDlg�� Ȱ��ȭ �Ǿ������� ���� ���� ��ȯ
BOOL CAniSpriteDlg::IsDlgOn()
{
	return m_bDlgOn;
}
// ���� �޺� �ڽ��� �̺�Ʈ�� �߻��Ǿ��� �� Texture List�� Update�ϴ� �Լ�
void CAniSpriteDlg::UpdateTextureList() 
{
	int Index = m_FolderComCtrl.GetCurSel();
	if (Index >= FOLDERMAX)
	{
		return;
	}

	CString SelectForder;
	m_FolderComCtrl.GetLBText(Index, SelectForder);
	tstring DirPath = CFilePathMgr::GetPath(SelectForder.GetString());

	// �����带 �̿��Ͽ� �ε��� �ؾ��Ҷ�
	if (FALSE == CEditGlobal::vecFolderData[Index].CheckLoadingStart)
	{
		// ��밡���� �����尡 �����Ҷ�
		if (TRUE == CEditGlobal::CheckUseThread())
		{
			CThreadMgr::CreateGlobvalThread(LoadingTexture, &CEditGlobal::vecFolderData[Index]);
			CEditGlobal::vecFolderData[Index].CheckLoadingStart = TRUE;
			CString FolderKey = CEditGlobal::vecFolderData[Index].FolderKey.c_str();
			CEditGlobal::TextureList->ResetList(_T("             ") + FolderKey + _T("  Loading  Start!"));
		}
		// ��밡���� �����尡 ������
		else
		{
			CEditGlobal::TextureList->ResetList(_T("                       ...Waiting..."));
		}

		return;
	}

	// �ε��� ������ ����Ǿ�����
	if (TRUE == CEditGlobal::vecFolderData[Index].CheckFinish)
	{
		CEditGlobal::TextureList->ResetList();
		CEditGlobal::TextureList->UpdateList((FOLDER)Index);
	}
	// �ε��� ������ ������ ������
	else
	{
		CEditGlobal::TextureList->ResetList(_T("                          Loading...."));
	}

}
// ���� �޺� �ڽ��� �̺�Ʈ�� �߻��Ǿ��� �� MultiSprite List�� Update�ϴ� �Լ�
void CAniSpriteDlg::UpdateMultiSpriteList()
{
	int Index = m_FolderComCtrl.GetCurSel();

	if (Index >= FOLDERMAX)
	{
		return;
	}

	if (ANI_ETC <= (ANISPRITETYPE)Index)
	{
		Index = ANI_ETC;
	}

	CEditGlobal::MultiSpriteList->UpdateList((ANISPRITETYPE)Index);

}

// ����Ʈ �ڽ��� ���ڿ��� ���õǾ����� �۵��ϴ� �Լ�
//Ŭ���� ���ڿ��� Ű������ ����ϴ� Texture Or MultiSprite ���� �������ϱ� ����
BOOL CAniSpriteDlg::SelectTextureList(const CString& _TexKey)
{
	
	if (TRUE == CEditGlobal::EditResourceMgr->ExistGameSprite((_T("_Tex") + _TexKey).GetString()))
	{
		CEditGlobal::EditMainCamera->SetZoomRatio(1.0f);
		CEditGlobal::InitAniSpriteModel();
		CEditGlobal::MTexCom->SetSprite((_T("_Tex") + _TexKey).GetString());
		Vec2 ImageSize = CEditGlobal::MTexCom->GetCurSpriteSize();
		CEditGlobal::MTexCom->SyncSpriteSize();
		CEditGlobal::MTexModel->On();
		UpdateData(TRUE);
		m_TextureNameCtrl.ResetContent();
		m_TextureNameCtrl.AddString(_TexKey);
		m_SpriteName = _TexKey;
		InitTexData(ImageSize);
		UpdateData(FALSE);
		return TRUE;
	}

	return FALSE;
}
void CAniSpriteDlg::InitTexData(const Vec2& _TexSize)
{
	m_fStartPosX = 0.0f;
	m_fStartPosY = 0.0f;
	m_fSizeX = _TexSize.x;
	m_fSizeY = _TexSize.y;
	m_iRow = 1;
	m_iColm = 1;
	m_iStartFrame = 0;
	m_iFrameCount = 1;
	m_iTempFrameCount = 1;
	m_fPivotX = 0.0f;
	m_fPivotY = 0.0f;
	m_PivotX = _T("0.0");
	m_PivotY = _T("0.0");
}
BOOL CAniSpriteDlg::SelectMultiSpriteList(const CString& _MultiSpriteKey)
{
	CEditGlobal::InitAniSpriteModel();

	if (FALSE == InitSpriteData(_MultiSpriteKey) )
	{
		return FALSE;
	}
	
	
	if (FALSE == m_bAnimationMode)
	{
		if (TRUE == CEditGlobal::EditResourceMgr->ExistGameMultiSprite(_MultiSpriteKey.GetString()))
		{
			CEditGlobal::MAniSpriteCom->SetSprite(_MultiSpriteKey.GetString());
			CEditGlobal::MAniSpriteModel->On();
			CEditGlobal::MAniSpriteCom->SyncSpriteSize();
			return TRUE;
		}

	}
	else
	{
		if (TRUE == CEditGlobal::MBaseAniCom->ExistAnimation(_MultiSpriteKey.GetString()))
		{
			CEditGlobal::MBaseAniCom->ChangeAni(_MultiSpriteKey.GetString());
			CEditGlobal::MBaseAniModel->On();
			CEditGlobal::MBaseAniCom->CurAniReset();
			return TRUE;
		}
	}

	return FALSE;
}
BOOL CAniSpriteDlg::InitSpriteData(const CString& _SpriteKey /*= _T("")*/)
{
	SPTR<CGameMultiSprite> MultiSprite = CEditGlobal::EditResourceMgr->FindMultiSprite(_SpriteKey.GetString());

	if (nullptr == MultiSprite)
	{
		UpdateData(TRUE);
		m_TextureNameCtrl.ResetContent();
		m_TextureNameCtrl.AddString(_T(""));
		m_SpriteName = _T("");
		m_fStartPosX = 0.0f;
		m_fStartPosY = 0.0f;
		m_fSizeX = 0.0f;
		m_fSizeY = 0.0f;
		m_iRow = 1;
		m_iColm = 1;
		m_iStartFrame = 0;
		m_iFrameCount = 1;
		m_fPivotX = 0.0f;
		m_fPivotY = 0.0f;
		m_PivotX = _T("0.0");
		m_PivotY = _T("0.0");
		UpdateData(FALSE);
		return FALSE;
	}
	else
	{
		UpdateData(TRUE);
		m_TextureNameCtrl.ResetContent();
		m_TextureNameCtrl.AddString(MultiSprite->GetTexKey().c_str());
		m_SpriteName = MultiSprite->Name_str();
		m_fStartPosX = MultiSprite->GetStartPos().x;
		m_fStartPosY = MultiSprite->GetStartPos().y;
		m_fSizeX = MultiSprite->GetMultiSpriteSize().x;
		m_fSizeY = MultiSprite->GetMultiSpriteSize().y;
		m_iRow = MultiSprite->GetXCount();
		m_iColm = MultiSprite->GetYCount();
		m_iStartFrame = MultiSprite->GetStartFrame();
		m_iFrameCount = MultiSprite->GetImageCount();
		m_fPivotX = MultiSprite->GetSpritePivot().x * CEditGlobal::MBaseAniModel->GetMagnification();
		m_fPivotY = MultiSprite->GetSpritePivot().y* CEditGlobal::MBaseAniModel->GetMagnification();
		m_PivotX.Format(_T("%.6f"), m_fPivotX);
		m_PivotY.Format(_T("%.6f"), m_fPivotY);
		UpdateData(FALSE);
		return TRUE;
	}
}

// Sence ���� ChangeAniMode Ű�� ���������� �۵��ϴ� �Լ�
// SpriteList�� Ȱ��ȭ �Ǿ������� �۵�
void CAniSpriteDlg::ChangeModeEvent()
{
	if (ANISPRITELIST != m_CurListIndex) 
	{
		return;
	}

	m_bAnimationMode = !m_bAnimationMode;

	CString MultiSpriteKey = CEditGlobal::MultiSpriteList->GetLastSelectString();

	if (_T("") == MultiSpriteKey)
	{
		return;
	}

	if (FALSE == m_bAnimationMode)
	{
		if (TRUE == CEditGlobal::EditResourceMgr->ExistGameMultiSprite(MultiSpriteKey.GetString()))
		{
			CEditGlobal::MBaseAniModel->Off();
			CEditGlobal::MAniSpriteCom->SetSprite(MultiSpriteKey.GetString());
			CEditGlobal::MAniSpriteModel->On();
			CEditGlobal::MAniSpriteCom->SyncSpriteSize();
			return;
		}
	}
	else
	{
		if (TRUE == CEditGlobal::MBaseAniCom->ExistAnimation(MultiSpriteKey.GetString()))
		{
			CEditGlobal::MAniSpriteModel->Off();
			CEditGlobal::MBaseAniCom->ChangeAni(MultiSpriteKey.GetString());
			CEditGlobal::MBaseAniModel->On();
			CEditGlobal::MBaseAniCom->CurAniReset();
			return;
		}
	}
}
// ��Ƽ ��������Ʈ�� ��������� EditBox�� ���� �˻� & ���� �ϴ� �Լ�
BOOL CAniSpriteDlg::SettingSpriteValue()
{
	if (m_TextureNameCtrl.GetCount() <= 0) 
	{
		return FALSE;
	}

	m_TextureNameCtrl.GetText(0, m_SelectTexName);
	

	SPTR<CGameSprite> Tex = CEditGlobal::EditResourceMgr->FindGameSprite((_T("_Tex") + m_SelectTexName).GetString());

	if (nullptr == Tex)
	{
		return FALSE;
	}

	m_fStartPosX = (float)GetDlgItemInt(IDC_NEWSTARTPOSX);
	if (FALSE == CEditGlobal::AvailableValue(m_fStartPosX, Tex->GetSpriteSize().x, _T("StartPos�� X�� ����ġ �ʽ��ϴ�.")))
	{
		GetDlgItem(IDC_NEWSTARTPOSX)->SetFocus();
		m_StartPosXCtrl.SetSel(0, -1);
		return FALSE;
	}

	m_fStartPosY = (float)GetDlgItemInt(IDC_NEWSTARTPOSY);
	if (FALSE == CEditGlobal::AvailableValue(m_fStartPosY, Tex->GetSpriteSize().x, _T("StartPos�� Y�� ����ġ �ʽ��ϴ�.")))
	{
		GetDlgItem(IDC_NEWSTARTPOSY)->SetFocus();
		m_StartPosYCtrl.SetSel(0, -1);
		return FALSE;
	}

	m_fSizeX = (float)GetDlgItemInt(IDC_NEWSIZEX);
	if (FALSE == CEditGlobal::AvailableValue(m_fSizeX, Tex->GetSpriteSize().x - m_fStartPosX, _T("Size�� X�� ����ġ �ʽ��ϴ�.")))
	{
		GetDlgItem(IDC_NEWSIZEX)->SetFocus();
		m_SizeXCtrl.SetSel(0, -1);
		return FALSE;
	}
	m_fSizeY = (float)GetDlgItemInt(IDC_NEWSIZEY);
	if (FALSE == CEditGlobal::AvailableValue(m_fSizeY, Tex->GetSpriteSize().y - m_fStartPosY, _T("Size�� Y�� ����ġ �ʽ��ϴ�.")))
	{
		GetDlgItem(IDC_NEWSIZEY)->SetFocus();
		m_SizeYCtrl.SetSel(0, -1);
		return FALSE;
	}
	m_iRow = GetDlgItemInt(IDC_XCOUNT);
	if (FALSE == CEditGlobal::AvailableValue(m_iRow, _T("���� ���� ����ġ �ʽ��ϴ�.")))
	{
		GetDlgItem(IDC_XCOUNT)->SetFocus();
		m_RowCtrl.SetSel(0, -1);
		return FALSE;
	}
	m_iColm = GetDlgItemInt(IDC_YCOUNT);
	if (FALSE == CEditGlobal::AvailableValue(m_iColm, _T("���� ���� ����ġ �ʽ��ϴ�.")))
	{
		GetDlgItem(IDC_YCOUNT)->SetFocus();
		m_ColmCtrl.SetSel(0, -1);
		return FALSE;
	}
	m_iStartFrame = GetDlgItemInt(IDC_STARTFRAME);
	if (FALSE == CEditGlobal::AvailableValue(m_iStartFrame, m_iRow * m_iColm, _T("���� ������ ���� ����ġ �ʽ��ϴ�.")))
	{
		GetDlgItem(IDC_STARTFRAME)->SetFocus();
		m_StartFrameCtrl.SetSel(0, -1);
		return FALSE;
	}
	m_iFrameCount = GetDlgItemInt(IDC_NEWFRAMECOUNT);
	if (FALSE == CEditGlobal::AvailableValue(m_iFrameCount, m_iRow * m_iColm,_T("������ ���� ����ġ �ʽ��ϴ�.")))
	{
		GetDlgItem(IDC_NEWFRAMECOUNT)->SetFocus();
		m_FrameCountCtrl.SetSel(0, -1);
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

// ��ư �̺�Ʈ �Լ� (����,����,�ε�,����)
BOOL CAniSpriteDlg::CreateButtonEvent() 
{
	// Texture List�� Ȱ��ȭ �Ǿ������� �۵�
	/*if (TEXTURELIST != m_CurListIndex)
	{
		return FALSE;
	}*/
	
	if (FALSE == SettingSpriteValue())
	{
		return FALSE;
	}


	tstring TexKey = m_SelectTexName.GetString();
	tstring SpriteKey = m_SpriteName.GetString();

	CEditGlobal::EditResourceMgr->CreateEditGameSprite(
		TexKey
		, SpriteKey
		, { m_fStartPosX,m_fStartPosY }
	, { m_fSizeX,m_fSizeY });

	float	InvModelScale = 1.0f / CEditGlobal::MBaseAniModel->GetMagnification();

	CGameMultiSprite* NewAniSprite = CEditGlobal::EditResourceMgr->CreateEditMultiSprite(
		TexKey
		, SpriteKey
		, { m_fStartPosX,m_fStartPosY }
		, { m_fSizeX,m_fSizeY }
		, m_iFrameCount
		, m_iRow
		, m_iColm
		, { m_fPivotX*InvModelScale ,m_fPivotY *InvModelScale }
		, m_iStartFrame
	);

	NewAniSprite->SetSpriteType((ANISPRITETYPE)m_FolderComCtrl.GetCurSel());


	CEditGlobal::MAniSpriteCom->SetSprite(SpriteKey);


	ANIDATA AniData(SpriteKey, SpriteKey);
	AniData.SettingFullRage({ 0,m_iFrameCount - 1 });

	CEditGlobal::MBaseAniCom->CreateEditAnimation(AniData);
	CEditGlobal::MBaseAniCom->ChangeAni(SpriteKey);


	CEditGlobal::InitAniSpriteModel();

	// Pivot�� ���߱� ���� ������ �ִϸ��̼����� �������Ѵ�
	CEditGlobal::MBaseAniModel->On();

	return TRUE;
}
BOOL CAniSpriteDlg::DeleteButtonEvent() 
{
	// MultiSprite List�� Ȱ��ȭ �Ǿ������� �۵�
	if (ANISPRITELIST != m_CurListIndex)
	{
		return FALSE;
	}
	
	
	CString SelectString = CEditGlobal::MultiSpriteList->GetCurSelectString();

	// ���� ����Ʈ��Ͽ��� �ƹ��͵� ������ ������ ��� ����
	if (_T("") == SelectString) 
	{
		return FALSE;
	}
	CEditGlobal::MultiSpriteList->EraseString();

	// �������� ��Ƽ��������Ʈ�� ������ ���ҽ��Ŵ����� �����Ұ��
	// ��Ƽ��������Ʈ �ʿ��� ����
	if (TRUE != CEditGlobal::EditResourceMgr->ExistGameMultiSprite(SelectString.GetString())) 
	{
		return FALSE;
	}

	CEditGlobal::InitAniSpriteModel();
	CEditGlobal::EditResourceMgr->EraseMultiSprite(SelectString.GetString());
	CEditGlobal::EditResourceMgr->EraseGameSprite(SelectString.GetString());
	CEditGlobal::MBaseAniCom->EraseAnimation(SelectString.GetString());
	CEditGlobal::MAniSpriteCom->RenderOff();
	CEditGlobal::MBaseAniCom->RenderOff();

	return TRUE;
}
BOOL CAniSpriteDlg::DataLoadButtonEvent() 
{
	
	int LoadDataIndex = m_FolderComCtrl.GetCurSel();
	
	if (LoadDataIndex < 0) 
	{
		MessageBox(_T("Folder�� �����Ͻʽÿ�!"), _T("MultiSpriteDlg"), MB_ICONERROR);
		return FALSE;
	}
	
	if (ANI_ETC <= LoadDataIndex) 
	{
		LoadDataIndex = ANI_ETC;
	}
	
	if (FALSE == CEditGlobal::vecFolderData[LoadDataIndex].CheckFinish) 
	{
		CString ErrorMsg = CEditGlobal::vecFolderData[LoadDataIndex].FolderKey.c_str();
		ErrorMsg += _T(" ���� ���� �ؽ��ĸ� �ε��Ͻʽÿ�!");
		MessageBox(ErrorMsg, _T("MultiSpriteDlg"), MB_ICONWARNING);
		return FALSE;
	}
	
	CGameFile RFile = CGameFile(_T("EditData"), CGameFile::vecSpriteDataFileName[LoadDataIndex]);

	if (FALSE == RFile.Open(_T("rb")))
	{
		MessageBox(CEditGlobal::vecSaveDataFileName[LoadDataIndex] + _T("�� ���� ���Ͽ����ϴ�."), _T("MultiSpriteDlg"), MB_ICONERROR);
		return FALSE;
	}

	vector<MULTISPRITEDATA> vecRegData;
	vecRegData.reserve(128);

	if (FALSE == CEditGlobal::EditResourceMgr->LoadSpriteData(RFile.GetFile(), (ANISPRITETYPE)LoadDataIndex, &vecRegData))
	{
		MessageBox(CEditGlobal::vecSaveDataFileName[LoadDataIndex] + _T("�� ������ �ε忡 �����Ͽ����ϴ�."), _T("MultiSpriteDlg"), MB_ICONERROR);
		return FALSE;
	}
	
	for (size_t i = 0; i < vecRegData.size(); i++)
	{
		CEditGlobal::EditResourceMgr->CreateEditGameSprite(
			vecRegData[i].TexKey
			, vecRegData[i].MultiSpriteKey
			, { vecRegData[i].StartPosX,vecRegData[i].StartPosY }
			, { vecRegData[i].SizeX,vecRegData[i].SizeY }
		);


		ANIDATA AniData(vecRegData[i].MultiSpriteKey, vecRegData[i].MultiSpriteKey);
		AniData.SettingFullRage({ 0,vecRegData[i].ImageCount - 1 });

		CEditGlobal::MBaseAniCom->CreateEditAnimation(AniData);
	}

	vecRegData.clear();
	MessageBox(CEditGlobal::vecSaveDataFileName[LoadDataIndex] + _T("�� �����͸� ���������� �ε� �Ͽ����ϴ�."));
	return TRUE;
}
BOOL CAniSpriteDlg::DataSaveButtonEvent()
{
	if (ANISPRITELIST != m_CurListIndex)
	{
		return FALSE;
	}

	ANISPRITETYPE GroupIndex = (ANISPRITETYPE)m_FolderComCtrl.GetCurSel();

	if (ANI_ETC <= GroupIndex)
	{
		GroupIndex = ANI_ETC;
	}

	vector<MULTISPRITEDATA> VecSaveDataGroup;

	CEditGlobal::EditResourceMgr->GetMultiSpriteData(&VecSaveDataGroup, GroupIndex);

	if (0 >= VecSaveDataGroup.size()) 
	{
		MessageBox(CEditGlobal::vecSaveDataFileName[GroupIndex] +  _T("�� ������ ��Ƽ��������Ʈ�� �����ϴ�."), _T("MultiSpriteDlg"),MB_ICONWARNING);
		return FALSE;
	}

	CGameFile SaveFile = CGameFile(_T("EditData"), CGameFile::vecSpriteDataFileName[(int)GroupIndex]);

	if (FALSE == SaveFile.Open(_T("wb")))
	{
		MessageBox(CEditGlobal::vecSaveDataFileName[GroupIndex] + _T("���⿡ �����Ͽ����ϴ�."), _T("MultiSpriteDlg"), MB_ICONERROR);
		return FALSE;
	}


	for (size_t i = 0; i < VecSaveDataGroup.size(); i++)
	{
		fwrite(&VecSaveDataGroup[i], sizeof(VecSaveDataGroup[i]), 1, SaveFile.GetFile());
	}

	MessageBox(CEditGlobal::vecSaveDataFileName[GroupIndex] + _T("�� ���ο� �����Ͱ� ����Ǿ����ϴ�."), _T("MultiSpriteDlg"));

	return TRUE;
}


// CAniSpriteDlg �޽��� ó�����Դϴ�.
BOOL CAniSpriteDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	CEditGlobal::AniSpriteDlg = this;

	m_FolderComCtrl.AddString(_T("Character"));
	m_FolderComCtrl.AddString(_T("Monster"));
	m_FolderComCtrl.AddString(_T("Effect"));
	m_FolderComCtrl.AddString(_T("UI"));
	

	CreateTab<CTextureListDlg>(IDD_TEXTURELIST, _T("Texture"));
	CreateTab<CMultiSpriteListDlg>(IDD_MULTISPRITELIST, _T("Multi Sprite"));

	ShowDlg(TEXTURELIST);


	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}


BOOL CAniSpriteDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	{
		if (GetFocus()->m_hWnd == GetDlgItem(IDC_SPRITEREGISTERBUTTON)->m_hWnd)
		{
			CreateButtonEvent();
			return TRUE;
		}
		if (GetFocus()->m_hWnd == GetDlgItem(IDC_SPRITEDELETEBUTTON)->m_hWnd)
		{
			DeleteButtonEvent();
			return TRUE;
		}
		if (GetFocus()->m_hWnd == GetDlgItem(IDC_SPRITELOADBUTTON)->m_hWnd)
		{
			DataLoadButtonEvent();
			return TRUE;
		}
		if (GetFocus()->m_hWnd == GetDlgItem(IDC_SPRITESAVEBUTTON)->m_hWnd) 
		{
			DataSaveButtonEvent();
			return TRUE;
		}
	}

	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE) {
			// VK_RETURN  : Enter Key
			// VK_ESCAPE  : ESC key

			return TRUE;
		}
	}
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == 'Q') 
	{
		if (ANISPRITELIST == m_CurListIndex) 
		{
			m_bAnimationMode = !m_bAnimationMode;
		}
		return TRUE;
	}

	if (pMsg->message == WM_LBUTTONDOWN)
	{
		if (pMsg->hwnd == GetDlgItem(IDC_SPRITEREGISTERBUTTON)->m_hWnd)
		{
			CreateButtonEvent();
			return TRUE;
		}
		if (pMsg->hwnd == GetDlgItem(IDC_SPRITEDELETEBUTTON)->m_hWnd)
		{
			DeleteButtonEvent();
			return TRUE;
		}
		if (pMsg->hwnd == GetDlgItem(IDC_SPRITELOADBUTTON)->m_hWnd)
		{
			DataLoadButtonEvent();
			return TRUE;
		}
		if (pMsg->hwnd == GetDlgItem(IDC_SPRITESAVEBUTTON)->m_hWnd)
		{
			DataSaveButtonEvent();
			return TRUE;
		}

		int nID = IDC_STARTFRAME;
		while (nID <= IDC_NEWPIVOTX)
		{
			if (nID == 1050) 
			{
				++nID;
				continue;
			}
			if (pMsg->hwnd == GetDlgItem(nID)->m_hWnd &&
				GetFocus()->m_hWnd != GetDlgItem(nID)->m_hWnd)
			{
				GetDlgItem(nID)->SetFocus();

				switch (nID)
				{
				case IDC_NEWSTARTPOSX:	m_StartPosXCtrl.SetSel(0, -1, true); return TRUE;
				case IDC_NEWSTARTPOSY:	m_StartPosYCtrl.SetSel(0, -1, true); return TRUE;
				case IDC_NEWSIZEX:		m_SizeXCtrl.SetSel(0, -1, true); return TRUE;
				case IDC_NEWSIZEY:		m_SizeYCtrl.SetSel(0, -1, true); return TRUE;
				case IDC_XCOUNT:		m_RowCtrl.SetSel(0, -1, true); return TRUE;
				case IDC_YCOUNT:		m_ColmCtrl.SetSel(0, -1, true); return TRUE;
				case IDC_STARTFRAME:	m_StartFrameCtrl.SetSel(0, -1, true); return TRUE;
				case IDC_NEWFRAMECOUNT:	m_FrameCountCtrl.SetSel(0, -1, true); return TRUE;
				case IDC_NEWPIVOTX:		m_PivotXCtrl.SetSel(0, -1, true); return TRUE;
				case IDC_NEWPIVOTY:		m_PivotYCtrl.SetSel(0, -1, true); return TRUE;
				default:
					break;
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


BOOL CAniSpriteDlg::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.

	
	if (nID == IDC_NEWPIVOTX)
	{
		int Sel = m_PivotXCtrl.GetSel();
		UpdateData(TRUE);
		CEditGlobal::ChangeFloatString(m_PivotX);
		UpdateData(FALSE);
		m_PivotXCtrl.SetSel(Sel);
	}
	if (nID == IDC_NEWPIVOTY)
	{
		int Sel = m_PivotYCtrl.GetSel();
		UpdateData(TRUE);
		CEditGlobal::ChangeFloatString(m_PivotY);
		UpdateData(FALSE);
		m_PivotYCtrl.SetSel(Sel);
	}

	return CDialogEx::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}


BOOL CAniSpriteDlg::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	SetCursor(LoadCursor(NULL, IDC_ARROW));
	return TRUE;
}


void CAniSpriteDlg::OnDestroy()
{
	for (size_t i = 0; i < m_vecListDlg.size(); i++)
	{
		m_vecListDlg[i]->SendMessage(WM_DESTROY);
		SAFE_DELETE(m_vecListDlg[i]);
	}

	m_vecListDlg.clear();

	CDialogEx::OnDestroy();

	
}


void CAniSpriteDlg::OnCbnSelchangeFoldercom()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	switch (m_CurListIndex)
	{
	case CAniSpriteDlg::TEXTURELIST: 
		UpdateTextureList();
		break;
	case CAniSpriteDlg::ANISPRITELIST:
		UpdateMultiSpriteList();
		break;
	default:
		break;
	}

}


void CAniSpriteDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	if (FALSE == bShow)
	{
		m_bDlgOn = FALSE;	
		CEditGlobal::InitAniSpriteModel();
	}
	else
	{
		m_bDlgOn = TRUE;
	}

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	CDialogEx::OnShowWindow(bShow, nStatus);
}


void CAniSpriteDlg::OnTcnSelchangeMultispritetab(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	
	TABINDEX Index = (TABINDEX)m_ListTabCtrl.GetCurSel();

	ShowDlg(Index);

	int FolderIndex = m_FolderComCtrl.GetCurSel();
	int AniSpriteType = FolderIndex;

	if ((int)ANI_ETC <= AniSpriteType)
	{
		AniSpriteType = ANI_ETC;
	}

	switch (m_CurListIndex)
	{
	case CAniSpriteDlg::TEXTURELIST:
		CEditGlobal::TextureList->UpdateList((FOLDER)FolderIndex);
		break;
	case CAniSpriteDlg::ANISPRITELIST:
		CEditGlobal::MultiSpriteList->UpdateList((ANISPRITETYPE)AniSpriteType);
		break;
	default:
		break;
	}

	*pResult = 0;
}


// EditBox�� �Է��� �ٲ������ �۵��ϴ� �Լ�
//(XCount,YCount,StartFrame,FrameCount)
void CAniSpriteDlg::OnEnChangeStartframe()
{
	UpdateData(TRUE);
	m_iFrameCount = m_iTempFrameCount - m_iStartFrame;
	if (m_iFrameCount <= 0) 
	{
		GetDlgItem(IDC_STARTFRAME)->SetFocus();
		m_StartFrameCtrl.SetSel(0, -1, true);
		m_iStartFrame = 0;
		m_iFrameCount = m_iTempFrameCount;
		AfxMessageBox(_T("Start Frame ���� ����ġ �ʽ��ϴ�."));
	}
	UpdateData(FALSE);
}


void CAniSpriteDlg::OnEnChangeXcount()
{
	
	int Row, Colm;
	UpdateData(TRUE);
	Row = GetDlgItemInt(IDC_XCOUNT);
	Colm = GetDlgItemInt(IDC_YCOUNT);
	m_iTempFrameCount = Row * Colm;
	m_iFrameCount = Row * Colm;
	UpdateData(FALSE);
	
}
void CAniSpriteDlg::OnEnChangeYcount()
{
	int Row, Colm;
	UpdateData(TRUE);
	Row = GetDlgItemInt(IDC_XCOUNT);
	Colm = GetDlgItemInt(IDC_YCOUNT);
	m_iTempFrameCount = Row * Colm;
	m_iFrameCount = Row * Colm;
	UpdateData(FALSE);
}


void CAniSpriteDlg::OnEnChangeNewframecount()
{
	if (m_iFrameCount <= 0) 
	{
		AfxMessageBox(_T("Frame Count ���� ����ġ �ʽ��ϴ�."));
		GetDlgItem(IDC_NEWFRAMECOUNT)->SetFocus();
		m_StartFrameCtrl.SetSel(0, -1, true);
		m_iStartFrame = m_iColm * m_iRow;
	}
	UpdateData(TRUE);
	m_iTempFrameCount = m_iFrameCount;
	UpdateData(FALSE);
}


