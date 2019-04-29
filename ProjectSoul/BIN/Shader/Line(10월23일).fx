#define BASICLINE 0 


struct VS_INPUT
{
    float3 vStartPos : STARTPOS;
    float3 vEndPos : ENDPOS;
    float2 vLineThick : LINETHICK;
    uint   vLineOption : LINEOPTION;
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
}


VS_INPUT VS_Main(VS_INPUT _Input)
{
    return _Input;
}

[maxvertexcount(12)]
void GS_Main(point VS_INPUT _gInput[1], inout TriangleStream<GS_OUT> _gOutStream)
{
    float3 Start = _gInput[0].vStartPos;
    float3 End = _gInput[0].vEndPos;
    float Long = max(_gInput[0].vLineThick.x, _gInput[0].vLineThick.y);
    float Short = min(_gInput[0].vLineThick.x, _gInput[0].vLineThick.y);

    float3 Dir = _gInput[0].vEndPos - _gInput[0].vStartPos;
    Dir = float3(normalize(Dir.xy),0.f);
   


    float Sin = sin(3.141592f * 0.5f);
    float Cos = cos(3.141592f * 0.5f);
    float3x3 Rotate = float3x3(   Cos, Sin, 0.f
                               , -Sin, Cos, 0.f
                               , 0.f, 0.f, 1.f);

    Dir = mul(Dir, Rotate);

    float2 BaseTexCoord[6] =
    {
        float2(0.f, 0.f)
      , float2(1.f, 0.f)
      , float2(0.f, 0.5f)
      , float2(1.f, 0.5f)
      , float2(0.f, 1.f)
      , float2(1.f, 1.f)
    };

    if (_gInput[0].vLineThick.x < _gInput[0].vLineThick.y)
    {
        BaseTexCoord[0].y = -1.f + (Short / Long);
        BaseTexCoord[2].y = 0.f;
        BaseTexCoord[4].y = 2.f - (Short / Long);
    }
    else
    {
        BaseTexCoord[1].y = -1.f + (Short / Long);
        BaseTexCoord[3].y = 0.f;
        BaseTexCoord[5].y = 2.f - (Short / Long);
    }


    float4 Vtx[6] =
    {
        float4(Start + Dir * Long * 0.5f, 1.f),
        float4(End + Dir * Long * 0.5f, 1.f),
        float4(Start, 1.f),
        float4(End, 1.f),
        float4(Start - Dir * Long * 0.5f, 1.f),
        float4(End - Dir * Long * 0.5f, 1.f)
    };

    GS_OUT OutData[6] = { (GS_OUT) 0, (GS_OUT) 0, (GS_OUT) 0, (GS_OUT) 0, (GS_OUT) 0, (GS_OUT) 0 };

    for (int i = 0; i < 6; ++i)
    {
        OutData[i].vPosH = mul(Vtx[i], gViewProj);
        OutData[i].vTexCoord = gSpriteUV.xy + (BaseTexCoord[i] * gSpriteUV.zw);
    }



    switch (_gInput[0].vLineOption)
    {
        case BASICLINE:
        {
                if (_gInput[0].vLineThick.x < _gInput[0].vLineThick.y)
                {
                    _gOutStream.Append(OutData[0]);
                    _gOutStream.Append(OutData[1]);
                    _gOutStream.Append(OutData[2]);
                    _gOutStream.Append(OutData[3]);
                    _gOutStream.Append(OutData[5]);
                    _gOutStream.RestartStrip();
                    _gOutStream.Append(OutData[2]);
                    _gOutStream.Append(OutData[5]);
                    _gOutStream.Append(OutData[4]);
                    _gOutStream.RestartStrip();
                }
                else
                {
                    _gOutStream.Append(OutData[1]);
                    _gOutStream.Append(OutData[0]);
                    _gOutStream.Append(OutData[3]);
                    _gOutStream.Append(OutData[2]);
                    _gOutStream.Append(OutData[4]);
                    _gOutStream.RestartStrip();
                    _gOutStream.Append(OutData[3]);
                    _gOutStream.Append(OutData[4]);
                    _gOutStream.Append(OutData[5]);
                    _gOutStream.RestartStrip();
                }




                //float4 Vtx[4] =
                //{
                //    float4(Start + Dir * Thick.x, 1.f),
                //     float4(End + Dir * Thick.x, 1.f),
                //     float4(Start - Dir * Thick.x, 1.f),
                //     float4(End - Dir * Thick.x, 1.f)
                //};

                //GS_OUT OutData = (GS_OUT) 0;
                //[unroll]
                //for (int i = 0; i < 4; ++i)
                //{
                //    OutData.vPosH = mul(Vtx[i], gViewProj);
                //    OutData.vTexCoord = BaseTexCoord[i];
                //    //gSpriteUV.xy + (BaseTexCoord[i] * gSpriteUV.zw);
                //    _gOutStream.Append(OutData);
                //}
                //_gOutStream.RestartStrip();
                }
                break;
            default:
                return;
        }


}

PIX_OUTPUT PS_Main(GS_OUT _Input)
{
    PIX_OUTPUT OutData = (PIX_OUTPUT) 0.f;

    //float2 UV = (float2) 0.f;


    //if (_Input.vTexCoord.y <= 0.5f)
    //{
    //    if ( (1.f - _Input.vTexCoord.x) * 0.25f > _Input.vTexCoord.y)
    //        clip(-1);

    //    float Y = (_Input.vTexCoord.y - (1.f - _Input.vTexCoord.x) * 0.25f) / (0.5f - (1.f - _Input.vTexCoord.x) * 0.25f);
    //    Y *= 0.5f;
       
    //    UV = float2(_Input.vTexCoord.x, Y);
    //}
    //else
    //{
    //    if (_Input.vTexCoord.x*0.25f + 0.75f < _Input.vTexCoord.y)
    //        clip(-1);

    //    float Y = (_Input.vTexCoord.y - 0.5f) / ((_Input.vTexCoord.x * 0.25f + 0.75f) - 0.5f);
    //    Y *= 0.5f;
    //    UV = float2(_Input.vTexCoord.x, Y + 0.5f);
    //}

    //UV = gSpriteUV.xy + (UV * gSpriteUV.zw);
    //OutData.vTexColor = Image.Sample(Sampler, UV);
    float2 UV = _Input.vTexCoord;

    if (UV.y < 0.f || UV.y > 1.f)
        clip(-1);

    OutData.vTexColor = Image.Sample(Sampler, UV);

    if (OutData.vTexColor.a <= 0.f)
        clip(-1);

    return OutData;
}
