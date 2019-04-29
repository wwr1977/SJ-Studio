#include "stdafx.h"
#include "ParameterRen.h"


CParameterRen::CParameterRen()
	:m_fRadius(40.0f), m_MiddlePos(Vec2{0.0f,0.0f})
{
}


CParameterRen::~CParameterRen()
{
}
void CParameterRen::Init() 
{
	m_vecPoint.push_back(Vec2{ 1.0f,0.0f });
	m_vecPoint.push_back(Vec2{ ROOTTWO * 0.5f,ROOTTWO *0.5f });
	m_vecPoint.push_back(Vec2{ 0.0f,1.0f });
	m_vecPoint.push_back(Vec2{ ROOTTWO * -0.5f,ROOTTWO *0.5f });
	m_vecPoint.push_back(Vec2{ -1.0f,0.0f });
	m_vecPoint.push_back(Vec2{ ROOTTWO * -0.5f,ROOTTWO *-0.5f });
	m_vecPoint.push_back(Vec2{ 0.0f,-1.0f });
	m_vecPoint.push_back(Vec2{ ROOTTWO * 0.5f,ROOTTWO *-0.5f });

	m_vecParameterName.push_back(_T("HP"));
	m_vecParameterName.push_back(_T("MP"));
	m_vecParameterName.push_back(_T("Str"));
	m_vecParameterName.push_back(_T("Dex"));
	m_vecParameterName.push_back(_T("Int"));
	m_vecParameterName.push_back(_T("Luk"));
	m_vecParameterName.push_back(_T("Demage"));
	m_vecParameterName.push_back(_T("Def"));

	memset(&m_fCurParameter, 0, sizeof(float) * MAXPARAMETER);

	for (size_t i = 0; i < MAXPARAMETER; i++)
	{
		m_fCurParameter[i] = 1.0f;
	}

	SPTR<CFixRenderer>	ParaBack = ACTOR->CreateCom<CFixRenderer>(RENDATA(LAYER_UIBACK,0.5f,true));
	ParaBack->SetSprite(_T("ParameterBack"));
	ParaBack->SetCustomSize({ 400.0f,400.0f });

	ParaBack = ACTOR->CreateCom<CFixRenderer>(RENDATA(LAYER_UIBACK, 0.5f, true));
	ParaBack->SetSprite(_T("ParameterBack"));
	ParaBack->SetCustomSize({ 320.0f,320.0f });

	ParaBack = ACTOR->CreateCom<CFixRenderer>(RENDATA(LAYER_UIBACK, 0.5f, true));
	ParaBack->SetSprite(_T("ParameterBack"));
	ParaBack->SetCustomSize({ 240.0f,240.0f });

	ParaBack = ACTOR->CreateCom<CFixRenderer>(RENDATA(LAYER_UIBACK, 0.5f, true));
	ParaBack->SetSprite(_T("ParameterBack"));
	ParaBack->SetCustomSize({ 160.0f,160.0f });

	ParaBack = ACTOR->CreateCom<CFixRenderer>(RENDATA(LAYER_UIBACK, 0.5f, true));
	ParaBack->SetSprite(_T("ParameterBack"));
	ParaBack->SetCustomSize({ 80.0f,80.0f });

	POINT WndSize = CEditGlobal::MainWindow->WndSize();
	Vec3 BackPos = Vec3{ WndSize.x*0.25f,WndSize.y*0.2f,100.0f };
	m_MiddlePos = Vec2{ WndSize.x*0.25f,WndSize.y*0.2f };

	ACTOR->SetPos(BackPos);

	CEditGlobal::SParameter = this;
}
void CParameterRen::Update() 
{

}
void CParameterRen::FontRender()
{
	float R = 220.0f;
	Vec3 MiddlePoint = TRANS->GetPos();

	for (size_t i = 0; i < m_vecPoint.size(); i++)
	{
		Vec3 FontDrawPos = MiddlePoint + Vec3{ 10.0f,-5.0f,0.0f } + Vec3{ R*m_vecPoint[i].x,R*m_vecPoint[i].y,90.0f };
		COMRESMGR->DrawFont(_T("µ¸¿ò"), m_vecParameterName[i], FontDrawPos, 1.5f ,D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));
	}



	for (int i = 0; i < MAXPARAMETER; i++)
	{
		int TriIndex1, TriIndex2;

		TriIndex1 = (i + 1) % MAXPARAMETER;
		TriIndex2 = i % MAXPARAMETER;

		Vec2 TriPos1 = m_MiddlePos + (m_fRadius * m_fCurParameter[TriIndex1] * m_vecPoint[TriIndex1]);
		Vec2 TriPos2 = m_MiddlePos + (m_fRadius * m_fCurParameter[TriIndex2] * m_vecPoint[TriIndex2]);


		COMRESMGR->DrawTriangle(m_MiddlePos, TriPos1 , TriPos2, D3DXCOLOR(0.2f,0.2f,1.0f,0.7f));

	}
	

}
void CParameterRen::InitParameterValue(const STATPARAMETERDATA& _InitData /*= STATPARAMETERDATA()*/)
{
	if (MAXMAINSTAT == _InitData.MainStat)
	{
		for (int i = 0; i < MAXPARAMETER; i++)
		{
			m_fCurParameter[i] = 1.0f;
		}
	}
	else 
	{
		m_fCurParameter[0] = _InitData.HpPara;
		m_fCurParameter[1] = _InitData.MpPara;
		m_fCurParameter[2] = _InitData.StrPara;
		m_fCurParameter[3] = _InitData.DexPara;
		m_fCurParameter[4] = _InitData.IntPara;
		m_fCurParameter[5] = _InitData.LuckPara;
		m_fCurParameter[6] = _InitData.DamagePara;
		m_fCurParameter[7] = _InitData.DefPara;

	}

}
void CParameterRen::SetParameterValue(const int& _Index, const float& _Value)
{
	if (_Index >= MAXPARAMETER) 
	{
		return;
	}

	m_fCurParameter[_Index] = _Value;
}
float* CParameterRen::GetParameterArr()
{
	return m_fCurParameter;
}