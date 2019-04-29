#include "GlobalValue.fx"

struct VTX_INPUT
{
    float4 vPos         : POSITION;
    float2 vTexCoord    : TEXCOORD;
    float4 vColor       : COLOR;

};

struct VTX_OUTPUT
{
	float4 vPos         : SV_POSITION;
    float4 vColor       : NORMAL;

};

struct PIX_OUTPUT
{
	float4 vOutColor : SV_Target;
};

cbuffer NaviAreaBuffer : register(b0)
{
    float4x4 gAreaWVP;
};


VTX_OUTPUT VS_Main(VTX_INPUT  _Input)
{
	VTX_OUTPUT OutData = (VTX_OUTPUT)0.f;
    OutData.vPos = mul(_Input.vPos, gAreaWVP);
    OutData.vColor = _Input.vColor;

	return OutData;
}

PIX_OUTPUT PS_Main(VTX_OUTPUT _Input)
{
	PIX_OUTPUT OutData = (PIX_OUTPUT)0.f;

    OutData.vOutColor = gColorFactor;
   
    if (OutData.vOutColor.a <= 0)
        clip(-1);

    return OutData;
}