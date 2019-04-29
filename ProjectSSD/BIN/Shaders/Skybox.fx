#include "GlobalValue.fx"

struct VTX_INPUT
{
    float3 vPos : POSITION;
    float3 vNormal : NORMAL;
    float2 vTexCoord : TEXCOORD;
};

struct VTX_OUTPUT
{
    float4 vPos : SV_POSITION;
    float2 vTexCoord : TEXCOORD;
    float4 vColor : COLOR;
};

struct PIX_OUTPUT
{
    float4 vTexColor : SV_Target;
};

VTX_OUTPUT VS_Main(VTX_INPUT _Input)
{
    VTX_OUTPUT OutData = (VTX_OUTPUT) 0.f;

    float3 ViewPos = mul(mul(_Input.vPos, (float3x3) gWorld), (float3x3) gView);

    OutData.vPos = mul(float4(ViewPos, 1.f), gProj);
    OutData.vTexCoord = _Input.vTexCoord;

    return OutData;
}

Texture2D Image : register(t0);
SamplerState Sampler : register(s0);

PIX_OUTPUT PS_Main(VTX_OUTPUT _Input)
{
    PIX_OUTPUT OutData = (PIX_OUTPUT) 0.f;
    
    OutData.vTexColor = Image.Sample(Sampler, _Input.vTexCoord);

    if (OutData.vTexColor.a == 0.f)
        clip(-1);

    return OutData;
}