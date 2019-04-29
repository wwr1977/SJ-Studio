#pragma once
#include <Logic.h>

class CMainMapMgr;
class CWorldMapUI;
class CTile;
class CLineRenderer;
class CWorldMap : public CLogic
{
private:
	CMainMapMgr*						m_pMainMapMgr;
	class CEventMgr*					m_pEventMgr;
	SPTR<CFixRenderer>					m_WorldMapRen;
	SPTR<CTile>							m_CurTrigerTile;

	SPTR<CWorldMapUI>					m_CurFocusUI;
	vector<SPTR<CWorldMapUI>>			m_WorldMapUI;
	vector<SPTR<CLineRenderer>>			m_vecLine;

public:
	void InitData(CMainMapMgr* _MapMgr);
	void Update();

public:
	void On();
	void On(SPTR<CTile> _TrigerTile);
	void SetCurTrigerTile(SPTR<CTile> _TrigerTile);
	void Off();
	void ChangeFocus(const UINT& _UINo);
	void SelectTileMapEvent(const MAINMAPNAME& _MapNo);
	void SetEventMgr(CEventMgr* _Mgr);

public:
	void SettingActiveWorldMapUI();

public:
	CWorldMap();
	~CWorldMap();
};

