#include "GlobalValue.fx"
#include "LightGlobal.fx"

struct VTX_INPUT
{
    float4 vT1 : POSITION0;
    float4 vT2 : POSITION1;
    float4 vT3 : POSITION2;
    float4 vColor : COLOR;

};

struct GEO_OUTPUT
{
    float4 vPos : SV_POSITION;
    float4 vColor : COLOR;
};

struct PIX_OUTPUT
{
    float4 vOutColor : SV_Target0;
};

VTX_INPUT VS_Main(VTX_INPUT _Input)
{
    return _Input;
}

[maxvertexcount(3)]
void GS_Main(point VTX_INPUT _gInput[1], inout TriangleStream<GEO_OUTPUT> _gOutStream)
{
    GEO_OUTPUT Data = (GEO_OUTPUT) 0.f;
    Data.vColor = _gInput[0].vColor;

    Data.vPos = float4(_gInput[0].vT1.xyz, 1.f);
    _gOutStream.Append(Data);

    Data.vPos = float4(_gInput[0].vT2.xyz, 1.f);
    _gOutStream.Append(Data);

    Data.vPos = float4(_gInput[0].vT3.xyz, 1.f);
    _gOutStream.Append(Data);

    _gOutStream.RestartStrip();

}

PIX_OUTPUT PS_Main(GEO_OUTPUT _Input)
{
    PIX_OUTPUT OutData = (PIX_OUTPUT) 0.f;

    OutData.vOutColor = _Input.vColor;

    return OutData;
}