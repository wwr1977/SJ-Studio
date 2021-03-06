// SceneListDlg.cpp: 구현 파일
//

#include "stdafx.h"
#include "SSDTool.h"
#include "SceneListDlg.h"
#include "afxdialogex.h"
#include "SSDToolGlobal.h"
#include <ReferenceBase.h>
#include <Actor.h>
#include <TransForm.h>
#include <Renderer.h>
#include <Base3DRenderer.h>
#include <SkeletalFbxRenderer.h>
#include <FbxRenderer.h>
#include <OBBCollider.h>
#include <SphereCollider.h>
#include <Light.h>
#include <FilterEffect.h>
#include <GaussianBlur.h>
#include <BloomFilter.h>
#include <Camera.h>
#include <Gizmo.h>
#include <NavigationObject.h>

#include "ControlDlg.h"
#include "EditDlg.h"
#include "ObjectDlg.h"
#include "LightDlg.h"
#include "FilterDlg.h"
#include "NaviDlg.h"


// SceneListDlg 대화 상자

IMPLEMENT_DYNAMIC(SceneListDlg, CDialogEx)

SceneListDlg::SceneListDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SCENELIST, pParent), TActorCheck(0), TRendererCheck(0), TColliderCheck(0), TLightCheck(0), TFilterCheck(0)
{

}

SceneListDlg::~SceneListDlg()
{
}

void SceneListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SCENEACTORTREE, m_SceneActorTree);
}


BEGIN_MESSAGE_MAP(SceneListDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &SceneListDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &SceneListDlg::OnBnClickedCancel)
	ON_NOTIFY(TVN_SELCHANGED, IDC_SCENEACTORTREE, &SceneListDlg::OnTvnSelchangedTree1)
	ON_BN_CLICKED(IDC_RESET, &SceneListDlg::OnBnClickedReset)
END_MESSAGE_MAP()


// SceneListDlg 메시지 처리기


void SceneListDlg::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//CDialogEx::OnOK();
}


void SceneListDlg::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//CDialogEx::OnCancel();
}

//기즈모 바뀌면 셀렉트 엑터 트렌스폼 바꿔주기
//다이얼로그에 값 업데이트 해주기

void SceneListDlg::OnTvnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;
	DWORD_PTR Ptr = m_SceneActorTree.GetItemData(pNMTreeView->itemNew.hItem);
	CString TempString = m_SceneActorTree.GetItemText(pNMTreeView->itemNew.hItem);
	TActorCheck = TempString.Find(L"Actor");
	TRendererCheck = TempString.Find(L"Renderer");
	TColliderCheck = TempString.Find(L"Collider");
	TLightCheck = TempString.Find(L"Light");
	TFilterCheck = TempString.Find(L"Filter");
	TNaviCheck = TempString.Find(L"Navigation");

	if (nullptr != SSDToolGlobal::ToolSelectActor)
	{
		SPTR<CGizmo> CurGizmo = SSDToolGlobal::ToolSelectActor->FindCom<CGizmo>();
		if (nullptr != CurGizmo)
		{
			CurGizmo->GizmoOff();
		}

	}

	if (-1 != TActorCheck)
	{
		CActor* ActorPtr = (CActor*)Ptr;
		SSDToolGlobal::SetSelectActor(ActorPtr);

		if (nullptr != SSDToolGlobal::ToolSelectActor && nullptr != SSDToolGlobal::ToolControlDlg)
		{
			if (true == SSDToolGlobal::ToolSelectActor->IsActive())
			{
				SSDToolGlobal::ToolControlDlg->ActorRadioOn();
			}
			else
			{
				SSDToolGlobal::ToolControlDlg->ActorRadioOff();
			}
		}

		if (nullptr != SSDToolGlobal::ToolEditDlg)
		{
			SSDToolGlobal::ToolEditDlg->SetRenderTransValue();
			SSDToolGlobal::ToolEditDlg->SetActorTransValue();
			SSDToolGlobal::ToolEditDlg->SetCollTransValue();
		}
		if (nullptr != SSDToolGlobal::ToolObjectDlg)
		{
			if (nullptr == ActorPtr)
			{
				return;
			}
			SSDToolGlobal::ToolObjectDlg->SetMeshList();
		}

		if (nullptr != SSDToolGlobal::ToolSelectActor)
		{
			SPTR<CGizmo> TempGiz = SSDToolGlobal::ToolSelectActor->FindCom<CGizmo>();
			if (nullptr == TempGiz)
			{
				TempGiz = SSDToolGlobal::ToolSelectActor->AddCom<CGizmo>(RENDATA{ RENDER_LAYER_DEBUG });
			}
			TempGiz->GizmoOn();
		}

		SSDToolGlobal::SetSelectNavigationCom(ActorPtr);
		
	}
	else if (-1 != TRendererCheck)
	{
		CBaseComponent* ComPtr = ActorCheck(Ptr);
		if (nullptr == ComPtr)
		{
			return;
		}

		if (nullptr == SSDToolGlobal::ToolEditDlg)
		{
			return;
		}

		if (true == ComPtr->EqualType<CBase3DRenderer>() ||
			true == ComPtr->EqualType<CSkeletalFbxRenderer>() ||
			true == ComPtr->EqualType<CFbxRenderer>())
		{
			SSDToolGlobal::ToolSelectRenderer = (CRenderer*)ComPtr;
			SSDToolGlobal::SetSelectActor(SSDToolGlobal::ToolSelectRenderer->GetActor());

			if (nullptr != SSDToolGlobal::ToolEditDlg)
			{
				SSDToolGlobal::ToolEditDlg->SetRenderTransValue();
				SSDToolGlobal::ToolEditDlg->SetActorTransValue();
			}
		}

		if (nullptr != SSDToolGlobal::ToolSelectActor)
		{
			SPTR<CGizmo> TempGiz = SSDToolGlobal::ToolSelectActor->FindCom<CGizmo>();
			if (nullptr == TempGiz)
			{
				TempGiz = SSDToolGlobal::ToolSelectActor->AddCom<CGizmo>(RENDATA{ RENDER_LAYER_DEBUG });
			}
			TempGiz->GizmoOn();
		}
	}
	else if (-1 != TColliderCheck)
	{
		CBaseComponent* ComPtr = ActorCheck(Ptr);
		if (nullptr == ComPtr)
		{
			return;
		}

		if (nullptr == SSDToolGlobal::ToolEditDlg)
		{
			return;
		}

		if (true == ComPtr->EqualType<COBBCollider>() ||
			true == ComPtr->EqualType<CSphereCollider>())
		{
			SSDToolGlobal::ToolSelectCollider = (CCollider*)ComPtr;
			SSDToolGlobal::SetSelectActor(SSDToolGlobal::ToolSelectCollider->GetActor());

			if (nullptr != SSDToolGlobal::ToolEditDlg)
			{
				SSDToolGlobal::ToolEditDlg->SetCollTransValue();
				SSDToolGlobal::ToolEditDlg->SetActorTransValue();
			}
		}

		if (nullptr != SSDToolGlobal::ToolSelectActor)
		{
			SPTR<CGizmo> TempGiz = SSDToolGlobal::ToolSelectActor->FindCom<CGizmo>();
			if (nullptr == TempGiz)
			{
				TempGiz = SSDToolGlobal::ToolSelectActor->AddCom<CGizmo>(RENDATA{ RENDER_LAYER_DEBUG });
			}
			TempGiz->GizmoOn();
		}
	}
	else if (-1 != TLightCheck)
	{
		CBaseComponent* ComPtr = ActorCheck(Ptr);
		if (nullptr == ComPtr)
		{
			return;
		}

		if (nullptr == SSDToolGlobal::ToolEditDlg)
		{
			return;
		}

		if (true == ComPtr->EqualType<CLight>())
		{
			SSDToolGlobal::ToolSelectLight = (CLight*)ComPtr;
			SSDToolGlobal::ToolEditDlg->DeleteLightDlg();
			SSDToolGlobal::ToolEditDlg->CreateLightDlg();
			SSDToolGlobal::ToolLightDlg->SetCurLightInfo();
		}
	}
	else if (-1 != TFilterCheck)
	{
		CFilterEffect* ComPtr = FilterCheck(Ptr);

		if (nullptr == SSDToolGlobal::ToolEditDlg)
		{
			return;
		}

		if (true == ComPtr->EqualType<CGaussianBlur>())
		{
			SSDToolGlobal::ToolSelectGaussian = (CGaussianBlur*)Ptr;
			SSDToolGlobal::ToolSelectBloom = nullptr;

			SSDToolGlobal::ToolEditDlg->DeleteFilterDlg();
			SSDToolGlobal::ToolEditDlg->CreateFilterDlg();
			SSDToolGlobal::ToolFilterDlg->SetCurGaussianInfo();
		}
		else if (true == ComPtr->EqualType<CBloomFilter>())
		{
			SSDToolGlobal::ToolSelectBloom = (CBloomFilter*)Ptr;
			SSDToolGlobal::ToolSelectGaussian = nullptr;

			SSDToolGlobal::ToolEditDlg->DeleteFilterDlg();
			SSDToolGlobal::ToolEditDlg->CreateFilterDlg();
			SSDToolGlobal::ToolFilterDlg->SetCurBloomInfo();
		}
	}
	else if (-1 != TNaviCheck)
	{
		CBaseComponent* NaviCom = ActorCheck(Ptr);
		if (nullptr == SSDToolGlobal::ToolNaviDlg)
		{
			SSDToolGlobal::ToolEditDlg->CreateNaviDlg();
		}
		SSDToolGlobal::SetSelectNavigationCom(SSDToolGlobal::ToolSelectActor);
		SSDToolGlobal::ToolNaviDlg->UpdataNaviDlgData();
	}
}

CBaseComponent* SceneListDlg::ActorCheck(DWORD_PTR _Ptr)
{
	CBaseComponent* ComPtr = (CBaseComponent*)_Ptr;
	if (nullptr == ComPtr)
	{
		return nullptr;
	}

	SSDToolGlobal::SetSelectActor(ComPtr->GetActor());

	if (nullptr != SSDToolGlobal::ToolSelectActor && nullptr != SSDToolGlobal::ToolControlDlg)
	{
		if (true == SSDToolGlobal::ToolSelectActor->IsActive())
		{
			SSDToolGlobal::ToolControlDlg->ActorRadioOn();
		}
		else
		{
			SSDToolGlobal::ToolControlDlg->ActorRadioOff();
		}
	}
	return ComPtr;
}

CFilterEffect* SceneListDlg::FilterCheck(DWORD_PTR _Ptr)
{
	CFilterEffect* ComPtr = (CFilterEffect*)_Ptr;
	if (nullptr == ComPtr)
	{
		return nullptr;
	}

	SSDToolGlobal::SetSelectActor(ComPtr->GetComponent()->GetActor());

	if (nullptr != SSDToolGlobal::ToolSelectActor && nullptr != SSDToolGlobal::ToolControlDlg)
	{
		if (true == SSDToolGlobal::ToolSelectActor->IsActive())
		{
			SSDToolGlobal::ToolControlDlg->ActorRadioOn();
		}
		else
		{
			SSDToolGlobal::ToolControlDlg->ActorRadioOff();
		}
	}
	return ComPtr;
}

void SceneListDlg::OnBnClickedReset()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	m_SceneActorTree.DeleteAllItems();

	HTREEITEM FirstItem = m_SceneActorTree.InsertItem(L"CurScene");
	m_AllActorMap = SSDToolGlobal::ToolGlobalScene->GetAllActor();
	HTREEITEM KeyItem = nullptr;

	m_StartIter = m_AllActorMap.begin();
	m_EndIter = m_AllActorMap.end();
	for (; m_StartIter != m_EndIter; ++m_StartIter)
	{
		char Buf[256];
		_itoa_s(m_StartIter->first, Buf, 10);
		std::string TempKey = std::string(Buf);

		std::wstring KeyName;
		KeyName.assign(TempKey.begin(), TempKey.end());

		LPCTSTR Key = KeyName.c_str();
		KeyItem = m_SceneActorTree.InsertItem(Key, FirstItem);

		m_ActorList = m_StartIter->second;
		m_StartList = m_ActorList.begin();
		m_EndList = m_ActorList.end();
		for (; m_StartList != m_EndList; ++m_StartList)
		{
			CActor* TempActor = *(m_StartList);

			std::wstring KeyName = TempActor->GetName();
			LPCTSTR Key = KeyName.c_str();
			HTREEITEM ActorItem = m_SceneActorTree.InsertItem(Key, KeyItem);

			DWORD_PTR TempPtr = (DWORD_PTR)TempActor;
			m_SceneActorTree.SetItemData(ActorItem, TempPtr);

			std::list<SPTR<class CBase3DRenderer>> mBaseList = Base3DCheck(TempActor);
			std::list<SPTR<class CSkeletalFbxRenderer>> mSkelList = SkeletalFbxCheck(TempActor);
			std::list<SPTR<class CFbxRenderer>> mFbxList = FbxCheck(TempActor);
			std::list<SPTR<class COBBCollider>> mObbList = ObbCollCheck(TempActor);
			std::list<SPTR<class CSphereCollider>> mSphereList = SphereCollCheck(TempActor);
			std::list<SPTR<class CLight>> mLightList = LightCheck(TempActor);
			std::list<SPTR<class CGaussianBlur>> mCGaussianBlur = GaussianBlurCheck(TempActor);
			std::list<SPTR<class CBloomFilter>> mBloomFilter = BloomFilterCheck(TempActor);
			std::list<SPTR<CNavigationMesh>> mNaviMesh = NavigationMeshCheck(TempActor);
			std::list<SPTR<CNaviMeshArea>> mNaviArea = NavigationAreaCheck(TempActor);

			if (0 < mBaseList.size())
			{
				std::list<SPTR<class CBase3DRenderer>>::iterator StartIter = mBaseList.begin();
				std::list<SPTR<class CBase3DRenderer>>::iterator EndIter = mBaseList.end();
				for (; StartIter != EndIter; ++StartIter)
				{
					HTREEITEM ComItem = m_SceneActorTree.InsertItem(L"Base3DRenderer", ActorItem);
					DWORD_PTR ComTempPtr = (DWORD_PTR)(*StartIter).GetPtr();
					m_SceneActorTree.SetItemData(ComItem, ComTempPtr);
				}
			}

			if (0 < mSkelList.size())
			{
				std::list<SPTR<class CSkeletalFbxRenderer>>::iterator StartIter = mSkelList.begin();
				std::list<SPTR<class CSkeletalFbxRenderer>>::iterator EndIter = mSkelList.end();
				for (; StartIter != EndIter; ++StartIter)
				{
					HTREEITEM ComItem = m_SceneActorTree.InsertItem(L"SkeletalFbxRenderer", ActorItem);
					DWORD_PTR ComTempPtr = (DWORD_PTR)(*StartIter).GetPtr();
					m_SceneActorTree.SetItemData(ComItem, ComTempPtr);
				}
			}

			if (0 < mFbxList.size())
			{
				std::list<SPTR<class CFbxRenderer>>::iterator StartIter = mFbxList.begin();
				std::list<SPTR<class CFbxRenderer>>::iterator EndIter = mFbxList.end();
				for (; StartIter != EndIter; ++StartIter)
				{
					HTREEITEM ComItem = m_SceneActorTree.InsertItem(L"FbxRenderer", ActorItem);
					DWORD_PTR ComTempPtr = (DWORD_PTR)(*StartIter).GetPtr();
					m_SceneActorTree.SetItemData(ComItem, ComTempPtr);

					SPTR<CFbx> TempFbx = (*StartIter)->GetRenderingFbx();
					if (nullptr != TempFbx)
					{
						HTREEITEM FbxItem = m_SceneActorTree.InsertItem(TempFbx->GetFileName().c_str(), ComItem);
						DWORD_PTR FbxTempPtr = (DWORD_PTR)TempFbx.GetPtr();
						m_SceneActorTree.SetItemData(FbxItem, FbxTempPtr);
					}
				}
			}

			if (0 < mObbList.size())
			{
				std::list<SPTR<class COBBCollider>>::iterator StartIter = mObbList.begin();
				std::list<SPTR<class COBBCollider>>::iterator EndIter = mObbList.end();
				for (; StartIter != EndIter; ++StartIter)
				{
					if (true != (*StartIter)->ExistTag(NOSAVE))
					{
						HTREEITEM ComItem = m_SceneActorTree.InsertItem(L"OBBCollider", ActorItem);
						DWORD_PTR ComTempPtr = (DWORD_PTR)(*StartIter).GetPtr();
						m_SceneActorTree.SetItemData(ComItem, ComTempPtr);
					}
				}
			}

			if (0 < mSphereList.size())
			{
				std::list<SPTR<class CSphereCollider>>::iterator StartIter = mSphereList.begin();
				std::list<SPTR<class CSphereCollider>>::iterator EndIter = mSphereList.end();
				for (; StartIter != EndIter; ++StartIter)
				{
					HTREEITEM ComItem = m_SceneActorTree.InsertItem(L"SphereCollider", ActorItem);
					DWORD_PTR ComTempPtr = (DWORD_PTR)(*StartIter).GetPtr();
					m_SceneActorTree.SetItemData(ComItem, ComTempPtr);
				}
			}

			if (0 < mLightList.size())
			{
				std::list<SPTR<class CLight>>::iterator StartIter = mLightList.begin();
				std::list<SPTR<class CLight>>::iterator EndIter = mLightList.end();
				for (; StartIter != EndIter; ++StartIter)
				{
					HTREEITEM ComItem = m_SceneActorTree.InsertItem(L"Light", ActorItem);
					DWORD_PTR ComTempPtr = (DWORD_PTR)(*StartIter).GetPtr();
					m_SceneActorTree.SetItemData(ComItem, ComTempPtr);
				}
			}

			if (0 < mCGaussianBlur.size())
			{
				std::list<SPTR<class CGaussianBlur>>::iterator StartIter = mCGaussianBlur.begin();
				std::list<SPTR<class CGaussianBlur>>::iterator EndIter = mCGaussianBlur.end();
				for (; StartIter != EndIter; ++StartIter)
				{
					HTREEITEM ComItem = m_SceneActorTree.InsertItem(L"GaussianBlurFilter", ActorItem);
					DWORD_PTR ComTempPtr = (DWORD_PTR)(*StartIter).GetPtr();
					m_SceneActorTree.SetItemData(ComItem, ComTempPtr);
				}
			}

			if (0 < mBloomFilter.size())
			{
				std::list<SPTR<class CBloomFilter>>::iterator StartIter = mBloomFilter.begin();
				std::list<SPTR<class CBloomFilter>>::iterator EndIter = mBloomFilter.end();
				for (; StartIter != EndIter; ++StartIter)
				{
					HTREEITEM ComItem = m_SceneActorTree.InsertItem(L"BloomFilter", ActorItem);
					DWORD_PTR ComTempPtr = (DWORD_PTR)(*StartIter).GetPtr();
					m_SceneActorTree.SetItemData(ComItem, ComTempPtr);
				}
			}

			if (0 < mNaviMesh.size())
			{
				std::list<SPTR<CNavigationMesh>>::iterator StartIter = mNaviMesh.begin();
				std::list<SPTR<CNavigationMesh>>::iterator EndIter = mNaviMesh.end();
				
				for (; StartIter != EndIter; ++StartIter)
				{
					HTREEITEM ComItem = m_SceneActorTree.InsertItem(L"NavigationMesh", ActorItem);
					DWORD_PTR ComTempPtr = (DWORD_PTR)(*StartIter).GetPtr();
					m_SceneActorTree.SetItemData(ComItem, ComTempPtr);
				}
			}

			if (0 < NaviAreaList.size())
			{
				std::list<SPTR<CNaviMeshArea>>::iterator StartIter = NaviAreaList.begin();
				std::list<SPTR<CNaviMeshArea>>::iterator EndIter = NaviAreaList.end();

				for (; StartIter != EndIter; ++StartIter)
				{
					HTREEITEM ComItem = m_SceneActorTree.InsertItem(L"NavigationArea", ActorItem);
					DWORD_PTR ComTempPtr = (DWORD_PTR)(*StartIter).GetPtr();
					m_SceneActorTree.SetItemData(ComItem, ComTempPtr);
				}
			}

			mBaseList.clear();
			mSkelList.clear();
			mFbxList.clear();
			mObbList.clear();
			mSphereList.clear();
			mLightList.clear();
			mCGaussianBlur.clear();
			mBloomFilter.clear();
			mNaviMesh.clear();
			mNaviArea.clear();

			BaseList.clear();
			SkelList.clear();
			FbxList.clear();
			ObbList.clear();
			SphereList.clear();
			LightList.clear();
			GaussianBlurList.clear();
			BloomList.clear();
			NaviMeshList.clear();
			NaviAreaList.clear();

			m_SceneActorTree.Expand(ActorItem, TVE_EXPAND);
		}
		m_SceneActorTree.Expand(KeyItem, TVE_EXPAND);
	}
	m_SceneActorTree.Expand(FirstItem, TVE_EXPAND);
	UpdateData(FALSE);

}

const std::list<SPTR<CBase3DRenderer>>& SceneListDlg::Base3DCheck(CActor * _Actor)
{
	_Actor->FindAllCom<CBase3DRenderer>(&BaseList);
	return BaseList;
}

const std::list<SPTR<CSkeletalFbxRenderer>>& SceneListDlg::SkeletalFbxCheck(CActor * _Actor)
{
	_Actor->FindAllCom<CSkeletalFbxRenderer>(&SkelList);
	return SkelList;
}

const std::list<SPTR<CFbxRenderer>>& SceneListDlg::FbxCheck(CActor * _Actor)
{
	_Actor->FindAllCom<CFbxRenderer>(&FbxList);
	return FbxList;
}

const std::list<SPTR<COBBCollider>>& SceneListDlg::ObbCollCheck(CActor * _Actor)
{
	_Actor->FindAllCom<COBBCollider>(&ObbList);
	return ObbList;
}

const std::list<SPTR<CSphereCollider>>& SceneListDlg::SphereCollCheck(CActor * _Actor)
{
	_Actor->FindAllCom<CSphereCollider>(&SphereList);
	return SphereList;
}


const std::list<SPTR<CLight>>& SceneListDlg::LightCheck(CActor * _Actor)
{
	_Actor->FindAllCom<CLight>(&LightList);
	return LightList;
}

const std::list<SPTR<CGaussianBlur>>& SceneListDlg::GaussianBlurCheck(CActor * _Actor)
{
	SPTR<CCamera> MainCamera = _Actor->FindCom<CCamera>();
	if (nullptr != MainCamera)
	{
		MainCamera->FindAllFilter<CGaussianBlur>(&GaussianBlurList);
	}
	return GaussianBlurList;
}


const std::list<SPTR<CNavigationMesh>>& SceneListDlg::NavigationMeshCheck(CActor* _Actor)
{
	_Actor->FindAllCom<CNavigationMesh>(&NaviMeshList);
	return NaviMeshList;
}

const std::list<SPTR<CNaviMeshArea>>& SceneListDlg::NavigationAreaCheck(CActor* _Actor)
{
	_Actor->FindAllCom<CNaviMeshArea>(&NaviAreaList);
	return NaviAreaList;
}

const std::list<SPTR<CBloomFilter>>& SceneListDlg::BloomFilterCheck(CActor * _Actor)
{
	SPTR<CCamera> MainCamera = _Actor->FindCom<CCamera>();
	if (nullptr != MainCamera)
	{
		MainCamera->FindAllFilter<CBloomFilter>(&BloomList);
	}
	return BloomList;
}


BOOL SceneListDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SSDToolGlobal::ToolSceneList = this;

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}
