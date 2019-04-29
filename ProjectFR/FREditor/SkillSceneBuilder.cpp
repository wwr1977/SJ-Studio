#include "stdafx.h"
#include "SkillSceneBuilder.h"
#include"MiddlePoint.h"
#include"AnimationDlg.h"
#include"AniSpriteDlg.h"
#include"EditCameraMen.h"
#include"TextureModel.h"
#include"MultiSpriteModel.h"
#include"BaseAniModel.h"
#include"EditBackGround.h"
#include"Gauge.h"
#include"EditEffectMgr.h"
#include"EffectDlg.h"
#include"SkillMgr.h"

CSkillSceneBuilder::CSkillSceneBuilder()
	:m_TexModel(nullptr), m_AniSpriteModel(nullptr)
	,m_BaseAniModel(nullptr), m_AnimationModel(nullptr)
	, m_AnimationCom(nullptr), m_MainCameraMen(nullptr)
	, m_EffectAniModel(nullptr), m_EffectAniCom(nullptr), m_EffectSoundPlayer(nullptr)
	,m_fTexScale(1.0f),m_fSpriteScale(1.0f)
	,m_fAniScale(2.0f), m_SkillMgr(nullptr)
{
}


CSkillSceneBuilder::~CSkillSceneBuilder()
{
}
bool CSkillSceneBuilder::SceneBuild() 
{
	RESMGR->LoadGameTexture(CFilePathMgr::GetPath(_T("UI")) + _T("ColorBase.png"));
	RESMGR->CreateGameSprite(_T("ColorBase"));

	KeySetting();
	CreateMainLight();
	CreateMainCamera();
	SettingMulSpriteModel();
	
	RESMGR->LoadGameTexture(CFilePathMgr::GetPath(_T("UI")) + _T("StatGaugeFrame.png"));
	RESMGR->CreateGameSprite(_T("StatGaugeFrame"));

	SPTR<CActObject> MiddlePoint = CreateActObject(_T("MiddlePoint"));
	MiddlePoint->CreateCom<CMiddlePoint>();
	MiddlePoint->SetPos({ 0.0f,0.0f,0.0f });
	MiddlePoint->SetSize({ 200.0f,200.0f,1.0f });

	CEditGlobal::EditMainCameraMen =  GetScene()->GetMainCamera();
	CEditGlobal::EditMainCamera = GetScene()->GetMainCamCom();


	RESMGR->LoadGameTexture(CFilePathMgr::GetPath(_T("Texture")) + _T("EditBack.png"));
	RESMGR->LoadGameTexture(CFilePathMgr::GetPath(_T("UI")) + _T("Number_Black.png"));
	RESMGR->LoadGameTexture(CFilePathMgr::GetPath(_T("UI")) + _T("Number_Red.png"));
	RESMGR->LoadGameTexture(CFilePathMgr::GetPath(_T("UI")) + _T("Number_Blue.png"));
	
	RESMGR->CreateGameSprite(_T("EditBack"));
	CreateActObject(_T("EditBack"))->CreateCom<CEditBackGround>();

	CEditEffectMgr* SkillEffectMgr = new  CEditEffectMgr();
	SkillEffectMgr->SettingScene(GetScene());
	CEditGlobal::EffectMgr = SkillEffectMgr;

	//RESMGR->LoadTextureInFolder(_T("PreLoad"));

	m_SkillMgr = new CSkillMgr(m_pScene, this);
	CEditGlobal::SkillScene = m_pScene;

	if (nullptr != m_SkillMgr)
		CEditGlobal::EditSkillMgr = m_SkillMgr;

	//// 검은색 숫자 Sprite를 만듦(한 Texture에 크기가 서로다른 Sprite라서,나중에 MultiSprite 합성)
	vector<SPTR<CGameSprite>> BlackNumber;
	BlackNumber.reserve(10);

	BlackNumber.push_back(RESMGR->CreateGameSprite(_T("Number_Black"), _T("Number_Black0"), { 0.0f,0.0f }, { 50.75f,64.0f }));
	BlackNumber.push_back(RESMGR->CreateGameSprite(_T("Number_Black"), _T("Number_Black1"), { 50.75f,0.0f }, { 34.22f,64.0f }));
	BlackNumber.push_back(RESMGR->CreateGameSprite(_T("Number_Black"), _T("Number_Black2"), { 84.97f,0.0f }, { 55.1f,64.0f }));
	BlackNumber.push_back(RESMGR->CreateGameSprite(_T("Number_Black"), _T("Number_Black3"), { 140.07f,0.0f }, { 51.62f,64.0f }));
	BlackNumber.push_back(RESMGR->CreateGameSprite(_T("Number_Black"), _T("Number_Black4"), { 191.69f,0.0f }, { 53.36f,64.0f }));
	BlackNumber.push_back(RESMGR->CreateGameSprite(_T("Number_Black"), _T("Number_Black5"), { 245.05f,0.0f }, { 53.36f,64.0f }));
	BlackNumber.push_back(RESMGR->CreateGameSprite(_T("Number_Black"), _T("Number_Black6"), { 298.41f,0.0f }, { 51.62f,64.0f }));
	BlackNumber.push_back(RESMGR->CreateGameSprite(_T("Number_Black"), _T("Number_Black7"), { 350.03f,0.0f }, { 53.36f,64.0f }));
	BlackNumber.push_back(RESMGR->CreateGameSprite(_T("Number_Black"), _T("Number_Black8"), { 403.39f,0.0f }, { 52.2f,64.0f }));
	BlackNumber.push_back(RESMGR->CreateGameSprite(_T("Number_Black"), _T("Number_Black9"), { 455.59f,0.0f }, { 51.62f,64.0f }));

	vector<SPTR<CGameSprite>> RedNumber;
	RedNumber.push_back(RESMGR->CreateGameSprite(_T("Number_Red"), _T("Number_Red0"), { 0.0f,0.0f }, { 94.0f,113.0f }));
	RedNumber.push_back(RESMGR->CreateGameSprite(_T("Number_Red"), _T("Number_Red1"), { 98.0f,0.0f }, { 77.0f,113.0f }));
	RedNumber.push_back(RESMGR->CreateGameSprite(_T("Number_Red"), _T("Number_Red2"), { 175.0f,0.0f }, { 92.0f,113.0f }));
	RedNumber.push_back(RESMGR->CreateGameSprite(_T("Number_Red"), _T("Number_Red3"), { 268.0f,0.0f }, { 94.0f,113.0f }));
	RedNumber.push_back(RESMGR->CreateGameSprite(_T("Number_Red"), _T("Number_Red4"), { 363.f,0.0f }, { 89.0f,113.0f }));
	RedNumber.push_back(RESMGR->CreateGameSprite(_T("Number_Red"), _T("Number_Red5"), { 452.0f,0.0f }, { 95.0f,113.0f }));
	RedNumber.push_back(RESMGR->CreateGameSprite(_T("Number_Red"), _T("Number_Red6"), { 547.0f,0.0f }, { 90.0f,113.0f }));
	RedNumber.push_back(RESMGR->CreateGameSprite(_T("Number_Red"), _T("Number_Red7"), { 637.0f,0.0f }, { 96.0f,113.0f }));
	RedNumber.push_back(RESMGR->CreateGameSprite(_T("Number_Red"), _T("Number_Red8"), { 734.0f,0.0f }, { 95.0f,113.0f }));
	RedNumber.push_back(RESMGR->CreateGameSprite(_T("Number_Red"), _T("Number_Red9"), { 829.0f,0.0f }, { 92.0f,113.0f }));

	vector<SPTR<CGameSprite>> BlueNumber;
	BlueNumber.push_back(RESMGR->CreateGameSprite(_T("Number_Blue"), _T("Number_Blue0"), { 0.0f,0.0f }, { 93.0f,111.0f }));
	BlueNumber.push_back(RESMGR->CreateGameSprite(_T("Number_Blue"), _T("Number_Blue1"), { 94.0f,0.0f }, { 79.0f,111.0f }));
	BlueNumber.push_back(RESMGR->CreateGameSprite(_T("Number_Blue"), _T("Number_Blue2"), { 172.0f,0.0f }, { 91.0f,111.0f }));
	BlueNumber.push_back(RESMGR->CreateGameSprite(_T("Number_Blue"), _T("Number_Blue3"), { 263.0f,0.0f }, { 94.0f,111.0f }));
	BlueNumber.push_back(RESMGR->CreateGameSprite(_T("Number_Blue"), _T("Number_Blue4"), { 358.0f,0.0f }, { 86.0f,111.0f }));
	BlueNumber.push_back(RESMGR->CreateGameSprite(_T("Number_Blue"), _T("Number_Blue5"), { 444.0f,0.0f }, { 96.0f,111.0f }));
	BlueNumber.push_back(RESMGR->CreateGameSprite(_T("Number_Blue"), _T("Number_Blue6"), { 540.0f,0.0f }, { 90.0f,111.0f }));
	BlueNumber.push_back(RESMGR->CreateGameSprite(_T("Number_Blue"), _T("Number_Blue7"), { 630.0f,0.0f }, { 96.0f,111.0f }));
	BlueNumber.push_back(RESMGR->CreateGameSprite(_T("Number_Blue"), _T("Number_Blue8"), { 727.0f,0.0f }, { 90.0f,111.0f }));
	BlueNumber.push_back(RESMGR->CreateGameSprite(_T("Number_Blue"), _T("Number_Blue9"), { 818.0f,0.0f }, { 90.0f,111.0f }));


	RESMGR->CreateMultiSprite(_T("Number_Black"), &BlackNumber);
	RESMGR->CreateMultiSprite(_T("Number_Red"), &RedNumber);
	RESMGR->CreateMultiSprite(_T("Number_Blue"), &BlueNumber);

	BlackNumber.clear();
	RedNumber.clear();
	BlueNumber.clear();


	return TRUE;
}
void CSkillSceneBuilder::SceneUpdate()
{
	CheckCamMove();

	//AnimtionDlg이 포커싱 되었을때만 키 작동
	if (TRUE == CEditGlobal::AniSpriteDlg->IsDlgOn())
	{
		if (true == KEYDOWN("ChangeAniMode"))
		{
			CEditGlobal::AniSpriteDlg->ChangeModeEvent();
		}
	}

	if (TRUE == CEditGlobal::AnimationDlg->IsDlgOn())
	{
		if (true == KEYDOWN("ChangeAniMode"))
		{
			CEditGlobal::AnimationDlg->ChangeAniMode();
		}
	}


	if (TRUE == KEYDOWN("CameraPosReset"))
	{
		CEditGlobal::EditMainCameraMen->SetPos({ 0.0f,0.0f,0.0f });
		//CEditGlobal::EditMainCamera->SetZoomRatio(1.0f);
	}

	CSceneBuilder::SceneUpdate();
	if (nullptr != m_SkillMgr && true == m_SkillMgr->IsActive())
		m_SkillMgr->Update();

	CEditGlobal::Update();
}
void CSkillSceneBuilder::CheckCamMove()
{
	Vec3 Dir = GAMEVEC::ZERO;

	if (true == KEY("CameraMoveUp"))
	{
		Dir += GAMEVEC::UP;
	}
	if (true == KEY("CameraMoveDown"))
	{
		Dir += GAMEVEC::DOWN;
	}
	if (true == KEY("CameraMoveLeft"))
	{
		Dir += GAMEVEC::LEFT;
	}
	if (true == KEY("CameraMoveRight"))
	{
		Dir += GAMEVEC::RIGHT;
	}

	D3DXVec3Normalize(&Dir, &Dir);

	if (GAMEVEC::ZERO != Dir)
	{
		GetScene()->GetMainCamera()->Move(Dir*DELTATIME*100.0f);
	}

}

void CSkillSceneBuilder::SettingMulSpriteModel()
{
	
	m_TexModel = CreateActObject(_T("TextureModel"));
	m_TexModel->SetImageScale(m_fTexScale);
	m_TexModel->CreateCom<CTextureModel>();
	m_TexModel->Off();

	m_AniSpriteModel = CreateActObject(_T("AniSpriteModel"));
	m_AniSpriteModel->SetImageScale(m_fSpriteScale);
	m_AniSpriteModel->CreateCom<CMultiSpriteModel>();
	m_AniSpriteModel->Off();

	m_BaseAniModel = CreateActObject(_T("BaseAnimationModel"));
	m_BaseAniModel->SetImageScale(m_fAniScale);
	m_BaseAniModel->CreateCom<CBaseAniModel>();
	m_BaseAniModel->Off();

	m_AnimationModel = CreateActObject(_T("AnimationModel"));
	m_AnimationCom = m_AnimationModel->CreateCom<CAniRenderer>(RENDATA(5, 0.8f));
	m_AnimationModel->SetImageScale(2.0f);
	m_AnimationModel->Off();

	m_EffectAniModel = CreateActObject(_T("EffectAniModel"));
	m_EffectAniCom = m_EffectAniModel->CreateCom<CAniRenderer>(RENDATA(5,1.0f));
	m_EffectSoundPlayer = m_EffectAniModel->CreateCom<CSoundPlayer>();
	
	/*
	m_AnimationModel->SetImageScale(1.0f);
	m_AnimationModel->Off();
	*/

	CEditGlobal::vecSpriteDlgModel.push_back(m_TexModel);
	CEditGlobal::vecSpriteDlgModel.push_back(m_AniSpriteModel);
	CEditGlobal::vecSpriteDlgModel.push_back(m_BaseAniModel);

	CEditGlobal::MTexModel = m_TexModel;
	
	CEditGlobal::MAniSpriteModel = m_AniSpriteModel;
	
	CEditGlobal::MBaseAniModel = m_BaseAniModel;
	
	CEditGlobal::AAnimationModel = m_AnimationModel;
	CEditGlobal::AAnimationCom = m_AnimationCom;

	CEditGlobal::EEffectAniModel = m_EffectAniCom;
	CEditGlobal::ESoundPlayer = m_EffectSoundPlayer;

	CEditGlobal::bEditInit = TRUE;

}
void CSkillSceneBuilder::SyncSpriteSize(SPTR<CActObject> _pObject, SPTR<CFixRenderer> _Renderer)
{
	// 이미지가 가로가 더 길경우 가로길이를 기준으로 스프라이트 비율 조정
	POINT WndSize = GetScene()->GetWindow()->WndSize();
	
	float SpriteAspect = _Renderer->GetCurSpriteAspect();
	float SpriteLongestSide = _Renderer->GetCurLongestSide();

	// 이미지가 가로로 긴경우(이미지의 크기를 가로 길이 기준으로 조정)
	if (1 >= SpriteAspect)
	{
		if (SpriteLongestSide >= WndSize.x) 
		{
			_pObject->SetImageScale(WndSize.x / SpriteLongestSide);
		}
	}
	else 
	{
		if (SpriteLongestSide >= WndSize.y)
		{
			_pObject->SetImageScale(WndSize.y / SpriteLongestSide);
		}
	}
	
	_pObject->SetImageScale(1.0f);
}
void CSkillSceneBuilder::KeySetting()
{
	CKeyMgr::CreateKey(_T("ChangeAniMode"),'Q');

	CKeyMgr::CreateKey(_T("CameraMoveUp"), 'W');
	CKeyMgr::CreateKey(_T("CameraMoveLeft"), 'A');
	CKeyMgr::CreateKey(_T("CameraMoveDown"), 'S');
	CKeyMgr::CreateKey(_T("CameraMoveRight"), 'D');
	
	/*
	CKeyMgr::CreateKey(_T("CameraMoveUpLeft"), 'W' , 'A');
	CKeyMgr::CreateKey(_T("CameraMoveUpRight"), 'W' , 'D');
	CKeyMgr::CreateKey(_T("CameraMoveDownLeft"), 'S' , 'A');
	CKeyMgr::CreateKey(_T("CameraMoveDownRight"), 'S' , 'D');
	*/

	CKeyMgr::CreateKey(_T("CameraPosReset"), 'R');
	CKeyMgr::CreateKey(_T("CameraZoomReset"), 'Z');

	CKeyMgr::CreateKey(_T("EffectKey"), VK_RBUTTON);
	CKeyMgr::CreateKey(_T("SkillTrigger"), VK_SPACE);


}
void CSkillSceneBuilder::WakeUpScene()
{
	CEditGlobal::EditMainCamera = GetMainCam();
}
void CSkillSceneBuilder::SceneRender()
{
	if (nullptr != CEditGlobal::EditSkillMgr)
	{
		SKILLMGRSTATE SkillMgrState = CEditGlobal::EditSkillMgr->GetSkillMgrState();

		CString DebugString;

		switch (SkillMgrState)
		{
		case STATE_INIT:
			DebugString.Format(_T("SkillMgr : Suspend..."));
			break;
		case STATE_LOADING:
			DebugString.Format(_T("SkillMgr : Loading..."));
			break;
		case STATE_COMPLETE:
			DebugString.Format(_T("SkillMgr : Complete!"));
			break;
		}

		
		RESMGR->DrawFont(_T("탭굴림"), DebugString.GetString(), { -340.0f,400.0f,10.0f }, 1.3f, D3DCOLOR_ARGB(255, 255, 255, 255));
		
		if (true == CEditGlobal::EditSkillMgr->IsSkillProgress())
			DebugString= _T("Skill Progress : Yes");
		else 
			DebugString = _T("Skill Progress : No");
		
		RESMGR->DrawFont(_T("탭굴림"), DebugString.GetString(), { -345.0f,375.0f,10.0f }, 1.3f, D3DCOLOR_ARGB(255, 255, 255, 255));

		UINT SkillCount = (UINT)CEditGlobal::EditSkillMgr->GetSkillCount();

		if (0 != SkillCount) 
		{
			DebugString.Format(_T("Current Skill Count : %u"), SkillCount);
			RESMGR->DrawFont(_T("탭굴림"), DebugString.GetString(), { -345.0f,350.0f,10.0f }, 1.3f, D3DCOLOR_ARGB(255, 255, 255, 255));
		}

		if (true == CEditGlobal::EditSkillMgr->IsSkillStandby())
		{
			UINT SkillNo = CEditGlobal::EditSkillMgr->GetCurSkillNo();
			DebugString.Format(_T("Select Skill No : %u"), SkillNo);
			RESMGR->DrawFont(_T("탭굴림"), DebugString.GetString(), { -340.0f,325.0f,10.0f }, 1.3f, D3DCOLOR_ARGB(255, 255, 255, 255));
			UINT SkillLv = CEditGlobal::EditSkillMgr->GetCurSkillLevel();
			DebugString.Format(_T("Select Skill Level : %u"), SkillLv + 1 );
			RESMGR->DrawFont(_T("탭굴림"), DebugString.GetString(), { -330.0f,300.0f,10.0f }, 1.3f, D3DCOLOR_ARGB(255, 255, 255, 255));
		}

#ifdef _WIN64
		DebugString.Format(_T("Ready : %I64u  Return : %I64u "), CEditGlobal::EditSkillMgr->GetReadyListSize(), CEditGlobal::EditSkillMgr->GetReturnListSize());
#else
		DebugString.Format(_T("Ready : %u  Return : %u "), CEditGlobal::EditSkillMgr->GetReadyListSize(), CEditGlobal::EditSkillMgr->GetReturnListSize());
#endif

		RESMGR->DrawFont(_T("탭굴림"), DebugString.GetString(), { 100.0f,400.0f,10.0f }, 1.3f, D3DCOLOR_ARGB(255, 255, 255, 255));

	}
		
}