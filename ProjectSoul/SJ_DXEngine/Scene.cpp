#include "Scene.h"
#include"ResourceMgr.h"
#include"SceneCamera.h"
#include"Device.h"
#include"GlobalDebug.h"
#include"TransForm.h"


SPTR<CScene>											CScene::pCurScene = nullptr;
SPTR<CScene>											CScene::pNextScene = nullptr;

//////////////////////////////////////		Scene Script Load Part		///////////////////////////////////////////
void CSceneScript::SetCameraPos(const Vec3& _Pos, const size_t& _CamIndex /*= 0*/)
{
	if (m_pPairScene->m_vecSceneCamera.size() <= _CamIndex)
		return;

	m_pPairScene->m_vecSceneCamera[_CamIndex]->SetPos(_Pos);
}
CActor * CSceneScript::CreateActor(const ACTOR_UPDATE_LAYER& _Layer/* = UPDATE_LAYER_NORMAL*/, const  std::wstring& _Name /*= L"UnnamedActor"*/)
{
	return CScene::CreateActor(m_pPairScene, _Layer, _Name);
}
CActor * CSceneScript::CreateActor(const std::wstring& _Name, const ACTOR_UPDATE_LAYER& _Layer/* = UPDATE_LAYER_NORMAL*/)
{
	return CScene::CreateActor(m_pPairScene, _Layer, _Name);
}
CActor * CSceneScript::CreateActorNoneTransform(const ACTOR_UPDATE_LAYER& _Layer /*= UPDATE_LAYER_NORMAL*/, const  std::wstring& _Name/* = L"UnnamedActor"*/)
{
	return CScene::CreateActorNoneTransform(m_pPairScene, _Layer, _Name);
}
CActor * CSceneScript::CreateActorNoneTransform(const std::wstring& _Name, const ACTOR_UPDATE_LAYER& _Layer /*= UPDATE_LAYER_NORMAL*/)
{
	return CScene::CreateActorNoneTransform(m_pPairScene, _Layer, _Name);
}

void CSceneScript::LinkCollsionChannel(const SCENECOLLISION& _Type, const std::wstring& _Src, const std::wstring& _Dest)
{
	switch (_Type)
	{
	case COLLISION_2D:
		return m_pPairScene->GetCollision2DMgr()->LinkCollsionChannel(_Src, _Dest);
	case COLLISION_3D:
		return m_pPairScene->GetCollision3DMgr()->LinkCollsionChannel(_Src, _Dest);
	default:
		return;
	}
}
void CSceneScript::DisConnectChannel(const SCENECOLLISION& _Type, const std::wstring& _Src, const std::wstring& _Dest)
{
	switch (_Type)
	{
	case COLLISION_2D:
		return m_pPairScene->GetCollision2DMgr()->DisConnectChannel(_Src, _Dest);
	case COLLISION_3D:
		return m_pPairScene->GetCollision3DMgr()->DisConnectChannel(_Src, _Dest);
	default:
		return;
	}
}

SPTR<CCamera>	CSceneScript::GetSceneCamera(const size_t& _CamIndex)
{
	return m_pPairScene->GetSceneCamera(_CamIndex);
}
CActor*	CSceneScript::GetSceneCameraActor(const size_t& _CamIndex)
{
	return m_pPairScene->GetSceneCameraActor(_CamIndex);
}
///////////////////////////////////////			Collision Channel				////////////////////////////////////////////////////////
CCollisionChannel::CCollisionChannel(const std::wstring& _Src)
	:m_ChannelKey(_Src)
{
	m_listCollider.clear();
	m_listCollisionChannel.clear();
}
CCollisionChannel::~CCollisionChannel()
{
	m_listCollider.clear();
	m_listCollisionChannel.clear();
}


void CCollisionChannel::Collision()
{
	if (true == m_listCollisionChannel.empty())
		return;

	for (CCollisionChannel* ColChannel : m_listCollisionChannel)
	{
		if (ColChannel != this)
			OtherCollision(ColChannel);
		else
			ThisCollision();
	}
}
void CCollisionChannel::DebugRender()
{
	if (true == m_listCollider.empty())
		return;

	for (SPTR<CCollider> Col : m_listCollider) 
	{
		if(true == Col->IsActive())
			Col->DebugRender();
	}
}
void CCollisionChannel::LastUpdate()
{
	if (true == m_listCollider.empty())
		return;

	for (SPTR<CCollider> Col : m_listCollider)
	{
		if(nullptr != Col)
			Col->DumpDeathCollider();
	}
}
void CCollisionChannel::Release()
{
	if (true == m_listCollider.empty())
		return;

	
	m_listCollider.erase(std::remove_if(m_listCollider.begin(), m_listCollider.end(), [](SPTR<CCollider> Collider)->bool 
	{
		return Collider->IsDeath();
	}), m_listCollider.end());
}
void CCollisionChannel::OtherCollision(CCollisionChannel* _OtherChannel)
{
	if (nullptr == _OtherChannel)
		return;

	for (SPTR<CCollider> MyCollider : m_listCollider)
	{
		if (false == MyCollider->IsActive()) 
		{
			MyCollider->m_bCollision = false;
			continue;
		}

		for (SPTR<CCollider> OtherCollider : _OtherChannel->m_listCollider) 
		{
			if (false == OtherCollider->IsActive()) 
			{
				OtherCollider->m_bCollision = false;
				continue;
			}

			MyCollider->Collision(OtherCollider);

			if (false == MyCollider->IsActive()) 
			{
				MyCollider->m_bCollision = false;
				OtherCollider->m_bCollision = false;
				MyCollider->ReleaseSet();	
			}

			if (false == OtherCollider->IsActive()) 
			{
				MyCollider->m_bCollision = false;
				OtherCollider->m_bCollision = false;
				MyCollider->EraseSet(OtherCollider);
			}
		}
		
	}
}
void CCollisionChannel::ThisCollision()
{
	std::list<SPTR<CCollider>>::iterator StartIter = m_listCollider.begin();
	std::list<SPTR<CCollider>>::iterator EndIter = m_listCollider.end();

	for (; StartIter != EndIter; ++StartIter )
	{
		if (false == (*StartIter)->IsActive())
			continue;

		std::list<SPTR<CCollider>>::iterator ColIter = StartIter;
		++ColIter;

		for (; ColIter != EndIter; ++ColIter) 
		{
			if (false == (*ColIter)->IsActive())
				continue;

			(*StartIter)->Collision(*ColIter);

			if (false == (*StartIter)->IsActive())
			{
				(*StartIter)->m_bCollision = false;
				(*ColIter)->m_bCollision = false;
				(*StartIter)->ReleaseSet();
			}

			if (false == (*ColIter)->IsActive())
			{
				(*StartIter)->m_bCollision = false;
				(*ColIter)->m_bCollision = false;
				(*StartIter)->EraseSet((*ColIter));
			}
		}
	}
}


///////////////////////////////////////			SceneCollisionMgr				/////////////////////////////////////////////////////
const bool CSceneCollisionMgr::PushCollider(CCollider* _Col)
{
	if (nullptr == _Col)
		return false;

	std::wstring Channel = _Col->GetChannel();

	auto Find = m_mapColliderChannel.find(Channel);

	// 채널이 없다면 채널부터 생성한 뒤 충돌체를 넣는다
	if (Find == m_mapColliderChannel.end())
	{
		CCollisionChannel* NewChannel = new CCollisionChannel(Channel);

		auto InsertIter = m_mapColliderChannel.insert(std::unordered_map<std::wstring, SPTR<CCollisionChannel>>::value_type(Channel, NewChannel));

		if (true == InsertIter.second)
			InsertIter.first->second->m_listCollider.push_back(_Col);
		else
			return false;
	}
	else
		Find->second->m_listCollider.push_back(_Col);

	return true;
}


void CSceneCollisionMgr::LinkCollsionChannel(const std::wstring& _Src, const std::wstring& _Dest)
{
	SPTR<CCollisionChannel> Src = UMAPFINDSPTR<SPTR<CCollisionChannel>>(m_mapColliderChannel, _Src);
	SPTR<CCollisionChannel> Dest = UMAPFINDSPTR<SPTR<CCollisionChannel>>(m_mapColliderChannel, _Dest);
	// Src Channel이 없는경우 새로 생성
	if (Src == nullptr)
	{
		CCollisionChannel* NewChannel = new CCollisionChannel(_Src);
		m_mapColliderChannel.insert(std::unordered_map<std::wstring, SPTR<CCollisionChannel>>::value_type(_Src, NewChannel));
		Src = NewChannel;
	}

	if (Dest == nullptr)
	{
		CCollisionChannel* NewChannel = new CCollisionChannel(_Dest);
		m_mapColliderChannel.insert(std::unordered_map<std::wstring, SPTR<CCollisionChannel>>::value_type(_Dest, NewChannel));
		Dest = NewChannel;
	}

	for (CCollisionChannel* Channel : Src->m_listCollisionChannel)
	{
		// 만일 추가하려는 리스트에 _Dest가 들어있는경우 연결을 하지 않는다
		if (Channel == Dest)
			return;
	}

	for (CCollisionChannel* Channel : Dest->m_listCollisionChannel)
	{
		// 만일 상대에 채널에 내가 있는경우 연결을 하지 않는다.
		if (Channel == Src)
			return;
	}

	Src->m_listCollisionChannel.push_back(Dest);
}

void CSceneCollisionMgr::DisConnectChannel(const std::wstring& _Src, const std::wstring& _Dest)
{
	SPTR<CCollisionChannel> Src = UMAPFINDSPTR<SPTR<CCollisionChannel>>(m_mapColliderChannel, _Src);


	if (nullptr == Src)
		return;

	std::list<CCollisionChannel*>::iterator Start = Src->m_listCollisionChannel.begin();
	std::list<CCollisionChannel*>::iterator End = Src->m_listCollisionChannel.end();
	
	if (true == Src->m_listCollisionChannel.empty())
		return;


	for (;Start!= End; ++Start)
	{
		if ((*Start)->m_ChannelKey == _Dest) 
		{
			Src->m_listCollisionChannel.erase(Start);
			return;
		}
	}
}
void CSceneCollisionMgr::Collision()
{
	if (true == m_mapColliderChannel.empty())
		return;

	SPTR<CCollisionChannel> CurChannel;

	for (auto& ChannelIter : m_mapColliderChannel)
	{
		CurChannel = ChannelIter.second;
		CurChannel->Collision();
	}
}
void CSceneCollisionMgr::DebugRender()
{
	if (true == m_mapColliderChannel.empty())
		return;

	for (auto& ChannelIter : m_mapColliderChannel)
	{
		SPTR<CCollisionChannel> Channel = ChannelIter.second;
		if (nullptr != Channel)
			Channel->DebugRender();
	}
}
void CSceneCollisionMgr::LastUpdate()
{
	if (true == m_mapColliderChannel.empty())
		return;

	for (std::map<std::wstring,SPTR<CCollisionChannel>>::value_type& ChannelIter : m_mapColliderChannel)
	{
		SPTR<CCollisionChannel> Channel = ChannelIter.second;
		if (nullptr != Channel)
			Channel->LastUpdate();
	}
}
void CSceneCollisionMgr::Release()
{
	if (true == m_mapColliderChannel.empty())
		return;

	for (auto& ChannelIter : m_mapColliderChannel) 
	{
		SPTR<CCollisionChannel> Channel = ChannelIter.second;
		
		if(nullptr != Channel)
			Channel->Release();
	}
	
}

////////////////////////////////////		Scene Render Mgr			//////////////////////////////////////////////////////
const bool CSceneRenderMgr::PushCamera(CCamera* _Camera)
{
	if (nullptr == _Camera)
		return false;


	CAMERA_GROUP Group = _Camera->m_CameraGroup;

	std::multimap<CAMERA_GROUP, SPTR<CCamera>>::iterator FindStartIter = m_mapCamera.find(Group);
	std::multimap<CAMERA_GROUP, SPTR<CCamera>>::iterator FindEndIter;

	// 그룹이 중복되는 카메라가 없는경우
	if (FindStartIter == m_mapCamera.end())
	{
		m_mapCamera.insert(std::multimap<CAMERA_GROUP, SPTR<CCamera>>::value_type(Group, _Camera));
		return true;
	}

	FindStartIter = m_mapCamera.lower_bound(Group);
	FindEndIter = m_mapCamera.upper_bound(Group);

	// 넣은 카메라를 또 넣으려 할때 튕겨낸다.
	for (; FindStartIter != FindEndIter; ++FindStartIter)
	{
		if (_Camera == FindStartIter->second)
			return false;
	}

	m_mapCamera.insert(std::multimap<CAMERA_GROUP, SPTR<CCamera>>::value_type(Group, _Camera));
	return true;
}
const bool CSceneRenderMgr::PushRenderer(CRenderer* _Ren)
{
	if (nullptr == _Ren)
		return false;

	auto FindIter = m_mapRendererList.find(_Ren->GetRenderLayer());
	
	// 해당 랜더 레이어에 소속된 렌더러가 없는경우
	if (m_mapRendererList.end() == FindIter)
	{
		auto Check = m_mapRendererList.insert(std::map< RENDER_LAYER, std::list<SPTR<CRenderer>>>::value_type(_Ren->GetRenderLayer(), std::list<SPTR<CRenderer>>()));

		if (true == Check.second)
			Check.first->second.push_back(_Ren);
		else
			return false;
	}
	else 
		FindIter->second.push_back(_Ren);

	return true;
	
}



void CSceneRenderMgr::Render() 
{
	m_CamStartIter = m_mapCamera.begin();
	m_CamEndIter = m_mapCamera.end();

	// 랜더 매니져의 등록된 카메라 만큼 랜더링 수행
	for (; m_CamStartIter!= m_CamEndIter; ++m_CamStartIter)
		RenderCameraLoop(m_CamStartIter);

}
void CSceneRenderMgr::Release()
{
	CameraRelease();
	RendererRelease();
}
void CSceneRenderMgr::CameraRelease() 
{
	m_CamStartIter = m_mapCamera.begin();
	m_CamEndIter = m_mapCamera.end();
	for ( ;m_CamStartIter != m_CamEndIter; )
	{
		if (true == m_CamStartIter->second->IsDeath())
			m_CamStartIter = m_mapCamera.erase(m_CamStartIter);
		else
			++m_CamStartIter;
	}
}
void CSceneRenderMgr::RendererRelease() 
{
	std::list<SPTR<CRenderer>>::iterator RenStart;
	std::list<SPTR<CRenderer>>::iterator RenEnd;
	for (auto& RenListIter : m_mapRendererList)
	{
		if (true == RenListIter.second.empty())
			continue;

		RenStart = RenListIter.second.begin();
		RenEnd = RenListIter.second.end();

		for (; RenStart != RenEnd;)
		{
			if (true == (*RenStart)->IsDeath())
				RenStart = RenListIter.second.erase(RenStart);
			else
				++RenStart;
		}
		
	}
}
void CSceneRenderMgr::RenderCameraLoop(std::multimap<CAMERA_GROUP, SPTR<CCamera>>::iterator& _CamIter)
{
	RENDER_LAYER Layer;

	if (false == _CamIter->second->IsActive())
		return;

	for (size_t i = 0; i < _CamIter->second->m_vecViewRenderLayer.size(); i++)
	{
		Layer = _CamIter->second->m_vecViewRenderLayer[i];
		m_FindRenListIter = m_mapRendererList.find(Layer);

		if (m_FindRenListIter == m_mapRendererList.end() 
			|| true == m_FindRenListIter->second.empty())
			continue;

		// 랜더 리스트안에 있는 랜더러의 랜더를 
		// 현재 카메라의 VP행렬을 넘겨주면서 돌려준다.
		for (SPTR<CRenderer> Ren : m_FindRenListIter->second) 
		{
			if(Ren && Ren->IsActive())
				Ren->Render(_CamIter->second->GetVPMatrix());
		}
	}
}



////////////////////////////////////		Static Scene Function		/////////////////////////////////////

SPTR<CScene> CScene::FindScene(const std::wstring& _SceneName)
{
	return CResourceMgr<CScene>::Find(_SceneName);
}
const bool CScene::ChangeScene(const std::wstring& _SceneName) 
{
	SPTR<CScene> ChangeScene = CResourceMgr<CScene>::Find(_SceneName);

	if (nullptr == ChangeScene)
	{
		TASSERT(true);
		return false;
	}
	pNextScene = ChangeScene;
	return true;
}
void CScene::SceneProgress(const float& _DeltaTime)
{
	if (nullptr != pNextScene)
	{
		if (nullptr != pCurScene)
			pCurScene->ShutDown();

		pNextScene->WakeUp();
		CScene::pCurScene = CScene::pNextScene;
		CScene::pNextScene = nullptr;
	}

	if (nullptr != pCurScene ) 
	{
		pCurScene->Update(_DeltaTime);
		pCurScene->Collision();
		CDevice::Clear();
		pCurScene->Render();
		CDevice::Present();
		pCurScene->LastUpdate(_DeltaTime);
		pCurScene->Release();

	}
}

CActor*  CScene::CreateActorNoneTransform(CScene* _pScene, const ACTOR_UPDATE_LAYER& _Layer, const std::wstring& _Name)
{
	if (nullptr == _pScene)
		return nullptr;

	CActor* NewActor = Engine::CreateGameObject<CActor>(_pScene);

	NewActor->SetName(_Name);
	NewActor->SetParentScene(_pScene);

	if (false == _pScene->PushActor(NewActor, _Layer))
	{
		TASSERT(true);
		return nullptr;
	}

	NewActor->Init();
	return NewActor;
}

CActor* CScene::CreateActor(CScene* _pScene, const ACTOR_UPDATE_LAYER& _Layer, const std::wstring& _Name)
{
	if (nullptr == _pScene)
		return nullptr;

	CActor* NewActor = Engine::CreateGameObject<CActor>(_pScene);

	NewActor->SetName(_Name);
	NewActor->SetParentScene(_pScene);
	
	if (false == _pScene->PushActor(NewActor, _Layer))
	{
		TASSERT(true);
		return nullptr;
	}

	NewActor->Init();
	NewActor->CreateCom<CTransForm>();
	return NewActor;
}


////////////////////////////////////////		Member Scene Func		///////////////////////////////////
CScene::CScene()
	: m_bLoadingComplete(false), m_PairCollision2DMgr(CSceneCollisionMgr(this))
	, m_PairRenderMgr(CSceneRenderMgr(this)), m_PairCollision3DMgr(CSceneCollisionMgr(this))
{
	
}
CScene::~CScene()
{
	m_listSceneScript.clear();
	m_mapActorList.clear();
}
void CScene::Init()
{
	// 씬이 처음 생성될 때 기본 카메라를 만들어 낸다. 
	//ASceneCamera* MainCamera = Engine::CreateGameObject<ASceneCamera>(this);
	//m_vecSceneCamera.push_back(MainCamera->GetCamera());
	//PushActor(MainCamera, UPDATE_LAYER_LAST);
	CActor* MainCamActor = CreateActor(this, UPDATE_LAYER_LAST,L"MainCameraActor");
	CCamera* pCamera = MainCamActor->CreateCom<CCamera>(CAMDATA{ (CAMERA_GROUP)0 , DirectX::XM_PIDIV2 ,0.3f, 1000.f });
	pCamera->SetWheelZoom(true);

	pCamera->SettingViewLayer(RENDER_LAYER_MAP, RENDER_LAYER_OBJECT, RENDER_LAYER_BACKEFFECT, RENDER_LAYER_PLAYER, RENDER_LAYER_FRONTEFFECT,RENDER_LAYER_FIRST);
	m_vecSceneCamera.push_back(pCamera);
}
const bool CScene::Loading()
{
	if (true == m_bLoadingComplete )
		return false;

	for ( SPTR<CSceneScript> Script : m_listSceneScript)
	{
		if (nullptr != Script && false == Script->m_bLoading)
		{
			Script->Loading();
			Script->m_bLoading = true;
		}
	}

	m_bLoadingComplete = true;
	return true;
}
void CScene::Update(const float& _DeltaTime)
{
	for (SPTR<CSceneScript> Script : m_listSceneScript)
	{
		if (nullptr != Script)
			Script->PreUpdate(_DeltaTime);
	}

	// Actor 업데이트
	for (auto& ActorList : m_mapActorList) 
	{
		for(SPTR<CActor> Actor : ActorList.second)
		{
			if(Actor->IsActive())
				Actor->PreUpdate(_DeltaTime);
		}
	}
	

	for (auto& ActorList : m_mapActorList)
	{
		for (SPTR<CActor> Actor : ActorList.second)
		{
			if (Actor->IsActive())
				Actor->Update(_DeltaTime);
		}
	}


	for (auto& ActorList : m_mapActorList)
	{
		for (SPTR<CActor> Actor : ActorList.second)
		{
			if (Actor->IsActive())
				Actor->PostUpdate(_DeltaTime);
		}
	}


	for (SPTR<CSceneScript> Script : m_listSceneScript)
	{
		if (nullptr != Script)
			Script->PostUpdate(_DeltaTime);
	}
}

void CScene::Collision()
{
	m_PairCollision2DMgr.Collision();
	m_PairCollision3DMgr.Collision();
}
// 스크립트 전용 디버그 랜더 호출
void CScene::DebugRender()
{
	for (SPTR<CScript> Script : m_listActorScript)
	{
		if(true == Script->IsActive())
			Script->DebugRender();
	}
}
void CScene::Render()
{
	m_PairRenderMgr.Render();

	if (GlobalDebug::bDebugMode) 
	{
		DebugRender();
		m_PairCollision2DMgr.DebugRender();
		m_PairCollision3DMgr.DebugRender();

		for (SPTR<CSceneScript> SceneScript : m_listSceneScript)
			SceneScript->DebugRender();
	}
	
}
void CScene::LastUpdate(const float& _DeltaTime)
{
	for (auto& ActorList : m_mapActorList) 
	{
		for (SPTR<CActor> Actor : ActorList.second)
		{
			if(nullptr != Actor)
				Actor->LastUpdate(_DeltaTime);
		}
	}


	for (SPTR<CSceneScript> Script : m_listSceneScript)
	{
		if (nullptr != Script)
			Script->LastUpdate(_DeltaTime);
	}

	m_PairCollision2DMgr.LastUpdate();
	m_PairCollision3DMgr.LastUpdate();
}
void CScene::Release() 
{
	for (auto& ActorList : m_mapActorList) 
	{
		std::list<SPTR<CActor>>::iterator StartActor = ActorList.second.begin();
		std::list<SPTR<CActor>>::iterator EndActor = ActorList.second.end();
			
		for (; StartActor != EndActor; )
		{
			if (true == (*StartActor)->IsDeath()
				|| nullptr !=  (*StartActor)->m_pParentActor ) // 액터의 부모가 있는 경우 씬 리스트에서는 제외한다.
			{
				StartActor = ActorList.second.erase(StartActor);
			}
			else
			{
				// 자기 안의 컴포넌트와 차일드 엑터를 릴리즈하는 작업을 수행
				(*StartActor)->PushWaitChild();
				(*StartActor)->Release();
				++StartActor;
			}
		}
	}


	// 랜더 & 컬라이더 매니져에서도 릴리즈를 실행해야한다.
	m_PairRenderMgr.Release();
	m_PairCollision2DMgr.Release();
	m_PairCollision3DMgr.Release();
}



void CScene::WakeUp()
{

	// 런타임중 씬스크립트가 추가된 경우
	// 스크립트 리스트에서 로딩이 되지 않은 스크립트를 로딩시킨다.
	if(false == m_bLoadingComplete)
		Loading();
	
	for (SPTR<CSceneScript> Script : m_listSceneScript)
	{
		if(nullptr != Script)
			Script->WakeUp();
	}

}
void CScene::ShutDown()
{
	for (SPTR<CSceneScript> Script : m_listSceneScript)
	{
		if (nullptr != Script)
			Script->ShutDown();
	}
}
const bool CScene::PushActor(CActor* _Actor, const ACTOR_UPDATE_LAYER& _Layer)
{
	if (nullptr == _Actor)
		return false;

	_Actor->SetUpdateLayer(_Layer);

	std::map<int, std::list<SPTR<CActor>>>::iterator MapIter = m_mapActorList.find(_Layer);

	// 새로운 레이어 번호가 들어온 경우
	if (m_mapActorList.end() == MapIter)
	{
		std::list<SPTR<CActor>> NewList;
		auto InsertIter =  m_mapActorList.insert(std::map<int, std::list<SPTR<CActor>>>::value_type(_Layer, NewList));
	
		if(true == InsertIter.second)
			InsertIter.first->second.push_back(_Actor);
	}
	// 기존 레이어 번호가 들어온 경우
	else
		MapIter->second.push_back(_Actor);

	return true;
}

const bool CScene::PushCollider(const SCENECOLLISION& _Type,CCollider* _Collider)
{
	if (nullptr == _Collider)
		return false;

	switch (_Type)
	{
	case COLLISION_2D:
		return m_PairCollision2DMgr.PushCollider(_Collider);
	case COLLISION_3D:
		return m_PairCollision3DMgr.PushCollider(_Collider);
	default:
		break;
	}
	return false;
}
const bool CScene::PushScript(CScript* _Script)
{
	if (nullptr == _Script)
		return false;

	m_listActorScript.push_back(_Script);
	return true;
}
const bool CScene::PushRenderer(CRenderer* _Renderer) 
{
	if (nullptr == _Renderer)
		return false;

	return m_PairRenderMgr.PushRenderer(_Renderer);
}
const bool CScene::PushCamera(CCamera* _Camera) 
{
	if (nullptr == _Camera)
		return false;

	return m_PairRenderMgr.PushCamera(_Camera);
}
const bool CScene::Create(const CREATE_DESC& _Desc)
{
	SetName(_Desc.Name);

	return true;
}

