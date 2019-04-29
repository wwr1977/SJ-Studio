
cbuffer LightBuffer : register(b0)
{
    float4 gCamPos;
}

struct VTXCOL_INPUT 
{
    float3      vPos : POSITION;
    float3      vNormal : NORMAL;
    float2      vTex : TEXCOORD;
};

struct VTXCOL_OUTPUT 
{
	float4	vPos : SV_POSITION;
    float2  vTex : TEXCOORD;
};

struct PS_OUTPUT 
{
    float4 vColor : SV_Target0;
};

Texture2D DiffColorTex : register(t0);
Texture2D DiffLightTex : register(t1);
Texture2D SpecLightTex : register(t2);
Texture2D EmissiveColorTex : register(t3);
Texture2D EmissiveFactor : register(t4);

SamplerState TexSampler : register(s0);

VTXCOL_OUTPUT VS_Main(VTXCOL_INPUT  _Input) 
{
	VTXCOL_OUTPUT OutData = (VTXCOL_OUTPUT)0.f;

    OutData.vPos = float4(_Input.vPos * float3(2.f, 2.f, 1.f), 1.f);
    OutData.vTex = _Input.vTex;

    return OutData;
}

PS_OUTPUT PS_Main(VTXCOL_OUTPUT _Input)
{
    PS_OUTPUT OutData = (PS_OUTPUT) 0.f;

    float4 Diff = DiffLightTex.Sample(TexSampler, _Input.vTex);
    float4 Spec = SpecLightTex.Sample(TexSampler, _Input.vTex);
    float4 EmisFactor = EmissiveFactor.Sample(TexSampler, _Input.vTex);
    float4 EmisColor = EmissiveColorTex.Sample(TexSampler, _Input.vTex);
    float Alpha = EmisColor.a;

    OutData.vColor = DiffColorTex.Sample(TexSampler, _Input.vTex);
    OutData.vColor = OutData.vColor * Diff + Spec + EmisFactor;

    if (Alpha > 0.f)
    {
        //OutData.vColor = lerp(OutData.vColor, EmisColor, EmisColor.a);
        OutData.vColor = (OutData.vColor * (1.f - Alpha)) + (EmisColor * Alpha);
    }

    OutData.vColor.a = 1.f;

    return OutData;
}