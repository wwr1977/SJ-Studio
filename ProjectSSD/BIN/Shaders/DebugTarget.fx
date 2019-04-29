#include "GlobalValue.fx"



cbuffer DebugBuffer : register(b0)
{
    uint2               gDivisionLevel;
    uint2               gPosition;
};


struct VTX_INPUT
{
    float3 vPos : POSITION;
    float3 vNormal : NORMAL;
    float2 vTexCoord : TEXCOORD;

};

struct VTX_OUTPUT
{
    float4 vPos : SV_POSITION;
    float3 vNormal : NORMAL;
    float2 vTexCoord : TEXCOORD;
};

struct PIX_OUTPUT
{
    float4 vTexColor : SV_Target;
};

Texture2D TargetTex : register(t0);
SamplerState Sampler : register(s0);


VTX_OUTPUT VS_Main(VTX_INPUT _Input)
{
    VTX_OUTPUT OutData = (VTX_OUTPUT) 0.f;
  
    float2 Scale = float2(2.f / (float) gDivisionLevel.x, 2.f / (float) gDivisionLevel.y);
    float2 Trans = float2(Scale.x * (0.5f + (float) gPosition.x) - 1.f, 1.f - ((float) gPosition.y + 0.5f) * Scale.y);

    OutData.vPos.xy = _Input.vPos.xy * Scale + Trans;
    OutData.vPos.zw = float2(0.f, 1.f);

    OutData.vNormal = _Input.vNormal;
    OutData.vTexCoord = _Input.vTexCoord;
    return OutData;
}


PIX_OUTPUT PS_Main(VTX_OUTPUT _Input)
{
    PIX_OUTPUT OutData = (PIX_OUTPUT) 0.f;
    
    OutData.vTexColor = TargetTex.Sample(Sampler, _Input.vTexCoord);
    OutData.vTexColor.a = 1.f;
    return OutData;
}