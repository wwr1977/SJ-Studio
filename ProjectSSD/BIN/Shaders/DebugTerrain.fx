#include "GlobalValue.fx"

#define HEIGHTPIVOT 0.2f
struct VTX_INPUT 
{
	float2 vMidPos      : POSITION;
	float4 vHeight      : HEIGHT;
    float4 vColor       : COLOR;

};

struct GEO_OUTPUT
{
    float4 vPos         : SV_POSITION;
    float4 vColor       : COLOR;
};

struct PIX_OUTPUT
{
    float4 vColor : SV_Target;
};

VTX_INPUT VS_Main(VTX_INPUT _Input)
{
    return _Input;
}

[maxvertexcount(6)]
void GS_Main(point VTX_INPUT _gInput[1], inout TriangleStream<GEO_OUTPUT> _gOutStream)
{
    float3 Pos = float3(_gInput[0].vMidPos.x, 0.f, _gInput[0].vMidPos.y);
    GEO_OUTPUT OutData = (GEO_OUTPUT) 0.f;
    OutData.vColor = _gInput[0].vColor;

    
    // LB
    OutData.vPos = mul(float4(float3(Pos.x, _gInput[0].vHeight[0] + HEIGHTPIVOT, Pos.z), 1.f), gWVP);
    _gOutStream.Append(OutData); 
    // LT
    OutData.vPos = mul(float4(float3(Pos.x, _gInput[0].vHeight[1] + HEIGHTPIVOT, Pos.z + 1.f), 1.f), gWVP);
    _gOutStream.Append(OutData);
    // RB
    OutData.vPos = mul(float4(float3(Pos.x + 1.f, _gInput[0].vHeight[3] + HEIGHTPIVOT, Pos.z), 1.f), gWVP);
    _gOutStream.Append(OutData);
    _gOutStream.RestartStrip();

    // LT
    OutData.vPos = mul(float4(float3(Pos.x, _gInput[0].vHeight[1] + HEIGHTPIVOT, Pos.z + 1.f), 1.f), gWVP);
    _gOutStream.Append(OutData);
    // RT
    OutData.vPos = mul(float4(float3(Pos.x + 1.f, _gInput[0].vHeight[2] + HEIGHTPIVOT, Pos.z + 1.f), 1.f), gWVP);
    _gOutStream.Append(OutData);
    // RB
    OutData.vPos = mul(float4(float3(Pos.x + 1.f, _gInput[0].vHeight[3] + HEIGHTPIVOT, Pos.z), 1.f), gWVP);
    _gOutStream.Append(OutData);
    _gOutStream.RestartStrip();

}

PIX_OUTPUT PS_Main(GEO_OUTPUT _Input)
{
    PIX_OUTPUT OutData = (PIX_OUTPUT) 0.f;

    OutData.vColor = _Input.vColor;

    return OutData;
}