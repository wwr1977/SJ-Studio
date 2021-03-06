
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
	float4 vCol : SV_Target;
};

VTXCOL_OUTPUT VS_Main(VTXCOL_INPUT _in)
{
	VTXCOL_OUTPUT outData = (VTXCOL_OUTPUT)0.0f;
    outData.vPos = _in.vPos;
	outData.vUv = _in.vUv;
	return outData;
}

Texture2D g_Tex_0 : register(t0);
Texture2D g_Tex_1 : register(t1);
SamplerState g_Smp_0 : register(s0);

PS_OUTPUT PS_Main(VTXCOL_OUTPUT _in)
{
    PS_OUTPUT outData = (PS_OUTPUT) 0.0f;

    float4 CamColor = g_Tex_0.Sample(g_Smp_0, _in.vUv);
    float4 BlurColor = g_Tex_1.Sample(g_Smp_0, _in.vUv);

    //outData.vCol = CamColor + (BlurColor * 2.f);
    //outData.vCol.w = 1.f;
    outData.vCol = BlurColor;
    return outData;
}