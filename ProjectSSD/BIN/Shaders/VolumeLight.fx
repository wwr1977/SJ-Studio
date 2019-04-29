#include "GlobalValue.fx"

struct VTX_OUTPUT
{
    float4 vPos : SV_POSITION;
};

VTX_OUTPUT VS_Main(float3 _Pos : POSITION)
{
    VTX_OUTPUT OutData = (VTX_OUTPUT) 0.f;
   
    OutData.vPos = mul(float4(_Pos, 1.f), gWVP);
    return OutData;
}

float4 PS_Main(VTX_OUTPUT _Input) : SV_Target
{
    float4 OutData = (float4) 0.f;
    return OutData;
}