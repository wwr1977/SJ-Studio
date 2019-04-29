#include "stdafx.h"
#include "EditCoreBuilder.h"
#include "SkillSceneBuilder.h"
#include "StatSceneBuilder.h"
#include "MapSceneBuilder.h"
#include "EditorHeader.h"
#include<GDevice.h>
#include"EffectDlg.h"
CEditCoreBuilder::CEditCoreBuilder(HWND _BaseWnd)
	:m_hBaseWnd(_BaseWnd)
{

}


CEditCoreBuilder::~CEditCoreBuilder()
{
}
bool CEditCoreBuilder::CoreBuild()
{
	assert(m_hBaseWnd);
	PathSetting();

	SPTR<CGameWindow> BaseWnd = CWindowMgr::Inst().CreateGameWindow(m_hBaseWnd, _T("DXView"));
	//Device¸¦ ¸¸µçµÚ Render¼³Á¤À» ÁöÁ¤ÇÑ´Ù
	CEditGlobal::MainWindow = BaseWnd;
	CEditGlobal::GlobalInit();

	BaseWnd->CreateDevice();
	BaseWnd->GetLPDevice()->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	//BaseWnd->GetWndDevice()->SetColorFactor({ 0, 0, 255 });
	CRenderer::RendererInit(BaseWnd->GetLPDevice());

	// ºûÀ» °³³ä ÀÚÃ¼¸¦ ¾ø´Ù.
	BaseWnd->GetLPDevice()->SetRenderState(D3DRS_LIGHTING, true);
	BaseWnd->GetLPDevice()->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	BaseWnd->GetLPDevice()->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	BaseWnd->GetLPDevice()->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	BaseWnd->GetLPDevice()->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_POINT);
	
	
	CEditGlobal::EditResourceMgr =  BaseWnd->GetResourceMgr();

	CWindowMgr::Inst().ChangeMouseCursor(_T("DXView"), CFilePathMgr::GetPath(_T("Texture")) + _T("DX9Cursor.ico"));
	BaseWnd->CreateScene<CSkillSceneBuilder>(_T("SkillScene"), true);
	BaseWnd->CreateScene<CStatSceneBuilder>(_T("StatScene"), true);
	BaseWnd->CreateScene<CMapSceneBuilder>(_T("MapScene"), true);

	BaseWnd->ChangeScene(_T("SkillScene"));

	BaseWnd->GetResourceMgr()->LoadGameFont(_T("±Ã¼­"), { 12.0f,20.0f });
	BaseWnd->GetResourceMgr()->LoadGameFont(_T("µ¸¿ò"), { 12.0f,20.0f });
	BaseWnd->GetResourceMgr()->LoadGameFont(_T("±¼¸²"), _T("ÅÇ±¼¸²"), { 10.0f,15.0f });


	CEditGlobal::SoundLoading();

	return TRUE;
}	
void CEditCoreBuilder::PathSetting()
{
	CFilePathMgr::AddRootFolder(_T("Texture"));
	CFilePathMgr::AddRootFolder(_T("Sound"));
	CFilePathMgr::AddRootFolder(_T("EditData"));
	
	CFilePathMgr::AddPlusFolder(_T("Texture"), _T("Character"));
	CFilePathMgr::AddPlusFolder(_T("Texture"), _T("Monster"));
	CFilePathMgr::AddPlusFolder(_T("Texture"), _T("Effect"));
	CFilePathMgr::AddPlusFolder(_T("Texture"), _T("UI"));
	CFilePathMgr::AddPlusFolder(_T("Texture"), _T("Map"));
	CFilePathMgr::AddPlusFolder(_T("Texture"), _T("PreLoad"));

	CFilePathMgr::AddPlusFolder(_T("Sound"), _T("SkillSound"));
}