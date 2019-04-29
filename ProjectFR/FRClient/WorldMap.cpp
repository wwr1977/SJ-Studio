#include "Precom.h"
#include "WorldMap.h"
#include "WorldMapUI.h"
#include "TileMap.h"
#include "MainMapMgr.h"
#include "EventMgr.h"
#include "Tile.h"

CWorldMap::CWorldMap()
	:m_pMainMapMgr(nullptr),m_WorldMapRen(nullptr)
	, m_CurFocusUI(nullptr)
	, m_CurTrigerTile(nullptr)
	, m_pEventMgr(nullptr)
{
	m_WorldMapUI.clear();
	m_vecLine.clear();
}


CWorldMap::~CWorldMap()
{
	m_WorldMapUI.clear();
	m_vecLine.clear();
	m_CurFocusUI = nullptr;
	m_CurTrigerTile = nullptr;
}

void CWorldMap::InitData(CMainMapMgr* _MapMgr)
{
	if (nullptr == _MapMgr)
		return;

	m_pMainMapMgr = _MapMgr;

	m_WorldMapRen = ACTOR->CreateCom<CFixRenderer>(RENDATA(LAYER_BACKGROUND, 1.0f));
	m_WorldMapRen->SetSprite(_T("WorldMap"));
	ACTOR->SetPos(Vec3{ 0.0f,0.0f,5.0f });

	CWorldMapUI::SetWorldMap(this);

	m_vecLine.push_back(ACTOR->CreateCom<CLineRenderer>(RENDATA(LAYER_MAPOBJECT, 0.5f)));
	m_vecLine.push_back(ACTOR->CreateCom<CLineRenderer>(RENDATA(LAYER_MAPOBJECT, 0.5f)));
	m_vecLine.push_back(ACTOR->CreateCom<CLineRenderer>(RENDATA(LAYER_MAPOBJECT, 0.5f)));
	

	for (size_t i = 0; i < m_vecLine.size(); i++) 
	{
		m_vecLine[i]->SetSprite(_T("ColorBase"));
		m_vecLine[i]->SetColor(D3DXCOLOR(0.15f, 1.0f, 1.0f, 1.0f));
	}
	
	m_vecLine[0]->SettingLineData(Vec3{ -475.0f,-100.0f,5.0f }, Vec3{ -375.0f,175.0f,5.0f });
	m_vecLine[1]->SettingLineData(Vec3{ -475.0f,-100.0f,5.0f }, Vec3{ -150.0f,-250.0f,5.0f });
	m_vecLine[2]->SettingLineData(Vec3{ -150.0f,-250.0f,5.0f }, Vec3{ 25.0f,25.0f,5.0f });

	m_WorldMapUI.push_back(ACTOR->CreateCom<CWorldMapUI>(Vec3{-475.0f,-100.0f,0.0f}));
	m_WorldMapUI.push_back(ACTOR->CreateCom<CWorldMapUI>(Vec3{-375.0f,175.0f,0.0f}));
	m_WorldMapUI.push_back(ACTOR->CreateCom<CWorldMapUI>(Vec3{ -150.0f,-250.0f,0.0f }));
	m_WorldMapUI.push_back(ACTOR->CreateCom<CWorldMapUI>(Vec3{ 25.0f,25.0f,0.0f }));

	m_WorldMapUI[TOWN]->SetPairTileMap(TOWN);
	m_WorldMapUI[RUINS]->SetPairTileMap(RUINS);
	m_WorldMapUI[FOREST]->SetPairTileMap(FOREST);
	m_WorldMapUI[DEVILCASTLE]->SetPairTileMap(DEVILCASTLE);
	

	/*
	m_WorldMapUI[2]->SetUIState(UI_NOACTIVE);
	m_WorldMapUI[3]->SetUIState(UI_NOACTIVE);
	*/

	ACTOR->Off();
}
void CWorldMap::Update() 
{
	if (nullptr == m_CurFocusUI)
		return;

	m_CurFocusUI->UpdateTriger();
}
void CWorldMap::On()
{
	for (size_t i = 0; i < m_WorldMapUI.size(); i++)
	{
		if (CClientGlobal::CurTileMap->GetTileMapNo() == m_WorldMapUI[i]->GetPairTileMapNo()) 
		{
			m_WorldMapUI[i]->SetUIState(UI_FOCUS);
			m_CurFocusUI = m_WorldMapUI[i];
		}
		else 
			m_WorldMapUI[i]->SetUIState(UI_NOFOCUS);

		//else if(UI_NOACTIVE !=  m_WorldMapUI[i]->GetCurState())
		//	m_WorldMapUI[i]->SetUIState(UI_NOFOCUS);
	}

	SettingActiveWorldMapUI();

	ACTOR->On();
}
void CWorldMap::On(SPTR<CTile> _TrigerTile)
{
	SetCurTrigerTile(_TrigerTile);
	On();
}
void CWorldMap::SetCurTrigerTile(SPTR<CTile> _TrigerTile)
{
	if (nullptr == _TrigerTile)
		return;

	m_CurTrigerTile = _TrigerTile;
}
void CWorldMap::Off()
{
	ACTOR->Off();
}
void CWorldMap::ChangeFocus(const UINT& _UINo)
{
	if (_UINo >= m_WorldMapUI.size())
		return;

	WORLDUISTATE State = m_WorldMapUI[_UINo]->GetCurState();

	if (UI_NOFOCUS == State)
	{
		m_CurFocusUI->SetUIState(UI_NOFOCUS);
		m_CurFocusUI = m_WorldMapUI[_UINo];
		m_CurFocusUI->SetUIState(UI_FOCUS);
	}
}
void CWorldMap::SelectTileMapEvent(const MAINMAPNAME& _MapNo)
{
	m_pMainMapMgr->SelectTileMapEvent(m_CurTrigerTile,_MapNo);
}
void CWorldMap::SetEventMgr(CEventMgr* _Mgr)
{
	if (nullptr != _Mgr)
		m_pEventMgr = _Mgr;
}
void CWorldMap::SettingActiveWorldMapUI()
{
	UINT TileMapNo = CClientGlobal::CurTileMap->GetTileMapNo();

	switch (TileMapNo)
	{
	case TOWN:
	{
		UINT Q1, Q3; 
		Q1 = m_pEventMgr->GetQuestCount(_T("Quest1"));
		Q3 = m_pEventMgr->GetQuestCount(_T("Quest3"));
			
		if (Q3 >= 1) 
		{
			//m_WorldMapUI[RUINS]->SetUIState();
			//m_WorldMapUI[FOREST]->SetUIState(UI_NOACTIVE);
			m_WorldMapUI[DEVILCASTLE]->SetUIState(UI_NOACTIVE);
			return;
		}
		else if (Q1 >= 2)
		{
			m_WorldMapUI[FOREST]->SetUIState(UI_NOACTIVE);
			m_WorldMapUI[DEVILCASTLE]->SetUIState(UI_NOACTIVE);
			return;
		}
		else 
		{
			m_WorldMapUI[RUINS]->SetUIState(UI_NOACTIVE);
			m_WorldMapUI[FOREST]->SetUIState(UI_NOACTIVE);
			m_WorldMapUI[DEVILCASTLE]->SetUIState(UI_NOACTIVE);
			return;
		}
	}
	break;
	case RUINS: 
	{
		UINT  Q3 = m_pEventMgr->GetQuestCount(_T("Quest3"));

		if (Q3 >= 1) 
		{
			m_WorldMapUI[DEVILCASTLE]->SetUIState(UI_NOACTIVE);
			return;
		}
		else 
		{
			m_WorldMapUI[FOREST]->SetUIState(UI_NOACTIVE);
			m_WorldMapUI[DEVILCASTLE]->SetUIState(UI_NOACTIVE);
			return;
		}
	}
	break;
	case FOREST: 
	{
		UINT TileType = m_CurTrigerTile->GetChangeMapOption();

		if (CTile::TYPE2 == TileType)
		{
			m_WorldMapUI[TOWN]->SetUIState(UI_NOACTIVE);
			m_WorldMapUI[RUINS]->SetUIState(UI_NOACTIVE);
			return;
		}
		else 
		{
			m_WorldMapUI[DEVILCASTLE]->SetUIState(UI_NOACTIVE);
			return;
		}
		
	}
	break;
	case DEVILCASTLE: 
	{
		m_WorldMapUI[TOWN]->SetUIState(UI_NOACTIVE);
		m_WorldMapUI[RUINS]->SetUIState(UI_NOACTIVE);
		return;
	}
		break;
	}

}