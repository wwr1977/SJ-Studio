#include "GlobalValue.fx"

struct VTX_INPUT 
{
	float3 vPos         : POSITION;
	float2 vTexCoord    : TEXCOORD;
    float4 vColor       : COLOR;

	float3 vNormal      : NORMAL;
    float3 vBiNormal    : BINORMAL;
    float3 vTangent     : TANGENT;

    float4 vWeight      : WEIGHT;
    float4 vIndices     : INDICES;
};

struct VTX_OUTPUT
{
    float4 vPos         : SV_POSITION;
    float2 vTexCoord    : TEXCOORD;

    float3 vNormal      : NORMAL;
    float3 vBiNormal    : BINORMAL;
    float3 vTangent     : TANGENT;

    float4 vWeight      : WEIGHT;
    float4 vIndices     : INDICES;
    float3 vViewPos    : TEXCOORD1;
};

struct PIX_OUTPUT
{
    float4 vOutColor : SV_Target;
};


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

    OutData.vWeight = _Input.vWeight;
    OutData.vIndices = _Input.vIndices;
   
    return OutData;
}


PIX_OUTPUT PS_Main(VTX_OUTPUT _Input)
{
    PIX_OUTPUT OutData = (PIX_OUTPUT) 0.f;
  
    OutData.vOutColor = gColorFactor;

    return OutData;
}