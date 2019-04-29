#include "GaussianGlobal.fx"
#include "GlobalValue.fx"

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

Texture2D MinimizeTarget : register(t0);
Texture2D BaseCamTarget : register(t1);
SamplerState g_Smp_0 : register(s0);

cbuffer BloomBuffer : register(b0)
{
    float       gTargetSize;

    float       None1;
    float       None2;
    float       None3;

    float2      gWindowSize;
    int         gRange;
    float       gTotalGaussianMap;

    float4      gGaussianMap[64];
};

PS_OUTPUT PS_Main(VTXCOL_OUTPUT _in)
{
	//PS_OUTPUT outData = (PS_OUTPUT)0.0f;
    
 //   float4 Lumi = MinimizeTarget.Sample(g_Smp_0, float3(_in.vUv.x, _in.vUv.y, 0.f));
 
 //   float PixelU = 1.f / gTargetSize;
 //   float PixelV = 1.f / gTargetSize;

 //   int StartX = -2;
 //   int StartY = -2;

 //   for (int Row = 0; Row < 5; ++Row)
 //   {
 //       for (int Colm = 0; Colm < 5; ++Colm)
 //       {
 //           float2 vUv = _in.vUv + float2((StartX + Colm) * PixelU, (StartY + Row) * PixelV);
 //           Lumi += MinimizeTarget.Sample(g_Smp_0, float3(vUv.x, vUv.y, 0.0f)) * g_GaussianArray[Row * 5 + Colm]* 0.01f;
 //       }
 //   } 

 //   Lumi /= 26.f;

 //   outData.vCol = BaseCamTarget.Sample(g_Smp_0, float3(_in.vUv.x, _in.vUv.y, 0.f));
 //   outData.vCol +=  Lumi;

    PS_OUTPUT outData = (PS_OUTPUT) 0.0f;
    
    float PixelU = 1.0f / gWindowSize.x;
    float PixelV = 1.0f / gWindowSize.y;

    int StartRow = -(gRange / 2);
    int StartColm = (gRange / 2);
    
    float4 Lumi = float4(0.f, 0.f, 0.f, 0.f);

    for (int Row = 0; Row < gRange; ++Row)
    {
        for (int Colm = 0; Colm < gRange; ++Colm)
        {
            int Index = Row * MAXRANGE + Colm;
            float Gau = gGaussianMap[Index / 4][Index % 4];
            float2 vUv = _in.vUv + float2((StartRow + Row) * PixelV, (StartColm - Colm) * PixelU);
            Lumi += MinimizeTarget.Sample(g_Smp_0, vUv) * Gau;
        }
    }
    outData.vCol /= gTotalGaussianMap;

    outData.vCol = BaseCamTarget.Sample(g_Smp_0, _in.vUv);
    outData.vCol += Lumi;
    outData.vCol.w = 1.f;

    return outData;
}