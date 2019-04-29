#include "SceneCamera.h"
#include"Scene.h"



ASceneCamera::ASceneCamera()
	:m_pCamera(nullptr)
{
	//CreateCom<CTransForm>();
	//m_pCamera = CreateCom<CCamera>(CAMDATA{ (CAMERA_GROUP)0 , DirectX::XM_PIDIV2 ,0.3f, 1000.f  });
	//m_pCamera->SettingViewLayer(RENDER_LAYER_MAP, RENDER_LAYER_BACKEFFECT, RENDER_LAYER_OBJECT, RENDER_LAYER_PLAYER, RENDER_LAYER_FRONTEFFECT,RENDER_LAYER_FIRST);
}
ASceneCamera::ASceneCamera(CScene* _Scene)
	:CActor(_Scene), m_pCamera(nullptr)
{
	//CreateCom<CTransForm>();
	//m_pCamera = CreateCom<CCamera>(CAMDATA{ (CAMERA_GROUP)0 , DirectX::XM_PIDIV2  , 0.3f, 1000.f });
	//m_pCamera->SettingViewLayer(RENDER_LAYER_MAP, RENDER_LAYER_BACKEFFECT, RENDER_LAYER_OBJECT, RENDER_LAYER_PLAYER ,RENDER_LAYER_FRONTEFFECT, RENDER_LAYER_FIRST);
}

ASceneCamera::~ASceneCamera()
{
	m_pCamera = nullptr;
}

void ASceneCamera::Init()
{

}

