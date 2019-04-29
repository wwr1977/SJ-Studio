#include "GlobalValue.fx"

struct VTX_INPUT 
{
	float3  vPos            : POSITION;
    float3  vViewPos        : VIEWPOSITION;
    float   vNum            : TEXNUMBER;
    float   vSizex          : SIZEX;
    float   vSizey          : SIZEY;
    float2  vTime           : PARTICLETIME;
    float4  vCheck          : CHECK;
    float3  vAxisX          : AXISX;
    float3  vAxisY          : AXISY;
    float4  vAnimUV         : ANIMUV;
};

struct GEO_OUTPUT
{
    float4  vPos            : SV_POSITION;
    float4  vProjPos        : POSITION;
    float4  vViewPos        : TEXCOORD1;
    float2  vTexCoord       : TEXCOORD;
    float   vNum            : TEXNUMBER;
    float2  vTime           : PARTICLETIME;
    float4  vCheck          : CHECK;
};

//vCheck.x = 빌보드인지 아닌지 체크 - 1이면 빌보드
//vCheck.y = 애니메이션인지 아닌지 체크 = 1이면 애니메이션

struct PIX_OUTPUT
{
    float4 vOutColor        : SV_Target;
};

cbuffer ParticleBuffer : register(b0)
{
    float4              gAxisX;
    float4              gAxisY;
    float4              gAccTime;
    float4              gParticleColor;
}

VTX_INPUT VS_Main(VTX_INPUT _Input)
{
    return _Input;
}

[maxvertexcount(4)]
void GS_Main(point VTX_INPUT _input[1], inout TriangleStream<GEO_OUTPUT> _stream)
{
    GEO_OUTPUT Output = (GEO_OUTPUT) 0;

    float fHalfX = _input[0].vSizex * 0.5f;
    float fHalfY = _input[0].vSizey * 0.5f;

    float3 AxisX;
    float3 AxisY;

    if (0 == _input[0].vCheck.x)
    {
        AxisX = _input[0].vAxisX;
        AxisY = _input[0].vAxisY;
    }
    else
    {
        AxisX = gAxisX.xyz;
        AxisY = gAxisY.xyz;
    }

    float3 vPos[4];
    vPos[0] = _input[0].vPos - (AxisX * fHalfX) - (AxisY * fHalfY);
    vPos[1] = _input[0].vPos - (AxisX * fHalfX) + (AxisY * fHalfY);
    vPos[2] = _input[0].vPos + (AxisX * fHalfX) - (AxisY * fHalfY);
    vPos[3] = _input[0].vPos + (AxisX * fHalfX) + (AxisY * fHalfY);

    float2 vUV[4] =
    {
        float2(0.f, 1.f),
		float2(0.f, 0.f),
		float2(1.f, 1.f),
		float2(1.f, 0.f)
    };

    float2 vAnimUV[4] =
    {
        float2(_input[0].vAnimUV.x, _input[0].vAnimUV.y + _input[0].vAnimUV.w),
		float2(_input[0].vAnimUV.x, _input[0].vAnimUV.y ),
		float2(_input[0].vAnimUV.x + _input[0].vAnimUV.z, _input[0].vAnimUV.y + _input[0].vAnimUV.w),
		float2(_input[0].vAnimUV.x + _input[0].vAnimUV.z, _input[0].vAnimUV.y)
    };

    for (int i = 0; i < 4; ++i)
    {
        Output.vProjPos = mul(float4(vPos[i], 1.f), gVP);
        Output.vViewPos = mul(float4(vPos[i], 1.f), gView);
        Output.vPos = Output.vProjPos;
        Output.vNum = _input[0].vNum;
        Output.vTime = _input[0].vTime;
        Output.vCheck = _input[0].vCheck;

        if (0 != _input[0].vCheck.y)
        {
            Output.vTexCoord = vAnimUV[i];
        }
        else
        {
            Output.vTexCoord = vUV[i];
        }
        _stream.Append(Output);
    }
}

Texture2D DepthTex          : register(t0);
Texture2DArray ParticleTex : register(t9);
Texture2DArray FadeTex : register(t8);

SamplerState Sampler        : register(s0);

PIX_OUTPUT PS_Main(GEO_OUTPUT _input)
{
    PIX_OUTPUT Output = (PIX_OUTPUT) 0;

    //왜곡효과
    if (0 != _input.vCheck.x)
    {
        // _input.vUV.y += sin(abs((_input.vUV.x - g_fAccTime * 파장 속도) * 진폭 밀도)) * 파장 크기
        _input.vTexCoord.y += sin(abs((_input.vTexCoord.x - gAccTime * 0.05f) * 150.f)) * 0.002f;
    }
    else
    {
        _input.vTexCoord.y += sin(abs((_input.vTexCoord.x - gAccTime * 0.02f) * 100.f)) * 0.002f;
    }

    Output.vOutColor = ParticleTex.Sample(Sampler, float3(_input.vTexCoord, _input.vNum));
   
    if (0.f != _input.vCheck.z)
    {
        Output.vOutColor = FadeTex.Sample(Sampler, float3(_input.vTexCoord, _input.vNum)) * gParticleColor;
    }

    if (0.f == Output.vOutColor.a)
    {
        clip(-1);
    }

    float2 DepthUV = _input.vProjPos.xy / _input.vProjPos.w;
    DepthUV.x = DepthUV.x * 0.5f + 0.5f;
    DepthUV.y = DepthUV.y * -0.5f + 0.5f;

    float4 vDepth = DepthTex.Sample(Sampler, DepthUV);
    float DepthDist = vDepth.x - _input.vProjPos.w;

    float Alpha = 0.f;
    if (0.f == vDepth.a)
    {
        Alpha = 0.8f;
    }
    else if (DepthDist < 0.f)
    {
        clip(-1);
    }
    else
    {
        Alpha = (DepthDist / 400.f);
        Alpha = min(Alpha, 0.8f);
    }

    if (0 != _input.vCheck.x)
    {
        //거리에 따라서 500.f 이내면 안개 서서히 사라지기
        float TempAlpha = Output.vOutColor.a * Alpha;
        if (_input.vViewPos.z <= 400.f)
        {
            float t = _input.vViewPos.z * 0.0025f;
            Output.vOutColor.a = saturate(lerp(0.f, TempAlpha, t));
        }
        else
        {
            Output.vOutColor.a = TempAlpha;
        }
    }

    //페이드용


    return Output;
}


//-------
    //Output.vOutColor = ParticleTex.Sample(Sampler, float3(_input.vTexCoord, _input.vNum));

    //if (0.f == Output.vOutColor.a)
    //{
    //    clip(-1);
    //}

    //float2 DepthUV = _input.vProjPos.xy / _input.vProjPos.w;
    //DepthUV.x = DepthUV.x * 0.5f + 0.5f;
    //DepthUV.y = DepthUV.y * -0.5f + 0.5f;

    //float4 vDepth = DepthTex.Sample(Sampler, DepthUV);
    //float DepthDist = vDepth.x - _input.vProjPos.w;

    //float Alpha = 0.f;
    //if (0.f == vDepth.a)
    //{
    //    Alpha = 0.8f;
    //}
    //else if (DepthDist < 0.f)
    //{
    //    clip(-1);
    //}
    //else
    //{
    //    Alpha = (DepthDist / 400.f);
    //    Alpha = min(Alpha, 0.8f);
    //}
    ////float Alpha = DepthDist / 0.4f;
    ////Alpha = min(Alpha, 1.f);

    ////Output.vOutColor.a *= Alpha;

    ////거리에 따라서 500.f 이내면 안개 서서히 사라지기
    //float TempAlpha = Output.vOutColor.a * Alpha;
    //if (_input.vViewPos.z <= 400.f)
    //{
    //    float t = _input.vViewPos.z * 0.0025f;
    //    Output.vOutColor.a = saturate(lerp(0.f, TempAlpha, t));
    //}
    //else
    //{
    //    Output.vOutColor.a = TempAlpha;
    //}
//--------