#include "GlobalValue.fx"
#include "LightGlobal.fx"

struct VTX_INPUT
{
    float3 vPos : POSITION;
    float3 vNormal : NORMAL;
    float2 vTexCoord : TEXCOORD;
};

struct VTX_OUTPUT
{
    float4 vPos : SV_POSITION;
    float4 vProjPos : POSITION;
    float2 vTexCoord : TEXCOORD;
};

struct PIX_OUTPUT
{
    float4 Diffuse  : SV_Target0;
    float4 Normal   : SV_Target1;
};

cbuffer DecalBuffer : register(b0)
{
    float4x4    WorldViewInv;
}

Texture2D ViewPosTex    : register(t0);
Texture2D DiffuseTex    : register(t1);
Texture2D NormalTex     : register(t2);
SamplerState Sampler    : register(s0);

VTX_OUTPUT VS_Main(VTX_INPUT _Input)
{
    VTX_OUTPUT OutData = (VTX_OUTPUT) 0.f;

    OutData.vPos = mul(float4(_Input.vPos, 1.f), gWVP);
    OutData.vTexCoord = _Input.vTexCoord;
    OutData.vProjPos = mul(float4(_Input.vPos, 1.f), gWVP);

    return OutData;
}

PIX_OUTPUT PS_Main(VTX_OUTPUT _Input)
{
    PIX_OUTPUT OutData = (PIX_OUTPUT) 0.f;

    //float2 DepthUV = _Input.vProjPos.xy / _Input.vProjPos.w;
    //DepthUV.x = DepthUV.x * 0.5f + 0.5f;
    //DepthUV.y = DepthUV.y * -0.5f + 0.5f;

    //float3 ViewPos = PosTex.Sample(Sampler, DepthUV).xyz;
    //float4 ObjPos = mul(float4(ViewPos, 1.f), WorldViewInv);

    //float3 Objabs = abs(ObjPos);
    //if (Objabs.x > 0.5f || Objabs.y > 0.5f || Objabs.z > 0.5f)
    //{
    //    clip(-1);
    //}
    //float2 FinalUV = ObjPos.xz + 0.5f;
    //OutData.Color = DiffTex.Sample(Sampler, FinalUV);

    ///// 
    float2 DepthUV = _Input.vProjPos.xy / _Input.vProjPos.w;
    DepthUV.x = DepthUV.x * 0.5f + 0.5f;
    DepthUV.y = DepthUV.y * -0.5f + 0.5f;

    float3 ViewPos = ViewPosTex.Sample(Sampler, DepthUV).xyz;
    float4 ObjPos = mul(float4(ViewPos, 1.f), WorldViewInv);

    float3 Objabs = abs(ObjPos);
    if (Objabs.x > 0.5f || Objabs.y > 0.5f || Objabs.z > 0.5f)
    {
        clip(-1);
    }
    float2 FinalUV = ObjPos.xz + 0.5f;
    OutData.Diffuse = DiffuseTex.Sample(Sampler, FinalUV);
    float3 BumpNormal = GetBumpNormal(NormalTex.Sample(Sampler, _Input.vTexCoord), float3(1.f, 0.f, 0.f), float3(0.f, 0.f, 1.f), float3(0.f, 1.f, 0.f));
    OutData.Normal = float4(BumpNormal, 1.f);

    if (0.f == OutData.Diffuse.a)
    {
        clip(-1);
    }

    //OutData.Color = NormalTex.Sample(Sampler, FinalUV);
    //OutData.Color = float4(1.f, 0.f, 0.f, 1.f);

    return OutData;
}
