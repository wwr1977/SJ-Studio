#pragma once
#include "ObjBase.h"
#include"GameScene.h"


class CResourceMgr;
class CDebugMgr;
class CGDevice;
class CGameWindow :public CObjBase
{
protected:
	//Window Infomation
	tstring				m_Title;
	HWND				m_hWnd;
	HDC					m_hDC;
	HCURSOR				m_hCursor;
	POINT				m_WndPos;
	POINT				m_WndSize;
	SPTR<CGDevice>		m_PairDevice;
	SPTR<CResourceMgr>	m_PairResMgr;
	SPTR<CDebugMgr>		m_PairDebugMgr;

	COLORREF			m_ColorRef;
private:
	//Scene Data
	map<tstring, SPTR<CGameScene>>						m_mapScene;
	SPTR<CGameScene>									m_CurScene;

//public:
//	enum WINDOWSTYLE
//	{
//		GAME_WINDOW,
//		SPLASH_WINDOW,
//		MAXWINDOWSTYLE
//	};

public:
	void Title(tstring _Title) { m_Title = _Title; }
	tstring Title() { return m_Title; }
	const TCHAR* Title_str() const { return m_Title.c_str(); }
	HWND hWnd() { return m_hWnd; }
	HDC  hDC() { return m_hDC; }
	POINT WndSize() { return  m_WndSize; }
	LONG WndWidth() { return m_WndSize.x; }
	LONG WndHeight() { return m_WndSize.y; }
	float WndFWidth() { return (float)m_WndSize.x; }
	float WndFHeight() { return (float)m_WndSize.y; }
	POINT WndPos() { return m_WndPos; }
	float WndAspect() { return (float)m_WndSize.x / (float)m_WndSize.y; }
	POINT GetMousePoint();
	Vec2 GetMousePos();
	Vec3 WndPosToWorldPos(const POINT& _WndPos);
	Vec3 WndPosToWorldPos(float _WndX,float _WndY);

public:
	template<typename T>
	bool CreateScene(tstring _Name,bool _Load = false) 
	{
		SPTR<CGameScene> Scene = FindGameScene(_Name);

		if (nullptr != Scene) 
		{
			return FALSE;
		}

		CSceneBuilder* SceneBuilder = new T();
		CGameScene* NewScene = new CGameScene(_Name, SceneBuilder ,this);

		SceneBuilder->SetGameScene(NewScene);

		if (FALSE == NewScene->isSetting()) 
		{
			return FALSE;
		}

		m_mapScene.insert(map<tstring, SPTR<CGameScene>>::value_type(_Name, NewScene));

		if (nullptr == m_CurScene) 
		{
			//ChangeScene(_Name);
		}

		/*if (true == _Load && m_CurScene)
		{
			NewScene->Loading();
		}*/
		
		if (true == _Load )
		{
			NewScene->Loading();
		}

		return TRUE;
	}
	bool ChangeScene(const tstring& _Name);
	SPTR<CGameScene> FindGameScene(const tstring& _Name);
	

public:
	CGameWindow* CreateGameWindow();
	CGameWindow* CreateSplashWindow();
	ATOM MyRegisterClass(HINSTANCE _hInst,const HBRUSH& BackColor = (HBRUSH)(COLOR_WINDOW + 1));
	//ATOM MySRegisterClass(HINSTANCE _hInst);
	bool ShowGameWindow(int _ShowMode = SW_HIDE);
	bool SetWindowSize(POINT _Size ,POINT _Pos);
	bool UpdateCursor();
	bool ChangeBaseCursor(tstring _Path);
	bool InvalidateWndCursor();
	bool ChangeIcon(HICON _hIcon);
	bool ChangeSmallIcon(HICON _hIcon);
	bool ChangeWndTitle(const tstring& _Title);
	const BOOL SetGameWindowColorKey(const COLORREF& _ColorKey);
	const BOOL SetGameWindowAlpha(const BYTE& _Alpha);

public:
	bool Progress();

public:
	virtual bool WndProc(UINT message, WPARAM wParam, LPARAM lParam);


public:		// Device 관련
	bool CreateDevice();
	void DeviceRenderStart();
	void DeviceRenderEnd();
	LPDIRECT3DDEVICE9	GetLPDevice();
	SPTR<CGDevice>		GetWndDevice();

public:		// ResourceMgr 관련
	SPTR<CResourceMgr> GetResourceMgr() 
	{
		return m_PairResMgr;
	}
	SPTR<CDebugMgr>	GetDebugMgr() 
	{
		return m_PairDebugMgr;
	}
	

public:
	CGameWindow();
	CGameWindow(tstring _Name,tstring _Title = _T(""));
	CGameWindow(HWND _hWnd, tstring _Name, tstring _Title = _T(""));
	~CGameWindow();
};

