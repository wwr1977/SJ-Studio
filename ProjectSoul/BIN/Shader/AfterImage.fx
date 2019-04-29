
struct VS_INPUT
{
    float3 vPos : POSITION;
    float3 vDir : DIRECTION;
    float2 vSize : SIZE;
    float vLife : LIFE;
    float vAlpha : ALPHA;
};


struct GS_OUT
{
    float4 vPosH : SV_Position;
    float2 vTexCoord : TEXCOORD;
    float vAlpha : ALPHA;
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
    float4 gSpriteUV;
    // gAfterImageData.xy = Size,  gAfterImageData.w = AfterImageTime: 
    float4 gAfterImageData;
}


VS_INPUT VS_Main(VS_INPUT _Input)
{
    return _Input;
}

[maxvertexcount(4)]
void GS_Main(point VS_INPUT _gInput[1], inout TriangleStream<GS_OUT> _gOutStream)
{
    float3 Pos = _gInput[0].vPos;
    float Ratio = _gInput[0].vLife / gAfterImageData.w;
    float3 Dir = _gInput[0].vDir;

    float3x3 Rotate = float3x3(  Dir.y,-Dir.x, 0.f
                               , Dir.x, Dir.y, 0.f
                               , 0.f, 0.f, 1.f);



    float3 Right = mul(float3(1.f, 0.f, 0.f),Rotate);
    float3 Up = mul(float3(0.f, 1.f, 0.f), Rotate);
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
        float4(Pos - Right * (HS.x * 0.8f) - Up * HS.y, 1.f),
        float4(Pos + Right * (HS.x * 0.8f) - Up * HS.y, 1.f)
    };

    for (int i = 0; i < 4; ++i)
    {
        if (Vtx[i].y >= 0.f)
            Vtx[i].z = log2(Vtx[i].y + 1.f) * 0.01f;
        else
            Vtx[i].z = log2(-(Vtx[i].y + 1.f)) * -0.01f;
    }


    GS_OUT OutData = (GS_OUT) 0;
    OutData.vAlpha = _gInput[0].vAlpha;

    [unroll]
    for (int i = 0; i < 4; ++i)
    {
        OutData.vPosH = mul(Vtx[i], gViewProj);
        OutData.vTexCoord = gSpriteUV.xy + (BaseTexCoord[i] * gSpriteUV.zw);
        _gOutStream.Append(OutData);
    }
    _gOutStream.RestartStrip();

}

PIX_OUTPUT PS_Main(GS_OUT _Input)
{
    PIX_OUTPUT OutData = (PIX_OUTPUT) 0.f;

    OutData.vTexColor = Image.Sample(Sampler, _Input.vTexCoord) * float4(1.f, 1.f, 1.f, _Input.vAlpha);
    
    if (OutData.vTexColor.a == 0.f)
        clip(-1);

    return OutData;
}


///////////////////////////////////////////////////////             StreamOut          ////////////////////////
//STREAM_VTX StreamOutVS(STREAM_VTX _Input)
//{
//    return _Input;
//}

//[maxvertexcount(4)]
//void StreamOutGS(point STREAM_VTX _gInput[1]
//            , inout PointStream<DRAW_VTX> _gDrawStream
//            , inout PointStream<STREAM_VTX> _gNextStream)
//{
//    STREAM_VTX Vtx = _gInput[0];
//    float dt = gPerFrameData.x;

//    if (Vtx.vLife < 0.f)
//        return;

//    Vtx.vLife = Vtx.vLife - dt;
//    _gNextStream.Append(Vtx);

//    //if (Vtx.vLife <= 0.f)
//    //    return;

//    float TrailTime = gTrailData.w;
//    //(Vtx.vLife / TrailTime)
//    float HS = gTrailData.x  * 0.5f;
//    float3 ExpDir = Vtx.vDir;
    
//    float Sin = sin(3.141592f * 0.5f);
//    float Cos = cos(3.141592f * 0.5f);
//    float3x3 Rotate = float3x3(   Cos, Sin, 0.f
//                               , -Sin, Cos, 0.f
//                               ,  0.f, 0.f, 1.f);

//    ExpDir = mul(ExpDir, Rotate);

//    DRAW_VTX OutData = (DRAW_VTX) 0;

//    OutData.vLife = clamp(Vtx.vLife / TrailTime, 0.f, 1.f);

//    OutData.vPos = Vtx.vPos + ExpDir * HS;
//    OutData.vTexCoord = float2(OutData.vLife,0.f);
//    _gDrawStream.Append(OutData);

//    OutData.vPos = Vtx.vPos - ExpDir * HS;
//    OutData.vTexCoord = float2(OutData.vLife,1.f);
//    _gDrawStream.Append(OutData);
  
   
//}


///////////////////////////////////////////////////////             Draw            ///////////////////////////               
