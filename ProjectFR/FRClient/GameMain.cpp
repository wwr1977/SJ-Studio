#include"Precom.h"
#include"ClientHeader.h"
#include"DX9CoreBuilder.h"



int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);


	
	CDX9Core::Init(hInstance,new CDX9CoreBuilder());
	
	CDX9Core::Progress();

	CDX9Core::ManagerRelease();

	CClientGlobal::ReleaseGlobalData();
	
	return 0;
}
