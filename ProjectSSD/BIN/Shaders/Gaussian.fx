#include "GaussianGlobal.fx"

#define MAXMAPSIZE 256
#define MAXRANGE 16


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
    float2              gWindowSize;
    int                 gRange;
    float               gTotalGaussianMap;

    float4              gGaussianMap[64];
};

Texture2D g_Tex_0 : register(t0);
SamplerState g_Smp_0 : register(s0);

PS_OUTPUT PS_Main(VTXCOL_OUTPUT _in)
{
	PS_OUTPUT outData = (PS_OUTPUT)0.0f;

    float PixelU = 1.0f / gWindowSize.x;
    float PixelV = 1.0f / gWindowSize.y;

    int StartRow = -(gRange / 2);
    int StartColm = (gRange / 2);
    
    for (int Row = 0; Row < gRange; ++Row)
    {
        for (int Colm = 0; Colm < gRange; ++Colm)
        {
            int Index = Row * MAXRANGE + Colm;
            float Gau = gGaussianMap[Index / 4][Index % 4];
            float2 vUv = _in.vUv + float2((StartRow + Row) * PixelV, (StartColm - Colm) * PixelU);
            outData.vCol += g_Tex_0.Sample(g_Smp_0, vUv) * Gau;
        }
    }
    outData.vCol /= gTotalGaussianMap;
    outData.vCol.w = 1.f;
    return outData;
}