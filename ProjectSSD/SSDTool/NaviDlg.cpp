// NaviDlg.cpp: 구현 파일
//

#include "stdafx.h"
#include "SSDTool.h"
#include "NaviDlg.h"
#include "afxdialogex.h"

#include <string>
#include "SSDToolGlobal.h"
// NaviDlg 대화 상자

#include <DeadByDaylightGlobal.h>
#include <Terrain.h>
#include <TerrainRenderer.h>
#include <NavigationMesh.h>
#include <NavigationObject.h>
#include <NaviMeshArea.h>

#include <Camera.h>
#include <GlobalDebug.h>

IMPLEMENT_DYNAMIC(NaviDlg, CDialogEx)

NaviDlg::NaviDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_NAVIDLG, pParent)
	, m_NaviAreaType(0)
	, m_NaviAreaIdx(-1)
	, m_fAreaPivotX(0)
	, m_fAreaPivotY(0)
	, m_LinkRadio1(0)
	, m_LinkRadio2(0)
	, m_LinkRadio3(0)
	, m_LinkMode(0)
	, m_CurSelectNaviMesh(nullptr)
{
	memset(m_SelectLinkVtx, -1, sizeof(m_SelectLinkVtx));
}

NaviDlg::~NaviDlg()
{
}

void NaviDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RECTAREA, (int&)m_NaviAreaType);
	DDX_Radio(pDX, IDC_LINKTRIRADIO, (int&)m_LinkMode);
	DDX_Radio(pDX, IDC_NONEVTX_1, (int&)m_LinkRadio1);
	DDX_Radio(pDX, IDC_NONEVTX_2, (int&)m_LinkRadio2);
	DDX_Radio(pDX, IDC_NONEVTX_3, (int&)m_LinkRadio3);
	DDX_Control(pDX, IDC_AREACOMBO, m_NaviAreaCombo);
	DDX_Control(pDX, IDC_NAVIMESHPATH, m_NavimeshPathEdit);
	DDX_Control(pDX, IDC_NAVIMESHFILE, m_NavimeshFileEdit);
	DDX_Control(pDX, IDC_AREAPIVOTX, m_AreaPivotXEdit);
	DDX_Control(pDX, IDC_AREAPIVOTY, m_AreaPivotYEdit);
	DDX_Control(pDX, IDC_LINKTRIANGLELIST, m_LinkTriList);
}


BEGIN_MESSAGE_MAP(NaviDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &NaviDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &NaviDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_NAVIMESHBUTTON, &NaviDlg::OnBnClickedNavimeshbutton)
	ON_EN_CHANGE(IDC_NAVIMESHFILE, &NaviDlg::OnEnChangeNavimeshfile)
	ON_BN_CLICKED(IDC_ADDAREA, &NaviDlg::OnBnClickedAddarea)
	ON_BN_CLICKED(IDC_DELAREA, &NaviDlg::OnBnClickedDelarea)
	ON_CONTROL_RANGE(BN_CLICKED,IDC_RECTAREA,IDC_CIRCLEAREA, ClickAreaTypeEvent)
	ON_EN_CHANGE(IDC_AREAPIVOTX, &NaviDlg::OnEnChangeAreapivotx)
	ON_CBN_SELCHANGE(IDC_AREACOMBO, &NaviDlg::OnCbnSelchangeAreacombo)
	ON_EN_CHANGE(IDC_AREAPIVOTY, &NaviDlg::OnEnChangeAreapivoty)
	ON_EN_CHANGE(IDC_AREASIZEX, &NaviDlg::OnEnChangeAreasizex)
	ON_EN_CHANGE(IDC_AREASIZEY, &NaviDlg::OnEnChangeAreasizey)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_NONEVTX_1, IDC_TERRAIN_14, LinkRadioEvent)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_NONEVTX_2, IDC_TERRAIN_24, LinkRadioEvent)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_NONEVTX_3, IDC_TERRAIN_34, LinkRadioEvent)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_LINKTRIRADIO, IDC_TERROFFRADIO, ChangeLinkMode)
	ON_BN_CLICKED(IDC_ADDLINKTRI, &NaviDlg::OnBnClickedAddlinkvtx)
	ON_LBN_SELCHANGE(IDC_LINKTRIANGLELIST, &NaviDlg::OnLbnSelchangeLinkvtxlist)
	ON_BN_CLICKED(IDC_DELLINKTRI, &NaviDlg::OnBnClickedDeletelinkvtx)
END_MESSAGE_MAP()


// NaviDlg 메시지 처리기


void NaviDlg::InitNaviMeshEdit()
{
	m_NavimeshPathEdit.SetSel(0, -1);
	m_NavimeshPathEdit.Clear();

	m_NavimeshFileEdit.SetSel(0, -1);
	m_NavimeshFileEdit.Clear();



	m_LinkTriList.ResetContent();

	CString LinkID;
	size_t LinkCount = (int)DBD::pMainTerrain->GetTerrainRen()->GetLinkTriCount();
	for (size_t i = 0; i < LinkCount; i++)
	{
		LinkID.Format(L"%d", (int)(i + 1));
		m_LinkTriList.AddString(LinkID);
	}

}
void NaviDlg::InitNaviAreaEdit()
{
	m_NaviAreaCombo.ResetContent();

	m_NaviAreaCombo.AddString(L"New");
	m_NaviAreaCombo.SetCurSel(0);

	m_NaviAreaIdx = -1;

	ClickAreaTypeEvent(IDC_RECTAREA);

	CString Zero;
	Zero.Format(L"%.2f", 0.f);
	GetDlgItemTextW(IDC_AREAPIVOTX, Zero);
	GetDlgItemTextW(IDC_AREAPIVOTY, Zero);
	GetDlgItemInt(IDC_AREASIZEX, 0);
	GetDlgItemInt(IDC_AREASIZEY, 0);


}

void NaviDlg::AddNaviArea() 
{
	if (nullptr == SSDToolGlobal::ToolSelectNaviArea) 
	{
		TASSERT(true);
		return;
	}

	Vec2 Pivot, Size;
	//Pivot = Vec2((float) GetDlgItemInt(IDC));
	Pivot = Vec2();
	SSDToolGlobal::ToolSelectNaviArea;
}
void NaviDlg::ModifyNaviArea(const int& _Idx)
{
	
}
void NaviDlg::DeleteNaviArea()
{

}

void NaviDlg::UpdataNaviDlgData()
{
	if (nullptr == SSDToolGlobal::ToolSelectNaviObj) 
	{
		TASSERT(true);
		return;
	}


	InitNaviMeshEdit();
	InitNaviAreaEdit();

	SPTR<CMesh> NaviMesh = SSDToolGlobal::ToolSelectNaviMesh->GetGeoNaviMesh();

	if (nullptr != NaviMesh)
	{
		GetDlgItem(IDC_NAVIMESHPATH)->SetWindowTextW(CFilePathMgr::GetPath( L"Obj" , NaviMesh->GetName()).c_str());
		GetDlgItem(IDC_NAVIMESHFILE)->SetWindowTextW(NaviMesh->GetName().c_str());
	}

	int MaxArea = (int)SSDToolGlobal::ToolSelectNaviArea->GetAreaCount();
	CString Temp;

	for (int i = 0; i < MaxArea; ++i)
	{
		Temp.Format(L"%d", i + 1);
		m_NaviAreaCombo.AddString(Temp);
		//SSDToolGlobal::ToolSelectNaviArea->GetAreaData(i);
	}
}

void NaviDlg::ClickAreaTypeEvent(UINT ID)
{
	switch (ID)
	{
	case IDC_RECTAREA:
		m_NaviAreaType = RECTAREA;
		break;
	case IDC_CIRCLEAREA:
		m_NaviAreaType = CIRCLEAREA;
		break;
	default:
		break;
	}
}

void NaviDlg::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//CDialogEx::OnOK();

}


void NaviDlg::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//CDialogEx::OnCancel();

}


BOOL NaviDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	DBD::pMainTerrain->GetTerrainRen()->SetLBCallBackFunc(this, &NaviDlg::TerrainLeftClickEvent);
	DBD::pMainTerrain->GetTerrainRen()->SetRBCallBackFunc(this, &NaviDlg::TerrainRightClickEvent);

	for (CNavigationObject* pNaviObj : SSDToolGlobal::setToolNaviObj)
	{
		pNaviObj->GetNaviRen()->SetNaviMeshClickFunc(this, &NaviDlg::NaviMeshClickEvent);
	}

	AllLinkRadioButtonOff();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	// 수지 컴퓨터 해상도
	//SetWindowPos(this, 0 - 2, 265, 331 - 4 * 0, 961 + 10, SWP_NOZORDER);
	
	// 석재 컴퓨터 해상도
	SetWindowPos(this, 0 - 2, 330, 331 , 971, SWP_NOZORDER);

	m_EditNaviLinkTri = DBD::pMainTerrain->GetTerrainRen()->GetMakingLinkTri();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}




void NaviDlg::OnBnClickedNavimeshbutton()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	CFileDialog fDlg(TRUE, NULL, NULL, OFN_NOCHANGEDIR | OFN_NOCHANGEDIR, L"All Files(*.*)|*.*||");
	if (fDlg.DoModal() == IDOK)
	{
		CString PathName = fDlg.GetPathName();
		std::wstring Path = PathName.GetString();

		wchar_t ArrDrive[MAXSTRING];
		wchar_t ArrFolder[MAXSTRING];
		wchar_t ArrFile[MAXSTRING];
		wchar_t ArrExt[MAXSTRING];

		_wsplitpath_s(Path.c_str(), ArrDrive, ArrFolder, ArrFile, ArrExt);


		if (0 != wcscmp(L".obj", ArrExt))
		{
			return;
		}

		if (nullptr == CResourceMgr<COBJMesh>::Find(ArrFile))
		{
			if (false == Engine::LoadNaviObjFile(ArrFile)) 
			{
				TASSERT(true);
				return;
			}
		}


		GetDlgItem(IDC_NAVIMESHPATH)->SetWindowTextW(PathName);
		GetDlgItem(IDC_NAVIMESHFILE)->SetWindowTextW(ArrFile);


		SSDToolGlobal::ToolSelectNaviMesh->CreateNavigationMesh(CResourceMgr<COBJMesh>::Find(ArrFile));

	}
}


void NaviDlg::OnEnChangeNavimeshfile()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void NaviDlg::OnBnClickedAddarea()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (m_NaviAreaIdx == -1) 
	{
		Vec2 Pivot = Vec2(m_fAreaPivotX, m_fAreaPivotY);
		Vec2 Size = Vec2((float)GetDlgItemInt(IDC_AREASIZEX), (float)GetDlgItemInt(IDC_AREASIZEY));
		size_t AddIdx;

		if (RECTAREA == m_NaviAreaType)
		{
			AddIdx =  SSDToolGlobal::ToolSelectNaviArea->AddRectArea(Pivot, Size);
		}
		else if (CIRCLEAREA == m_NaviAreaType) 
		{
			AddIdx = SSDToolGlobal::ToolSelectNaviArea->AddCircleArea(Pivot, Size.x);
		}

		m_NaviAreaCombo.ResetContent();
		m_NaviAreaCombo.AddString(L"New");

		size_t MaxArea = SSDToolGlobal::ToolSelectNaviArea->GetAreaCount();
		CString Number;
		for (size_t i = 0; i < MaxArea; i++)
		{
			Number.Format(L"%d", (int)(i + 1));
			m_NaviAreaCombo.AddString(Number);
		}
		m_NaviAreaCombo.SetCurSel((int)(AddIdx + 1));
		OnCbnSelchangeAreacombo();
	}
}


void NaviDlg::OnBnClickedDelarea()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void NaviDlg::OnCbnSelchangeAreacombo()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int Cursel = m_NaviAreaCombo.GetCurSel();

	if (Cursel >= 1) 
	{
		m_NaviAreaIdx = Cursel -1 ;
		Vec2 Pivot = SSDToolGlobal::ToolSelectNaviArea->GetAreaData(m_NaviAreaIdx).AreaPos;
		Vec2 Size = SSDToolGlobal::ToolSelectNaviArea->GetAreaData(m_NaviAreaIdx).AreaSize;
		
		CString strPivot;
		strPivot.Format(L"%.1f", Pivot.x);
		SetDlgItemTextW(IDC_AREAPIVOTX, strPivot);
		strPivot.Format(L"%.1f", Pivot.y);
		SetDlgItemTextW(IDC_AREAPIVOTY, strPivot);
		SetDlgItemInt(IDC_AREASIZEX, (int)Size.x);
		SetDlgItemInt(IDC_AREASIZEY, (int)Size.y);
	}
	else 
	{
		m_NaviAreaIdx = -1;
	}

}
void NaviDlg::OnEnChangeAreapivotx()
{
	UpdateData(TRUE);
	GetDlgItemText(IDC_AREAPIVOTX, m_strAreaPivotX);
	m_fAreaPivotX = (float)_tstof(m_strAreaPivotX);

	if (m_NaviAreaIdx == -1)
		return;

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	SSDToolGlobal::ToolSelectNaviArea->ModifyAreaPivot(m_NaviAreaIdx,Vec2(m_fAreaPivotX, m_fAreaPivotY));

}




void NaviDlg::OnEnChangeAreapivoty()
{
	UpdateData(TRUE);
	GetDlgItemText(IDC_AREAPIVOTY, m_strAreaPivotY);
	m_fAreaPivotY = (float)_tstof(m_strAreaPivotY);

	if (m_NaviAreaIdx == -1)
		return;

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	SSDToolGlobal::ToolSelectNaviArea->ModifyAreaPivot(m_NaviAreaIdx, Vec2(m_fAreaPivotX, m_fAreaPivotY));

}


void NaviDlg::OnEnChangeAreasizex()
{
	if (m_NaviAreaIdx == -1)
		return;

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	SSDToolGlobal::ToolSelectNaviArea->ModifyAreaSize(m_NaviAreaIdx, Vec2((float)GetDlgItemInt(IDC_AREASIZEX), (float)GetDlgItemInt(IDC_AREASIZEY)));

}


void NaviDlg::OnEnChangeAreasizey()
{
	if (m_NaviAreaIdx == -1)
		return;

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	SSDToolGlobal::ToolSelectNaviArea->ModifyAreaSize(m_NaviAreaIdx, Vec2((float)GetDlgItemInt(IDC_AREASIZEX), (float)GetDlgItemInt(IDC_AREASIZEY)));
}

void NaviDlg::ChangeLinkMode(UINT _ID)
{
	switch (_ID)
	{
	case IDC_LINKTRIRADIO: 
	{
		m_LinkMode = LINK_MODE;
		
		ActiveNaviVtxRadioButton(nullptr != m_CurSelectNaviMesh);
		ActiveTerrainVtxRadioButton(DBD::pMainTerrain->GetTerrainRen()->IsSelectTri());
		InitSelectLinkVtx();
	}
		break;
	case IDC_TERRONRADIO: 
	{
		m_LinkMode = TERRAINON_MODE;
		SelectTriOff();
		AllLinkRadioButtonOff();
		InitSelectLinkVtx();
	}
		break;
	case IDC_TERROFFRADIO: 
	{
		m_LinkMode = TERRAINOFF_MODE;
		SelectTriOff();
		AllLinkRadioButtonOff();
		InitSelectLinkVtx();
	}
		break;
	default:
		break;
	}
}

void NaviDlg::LinkRadioEvent(UINT _ID)
{
	if (_ID < IDC_NONEVTX_2) 
	{
		m_LinkRadio1 = _ID - IDC_NONEVTX_1;
		return SelectEventLinkV1(m_LinkRadio1);
	}

	if (_ID < IDC_NONEVTX_3)
	{
		m_LinkRadio2 = _ID - IDC_NONEVTX_2;
		return SelectEventLinkV2(m_LinkRadio2);
	}

	m_LinkRadio3 = _ID - IDC_NONEVTX_3;
	return SelectEventLinkV3(m_LinkRadio3);
}

void NaviDlg::SelectEventLinkV1(const int& _ID) 
{
	m_SelectLinkVtx[0] = _ID;

	if (m_SelectLinkVtx[0] == m_LinkRadio2) 
	{
		m_LinkRadio2 = 0;
		UpdateData(FALSE);
	}

	if (m_SelectLinkVtx[0] == m_LinkRadio3)
	{
		m_LinkRadio3 = 0;
		UpdateData(FALSE);
	}

	if (nullptr != m_CurSelectNaviMesh)
	{
		m_CurSelectNaviMesh->InitSelectVtxColor();
	}

	UpdateLinkTriangle();
}
void NaviDlg::SelectEventLinkV2(const int& _ID) 
{
	m_SelectLinkVtx[1] = _ID;

	if (m_SelectLinkVtx[1] == m_LinkRadio1)
	{
		m_LinkRadio1 = 0;
		UpdateData(FALSE);
	}

	if (m_SelectLinkVtx[1] == m_LinkRadio3)
	{
		m_LinkRadio3 = 0;
		UpdateData(FALSE);
	}

	UpdateLinkTriangle();
}
void NaviDlg::SelectEventLinkV3(const int& _ID) 
{
	m_SelectLinkVtx[2] = _ID;

	if (m_SelectLinkVtx[2] == m_LinkRadio1)
	{
		m_LinkRadio1 = 0;
		UpdateData(FALSE);
	}

	if (m_SelectLinkVtx[2] == m_LinkRadio2)
	{
		m_LinkRadio2 = 0;
		UpdateData(FALSE);
	}

	UpdateLinkTriangle();
}

void NaviDlg::AllLinkRadioButtonOff()
{
	GetDlgItem(IDC_NAVI_11)->EnableWindow(false);
	GetDlgItem(IDC_NAVI_12)->EnableWindow(false);
	GetDlgItem(IDC_NAVI_13)->EnableWindow(false);
	GetDlgItem(IDC_TERRAIN_11)->EnableWindow(false);
	GetDlgItem(IDC_TERRAIN_12)->EnableWindow(false);
	GetDlgItem(IDC_TERRAIN_13)->EnableWindow(false);
	GetDlgItem(IDC_TERRAIN_14)->EnableWindow(false);

	GetDlgItem(IDC_NAVI_21)->EnableWindow(false);
	GetDlgItem(IDC_NAVI_22)->EnableWindow(false);
	GetDlgItem(IDC_NAVI_23)->EnableWindow(false);
	GetDlgItem(IDC_TERRAIN_21)->EnableWindow(false);
	GetDlgItem(IDC_TERRAIN_22)->EnableWindow(false);
	GetDlgItem(IDC_TERRAIN_23)->EnableWindow(false);
	GetDlgItem(IDC_TERRAIN_24)->EnableWindow(false);

	GetDlgItem(IDC_NAVI_31)->EnableWindow(false);
	GetDlgItem(IDC_NAVI_32)->EnableWindow(false);
	GetDlgItem(IDC_NAVI_33)->EnableWindow(false);
	GetDlgItem(IDC_TERRAIN_31)->EnableWindow(false);
	GetDlgItem(IDC_TERRAIN_32)->EnableWindow(false);
	GetDlgItem(IDC_TERRAIN_33)->EnableWindow(false);
	GetDlgItem(IDC_TERRAIN_34)->EnableWindow(false);

}

void NaviDlg::ActiveNaviVtxRadioButton(const bool& _bActive)
{
	GetDlgItem(IDC_NAVI_11)->EnableWindow(_bActive);
	GetDlgItem(IDC_NAVI_12)->EnableWindow(_bActive);
	GetDlgItem(IDC_NAVI_13)->EnableWindow(_bActive);

	GetDlgItem(IDC_NAVI_21)->EnableWindow(_bActive);
	GetDlgItem(IDC_NAVI_22)->EnableWindow(_bActive);
	GetDlgItem(IDC_NAVI_23)->EnableWindow(_bActive);

	GetDlgItem(IDC_NAVI_31)->EnableWindow(_bActive);
	GetDlgItem(IDC_NAVI_32)->EnableWindow(_bActive);
	GetDlgItem(IDC_NAVI_33)->EnableWindow(_bActive);
}
void NaviDlg::ActiveTerrainVtxRadioButton(const bool& _bActive)
{
	GetDlgItem(IDC_TERRAIN_11)->EnableWindow(_bActive);
	GetDlgItem(IDC_TERRAIN_12)->EnableWindow(_bActive);
	GetDlgItem(IDC_TERRAIN_13)->EnableWindow(_bActive);
	GetDlgItem(IDC_TERRAIN_14)->EnableWindow(_bActive);

	GetDlgItem(IDC_TERRAIN_21)->EnableWindow(_bActive);
	GetDlgItem(IDC_TERRAIN_22)->EnableWindow(_bActive);
	GetDlgItem(IDC_TERRAIN_23)->EnableWindow(_bActive);
	GetDlgItem(IDC_TERRAIN_24)->EnableWindow(_bActive);

	GetDlgItem(IDC_TERRAIN_31)->EnableWindow(_bActive);
	GetDlgItem(IDC_TERRAIN_32)->EnableWindow(_bActive);
	GetDlgItem(IDC_TERRAIN_33)->EnableWindow(_bActive);
	GetDlgItem(IDC_TERRAIN_34)->EnableWindow(_bActive);

}
void NaviDlg::AllLinkRadioButtonOn() 
{
	GetDlgItem(IDC_NAVI_11)->EnableWindow(true);
	GetDlgItem(IDC_NAVI_12)->EnableWindow(true);
	GetDlgItem(IDC_NAVI_13)->EnableWindow(true);
	GetDlgItem(IDC_TERRAIN_11)->EnableWindow(true);
	GetDlgItem(IDC_TERRAIN_12)->EnableWindow(true);
	GetDlgItem(IDC_TERRAIN_13)->EnableWindow(true);
	GetDlgItem(IDC_TERRAIN_14)->EnableWindow(true);

	GetDlgItem(IDC_NAVI_21)->EnableWindow(true);
	GetDlgItem(IDC_NAVI_22)->EnableWindow(true);
	GetDlgItem(IDC_NAVI_23)->EnableWindow(true);
	GetDlgItem(IDC_TERRAIN_21)->EnableWindow(true);
	GetDlgItem(IDC_TERRAIN_22)->EnableWindow(true);
	GetDlgItem(IDC_TERRAIN_23)->EnableWindow(true);
	GetDlgItem(IDC_TERRAIN_24)->EnableWindow(true);

	GetDlgItem(IDC_NAVI_31)->EnableWindow(true);
	GetDlgItem(IDC_NAVI_32)->EnableWindow(true);
	GetDlgItem(IDC_NAVI_33)->EnableWindow(true);
	GetDlgItem(IDC_TERRAIN_31)->EnableWindow(true);
	GetDlgItem(IDC_TERRAIN_32)->EnableWindow(true);
	GetDlgItem(IDC_TERRAIN_33)->EnableWindow(true);
	GetDlgItem(IDC_TERRAIN_34)->EnableWindow(true);
}
void NaviDlg::SelectTriOff()
{
	if (nullptr != m_CurSelectNaviMesh) 
	{
		m_CurSelectNaviMesh->InitSelectTriIdx();
		m_CurSelectNaviMesh = nullptr;
	}

	if (nullptr != DBD::pMainTerrain) 
	{
		DBD::pMainTerrain->GetTerrainRen()->InitSelectTerrainTri();
	}
}
void NaviDlg::TerrainLeftClickEvent(SPTR<CTerrainRenderer> _pTR) 
{
	switch (m_LinkMode)
	{
	case LINK_MODE:
		return SelectLinkTerrain(_pTR);
	case TERRAINON_MODE:
		return TerrainNaviTriOn(_pTR);
	case TERRAINOFF_MODE:
		return TerrainNaviTriOff(_pTR);
	}

}
void NaviDlg::TerrainRightClickEvent(SPTR<CTerrainRenderer> _pTR) 
{


	
}
void NaviDlg::NaviMeshClickEvent(SPTR<CNavigationMesh> _pNaviMesh) 
{
	if (m_LinkMode == LINK_MODE)
	{
		SelectLinkNaviMeshTri(_pNaviMesh);
	}
}

void NaviDlg::SelectLinkTerrain(SPTR<CTerrainRenderer> _pTR) 
{
	if (false == _pTR->ConnectableTerrain())
		return;
	
	_pTR->SelectTerrainTri();
	_pTR->InitTerrainVtxColor();
	m_EditNaviLinkTri->SetLinkTerrainIdx(_pTR->GetSelectTriIdx());

	ActiveTerrainVtxRadioButton(true);

	for (size_t i = 0; i < 3; i++)
	{
		if (m_SelectLinkVtx[i] >= 4 && m_SelectLinkVtx[i] <= 7) 
		{
			_pTR->SetTerrainVtxColor(m_SelectLinkVtx[i] - 4, Vec4::PastelRed);
		}
	}

	
}
void NaviDlg::TerrainNaviTriOn(SPTR<CTerrainRenderer> _pTR) 
{
	_pTR->SelectTriOn();
}
void NaviDlg::TerrainNaviTriOff(SPTR<CTerrainRenderer> _pTR) 
{
	_pTR->SelectTriOff();
}
void NaviDlg::SelectLinkNaviMeshTri(SPTR<CNavigationMesh> _pNaviMesh) 
{
	m_CurSelectNaviMesh = _pNaviMesh;
	m_CurSelectNaviMesh->SelectTriIdx();
	m_CurSelectNaviMesh->InitSelectVtxColor();
	m_EditNaviLinkTri->LinkNavigationMesh(m_CurSelectNaviMesh);
	m_EditNaviLinkTri->SetNaviMeshTriIdx(m_CurSelectNaviMesh->GetSelectTriIdex());

	ActiveNaviVtxRadioButton(true);

	for (size_t i = 0; i < 3; i++)
	{
		if (m_SelectLinkVtx[i] >= 1 && m_SelectLinkVtx[i] <= 3)
		{
			m_CurSelectNaviMesh->SetSelectTriVtx(m_SelectLinkVtx[i] - 1,Vec4::PastelRed);
		}
	}
}

void NaviDlg::DebugRender(SPTR<CCamera> _Cam)
{
	int a = 0;

	//GlobalDebug::DrawDebugSphere(_Cam->GetVPMatrix(), Vec3(500.f, 0.f, 500.f), 30.f, Vec4::Red);
}

void NaviDlg::AddNavigationMeshEvent(SPTR<CNavigationMesh> _pNaviMesh)
{
	_pNaviMesh->SetNaviMeshClickFunc(this, &NaviDlg::NaviMeshClickEvent);
}

void NaviDlg::UpdateLinkTriangle()
{
	if (nullptr != m_CurSelectNaviMesh)
	{
		m_CurSelectNaviMesh->InitSelectVtxColor();
	}

	DBD::pMainTerrain->GetTerrainRen()->InitTerrainVtxColor();

	for (UINT i = 0; i < 3; i++)
	{
		if (0 < m_SelectLinkVtx[i])
		{
			if (m_SelectLinkVtx[i] <= 3)
			{
				m_CurSelectNaviMesh->SetSelectTriVtx(m_SelectLinkVtx[i] - 1, Vec4::PastelRed);
				m_EditNaviLinkTri->SetLinkVtx(i, true, m_SelectLinkVtx[i] - 1);
			}
			else if (m_SelectLinkVtx[i] <= 7)
			{
				DBD::pMainTerrain->GetTerrainRen()->SetTerrainVtxColor(m_SelectLinkVtx[i] - 4, Vec4::PastelRed);
				m_EditNaviLinkTri->SetLinkVtx(i, false, m_SelectLinkVtx[i] - 4);
			}
		}
	}
}
void NaviDlg::InitSelectLinkVtx()
{
	m_SelectLinkVtx[0] = -1;
	m_SelectLinkVtx[1] = -1;
	m_SelectLinkVtx[2] = -1;
}
BOOL NaviDlg::DestroyWindow()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (m_CurSelectNaviMesh != nullptr) 
	{
		m_CurSelectNaviMesh->InitSelectTriIdx();
		m_EditNaviLinkTri->Init();
		m_EditNaviLinkTri->Off();
	}

	if (DBD::pMainTerrain != nullptr) 
	{
		DBD::pMainTerrain->GetTerrainRen()->InitSelectTerrainTri();
	}

	m_EditNaviLinkTri = nullptr;
	return CDialogEx::DestroyWindow();
}


void NaviDlg::OnBnClickedAddlinkvtx()
{
	// 현재 에디팅중인 링크 삼각형이 구성된 경우 터레인에 링크 삼각형을 추가한다.
	if (true == m_EditNaviLinkTri->IsLinkTriangle()) 
	{
		DBD::pMainTerrain->GetTerrainRen()->AddLinkVtx();
		m_EditNaviLinkTri = DBD::pMainTerrain->GetTerrainRen()->GetMakingLinkTri();
		m_EditNaviLinkTri->LinkNavigationMesh(m_CurSelectNaviMesh);
		UpdateLinkTriangle();
		
		CString LinkID;
		LinkID.Format(L"%d", (int)DBD::pMainTerrain->GetTerrainRen()->GetLinkTriCount());
		m_LinkTriList.AddString(LinkID);
	}



}

void NaviDlg::OnBnClickedDeletelinkvtx()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UINT Idx = (UINT)m_LinkTriList.GetCurSel();
	if (true == DBD::pMainTerrain->GetTerrainRen()->DeleteLinkVtx(Idx)) 
	{
		int Last = m_LinkTriList.GetCount();
		if (Last >= 1)
		{
			m_LinkTriList.DeleteString(Last - 1);
			m_LinkTriList.SetCurSel(-1);
		}
	}
}

void NaviDlg::OnLbnSelchangeLinkvtxlist()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UINT Idx = (UINT)m_LinkTriList.GetCurSel();
	DBD::pMainTerrain->GetTerrainRen()->SelectLinkTri(Idx);
}


