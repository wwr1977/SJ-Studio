#include <ClientConnectMgr.h>
#include "TestCoreBuilder.h"
#include <Collider.h>
#include <Scene.h>
#include <FilePathMgr.h>
#include "TestScene.h"
#include "TestScene2.h"
#include "IntroScene.h"
#include <DeadByDaylightGlobal.h>

CTestCoreBuilder::CTestCoreBuilder()
{
}


CTestCoreBuilder::~CTestCoreBuilder()
{
}
const BOOL CTestCoreBuilder::Build()
{
	GlobalDBD::Init();
	GETSINGLE(CClientConnectMgr).Init();
	CCollider::ColliderInit();
	

	// ���ε���  ���� ������ ������� ������
	// �� �۾��� ������ �κ� ������ �̵�
	// 1. ��Ʈ�� �� �ε� : �ؽ���,��������Ʈ �Ͱ��� �׷��� ���ҽ� �ε�
	// 2. �κ� �� �ε� : �κ� UI ��  �κ���� ĳ���� �̸� �����
	// 3. ����(����) �� �ε� : ������ ������ �ε� �� ������Ʈ �̸� ����
	// ������ ���� �� �� 
	//CScene::CreateScene<CTestScene>(L"TestScene", true);
	//CScene::CreateScene<CIntroScene>(L"IntroScene", true);
	//CScene::ChangeScene(L"IntroScene");



	// ������ ���� �ȵ� ��
	CScene::CreateScene<CTestScene2>(L"TestScene2", true);
	CScene::ChangeScene(L"TestScene2");

	return TRUE;
}

void CTestCoreBuilder::BuilderProc(HWND _hWnd, UINT _Msg, WPARAM _wParam, LPARAM _lParam)
{

}
const BOOL CTestCoreBuilder::Release()
{
	GlobalDBD::Release();
	//GETSINGLE(ClientConnector).DisConnectLogicServer();
	return TRUE;
}


CORESTART(CTestCoreBuilder, L"Test Client", Vec2(1360, 768), Vec2(0, 15))