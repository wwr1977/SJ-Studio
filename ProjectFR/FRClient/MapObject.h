#pragma once
#include <Logic.h>
class CMapObject : public CLogic
{
	static class CMainMapMgr*			MapMgr;
	static class SPTR<CGameSprite>		ObjectDebugSprite;

public:
	static void SetMapMgr(CMainMapMgr* _Mgr);

private:
	class CTileMap*							m_BelongTileMap;
	UINT									m_ObjectNo;
	tstring									m_ObjectKName;
	SPTR<CFixRenderer>						m_ObjectRen;
	SPTR<CFixRenderer>						m_DebugRen;
	// �ڽ��� ��ġ�� Ÿ�ϸ� ��ġ�� ���� �ϴ��� �ǹ��Ѵ�.
	POINT									m_LBIndex;

public:
	void Init();
	void InitData(const UINT& _ObjectNo);
	void DebugRender();

public:
	void SetTileLBIndex(const POINT& _Index);
	void SetTileMap(CTileMap* _TileMap);
	const POINT GetLBIndex();

	UINT GetObjectNo();

public:
	void MapObjectOn();
	void MapObjectOff();
	void EnableMapObject(const BOOL& _Enable);

public:
	CMapObject();
	~CMapObject();
};

