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
};

struct PIX_OUTPUT
{
    float4 vColor				 : SV_Target;
};

cbuffer ForwardLightBuffer : register(b0)
{
	LIGHTDATA					gArrLightData[128];
	int							gLightCount;
	int							Temp1;
	int							Temp2;
    int                         Temp3;
}

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

Texture2D Tex_Diff : register(t0);
Texture2D Tex_Bump : register(t1);
Texture2D Tex_Spec : register(t2);
Texture2D Tex_Emiv : register(t3);
Texture2D Tex_Cam : register(t4);

SamplerState Sampler : register(s0);

PIX_OUTPUT PS_Main(VTX_OUTPUT _Input)
{
    PIX_OUTPUT OutData = (PIX_OUTPUT) 0.f;
 
	float4 FinalColor = Tex_Diff.Sample(Sampler, _Input.vTexCoord);
	float3 BumpNormal = GetBumpNormal(Tex_Bump.Sample(Sampler, _Input.vTexCoord)
		, _Input.vTangent
		, _Input.vBiNormal
		, _Input.vNormal);

    LIGHTCOLOR FinalLC = (LIGHTCOLOR) 0.f;
	for (int i = 0; i < gLightCount; ++i)
	{ 
        LIGHTCOLOR LC = GetLight(_Input.vViewPos.xyz, BumpNormal.xyz, gArrLightData[i]);
        FinalLC.Diffuse = LC.Diffuse;
        FinalLC.Specular = LC.Specular;
        FinalLC.Ambient = LC.Ambient;
	}

    FinalLC.Ambient /= gLightCount;
    OutData.vColor = FinalColor * FinalLC.Diffuse + FinalLC.Specular + FinalLC.Ambient;

    if (0.f == FinalColor.a)
    {
        clip(-1);
    }
    OutData.vColor.a = 1.f;
    return OutData;
}