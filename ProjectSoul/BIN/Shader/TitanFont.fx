
struct VS_INPUT
{
    float3 vPos : POSITION;
    float2 vSize : SIZE;
    float4 vSpriteTexCoord : SPRITETEXCOORD;
};


struct GS_OUT
{
    float4 vPosH : SV_Position;
    float2 vTexCoord : TEXCOORD;
};


struct PIX_OUTPUT
{
	float4 vTexColor : SV_Target;
};

texture2D Image : register(t0);
SamplerState Sampler : register(s0);


cbuffer TransformBuffer : register(b0)
{
    matrix gViewProj;
    float4 gColorFactor;
}


VS_INPUT VS_Main(VS_INPUT _Input)
{
    return _Input;
}

[maxvertexcount(4)]
void GS_Main(point VS_INPUT _gInput[1], inout TriangleStream<GS_OUT> _gOutStream)
{
    float3 Pos = _gInput[0].vPos;
   
    

    float3 Right = float3(1.f, 0.f, 0.f);
    float3 Up = float3(0.f, 1.f, 0.f);
    float2 HS = _gInput[0].vSize * 0.5f;

    float2 BaseTexCoord[4] =
    {
        float2(0.f, 0.f)
      , float2(1.f, 0.f)
      , float2(0.f, 1.f)
      , float2(1.f, 1.f)
    };

    float4 Vtx[4] =
    {
        float4(Pos - Right * HS.x + Up * HS.y, 1.f),
        float4(Pos + Right * HS.x + Up * HS.y, 1.f),
        float4(Pos - Right * HS.x - Up * HS.y, 1.f),
        float4(Pos + Right * HS.x - Up * HS.y, 1.f)
    };

    GS_OUT OutData = (GS_OUT) 0;

    [unroll]
    for (int i = 0; i < 4; ++i)
    {
        OutData.vPosH = mul(Vtx[i], gViewProj);
        OutData.vTexCoord = _gInput[0].vSpriteTexCoord.xy + (BaseTexCoord[i] * _gInput[0].vSpriteTexCoord.zw);
        _gOutStream.Append(OutData);
    }
    _gOutStream.RestartStrip();

}

PIX_OUTPUT PS_Main(GS_OUT _Input)
{
    PIX_OUTPUT OutData = (PIX_OUTPUT) 0.f;

    OutData.vTexColor = Image.Sample(Sampler, _Input.vTexCoord) * gColorFactor;
    
    if (OutData.vTexColor.a == 0.f)
        clip(-1);

    return OutData;
}

