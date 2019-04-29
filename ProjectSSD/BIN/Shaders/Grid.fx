#include "GlobalValue.fx"

cbuffer GridDataBuffer : register(b0)
{
    int Step;
    float MinRange;
    float MaxRange;
    float Border;
    float MinStep;
    float MaxStep;
    float CalMaxRange;
    float Y;
};

struct VTX_OUTPUT
{
    float4 vPos : SV_POSITION;
    float4 vWorldPos : POSITION;
};

struct PIX_OUTPUT
{
    float4 vTexColor : SV_Target;
};

static matrix X90ROTMAT =
{
    { 100000.0000, 0.000000000, 0.000000000, 0.000000000 },
    { 0.000000000, 0, 100000.0000, 0.000000000 },
    { 0.000000000, -100000.0000, 0, 0.000000000 },
    { 0.000000000, 0.000000000, 0.000000000, 1.00000000 },
};

VTX_OUTPUT VS_Main(float4 _Input : POSITION)
{
    VTX_OUTPUT OutData = (VTX_OUTPUT) 0.f;
    OutData.vWorldPos = mul(_Input, X90ROTMAT);
    OutData.vPos = mul(OutData.vWorldPos, gVP);

    return OutData;
}

PIX_OUTPUT PS_Main(VTX_OUTPUT _Input)
{
    PIX_OUTPUT OutData = (PIX_OUTPUT) 0.f;

    float MaxAlpha = 1.0f;
    float MinAlpha = 1.0f;
    float TempBorder;

    float L = length(_Input.vWorldPos.xz - gCamWorldPos.xz);
    if (MinRange >= L)
    {
        MinAlpha = 1.0f;
        MinAlpha = (MaxStep - Y) / (MaxStep - MinStep);
        MaxAlpha *= 1.0f;
    }
    else
    {
        TempBorder = 0.5f + ((1.f - (MaxRange - L) / (MaxRange - MinRange)) * 2.f);

        MaxAlpha = (CalMaxRange - L) / (CalMaxRange - MinRange);
        MinAlpha = (MaxStep - Y) / (MaxStep - MinStep);
        MinAlpha *= (CalMaxRange - L) / (CalMaxRange - MinRange);
    }
    
    float LineMinStepX = abs(_Input.vWorldPos.x % MinStep);
    float LineMinStepZ = abs(_Input.vWorldPos.z % MinStep);
    float LineMaxStepX = abs(_Input.vWorldPos.x % MaxStep);
    float LineMaxStepZ = abs(_Input.vWorldPos.z % MaxStep);

    if (TempBorder > LineMaxStepX || TempBorder > LineMaxStepZ)
    {
        OutData.vTexColor = float4(gColorFactor.x, gColorFactor.y, gColorFactor.z, MaxAlpha );


        return OutData;
    }
    if (TempBorder > LineMinStepX || TempBorder > LineMinStepZ)
    {
        OutData.vTexColor = float4(gColorFactor.x, gColorFactor.y, gColorFactor.z, MinAlpha );

        return OutData;
    }
    clip(-1);

    return OutData;
}