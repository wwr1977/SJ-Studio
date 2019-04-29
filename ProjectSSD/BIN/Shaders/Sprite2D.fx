#include "GlobalValue.fx"

cbuffer SpriteUVBuffer : register(b0)
{
    // SpriteUV.xy = UV Pos
    // SpriteUV.zw = UV Interval
    float4 SpriteUV;
}


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

Texture2D Image : register(t0);
SamplerState Sampler : register(s0);


VTX_OUTPUT VS_Main(VTX_INPUT _Input)
{
    VTX_OUTPUT OutData = (VTX_OUTPUT) 0.f;
    OutData.vPos = mul(float4(_Input.vPos, 1.f), gWVP);
    OutData.vNormal = _Input.vNormal;

    OutData.vTexCoord.x = SpriteUV.x + SpriteUV.z * _Input.vTexCoord.x;
    OutData.vTexCoord.y = SpriteUV.y + SpriteUV.w * _Input.vTexCoord.y;

    return OutData;
}

PIX_OUTPUT PS_Main(VTX_OUTPUT _Input)
{
    PIX_OUTPUT OutData = (PIX_OUTPUT) 0.f;
    OutData.vTexColor = Image.Sample(Sampler, _Input.vTexCoord);
    
    if (OutData.vTexColor.a == 0.f)
        clip(-1);
   
    return OutData;
}
