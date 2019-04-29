
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

Texture2D OriginalTexture : register(t0);
Texture2D DepthTexture : register(t1);
Texture2D Mini512Texture : register(t2);
Texture2D Mini256Texture : register(t3);
Texture2D Mini128Texture : register(t4);
Texture2D Mini64Texture : register(t5);

SamplerState g_Smp_0 : register(s0);

cbuffer MinimizeBuffer : register(b0)
{

};

PS_OUTPUT PS_Main(VTXCOL_OUTPUT _in)
{
	PS_OUTPUT outData = (PS_OUTPUT)0.0f;
    
    
    float4 D = DepthTexture.Sample(g_Smp_0, _in.vUv);
    if (D.w <= 0.f)
    {
        outData.vColor = OriginalTexture.Sample(g_Smp_0, _in.vUv);
    }
    else
    {
        float Depth = D.r, T;
        float4 Src1, Src2;
        if (Depth <= 1000.f)
        {
            outData.vColor = OriginalTexture.Sample(g_Smp_0, _in.vUv);
        }
        else if (Depth > 1000.f && Depth <= 2500.f)
        {
            Src1 = OriginalTexture.Sample(g_Smp_0, _in.vUv);
            Src2 = Mini512Texture.Sample(g_Smp_0, _in.vUv);
            T = (Depth - 1000.f) / 1500.f;
            T = pow(T, 2.f);
            outData.vColor = (Src1 * (1.f - T)) + (Src2 * T);
            outData.vColor.a = 1.f;
        }
        else if (Depth > 2500.f && Depth <= 5000.f)
        {
            Src1 = Mini512Texture.Sample(g_Smp_0, _in.vUv);
            Src2 = Mini256Texture.Sample(g_Smp_0, _in.vUv);
            T = (Depth - 2500.f) / 2500.f;
            outData.vColor = (Src1 * (1.f - T)) + (Src2 * T);
            outData.vColor.a = 1.f;

    
        }
        else if (Depth > 5000.f && Depth <= 8000.f)
        {
            Src1 = Mini256Texture.Sample(g_Smp_0, _in.vUv);
            Src2 = Mini128Texture.Sample(g_Smp_0, _in.vUv);
            T = (Depth - 5000.f) / 3000.f;
            outData.vColor = (Src1 * (1.f - T)) + (Src2 * T);
            outData.vColor.a = 1.f;
        }
        else
        {
            outData.vColor = Mini128Texture.Sample(g_Smp_0, _in.vUv);
        }
    }

    //    if (Depth <= 200.f)
    //    {
    //        outData.vColor = OriginalTexture.Sample(g_Smp_0, _in.vUv);
    //    }
    //    else if (Depth > 200.f && Depth <= 500.f)
    //    {
    //        Src1 = OriginalTexture.Sample(g_Smp_0, _in.vUv);
    //        Src2 = Mini512Texture.Sample(g_Smp_0, _in.vUv);
    //        T = (Depth - 1000.f) / 1500.f;
    //        T = pow(T, 2.f);
    //        outData.vColor = (Src1 * (1.f - T)) + (Src2 * T);
    //        outData.vColor.a = 1.f;
    //    }
    //    else if (Depth > 500.f && Depth <= 1000.f)
    //    {
    //        Src1 = Mini512Texture.Sample(g_Smp_0, _in.vUv);
    //        Src2 = Mini256Texture.Sample(g_Smp_0, _in.vUv);
    //        T = (Depth - 2500.f) / 2500.f;
    //        outData.vColor = (Src1 * (1.f - T)) + (Src2 * T);
    //        outData.vColor.a = 1.f;

    
    //    }
    //    else if (Depth > 1000.f && Depth <= 2000.f)
    //    {
    //        Src1 = Mini256Texture.Sample(g_Smp_0, _in.vUv);
    //        Src2 = Mini128Texture.Sample(g_Smp_0, _in.vUv);
    //        T = (Depth - 5000.f) / 3000.f;
    //        outData.vColor = (Src1 * (1.f - T)) + (Src2 * T);
    //        outData.vColor.a = 1.f;
    //    }
    //    else
    //    {
    //        outData.vColor = Mini128Texture.Sample(g_Smp_0, _in.vUv);
    //    }
    //}

       
    if (outData.vColor.a <= 0.f)
    {
        clip(-1);
    }

    return outData;
}