#include "stdafx.h"
#include "MapSceneBuilder.h"
#include "EditMapMgr.h"
#include "MapDlg.h"
#include "EditTile.h"

CMapSceneBuilder::CMapSceneBuilder()
	:m_pMapMgr(nullptr), m_fCurCameraRatio(1.f)
	, m_CurMousePos(Vec2{0.f,0.f}), m_bInWindow(true)
{
}


CMapSceneBuilder::~CMapSceneBuilder()
{
	SAFE_DELETE(m_pMapMgr);
}
void CMapSceneBuilder::WakeUpScene() 
{
	CEditGlobal::EditMainCamera = GetMainCam();
}
bool CMapSceneBuilder::SceneBuild() 
{

	CKeyMgr::CreateKey(_T("MouseLeft"), VK_LBUTTON);
	CKeyMgr::CreateKey(_T("MouseRight"), VK_RBUTTON);
	CKeyMgr::CreateKey(_T("DebugMode"), 'F');


	CreateMainLight();
	CreateMainCamera();

	LoadingTexture(&CEditGlobal::vecFolderData[MAP]);
	CEditGlobal::Update();

	CreateMapSprite();
	CreateObjectSprite();

	m_pMapMgr = new CEditMapMgr();
	m_pMapMgr->Init(GetScene(), this);




	CEditGlobal::MapDlg->InitMapSceneEvent();
	return true;
}
void CMapSceneBuilder::SceneUpdate() 
{
	MousePosUpdate();
	CheckCamMove();
	if (true == CKeyMgr::GetWheelEvent())
		ZoomUpdate();
	
	if (true == IsMousePosInWindow()) 
	{
		Vec2 CamPos = Vec2{ GetMainCam()->GetCameraPos().x,GetMainCam()->GetCameraPos().y };
		float ZoomRatio = GetMainCam()->GetZoomRatio();

		m_pMapMgr->FindFocusTile(m_CurMousePos, CamPos, ZoomRatio);
	}
	else
		m_pMapMgr->NoFocusTile();
	

	if (TRUE == KEYDOWN("CameraPosReset"))
		CEditGlobal::EditMainCamera->InitPos();
	
	if(TRUE == KEYDOWN("CameraZoomReset"))
		CEditGlobal::EditMainCamera->SetZoomRatio(1.0f);


	m_pMapMgr->Update();

}
void CMapSceneBuilder::SceneRender()
{
	CString DebugString = _T("");


	if (nullptr == m_pMapMgr->GetCurFocusTile()) 
	{
		DebugString = _T("No Focus Tile ");
		RESMGR->DrawFont(_T("ÅÇ±¼¸²"), DebugString.GetString(), { -340.0f,400.0f,10.0f }, 1.3f, D3DCOLOR_ARGB(255, 255, 255, 255));
	}
	else 
	{
		SPTR<CEditTile>	Tile = m_pMapMgr->GetCurFocusTile();
		POINT Index = Tile->GetTileIndex();
		DebugString.Format(_T("Tile Index : (%d , %d)"), Index.x, Index.y);
		RESMGR->DrawFont(_T("ÅÇ±¼¸²"), DebugString.GetString(), { -340.0f,400.0f,10.0f }, 1.3f, D3DCOLOR_ARGB(255, 255, 255, 255));
		
		DebugString = Tile->GetTileOptionString().c_str();
		RESMGR->DrawFont(_T("ÅÇ±¼¸²"), DebugString.GetString(), { -300.0f,375.f,10.0f }, 1.3f, D3DCOLOR_ARGB(255, 255, 255, 255));

		switch (Tile->GetTileOption())
		{
		case STEPTILE:
		case STEPSTAIRTILE: 
		{
			DebugString = Tile->GetTileColorString().c_str();
			RESMGR->DrawFont(_T("ÅÇ±¼¸²"), DebugString.GetString(), { -340.0f,350.f,10.0f }, 1.3f, D3DCOLOR_ARGB(255, 255, 255, 255));
		}
			break;
		case MAPCHANGETILE:
		{
			DebugString = Tile->GetChangeTileMapName().c_str();
			RESMGR->DrawFont(_T("ÅÇ±¼¸²"), DebugString.GetString(), { -320.0f,350.f,10.0f }, 1.3f, D3DCOLOR_ARGB(255, 255, 255, 255));
			DebugString = Tile->GetSpawnIndexString().c_str();
			RESMGR->DrawFont(_T("ÅÇ±¼¸²"), DebugString.GetString(), { -320.0f,325.f,10.0f }, 1.3f, D3DCOLOR_ARGB(255, 255, 255, 255));
		}
			break;
		case WORLDMAPTILE: 
		{
			DebugString = Tile->GetWorldMapTypeString().c_str();
			RESMGR->DrawFont(_T("ÅÇ±¼¸²"), DebugString.GetString(), { -320.0f,350.f,10.0f }, 1.3f, D3DCOLOR_ARGB(255, 255, 255, 255));
		}
			break;
		}
	}

	

}
void CMapSceneBuilder::CreateMapSprite()
{
	RESMGR->CreateGameSprite(_T("Town"));
	RESMGR->CreateGameSprite(_T("Ruins"));
	RESMGR->CreateGameSprite(_T("Forest"));
	RESMGR->CreateGameSprite(_T("DevilCastle"));
	RESMGR->CreateGameSprite(_T("WeaponStore"));
	RESMGR->CreateGameSprite(_T("ItemStore"));
	RESMGR->CreateGameSprite(_T("TileBase"));
	RESMGR->CreateGameSprite(_T("TileXBase"));
	RESMGR->CreateGameSprite(_T("ObjectDebug"));

}
void CMapSceneBuilder::CreateObjectSprite()
{
	RESMGR->CreateGameSprite(_T("Object"), _T("BigWood"), Vec2{ 128.f,128.f }, Vec2{ 128.f,128.f });
	RESMGR->CreateGameSprite(_T("Object"), _T("BigWood_UpSide"), Vec2{ 128.f,128.f }, Vec2{ 128.f,64.f });
	RESMGR->CreateGameSprite(_T("Object"), _T("SmallWood"), Vec2{ 256.f,128.f }, Vec2{ 64.f,128.f });
	RESMGR->CreateGameSprite(_T("Object"), _T("BlueRoof"), Vec2{ 0.f,256.f }, Vec2{ 128.f,128.f });
	RESMGR->CreateGameSprite(_T("Object"), _T("BigBlueRoof"), Vec2{ 640.f,0.f }, Vec2{ 192.f,192.f });
	RESMGR->CreateGameSprite(_T("Object"), _T("DoubleBlueRoof"), Vec2{ 704.f,192.f }, Vec2{ 320.f,320.f });
	RESMGR->CreateGameSprite(_T("Object"), _T("YellowRoof"), Vec2{ 832.f,0.f }, Vec2{ 192.f,192.f });
	RESMGR->CreateGameSprite(_T("Object"), _T("StreetLamp"), Vec2{ 576.f,0.f }, Vec2{ 64.f,192.f });
	RESMGR->CreateGameSprite(_T("Object"), _T("StonePillar"), Vec2{ 384.f,128.f }, Vec2{ 64.f,128.f });
	RESMGR->CreateGameSprite(_T("Object"), _T("GlassStatue"), Vec2{ 448.f,128.f }, Vec2{ 64.f,128.f });
	RESMGR->CreateGameSprite(_T("Object"), _T("GoldCoin"), Vec2{ 0.f,0.f }, Vec2{128.f,128.f});
	RESMGR->CreateGameSprite(_T("Object"), _T("SilverCoin"), Vec2{ 128.f,0.f }, Vec2{ 128.f,128.f });
	RESMGR->CreateGameSprite(_T("Object"), _T("GoldCoinBox"), Vec2{256.f,0.f }, Vec2{ 64.f,64.f });
	RESMGR->CreateGameSprite(_T("Object"), _T("SilverCoinBox"), Vec2{ 256.f,64.f }, Vec2{ 64.f,64.f });
	RESMGR->CreateGameSprite(_T("Object"), _T("GoldBar"), Vec2{ 320.f,0.f }, Vec2{ 64.f,64.f });
	RESMGR->CreateGameSprite(_T("Object"), _T("SilverBar"), Vec2{ 320.f,64.f }, Vec2{ 64.f,64.f });
	RESMGR->CreateGameSprite(_T("Object"), _T("DragonStatue"), Vec2{ 448.f,0.f }, Vec2{ 128.f,128.f });
	RESMGR->CreateGameSprite(_T("Object"), _T("HumanSkull"), Vec2{ 512.f,128.f }, Vec2{ 64.f,64.f });
	RESMGR->CreateGameSprite(_T("Object"), _T("BeastSkull"), Vec2{ 512.f,192.f }, Vec2{ 64.f,64.f });
	RESMGR->CreateGameSprite(_T("Object"), _T("Bush2"), Vec2{ 128.f,256.f }, Vec2{ 64.f,128.f });
	RESMGR->CreateGameSprite(_T("Object"), _T("DevilStatue"), Vec2{ 384.f,0.f }, Vec2{ 64.f,128.f });
	RESMGR->CreateGameSprite(_T("Object"), _T("weed1"), Vec2{ 192.f,256.f }, Vec2{ 64.f,64.f });
	RESMGR->CreateGameSprite(_T("Object"), _T("weed2"), Vec2{ 256,256.f }, Vec2{ 64.f,64.f });
	RESMGR->CreateGameSprite(_T("Object"), _T("BlueFlower"), Vec2{ 320.f,256.f }, Vec2{ 64.f,64.f });
	RESMGR->CreateGameSprite(_T("Object"), _T("Stub"), Vec2{ 384.f,256.f }, Vec2{ 64.f,64.f });
	RESMGR->CreateGameSprite(_T("Object"), _T("Bush1"), Vec2{ 448.f,256.f }, Vec2{ 64.f,64.f });
	RESMGR->CreateGameSprite(_T("Object"), _T("WhiteFlower"), Vec2{ 192.f,320.f }, Vec2{ 64.f,64.f });
	RESMGR->CreateGameSprite(_T("Object"), _T("PinkFlower"), Vec2{ 256.f,320.f }, Vec2{ 64.f,64.f });
	RESMGR->CreateGameSprite(_T("Object"), _T("OrangeFlower"), Vec2{ 320.0f,320.f }, Vec2{ 64.f,64.f });
	RESMGR->CreateGameSprite(_T("Object"), _T("Log"), Vec2{ 384.f,320.f }, Vec2{ 64.f,64.f });
}
void CMapSceneBuilder::MousePosUpdate()
{
	if (nullptr != CEditGlobal::MainWindow)
		m_CurMousePos = CEditGlobal::MainWindow->GetMousePos();
}

void CMapSceneBuilder::CheckCamMove()
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
		GetScene()->GetMainCamera()->Move(Dir*DELTATIME*180.0f);
	}

	if (TRUE == KEYDOWN("DebugMode")) 
		CEditGlobal::MainWindow->GetDebugMgr()->ChangeDebugMode();
	

}


void CMapSceneBuilder::ZoomUpdate()
{

	if (CKeyMgr::GetWheelSign() > 0)
	{
		if (m_fCurCameraRatio <= 0.15f)
		{
			m_fCurCameraRatio = 0.15f;
			GetMainCam()->SetZoomRatio(m_fCurCameraRatio);
		}
		else
		{
			m_fCurCameraRatio -= 0.05f;
			GetMainCam()->SetZoomRatio(m_fCurCameraRatio);
		}
	}
	// ÈÙÀ» ¾ÕÀ¸·Î µ¹¸®¸é(Ä«¸Þ¶ó´Â µÚ·Î °£´Ù)
	else
	{
		if (m_fCurCameraRatio >= 2.0f)
		{
			m_fCurCameraRatio = 2.0f;
			GetMainCam()->SetZoomRatio(m_fCurCameraRatio);
		}
		else
		{
			m_fCurCameraRatio += 0.05f;
			GetMainCam()->SetZoomRatio(m_fCurCameraRatio);
		}
	}
}
const bool CMapSceneBuilder::IsMousePosInWindow()
{
	POINT Size = CEditGlobal::MainWindow->WndSize();

	float HX = Size.x*0.5f;
	float HY = Size.y*0.5f;

	if (m_CurMousePos.x >= HX || m_CurMousePos.x <= -HX)
		return false;


	if (m_CurMousePos.y >= HY || m_CurMousePos.y <= -HY)
		return false;

	return true;
}