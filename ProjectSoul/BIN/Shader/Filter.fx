
cbuffer FilterBuffer : register(b0) 
{
	matrix g_WVP;
    float4 gFilterColor;
}

struct VTXCOL_INPUT 
{
	float3	vPos : POSITION;
    float3  vNormal : NORMAL;
    float2  vTex : TEXCOORD;
};

struct VTXCOL_OUTPUT 
{
	float4	vPos : SV_POSITION;
    float3  vNormal : NORMAL;
    float2  vTex : TEXCOORD;
};

struct PS_OUTPUT 
{
	float4 vColor : SV_Target;
};


VTXCOL_OUTPUT VS_Main(VTXCOL_INPUT  _Input) 
{
	VTXCOL_OUTPUT OutData = (VTXCOL_OUTPUT)0.f;

    OutData.vPos = mul(float4(_Input.vPos, 1.f), g_WVP);
    OutData.vTex = _Input.vTex;

    return OutData;
}

PS_OUTPUT PS_Main(VTXCOL_OUTPUT _Input)
{
    PS_OUTPUT OutData = (PS_OUTPUT) 0.f;
    
    float Len = length(float2(0.5f, 0.5f) - _Input.vTex);

    if (Len <= 0.3f)
        clip(-1);

    OutData.vColor = gFilterColor * float4(1.f, 1.f, 1.f, (Len - 0.3f)*3.f);

    return OutData;
}