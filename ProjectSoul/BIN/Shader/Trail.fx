
#define PI 3.1415926535f 

//struct VTX_INPUT 
//{
//	float3 vPos : POSITION;
//    float3 vDir : DIRECTION;
//	float vLife : LIFE;
//};

struct STREAM_VTX
{
    float3 vPos : POSITION;
    float3 vDir : DIRECTION;
    float vLife : LIFE;
};


struct DRAW_VTX
{
    float3 vPos : POSITION;
    float2 vTexCoord : TEXCOORD;
    float vLife : LIFE;
};

struct VS_OUT
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
    // TrailData xy : 트레일 너비,높이 , TrailData.w : 트레일 생존시간
    float4 gTrailData;
    // gPerFrameData.x = DeltaTime : 
    float4 gPerFrameData;
}



///////////////////////////////////////////////////////             StreamOut          ////////////////////////
STREAM_VTX StreamOutVS(STREAM_VTX _Input)
{
    return _Input;
}

[maxvertexcount(4)]
void StreamOutGS(point STREAM_VTX _gInput[1]
            , inout PointStream<DRAW_VTX> _gDrawStream
            , inout PointStream<STREAM_VTX> _gNextStream)
{
    STREAM_VTX Vtx = _gInput[0];
    float dt = gPerFrameData.x;

    if (Vtx.vLife < 0.f)
        return;

    Vtx.vLife = Vtx.vLife - dt;
    _gNextStream.Append(Vtx);

    //if (Vtx.vLife <= 0.f)
    //    return;

    float TrailTime = gTrailData.w;
    //(Vtx.vLife / TrailTime)
    float HS = gTrailData.x  * 0.5f;
    float3 ExpDir = Vtx.vDir;
    
    float Sin = sin(3.141592f * 0.5f);
    float Cos = cos(3.141592f * 0.5f);
    float3x3 Rotate = float3x3(   Cos, Sin, 0.f
                               , -Sin, Cos, 0.f
                               ,  0.f, 0.f, 1.f);

    ExpDir = mul(ExpDir, Rotate);

    DRAW_VTX OutData = (DRAW_VTX) 0;

    OutData.vLife = clamp(Vtx.vLife / TrailTime, 0.f, 1.f);

    OutData.vPos = Vtx.vPos + ExpDir * HS;
    OutData.vTexCoord = float2(OutData.vLife,0.f);
    _gDrawStream.Append(OutData);

    OutData.vPos = Vtx.vPos - ExpDir * HS;
    OutData.vTexCoord = float2(OutData.vLife,1.f);
    _gDrawStream.Append(OutData);
   
    //Vtx.vPos = _gInput[0].vPos + ExpDir * HS;
    //_gDrawStream.Append(Vtx);

    //Vtx.vPos = _gInput[0].vPos - ExpDir * HS;
    //_gDrawStream.Append(Vtx);
   
}


///////////////////////////////////////////////////////             Draw            ///////////////////////////               
VS_OUT VS_Main(DRAW_VTX _Input)
{
    VS_OUT OutData = (VS_OUT) 0;
    OutData.vPosH = mul(float4(_Input.vPos, 1.f), gViewProj);
    OutData.vAlpha = _Input.vLife;
    OutData.vTexCoord.x = gSpriteUV.x + gSpriteUV.z * _Input.vTexCoord.x;
    OutData.vTexCoord.y = gSpriteUV.y + gSpriteUV.w * _Input.vTexCoord.y;
    
    return OutData;
}

PIX_OUTPUT PS_Main(VS_OUT _Input)
{
	PIX_OUTPUT OutData = (PIX_OUTPUT)0.f;

    OutData.vTexColor = Image.Sample(Sampler, _Input.vTexCoord);
    
    if (OutData.vTexColor.a == 0.f)
        clip(-1);

   //OutData.vTexColor = float4(1.f, 1.f, 1.f, _Input.vAlpha);

   return OutData;
}