#include "GlobalValue.fx"
#include "LightGlobal.fx"

struct VTX_INPUT
{
    float2 vTerrainIdx  : POSITION;
    float4 vHeight      : HEIGHT;
    float4 vColor       : COLOR;

};

struct GEO_OUTPUT
{
    float4 vPos         : SV_POSITION;
    float2 vTexCoord    : TEXCOORD;

    float3 vNormal      : NORMAL;
    float3 vBiNormal    : BINORMAL;
    float3 vTangent     : TANGENT;
    float3 vViewPos     : VIEWPOSITION;
};

struct PIX_OUTPUT
{
    float4 DiffuseColor : SV_Target0;
    float4 Position : SV_Target1;
    float4 Normal : SV_Target2;
    float4 Depth : SV_Target3;
};

cbuffer TerrainBuffer : register(b0)
{
    int gFloorCount;
    float gSizeX;
    float gSizeZ;

    float gNone1;
};

VTX_INPUT VS_Main(VTX_INPUT _Input)
{
    return _Input;
}




[maxvertexcount(6)]
void GS_Main(point VTX_INPUT _gInput[1], inout TriangleStream<GEO_OUTPUT> _gOutStream)
{
    float3 Pos = float3(_gInput[0].vTerrainIdx.x, 0.f, _gInput[0].vTerrainIdx.y);
    GEO_OUTPUT OutData = (GEO_OUTPUT) 0.f;

    float3 T, B, N;

    T = float3(1.f, 0.f, 0.f);
    B = float3(0.f, 0.f, -1.f);
    N = float3(0.f, 1.f, 0.f);
    OutData.vNormal = normalize(mul(N, (float3x3) gNWV));
    OutData.vTangent = normalize(mul(T, (float3x3) gNWV));
    OutData.vBiNormal = normalize(mul(B, (float3x3) gNWV));


    // LB
    float4 LB, LT, RT, RB;
    float4 VLB, VLT, VRT, VRB; // 뷰 공간에서의 정점 위치
    float4 HLB, HLT, HRT, HRB; // 투영 공간에서의 정점 위치
   
 
    LB = float4(float3(Pos.x, _gInput[0].vHeight[0], Pos.z), 1.f);
    LT = float4(float3(Pos.x, _gInput[0].vHeight[1], Pos.z + 1.f), 1.f);
    RT = float4(float3(Pos.x + 1.f, _gInput[0].vHeight[2], Pos.z + 1.f), 1.f);
    RB = float4(float3(Pos.x + 1.f, _gInput[0].vHeight[3], Pos.z), 1.f);


    VLB = mul(LB, gWV);
    VLT = mul(LT, gWV);
    VRT = mul(RT, gWV);
    VRB = mul(RB, gWV);

    HLB = mul(LB, gWVP);
    HLT = mul(LT, gWVP);
    HRT = mul(RT, gWVP);
    HRB = mul(RB, gWVP);
   
    // LB
    OutData.vPos = HLB;
    OutData.vViewPos = VLB;
    OutData.vTexCoord = float2(_gInput[0].vTerrainIdx.x, gSizeZ - _gInput[0].vTerrainIdx.y);
    _gOutStream.Append(OutData);
    // LT
    OutData.vPos = HLT;
    OutData.vViewPos = VLT;
    OutData.vTexCoord = float2(_gInput[0].vTerrainIdx.x, gSizeZ - (_gInput[0].vTerrainIdx.y + 1.f));
    _gOutStream.Append(OutData);
    // RB
    OutData.vPos = HRB;
    OutData.vViewPos = VRB;
    OutData.vTexCoord = float2(_gInput[0].vTerrainIdx.x + 1.f, gSizeZ - _gInput[0].vTerrainIdx.y);
    _gOutStream.Append(OutData);
    _gOutStream.RestartStrip();

    // LT
    OutData.vPos = HLT;
    OutData.vViewPos = VLT;
    OutData.vTexCoord = float2(_gInput[0].vTerrainIdx.x, gSizeZ - (_gInput[0].vTerrainIdx.y + 1.f));
    _gOutStream.Append(OutData);
    // RT
    OutData.vPos = HRT;
    OutData.vViewPos = VRT;
    OutData.vTexCoord = float2(_gInput[0].vTerrainIdx.x + 1.f, gSizeZ - (_gInput[0].vTerrainIdx.y + 1.f));
    _gOutStream.Append(OutData);
    // RB
    OutData.vPos = HRB;
    OutData.vViewPos = VRB;
    OutData.vTexCoord = float2(_gInput[0].vTerrainIdx.x + 1.f, gSizeZ - _gInput[0].vTerrainIdx.y);
    _gOutStream.Append(OutData);
    _gOutStream.RestartStrip();

}

Texture2D Tex_BaseDiffuse : register(t0);
Texture2D Tex_BaseNormal : register(t1);
Texture2DArray Tex_Floor : register(t9);

SamplerState Sampler : register(s0);



PIX_OUTPUT PS_Main(GEO_OUTPUT _Input)
{
    PIX_OUTPUT OutData = (PIX_OUTPUT) 0.f;

    //가장 아래 깔리는 바닥 디퓨즈, 노말
    OutData.DiffuseColor = Tex_BaseDiffuse.Sample(Sampler, _Input.vTexCoord);
    OutData.Normal = float4(GetBumpNormal(Tex_BaseNormal.Sample(Sampler, _Input.vTexCoord),
                                    _Input.vTangent,
                                    _Input.vBiNormal,
                                    _Input.vNormal), 0.f);

    for (int i = 0; i < gFloorCount; ++i)
    {
        float2 SplattingUv = float2(_Input.vTexCoord.x / gSizeX, _Input.vTexCoord.y / gSizeZ);
        float4 Ratio = Tex_Floor.Sample(Sampler, float3(SplattingUv.x, SplattingUv.y, 0.f));

        float4 DownColor = OutData.DiffuseColor;
        float4 UpColor = Tex_Floor.Sample(Sampler, float3(_Input.vTexCoord.x, _Input.vTexCoord.y, 1.f)).bgra;

        //이 위치에 있는 픽셀의 비율만큼 Up의 컬러와 Down의 컬러를 정한다
        OutData.DiffuseColor = (UpColor * Ratio.x) + (DownColor * (1.f - Ratio.x));

        float4 DownNormal = OutData.Normal;
        float4 UpNormal = float4(GetBumpNormal(Tex_Floor.Sample(Sampler, float3(_Input.vTexCoord.x, _Input.vTexCoord.y, 2.f)).bgra,
                                    _Input.vTangent,
                                    _Input.vBiNormal,
                                    _Input.vNormal), 0.f);

        //이 위치에 있는 픽셀의 비율만큼 Up의 노말과 Down의 노말을 정한다
        OutData.Normal = (UpNormal * Ratio.x) + (DownNormal * (1.f - Ratio.x));
        OutData.Normal = normalize(OutData.Normal);
    }

    OutData.Position = float4(_Input.vViewPos, 1.f);
    OutData.Depth.x = _Input.vViewPos.z;
    OutData.Depth.y = 0.f;
    OutData.Depth.z = 0.f;
    OutData.Depth.w = 1.f;

    OutData.DiffuseColor.w = 1.f;
    OutData.Normal.w = 1.f;

    return OutData;
}