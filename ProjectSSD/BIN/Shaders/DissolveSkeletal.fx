#include "GlobalValue.fx"
#include "LightGlobal.fx"


struct VTX_INPUT 
{
	float3 vPos         : POSITION;
	float2 vTexCoord    : TEXCOORD;
    float4 vColor       : COLOR;

	float3 vNormal      : NORMAL;
    float3 vBiNormal    : BINORMAL;
    float3 vTangent     : TANGENT;

    float4 vWeight      : WEIGHT;
    int4  vIndices     : INDICES;
};

struct VTX_OUTPUT
{
    float4 vPos         : SV_POSITION;
    float2 vTexCoord    : TEXCOORD;

    float3 vNormal      : NORMAL;
    float3 vBiNormal    : BINORMAL;
    float3 vTangent     : TANGENT;

    float4 vWeight      : WEIGHT;
    int4   vIndices     : INDICES;
    float3 vViewPos    : WORLDPOS;
};


struct PIX_OUTPUT
{
    float4 DiffuseColor : SV_Target0;
    float4 Position     : SV_Target1;
    float4 Normal       : SV_Target2;
    float4 Depth        : SV_Target3;
    float4 Specular     : SV_Target4;
    float4 Emissive     : SV_Target5;
};

Texture2D Tex_Diff : register(t0);
Texture2D Tex_Bump : register(t1);
Texture2D Tex_Spec : register(t2);
Texture2D Tex_Emiv : register(t3);

Texture2D NoiseTex : register(t8);
Texture2D BurnTex : register(t9);

Texture2D AnimTex : register(t10);

SamplerState Sampler : register(s0);

cbuffer DissolveBuffer : register(b0)
{
    uint  gActive;
    float gProgress;
    
    float Dump;
    float Dump2;
}


matrix GetAnimMatrix(uint _Indices, int _Row)
{
    matrix AnimMat;
    int Indices = _Indices * 4;

    AnimMat[0] = AnimTex.Load(int3(Indices, _Row, 0));
    AnimMat[1] = AnimTex.Load(int3(Indices + 1, _Row, 0));
    AnimMat[2] = AnimTex.Load(int3(Indices + 2, _Row, 0));
    AnimMat[3] = AnimTex.Load(int3(Indices + 3, _Row, 0));
    
    return AnimMat;
}

void Skinning(inout float4 _Pos, inout float4 _Weights, inout float3 _Normal, inout float4 _Indices)
{
    float4 ResultPos = (float4) 0.f;
    float4 ResultNormal = (float4) 0.f;

    for (int i = 0; i < 4; ++i)
    {
        if (_Weights[i] <= 0.f)
            continue;

        matrix BoneMat = GetAnimMatrix(_Indices[i], 0);

        ResultPos += mul(_Pos, BoneMat) * _Weights[i];
        ResultNormal += mul(float4(_Normal, 0.f), BoneMat) * _Weights[i];
    }

    ResultPos.w = 1.f;
    ResultNormal.w = 0.f;

    _Pos = ResultPos;
    _Normal = ResultNormal;
    return;
}

VTX_OUTPUT VS_Main(VTX_INPUT _Input)
{
	VTX_OUTPUT OutData = (VTX_OUTPUT)0.f;

    float4 Pos = float4(_Input.vPos, 1.f);

    Skinning(Pos, _Input.vWeight, _Input.vNormal, _Input.vIndices);

    OutData.vPos = mul(Pos, gWVP);
    OutData.vTexCoord = _Input.vTexCoord;
    
    OutData.vNormal = normalize(mul(_Input.vNormal, (float3x3) gNWV));
    OutData.vBiNormal = normalize(mul(_Input.vBiNormal, (float3x3) gNWV));
    OutData.vTangent = normalize(mul(_Input.vTangent, (float3x3) gNWV));
    OutData.vViewPos = mul(Pos, gWV).xyz;

    OutData.vWeight = _Input.vWeight;
    OutData.vIndices = _Input.vIndices;
   
	return OutData;
}

PIX_OUTPUT PS_Main(VTX_OUTPUT _Input)
{
    PIX_OUTPUT OutData = (PIX_OUTPUT) 0.f;
    float Noise = 0.f;
    
    if (gActive != 0)
    {
        float Noise = NoiseTex.Sample(Sampler, _Input.vTexCoord).r;

        if (gProgress >= Noise)
        {
            clip(-1);
        }
        
        float3 BumpNormal = GetBumpNormal(Tex_Bump.Sample(Sampler, _Input.vTexCoord)
                                    , _Input.vTangent
                                    , _Input.vBiNormal
                                    , _Input.vNormal);

        if (Noise - gProgress <= 0.02f)
        {
            OutData.Emissive = BurnTex.Sample(Sampler, _Input.vTexCoord) * float4(3.f,2.5f,1.f,1.f);
            //OutData.Emissive.a = ((Noise - gProgress) / 0.02f);
            OutData.Emissive.a = 1.f;

        }
        else
        {
            OutData.DiffuseColor = Tex_Diff.Sample(Sampler, _Input.vTexCoord);
        }

        OutData.Position = float4(_Input.vViewPos, 1.f);
        OutData.Normal = float4(BumpNormal, 1.f);
        OutData.Depth = float4(_Input.vViewPos.z, 0.f, 0.f, 1.f);

    }
    else
    {
        float3 BumpNormal = GetBumpNormal(Tex_Bump.Sample(Sampler, _Input.vTexCoord)
                                    , _Input.vTangent
                                    , _Input.vBiNormal
                                    , _Input.vNormal);

        OutData.DiffuseColor = Tex_Diff.Sample(Sampler, _Input.vTexCoord);
        OutData.Position = float4(_Input.vViewPos, 1.f);
        OutData.Normal = float4(BumpNormal, 1.f);
        OutData.Depth = float4(_Input.vViewPos.z, 0.f, 0.f, 1.f);
    }
  
    return OutData;
}