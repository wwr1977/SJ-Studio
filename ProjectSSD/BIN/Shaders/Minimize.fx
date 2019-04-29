#define BASEMINIMIZE 0
#define LUMINANCEMINIMIZE 1



struct VTXCOL_INPUT
{
	float4 vPos : POSITION;
	float2 vUv : TEXCOORD;
};


struct VTXCOL_OUTPUT
{
	float4 vPos : SV_POSITION;
	float2 vUv : TEXCOORD;
};


struct PS_OUTPUT
{
	float4 vColor  : SV_Target0;
};


VTXCOL_OUTPUT VS_Main(VTXCOL_INPUT _in)
{
	VTXCOL_OUTPUT outData = (VTXCOL_OUTPUT)0.0f;
    outData.vPos = _in.vPos;
	outData.vUv = _in.vUv;
	return outData;
}

Texture2D g_Tex_0 : register(t0);
SamplerState g_Smp_0 : register(s0);

cbuffer MinimizeBuffer : register(b0)
{
    uint                gMinimizeType;
    float               gAmount;
    float3              gLuminance;
    float               gMultiple;

   // float               None1;
    float               None2;
    float               None3;
};

PS_OUTPUT PS_Main(VTXCOL_OUTPUT _in)
{
	PS_OUTPUT outData = (PS_OUTPUT)0.0f;
    float4 Mini = g_Tex_0.Sample(g_Smp_0, _in.vUv);

    if (gMinimizeType == BASEMINIMIZE)
    {
        if (Mini.a == 0.f)
        {
            clip(-1);
        }

        outData.vColor = Mini;
    }
    else if (gMinimizeType == LUMINANCEMINIMIZE)
    {
        float Lumi = (Mini.r * gLuminance.x) + (Mini.g * gLuminance.y) + (Mini.b * gLuminance.z);
    
        if (gAmount >= Lumi)
        {
            outData.vColor = float4(0.f, 0.f, 0.f, 1.f);
            return outData;
        }

        outData.vColor = float4(Lumi * gMultiple, Lumi * gMultiple, Lumi * gMultiple, 1.f);
    }
 
    return outData;
}