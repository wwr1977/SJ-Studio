#include "GlobalValue.fx"


cbuffer GizmoBuffer : register(b0)
{
    float4x4 gGizmoWVP;
}
struct VTX_INPUT
{
    float4 vPos         : POSITION;
    float4 vColor       : COLOR;
    float2 vTexCoord    : TEXCOORD;

};

struct VTX_OUTPUT
{
    float4 vPos         : SV_Position;
    float4 vColor       : COLOR;
    float2 vTexCoord       : TEXCOORD;

};

struct PIX_OUTPUT
{
    float4 vTexColor : SV_Target;
};

Texture2D Image : register(t0);
SamplerState Sampler : register(s0);


VTX_OUTPUT VS_Main(VTX_INPUT _Input)
{
    VTX_OUTPUT OutData = (VTX_OUTPUT) 0.f;

    OutData.vPos = mul(_Input.vPos, gGizmoWVP);
    OutData.vColor = _Input.vColor;
    OutData.vTexCoord = _Input.vTexCoord;
   
    return OutData;
}


PIX_OUTPUT PS_Main(VTX_OUTPUT _Input)
{
    PIX_OUTPUT OutData = (PIX_OUTPUT) 0.f;

    OutData.vTexColor = _Input.vColor * gColorFactor;

    if (OutData.vTexColor.a <= 0)
        clip(-1);

    return OutData;
}
