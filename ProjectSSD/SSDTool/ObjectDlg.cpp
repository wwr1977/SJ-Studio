// ObjectDlg.cpp: 구현 파일
//

#include "stdafx.h"
#include "SSDTool.h"
#include "ObjectDlg.h"
#include "afxdialogex.h"
#include "SceneListDlg.h"

#include "EditDlg.h"
#include "MaterialDlg.h"
#include <FileStream.h>
#include <string>
#include <Base3DRenderer.h>
#include <FilePathMgr.h>
#include <Fbx.h>
#include <FbxRenderer.h>
#include <Scene.h>
#include <Material.h>
#include "NaviDlg.h"
#include <DeadByDaylightGlobal.h>

// ObjectDlg 대화 상자



IMPLEMENT_DYNAMIC(ObjectDlg, CDialogEx)

ObjectDlg::ObjectDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_OBJECTDLG, pParent), TempName(nullptr), RenderActor(nullptr)
{

}

ObjectDlg::~ObjectDlg()
{
	std::list<FbxData*>::iterator StartIter = m_FbxListDatalist.begin();
	std::list<FbxData*>::iterator EndIter = m_FbxListDatalist.end();
	for (; StartIter != EndIter; ++StartIter)
	{
		if (nullptr != (*StartIter))
		{
			delete (*StartIter);
		}
	}
	m_FbxListDatalist.clear();
}

void ObjectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_FBXTREE, m_FbxTree);
	DDX_Control(pDX, IDC_MESHLIST, m_MeshList);
}


BEGIN_MESSAGE_MAP(ObjectDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &ObjectDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &ObjectDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON1, &ObjectDlg::OnBnClickedButton1)
	ON_NOTIFY(TVN_SELCHANGED, IDC_FBXTREE, &ObjectDlg::OnTvnSelchangedFbxtree)
	ON_BN_CLICKED(IDC_RESET, &ObjectDlg::OnBnClickedReset)
	ON_EN_CHANGE(IDC_Mesh, &ObjectDlg::OnEnChangeMesh)
	ON_EN_CHANGE(IDC_MAt, &ObjectDlg::OnEnChangeMat)
	ON_LBN_SELCHANGE(IDC_MESHLIST, &ObjectDlg::OnLbnSelchangeMeshlist)
END_MESSAGE_MAP()


// ObjectDlg 메시지 처리기


void ObjectDlg::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//CDialogEx::OnOK();
}


void ObjectDlg::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//CDialogEx::OnCancel();
}


void ObjectDlg::OnBnClickedButton1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	SSDToolGlobal::ToolEditDlg->CreateMatDlg();
}

//스테틱 FBX선택
void ObjectDlg::OnTvnSelchangedFbxtree(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;

	UpdateData(TRUE);
	m_MeshList.ResetContent();

	DWORD_PTR Ptr = m_FbxTree.GetItemData(pNMTreeView->itemNew.hItem);
	TempName = ((FbxData*)Ptr);
	if (nullptr == TempName)
		return;

	

	//Static Mesh
	if (FBX_TYPE::FBXTYPE_BASE == TempName->Type)
	{
		if (nullptr == SSDToolGlobal::ToolSelectActor)
		{
			StringReset();
			return;
		}

		SPTR<CBase3DRenderer> TempRenderer = SSDToolGlobal::ToolSelectActor->FindCom<CBase3DRenderer>();
		if (nullptr == TempRenderer)
		{
			StringReset();
			return;
		}
		RENDER_MODE TempMode = TempRenderer->GetRenderMode();
		if (RENDER_MODE::RENDMODE_NORMAL != TempMode)
		{
			StringReset();
			return;
		}

		SSDToolGlobal::ToolSelectMesh = TempName->MeshName.c_str();
		SSDToolGlobal::ToolSelectMat = TempName->MatName.c_str();

		LPCTSTR MeshKey = SSDToolGlobal::ToolSelectMesh.c_str();
		LPCTSTR MatKey = SSDToolGlobal::ToolSelectMat.c_str();

		GetDlgItem(IDC_Mesh)->SetWindowTextW(MeshKey);
		GetDlgItem(IDC_MAt)->SetWindowTextW(MatKey);

		TempRenderer->PushMesh(TempName->MeshName.c_str());
		TempRenderer->PushMaterial(TempName->MatName.c_str());
	}

	//Architecture
	if (FBX_TYPE::FBXTYPE_FBX == TempName->Type)
	{
		CString AllStr = L"All";
		m_MeshList.AddString(AllStr);

		std::vector<SPTR<CFBXMesh>> TempVec = TempName->FbxPtr->GetAllFBXMeshVec();
		for (size_t i = 0; i < TempVec.size(); ++i)
		{
			CString Str;
			Str.Format(L"%d", (int)i);

			m_MeshList.AddString(Str);
		}

		//if (nullptr != SSDToolGlobal::ToolEditDlg)
		//{
		//	SSDToolGlobal::ToolEditDlg->DeleteMatDlg();
		//	SSDToolGlobal::ToolEditDlg->CreateMatDlg();
		//	SSDToolGlobal::ToolMatDlg->SetMaterialValuSe(TempName);
		//}
	}

	UpdateData(FALSE);
}

void ObjectDlg::OnBnClickedReset()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	m_FbxTree.DeleteAllItems();

	StaticItem = m_FbxTree.InsertItem(L"Static Mesh");

	HTREEITEM CubeItem = m_FbxTree.InsertItem(L"Cube", StaticItem);
	HTREEITEM Sphere_Lv8 = m_FbxTree.InsertItem(L"Sphere_Lv8", StaticItem);
	HTREEITEM Sphere_Lv16 = m_FbxTree.InsertItem(L"Sphere_Lv16", StaticItem);
	HTREEITEM Sphere_Lv32 = m_FbxTree.InsertItem(L"Sphere_Lv32", StaticItem);
	HTREEITEM Sphere_Lv64 = m_FbxTree.InsertItem(L"Sphere_Lv64", StaticItem);

	FbxData* Temp = new FbxData(FBX_TYPE::FBXTYPE_BASE, L"Cube", L"Base3D");
	DWORD_PTR TempPtr = (DWORD_PTR)Temp;
	m_FbxTree.SetItemData(CubeItem, TempPtr);
	m_FbxListDatalist.push_back(Temp);

	Temp = new FbxData(FBX_TYPE::FBXTYPE_BASE, L"Sphere_Lv8", L"Base3D");
	TempPtr = (DWORD_PTR)Temp;
	m_FbxTree.SetItemData(Sphere_Lv8, TempPtr);
	m_FbxListDatalist.push_back(Temp);

	Temp = new FbxData(FBX_TYPE::FBXTYPE_BASE, L"Sphere_Lv16", L"Base3D");
	TempPtr = (DWORD_PTR)Temp;
	m_FbxTree.SetItemData(Sphere_Lv16, TempPtr);
	m_FbxListDatalist.push_back(Temp);

	Temp = new FbxData(FBX_TYPE::FBXTYPE_BASE, L"Sphere_Lv32", L"Base3D");
	TempPtr = (DWORD_PTR)Temp;
	m_FbxTree.SetItemData(Sphere_Lv32, TempPtr);
	m_FbxListDatalist.push_back(Temp);

	Temp = new FbxData(FBX_TYPE::FBXTYPE_BASE, L"Sphere_Lv64", L"Base3D");
	TempPtr = (DWORD_PTR)Temp;
	m_FbxTree.SetItemData(Sphere_Lv64, TempPtr);
	m_FbxListDatalist.push_back(Temp);

	m_FbxTree.Expand(StaticItem, TVE_EXPAND);

	StaticFbxFinder();
	UpdateData(FALSE);
}


void ObjectDlg::OnEnChangeMesh()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void ObjectDlg::OnEnChangeMat()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void ObjectDlg::StringReset()
{
	SSDToolGlobal::ToolSelectMesh = L"";
	SSDToolGlobal::ToolSelectMat = L"";
	GetDlgItem(IDC_Mesh)->SetWindowTextW(L"");
	GetDlgItem(IDC_MAt)->SetWindowTextW(L"");
}


BOOL ObjectDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	SSDToolGlobal::ToolObjectDlg = this;

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void ObjectDlg::StaticFbxFinder()
{
	CFileFind FileFinder;
	CFilePathMgr::AddPlusFolder(L"Fbx", L"Architecture");

	CString TempPath = CFilePathMgr::GetPath(L"Architecture").c_str();
	//BOOL FileCheck = FileFinder.FindFile(TempPath + L"*.fbx");
	BOOL FileCheck = FileFinder.FindFile(TempPath + L"*.sfbx");
	HTREEITEM ArchItem;

	ArchItem = m_FbxTree.InsertItem(L"Architecture");

	while (TRUE == FileCheck)
	{
		FileCheck = FileFinder.FindNextFileW();

		if (FileFinder.IsDots())
		{
			continue;
		}
		else
		{
			std::wstring FileName = FileFinder.GetFileName();
			std::wstring PureFileName = FileFinder.GetFileName();

			size_t Last_Index = FileName.find_last_of(L".");
			size_t Count = FileName.size() - Last_Index;
			PureFileName.replace(Last_Index, Count, L"");


			SPTR<CFbx> Ptr = nullptr;
			//Ptr = CResourceMgr<CFbx>::Load(CFilePathMgr::GetPath(L"Architecture", FileName.c_str()), CFbx::LOAD_DESC{ FBX_STATIC });
			Ptr = CResourceMgr<CFbx>::Load(PureFileName.c_str(), CFilePathMgr::GetPath(L"Architecture", FileName.c_str()), CFbx::LOAD_DESC{ FBX_STATIC, L"" });
			//Ptr = CResourceMgr<CFbx>::Load(PureFileName.c_str(), CFilePathMgr::GetPath(L"Architecture", FileName.c_str()), CFbx::LOAD_DESC{ FBX_STATIC_NONAUTO, L"" });

			HTREEITEM Item = m_FbxTree.InsertItem(PureFileName.c_str(), ArchItem);

			FbxData* Temp = new FbxData();
			Temp->FbxPtr = Ptr;
			Temp->Type = FBX_TYPE::FBXTYPE_FBX;
			m_FbxListDatalist.push_back(Temp);

			DWORD_PTR PTR = (DWORD_PTR)Temp;
			m_FbxTree.SetItemData(Item, PTR);
		}
	}
	m_FbxTree.Expand(ArchItem, TVE_EXPAND);
}

//글로벌 엑터가 있으면 - fbx랜더러를 가지고 있으면 - 매쉬 바뀜
//글로벌 엑터가 있으면 - fbx랜더러를 안가지고있으면 - 리턴

//글로벌 엑터가 없으면 - 크리에이트 엑터를 하면 fbx랜더러 넣어서 엑터 생성 - 매쉬 바뀜
//글로벌 엑터가 없으면 - 리턴

//서브셋 선택
void ObjectDlg::OnLbnSelchangeMeshlist()
{ 
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString TempString;
	m_MeshList.GetText(m_MeshList.GetCurSel(), TempString);

	int TempIndex = _ttoi(TempString);

	//글로벌 엑터 없으면
	if (nullptr != SSDToolGlobal::ToolSelectActor)
	{
		if (nullptr != SSDToolGlobal::ToolEditDlg)
		{
			SSDToolGlobal::ToolEditDlg->DeleteMatDlg();
			SSDToolGlobal::ToolEditDlg->CreateMatDlg();
			SSDToolGlobal::ToolMatDlg->SetMaterialValue(TempName, TempIndex);
		}

		if (nullptr == SSDToolGlobal::ToolSelectRenderer)
		{
			return;
		}
		CFbxRenderer* CheckStaticRender = SSDToolGlobal::ToolSelectActor->FindCom<CFbxRenderer>();
		if (nullptr == CheckStaticRender)
		{
			return;
		}
		if (nullptr == TempName)
		{
			return;
		}
		if (nullptr == TempName->FbxPtr)
		{
			return;
		}

		//가지고있으면
		SSDToolGlobal::ToolSelectMesh = TempString;
		GetDlgItem(IDC_Mesh)->SetWindowTextW(TempString);

		CFbxRenderer* TempStaticRender = (CFbxRenderer*)SSDToolGlobal::ToolSelectRenderer;
		TempStaticRender->SetFbx(TempName->FbxPtr->GetFileName().c_str());
		if (L"All" == TempString)
		{
			TempStaticRender->SetAllFbxMesh();
		}
		else 
		{
			TempStaticRender->SettingFbxMesh(TempIndex);
		}

		if (nullptr == SSDToolGlobal::ToolSceneList)
		{
			return;
		}
		SSDToolGlobal::ToolSceneList->OnBnClickedReset();
	}
}

void ObjectDlg::SetMeshList()
{
	UpdateData(TRUE);

	m_MeshList.ResetContent();
	CFbxRenderer* TempStaticRender = SSDToolGlobal::ToolSelectActor->FindCom<CFbxRenderer>();
	if (nullptr == TempStaticRender)
	{
		return;
	}
	CFBXMesh* TempMesh = (CFBXMesh*)TempStaticRender->GetMesh(0).PTR;

	if (nullptr == TempMesh)
	{
		return;	
	}

	CString TempStr;
	TempStr.Format(L"%d", (int)TempMesh->GetMeshIdx());
	GetDlgItem(IDC_Mesh)->SetWindowTextW(TempStr);
	TempStr.Empty();

	std::list<SPTR<CMaterial>> TempList = TempMesh->GetMatList(0);
	std::list<SPTR<CMaterial>>::iterator StartIter = TempList.begin();
	TempStr = (*StartIter)->GetPath().c_str();
	GetDlgItem(IDC_MAt)->SetWindowTextW(TempStr);

	std::vector<SPTR<CFBXMesh>> TempVec = TempStaticRender->GetRenderingFbx()->GetAllFBXMeshVec();
	for (size_t i = 0; i < TempVec.size(); ++i)
	{
		CString Str;
		Str.Format(L"%d", (int)i);
		m_MeshList.AddString(Str);
	}
	UpdateData(FALSE);
}

