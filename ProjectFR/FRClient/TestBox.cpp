#include "Precom.h"
#include "TestBox.h"
#include <GameMultiSprite.h>


CTestBox::CTestBox()
	:m_fAccTime(1.0f), m_fURot(0.0f), m_fCurWaveSize(30.0f)
	, m_BackRenderer(nullptr), m_Renderer3(nullptr), m_TestSkillIcon(nullptr)
{
}


CTestBox::~CTestBox()
{
}
void CTestBox::Init() 
{

	m_Renderer = ACTOR->CreateCom<CFixRenderer>(RENDATA(LAYER_MOUSE, 1.0f, true));
	m_TestSkillIcon = COMRESMGR->FindMultiSprite(_T("SkillMainIcon"));
	
	ACTOR->SetPos(Vec3(0.0f, 0.0f, 10.0f));


	m_Ani = ACTOR->CreateCom<CAniRenderer>(RENDATA(LAYER_BACKEFFECT, 1.0f, true));
	ANIDATA AniData1(_T("SkillMainIcon"), _T("SkillMainIcon"));
	AniData1.SettingFullRage({ 0,97 }, 0.1f);
	m_Ani->CreateAnimation(AniData1);
	m_Ani->ChangeAni(_T("SkillMainIcon"));
	ACTOR->SetImageScale(0.25f);



	
	//ACTOR->SetPos(Vec3(0.0f, 0.0f, 10.0f));
	//
	//
	//
	//m_Ani = ACTOR->CreateCom<CAniRenderer>(RENDATA(LAYER_BACKEFFECT, 1.0f, true));
	//
	//ANIDATA AniData1(_T("SkillMainIcon"), _T("SkillMainIcon"));
	//AniData1.SettingFullRage({ 0,97 },0.1f);
	//m_Ani->CreateAnimation(AniData1);
	//m_Ani->ChangeAni(_T("SkillMainIcon"));
	//ACTOR->SetImageScale(0.25f);








	//ANIDATA AniData2(_T("HitIcon"), _T("HitIcon"));
	//AniData2.SettingFullRage({ 0,4 }, 0.2f);
	//m_Ani->CreateAnimation(AniData2);
	//ANIDATA AniData3(_T("ShieldIcon"), _T("ShieldIcon"));
	//AniData3.SettingFullRage({ 0,4 }, 0.2f);
	//m_Ani->CreateAnimation(AniData3);
	//ANIDATA AniData4(_T("SkillIcon"), _T("SkillIcon"));
	//AniData4.SettingFullRage({ 0,4 }, 0.2f);
	//m_Ani->CreateAnimation(AniData4);


	m_Ani->ChangeAni(_T("SkillMainIcon"));
	
	
	ACTOR->SetImageScale(0.25f);
	

}
void CTestBox::Update() 
{
	/*int a = m_Ani->GetCurAniFrame();


	if (a > 10 && a < 18)
	{
		m_SceneMainCamera->BackOut();
	}
	else if(a >= 18)
	{
		m_SceneMainCamera->BackOutOff();
	}
	
	*/

}
void CTestBox::FontRender()
{
	/*COMDEBUGMGR->RectRender(D3DCOLOR_ARGB(255, 255, 255, 255), { 0.0f,0.0f,0.0f }, { 200.0f,200.0f,200.0f });
	COMDEBUGMGR->RectRender(D3DCOLOR_ARGB(255, 255, 255, 255), { 100.0f,0.0f,0.0f }, { 2.0f,2.0f,2.0f });
*/

	TCHAR FPS[MAXKEYLENGTH];

	_stprintf_s(FPS, _T("%f"), CTimerMgr::FPS());

	if (CKeyMgr::IsUpdate()) 
	{
		//COMRESMGR->DrawFont(_T("Å«±Ã¼­"), FPS, {300.0f,300.0f,10.0f}, 1.3f, D3DCOLOR_ARGB(255, 255, 255, 255));
		COMRESMGR->DrawFont(_T("Å«±Ã¼­"), _T("On"), Vec2{-400.0f,300.0f}, 1.3f, D3DCOLOR_ARGB(255, 255, 255, 255));
	}
	else 
	{
		//COMRESMGR->DrawFont(_T("Å«±Ã¼­"), FPS, { 300.0f,300.0f,10.0f }, 1.3f, D3DCOLOR_ARGB(255, 255, 255, 255));
		COMRESMGR->DrawFont(_T("Å«±Ã¼­"), _T("Off"), Vec2{ -400.0f,300.0f }, 1.3f, D3DCOLOR_ARGB(255, 255, 255, 255));
	}

}
void CTestBox::PreUpdate()
{

}
void CTestBox::TraceCamDir()
{
	
	Vec3 CamEye = ACTOR->GetScene()->GetMainPersCamCom()->GetEye();
	Vec3 Nor = TRANS->AxisN();

	CamEye -= ACTOR->GetPos();
	D3DXVec3Normalize(&CamEye, &CamEye);

	float _Rad = 0.0f;
	Vec3 Axis;

	_Rad = D3DXVec3Dot(&Nor, &CamEye);
	_Rad = acosf(_Rad);
	D3DXVec3Cross(&Axis, &Nor, &CamEye);

	D3DXVec3Normalize(&Axis, &Axis);
	Quat RotQuat;
	CSJMath::ConversionQuaternion(&RotQuat, Axis, _Rad);

	TRANS->AxisRot(RotQuat);
	
}