#include "GlobalValue.fx"

#define SPHERE 1
#define CUBE 2
#define CONE 3
#define FRUSTUM 4

cbuffer DebugBuffer : register(b0)
{
    uint                          gFiOption;
    float                         gBoarderSize;
    float2                        gFiFactor;
    float4x4                      gFrustumMat;
};


struct VTX_INPUT
{
    float3 vPos : POSITION;
    float3 vNormal : NORMAL;
    float2 vTexCoord : TEXCOORD;

};

struct VTX_OUTPUT
{
    float4 vPos : SV_POSITION;
    float3 vNormal : NORMAL;
    float2 vTexCoord : TEXCOORD;
    float3 vWorldPos : TEXCOORD1;
};

struct PIX_OUTPUT
{
    float4 vTexColor : SV_Target;
};

Texture2D Image : register(t0);
SamplerState Sampler : register(s0);


VTX_OUTPUT VS_Main(VTX_INPUT _Input)
{
    VTX_OUTPUT OutData = (VTX_OUTPUT) 0.f;

    if (gFiOption == FRUSTUM)
    {
        float4 FrustumPos = mul(float4(_Input.vPos, 1.f), gFrustumMat);
        OutData.vWorldPos = FrustumPos.xyz / abs(FrustumPos.w);
        OutData.vPos = mul(float4(OutData.vWorldPos, 1.f), gVP);
        OutData.vNormal = mul(_Input.vNormal, (float3x3) gFrustumMat);
        OutData.vTexCoord = _Input.vTexCoord;
    }
    else
    {
        OutData.vWorldPos = mul(_Input.vPos, (float3x3) gWorld);
        OutData.vPos = mul(float4(_Input.vPos, 1.f), gWVP);
        OutData.vNormal = mul(_Input.vNormal, (float3x3) gWorld);
        OutData.vTexCoord = _Input.vTexCoord;
    }
    return OutData;
}


bool CheckCubeBoarder(float2 _Tex)
{
    if (_Tex.x < 0.02f || _Tex.x > 0.98f)
        return true;

    if (_Tex.y < 0.02f || _Tex.y > 0.98f)
        return true;

    return false;
}

bool CheckSphereBoarder(float3 _WorldPos)
{
    return true;

    if (_WorldPos.x < gBoarderSize * 0.5f && _WorldPos.x > gBoarderSize * -0.5f)
        return true;

    if (_WorldPos.y < gBoarderSize * 0.5f && _WorldPos.y > gBoarderSize * -0.5f)
        return true;

    if (_WorldPos.z < gBoarderSize * 0.5f && _WorldPos.z > gBoarderSize * -0.5f)
        return true;

    return false;
}

bool CheckConeBoarder(float3 _WorldPos)
{
    return true;

    if (_WorldPos.x < gBoarderSize * 0.5f && _WorldPos.x > gBoarderSize * -0.5f)
        return true;

    if (_WorldPos.y < gBoarderSize * 0.5f && _WorldPos.y > gBoarderSize * -0.5f)
        return true;

    if (_WorldPos.z < gBoarderSize * 0.5f && _WorldPos.z > gBoarderSize * -0.5f)
        return true;

    return false;
}

//bool CheckConeBoarder(float3 _WorldPos)
//{
//    if (_WorldPos.x < gBoarderSize * 0.5f && _WorldPos.x > gBoarderSize * -0.5f)
//        return true;

//    if (_WorldPos.y < gBoarderSize * 0.5f && _WorldPos.y > gBoarderSize * -0.5f)
//        return true;

//    if (_WorldPos.z < gBoarderSize * 0.5f && _WorldPos.z > gBoarderSize * -0.5f)
//        return true;

//    return false;
//}

PIX_OUTPUT PS_Main(VTX_OUTPUT _Input)
{
    PIX_OUTPUT OutData = (PIX_OUTPUT) 0.f;
    
    //if (gFiOption == SPHERE)
    //{
    //    if (true == CheckSphereBoarder(_Input.vWorldPos))
    //        OutData.vTexColor = gColorFactor;
    //    else
    //        clip(-1);

    //}
    //else if (gFiOption == CUBE)
    //{
    //    if (true == CheckCubeBoarder(_Input.vTexCoord))
    //        OutData.vTexColor = gColorFactor;
    //    else
    //        clip(-1);
    //}
    //else
    //    clip(-1);
   
    if (gFiOption == CUBE)
    {
        if (true == CheckCubeBoarder(_Input.vTexCoord))
        {
            OutData.vTexColor = gColorFactor;
        }
        else
        {
            clip(-1);
        }
    }

    OutData.vTexColor = gColorFactor;
    return OutData;
}