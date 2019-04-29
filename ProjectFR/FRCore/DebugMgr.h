#pragma once
#include "ObjBase.h"

class CGameWindow;
class ResourceMgr;
class CGameMesh;
class CDebugMgr : public CObjBase
{
private:
	static VTXLINE LineVtx[5];
	BOOL	m_bDebugMode;

private:
	CGameWindow*									m_PairWindow;
	LPDIRECT3DDEVICE9								m_PairDevice;


private:
	map<DWORD, SPTR<CGameMesh>>::iterator m_FindLineIter;
	map<DWORD, SPTR<CGameMesh>> m_LineMesh;
	Mat							matDebugPos;
	Mat							matDebugSize;
	
public:
	void DebugModeOff() { m_bDebugMode = FALSE; }
	void DebugModeOn() { m_bDebugMode = TRUE; }
	BOOL IsDebugMode() 
	{
		return m_bDebugMode;
	}
	void ChangeDebugMode();

public:
	void RectRender(DWORD _Color, const Vec3& _Pos, const Vec3& _Size);
	void RectRender(DWORD _Color, const Mat& _Mat);

private:
	SPTR<CGameMesh> CreateLineMesh(DWORD _Color);
	
public:
	CDebugMgr(CGameWindow* _pGameWnd, LPDIRECT3DDEVICE9 _pDevice);
	~CDebugMgr();
};

