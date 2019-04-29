#define BASICLINE 0 


#define FALSEUV -100.f

#define DECREASE -1
#define EQUAL 0
#define INCREASE 1

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
    float vRatio : RATIO;
    int vSign : SIGN;
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

[maxvertexcount(4)]
void GS_Main(point VS_INPUT _gInput[1], inout TriangleStream<GS_OUT> _gOutStream)
{
    float3 Start = _gInput[0].vStartPos;
    float3 End = _gInput[0].vEndPos;
    float Long = max(_gInput[0].vLineThick.x, _gInput[0].vLineThick.y);
    float Short = min(_gInput[0].vLineThick.x, _gInput[0].vLineThick.y);
    float Ratio = Short / Long;

    float3 Dir = _gInput[0].vEndPos - _gInput[0].vStartPos;
    Dir = float3(normalize(Dir.xy),0.f);
   


    float Sin = sin(3.141592f * 0.5f);
    float Cos = cos(3.141592f * 0.5f);
    float3x3 Rotate = float3x3(   Cos, Sin, 0.f
                               , -Sin, Cos, 0.f
                               , 0.f, 0.f, 1.f);

    Dir = mul(Dir, Rotate);

    float2 BaseTexCoord[4] =
    {
        float2(0.f, 0.f)
      , float2(1.f, 0.f)
      , float2(0.f, 1.f)
      , float2(1.f, 1.f)
    };


    float4 Vtx[4] =
    {
        float4(Start + Dir * Long * 0.5f, 1.f),
        float4(End + Dir * Long * 0.5f, 1.f),
        float4(Start - Dir * Long * 0.5f, 1.f),
        float4(End - Dir * Long * 0.5f, 1.f)
    };


    switch (_gInput[0].vLineOption)
    {
        case BASICLINE:
        {
                GS_OUT OutData[4] = { (GS_OUT) 0, (GS_OUT) 0, (GS_OUT) 0, (GS_OUT) 0 };

                for (int i = 0; i < 4; ++i)
                {
                    OutData[i].vPosH = mul(Vtx[i], gViewProj);
                    OutData[i].vTexCoord = BaseTexCoord[i];
                    OutData[i].vRatio = Short / Long;
                    
                    if (_gInput[0].vLineThick.x == _gInput[0].vLineThick.y)
                        OutData[i].vSign = EQUAL;
                    else if (_gInput[0].vLineThick.x < _gInput[0].vLineThick.y)
                        OutData[i].vSign = INCREASE;
                    else 
                        OutData[i].vSign = DECREASE;


                    _gOutStream.Append(OutData[i]);
                }
                _gOutStream.RestartStrip();
        }
             break;
            default:
                return;
    }


}

float2 CalIncreaseUV(float2 _UV, float _Ratio)
{
    float2 CalUV = (float2) 0;

    if (_UV.y <= 0.5f)
    {
        float ClipY = (1.f - _UV.x) * (1.f - _Ratio) * 0.5f;
        if (ClipY > _UV.y)
            return float2(FALSEUV, FALSEUV);

        float Y = (_UV.y - ClipY) / (0.5f - ClipY);
        Y *= 0.5f;
       
        CalUV = float2(_UV.x, Y);
    }
    else
    {
        float ClipY = (1.f - _Ratio)* 0.5f * _UV.x + (1.f + _Ratio) * 0.5f;

        if (ClipY < _UV.y)
            return float2(FALSEUV, FALSEUV);

        float Y = (_UV.y - 0.5f) / ( ClipY - 0.5f);
        Y *= 0.5f;
        CalUV = float2(_UV.x, Y + 0.5f);
    }

    return CalUV;
}

float2 CalDecreaseUV(float2 _UV, float _Ratio)
{
    float2 CalUV = (float2) 0;

    if (_UV.y <= 0.5f)
    {
        float ClipY = (1.f - _Ratio) * 0.5f * _UV.x;
        if (ClipY > _UV.y)
            return float2(FALSEUV, FALSEUV);

        float Y = (_UV.y - ClipY) / (0.5f - ClipY);
        Y *= 0.5f;
       
        CalUV = float2(_UV.x, Y);
    }
    else
    {
        float ClipY = (_Ratio - 1.f) * 0.5f * _UV.x + 1.f ;

        if (ClipY < _UV.y)
            return float2(FALSEUV, FALSEUV);

        float Y = (_UV.y - 0.5f) / (ClipY - 0.5f);
        Y *= 0.5f;
        CalUV = float2(_UV.x, Y + 0.5f);
    }

    return CalUV;
}

PIX_OUTPUT PS_Main(GS_OUT _Input)
{
    PIX_OUTPUT OutData = (PIX_OUTPUT) 0.f;

    float2 UV = float2(FALSEUV, FALSEUV);

    switch (_Input.vSign)
    {
        case EQUAL:
            UV = _Input.vTexCoord;
            break;
        case INCREASE:
            UV = CalIncreaseUV(_Input.vTexCoord, _Input.vRatio);
            break;
        case DECREASE:
            UV = CalDecreaseUV(_Input.vTexCoord, _Input.vRatio);
            break;
        default:
            break;   
    }

    if (UV.x == FALSEUV && UV.y == FALSEUV)
        clip(-1);

    UV = gSpriteUV.xy + (UV * gSpriteUV.zw);
    OutData.vTexColor = Image.Sample(Sampler, UV);

    if (OutData.vTexColor.a <= 0.f)
        clip(-1);

    return OutData;

}
