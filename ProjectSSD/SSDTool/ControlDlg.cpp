// ControlDlg.cpp: 구현 파일
//

#include "stdafx.h"
#include "SSDTool.h"
#include "ControlDlg.h"
#include "afxdialogex.h"

#include "SceneListDlg.h"
#include <Material.h>
#include <TransForm.h>
#include <FbxRenderer.h>
#include <SkeletalFbxRenderer.h>
#include <Base3DRenderer.h>
#include <OBBCollider.h>
#include <SphereCollider.h>
#include <WriteStream.h>
#include <ReadStream.h>
#include <Actor.h>
#include <Light.h>
#include <GaussianBlur.h>
#include <BloomFilter.h>
#include <Skybox.h>
#include <Terrain.h>
#include <TerrainRenderer.h>
#include "ObjectDlg.h"
#include <Gizmo.h>

#include "EditDlg.h"
#include "LightDlg.h"
#include "NaviDlg.h"
#include "ToolMainScene.h"
#include <NavigationObject.h>
#include <NavigationMesh.h>
#include <DeadByDaylightGlobal.h>
#include "FogDlg.h"
#include <MagicCircle.h>
#include <DeadByDaylightGlobal.h>

// ControlDlg 대화 상자
#define DEFCOLGROUP L"Default"



IMPLEMENT_DYNAMIC(ControlDlg, CDialogEx)

ControlDlg::ControlDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CONTROLDLG, pParent)
	, m_ForwardValue(FALSE)
	, m_ActorOnOff(FALSE)
{

}

ControlDlg::~ControlDlg()
{
	//std::map<std::wstring, PrefabData*>::iterator StartIter = m_PreFabDataMap.begin();
	//std::map<std::wstring, PrefabData*>::iterator EndIter = m_PreFabDataMap.end();
	//for (; StartIter != EndIter; ++StartIter)
	//{
	//	if (nullptr != StartIter->second)
	//	{
	//		delete StartIter->second;
	//		StartIter->second = nullptr;
	//	}
	//}
	//m_PreFabDataMap.clear();
	if (nullptr != SSDToolGlobal::ToolNaviDlg)
	{
		SSDToolGlobal::ToolNaviDlg->DestroyWindow();
		delete SSDToolGlobal::ToolNaviDlg;
	}
	if (nullptr != SSDToolGlobal::ToolFogDlg)
	{
		SSDToolGlobal::ToolFogDlg->DestroyWindow();
		delete SSDToolGlobal::ToolFogDlg;
	}

}

void ControlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PRELIST, m_PreList);
	DDX_Control(pDX, IDC_COLLGROUP, m_CollGroupName);
	DDX_Control(pDX, IDC_PREFABNAME, m_PreFabName);
	DDX_Control(pDX, IDC_DATANAME, m_FileName);
	DDX_Radio(pDX, IDC_RADIO1, m_ForwardValue);
	DDX_Radio(pDX, IDC_ON, m_ActorOnOff);
	DDX_Control(pDX, IDC_PLAYBUTTON1, m_PlayButton);
	DDX_Control(pDX, IDC_PAUSEBUTTON1, m_PauseButton);
	DDX_Control(pDX, IDC_STOPBUTTON1, m_StopButton);
}


BEGIN_MESSAGE_MAP(ControlDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &ControlDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &ControlDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_ACTORDELETE, &ControlDlg::OnBnClickedDelete)
	ON_BN_CLICKED(IDC_ACTORCREATE, &ControlDlg::OnBnClickedActorcreate)
	ON_BN_CLICKED(IDC_FBXRENDER, &ControlDlg::OnBnClickedFbxrender)
	ON_BN_CLICKED(IDC_SKELRENDER, &ControlDlg::OnBnClickedSkelrender)
	ON_BN_CLICKED(IDC_BASERENDER, &ControlDlg::OnBnClickedBaserender)
	ON_BN_CLICKED(IDC_OBBCOLL, &ControlDlg::OnBnClickedObbcoll)
	ON_BN_CLICKED(IDC_SPHERECOLL, &ControlDlg::OnBnClickedSpherecoll)
	ON_BN_CLICKED(IDC_PRECREATE, &ControlDlg::OnBnClickedPrecreate)
	ON_BN_CLICKED(IDC_PREDELETE, &ControlDlg::OnBnClickedPredelete)
	ON_LBN_SELCHANGE(IDC_PRELIST, &ControlDlg::OnLbnSelchangePrelist)
	ON_EN_CHANGE(IDC_COLLGROUP, &ControlDlg::OnEnChangeCollgroup)
	ON_BN_CLICKED(IDC_RENDDELETE, &ControlDlg::OnBnClickedRenddelete)
	ON_BN_CLICKED(IDC_COLLDELETE, &ControlDlg::OnBnClickedColldelete)
	ON_EN_CHANGE(IDC_PREFABNAME, &ControlDlg::OnEnChangePrefabname)
	ON_BN_CLICKED(IDC_BINARTSAVE, &ControlDlg::OnBnClickedBinartsave)
	ON_EN_CHANGE(IDC_DATANAME, &ControlDlg::OnEnChangeDataname)
	ON_BN_CLICKED(IDC_BINARYLOAD, &ControlDlg::OnBnClickedBinaryload)
	ON_BN_CLICKED(IDC_RADIO1, &ControlDlg::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, &ControlDlg::OnBnClickedRadio2)
	ON_BN_CLICKED(IDC_LIGHT, &ControlDlg::OnBnClickedLight)
	ON_BN_CLICKED(IDC_FILTER, &ControlDlg::OnBnClickedFilter)
	ON_BN_CLICKED(IDC_VOLUMELIGHT, &ControlDlg::OnBnClickedVolumelight)
	ON_BN_CLICKED(IDC_BLOOM, &ControlDlg::OnBnClickedBloom)
	ON_BN_CLICKED(IDC_GAU, &ControlDlg::OnBnClickedGau)
	ON_BN_CLICKED(IDC_ON, &ControlDlg::OnBnClickedOn)
	ON_BN_CLICKED(IDC_OFF, &ControlDlg::OnBnClickedOff)
	ON_BN_CLICKED(IDC_SCENESAVE, &ControlDlg::OnBnClickedScenesave)
	ON_BN_CLICKED(IDC_SCENELOAD, &ControlDlg::OnBnClickedSceneload)
	ON_BN_CLICKED(IDC_CAMSAVE, &ControlDlg::OnBnClickedCamsave)
	ON_BN_CLICKED(IDC_CAMLOAD, &ControlDlg::OnBnClickedCamload)
	ON_BN_CLICKED(IDC_COLLMOVE, &ControlDlg::OnBnClickedCollmove)
	ON_BN_CLICKED(IDC_GIZMO, &ControlDlg::OnBnClickedGizmo)
	ON_BN_CLICKED(IDC_GIZMO2, &ControlDlg::OnBnClickedGizmo2)
	ON_BN_CLICKED(IDC_NAVI, &ControlDlg::OnBnClickedNavi)
	ON_BN_CLICKED(IDC_NAVIDELETE, &ControlDlg::OnBnClickedNavidelete)
	ON_BN_CLICKED(IDC_PLAYBUTTON1, &ControlDlg::OnBnClickedPlaybutton1)
	ON_BN_CLICKED(IDC_PAUSEBUTTON1, &ControlDlg::OnBnClickedPausebutton1)
	ON_BN_CLICKED(IDC_STOPBUTTON1, &ControlDlg::OnBnClickedStopbutton1)
	ON_BN_CLICKED(IDC_FOG, &ControlDlg::OnBnClickedFog)
	ON_BN_CLICKED(IDC_ADDCOLL, &ControlDlg::OnBnClickedAddcoll)
	ON_BN_CLICKED(IDC_NEWSCENELOAD, &ControlDlg::OnBnClickedNewsceneload)
	ON_BN_CLICKED(IDC_MAGIC, &ControlDlg::OnBnClickedMagic)
	ON_BN_CLICKED(IDC_GREEN, &ControlDlg::OnBnClickedGreen)
	ON_BN_CLICKED(IDC_CHANGE, &ControlDlg::OnBnClickedChange)
END_MESSAGE_MAP()


// ControlDlg 메시지 처리기
BOOL ControlDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SSDToolGlobal::ToolControlDlg = this;

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	GetDlgItem(IDC_COLLGROUP)->SetWindowTextW(DEFCOLGROUP);



	HRESULT hRes = m_PlayImage[IDLE_BUTTON].Load(CFilePathMgr::GetPath(L"Texture", L"IdlePlay.png").c_str());

	if (hRes != S_OK)
		return NULL;

	hRes = m_PlayImage[FOCUS_BUTTON].Load(CFilePathMgr::GetPath(L"Texture", L"FocusPlay.png").c_str());
	if (hRes != S_OK)
		return NULL;

	hRes = m_PlayImage[PUSH_BUTTON].Load(CFilePathMgr::GetPath(L"Texture", L"PushPlay.png").c_str());
	if (hRes != S_OK)
		return NULL;

	m_PlayButton.m_nFlatStyle = CMFCButton::BUTTONSTYLE_NOBORDERS;
	m_PlayButton.m_bTransparent = TRUE;    // 투명도 설정
	m_PlayButton.m_bDrawFocus = FALSE;

	m_PlayButton.SetImage(m_PlayImage[IDLE_BUTTON], TRUE, m_PlayImage[FOCUS_BUTTON].Detach());
	m_PlayButton.SetWindowText(_T(""));
	m_PlayButton.SetMouseCursorHand();


	hRes = m_PauseImage[IDLE_BUTTON].Load(CFilePathMgr::GetPath(L"Texture", L"IdlePause.png").c_str());

	if (hRes != S_OK)
		return NULL;

	hRes = m_PauseImage[FOCUS_BUTTON].Load(CFilePathMgr::GetPath(L"Texture", L"FocusPause.png").c_str());

	if (hRes != S_OK)
		return NULL;

	hRes = m_PauseImage[PUSH_BUTTON].Load(CFilePathMgr::GetPath(L"Texture", L"PushPause.png").c_str());


	if (hRes != S_OK)
		return NULL;

	m_PauseButton.m_nFlatStyle = CMFCButton::BUTTONSTYLE_NOBORDERS;
	m_PauseButton.m_bTransparent = TRUE;    // 투명도 설정
	m_PauseButton.m_bDrawFocus = FALSE;

	m_PauseButton.SetImage(m_PauseImage[IDLE_BUTTON], TRUE, m_PauseImage[FOCUS_BUTTON].Detach(), 0, m_PauseImage[PUSH_BUTTON]);
	m_PauseButton.SetMouseCursorHand();


	hRes = m_StopImage[IDLE_BUTTON].Load(CFilePathMgr::GetPath(L"Texture", L"IdleStop.png").c_str());

	if (hRes != S_OK)
		return NULL;

	hRes = m_StopImage[FOCUS_BUTTON].Load(CFilePathMgr::GetPath(L"Texture", L"FocusStop.png").c_str());

	if (hRes != S_OK)
		return NULL;


	hRes = m_StopImage[PUSH_BUTTON].Load(CFilePathMgr::GetPath(L"Texture", L"PushStop.png").c_str());

	if (hRes != S_OK)
		return NULL;

	m_StopButton.m_nFlatStyle = CMFCButton::BUTTONSTYLE_NOBORDERS;
	m_StopButton.m_bTransparent = TRUE;    // 투명도 설정
	m_StopButton.m_bDrawFocus = FALSE;

	m_StopButton.SetImage(m_StopImage[IDLE_BUTTON].Detach(), TRUE, m_StopImage[FOCUS_BUTTON].Detach());
	m_StopButton.SetMouseCursorHand();


	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}



void ControlDlg::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//CDialogEx::OnOK();
}


void ControlDlg::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//CDialogEx::OnCancel();
}


void ControlDlg::OnBnClickedDelete()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (nullptr == SSDToolGlobal::ToolSelectActor)
	{
		return;
	}

	if (nullptr == SSDToolGlobal::ToolSceneList)
	{
		return;
	}

	SSDToolGlobal::ToolSelectActor->Death();
	SSDToolGlobal::ToolSelectActor = nullptr;
	SSDToolGlobal::ToolSceneList->OnBnClickedReset();
}


void ControlDlg::OnBnClickedActorcreate()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (nullptr == SSDToolGlobal::ToolSceneList)
	{
		return;
	}
	//SSDToolGlobal::ToolSelectActor = nullptr;
	NewActor = SSDToolGlobal::ToolGlobalScene->CreateActor(SSDToolGlobal::ToolGlobalScene, UPDATE_LAYER_NORMAL, L"NewActor");
	SSDToolGlobal::SetSelectActor(NewActor);
	SSDToolGlobal::ToolSceneList->OnBnClickedReset();
}

void ControlDlg::OnBnClickedFbxrender()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (nullptr == SSDToolGlobal::ToolSelectActor)
	{
		return;
	}
	if (nullptr == SSDToolGlobal::ToolSceneList)
	{
		return;
	}
	SPTR<CStaticFbxRenderer> FbxRen = SSDToolGlobal::ToolSelectActor->AddCom<CStaticFbxRenderer>(RENDATA{ RENDER_LAYER_PLAYER });
	SSDToolGlobal::ToolSceneList->OnBnClickedReset();
}

void ControlDlg::OnBnClickedSkelrender()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (nullptr == SSDToolGlobal::ToolSkelRender)
	{
		return;
	}
	if (nullptr == SSDToolGlobal::ToolSelectActor)
	{
		return;
	}
	if (nullptr == SSDToolGlobal::ToolSceneList)
	{
		return;
	}
	SPTR<CSkeletalFbxRenderer> FbxRen = SSDToolGlobal::ToolSelectActor->AddCom<CSkeletalFbxRenderer>(RENDATA{ RENDER_LAYER_PLAYER });
	SSDToolGlobal::ToolSceneList->OnBnClickedReset();
}

void ControlDlg::OnBnClickedBaserender()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (nullptr == SSDToolGlobal::ToolSelectActor)
	{
		return;
	}
	if (nullptr == SSDToolGlobal::ToolSceneList)
	{
		return;
	}
	SPTR<CBase3DRenderer> BaseRen = SSDToolGlobal::ToolSelectActor->AddCom<CBase3DRenderer>(RENDATA{ RENDER_LAYER_PLAYER });
	SSDToolGlobal::ToolSceneList->OnBnClickedReset();
}

void ControlDlg::OnBnClickedRenddelete()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (nullptr == SSDToolGlobal::ToolSelectRenderer)
	{
		return;
	}
	if (nullptr == SSDToolGlobal::ToolSceneList)
	{
		return;
	}
	SSDToolGlobal::ToolSelectRenderer->Death();
	SSDToolGlobal::ToolSelectRenderer = nullptr;
	SSDToolGlobal::ToolSceneList->OnBnClickedReset();
}

void ControlDlg::OnBnClickedObbcoll()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (nullptr == SSDToolGlobal::ToolSelectActor)
	{
		return;
	}
	if (nullptr == SSDToolGlobal::ToolSceneList)
	{
		return;
	}
	CString TempString;
	m_CollGroupName.GetWindowTextW(TempString);
	if (L"" == TempString)
	{
		return;
	}
	SPTR<COBBCollider> SphereColl = SSDToolGlobal::ToolSelectActor->AddCom<COBBCollider>(COLDATA{ TempString, COLLISION_3D });
	SSDToolGlobal::ToolSceneList->OnBnClickedReset();
}

void ControlDlg::OnBnClickedSpherecoll()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (nullptr == SSDToolGlobal::ToolSelectActor)
	{
		return;
	}
	if (nullptr == SSDToolGlobal::ToolSceneList)
	{
		return;
	}
	CString TempString;
	m_CollGroupName.GetWindowTextW(TempString);
	if (L"" == TempString)
	{
		return;
	}
	SPTR<CSphereCollider> SphereColl = SSDToolGlobal::ToolSelectActor->AddCom<CSphereCollider>(COLDATA{ TempString, COLLISION_3D });
	SSDToolGlobal::ToolSceneList->OnBnClickedReset();
}

void ControlDlg::OnBnClickedColldelete()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (nullptr == SSDToolGlobal::ToolSelectCollider)
	{
		return;
	}
	if (nullptr == SSDToolGlobal::ToolSceneList)
	{
		return;
	}
	SSDToolGlobal::ToolSelectCollider->Death();
	SSDToolGlobal::ToolSelectCollider = nullptr;
	SSDToolGlobal::ToolSceneList->OnBnClickedReset();
}

void ControlDlg::OnEnChangeCollgroup()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.
	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.

}

///////////////////////////////////////////////Prefab

void ControlDlg::OnBnClickedPrecreate()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString TempName;
	m_PreFabName.GetWindowTextW(TempName);
	if (TRUE == TempName.IsEmpty())
	{
		return;
	}
	if (nullptr == SSDToolGlobal::ToolSelectActor)
	{
		return;
	}

	//저장할 데이터 만들기 - 엑터 트렌스폼 넣어주면서 생성
	PrefabData* NewData = new PrefabData(
		SSDToolGlobal::ToolSelectActor->TRANS->GetScale(),
		SSDToolGlobal::ToolSelectActor->GetWorldPos(),
		SSDToolGlobal::ToolSelectActor->TRANS->GetRotate());

	//fbx랜더러 정보
	SSDToolGlobal::ToolSelectActor->FindAllCom<CFbxRenderer>(&m_FbxList);
	if (0 != m_FbxList.size())
	{
		std::list<SPTR<CFbxRenderer>>::iterator RenStartIter = m_FbxList.begin();
		std::list<SPTR<CFbxRenderer>>::iterator RenEndIter = m_FbxList.end();
		for (; RenStartIter != RenEndIter; ++RenStartIter)
		{
			PrefabData::FbxSaveData TempData = PrefabData::FbxSaveData();
			const std::vector<SPTR<CFBXMesh>> TempFbxMeshVec = (*RenStartIter)->GetRenderingFbx()->GetAllFBXMeshVec();
			const std::vector<SPTR<CMesh>> TempAllMeshVec = (*RenStartIter)->GetAllMeshVec();
			if (0 < TempFbxMeshVec.size())
			{
				if (TempFbxMeshVec.size() == TempAllMeshVec.size())
				{
					TempData.AllCheck = true;
				}
				else
				{
					TempData.AllCheck = false;
				}

				//매쉬 이름 넣기
				TempData.MeshName = (*RenStartIter)->GetRenderingFbx()->GetFileName();
				//인덱스 넣기
				for (int i = 0; i < TempAllMeshVec.size(); ++i)
				{
					CFBXMesh* Test = (CFBXMesh*)TempAllMeshVec[i].PTR;
					UINT Index = Test->GetMeshIdx();
					TempData.MatInfoMap.insert(std::map<int, std::wstring>::value_type(Index, Test->GetMatStart(0)->GetPath()));
				}

				//랜더러 사이즈 포스 넣기
				TempData.RenderSize = (*RenStartIter)->GetRenSize();
				TempData.RenderPos = (*RenStartIter)->GetRenPivot();
				TempData.IsDeferred = (*RenStartIter)->GetFbxMesh(0)->GetMatStart(0)->IsDeferred();

				NewData->vecFbxData.push_back(TempData);
			}
		}
	}


	//Obb충돌체 정보 
	SSDToolGlobal::ToolSelectActor->FindAllCom<COBBCollider>(&m_ObbList);
	if (0 < m_ObbList.size())
	{
		std::list<SPTR<COBBCollider>>::iterator ColStartIter = m_ObbList.begin();
		std::list<SPTR<COBBCollider>>::iterator ColEndIter = m_ObbList.end();
		for (; ColStartIter != ColEndIter; ++ColStartIter)
		{
			if (true != (*ColStartIter)->ExistTag(NOSAVE))
			{
				PrefabData::ColSaveData TempData = PrefabData::ColSaveData(COL_TYPE::COLTYPE_OBB);
				TempData.ColGroupName = (*ColStartIter)->GetChannel().c_str();
				TempData.ColPos = (*ColStartIter)->GetColliderPivot();
				TempData.ColSize = (*ColStartIter)->GetColliderSize();

				NewData->vecColData.push_back(TempData);
			}
		}
	}

	//Sphere 충돌체 정보
	SSDToolGlobal::ToolSelectActor->FindAllCom<CSphereCollider>(&m_SphereList);
	if (0 < m_SphereList.size())
	{
		std::list<SPTR<CSphereCollider>>::iterator SColStartIter = m_SphereList.begin();
		std::list<SPTR<CSphereCollider>>::iterator SColEndIter = m_SphereList.end();
		for (; SColStartIter != SColEndIter; ++SColStartIter)
		{
			PrefabData::ColSaveData TempData = PrefabData::ColSaveData(COL_TYPE::COLTYPE_SPHERE);
			TempData.ColGroupName = (*SColStartIter)->GetChannel().c_str();
			TempData.ColPos = (*SColStartIter)->GetColliderPivot();
			TempData.ColSize = (*SColStartIter)->GetColliderSize();

			NewData->vecColData.push_back(TempData);
		}
	}
	m_PreFabDataMap.insert(std::map<std::wstring, SPTR<PrefabData>>::value_type(TempName, NewData));

	m_FbxList.clear();
	m_ObbList.clear();
	m_SphereList.clear();

	SetPrefabData();
}


void ControlDlg::OnBnClickedPredelete()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString TempString;
	m_PreList.GetText(m_PreList.GetCurSel(), TempString);

	std::map<std::wstring, SPTR<PrefabData>>::iterator FindIter = m_PreFabDataMap.find(TempString.GetString());
	if (FindIter != m_PreFabDataMap.end())
	{
		if (nullptr != FindIter->second)
		{
			m_PreFabDataMap.erase(FindIter);
		}
	}
	SetPrefabData();

	//if (0 == m_PreFabDataMap.size())
	//{
	//	return;
	//}
	//m_PreFabDataMap.clear();
}


void ControlDlg::OnLbnSelchangePrelist()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString TempString;
	m_PreList.GetText(m_PreList.GetCurSel(), TempString);

	GetDlgItem(IDC_DATANAME)->SetWindowTextW(TempString);

	if (nullptr == SSDToolGlobal::ToolSelectActor)
	{
		return;
	}
	SPTR<CFbxRenderer> FbxCom = SSDToolGlobal::ToolSelectActor->FindCom<CFbxRenderer>();
	if (nullptr != FbxCom)
	{
		return;
	}
	SPTR<COBBCollider> ObbCom = SSDToolGlobal::ToolSelectActor->FindCom<COBBCollider>();
	if (nullptr != ObbCom)
	{
		return;
	}
	SPTR<CSphereCollider> SphereCom = SSDToolGlobal::ToolSelectActor->FindCom<CSphereCollider>();
	if (nullptr != SphereCom)
	{
		return;
	}

	std::map<std::wstring, SPTR<PrefabData>>::iterator FindIter = m_PreFabDataMap.find(TempString.GetString());
	if (FindIter != m_PreFabDataMap.end())
	{
		if (m_PreFabDataMap.end() == FindIter)
		{
			return;
		}
		SSDToolGlobal::ToolSelectActor->SetPos(FindIter->second->ActorPos);
		SSDToolGlobal::ToolSelectActor->SetSize(FindIter->second->ActorSize);
		SSDToolGlobal::ToolSelectActor->TRANS->SetRotate(FindIter->second->ActorRot);

		//랜더러
		std::vector<PrefabData::FbxSaveData> FbxVector = FindIter->second->vecFbxData;
		for (size_t i = 0; i < FbxVector.size(); ++i)
		{
			SPTR<CFbxRenderer> TempRender = SSDToolGlobal::ToolSelectActor->AddCom<CFbxRenderer>(RENDATA{ RENDER_LAYER_PLAYER });
			TempRender->SetRenSize(FbxVector[i].RenderSize);
			TempRender->SetRenPivot(FbxVector[i].RenderPos);

			TempRender->SetFbx(FbxVector[i].MeshName.c_str());
			if (true == FbxVector[i].AllCheck)
			{
				TempRender->SetAllFbxMesh();
			}
			else if (false == FbxVector[i].AllCheck)
			{
				std::map<int, std::wstring>::iterator StartIter = FbxVector[i].MatInfoMap.begin();
				std::map<int, std::wstring>::iterator EndIter = FbxVector[i].MatInfoMap.end();
				for (; StartIter != EndIter; ++StartIter)
				{
					TempRender->SettingFbxMesh(StartIter->first);
				}
			}
		}

		//Obb콜리전
		std::vector<PrefabData::ColSaveData> ColSaveVector = FindIter->second->vecColData;
		for (size_t j = 0; j < ColSaveVector.size(); ++j)
		{
			if (COL_TYPE::COLTYPE_OBB == FindIter->second->vecColData[j].ColType)
			{
				SPTR<COBBCollider> TempColl = SSDToolGlobal::ToolSelectActor->
					AddCom<COBBCollider>(COLDATA{ FindIter->second->vecColData[j].ColGroupName, COLLISION_3D });
				TempColl->SetColSize(FindIter->second->vecColData[j].ColSize);
				TempColl->SetColPivot(FindIter->second->vecColData[j].ColPos);
			}
			else if (COL_TYPE::COLTYPE_SPHERE == FindIter->second->vecColData[j].ColType)
			{
				SPTR<CSphereCollider> TempColl = SSDToolGlobal::ToolSelectActor->
					AddCom<CSphereCollider>(COLDATA{ FindIter->second->vecColData[j].ColGroupName, COLLISION_3D });
				TempColl->SetColSize(FindIter->second->vecColData[j].ColSize);
				TempColl->SetColPivot(FindIter->second->vecColData[j].ColPos);
			}
		}
	}

	if (nullptr == SSDToolGlobal::ToolSceneList)
	{
		return;
	}
	SSDToolGlobal::ToolSceneList->OnBnClickedReset();
}

void ControlDlg::OnEnChangePrefabname()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void ControlDlg::SetPrefabData()
{
	UpdateData(TRUE);
	m_PreList.ResetContent();

	std::map<std::wstring, SPTR<PrefabData>>::iterator StartIter = m_PreFabDataMap.begin();
	std::map<std::wstring, SPTR<PrefabData>>::iterator EndIter = m_PreFabDataMap.end();
	for (; StartIter != EndIter; ++StartIter)
	{
		CString Str = StartIter->first.c_str();
		m_PreList.AddString(Str);
	}
	if (nullptr == SSDToolGlobal::ToolSceneList)
	{
		return;
	}
	SSDToolGlobal::ToolSceneList->OnBnClickedReset();

	UpdateData(FALSE);
}

void ControlDlg::SavePrefabData(const wchar_t* _Path)
{
	CWriteStream WriteData = CWriteStream(_Path);

	int MapSize = (int)m_PreFabDataMap.size();
	WriteData.Write(MapSize);

	std::map<std::wstring, SPTR<PrefabData>>::iterator StartIter = m_PreFabDataMap.begin();
	std::map<std::wstring, SPTR<PrefabData>>::iterator EndIter = m_PreFabDataMap.end();
	for (; StartIter != EndIter; ++StartIter)
	{
		WriteData.Write(StartIter->first);
		WriteData.Write(StartIter->second->ActorPos);
		WriteData.Write(StartIter->second->ActorSize);
		WriteData.Write(StartIter->second->ActorRot);

		int FbxVecSize = (int)StartIter->second->vecFbxData.size();
		WriteData.Write(FbxVecSize);

		for (size_t i = 0; i < StartIter->second->vecFbxData.size(); ++i)
		{
			WriteData.Write(StartIter->second->vecFbxData[i].AllCheck);
			WriteData.Write(StartIter->second->vecFbxData[i].MeshName);
			WriteData.Write(StartIter->second->vecFbxData[i].RenderPos);
			WriteData.Write(StartIter->second->vecFbxData[i].RenderSize);

			int MatMapSize = (int)StartIter->second->vecFbxData[i].MatInfoMap.size();
			WriteData.Write(MatMapSize);

			std::map<int, std::wstring>::iterator MatStartIter = StartIter->second->vecFbxData[i].MatInfoMap.begin();
			std::map<int, std::wstring>::iterator MatEndIter = StartIter->second->vecFbxData[i].MatInfoMap.end();
			for (; MatStartIter != MatEndIter; ++MatStartIter)
			{
				int Temp = MatStartIter->first;
				WriteData.Write(Temp);
				WriteData.Write(MatStartIter->second);
			}
		}

		int ColVecSize = (int)StartIter->second->vecColData.size();
		WriteData.Write(ColVecSize);

		for (size_t j = 0; j < StartIter->second->vecColData.size(); ++j)
		{
			WriteData.Write(StartIter->second->vecColData[j].ColType);
			WriteData.Write(StartIter->second->vecColData[j].ColGroupName);
			WriteData.Write(StartIter->second->vecColData[j].ColPos);
			WriteData.Write(StartIter->second->vecColData[j].ColSize);
		}
	}
}

void ControlDlg::ReadPrefabData()
{
	CFileFind FileFinder;
	CString TempPath = CFilePathMgr::GetPath(L"Data").c_str();
	BOOL FileCheck = FileFinder.FindFile(TempPath + L"*.predata");

	while (TRUE == FileCheck)
	{
		FileCheck = FileFinder.FindNextFileW();

		if (FileFinder.IsDots())
		{
			continue;
		}
		else
		{
			std::wstring FileName = FileFinder.GetFilePath();
			CReadStream ReadData = CReadStream(FileName.c_str());

			int MapSize;
			ReadData.Read(MapSize);

			for (int i = 0; i < MapSize; ++i)
			{
				std::wstring First;
				ReadData.Read(First);
				Vec3 Pos;
				ReadData.Read(Pos);
				Vec3 Size;
				ReadData.Read(Size);
				Vec3 Rot;
				ReadData.Read(Rot);

				SPTR<PrefabData> NewData = new PrefabData(Size, Pos, Rot);
				//NewData->ActorPos = Pos;
				//NewData->ActorSize = Size;
				//NewData->ActorRot = Rot;

				int FbxVecSize;
				ReadData.Read(FbxVecSize);

				NewData->vecFbxData.resize(FbxVecSize);
				for (size_t j = 0; j < NewData->vecFbxData.size(); ++j)
				{
					bool AllCheck;
					ReadData.Read(AllCheck);
					std::wstring MeshName;
					ReadData.Read(MeshName);
					Vec3 RenderPos;
					ReadData.Read(RenderPos);
					Vec3 RenderSize;
					ReadData.Read(RenderSize);

					NewData->vecFbxData[j].AllCheck = AllCheck;
					NewData->vecFbxData[j].MeshName = MeshName;
					NewData->vecFbxData[j].RenderPos = RenderPos;
					NewData->vecFbxData[j].RenderSize = RenderSize;

					int MatMapSize;
					ReadData.Read(MatMapSize);
					for (size_t k = 0; k < MatMapSize; ++k)
					{
						int Index;
						ReadData.Read(Index);
						std::wstring MatName;
						ReadData.Read(MatName);

						NewData->vecFbxData[j].MatInfoMap.insert(std::map<int, std::wstring>::value_type(Index, MatName));
					}
				}

				int ColVecSize;
				ReadData.Read(ColVecSize);

				NewData->vecColData.resize(ColVecSize);
				for (size_t k = 0; k < ColVecSize; ++k)
				{
					COL_TYPE Type;
					ReadData.Read(Type);
					std::wstring GroupName;
					ReadData.Read(GroupName);
					Vec3 ColPos;
					ReadData.Read(ColPos);
					Vec3 ColSize;
					ReadData.Read(ColSize);

					NewData->vecColData[k].ColType = Type;
					NewData->vecColData[k].ColGroupName = GroupName;
					NewData->vecColData[k].ColPos = ColPos;
					NewData->vecColData[k].ColSize = ColSize;
				}
				m_PreFabDataMap.insert(std::map<std::wstring, SPTR<PrefabData>>::value_type(First, NewData));
			}
		}
	}
	SetPrefabData();
}


void ControlDlg::OnBnClickedBinartsave()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString TempName;
	m_FileName.GetWindowTextW(TempName);
	if (TRUE == TempName.IsEmpty())
	{
		return;
	}
	SavePrefabData((CFilePathMgr::GetPath(L"Data") + TempName.GetString() + L".predata").c_str());
}


void ControlDlg::OnEnChangeDataname()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void ControlDlg::OnBnClickedBinaryload()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (nullptr != SSDToolGlobal::ToolObjectDlg)
	{
		SSDToolGlobal::ToolObjectDlg->OnBnClickedReset();
	}
	ReadPrefabData();
}


void ControlDlg::OnBnClickedRadio1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void ControlDlg::OnBnClickedRadio2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void ControlDlg::OnBnClickedLight()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (nullptr == SSDToolGlobal::ToolSelectActor)
	{
		return;
	}
	if (nullptr == SSDToolGlobal::ToolSceneList)
	{
		return;
	}

	SSDToolGlobal::ToolEditDlg->DeleteLightDlg();
	SSDToolGlobal::ToolEditDlg->CreateLightDlg();
	SPTR<CLight> DLight = SSDToolGlobal::ToolSelectActor->AddCom<CLight>(LIGHT_DATA{ RENDER_LAYER_PLAYER, DIRECTION_LIGHT, false });
	DLight->SetLightColor(Vec4::White);
	DLight->SetLightDir(Vec3(1.f, 1.f, 1.f));
	DLight->SetLightPower(1.f);
	DLight->SetSpecPower(40.f);

	SSDToolGlobal::ToolSelectLight = DLight;

	SSDToolGlobal::ToolSceneList->OnBnClickedReset();
	SSDToolGlobal::ToolLightDlg->SetNewLightInfo(false);
}


void ControlDlg::OnBnClickedFilter()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

}


void ControlDlg::OnBnClickedVolumelight()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (nullptr == SSDToolGlobal::ToolSelectActor)
	{
		return;
	}
	if (nullptr == SSDToolGlobal::ToolSceneList)
	{
		return;
	}

	SSDToolGlobal::ToolEditDlg->DeleteLightDlg();
	SSDToolGlobal::ToolEditDlg->CreateLightDlg();
	SPTR<CLight> DLight = SSDToolGlobal::ToolSelectActor->AddCom<CLight>(LIGHT_DATA{ RENDER_LAYER_PLAYER, POINT_LIGHT, true, L"Cone_Lv360" });

	SSDToolGlobal::ToolSelectLight = DLight;

	SSDToolGlobal::ToolSceneList->OnBnClickedReset();
	SSDToolGlobal::ToolLightDlg->SetNewLightInfo(true);
}


void ControlDlg::OnBnClickedBloom()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.


}


void ControlDlg::OnBnClickedGau()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void ControlDlg::OnBnClickedOn()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//m_ActorOnOff = true;
	if (nullptr == SSDToolGlobal::ToolSceneList)
	{
		return;
	}
	if (nullptr == SSDToolGlobal::ToolSelectActor)
	{
		return;
	}
	SSDToolGlobal::ToolSelectActor->On();
}


void ControlDlg::OnBnClickedOff()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//m_ActorOnOff = false;
	if (nullptr == SSDToolGlobal::ToolSceneList)
	{
		return;
	}
	if (nullptr == SSDToolGlobal::ToolSelectActor)
	{
		return;
	}
	SSDToolGlobal::ToolSelectActor->Off();
}


BOOL ControlDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	CEdit *webmacrotext = (CEdit *)GetDlgItem(IDC_COLLGROUP);
	if (pMsg->message == WM_LBUTTONDOWN && pMsg->hwnd == webmacrotext->m_hWnd &&
		GetFocus()->m_hWnd != webmacrotext->m_hWnd)
	{
		webmacrotext->SetFocus();
		webmacrotext->SetSel(0, -1, true);
		return true;
	}

	webmacrotext = (CEdit *)GetDlgItem(IDC_PREFABNAME);
	if (pMsg->message == WM_LBUTTONDOWN && pMsg->hwnd == webmacrotext->m_hWnd &&
		GetFocus()->m_hWnd != webmacrotext->m_hWnd)
	{
		webmacrotext->SetFocus();
		webmacrotext->SetSel(0, -1, true);
		return true;
	}

	webmacrotext = (CEdit *)GetDlgItem(IDC_DATANAME);
	if (pMsg->message == WM_LBUTTONDOWN && pMsg->hwnd == webmacrotext->m_hWnd &&
		GetFocus()->m_hWnd != webmacrotext->m_hWnd)
	{
		webmacrotext->SetFocus();
		webmacrotext->SetSel(0, -1, true);
		return true;
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}

void ControlDlg::OnBnClickedScenesave()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//씬에서 저장해야 할 것
	//엑터 - 랜더러 콜리전 라이트 트랜스폼 서브트렌스폼
	//메인 카메라 정보 - 필터 컴포넌트 
	//이름
	//세이브 클릭하면 패스 - 이름 넣고 저장하기
	//현재 씬에 있는 모든 엑터 삭제하고 거기에 넣어주기
	//씬 로드 함수 만들기

	CFileDialog fDlg(FALSE, L"scene", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, L"All Files(*.*)|*.*||");
	if (fDlg.DoModal() == IDOK)
	{
		CString PathName = fDlg.GetPathName();
		//SaveScene(PathName);
		DeadByDaylightGlobal::SaveScene(PathName, SSDToolGlobal::ToolGlobalScene);
	}
}

void ControlDlg::SaveScene(const CString& _Path)
{
	m_vecActor.clear();
	m_vecFilter.clear();

	//엑터 가져오기
	m_AllActorMap = SSDToolGlobal::ToolGlobalScene->GetAllActor();

	if (0 >= m_AllActorMap.size())
	{
		return;
	}

	m_StartIter = m_AllActorMap.begin();
	m_EndIter = m_AllActorMap.end();
	for (; m_StartIter != m_EndIter; ++m_StartIter)
	{
		std::list<SPTR<CActor>>::iterator ListStartIter = m_StartIter->second.begin();
		std::list<SPTR<CActor>>::iterator ListEndIter = m_StartIter->second.end();
		for (; ListStartIter != ListEndIter; ++ListStartIter)
		{
			//저장할 데이터 만들기 - 엑터 트렌스폼 넣어주면서 생성
			SPTR<ActorSaveData> NewData = new ActorSaveData(
				(*ListStartIter)->TRANS->GetScale(),
				(*ListStartIter)->GetWorldPos(),
				(*ListStartIter)->TRANS->GetLocalRotate());

			NewData->UpdateLayer = m_StartIter->first;

			//fbx랜더러 정보
			(*ListStartIter)->FindAllCom<CFbxRenderer>(&m_FbxList);
			if (0 < m_FbxList.size())
			{
				std::list<SPTR<CFbxRenderer>>::iterator RenStartIter = m_FbxList.begin();
				std::list<SPTR<CFbxRenderer>>::iterator RenEndIter = m_FbxList.end();
				for (; RenStartIter != RenEndIter; ++RenStartIter)
				{
					PrefabData::FbxSaveData TempData = PrefabData::FbxSaveData();
					const std::vector<SPTR<CFBXMesh>> TempFbxMeshVec = (*RenStartIter)->GetRenderingFbx()->GetAllFBXMeshVec();
					const std::vector<SPTR<CMesh>> TempAllMeshVec = (*RenStartIter)->GetAllMeshVec();
					if (0 < TempFbxMeshVec.size())
					{
						if (TempFbxMeshVec.size() == TempAllMeshVec.size())
						{
							TempData.AllCheck = true;
						}
						else
						{
							TempData.AllCheck = false;
						}

						//매쉬 이름 넣기
						TempData.MeshName = (*RenStartIter)->GetRenderingFbx()->GetFileName();
						//인덱스 넣기
						for (int i = 0; i < TempAllMeshVec.size(); ++i)
						{
							CFBXMesh* Test = (CFBXMesh*)TempAllMeshVec[i].PTR;
							UINT Index = Test->GetMeshIdx();
							TempData.MatInfoMap.insert(std::map<int, std::wstring>::value_type(Index, Test->GetMatStart(0)->GetPath()));
						}

						//랜더러 사이즈 포스 넣기
						TempData.RenderSize = (*RenStartIter)->GetRenSize();
						TempData.RenderPos = (*RenStartIter)->GetRenPivot();
						TempData.IsDeferred = (*RenStartIter)->GetFbxMesh(0)->GetMatStart(0)->IsDeferred();

						NewData->vecFbxData.push_back(TempData);
					}
				}
			}

			//Obb충돌체 정보 
			(*ListStartIter)->FindAllCom<COBBCollider>(&m_ObbList);
			if (0 < m_ObbList.size())
			{
				std::list<SPTR<COBBCollider>>::iterator ColStartIter = m_ObbList.begin();
				std::list<SPTR<COBBCollider>>::iterator ColEndIter = m_ObbList.end();
				for (; ColStartIter != ColEndIter; ++ColStartIter)
				{
					if (true != (*ColStartIter)->ExistTag(NOSAVE))
					{
						PrefabData::ColSaveData TempData = PrefabData::ColSaveData(COL_TYPE::COLTYPE_OBB);
						TempData.ColGroupName = (*ColStartIter)->GetChannel().c_str();
						TempData.ColPos = (*ColStartIter)->GetColliderPivot();
						TempData.ColSize = (*ColStartIter)->GetColliderSize();

						NewData->vecColData.push_back(TempData);
					}
				}
			}

			//Sphere 충돌체 정보
			(*ListStartIter)->FindAllCom<CSphereCollider>(&m_SphereList);
			if (0 < m_SphereList.size())
			{
				std::list<SPTR<CSphereCollider>>::iterator SColStartIter = m_SphereList.begin();
				std::list<SPTR<CSphereCollider>>::iterator SColEndIter = m_SphereList.end();
				for (; SColStartIter != SColEndIter; ++SColStartIter)
				{
					PrefabData::ColSaveData TempData = PrefabData::ColSaveData(COL_TYPE::COLTYPE_SPHERE);
					TempData.ColGroupName = (*SColStartIter)->GetChannel().c_str();
					TempData.ColPos = (*SColStartIter)->GetColliderPivot();
					TempData.ColSize = (*SColStartIter)->GetColliderSize();

					NewData->vecColData.push_back(TempData);
				}
			}


			// 네비게이션 메쉬 정보 
			(*ListStartIter)->FindAllCom<CNavigationObject>(&m_NaviObjList);
			if (false == m_NaviObjList.empty())
			{
				std::list<SPTR<CNavigationObject>>::iterator NaviStart = m_NaviObjList.begin();
				std::list<SPTR<CNavigationObject>>::iterator NaviEnd = m_NaviObjList.end();

				for (; NaviStart != NaviEnd; ++NaviStart)
				{
					PrefabData::NavigationData NaviData = PrefabData::NavigationData();
					NaviData.NaviObjName = (*NaviStart)->GetNaviRen()->GetName();
					NaviData.NaviSerialNumber = (*NaviStart)->GetNaviRen()->GetSerialNumber();
					NaviData.RenderLayer = (int)(*NaviStart)->GetNaviRen()->GetRenderLayer();

					NewData->vecNaviData.push_back(NaviData);
				}

			}

			//라이트 정보 - 엑터에 라이트는 무조건 하나
			SPTR<CLight> FindLight = (*ListStartIter)->FindCom<CLight>();
			if (nullptr != FindLight)
			{
				ActorSaveData::LightSaveData TempLightData = ActorSaveData::LightSaveData();
				TempLightData.Layer = FindLight->m_LightData.Layer;
				TempLightData.LightSubSize = FindLight->GetLightSize();
				TempLightData.VolumeCheck = FindLight->m_LightData.VolumeCheck;
				if (true == TempLightData.VolumeCheck)
				{
					TempLightData.MeshName = FindLight->GetVolumeMeshName();
				}
				else
				{
					TempLightData.MeshName = L"";
				}
				TempLightData.LightBuffer = FindLight->m_LightBufferData;
				NewData->IsLight = true;
				NewData->LightData = TempLightData;
			}

			//마법진 정보 저장 - 무조건 하나
			SPTR<CMagicCircle> FindCircle = (*ListStartIter)->FindCom<CMagicCircle>();
			if (nullptr != FindCircle)
			{
				NewData->IsMagicCircle = true;
				NewData->MagicCircleColor = FindCircle->GetMagicCircleColor();
				NewData->MagicCircleScale = FindCircle->GetMagicCircleRen()->GetRenSize();
				NewData->MagicCirclePivot = FindCircle->GetMagicCircleRen()->GetRenPivot();
			}

			if (0 >= m_FbxList.size() && 0 >= m_ObbList.size() && 0 >= m_SphereList.size() && nullptr == FindLight)
			{
				continue;
			}
			m_vecActor.push_back(NewData);

			m_FbxList.clear();
			m_ObbList.clear();
			m_SphereList.clear();
			m_NaviObjList.clear();
		}
	}
	SaveBinaryScene(_Path);
}

void ControlDlg::SaveBinaryScene(const CString& _Path)
{
	CWriteStream WriteData = CWriteStream(_Path);

	// 씬파일 저장순서
	// 1. 씬 해더 파일
	// 2. 액터 정보
	// 3. 터레인 정보

	// 1. 씬파일의 요약 정보를 담은 씬파일해더 정보를 파일에 저장
	SceneFileHeader SceneHeader;
	memset(&SceneHeader, 0, sizeof(SceneFileHeader));
	SceneHeader.FileVersion = (int)MAX_SCENEFILEVISION - 1;
	SceneHeader.ActorCount = (int)m_vecActor.size();
	WriteData.Write(SceneHeader);

	// 2. 액터 정보 저장
	for (size_t i = 0; i < m_vecActor.size(); ++i)
	{
		//엑터 정보
		WriteData.Write(m_vecActor[i]->UpdateLayer);
		WriteData.Write(m_vecActor[i]->ActorPos);
		WriteData.Write(m_vecActor[i]->ActorSize);
		WriteData.Write(m_vecActor[i]->ActorRot);

		//fbx정보 읽기
		int FbxSize = (int)m_vecActor[i]->vecFbxData.size();
		WriteData.Write(FbxSize);

		for (size_t j = 0; j < FbxSize; ++j)
		{
			WriteData.Write(m_vecActor[i]->vecFbxData[j].AllCheck);
			WriteData.Write(m_vecActor[i]->vecFbxData[j].MeshName);
			WriteData.Write(m_vecActor[i]->vecFbxData[j].RenderPos);
			WriteData.Write(m_vecActor[i]->vecFbxData[j].RenderSize);
			WriteData.Write(m_vecActor[i]->vecFbxData[j].IsDeferred);

			int MatMapSize = (int)m_vecActor[i]->vecFbxData[j].MatInfoMap.size();
			WriteData.Write(MatMapSize);

			std::map<int, std::wstring>::iterator MatStartIter = m_vecActor[i]->vecFbxData[j].MatInfoMap.begin();
			std::map<int, std::wstring>::iterator MatEndIter = m_vecActor[i]->vecFbxData[j].MatInfoMap.end();
			for (; MatStartIter != MatEndIter; ++MatStartIter)
			{
				int Temp = MatStartIter->first;
				WriteData.Write(Temp);
				WriteData.Write(MatStartIter->second);
			}
		}

		int ColVecSize = (int)m_vecActor[i]->vecColData.size();
		WriteData.Write(ColVecSize);

		for (size_t k = 0; k < ColVecSize; ++k)
		{
			WriteData.Write(m_vecActor[i]->vecColData[k].ColType);
			WriteData.Write(m_vecActor[i]->vecColData[k].ColGroupName);
			WriteData.Write(m_vecActor[i]->vecColData[k].ColPos);
			WriteData.Write(m_vecActor[i]->vecColData[k].ColSize);
		}

		int NaviVecSize = (int)m_vecActor[i]->vecNaviData.size();
		WriteData.Write(NaviVecSize);

		for (size_t k = 0; k < NaviVecSize; ++k)
		{
			WriteData.Write(m_vecActor[i]->vecNaviData[k].RenderLayer);
			WriteData.Write(m_vecActor[i]->vecNaviData[k].NaviSerialNumber);
			WriteData.Write(m_vecActor[i]->vecNaviData[k].NaviObjName);
		}

		bool IsLightSave = m_vecActor[i]->IsLight;
		WriteData.Write(IsLightSave);

		if (true == IsLightSave)
		{
			WriteData.Write(m_vecActor[i]->LightData.Layer);
			WriteData.Write(m_vecActor[i]->LightData.LightBuffer);
			WriteData.Write(m_vecActor[i]->LightData.VolumeCheck);
			WriteData.Write(m_vecActor[i]->LightData.MeshName);
			WriteData.Write(m_vecActor[i]->LightData.LightSubSize);
		}

		bool IsCircleSave = m_vecActor[i]->IsMagicCircle;
		WriteData.Write(IsCircleSave);

		if (true == IsCircleSave)
		{
			WriteData.Write(m_vecActor[i]->MagicCircleColor);
			WriteData.Write(m_vecActor[i]->MagicCircleScale);
			WriteData.Write(m_vecActor[i]->MagicCirclePivot);
		}
	}

	// 3. 터레인 정보 저장
	SPTR<CTerrainRenderer> TerrainRen = DBD::pMainTerrain->GetTerrainRen();
	std::list<SPTR<CNaviLinkTri>>* pLinkList = TerrainRen->GetLinkTriList();

	TerrainData::TerrainDataHeader HeaderData;
	HeaderData.TerrainSizeX = TerrainRen->GetTerrainRowCount();
	HeaderData.TerrainSizeZ = TerrainRen->GetTerrainColmCount();
	HeaderData.TerrainCellSize = TerrainRen->GetCellSize();
	HeaderData.TerrainMaxHeight = TerrainRen->GetMaxHeight();
	HeaderData.LinkTriCount = (unsigned int)pLinkList->size();

	WriteData.Write(HeaderData);
	size_t MaxTerrainCount = (size_t)(HeaderData.TerrainSizeX * HeaderData.TerrainSizeZ);

	for (size_t i = 0; i < MaxTerrainCount; i++)
	{
		GEOTERRAINVTX GeoData = TerrainRen->GetGeoTerrainVtx(i);

		if (GeoData.Color != NONAVITRICOLOR)
		{
			GeoData.Color = NAVITRICOLOR;
		}

		WriteData.Write(GeoData);
	}

	std::list<SPTR<CNaviLinkTri>>::iterator Start = pLinkList->begin();
	std::list<SPTR<CNaviLinkTri>>::iterator End = pLinkList->end();

	TerrainData::NaviLinkData LData;

	for (; Start != End; ++Start)
	{
		LData.NaviMeshID = (*Start)->GetLinkNaviMesh()->GetSerialNumber();
		LData.NaviTriIdx = (*Start)->GetLinkNaviTriIdx();
		LData.TerrainTriIdx = (*Start)->GetLinkTerrainIdx();
		for (size_t i = 0; i < 3; i++)
		{
			LData.LinkVtxData[i] = (*Start)->m_LinkVtx[i];
		}

		WriteData.Write(LData);
	}
}

void ControlDlg::OnBnClickedCamsave()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (nullptr == SSDToolGlobal::ToolSelectActor)
	{
		return;
	}
	SPTR<CCamera> FinCam = SSDToolGlobal::ToolSelectActor->FindCom<CCamera>();
	if (nullptr == FinCam)
	{
		return;
	}

	CFileDialog fDlg(FALSE, L"camera", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, L"All Files(*.*)|*.*||");
	if (fDlg.DoModal() == IDOK)
	{
		CString PathName = fDlg.GetPathName();
		SaveFilter(PathName, FinCam);
	}
}



void ControlDlg::OnBnClickedSceneload()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CFileDialog fDlg(TRUE, _T(".scene"), NULL, OFN_NOCHANGEDIR | OFN_OVERWRITEPROMPT, L"All Files(*.*)|*.*||");
	if (fDlg.DoModal() == IDOK)
	{
		CString PathName = fDlg.GetPathName();
		DeadByDaylightGlobal::ReadScene(PathName);
		//ReadScene(PathName);
	}
}

// 씬 파일 해더가 추가된 파일형으로 저장
void ControlDlg::OnBnClickedNewsceneload()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CFileDialog fDlg(TRUE, _T(".scene"), NULL, OFN_NOCHANGEDIR | OFN_OVERWRITEPROMPT, L"All Files(*.*)|*.*||");
	if (fDlg.DoModal() == IDOK)
	{
		CString PathName = fDlg.GetPathName();


		if (nullptr == SSDToolGlobal::ToolSceneList)
		{
			return;
		}
		if (nullptr != SSDToolGlobal::ToolObjectDlg)
		{
			SSDToolGlobal::ToolObjectDlg->OnBnClickedReset();
		}

		SSDToolGlobal::setToolNaviObj.clear();

		DeadByDaylightGlobal::ReadScene(PathName, SSDToolGlobal::ToolGlobalScene);
		//NewReadScene(PathName);
	}
}


void ControlDlg::ReadScene(const CString& _Path)
{
	CReadStream ReadData = CReadStream(_Path);

	int ActorSize;
	ReadData.Read(ActorSize);

	for (size_t i = 0; i < ActorSize; ++i)
	{
		int UpdateLayer;
		ReadData.Read(UpdateLayer);
		Vec3 Pos;
		ReadData.Read(Pos);
		Vec3 Size;
		ReadData.Read(Size);
		Vec3 Rot;
		ReadData.Read(Rot);

		ActorSaveData* NewData = new ActorSaveData(Size, Pos, Rot);
		NewData->UpdateLayer = UpdateLayer;

		int FbxVecSize;
		ReadData.Read(FbxVecSize);

		NewData->vecFbxData.resize(FbxVecSize);
		for (size_t j = 0; j < NewData->vecFbxData.size(); ++j)
		{
			bool AllCheck;
			ReadData.Read(AllCheck);
			std::wstring MeshName;
			ReadData.Read(MeshName);
			Vec3 RenderPos;
			ReadData.Read(RenderPos);
			Vec3 RenderSize;
			ReadData.Read(RenderSize);

			NewData->vecFbxData[j].AllCheck = AllCheck;
			NewData->vecFbxData[j].MeshName = MeshName;
			NewData->vecFbxData[j].RenderPos = RenderPos;
			NewData->vecFbxData[j].RenderSize = RenderSize;

			int MatMapSize;
			ReadData.Read(MatMapSize);
			for (size_t k = 0; k < MatMapSize; ++k)
			{
				int Index;
				ReadData.Read(Index);
				std::wstring MatName;
				ReadData.Read(MatName);

				NewData->vecFbxData[j].MatInfoMap.insert(std::map<int, std::wstring>::value_type(Index, MatName));
			}
		}

		int ColVecSize;
		ReadData.Read(ColVecSize);

		NewData->vecColData.resize(ColVecSize);
		for (size_t k = 0; k < ColVecSize; ++k)
		{
			COL_TYPE Type;
			ReadData.Read(Type);
			std::wstring GroupName;
			ReadData.Read(GroupName);
			Vec3 ColPos;
			ReadData.Read(ColPos);
			Vec3 ColSize;
			ReadData.Read(ColSize);

			NewData->vecColData[k].ColType = Type;
			NewData->vecColData[k].ColGroupName = GroupName;
			NewData->vecColData[k].ColPos = ColPos;
			NewData->vecColData[k].ColSize = ColSize;
		}

		bool IsLight;
		ReadData.Read(IsLight);
		NewData->IsLight = IsLight;
		if (true == IsLight)
		{
			RENDER_LAYER Layer;
			ReadData.Read(Layer);
			CBUFFER::LightBuffer LightBuffer;
			ReadData.Read(LightBuffer);
			bool VolumeCheck;
			ReadData.Read(VolumeCheck);
			std::wstring MeshName;
			ReadData.Read(MeshName);
			Vec3 LightSubSize;
			ReadData.Read(LightSubSize);

			NewData->LightData.Layer = Layer;
			NewData->LightData.LightBuffer = LightBuffer;
			NewData->LightData.VolumeCheck = VolumeCheck;
			NewData->LightData.MeshName = MeshName;
			NewData->LightData.LightSubSize = LightSubSize;
		}

		//bool IsCircleSave;
		//ReadData.Read(IsCircleSave);
		//if (true == IsCircleSave)
		//{
		//	MAGICCIRCLE_COLOR TempColor;
		//	ReadData.Read(TempColor);
		//	Vec3 TempScale;
		//	ReadData.Read(TempScale);
		//	Vec3 TempPivot;
		//	ReadData.Read(TempPivot);

		//	NewData->IsMagicCircle = IsCircleSave;
		//	NewData->MagicCircleColor = TempColor;
		//	NewData->MagicCircleScale = TempScale;
		//	NewData->MagicCirclePivot = TempPivot;
		//}
		m_vecLoadActor.push_back(NewData);
	}

	//씬에 배치해주기
	LoadScene();
}

void ControlDlg::NewReadScene(const CString& _Path)
{
	CReadStream ReadData = CReadStream(_Path);


	// 1. 씬 해더파일 읽기
	// 2. 액터 데이터 일기
	// 3. 터레인 데이터 읽기

	SceneFileHeader SHeader;

	ReadData.Read(SHeader);

	if (SHeader.FileVersion >= MAX_SCENEFILEVISION)
	{
		int a = 0;
	}
	if (SHeader.FileVersion == TERRAIN_NAVIGATION_SCENE_FILE)
	{
		for (size_t i = 0; i < SHeader.ActorCount; ++i)
		{
			int UpdateLayer;
			ReadData.Read(UpdateLayer);
			Vec3 Pos;
			ReadData.Read(Pos);
			Vec3 Size;
			ReadData.Read(Size);
			Vec3 Rot;
			ReadData.Read(Rot);

			ActorSaveData* NewData = new ActorSaveData(Size, Pos, Rot);
			NewData->UpdateLayer = UpdateLayer;

			int FbxVecSize;
			ReadData.Read(FbxVecSize);

			NewData->vecFbxData.resize(FbxVecSize);
			for (size_t j = 0; j < NewData->vecFbxData.size(); ++j)
			{
				bool AllCheck;
				ReadData.Read(AllCheck);
				std::wstring MeshName;
				ReadData.Read(MeshName);
				Vec3 RenderPos;
				ReadData.Read(RenderPos);
				Vec3 RenderSize;
				ReadData.Read(RenderSize);

				NewData->vecFbxData[j].AllCheck = AllCheck;
				NewData->vecFbxData[j].MeshName = MeshName;
				NewData->vecFbxData[j].RenderPos = RenderPos;
				NewData->vecFbxData[j].RenderSize = RenderSize;

				int MatMapSize;
				ReadData.Read(MatMapSize);
				for (size_t k = 0; k < MatMapSize; ++k)
				{
					int Index;
					ReadData.Read(Index);
					std::wstring MatName;
					ReadData.Read(MatName);

					NewData->vecFbxData[j].MatInfoMap.insert(std::map<int, std::wstring>::value_type(Index, MatName));
				}
			}

			int ColVecSize;
			ReadData.Read(ColVecSize);

			NewData->vecColData.resize(ColVecSize);
			for (size_t k = 0; k < ColVecSize; ++k)
			{
				COL_TYPE Type;
				ReadData.Read(Type);
				std::wstring GroupName;
				ReadData.Read(GroupName);
				Vec3 ColPos;
				ReadData.Read(ColPos);
				Vec3 ColSize;
				ReadData.Read(ColSize);

				NewData->vecColData[k].ColType = Type;
				NewData->vecColData[k].ColGroupName = GroupName;
				NewData->vecColData[k].ColPos = ColPos;
				NewData->vecColData[k].ColSize = ColSize;
			}

			int NaviVecSize;
			ReadData.Read(NaviVecSize);

			NewData->vecNaviData.resize(NaviVecSize);
			for (size_t k = 0; k < NaviVecSize; ++k)
			{
				ReadData.Read(NewData->vecNaviData[k].RenderLayer);
				ReadData.Read(NewData->vecNaviData[k].NaviSerialNumber);
				ReadData.Read(NewData->vecNaviData[k].NaviObjName);
			}

			bool IsLight;
			ReadData.Read(IsLight);
			NewData->IsLight = IsLight;
			if (true == IsLight)
			{
				RENDER_LAYER Layer;
				ReadData.Read(Layer);
				CBUFFER::LightBuffer LightBuffer;
				ReadData.Read(LightBuffer);
				bool VolumeCheck;
				ReadData.Read(VolumeCheck);
				std::wstring MeshName;
				ReadData.Read(MeshName);
				Vec3 LightSubSize;
				ReadData.Read(LightSubSize);

				NewData->LightData.Layer = Layer;
				NewData->LightData.LightBuffer = LightBuffer;
				NewData->LightData.VolumeCheck = VolumeCheck;
				NewData->LightData.MeshName = MeshName;
				NewData->LightData.LightSubSize = LightSubSize;
			}
			m_vecLoadActor.push_back(NewData);
		}

		// 3. 터레인 정보 읽어오기
		m_TerrainLoadData.NaviLinkTriList.clear();
		m_TerrainLoadData.TerrainDataVec.clear();

		ReadData.Read(m_TerrainLoadData.Header);

		size_t MaxVtx = (size_t)(m_TerrainLoadData.Header.TerrainSizeX*m_TerrainLoadData.Header.TerrainSizeZ);

		m_TerrainLoadData.TerrainDataVec.resize(MaxVtx);
		for (size_t i = 0; i < MaxVtx; i++)
		{
			ReadData.Read(m_TerrainLoadData.TerrainDataVec[i]);
		}

		TerrainData::NaviLinkData LData;
		for (size_t i = 0; i < m_TerrainLoadData.Header.LinkTriCount; i++)
		{
			ReadData.Read(LData);
			m_TerrainLoadData.NaviLinkTriList.push_back(LData);
		}
	}
	else if (SHeader.FileVersion == TERRAIN_MAGIC_FILE)
	{
		for (size_t i = 0; i < SHeader.ActorCount; ++i)
		{
			int UpdateLayer;
			ReadData.Read(UpdateLayer);
			Vec3 Pos;
			ReadData.Read(Pos);
			Vec3 Size;
			ReadData.Read(Size);
			Vec3 Rot;
			ReadData.Read(Rot);

			ActorSaveData* NewData = new ActorSaveData(Size, Pos, Rot);
			NewData->UpdateLayer = UpdateLayer;

			int FbxVecSize;
			ReadData.Read(FbxVecSize);

			NewData->vecFbxData.resize(FbxVecSize);
			for (size_t j = 0; j < NewData->vecFbxData.size(); ++j)
			{
				bool AllCheck;
				ReadData.Read(AllCheck);
				std::wstring MeshName;
				ReadData.Read(MeshName);
				Vec3 RenderPos;
				ReadData.Read(RenderPos);
				Vec3 RenderSize;
				ReadData.Read(RenderSize);

				NewData->vecFbxData[j].AllCheck = AllCheck;
				NewData->vecFbxData[j].MeshName = MeshName;
				NewData->vecFbxData[j].RenderPos = RenderPos;
				NewData->vecFbxData[j].RenderSize = RenderSize;

				int MatMapSize;
				ReadData.Read(MatMapSize);
				for (size_t k = 0; k < MatMapSize; ++k)
				{
					int Index;
					ReadData.Read(Index);
					std::wstring MatName;
					ReadData.Read(MatName);

					NewData->vecFbxData[j].MatInfoMap.insert(std::map<int, std::wstring>::value_type(Index, MatName));
				}
			}

			int ColVecSize;
			ReadData.Read(ColVecSize);

			NewData->vecColData.resize(ColVecSize);
			for (size_t k = 0; k < ColVecSize; ++k)
			{
				COL_TYPE Type;
				ReadData.Read(Type);
				std::wstring GroupName;
				ReadData.Read(GroupName);
				Vec3 ColPos;
				ReadData.Read(ColPos);
				Vec3 ColSize;
				ReadData.Read(ColSize);

				NewData->vecColData[k].ColType = Type;
				NewData->vecColData[k].ColGroupName = GroupName;
				NewData->vecColData[k].ColPos = ColPos;
				NewData->vecColData[k].ColSize = ColSize;
			}

			int NaviVecSize;
			ReadData.Read(NaviVecSize);

			NewData->vecNaviData.resize(NaviVecSize);
			for (size_t k = 0; k < NaviVecSize; ++k)
			{
				ReadData.Read(NewData->vecNaviData[k].RenderLayer);
				ReadData.Read(NewData->vecNaviData[k].NaviSerialNumber);
				ReadData.Read(NewData->vecNaviData[k].NaviObjName);
			}

			bool IsLight;
			ReadData.Read(IsLight);
			NewData->IsLight = IsLight;
			if (true == IsLight)
			{
				RENDER_LAYER Layer;
				ReadData.Read(Layer);
				CBUFFER::LightBuffer LightBuffer;
				ReadData.Read(LightBuffer);
				bool VolumeCheck;
				ReadData.Read(VolumeCheck);
				std::wstring MeshName;
				ReadData.Read(MeshName);
				Vec3 LightSubSize;
				ReadData.Read(LightSubSize);

				NewData->LightData.Layer = Layer;
				NewData->LightData.LightBuffer = LightBuffer;
				NewData->LightData.VolumeCheck = VolumeCheck;
				NewData->LightData.MeshName = MeshName;
				NewData->LightData.LightSubSize = LightSubSize;
			}

			bool IsCircleSave;
			ReadData.Read(IsCircleSave);
			if (true == IsCircleSave)
			{
				MAGICCIRCLE_COLOR TempColor;
				ReadData.Read(TempColor);
				Vec3 TempScale;
				ReadData.Read(TempScale);
				Vec3 TempPivot;
				ReadData.Read(TempPivot);

				NewData->IsMagicCircle = IsCircleSave;
				NewData->MagicCircleColor = TempColor;
				NewData->MagicCircleScale = TempScale;
				NewData->MagicCirclePivot = TempPivot;
			}

			m_vecLoadActor.push_back(NewData);
		}


		// 3. 터레인 정보 읽어오기
		m_TerrainLoadData.NaviLinkTriList.clear();
		m_TerrainLoadData.TerrainDataVec.clear();

		ReadData.Read(m_TerrainLoadData.Header);

		size_t MaxVtx = (size_t)(m_TerrainLoadData.Header.TerrainSizeX*m_TerrainLoadData.Header.TerrainSizeZ);

		m_TerrainLoadData.TerrainDataVec.resize(MaxVtx);
		for (size_t i = 0; i < MaxVtx; i++)
		{
			ReadData.Read(m_TerrainLoadData.TerrainDataVec[i]);
		}

		TerrainData::NaviLinkData LData;
		for (size_t i = 0; i < m_TerrainLoadData.Header.LinkTriCount; i++)
		{
			ReadData.Read(LData);
			m_TerrainLoadData.NaviLinkTriList.push_back(LData);
		}
	}
	else if (SHeader.FileVersion == TERRAIN_MAGIC_DEFERRED_FILE)
	{
		for (size_t i = 0; i < SHeader.ActorCount; ++i)
		{
			int UpdateLayer;
			ReadData.Read(UpdateLayer);
			Vec3 Pos;
			ReadData.Read(Pos);
			Vec3 Size;
			ReadData.Read(Size);
			Vec3 Rot;
			ReadData.Read(Rot);

			ActorSaveData* NewData = new ActorSaveData(Size, Pos, Rot);
			NewData->UpdateLayer = UpdateLayer;

			int FbxVecSize;
			ReadData.Read(FbxVecSize);

			NewData->vecFbxData.resize(FbxVecSize);
			for (size_t j = 0; j < NewData->vecFbxData.size(); ++j)
			{
				bool AllCheck;
				ReadData.Read(AllCheck);
				std::wstring MeshName;
				ReadData.Read(MeshName);
				Vec3 RenderPos;
				ReadData.Read(RenderPos);
				Vec3 RenderSize;
				ReadData.Read(RenderSize);
				bool IsDeferred;
				ReadData.Read(IsDeferred);

				NewData->vecFbxData[j].AllCheck = AllCheck;
				NewData->vecFbxData[j].MeshName = MeshName;
				NewData->vecFbxData[j].RenderPos = RenderPos;
				NewData->vecFbxData[j].RenderSize = RenderSize;
				NewData->vecFbxData[j].IsDeferred = IsDeferred;

				int MatMapSize;
				ReadData.Read(MatMapSize);
				for (size_t k = 0; k < MatMapSize; ++k)
				{
					int Index;
					ReadData.Read(Index);
					std::wstring MatName;
					ReadData.Read(MatName);

					NewData->vecFbxData[j].MatInfoMap.insert(std::map<int, std::wstring>::value_type(Index, MatName));
				}
			}

			int ColVecSize;
			ReadData.Read(ColVecSize);

			NewData->vecColData.resize(ColVecSize);
			for (size_t k = 0; k < ColVecSize; ++k)
			{
				COL_TYPE Type;
				ReadData.Read(Type);
				std::wstring GroupName;
				ReadData.Read(GroupName);
				Vec3 ColPos;
				ReadData.Read(ColPos);
				Vec3 ColSize;
				ReadData.Read(ColSize);

				NewData->vecColData[k].ColType = Type;
				NewData->vecColData[k].ColGroupName = GroupName;
				NewData->vecColData[k].ColPos = ColPos;
				NewData->vecColData[k].ColSize = ColSize;
			}

			int NaviVecSize;
			ReadData.Read(NaviVecSize);

			NewData->vecNaviData.resize(NaviVecSize);
			for (size_t k = 0; k < NaviVecSize; ++k)
			{
				ReadData.Read(NewData->vecNaviData[k].RenderLayer);
				ReadData.Read(NewData->vecNaviData[k].NaviSerialNumber);
				ReadData.Read(NewData->vecNaviData[k].NaviObjName);
			}

			bool IsLight;
			ReadData.Read(IsLight);
			NewData->IsLight = IsLight;
			if (true == IsLight)
			{
				RENDER_LAYER Layer;
				ReadData.Read(Layer);
				CBUFFER::LightBuffer LightBuffer;
				ReadData.Read(LightBuffer);
				bool VolumeCheck;
				ReadData.Read(VolumeCheck);
				std::wstring MeshName;
				ReadData.Read(MeshName);
				Vec3 LightSubSize;
				ReadData.Read(LightSubSize);

				NewData->LightData.Layer = Layer;
				NewData->LightData.LightBuffer = LightBuffer;
				NewData->LightData.VolumeCheck = VolumeCheck;
				NewData->LightData.MeshName = MeshName;
				NewData->LightData.LightSubSize = LightSubSize;
			}

			bool IsCircleSave;
			ReadData.Read(IsCircleSave);
			if (true == IsCircleSave)
			{
				MAGICCIRCLE_COLOR TempColor;
				ReadData.Read(TempColor);
				Vec3 TempScale;
				ReadData.Read(TempScale);
				Vec3 TempPivot;
				ReadData.Read(TempPivot);

				NewData->IsMagicCircle = IsCircleSave;
				NewData->MagicCircleColor = TempColor;
				NewData->MagicCircleScale = TempScale;
				NewData->MagicCirclePivot = TempPivot;
			}

			m_vecLoadActor.push_back(NewData);
		}


		// 3. 터레인 정보 읽어오기
		m_TerrainLoadData.NaviLinkTriList.clear();
		m_TerrainLoadData.TerrainDataVec.clear();

		ReadData.Read(m_TerrainLoadData.Header);

		size_t MaxVtx = (size_t)(m_TerrainLoadData.Header.TerrainSizeX*m_TerrainLoadData.Header.TerrainSizeZ);

		m_TerrainLoadData.TerrainDataVec.resize(MaxVtx);
		for (size_t i = 0; i < MaxVtx; i++)
		{
			ReadData.Read(m_TerrainLoadData.TerrainDataVec[i]);
		}

		TerrainData::NaviLinkData LData;
		for (size_t i = 0; i < m_TerrainLoadData.Header.LinkTriCount; i++)
		{
			ReadData.Read(LData);
			m_TerrainLoadData.NaviLinkTriList.push_back(LData);
		}
	}

	// 4. 파일에서 읽어들인 터레인 정보를 적용한다.  
	DBD::pMainTerrain->LoadTerrainData(m_TerrainLoadData);

}


void ControlDlg::LoadScene()
{
	if (nullptr == SSDToolGlobal::ToolSceneList)
	{
		return;
	}
	if (nullptr != SSDToolGlobal::ToolObjectDlg)
	{
		SSDToolGlobal::ToolObjectDlg->OnBnClickedReset();
	}

	Engine::ClearNaviMeshMap();

	//터레인, 카메라, 스카이박스 제외한 엑터 전부 삭제하기
	m_mapAllLoadActor = SSDToolGlobal::ToolGlobalScene->GetAllActor();
	std::map<int, std::list<SPTR<CActor>>>::iterator Startiter = m_mapAllLoadActor.begin();
	std::map<int, std::list<SPTR<CActor>>>::iterator Enditer = m_mapAllLoadActor.end();
	for (; Startiter != Enditer; ++Startiter)
	{
		std::list<SPTR<CActor>> ActorList = Startiter->second;
		std::list<SPTR<CActor>>::iterator SIter = ActorList.begin();
		std::list<SPTR<CActor>>::iterator EIter = ActorList.end();
		for (; SIter != EIter; ++SIter)
		{
			SPTR<CCamera> TempCamera = (*SIter)->FindCom<CCamera>();
			if (nullptr != TempCamera)
			{
				continue;
			}

			SPTR<CSkybox> TempSky = (*SIter)->FindCom<CSkybox>();
			if (nullptr != TempSky)
			{
				continue;
			}

			SPTR<CTerrain> TempTerrain = (*SIter)->FindCom<CTerrain>();

			if (nullptr != TempTerrain)
			{
				continue;
			}

			if (nullptr != (*SIter))
			{
				(*SIter)->Death();
			}
		}
	}

	SSDToolGlobal::setToolNaviObj.clear();

	for (size_t k = 0; k < m_vecLoadActor.size(); ++k)
	{
		if (nullptr != m_vecLoadActor[k])
		{
			ACTOR_UPDATE_LAYER Layer = (ACTOR_UPDATE_LAYER)m_vecLoadActor[k]->UpdateLayer;
			bool IsLight = m_vecLoadActor[k]->IsLight;
			bool IsCircle = m_vecLoadActor[k]->IsMagicCircle;

			//라이트 엑터
			if (true == IsLight)
			{
				SPTR<CActor> NewActor = SSDToolGlobal::ToolGlobalScene->CreateActor(SSDToolGlobal::ToolGlobalScene, Layer, L"LightActor");
				NewActor->TRANS->SetPos(m_vecLoadActor[k]->ActorPos);
				NewActor->TRANS->SetScale(m_vecLoadActor[k]->ActorSize);
				NewActor->TRANS->SetRotate(m_vecLoadActor[k]->ActorRot);

				ActorSaveData::LightSaveData TempData = m_vecLoadActor[k]->LightData;
				SPTR<CLight> TempLight = NewActor->AddCom<CLight>(LIGHT_DATA{
					TempData.Layer,
					(LIGHT_TYPE)TempData.LightBuffer.LightType,
					TempData.VolumeCheck,
					TempData.MeshName });

				TempLight->SetLightBuffer(TempData.LightBuffer);
				TempLight->SetLightSize(TempData.LightSubSize);
				TempLight->SetLightDir(TempData.LightBuffer.LightDir.vec3);
			}
			//마법진 엑터
			else if (true == IsCircle)
			{
				SPTR<CActor> NewActor = SSDToolGlobal::ToolGlobalScene->CreateActor(SSDToolGlobal::ToolGlobalScene, Layer, L"LightActor");
				NewActor->TRANS->SetPos(m_vecLoadActor[k]->ActorPos);
				NewActor->TRANS->SetScale(m_vecLoadActor[k]->ActorSize);
				NewActor->TRANS->SetRotate(m_vecLoadActor[k]->ActorRot);

				SPTR<CMagicCircle> TempCircle = NewActor->AddCom<CMagicCircle>(m_vecLoadActor[k]->MagicCircleColor);
				TempCircle->GetMagicCircleRen()->SetRenSize(m_vecLoadActor[k]->MagicCircleScale);
				TempCircle->GetMagicCircleRen()->SetRenPivot(m_vecLoadActor[k]->MagicCirclePivot);
			}
			else
			{
				SPTR<CActor> NewActor = SSDToolGlobal::ToolGlobalScene->CreateActor(SSDToolGlobal::ToolGlobalScene, Layer, L"NewActor");
				NewActor->TRANS->SetPos(m_vecLoadActor[k]->ActorPos);
				NewActor->TRANS->SetScale(m_vecLoadActor[k]->ActorSize);
				NewActor->TRANS->SetRotate(m_vecLoadActor[k]->ActorRot);

				//랜더러
				std::vector<PrefabData::FbxSaveData>& FbxVector = m_vecLoadActor[k]->vecFbxData;
				for (size_t i = 0; i < FbxVector.size(); ++i)
				{
					SPTR<CFbxRenderer> TempRender = NewActor->AddCom<CFbxRenderer>(RENDATA{ RENDER_LAYER_PLAYER });
					TempRender->SetRenSize(m_vecLoadActor[k]->vecFbxData[i].RenderSize);
					TempRender->SetRenPivot(m_vecLoadActor[k]->vecFbxData[i].RenderPos);

					TempRender->SetFbx(m_vecLoadActor[k]->vecFbxData[i].MeshName.c_str());
					if (true == m_vecLoadActor[k]->vecFbxData[i].AllCheck)
					{
						TempRender->SetAllFbxMesh();
					}
					else if (false == m_vecLoadActor[k]->vecFbxData[i].AllCheck)
					{
						std::map<int, std::wstring>::iterator StartIter = m_vecLoadActor[k]->vecFbxData[i].MatInfoMap.begin();
						std::map<int, std::wstring>::iterator EndIter = m_vecLoadActor[k]->vecFbxData[i].MatInfoMap.end();
						for (; StartIter != EndIter; ++StartIter)
						{
							TempRender->SettingFbxMesh(StartIter->first);
						}
					}
				}

				//충돌체
				std::vector<PrefabData::ColSaveData>& ColSaveVector = m_vecLoadActor[k]->vecColData;
				for (size_t j = 0; j < ColSaveVector.size(); ++j)
				{
					if (COL_TYPE::COLTYPE_OBB == m_vecLoadActor[k]->vecColData[j].ColType)
					{
						SPTR<COBBCollider> TempColl = NewActor->
							AddCom<COBBCollider>(COLDATA{ m_vecLoadActor[k]->vecColData[j].ColGroupName, COLLISION_3D });
						TempColl->SetColSize(m_vecLoadActor[k]->vecColData[j].ColSize);
						TempColl->SetColPivot(m_vecLoadActor[k]->vecColData[j].ColPos);
					}
					else if (COL_TYPE::COLTYPE_SPHERE == m_vecLoadActor[k]->vecColData[j].ColType)
					{
						SPTR<CSphereCollider> TempColl = NewActor->
							AddCom<CSphereCollider>(COLDATA{ m_vecLoadActor[k]->vecColData[j].ColGroupName, COLLISION_3D });
						TempColl->SetColSize(m_vecLoadActor[k]->vecColData[j].ColSize);
						TempColl->SetColPivot(m_vecLoadActor[k]->vecColData[j].ColPos);
					}
				}

			}
		}
	}

	if (nullptr != SSDToolGlobal::ToolSceneList)
	{
		//SSDToolGlobal::ToolSceneList->OnBnClickedReset();
	}
	m_vecLoadActor.clear();

}

void ControlDlg::NewLoadScene(const SCENEFILEVISION& _Version)
{
	if (nullptr == SSDToolGlobal::ToolSceneList)
	{
		return;
	}
	if (nullptr != SSDToolGlobal::ToolObjectDlg)
	{
		SSDToolGlobal::ToolObjectDlg->OnBnClickedReset();
	}

	SSDToolGlobal::setToolNaviObj.clear();

	//터레인, 카메라, 스카이박스 제외한 엑터 전부 삭제하기
	m_mapAllLoadActor = SSDToolGlobal::ToolGlobalScene->GetAllActor();

	std::map<int, std::list<SPTR<CActor>>>::iterator Startiter = m_mapAllLoadActor.begin();
	std::map<int, std::list<SPTR<CActor>>>::iterator Enditer = m_mapAllLoadActor.end();
	for (; Startiter != Enditer; ++Startiter)
	{
		std::list<SPTR<CActor>> ActorList = Startiter->second;
		std::list<SPTR<CActor>>::iterator SIter = ActorList.begin();
		std::list<SPTR<CActor>>::iterator EIter = ActorList.end();
		for (; SIter != EIter; ++SIter)
		{
			SPTR<CCamera> TempCamera = (*SIter)->FindCom<CCamera>();
			if (nullptr != TempCamera)
			{
				continue;
			}

			SPTR<CSkybox> TempSky = (*SIter)->FindCom<CSkybox>();
			if (nullptr != TempSky)
			{
				continue;
			}

			SPTR<CTerrain> TempTerrain = (*SIter)->FindCom<CTerrain>();
			if (nullptr != TempTerrain)
			{
				continue;
			}

			if (nullptr != (*SIter))
			{
				(*SIter)->Death();
			}
		}
	}




	Engine::ClearNaviMeshMap();
	if (_Version >= MAX_SCENEFILEVISION)
	{
		int a = 0;
	}
	else if (_Version == TERRAIN_NAVIGATION_SCENE_FILE)
	{
		// 1. 파일에서 읽어들인 액터 정보를 적용
		for (size_t k = 0; k < m_vecLoadActor.size(); ++k)
		{
			if (nullptr != m_vecLoadActor[k])
			{
				ACTOR_UPDATE_LAYER Layer = (ACTOR_UPDATE_LAYER)m_vecLoadActor[k]->UpdateLayer;
				bool IsLight = m_vecLoadActor[k]->IsLight;
				bool IsCircle = m_vecLoadActor[k]->IsMagicCircle;

				//라이트 엑터
				if (true == IsLight)
				{
					SPTR<CActor> NewActor = SSDToolGlobal::ToolGlobalScene->CreateActor(SSDToolGlobal::ToolGlobalScene, Layer, L"LightActor");
					NewActor->TRANS->SetPos(m_vecLoadActor[k]->ActorPos);
					NewActor->TRANS->SetScale(m_vecLoadActor[k]->ActorSize);
					NewActor->TRANS->SetRotate(m_vecLoadActor[k]->ActorRot);

					ActorSaveData::LightSaveData TempData = m_vecLoadActor[k]->LightData;

					SPTR<CLight> TempLight = NewActor->AddCom<CLight>(LIGHT_DATA{
						TempData.Layer,
						(LIGHT_TYPE)TempData.LightBuffer.LightType,
						TempData.VolumeCheck,
						TempData.MeshName });

					TempLight->SetLightBuffer(TempData.LightBuffer);
					TempLight->SetLightSize(TempData.LightSubSize);
					TempLight->SetLightDir(TempData.LightBuffer.LightDir.vec3);
				}
				////마법진 엑터
				//else if (true == IsCircle)
				//{
				//	SPTR<CActor> NewActor = SSDToolGlobal::ToolGlobalScene->CreateActor(SSDToolGlobal::ToolGlobalScene, Layer, L"LightActor");
				//	NewActor->TRANS->SetPos(m_vecLoadActor[k]->ActorPos);
				//	NewActor->TRANS->SetScale(m_vecLoadActor[k]->ActorSize);
				//	NewActor->TRANS->SetRotate(m_vecLoadActor[k]->ActorRot);

				//	SPTR<CMagicCircle> TempCircle = NewActor->AddCom<CMagicCircle>(m_vecLoadActor[k]->MagicCircleColor);
				//	TempCircle->GetMagicCircleRen()->SetRenSize(m_vecLoadActor[k]->MagicCircleScale);
				//	TempCircle->GetMagicCircleRen()->SetRenPivot(m_vecLoadActor[k]->MagicCirclePivot);
				//}
				else
				{
					SPTR<CActor> NewActor = SSDToolGlobal::ToolGlobalScene->CreateActor(SSDToolGlobal::ToolGlobalScene, Layer, L"NewActor");
					NewActor->TRANS->SetPos(m_vecLoadActor[k]->ActorPos);
					NewActor->TRANS->SetScale(m_vecLoadActor[k]->ActorSize);
					NewActor->TRANS->SetRotate(m_vecLoadActor[k]->ActorRot);

					//랜더러
					std::vector<PrefabData::FbxSaveData> FbxVector = m_vecLoadActor[k]->vecFbxData;
					for (size_t i = 0; i < FbxVector.size(); ++i)
					{
						SPTR<CFbxRenderer> TempRender = NewActor->AddCom<CFbxRenderer>(RENDATA{ RENDER_LAYER_PLAYER });
						TempRender->SetRenSize(m_vecLoadActor[k]->vecFbxData[i].RenderSize);
						TempRender->SetRenPivot(m_vecLoadActor[k]->vecFbxData[i].RenderPos);

						TempRender->SetFbx(m_vecLoadActor[k]->vecFbxData[i].MeshName.c_str());
						if (true == m_vecLoadActor[k]->vecFbxData[i].AllCheck)
						{
							TempRender->SetAllFbxMesh();
						}
						else if (false == m_vecLoadActor[k]->vecFbxData[i].AllCheck)
						{
							std::map<int, std::wstring>::iterator StartIter = m_vecLoadActor[k]->vecFbxData[i].MatInfoMap.begin();
							std::map<int, std::wstring>::iterator EndIter = m_vecLoadActor[k]->vecFbxData[i].MatInfoMap.end();
							for (; StartIter != EndIter; ++StartIter)
							{
								TempRender->SettingFbxMesh(StartIter->first);
							}
						}
					}

					//충돌체
					std::vector<PrefabData::ColSaveData> ColSaveVector = m_vecLoadActor[k]->vecColData;
					for (size_t j = 0; j < ColSaveVector.size(); ++j)
					{
						if (COL_TYPE::COLTYPE_OBB == m_vecLoadActor[k]->vecColData[j].ColType)
						{
							SPTR<COBBCollider> TempColl = NewActor->
								AddCom<COBBCollider>(COLDATA{ m_vecLoadActor[k]->vecColData[j].ColGroupName, COLLISION_3D });
							TempColl->SetColSize(m_vecLoadActor[k]->vecColData[j].ColSize);
							TempColl->SetColPivot(m_vecLoadActor[k]->vecColData[j].ColPos);
						}
						else if (COL_TYPE::COLTYPE_SPHERE == m_vecLoadActor[k]->vecColData[j].ColType)
						{
							SPTR<CSphereCollider> TempColl = NewActor->
								AddCom<CSphereCollider>(COLDATA{ m_vecLoadActor[k]->vecColData[j].ColGroupName, COLLISION_3D });
							TempColl->SetColSize(m_vecLoadActor[k]->vecColData[j].ColSize);
							TempColl->SetColPivot(m_vecLoadActor[k]->vecColData[j].ColPos);
						}
					}

					// 네비게이션 메쉬
					std::vector<PrefabData::NavigationData>& NaviDataVec = m_vecLoadActor[k]->vecNaviData;
					size_t NaviCount = NaviDataVec.size();
					for (size_t i = 0; i < NaviCount; i++)
					{

						SPTR<CNavigationObject> NaviObj = NewActor->AddCom<CNavigationObject>();

						NaviObj->CreateNaviArea(DBD::pMainTerrain);
						NaviObj->CreateNavigationRenderer(NAVIMESH_DATA{ (RENDER_LAYER)NaviDataVec[i].RenderLayer, NaviDataVec[i].NaviSerialNumber, NaviDataVec[i].NaviObjName });
						NaviObj->GetNaviRen()->CreateNaviMeshCollider(L"NaviTri");
						NaviObj->CreateNavigationMesh(NaviDataVec[i].NaviObjName.c_str(), NAVIMESH_OBJ);
					}
				}
			}
		}
	}
	else if (_Version == TERRAIN_MAGIC_FILE)
	{
		// 1. 파일에서 읽어들인 액터 정보를 적용
		for (size_t k = 0; k < m_vecLoadActor.size(); ++k)
		{
			if (nullptr != m_vecLoadActor[k])
			{
				ACTOR_UPDATE_LAYER Layer = (ACTOR_UPDATE_LAYER)m_vecLoadActor[k]->UpdateLayer;
				bool IsLight = m_vecLoadActor[k]->IsLight;
				bool IsCircle = m_vecLoadActor[k]->IsMagicCircle;

				//라이트 엑터
				if (true == IsLight)
				{
					SPTR<CActor> NewActor = SSDToolGlobal::ToolGlobalScene->CreateActor(SSDToolGlobal::ToolGlobalScene, Layer, L"LightActor");
					NewActor->TRANS->SetPos(m_vecLoadActor[k]->ActorPos);
					NewActor->TRANS->SetScale(m_vecLoadActor[k]->ActorSize);
					NewActor->TRANS->SetRotate(m_vecLoadActor[k]->ActorRot);

					ActorSaveData::LightSaveData TempData = m_vecLoadActor[k]->LightData;

					SPTR<CLight> TempLight = NewActor->AddCom<CLight>(LIGHT_DATA{
						TempData.Layer,
						(LIGHT_TYPE)TempData.LightBuffer.LightType,
						TempData.VolumeCheck,
						TempData.MeshName });

					TempLight->SetLightBuffer(TempData.LightBuffer);
					TempLight->SetLightSize(TempData.LightSubSize);
					TempLight->SetLightDir(TempData.LightBuffer.LightDir.vec3);
				}
				//마법진 엑터
				else if (true == IsCircle)
				{
					SPTR<CActor> NewActor = SSDToolGlobal::ToolGlobalScene->CreateActor(SSDToolGlobal::ToolGlobalScene, Layer, L"LightActor");
					NewActor->TRANS->SetPos(m_vecLoadActor[k]->ActorPos);
					NewActor->TRANS->SetScale(m_vecLoadActor[k]->ActorSize);
					NewActor->TRANS->SetRotate(m_vecLoadActor[k]->ActorRot);

					SPTR<CMagicCircle> TempCircle = NewActor->AddCom<CMagicCircle>(m_vecLoadActor[k]->MagicCircleColor);
					TempCircle->GetMagicCircleRen()->SetRenSize(m_vecLoadActor[k]->MagicCircleScale);
					TempCircle->GetMagicCircleRen()->SetRenPivot(m_vecLoadActor[k]->MagicCirclePivot);
				}
				else
				{
					SPTR<CActor> NewActor = SSDToolGlobal::ToolGlobalScene->CreateActor(SSDToolGlobal::ToolGlobalScene, Layer, L"NewActor");
					NewActor->TRANS->SetPos(m_vecLoadActor[k]->ActorPos);
					NewActor->TRANS->SetScale(m_vecLoadActor[k]->ActorSize);
					NewActor->TRANS->SetRotate(m_vecLoadActor[k]->ActorRot);

					//랜더러
					std::vector<PrefabData::FbxSaveData> FbxVector = m_vecLoadActor[k]->vecFbxData;
					for (size_t i = 0; i < FbxVector.size(); ++i)
					{
						SPTR<CFbxRenderer> TempRender = NewActor->AddCom<CFbxRenderer>(RENDATA{ RENDER_LAYER_PLAYER });
						TempRender->SetRenSize(m_vecLoadActor[k]->vecFbxData[i].RenderSize);
						TempRender->SetRenPivot(m_vecLoadActor[k]->vecFbxData[i].RenderPos);

						TempRender->SetFbx(m_vecLoadActor[k]->vecFbxData[i].MeshName.c_str());
						if (true == m_vecLoadActor[k]->vecFbxData[i].AllCheck)
						{
							TempRender->SetAllFbxMesh();
						}
						else if (false == m_vecLoadActor[k]->vecFbxData[i].AllCheck)
						{
							std::map<int, std::wstring>::iterator StartIter = m_vecLoadActor[k]->vecFbxData[i].MatInfoMap.begin();
							std::map<int, std::wstring>::iterator EndIter = m_vecLoadActor[k]->vecFbxData[i].MatInfoMap.end();
							for (; StartIter != EndIter; ++StartIter)
							{
								TempRender->SettingFbxMesh(StartIter->first);
							}
						}
					}

					//충돌체
					std::vector<PrefabData::ColSaveData> ColSaveVector = m_vecLoadActor[k]->vecColData;
					for (size_t j = 0; j < ColSaveVector.size(); ++j)
					{
						if (COL_TYPE::COLTYPE_OBB == m_vecLoadActor[k]->vecColData[j].ColType)
						{
							SPTR<COBBCollider> TempColl = NewActor->
								AddCom<COBBCollider>(COLDATA{ m_vecLoadActor[k]->vecColData[j].ColGroupName, COLLISION_3D });
							TempColl->SetColSize(m_vecLoadActor[k]->vecColData[j].ColSize);
							TempColl->SetColPivot(m_vecLoadActor[k]->vecColData[j].ColPos);
						}
						else if (COL_TYPE::COLTYPE_SPHERE == m_vecLoadActor[k]->vecColData[j].ColType)
						{
							SPTR<CSphereCollider> TempColl = NewActor->
								AddCom<CSphereCollider>(COLDATA{ m_vecLoadActor[k]->vecColData[j].ColGroupName, COLLISION_3D });
							TempColl->SetColSize(m_vecLoadActor[k]->vecColData[j].ColSize);
							TempColl->SetColPivot(m_vecLoadActor[k]->vecColData[j].ColPos);
						}
					}

					// 네비게이션 메쉬
					std::vector<PrefabData::NavigationData>& NaviDataVec = m_vecLoadActor[k]->vecNaviData;
					size_t NaviCount = NaviDataVec.size();
					for (size_t i = 0; i < NaviCount; i++)
					{

						SPTR<CNavigationObject> NaviObj = NewActor->AddCom<CNavigationObject>();

						NaviObj->CreateNaviArea(DBD::pMainTerrain);
						NaviObj->CreateNavigationRenderer(NAVIMESH_DATA{ (RENDER_LAYER)NaviDataVec[i].RenderLayer, NaviDataVec[i].NaviSerialNumber, NaviDataVec[i].NaviObjName });
						NaviObj->GetNaviRen()->CreateNaviMeshCollider(L"NaviTri");
						NaviObj->CreateNavigationMesh(NaviDataVec[i].NaviObjName.c_str(), NAVIMESH_OBJ);
					}
				}
			}
		}
	}
	else if (_Version == TERRAIN_MAGIC_DEFERRED_FILE)
	{
		// 1. 파일에서 읽어들인 액터 정보를 적용
		for (size_t k = 0; k < m_vecLoadActor.size(); ++k)
		{
			if (nullptr != m_vecLoadActor[k])
			{
				ACTOR_UPDATE_LAYER Layer = (ACTOR_UPDATE_LAYER)m_vecLoadActor[k]->UpdateLayer;
				bool IsLight = m_vecLoadActor[k]->IsLight;
				bool IsCircle = m_vecLoadActor[k]->IsMagicCircle;

				//라이트 엑터
				if (true == IsLight)
				{
					SPTR<CActor> NewActor = SSDToolGlobal::ToolGlobalScene->CreateActor(SSDToolGlobal::ToolGlobalScene, Layer, L"LightActor");
					NewActor->TRANS->SetPos(m_vecLoadActor[k]->ActorPos);
					NewActor->TRANS->SetScale(m_vecLoadActor[k]->ActorSize);
					NewActor->TRANS->SetRotate(m_vecLoadActor[k]->ActorRot);

					ActorSaveData::LightSaveData TempData = m_vecLoadActor[k]->LightData;

					SPTR<CLight> TempLight = NewActor->AddCom<CLight>(LIGHT_DATA{
						TempData.Layer,
						(LIGHT_TYPE)TempData.LightBuffer.LightType,
						TempData.VolumeCheck,
						TempData.MeshName });

					TempLight->SetLightBuffer(TempData.LightBuffer);
					TempLight->SetLightSize(TempData.LightSubSize);
					TempLight->SetLightDir(TempData.LightBuffer.LightDir.vec3);

					if (0 == CSceneRenderMgr::FLightBuffer.LightCount)
					{
						CSceneRenderMgr::FLightBuffer.ArrLightData[0] = TempLight->m_LightBufferData;
						CSceneRenderMgr::FLightBuffer.LightCount = 1;
					}
				}
				//마법진 엑터
				else if (true == IsCircle)
				{
					SPTR<CActor> NewActor = SSDToolGlobal::ToolGlobalScene->CreateActor(SSDToolGlobal::ToolGlobalScene, Layer, L"LightActor");
					NewActor->TRANS->SetPos(m_vecLoadActor[k]->ActorPos);
					NewActor->TRANS->SetScale(m_vecLoadActor[k]->ActorSize);
					NewActor->TRANS->SetRotate(m_vecLoadActor[k]->ActorRot);

					SPTR<CMagicCircle> TempCircle = NewActor->AddCom<CMagicCircle>(m_vecLoadActor[k]->MagicCircleColor);
					TempCircle->GetMagicCircleRen()->SetRenSize(m_vecLoadActor[k]->MagicCircleScale);
					TempCircle->GetMagicCircleRen()->SetRenPivot(m_vecLoadActor[k]->MagicCirclePivot);
				}
				else
				{
					SPTR<CActor> NewActor = SSDToolGlobal::ToolGlobalScene->CreateActor(SSDToolGlobal::ToolGlobalScene, Layer, L"NewActor");
					NewActor->TRANS->SetPos(m_vecLoadActor[k]->ActorPos);
					NewActor->TRANS->SetScale(m_vecLoadActor[k]->ActorSize);
					NewActor->TRANS->SetRotate(m_vecLoadActor[k]->ActorRot);

					//랜더러
					std::vector<PrefabData::FbxSaveData> FbxVector = m_vecLoadActor[k]->vecFbxData;
					for (size_t i = 0; i < FbxVector.size(); ++i)
					{
						SPTR<CFbxRenderer> TempRender = NewActor->AddCom<CFbxRenderer>(RENDATA{ RENDER_LAYER_PLAYER });
						TempRender->SetRenSize(FbxVector[i].RenderSize);
						TempRender->SetRenPivot(FbxVector[i].RenderPos);

						TempRender->SetFbx(FbxVector[i].MeshName.c_str());
						if (true == FbxVector[i].AllCheck)
						{
							if (true == FbxVector[i].IsDeferred)
							{
								TempRender->SetAllFbxMesh();
							}
							else
							{
								TempRender->SetAllFbxMesh(L"ForwardFBXMaterial");
							}
						}
						else if (false == FbxVector[i].AllCheck)
						{
							std::map<int, std::wstring>::iterator StartIter = FbxVector[i].MatInfoMap.begin();
							std::map<int, std::wstring>::iterator EndIter = FbxVector[i].MatInfoMap.end();
							for (; StartIter != EndIter; ++StartIter)
							{
								TempRender->SettingFbxMesh(StartIter->first);
								//if (true == FbxVector[i].IsDeferred)
								//{
								//	TempRender->SettingFbxMesh(L"ForwardFBXMaterial", StartIter->first);
								//}
								//else
								//{
								//	TempRender->SettingFbxMesh(L"ForwardFBXMaterial", StartIter->first);
								//}
							}
						}

						for (UINT m = 0; m < TempRender->GetFbxMesh(0)->GetMatSize(); ++m)
						{
							TempRender->GetFbxMesh(0)->GetMatStart(m)->SetDeferred(FbxVector[i].IsDeferred);
						}
					}

					//충돌체
					std::vector<PrefabData::ColSaveData> ColSaveVector = m_vecLoadActor[k]->vecColData;
					for (size_t j = 0; j < ColSaveVector.size(); ++j)
					{
						if (COL_TYPE::COLTYPE_OBB == m_vecLoadActor[k]->vecColData[j].ColType)
						{
							SPTR<COBBCollider> TempColl = NewActor->
								AddCom<COBBCollider>(COLDATA{ m_vecLoadActor[k]->vecColData[j].ColGroupName, COLLISION_3D });
							TempColl->SetColSize(m_vecLoadActor[k]->vecColData[j].ColSize);
							TempColl->SetColPivot(m_vecLoadActor[k]->vecColData[j].ColPos);
						}
						else if (COL_TYPE::COLTYPE_SPHERE == m_vecLoadActor[k]->vecColData[j].ColType)
						{
							SPTR<CSphereCollider> TempColl = NewActor->
								AddCom<CSphereCollider>(COLDATA{ m_vecLoadActor[k]->vecColData[j].ColGroupName, COLLISION_3D });
							TempColl->SetColSize(m_vecLoadActor[k]->vecColData[j].ColSize);
							TempColl->SetColPivot(m_vecLoadActor[k]->vecColData[j].ColPos);
						}
					}

					// 네비게이션 메쉬
					std::vector<PrefabData::NavigationData>& NaviDataVec = m_vecLoadActor[k]->vecNaviData;
					size_t NaviCount = NaviDataVec.size();
					for (size_t i = 0; i < NaviCount; i++)
					{

						SPTR<CNavigationObject> NaviObj = NewActor->AddCom<CNavigationObject>();

						NaviObj->CreateNaviArea(DBD::pMainTerrain);
						NaviObj->CreateNavigationRenderer(NAVIMESH_DATA{ (RENDER_LAYER)NaviDataVec[i].RenderLayer, NaviDataVec[i].NaviSerialNumber, NaviDataVec[i].NaviObjName });
						NaviObj->GetNaviRen()->CreateNaviMeshCollider(L"NaviTri");
						NaviObj->CreateNavigationMesh(NaviDataVec[i].NaviObjName.c_str(), NAVIMESH_OBJ);
					}
				}
			}
		}
	}

	m_vecLoadActor.clear();

}

void ControlDlg::SaveFilter(const CString& _Path, SPTR<class CCamera> _SaveCam)
{
	m_vecFilter.clear();


	//카메라 컴포넌트이면 필터 포문 돌면서 필터정보 넣어주기
	if (nullptr != _SaveCam)
	{
		m_listFilter = _SaveCam->GetCamFilterList();

		m_StartList = m_listFilter.begin();
		m_EndList = m_listFilter.end();
		for (; m_StartList != m_EndList; ++m_StartList)
		{
			SPTR<FilterSaveData> NewSaveData = new FilterSaveData();
			if (true == (*m_StartList)->EqualType<CGaussianBlur>())
			{
				FilterSaveData::GaussianSaveData TempSaveData = FilterSaveData::GaussianSaveData();
				SPTR<CGaussianBlur> TempGau = (*m_StartList);

				TempSaveData.GaussianBuffer = TempGau->GetGaussianBuffer();
				TempSaveData.Sigma = TempGau->GetSigma();

				NewSaveData->listGauData.push_back(TempSaveData);
			}
			else if (true == (*m_StartList)->EqualType<CBloomFilter>())
			{
				FilterSaveData::BloomSaveData TempSaveData = FilterSaveData::BloomSaveData();
				SPTR<CBloomFilter> TempBloom = (*m_StartList);

				TempSaveData.BloomBuffer = TempBloom->GetBloomBuffer();
				TempSaveData.BloomBufferSize = TempBloom->GetBloomBufferSize();
				TempSaveData.BloomResNum = TempBloom->GetBloomResNum();
				TempSaveData.BufferType = TempBloom->GetBloomMiniBufferType();
				TempSaveData.MiniBuffer = TempBloom->GetMiniBuffer();
				TempSaveData.Sigma = TempBloom->GetSigma();

				NewSaveData->listBloomData.push_back(TempSaveData);
			}
			m_vecFilter.push_back(NewSaveData);
		}
	}

	SaveBinaryFilter(_Path);
}

void ControlDlg::SaveBinaryFilter(const CString & _Path)
{
	CWriteStream WriteData = CWriteStream(_Path);

	int FilterSize = (int)m_vecFilter.size();
	WriteData.Write(FilterSize);
	for (size_t i = 0; i < FilterSize; ++i)
	{
		std::list<FilterSaveData::GaussianSaveData> TempGauList = m_vecFilter[i]->listGauData;
		int GauSize = (int)TempGauList.size();
		WriteData.Write(GauSize);

		if (0 != GauSize)
		{
			std::list<FilterSaveData::GaussianSaveData>::iterator GSIter = TempGauList.begin();
			std::list<FilterSaveData::GaussianSaveData>::iterator GEIter = TempGauList.end();
			for (; GSIter != GEIter; ++GSIter)
			{
				WriteData.Write((*GSIter).Sigma);
				WriteData.Write((*GSIter).GaussianBuffer);
			}
		}

		std::list<FilterSaveData::BloomSaveData> TempBloomList = m_vecFilter[i]->listBloomData;
		int BloomSize = (int)TempBloomList.size();
		WriteData.Write(BloomSize);
		if (0 != BloomSize)
		{
			std::list<FilterSaveData::BloomSaveData>::iterator BSIter = TempBloomList.begin();
			std::list<FilterSaveData::BloomSaveData>::iterator BEIter = TempBloomList.end();
			for (; BSIter != BEIter; ++BSIter)
			{
				WriteData.Write((*BSIter).BloomBuffer);
				WriteData.Write((*BSIter).BloomBufferSize);
				WriteData.Write((*BSIter).BloomResNum);
				WriteData.Write((*BSIter).BufferType);
				WriteData.Write((*BSIter).MiniBuffer);
				WriteData.Write((*BSIter).Sigma);
			}
		}
	}

	m_listFilter.clear();
}

void ControlDlg::LoadFilter(const CString & _Path, SPTR<CCamera> _Cam)
{
	if (nullptr != SSDToolGlobal::ToolSelectBloom)
	{
		SSDToolGlobal::ToolSelectBloom = nullptr;
	}
	if (nullptr != SSDToolGlobal::ToolSelectGaussian)
	{
		SSDToolGlobal::ToolSelectGaussian = nullptr;
	}

	CReadStream ReadData = CReadStream(_Path);
	m_vecFilter.clear();
	m_listDeleteFilter.clear();

	int FilterSize;
	ReadData.Read(FilterSize);

	if (0 == FilterSize)
	{
		return;
	}
	_Cam->ResetCamFilter();

	for (size_t i = 0; i < FilterSize; i++)
	{
		int GauSize;
		ReadData.Read(GauSize);
		if (0 != GauSize)
		{
			float Sigma;
			ReadData.Read(Sigma);
			CBUFFER::GaussianBuffer GauBuffer;
			ReadData.Read(GauBuffer);

			SPTR<CGaussianBlur> NewBlur = _Cam->AddFilter<CGaussianBlur>();
			NewBlur->SetGaussianBuffer((float)Sigma, (int)GauBuffer.Range);
			SSDToolGlobal::ToolSelectGaussian = NewBlur;
		}

		int BloomSize;
		ReadData.Read(BloomSize);
		if (0 != BloomSize)
		{
			CBUFFER::BloomBuffer BloomBuffer;
			ReadData.Read(BloomBuffer);
			float BloomBufferSize;
			ReadData.Read(BloomBufferSize);
			UINT BloomResNum;
			ReadData.Read(BloomResNum);
			MINIMIZE_BUFFER_TYPE BufferType;
			ReadData.Read(BufferType);
			CBUFFER::MinimizeBuffer MiniBuffer;
			ReadData.Read(MiniBuffer);
			float Sigma;
			ReadData.Read(Sigma);

			SPTR<CBloomFilter> NewBloom = _Cam->AddFilter<CBloomFilter>((MINIMIZE_BUFFER_TYPE)BufferType);
			NewBloom->SetGaussianBuffer(Sigma, BloomBuffer.Range);
			NewBloom->SetMiniBufferAmount(MiniBuffer.Amount);
			NewBloom->SetMiniBufferLuminance(MiniBuffer.Luminance);
			NewBloom->SetMiniBufferMultiple(MiniBuffer.Multiple);
			NewBloom->SetBloomResTarget(_Cam);
			SSDToolGlobal::ToolSelectBloom = NewBloom;
		}
	}
	if (nullptr != SSDToolGlobal::ToolSceneList)
	{
		SSDToolGlobal::ToolSceneList->OnBnClickedReset();
	}
}

void ControlDlg::OnBnClickedCamload()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (nullptr == SSDToolGlobal::ToolSelectActor)
	{
		return;
	}
	SPTR<CCamera> TempCam = SSDToolGlobal::ToolSelectActor->FindCom<CCamera>();
	if (nullptr == TempCam)
	{
		return;
	}

	CFileDialog fDlg(TRUE, L".camera", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, L"All Files(*.*)|*.*||");
	if (fDlg.DoModal() == IDOK)
	{
		CString PathName = fDlg.GetPathName();
		LoadFilter(PathName, TempCam);
	}
}


void ControlDlg::OnBnClickedCollmove()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (true == SSDToolGlobal::CollEdit)
	{
		SSDToolGlobal::CollEdit = false;
	}
	else
	{
		SSDToolGlobal::CollEdit = true;
	}
}


void ControlDlg::OnBnClickedGizmo()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (nullptr == SSDToolGlobal::ToolMainGizmo)
	{
		return;
	}
	SSDToolGlobal::ToolMainGizmo->GizmoOn();
}


void ControlDlg::OnBnClickedGizmo2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (nullptr == SSDToolGlobal::ToolMainGizmo)
	{
		return;
	}
	SSDToolGlobal::ToolMainGizmo->GizmoOff();
}


void ControlDlg::OnBnClickedNavi()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (nullptr == SSDToolGlobal::ToolSelectActor)
	{
		return;
	}


	if (nullptr == SSDToolGlobal::ToolSelectActor->FindCom<CNavigationObject>())
	{
		SPTR<CNavigationObject> NaviObj = SSDToolGlobal::ToolSelectActor->AddCom<CNavigationObject>();

		if (nullptr == NaviObj->ACTOR->FindCom<CFbxRenderer>())
		{
			NaviObj->CreateFbxRenderer(RENDATA{ RENDER_LAYER_PLAYER });
		}

		NaviObj->CreateNavigationRenderer();
		NaviObj->CreateNaviArea(DBD::pMainTerrain);
		NaviObj->GetNaviRen()->CreateNaviMeshCollider(L"NaviTri");
		SSDToolGlobal::setToolNaviObj.insert(NaviObj);
	}

	SSDToolGlobal::ToolEditDlg->CreateNaviDlg();
	SSDToolGlobal::ToolSceneList->OnBnClickedReset();

}


void ControlDlg::OnBnClickedNavidelete()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	SSDToolGlobal::ToolEditDlg->DeleteNaviDlg();
}



void ControlDlg::OnBnClickedPlaybutton1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	SSDToolGlobal::ToolMainScene->ChangePlayerVision();
}


void ControlDlg::OnBnClickedPausebutton1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void ControlDlg::OnBnClickedStopbutton1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void ControlDlg::OnBnClickedFog()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	if (nullptr == SSDToolGlobal::ToolEditDlg)
	{
		return;
	}

	if (nullptr == SSDToolGlobal::ToolFogDlg)
	{
		SSDToolGlobal::ToolEditDlg->CreateFogDlg();
		SSDToolGlobal::ToolSceneList->OnBnClickedReset();
	}
	else
	{
		SSDToolGlobal::ToolEditDlg->DeleteFogDlg();
	}
}

void ControlDlg::OnBnClickedAddcoll()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CFileDialog fDlg(TRUE, _T(".scene"), NULL, OFN_NOCHANGEDIR | OFN_OVERWRITEPROMPT, L"All Files(*.*)|*.*||");
	if (fDlg.DoModal() == IDOK)
	{
		CString PathName = fDlg.GetPathName();
		ReadScene(PathName);
	}

	//프리펩 데이터 읽어오기
	ReadCollPreData();
	m_mapAddCollPreData;

	//엑터 fbx랜더러 정보 읽어오기
	m_mapAllLoadActor = SSDToolGlobal::ToolGlobalScene->GetAllActor();
	std::map<int, std::list<SPTR<CActor>>>::iterator Startiter = m_mapAllLoadActor.begin();
	std::map<int, std::list<SPTR<CActor>>>::iterator Enditer = m_mapAllLoadActor.end();
	for (; Startiter != Enditer; ++Startiter)
	{
		std::list<SPTR<CActor>>::iterator ListStartIter = Startiter->second.begin();
		std::list<SPTR<CActor>>::iterator ListEndIter = Startiter->second.end();
		for (; ListStartIter != ListEndIter; ++ListStartIter)
		{
			//fbx랜더러 정보
			m_listAddCollFbx.clear();
			(*ListStartIter)->FindAllCom<CFbxRenderer>(&m_listAddCollFbx);
			if (0 < m_listAddCollFbx.size())
			{
				std::list<SPTR<CFbxRenderer>>::iterator RenStartIter = m_listAddCollFbx.begin();
				std::list<SPTR<CFbxRenderer>>::iterator RenEndIter = m_listAddCollFbx.end();
				for (; RenStartIter != RenEndIter; ++RenStartIter)
				{
					std::wstring TempName = (*RenStartIter)->GetRenderingFbx()->GetFileName();

					if (0 != m_mapAddCollPreData.size())
					{
						//프리펩 데이터에서 메쉬와 같은 이름의 충돌체 찾기
						std::map<std::wstring, SPTR<PrefabData>>::iterator FindIter = m_mapAddCollPreData.find(TempName);
						if (m_mapAddCollPreData.end() == FindIter)
						{
							continue;
						}

						//충돌체 넣기
						std::vector<PrefabData::ColSaveData> ColSaveVector = FindIter->second->vecColData;
						for (size_t j = 0; j < ColSaveVector.size(); ++j)
						{
							if (COL_TYPE::COLTYPE_OBB == FindIter->second->vecColData[j].ColType)
							{
								SPTR<COBBCollider> TempColl = (*ListStartIter)->
									AddCom<COBBCollider>(COLDATA{ FindIter->second->vecColData[j].ColGroupName, COLLISION_3D });
								TempColl->SetColSize(FindIter->second->vecColData[j].ColSize);
								//TempColl->SetColPivot(FindIter->second->vecColData[j].ColPos);
								TempColl->SetColPivot((*RenStartIter)->GetRenPivot() + FindIter->second->vecColData[j].ColPos * (*RenStartIter)->GetActor()->GetTrans()->GetScale());
							}
							else if (COL_TYPE::COLTYPE_SPHERE == FindIter->second->vecColData[j].ColType)
							{
								SPTR<CSphereCollider> TempColl = (*ListStartIter)->
									AddCom<CSphereCollider>(COLDATA{ FindIter->second->vecColData[j].ColGroupName, COLLISION_3D });
								TempColl->SetColSize(FindIter->second->vecColData[j].ColSize);
								//TempColl->SetColPivot(FindIter->second->vecColData[j].ColPos);
								TempColl->SetColPivot(((*RenStartIter)->GetRenPivot() + FindIter->second->vecColData[j].ColPos) * (*RenStartIter)->GetActor()->GetTrans()->GetScale());
							}
						}
					}
				}
			}
		}
	}
	m_listAddCollFbx.clear();
	m_mapAddCollPreData.clear();
}

void ControlDlg::ReadCollPreData()
{
	CString TempPath = CFilePathMgr::GetPath(L"Data").c_str();
	CReadStream ReadData = CReadStream(TempPath + L"PreData.predata");

	int MapSize;
	ReadData.Read(MapSize);

	for (int i = 0; i < MapSize; ++i)
	{
		std::wstring First;
		ReadData.Read(First);
		Vec3 Pos;
		ReadData.Read(Pos);
		Vec3 Size;
		ReadData.Read(Size);
		Vec3 Rot;
		ReadData.Read(Rot);

		SPTR<PrefabData> NewData = new PrefabData(Size, Pos, Rot);

		int FbxVecSize;
		ReadData.Read(FbxVecSize);

		NewData->vecFbxData.resize(FbxVecSize);
		for (size_t j = 0; j < NewData->vecFbxData.size(); ++j)
		{
			bool AllCheck;
			ReadData.Read(AllCheck);
			std::wstring MeshName;
			ReadData.Read(MeshName);
			Vec3 RenderPos;
			ReadData.Read(RenderPos);
			Vec3 RenderSize;
			ReadData.Read(RenderSize);

			NewData->vecFbxData[j].AllCheck = AllCheck;
			NewData->vecFbxData[j].MeshName = MeshName;
			NewData->vecFbxData[j].RenderPos = RenderPos;
			NewData->vecFbxData[j].RenderSize = RenderSize;

			int MatMapSize;
			ReadData.Read(MatMapSize);
			for (size_t k = 0; k < MatMapSize; ++k)
			{
				int Index;
				ReadData.Read(Index);
				std::wstring MatName;
				ReadData.Read(MatName);

				//NewData->vecFbxData[j].MatInfoMap.insert(std::map<int, std::wstring>::value_type(Index, MatName));
			}
		}

		int ColVecSize;
		ReadData.Read(ColVecSize);

		NewData->vecColData.resize(ColVecSize);
		for (size_t k = 0; k < ColVecSize; ++k)
		{
			COL_TYPE Type;
			ReadData.Read(Type);
			std::wstring GroupName;
			ReadData.Read(GroupName);
			Vec3 ColPos;
			ReadData.Read(ColPos);
			Vec3 ColSize;
			ReadData.Read(ColSize);

			NewData->vecColData[k].ColType = Type;
			NewData->vecColData[k].ColGroupName = GroupName;
			NewData->vecColData[k].ColPos = ColPos;
			NewData->vecColData[k].ColSize = ColSize;
		}
		m_mapAddCollPreData.insert(std::map<std::wstring, SPTR<PrefabData>>::value_type(First, NewData));
	}
}

void ControlDlg::OnBnClickedMagic()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (nullptr == SSDToolGlobal::ToolSelectActor)
	{
		return;
	}
	//if (nullptr != SSDToolGlobal::ToolObjectDlg)
	//{
	//	SSDToolGlobal::ToolObjectDlg->OnBnClickedReset();
	//}
	SSDToolGlobal::ToolSelectActor->AddCom<CMagicCircle>(MAGICCIRCLE_COLOR::MC_RED);
	SSDToolGlobal::ToolSceneList->OnBnClickedReset();
}


void ControlDlg::OnBnClickedGreen()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (nullptr == SSDToolGlobal::ToolSelectActor)
	{
		return;
	}
	//if (nullptr != SSDToolGlobal::ToolObjectDlg)
	//{
	//	SSDToolGlobal::ToolObjectDlg->OnBnClickedReset();
	//}
	SSDToolGlobal::ToolSelectActor->AddCom<CMagicCircle>(MAGICCIRCLE_COLOR::MC_GREEN);
	SSDToolGlobal::ToolSceneList->OnBnClickedReset();
}

void ControlDlg::SetForward()
{


}


void ControlDlg::OnBnClickedChange()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CFileDialog fDlg(TRUE, _T(".scene"), NULL, OFN_NOCHANGEDIR | OFN_OVERWRITEPROMPT, L"All Files(*.*)|*.*||");
	if (fDlg.DoModal() == IDOK)
	{
		CString PathName = fDlg.GetPathName();
		NewReadScene(PathName);
	}

	//엑터 fbx랜더러 정보 읽어오기
	m_mapChangeActor = SSDToolGlobal::ToolGlobalScene->GetAllActor();
	std::map<int, std::list<SPTR<CActor>>>::iterator Startiter = m_mapChangeActor.begin();
	std::map<int, std::list<SPTR<CActor>>>::iterator Enditer = m_mapChangeActor.end();
	for (; Startiter != Enditer; ++Startiter)
	{
		std::list<SPTR<CActor>>::iterator ListStartIter = Startiter->second.begin();
		std::list<SPTR<CActor>>::iterator ListEndIter = Startiter->second.end();
		for (; ListStartIter != ListEndIter; ++ListStartIter)
		{
			//fbx랜더러 정보
			m_listChangeFbx.clear();
			(*ListStartIter)->FindAllCom<CFbxRenderer>(&m_listChangeFbx);
			if (0 < m_listChangeFbx.size())
			{
				std::list<SPTR<CFbxRenderer>>::iterator RenStartIter = m_listChangeFbx.begin();
				std::list<SPTR<CFbxRenderer>>::iterator RenEndIter = m_listChangeFbx.end();
				for (; RenStartIter != RenEndIter; ++RenStartIter)
				{
					std::wstring TempName = (*RenStartIter)->GetRenderingFbx()->GetFileName();

					/*		if (TempName == L"Bush01")
							{
								for (UINT i = 0; i < (*RenStartIter)->GetFbxMesh(0)->GetMatSize(); i++)
								{
									(*RenStartIter)->GetFbxMesh(0)->GetMatStart(i)->DeferredOff();
								}
							}

							if (TempName == L"Bush02")
							{
								for (UINT i = 0; i < (*RenStartIter)->GetFbxMesh(0)->GetMatSize(); i++)
								{
									(*RenStartIter)->GetFbxMesh(0)->GetMatStart(i)->DeferredOff();
								}
							}

							if (TempName == L"Bush03")
							{
								for (UINT i = 0; i < (*RenStartIter)->GetFbxMesh(0)->GetMatSize(); i++)
								{
									(*RenStartIter)->GetFbxMesh(0)->GetMatStart(i)->DeferredOff();
								}
							}

							if (TempName == L"Bush04")
							{
								for (UINT i = 0; i < (*RenStartIter)->GetFbxMesh(0)->GetMatSize(); i++)
								{
									(*RenStartIter)->GetFbxMesh(0)->GetMatStart(i)->DeferredOff();
								}
							}

							if (TempName == L"Bush05")
							{
								for (UINT i = 0; i < (*RenStartIter)->GetFbxMesh(0)->GetMatSize(); i++)
								{
									(*RenStartIter)->GetFbxMesh(0)->GetMatStart(i)->DeferredOff();
								}
							}

							if (TempName == L"Bush06")
							{
								for (UINT i = 0; i < (*RenStartIter)->GetFbxMesh(0)->GetMatSize(); i++)
								{
									(*RenStartIter)->GetFbxMesh(0)->GetMatStart(i)->DeferredOff();
								}
							}

							if (TempName == L"Red")
							{
								for (UINT i = 0; i < (*RenStartIter)->GetFbxMesh(0)->GetMatSize(); i++)
								{
									(*RenStartIter)->GetFbxMesh(0)->GetMatStart(i)->DeferredOff();
								}
							}

							if (TempName == L"Green")
							{
								for (UINT i = 0; i < (*RenStartIter)->GetFbxMesh(0)->GetMatSize(); i++)
								{
									(*RenStartIter)->GetFbxMesh(0)->GetMatStart(i)->DeferredOff();
								}
							}

							if (TempName == L"LV_Campagna_Tree01")
							{
								for (UINT i = 0; i < (*RenStartIter)->GetFbxMesh(0)->GetMatSize(); i++)
								{
									(*RenStartIter)->GetFbxMesh(0)->GetMatStart(i)->DeferredOff();
								}
							}

							if (TempName == L"LV_Campagna_Tree02")
							{
								for (UINT i = 0; i < (*RenStartIter)->GetFbxMesh(0)->GetMatSize(); i++)
								{
									(*RenStartIter)->GetFbxMesh(0)->GetMatStart(i)->DeferredOff();
								}
							}

							if (TempName == L"LV_Campagna_Tree03")
							{
								for (UINT i = 0; i < (*RenStartIter)->GetFbxMesh(0)->GetMatSize(); i++)
								{
									(*RenStartIter)->GetFbxMesh(0)->GetMatStart(i)->DeferredOff();
								}
							}

							if (TempName == L"LV_Campagna_Tree04")
							{
								for (UINT i = 0; i < (*RenStartIter)->GetFbxMesh(0)->GetMatSize(); i++)
								{
									(*RenStartIter)->GetFbxMesh(0)->GetMatStart(i)->DeferredOff();
								}
							}

							if (TempName == L"LV_Campagna_Tree05")
							{
								for (UINT i = 0; i < (*RenStartIter)->GetFbxMesh(0)->GetMatSize(); i++)
								{
									(*RenStartIter)->GetFbxMesh(0)->GetMatStart(i)->DeferredOff();
								}
							}

							if (TempName == L"LV_DeadForest_Tree_DeadForestTree")
							{
								for (UINT i = 0; i < (*RenStartIter)->GetFbxMesh(0)->GetMatSize(); i++)
								{
									(*RenStartIter)->GetFbxMesh(0)->GetMatStart(i)->DeferredOff();
								}
							}*/

					if (TempName == L"Bush01")
						(*RenStartIter)->SetRenderLayer(RENDER_LAYER::RENDER_LAYER_ALPHA);

					if (TempName == L"Bush02")
						(*RenStartIter)->SetRenderLayer(RENDER_LAYER::RENDER_LAYER_ALPHA);

					if (TempName == L"Bush03")
						(*RenStartIter)->SetRenderLayer(RENDER_LAYER::RENDER_LAYER_ALPHA);

					if (TempName == L"Bush04")
						(*RenStartIter)->SetRenderLayer(RENDER_LAYER::RENDER_LAYER_ALPHA);

					if (TempName == L"Bush05")
						(*RenStartIter)->SetRenderLayer(RENDER_LAYER::RENDER_LAYER_ALPHA);

					if (TempName == L"Bush06")
						(*RenStartIter)->SetRenderLayer(RENDER_LAYER::RENDER_LAYER_ALPHA);

					if (TempName == L"LV_Campagna_Tree01")
						(*RenStartIter)->SetRenderLayer(RENDER_LAYER::RENDER_LAYER_ALPHA);

					if (TempName == L"LV_Campagna_Tree02")
						(*RenStartIter)->SetRenderLayer(RENDER_LAYER::RENDER_LAYER_ALPHA);

					if (TempName == L"LV_Campagna_Tree03")
						(*RenStartIter)->SetRenderLayer(RENDER_LAYER::RENDER_LAYER_ALPHA);

					if (TempName == L"LV_Campagna_Tree04")
						(*RenStartIter)->SetRenderLayer(RENDER_LAYER::RENDER_LAYER_ALPHA);

					if (TempName == L"LV_Campagna_Tree05")
						(*RenStartIter)->SetRenderLayer(RENDER_LAYER::RENDER_LAYER_ALPHA);

					if (TempName == L"LV_DeadForest_Tree_DeadForestTree")
						(*RenStartIter)->SetRenderLayer(RENDER_LAYER::RENDER_LAYER_ALPHA);

				}
			}
		}
	}
	m_listChangeFbx.clear();
}
