#include "Precom.h"
#include "CharacterParameter.h"


CCharacterParameter::CCharacterParameter()
	:m_fRadius(30.0f)
{
	m_vecParameterBack.clear();
	
	for (size_t i = 0; i < MAXPARAMETER; i++)
		m_fCurParameter[i] = 3.0f;
	
}


CCharacterParameter::~CCharacterParameter()
{
	m_vecParameterBack.clear();
}
void CCharacterParameter::Init() 
{
	for (size_t i = 0; i < 5; ++i)
	{
		m_vecParameterBack.push_back(ACTOR->CreateCom<CFixRenderer>(RENDATA(LAYER_UIBACK, 0.74f - 0.06f*i, true)));
		m_vecParameterBack[i]->SetSprite(_T("ParameterBack"));
		m_vecParameterBack[i]->SetCustomSize({ 300.f - 60.f*i,300.0f - 60.f*i });
	}

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
}
void CCharacterParameter::Update() 
{

}
void CCharacterParameter::UIFontRender()
{
	float R = 165.0f;
	Vec3 MiddlePoint = TRANS->GetPos();
	Vec2 MidPos = Vec2{ MiddlePoint.x,MiddlePoint.y };

	for (size_t i = 0; i < m_vecPoint.size(); i++)
	{
		Vec3 FontDrawPos = MiddlePoint + Vec3{ 10.0f,-5.0f,0.0f } +Vec3{ R*m_vecPoint[i].x,R*m_vecPoint[i].y,90.0f };
		COMRESMGR->DrawFont(_T("±¼¸²"), m_vecParameterName[i], FontDrawPos, 1.5f, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));
	}



	for (int i = 0; i < MAXPARAMETER; i++)
	{
		int TriIndex1, TriIndex2;

		TriIndex1 = (i + 1) % MAXPARAMETER;
		TriIndex2 = i % MAXPARAMETER;

		Vec2 TriPos1 = MidPos +(m_fRadius * m_fCurParameter[TriIndex1] * m_vecPoint[TriIndex1]);
		Vec2 TriPos2 = MidPos + (m_fRadius * m_fCurParameter[TriIndex2] * m_vecPoint[TriIndex2]);


		COMRESMGR->DrawTriangle(MidPos, TriPos1, TriPos2, D3DXCOLOR(0.2f, 0.2f, 1.0f, 0.7f));

	}
}
void CCharacterParameter::SetChangeParameter(const CharacterIndex& _Index)
{
	if (_Index >= MAXBATTLECHARACTER)
		return;

	tstring CharName = CClientGlobal::CharacterName[_Index].UnitName;

	STATPARAMETERDATA Para = CClientGlobal::CharacterParameter[CharName];

	m_fCurParameter[0] = Para.HpPara;
	m_fCurParameter[1] = Para.MpPara;
	m_fCurParameter[2] = Para.StrPara;
	m_fCurParameter[3] = Para.DexPara;
	m_fCurParameter[4] = Para.IntPara;
	m_fCurParameter[5] = Para.LuckPara;
	m_fCurParameter[6] = Para.DamagePara;
	m_fCurParameter[7] = Para.DefPara;

}
void CCharacterParameter::SetPos(const Vec3& _Pos)
{
	TRANS->SetPos(_Pos);
}
void CCharacterParameter::ParameterOn() 
{
	ACTOR->On();
}
void CCharacterParameter::ParameterOff()
{
	ACTOR->Off();
}