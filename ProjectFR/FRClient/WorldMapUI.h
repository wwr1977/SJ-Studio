#pragma once
#include <Logic.h>

class CWorldMap;
class CWorldMapUI : public CLogic
{
	static CWorldMap*					pWorldMap;

public:
	static void SetWorldMap(CWorldMap* _WorldMap);

public:
	SPTR<CFixRenderer>					m_WorldUIRen;
	WORLDUISTATE						m_CurState;
	MAINMAPNAME							m_PairTileMapNo;
	
	UINT								m_PrevUINo;
	UINT								m_NextUINo;

	float								m_fAccTime;
public:
	void InitData(const Vec3& _InitPos);
	void UpdateTriger();

public:
	void SetUIState(const WORLDUISTATE& _State);
	void WorldUIOn();
	void WorldUIOff();
	void SetPairTileMap(const MAINMAPNAME& _MapNo);
	const MAINMAPNAME GetPairTileMapNo();
	const WORLDUISTATE GetCurState();
	const UINT GetPrevUINo();
	const UINT GetNextUINo();
public:
	CWorldMapUI();
	~CWorldMapUI();
};

