#pragma once
#include "ObjBase.h"
#include "WindowMgr.h"

class CGameWindow;
class CGDevice :public CObjBase
{
public:
	friend bool CWindowMgr::Init(HINSTANCE _hInst);
	friend CGameWindow;

private:
	static LPDIRECT3D9						m_pD3D;
	static D3DCAPS9							m_Caps;
	static DWORD							m_dwVtProcessing;

private:
	CGameWindow*							m_pPairWindow;
	D3DPRESENT_PARAMETERS					m_DevicePara;
	LPDIRECT3DDEVICE9						m_pDevice;
	bool									m_bRender;
	COLORFACTOR								m_ColorFactor;

private:	//	Static 
	static void SetDeviceInfomation();


private:	//  Member Function
	LPDIRECT3DDEVICE9 InitDevice();
	void SettingParameter();
public:
	void RenderStart();
	void RenderEnd();

public:
	void SetColorFactor(COLORFACTOR _ColorFactor);
	COLORFACTOR GetColorFactor();
	LPDIRECT3DDEVICE9 GetDevice();


public:
	CGDevice(CGameWindow* _Window);
	~CGDevice();
};

