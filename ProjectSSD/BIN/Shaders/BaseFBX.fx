#include "GlobalValue.fx"
#include "LightGlobal.fx"

struct VTX_INPUT 
{
	float3 vPos                  : POSITION;
	float2 vTexCoord             : TEXCOORD;
    float4 vColor                : COLOR;

	float3 vNormal               : NORMAL;
    float3 vBiNormal             : BINORMAL;
    float3 vTangent              : TANGENT;
};

struct VTX_OUTPUT
{
    float4 vPos                  : SV_POSITION;
    float2 vTexCoord             : TEXCOORD;

    float3 vNormal               : NORMAL;
    float3 vBiNormal             : BINORMAL;
    float3 vTangent              : TANGENT;

    float3 vViewPos              : TEXCOORD1;
    float3 vProjPos              : TEXCOORD2;
};

struct PIX_OUTPUT
{
    float4 DiffuseColor         : SV_Target0;
    float4 Position             : SV_Target1;
    float4 Normal               : SV_Target2;
    float4 Depth                : SV_Target3;
    float4 Specular             : SV_Target4;
    float4 Emissive             : SV_Target5;
};

cbuffer MaterialColor : register(b0)
{
    float4                      DiffuseColor;
    float4                      SpecularColor;
    float4                      AmbientColor;
    float4                      EmissiveColor;
}

Texture2D Tex_Diff : register(t0);
Texture2D Tex_Bump : register(t1);
Texture2D Tex_Spec : register(t2);
Texture2D Tex_Emiv : register(t3);

SamplerState Sampler : register(s0);

VTX_OUTPUT VS_Main(VTX_INPUT _Input)
{
    VTX_OUTPUT OutData = (VTX_OUTPUT) 0.f;

    OutData.vPos = mul(float4(_Input.vPos, 1.f), gWVP);
    OutData.vTexCoord = _Input.vTexCoord;

    OutData.vNormal = normalize(mul(_Input.vNormal, (float3x3) gNWV));
    OutData.vBiNormal = normalize(mul(_Input.vBiNormal, (float3x3) gNWV));
    OutData.vTangent = normalize(mul(_Input.vTangent, (float3x3) gNWV));
    OutData.vViewPos = mul(float4(_Input.vPos, 1.f), gWV).xyz;

    return OutData;
}

PIX_OUTPUT PS_Main(VTX_OUTPUT _Input)
{
    PIX_OUTPUT OutData = (PIX_OUTPUT) 0.f;
 
    float3 BumpNormal = GetBumpNormal(Tex_Bump.Sample(Sampler, _Input.vTexCoord)
                                    , _Input.vTangent
                                    , _Input.vBiNormal
                                    , _Input.vNormal);

    OutData.DiffuseColor = Tex_Diff.Sample(Sampler, _Input.vTexCoord);
    OutData.Position = float4(_Input.vViewPos, 1.f);
    OutData.Normal = float4(BumpNormal, 1.f);
    OutData.Depth = float4(_Input.vViewPos.z, _Input.vPos.z, _Input.vViewPos.z / Far, 1.f);
    OutData.Specular = Tex_Spec.Sample(Sampler, _Input.vTexCoord);
    //OutData.Emissive = EmissiveColor;
    //OutData.Emissive.w = 1.f;

    return OutData;
}