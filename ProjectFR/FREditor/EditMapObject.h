#pragma once
#include <Logic.h>
class CEditMapObject : public CLogic
{
	static class CEditMapMgr*			MapMgr;
	static class SPTR<CGameSprite>		ObjectDebugSprite;

public:
	static void SetMapMgr(CEditMapMgr* _Mgr);

private:
	class CEditTileMap*						m_BelongTileMap;
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
	void SetTileMap(CEditTileMap* _TileMap);
	const POINT GetLBIndex();
	
	MAPOBJECTDATA GetObjectData();
	UINT GetObjectNo();

public:
	void MapObjectOn();
	void MapObjectOff();


	// Edit ����
public:
	bool							m_bTest;
	void EnableMapObject(const BOOL& _Enable);
	void SetTestMapObject();
	void ChangeMapObject(const UINT& _ObjectNo);
	void DeadMapObject();
public:
	CEditMapObject();
	~CEditMapObject();
};

