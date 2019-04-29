#include "GlobalValue.fx"
#include "LightGlobal.fx"

//cbuffer LightBuffer : register(b0)
//{
//    LIGHTDATA                   gLightData;
//}

//struct VTXCOL_INPUT 
//{
//	  float3	    vPos : POSITION;
//    float3      vNormal : POSITION;
//    float2      vTex : TEXCOORD;
//};

//struct VTXCOL_OUTPUT 
//{
//	float4	vPos : SV_POSITION;
//    float2  vTex : TEXCOORD;
//};

//struct PS_OUTPUT 
//{
//    float4 DiffFactor : SV_Target0;
//    float4 SpecColor : SV_Target1;
//};

//Texture2DArray PositionTex : register(t0);
//Texture2DArray NormalTex : register(t1);
//Texture2DArray DepthTex : register(t2);

//SamplerState TexSampler : register(s0);

//VTXCOL_OUTPUT VS_Main(VTXCOL_INPUT  _Input) 
//{
//	VTXCOL_OUTPUT OutData = (VTXCOL_OUTPUT)0.f;

//    OutData.vPos = float4(_Input.vPos * float3(2.f, 2.f, 1.f), 1.f);
//    OutData.vTex = _Input.vTex;

//    return OutData;
//}


//PS_OUTPUT PS_Main(VTXCOL_OUTPUT _Input)
//{
//    PS_OUTPUT OutData = (PS_OUTPUT) 0.f;
    
//    float4 ViewPos = PositionTex.Sample(TexSampler, float3(_Input.vTex, 0.f));
//    float4 ViewNormal = NormalTex.Sample(TexSampler, float3(_Input.vTex, 0.f));
   
//    LIGHTCOLOR LC = GetLight(ViewPos.xyz, ViewNormal.xyz, gLightData);

//    OutData.DiffFactor = LC.Diffuse + LC.Ambient;
//    OutData.SpecColor = LC.Specular;
//    OutData.SpecColor.w = 1.f;

//    return OutData;
//}

cbuffer LightBuffer : register(b0)
{
    LIGHTDATA gLightData;
}

struct VTXCOL_INPUT
{
    float3 vPos : POSITION;
    float3 vNormal : POSITION;
    float2 vTex : TEXCOORD;
};

struct VTXCOL_OUTPUT
{
    float4 vPos : SV_POSITION;
    float2 vTex : TEXCOORD;
};

struct PS_OUTPUT
{
    float4 DiffFactor : SV_Target0;
    float4 SpecColor : SV_Target1;
    float4 EmvFactor : SV_Target2;
};

Texture2D PositionTex : register(t0);
Texture2D NormalTex : register(t1);
Texture2D DepthTex : register(t2);
Texture2D SpecularTex : register(t3);
Texture2D EmissiveTex : register(t4);

SamplerState TexSampler : register(s0);

VTXCOL_OUTPUT VS_Main(VTXCOL_INPUT _Input)
{
    VTXCOL_OUTPUT OutData = (VTXCOL_OUTPUT) 0.f;

    OutData.vPos = float4(_Input.vPos * float3(2.f, 2.f, 1.f), 1.f);
    OutData.vTex = _Input.vTex;

    return OutData;
}


PS_OUTPUT PS_Main(VTXCOL_OUTPUT _Input)
{
    PS_OUTPUT OutData = (PS_OUTPUT) 0.f;
    
    float4 ViewPos = PositionTex.Sample(TexSampler, _Input.vTex);
    float4 ViewNormal = NormalTex.Sample(TexSampler, _Input.vTex);
    float4 SpecularColor = SpecularTex.Sample(TexSampler, _Input.vTex);
    float4 EmissiveColor = EmissiveTex.Sample(TexSampler, _Input.vTex);

    LIGHTCOLOR LC = GetLight(ViewPos.xyz, ViewNormal.xyz, gLightData);

    OutData.DiffFactor = LC.Diffuse + LC.Ambient * 5.f;
    OutData.SpecColor = LC.Specular * SpecularColor;
    OutData.SpecColor.w = 1.f;

    OutData.EmvFactor = SpecularColor * float4(3.f, 3.f, 3.f, 1.f);
    OutData.EmvFactor.w = 1.f;

    return OutData;
}
