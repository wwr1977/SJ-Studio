#include "GlobalValue.fx"
#define CLEARCOLOR float3(1.f ,0.f ,1.f)

struct VTX_INPUT 
{
    float3      vPos : POSITION;
    float3      vNormal : NORMAL;
    float2      vTex : TEXCOORD;
};

struct VTX_OUTPUT 
{
	float4	vPos : SV_POSITION;
    float2  vTex : TEXCOORD;
};

struct PS_OUTPUT 
{
    float4 vColor : SV_Target0;
};

Texture2D CamTargetTex : register(t0);
Texture2D UITargetTex : register(t1);

SamplerState TexSampler : register(s0);

VTX_OUTPUT VS_Main(VTX_INPUT  _Input) 
{
	VTX_OUTPUT OutData = (VTX_OUTPUT)0.f;

    OutData.vPos = float4(_Input.vPos * float3(2.f, 2.f, 1.f), 1.f);
    OutData.vTex = _Input.vTex;

    return OutData;
}

PS_OUTPUT PS_Main(VTX_OUTPUT _Input)
{
    PS_OUTPUT OutData = (PS_OUTPUT) 0.f;

    float4 UIColor = UITargetTex.Sample(TexSampler, _Input.vTex);
    float4 CamColor = CamTargetTex.Sample(TexSampler, _Input.vTex);

    float Alpha = UIColor.a;
    if (UIColor.r == 1.f && UIColor.g == 0.f && UIColor.b == 1.f)
    {
        Alpha = 0.f;
    }

    if (Alpha == 0.f)
    {
        OutData.vColor = CamColor;
    }
    else
    {
        OutData.vColor = (CamColor * (1.f - Alpha)) + (UIColor * Alpha);
    }
    OutData.vColor.a = 1.f;

    return OutData;
}