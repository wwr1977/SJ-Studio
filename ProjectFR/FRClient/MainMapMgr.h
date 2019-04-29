#pragma once
#include<ObjBase.h>


class CGameMaster;
class CMapBackground;
class CTileMap;
class CGameScene;
class CWorldMap;
class CTile;
class CNPCBase;
class CMainMapMgr : public CObjBase
{
	enum
	{
		MUSIC_START,
		MUSIC_END,
		MAXMUSIC
	}m_CurMusicOp;
private:
	CGameScene*								m_pMainScene;
	CSceneBuilder*							m_pMainSceneBuilder;
	CGameMaster*							m_pGameMaster;
	
	SPTR<CWorldMap>							m_WorldMap;
	SPTR<CTileMap>							m_CurTileMap;
	map<UINT, SPTR<CTileMap>>				m_MainTileMap;
	

	// 월드맵을 통한 이동이 존재할때 캐릭터의 초기 타일 위치를 저장
	//map<TileMapKey, POINT>				m_WorldMapInitPoint;
	vector<tstring>							m_ObjectName;
	

	// 메인씬 BGM 플레이어
	SPTR<CSoundPlayer>						m_MapSpeaker;
	tstring									m_CurBgmName;
	bool									m_bBGMPause;
	bool									m_bMusicUpdate;
	float									m_fAccMusicTime;
	float									m_fSpeakerVol;
	float									m_fMusicFadeTime;

public:
	//typedef CMainMapMgr CTileMapMgr;

	void Init(CGameScene* _MainScene, CSceneBuilder* _SceneBuilder);
	void Update();
	void ResetMgr();
public:
	CTileMap*  CreateTileMap(const TILEMAPDATA& _Data);
	SPTR<CTileMap> FindTileMap(const UINT& _TileMapNo);
	void InitializeTileMap();
	SPTR<CTileMap>  ChangeTileMap(const UINT& _TileMapNo);
	const size_t GetTileMapCount();
	void ChangeCurTileMapDebugMode();
	void SetCurTileMapDeugMode(const bool& _Mode);
	void ShowWolrdMapEvent(SPTR<CTile> _TrigerTile);
	void SelectTileMapEvent(SPTR<CTile> TrigerTile , const MAINMAPNAME& _MapNo);
	
public:
	void LoadTileData();

	// 맵 오브젝트 관련	
public:
	const size_t GetObjectCount();
	const tstring GetObjectName(const UINT& _ObjectNo);
	void LoadMapObjectData();
	void PushMapNPC(const MAINMAPNAME& _MapName, SPTR<CNPCBase> _NPC);


public:
	void CurTileMapMusicOn();
	void MapMusicStart();
	void MapMusicEnd();
	void MapMusicPause(const bool& _Pause);
	void MusicOnEvent(const bool& _MusicOn);
	const bool IsMapBGMPause();


public:
	void WorldMapActive(const bool& _Enable);
	void SetEventMgr(class CEventMgr* _Mgr);
public:
	CMainMapMgr();
	CMainMapMgr(CGameMaster* _Master);
	~CMainMapMgr();
};

