#include "GlobalValue.fx"

struct VTXCOL_INPUT
{
	float4 vPos : POSITION;
	float2 vUv : TEXCOORD;
};


struct VTXCOL_OUTPUT
{
	float4 vPos : SV_POSITION;
	float2 vUv : TEXCOORD;
};


struct PS_OUTPUT
{
	float4 vCol : SV_Target;
};


VTXCOL_OUTPUT VS_Main(VTXCOL_INPUT _in)
{
	VTXCOL_OUTPUT outData = (VTXCOL_OUTPUT)0.0f;

    outData.vPos = _in.vPos;
	outData.vUv = _in.vUv;
	return outData;
}

cbuffer GaussianBuffer : register(b0)
{
    float3          FogColor;
    float           FogStartDepth;
    float3          FogHightlightColor;
    float           FogGlobalDensity;
    float3          FogSunDir;
    float           FogHeightFallOff;
};

Texture2D g_Tex_0 : register(t0);
Texture2D g_PosTex : register(t1);
Texture2D g_DepthTex : register(t2);
SamplerState g_Smp_0 : register(s0);

float3 ApplyFog(float3 _OriginalColor, float _EyePosY, float3 _EyetoPixel)
{
    float PixelDist = length(_EyetoPixel);
    //////float3 EyetoPixelNormal = _EyetoPixel / PixelDist;
    //float3 EyetoPixelNormal = normalize(_EyetoPixel);

    //////픽셀 거리에 대한 안개 시작 지점 계산
    //float FogDist = max(PixelDist - FogStartDepth, 0.f);

    //////안개 세기에 대한 거리 계산
    //float FogHeightDensityAtViewer = exp(-FogHeightFallOff * _EyePosY);
    //float FogDistInt = FogDist * FogHeightDensityAtViewer;

    ////안개 세기에 대한 높이 계산
    //float EyeToPixelY = _EyetoPixel.y * (FogDist / PixelDist);
    //float t = FogHeightFallOff * EyeToPixelY;
    //const float ThresHoldT = 0.01f;
    //float FogHeightInt = abs(t) > ThresHoldT ? (1.0 - exp(-t)) / t : 1.f;

    //float FogFinalFactor = exp(-FogGlobalDensity * FogDistInt * FogHeightInt);

    ////태양 하이라이트 계산 및 안개 색생 혼합
    //float SunHighlightFactor = saturate(dot(EyetoPixelNormal, FogSunDir));
    //SunHighlightFactor = pow(SunHighlightFactor, 8.f);
    //float3 FogFinalColor = lerp(FogColor, FogHightlightColor, SunHighlightFactor);

    //return lerp(FogFinalColor, _OriginalColor, FogFinalFactor);

    if (PixelDist < FogStartDepth)
    {
        return _OriginalColor;
    }
    float FogFactor = (PixelDist - FogStartDepth) * 0.0001f;
    float FinalFactor = min(FogFactor, 0.3f); 
    return (float3(0.8f, 0.8f, 0.8f) * FinalFactor) + _OriginalColor * (1.f - FinalFactor);
}

PS_OUTPUT PS_Main(VTXCOL_OUTPUT _in)
{
	PS_OUTPUT outData = (PS_OUTPUT)0.0f;
    float4 OriColor = g_Tex_0.Sample(g_Smp_0, _in.vUv);
    float3 Pos = g_PosTex.Sample(g_Smp_0, _in.vUv);
    float3 Depth = g_DepthTex.Sample(g_Smp_0, _in.vUv);

    float3 EyetoPix = Pos/* - gCamWorldPos.xyz*/;
    float3 FinalColor = ApplyFog(OriColor.xyz, Pos.y, EyetoPix);
    outData.vCol = float4(FinalColor, 1.f);

    return outData;
}