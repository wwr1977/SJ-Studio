#include "Precom.h"
#include "WorldMapUI.h"
#include "WorldMap.h"
#include "GameMaster.h"

CWorldMap*	CWorldMapUI::pWorldMap = nullptr;

void CWorldMapUI::SetWorldMap(CWorldMap* _WorldMap)
{
	if (nullptr == _WorldMap)
		return;

	pWorldMap = _WorldMap;
}
CWorldMapUI::CWorldMapUI()
	: m_WorldUIRen(nullptr), m_CurState(UI_NOACTIVE)
	, m_PrevUINo(-1), m_NextUINo(-1)
	, m_fAccTime(0.0f)
{
}


CWorldMapUI::~CWorldMapUI()
{
}

void CWorldMapUI::InitData(const Vec3& _InitPos)
{
	m_WorldUIRen = ACTOR->CreateCom<CFixRenderer>(RENDATA(LAYER_MAPOBJECT, 1.0f));
	m_WorldUIRen->SetSprite(_T("WorldNoFocus"));
	m_WorldUIRen->SetRenPivot(_InitPos);
	m_WorldUIRen->SetCustomSize(Vec2{ 32.0f,32.0f });
	Off();
}
void CWorldMapUI::UpdateTriger()
{
	if (true == KEYDOWN("LEFT"))
	{
		if (m_PrevUINo != -1)
			pWorldMap->ChangeFocus(m_PrevUINo);
	}
	else if (true == KEYDOWN("RIGHT"))
	{
		if (m_NextUINo != -1)
			pWorldMap->ChangeFocus(m_NextUINo);	
	}
	else if (true == KEYDOWN("Enter"))
		pWorldMap->SelectTileMapEvent(m_PairTileMapNo);
		
	
}
void CWorldMapUI::SetUIState(const WORLDUISTATE& _State) 
{
	if (MAXWORLDUISTATE <= _State)
		return;

	m_CurState = _State;

	if (nullptr == m_WorldUIRen)
		return;

	switch (m_CurState)
	{
	case UI_NOACTIVE: 
		m_WorldUIRen->SetSprite(_T("WorldNoActive"));
		return;
	case UI_NOFOCUS:
		m_WorldUIRen->SetSprite(_T("WorldNoFocus"));
		return;
	case UI_FOCUS:
		m_WorldUIRen->SetSprite(_T("WorldFocus"));
		return;
	}

	return;
}
void CWorldMapUI::WorldUIOn()
{
	ACTOR->On();
}
void CWorldMapUI::WorldUIOff()
{
	ACTOR->Off();
}
void CWorldMapUI::SetPairTileMap(const MAINMAPNAME& _MapNo)
{
	if (_MapNo >= MAXMAINMAPNAME)
		return;

	m_PairTileMapNo = _MapNo;

	switch (m_PairTileMapNo)
	{
	case TOWN: 
	{
		m_PrevUINo = RUINS;
		m_NextUINo = FOREST;
	}
	break;
	case RUINS:
		m_NextUINo = TOWN;	
		break;
	case FOREST:
	{
		m_PrevUINo = TOWN;
		m_NextUINo = DEVILCASTLE;
	}
		break;
	case DEVILCASTLE:
		m_PrevUINo = FOREST;
		break;
	}
}
const MAINMAPNAME CWorldMapUI::GetPairTileMapNo()
{
	return m_PairTileMapNo;
}
const WORLDUISTATE CWorldMapUI::GetCurState()
{
	return m_CurState;
}
const UINT CWorldMapUI::GetPrevUINo() 
{
	return m_PrevUINo;
}
const UINT CWorldMapUI::GetNextUINo()
{
	return m_NextUINo;
}