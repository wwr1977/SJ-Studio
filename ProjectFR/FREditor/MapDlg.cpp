// MapDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "FREditor.h"
#include "MapDlg.h"
#include "afxdialogex.h"
#include "EditMapMgr.h"
#include "EditTileMap.h"


// CMapDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CMapDlg, CDialogEx)

CMapDlg::CMapDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_MAPDLG, pParent)
	, m_bMapSceneInit(false)
	, m_TileEditRadio(0)
	, m_uiSpawnTileIndexX(0)
	, m_uiSpawnTileIndexY(0)
	, m_strPortalTileMapSize(_T(""))
	, m_CurEditMode(TILE_EDIT)
	, m_uiCurObjectNo(0)
	, m_strCurObjectSize(_T(""))
{
	CEditGlobal::MapDlg = this;
}

CMapDlg::~CMapDlg()
{
}

void CMapDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_TILEMAPCOM, m_TileMapCom);
	DDX_Radio(pDX, IDC_TILERADIO, (int &)m_TileEditRadio);
	DDX_Control(pDX, IDC_TILETYPECOM, m_TileTypeCom);
	DDX_Control(pDX, IDC_TILEOPTIONCOM, m_TileOptionCom);
	DDX_Control(pDX, IDC_TILEINDEXXEDIT, m_SpawnTileIndexX);
	DDX_Text(pDX, IDC_TILEINDEXXEDIT, m_uiSpawnTileIndexX);
	DDX_Control(pDX, IDC_TILEINDEXYEDIT, m_SpawnTileIndexY);
	DDX_Text(pDX, IDC_TILEINDEXYEDIT, m_uiSpawnTileIndexY);
	DDX_Text(pDX, IDC_EDIT1, m_strPortalTileMapSize);
	DDX_Control(pDX, IDC_MAPOBJECTCOMBO, m_MapObjectCom);
	DDX_Text(pDX, IDC_MAPOBJECTNO, m_uiCurObjectNo);
	DDX_Text(pDX, IDC_MAPOBJECTSIZE, m_strCurObjectSize);
}


BEGIN_MESSAGE_MAP(CMapDlg, CDialogEx)
	ON_WM_CREATE()
	ON_WM_GETMINMAXINFO()
	ON_WM_DESTROY()
	ON_WM_SETCURSOR()
	ON_WM_SHOWWINDOW()
	ON_WM_MOUSEWHEEL()
	ON_CBN_SELCHANGE(IDC_TILEMAPCOM, &CMapDlg::OnCbnSelchangeTilemapcom)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_TILERADIO, IDC_OBJECTRADIO, CMapDlg::SetRadioStatus)
	ON_CBN_SELCHANGE(IDC_TILETYPECOM, &CMapDlg::OnCbnSelchangeTiletypecom)
	ON_CBN_SELCHANGE(IDC_TILEOPTIONCOM, &CMapDlg::OnCbnSelchangeTileoptioncom)
	ON_BN_CLICKED(IDC_TILELOADBUTTON, &CMapDlg::OnBnClickedTileloadbutton)
	ON_BN_CLICKED(IDC_TILESAVEBUTTON, &CMapDlg::OnBnClickedTilesavebutton)
	ON_BN_CLICKED(IDC_OBJECTLOADBUTTON, &CMapDlg::OnBnClickedObjectloadbutton)
	ON_BN_CLICKED(IDC_OBJECTSAVEBUTTON, &CMapDlg::OnBnClickedObjectsavebutton)
	ON_CBN_SELCHANGE(IDC_MAPOBJECTCOMBO, &CMapDlg::OnCbnSelchangeMapobjectcombo)
END_MESSAGE_MAP()


// CMapDlg 메시지 처리기입니다.

void CMapDlg::InitMapSceneEvent()
{
	m_bMapSceneInit = true;


	if (nullptr == CEditGlobal::EditMapMgr)
		return;

	size_t ObjectCount = CEditGlobal::EditMapMgr->GetObjectCount();
	CString Temp;

	m_MapObjectCom.ResetContent();

	for (UINT i = 0; i < ObjectCount; i++) 
	{
		Temp.Format(_T("%2d. %s"), i + 1, CEditGlobal::EditMapMgr->GetObjectKName(i).c_str());
		m_MapObjectCom.AddString(Temp);
	}
	
	m_MapObjectCom.SetCurSel(0);
	OnCbnSelchangeMapobjectcombo();
	EnableObjectDataEdit(FALSE);
}
void CMapDlg::EnableTileDataEdit(const BOOL& _Enable)
{
	m_TileTypeCom.EnableWindow(_Enable);
	m_TileOptionCom.EnableWindow(_Enable);
	m_SpawnTileIndexX.EnableWindow(_Enable);
	m_SpawnTileIndexY.EnableWindow(_Enable);
}
void CMapDlg::EnableObjectDataEdit(const BOOL& _Enable)
{
	m_MapObjectCom.EnableWindow(_Enable);

}
void CMapDlg::EnableTileIndexEdit(const BOOL& _Enable)
{
	m_SpawnTileIndexX.EnableWindow(_Enable);
	m_SpawnTileIndexY.EnableWindow(_Enable);
}
void CMapDlg::CurEnableTileDataEdit()
{
	TILEOPTION Op = (TILEOPTION)m_TileTypeCom.GetCurSel();

	EnableTileIndexEdit(FALSE);
	m_TileOptionCom.EnableWindow(TRUE);

	switch (Op)
	{
	case STEPTILE:
	case STEPSTAIRTILE:
	case WORLDMAPTILE:
	return;
	case STAIRTILE:
	case NOENTERTILE:
		m_TileOptionCom.EnableWindow(FALSE);
	return;
	case MAPCHANGETILE:
		EnableTileIndexEdit(TRUE);
	return;
	}
}
void CMapDlg::SelectTileMapCombo(const int& _TileMapIndex)
{
	if (_TileMapIndex >= m_TileMapCom.GetCount())
		return;

	CEditGlobal::EditMapMgr->ChangeTileMap((UINT)_TileMapIndex);
	CEditGlobal::EditMapMgr->InitCameraPos();
}
void CMapDlg::SelectTileTypeCombo(const int& _TileTypeIndex)
{
	TILEOPTION Op = (TILEOPTION)_TileTypeIndex;

	EnableTileIndexEdit(FALSE);
	m_TileOptionCom.EnableWindow(TRUE);
	m_strPortalTileMapSize = _T("");


	switch (Op)
	{
	case STEPTILE:
	case STEPSTAIRTILE:
	{
		m_TileOptionCom.ResetContent();
		for (size_t i = 0; i < m_TileTypeString[TILECOLOR].size(); i++)
			m_TileOptionCom.AddString(m_TileTypeString[TILECOLOR][i]);
		m_TileOptionCom.SetCurSel(0);
	}
	break;
	case STAIRTILE: 
	case NOENTERTILE:
	{
		m_TileOptionCom.SetCurSel(-1);
		m_TileOptionCom.EnableWindow(FALSE);
	}
	break;
	case MAPCHANGETILE: 
	{
		m_TileOptionCom.ResetContent();
		for (size_t i = 0; i < m_TileTypeString[TILEMAPNAME].size(); i++)
			m_TileOptionCom.AddString(m_TileTypeString[TILEMAPNAME][i]);

		m_TileOptionCom.SetCurSel(0);
		EnableTileIndexEdit(TRUE);

		POINT MapSize = CEditGlobal::EditMapMgr->GetTileMapSize(0);
		m_strPortalTileMapSize.Format(_T("%3d * %3d"), MapSize.x, MapSize.y);


		m_uiSpawnTileIndexX = 0;
		m_uiSpawnTileIndexY = 0;
		UpdateData(FALSE);
	}
	break;
	case WORLDMAPTILE: 
	{
		m_TileOptionCom.ResetContent();
		for (size_t i = 0; i < m_TileTypeString[WORLDMAPOPTION].size(); i++)
			m_TileOptionCom.AddString(m_TileTypeString[WORLDMAPOPTION][i]);

		m_TileOptionCom.SetCurSel(0);
	}
	break;
	}

	UpdateData(FALSE);
}
void CMapDlg::SelectTileOptionCombo(const int& _TileIOptionIndex)
{
	TILEOPTION Op = (TILEOPTION)m_TileTypeCom.GetCurSel();
	
	switch (Op)
	{
	case STEPTILE:
	case STEPSTAIRTILE:
		ChangeColorOptionEvent(_TileIOptionIndex);
		return;
	case MAPCHANGETILE:
		ChangeTileMapEvent(_TileIOptionIndex);
		return;
	case WORLDMAPTILE:
		ChangeWorldMapTypeEvnet(_TileIOptionIndex);
		return;
	}

	return;
}
void CMapDlg::SelectMapObjectCombo(const int& _ObjectNo)
{
	if (nullptr == CEditGlobal::EditMapMgr)
		return;

	if (_ObjectNo >= CEditGlobal::EditMapMgr->GetObjectCount())
		return;

	m_uiCurObjectNo = _ObjectNo;
	
	CEditGlobal::EditMapMgr->ChangeCurMapObject(_ObjectNo);
	POINT ObjectSize = CEditGlobal::EditMapMgr->GetObjectSize(_ObjectNo);

	m_strCurObjectSize.Format(_T("%2d * %2d"), ObjectSize.x, ObjectSize.y);
	
	UpdateData(FALSE);
	return;
}
void CMapDlg::ChangeColorOptionEvent(const int& TileMapNo)
{

}
void CMapDlg::ChangeTileMapEvent(const int& TileMapNo)
{
	POINT MapSize = CEditGlobal::EditMapMgr->GetTileMapSize(TileMapNo);
	m_strPortalTileMapSize.Format(_T("%3d * %3d"), MapSize.x, MapSize.y);

	m_uiSpawnTileIndexX = 0;
	m_uiSpawnTileIndexY = 0;

	UpdateData(FALSE);
}
void CMapDlg::ChangeWorldMapTypeEvnet(const int& TileMapNo)
{
	
}

BOOL CMapDlg::CheckButtonEvent(MSG* pMsg)
{
	if (pMsg->hwnd == GetDlgItem(IDC_TILELOADBUTTON)->m_hWnd)
	{
		OnBnClickedTileloadbutton();
		return TRUE;
	}
	if (pMsg->hwnd == GetDlgItem(IDC_TILESAVEBUTTON)->m_hWnd)
	{
		OnBnClickedTilesavebutton();
		return TRUE;
	}
	if (pMsg->hwnd == GetDlgItem(IDC_OBJECTLOADBUTTON)->m_hWnd)
	{
		OnBnClickedObjectloadbutton();
		return TRUE;
	}
	if (pMsg->hwnd == GetDlgItem(IDC_OBJECTSAVEBUTTON)->m_hWnd)
	{
		OnBnClickedObjectsavebutton();
		return TRUE;
	}

	return FALSE;
}
BOOL CMapDlg::EditBoxClickCheck(MSG* _pMsg)
{
	for (size_t i = 0; i < m_vecEdit.size(); i++)
	{
		if (_pMsg->hwnd == m_vecEdit[i]->m_hWnd
			&& GetFocus()->m_hWnd != m_vecEdit[i]->m_hWnd)
		{
			m_vecEdit[i]->SetFocus();
			m_vecEdit[i]->SetSel(0, -1, true);
			return TRUE;
		}
	}
	return FALSE;
}
BOOL CMapDlg::CheckTileXIndex()
{
	UINT PrevX = m_uiSpawnTileIndexX;
	
	UpdateData(TRUE);

	UINT CurX = m_uiSpawnTileIndexX;

	POINT MaxIndex = CEditGlobal::EditMapMgr->GetTileMapMaxIndex(m_TileOptionCom.GetCurSel());

	if (CurX > (UINT)MaxIndex.x) 
	{
		m_uiSpawnTileIndexX = PrevX;
		UpdateData(FALSE);
		return FALSE;
	}

	return TRUE;
}
BOOL CMapDlg::CheckTileYIndex()
{
	UINT PrevY = m_uiSpawnTileIndexY;

	UpdateData(TRUE);

	UINT CurY = m_uiSpawnTileIndexY;

	POINT MaxIndex = CEditGlobal::EditMapMgr->GetTileMapMaxIndex(m_TileOptionCom.GetCurSel());

	if (CurY > (UINT)MaxIndex.y)
	{
		m_uiSpawnTileIndexX = PrevY;
		UpdateData(FALSE);
		return FALSE;
	}

	return TRUE;
}

BOOL CMapDlg::LoadTileData()
{
	CGameFile LoadTileFile(_T("EditData"), _T("TileData.bin"));

	if (FALSE == LoadTileFile.Open(_T("rb")))
	{
		MessageBox(_T("Tile Data File을 열지 못하였습니다."));
		return FALSE;
	}

	if (FALSE == CEditGlobal::EditMapMgr->LoadTileData(LoadTileFile.GetFile()))
	{
		MessageBox(_T("타일 로드에 실패하였습니다."));
		return FALSE;
	}

	return TRUE;
}
BOOL CMapDlg::SaveTileData() 
{
	CGameFile SaveTileFile(_T("EditData"), _T("TileData.bin"));
	
	if (FALSE == SaveTileFile.Open(_T("wb")))
	{
		MessageBox(_T("Tile Data File을 열지 못하였습니다."));
		return FALSE;
	}

	if (FALSE == CEditGlobal::EditMapMgr->SaveTileData(SaveTileFile.GetFile()))
	{
		MessageBox(_T("타일 저장에 실패하였습니다."));
		return FALSE;
	}

	return TRUE;
}
BOOL CMapDlg::LoadObjectData() 
{
	CGameFile LoadTileFile(_T("EditData"), _T("MapObjectData.bin"));


	if (FALSE == CEditGlobal::EditMapMgr->LoadMapObjectData(&LoadTileFile))
	{
		MessageBox(_T("맵 오브젝트 로드에 실패하였습니다."));
		return FALSE;
	}


	return TRUE;
}
BOOL CMapDlg::SaveObjectData() 
{
	CGameFile SaveFile(_T("EditData"), _T("MapObjectData.bin"));

	if (FALSE == SaveFile.Open(_T("wb")))
	{
		MessageBox(_T("Map Object Data File을 열지 못하였습니다."));
		return FALSE;
	}

	if (FALSE == CEditGlobal::EditMapMgr->SaveMapObjectData(SaveFile.GetFile()))
	{
		MessageBox(_T("오브젝트 저장에 실패하였습니다."));
		return FALSE;
	}

	return TRUE;
}
const CMapDlg::EDITMODE CMapDlg::GetCurEditMode()
{
	return m_CurEditMode;
}
const TILEOPTION CMapDlg::GetCurTileType()
{
	return (TILEOPTION)m_TileTypeCom.GetCurSel();
}
const UINT CMapDlg::GetCurTileSubOption()
{
	return (UINT)m_TileOptionCom.GetCurSel();
}
const POINT CMapDlg::GetSpawnTileIndex()
{
	UpdateData(TRUE);

	return POINT{ (LONG)m_uiSpawnTileIndexX,(LONG)m_uiSpawnTileIndexY };
}
const UINT CMapDlg::GetCurMapObject()
{
	return (UINT)m_MapObjectCom.GetCurSel();
}
void CMapDlg::OnOK()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	return;

	//CDialogEx::OnOK();
}


int CMapDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
	m_DlgRect = { 0,0,629,818 };//818
	return 0;
}


void CMapDlg::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	lpMMI->ptMaxTrackSize.x = m_DlgRect.right - m_DlgRect.left;
	lpMMI->ptMaxTrackSize.y = m_DlgRect.bottom - m_DlgRect.top;
	lpMMI->ptMinTrackSize.x = m_DlgRect.right - m_DlgRect.left;
	lpMMI->ptMinTrackSize.y = m_DlgRect.bottom - m_DlgRect.top;

	CDialogEx::OnGetMinMaxInfo(lpMMI);
}


BOOL CMapDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	
	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	m_TileMapCom.AddString(_T("1. 마을"));
	m_TileMapCom.AddString(_T("2. 유적"));
	m_TileMapCom.AddString(_T("3. 숲"));
	m_TileMapCom.AddString(_T("4. 악마 성"));
	m_TileMapCom.AddString(_T("5. 무기점"));
	m_TileMapCom.AddString(_T("6. 악세사리 점"));

	m_TileTypeCom.AddString(_T("1. 발판 타일"));
	m_TileTypeCom.AddString(_T("2. 계단 타일"));
	m_TileTypeCom.AddString(_T("3. 발판 & 계단 타일"));
	m_TileTypeCom.AddString(_T("4. 포탈 타일"));
	m_TileTypeCom.AddString(_T("5. 월드맵 타일"));
	m_TileTypeCom.AddString(_T("6. 진입 불가 타일"));
	
	
	vector<CString> Temp;
	Temp.push_back(_T("1. Green"));
	Temp.push_back(_T("2. Blue"));
	Temp.push_back(_T("3. Yellow"));

	m_TileTypeString.push_back(Temp);
	Temp.clear();

	Temp.push_back(_T("1. 마을"));
	Temp.push_back(_T("2. 유적"));
	Temp.push_back(_T("3. 숲"));
	Temp.push_back(_T("4. 악마 성"));
	Temp.push_back(_T("5. 무기점"));
	Temp.push_back(_T("6. 악세사리 점"));

	m_TileTypeString.push_back(Temp);
	Temp.clear();

	Temp.push_back(_T("1. Type1"));
	Temp.push_back(_T("2. Type2"));

	m_TileTypeString.push_back(Temp);
	Temp.clear();

	for (size_t i = 0; i < m_TileTypeString[TILECOLOR].size(); i++)
		m_TileOptionCom.AddString(m_TileTypeString[TILECOLOR][i]);

	m_TileTypeCom.SetCurSel(0);
	m_TileOptionCom.SetCurSel(0);

	m_TileMapCom.SetCurSel(0);

	m_vecEdit.push_back(&m_SpawnTileIndexX);
	m_vecEdit.push_back(&m_SpawnTileIndexY);
	
	EnableTileIndexEdit(FALSE);


	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CMapDlg::OnDestroy()
{

	CDialogEx::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}


BOOL CMapDlg::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	SetCursor(LoadCursor(NULL, IDC_ARROW));
	return TRUE;
	//return CDialogEx::OnSetCursor(pWnd, nHitTest, message);
}


void CMapDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);


	if (true == m_bMapSceneInit) 
	{
		if (TRUE == bShow)
		{
			CEditGlobal::EditMapMgr->CurTileMapOn();
		}
		else 
		{
			CEditGlobal::EditMapMgr->CurTileMapOff();
		}
	}
	
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}


BOOL CMapDlg::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	return CDialogEx::OnMouseWheel(nFlags, zDelta, pt);
}
BOOL CMapDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	{
		if (TRUE == CheckButtonEvent(pMsg))
			return TRUE;
	}

	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE) {
			// VK_RETURN  : Enter Key
			// VK_ESCAPE  : ESC key

			return TRUE;
		}
	}

	if (pMsg->message == WM_LBUTTONDOWN)
	{
		SendMessageA(this->m_hWnd, WM_SETFOCUS, NULL, NULL);

		if (TRUE == EditBoxClickCheck(pMsg))
		{
			return TRUE;
		}

	}
	return CDialogEx::PreTranslateMessage(pMsg);
}
BOOL CMapDlg::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	/*UpdateData(TRUE);
	CheckEidtValue(nID);
	UpdateData(FALSE);*/
	if (nID == IDC_TILEINDEXXEDIT) 
	{
		if (FALSE == CheckTileXIndex())
			m_SpawnTileIndexX.SetSel(-1, -1);	
	}
	else if (nID == IDC_TILEINDEXYEDIT) 
	{
		if (FALSE == CheckTileYIndex())
			m_SpawnTileIndexY.SetSel(-1, -1);
	}

	return CDialogEx::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}


void CMapDlg::OnCbnSelchangeTilemapcom()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	SelectTileMapCombo(m_TileMapCom.GetCurSel());
}
void CMapDlg::OnCbnSelchangeTiletypecom()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	SelectTileTypeCombo(m_TileTypeCom.GetCurSel());
}
void CMapDlg::OnCbnSelchangeTileoptioncom()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	SelectTileOptionCombo(m_TileOptionCom.GetCurSel());

}
void CMapDlg::SetRadioStatus(UINT value)
{
	int a = 0;

	UpdateData(TRUE); 
	
	switch (m_TileEditRadio)
	{ 
	case 0:    
	{
		EnableTileDataEdit(TRUE);
		EnableObjectDataEdit(FALSE);
		CurEnableTileDataEdit();
		m_CurEditMode = TILE_EDIT;
		if (nullptr != CEditGlobal::EditMapMgr)
			CEditGlobal::EditMapMgr->CurMapObjectEnable(FALSE);
	}
		break;
	case 1:    
	{
		EnableTileDataEdit(FALSE);
		EnableObjectDataEdit(TRUE);
		m_CurEditMode = OBJECT_EDIT;
		if (nullptr != CEditGlobal::EditMapMgr)
			CEditGlobal::EditMapMgr->CurMapObjectEnable(TRUE);
	}
		break;
	default:
		break;
	}

}

void CMapDlg::OnBnClickedTileloadbutton()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	LoadTileData();
}


void CMapDlg::OnBnClickedTilesavebutton()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	SaveTileData();
}


void CMapDlg::OnBnClickedObjectloadbutton()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	LoadObjectData();
}


void CMapDlg::OnBnClickedObjectsavebutton()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	SaveObjectData();
}


void CMapDlg::OnCbnSelchangeMapobjectcombo()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	SelectMapObjectCombo(m_MapObjectCom.GetCurSel());
}
