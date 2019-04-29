#include "stdafx.h"
#include "Light.h"


CLight::CLight()
{
}


CLight::~CLight()
{
}
void CLight::Init() 
{

}
void CLight::Update() 
{

}
void CLight::SetDirectionLight()
{
	m_Light.Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f); // 환경광 -> 주변환경.
	m_Light.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f); // 난반사 -> 자기 자신의 색깔.
	m_Light.Specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f); // 정반사 -> 빛이 반사될때의 색깔.
	m_Light.Type = D3DLIGHT_DIRECTIONAL;
	m_Light.Direction = Vec3(0.0f, 0.0f, 1.0f);
	
	COMDEV->SetLight(0, &m_Light);
}
void CLight::SetAmbient(D3DXCOLOR _AmbColor) 
{
	m_Light.Ambient = _AmbColor;
}
void CLight::SetDiffuse(D3DXCOLOR _DifColor)
{
	m_Light.Diffuse = _DifColor;
}
void CLight::SetSpecular(D3DXCOLOR _SpecColor) 
{
	m_Light.Specular = _SpecColor;
}
void CLight::SetLight()
{
	COMDEV->SetLight(0, &m_Light);
}