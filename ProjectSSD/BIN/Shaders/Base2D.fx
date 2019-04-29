
cbuffer Base2DBuffer : register(b0)
{
    matrix              gWVP;
    float3              gColor;
    int                 gType;
    float4              gBorderData;
}

struct VTXCOL_INPUT 
{
	float3	    vPos : POSITION;
    float3      vNormal : NORMAL;
    float2      vTex : TEXCOORD;
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

    OutData.vPos = mul(float4(_Input.vPos, 1.f), gWVP);
    OutData.vNormal = _Input.vNormal;
    OutData.vTex = _Input.vTex;

    return OutData;
}


bool RectLine(const float2 _vTex)
{
    float X = gBorderData.x * gBorderData.z;
    float Y = gBorderData.y * gBorderData.z;


    if (_vTex.x >= X && _vTex.x <= 1.f - X
        && _vTex.y >= Y && _vTex.y <= 1.f - Y)
        return false;

    return true;
}
bool CircleLine(const float2 _vTex)
{
    float2 Mid = float2(0.5f, 0.5f);
    float BorderSize = gBorderData.x * gBorderData.z;

    float Dis = length(Mid - _vTex);

    if (Dis <= 0.5f && Dis >= 0.5f - BorderSize)
        return true;

    return false;
}
PS_OUTPUT PS_Main(VTXCOL_OUTPUT _Input)
{
    PS_OUTPUT OutData = (PS_OUTPUT) 0.f;
    
    OutData.vColor = float4( gColor, 1.f);

    if (gType == 1)
    {
        if (false == RectLine(_Input.vTex))
            clip(-1);
    }
    else if (gType == 2)
    {
        if (false == CircleLine(_Input.vTex))
            clip(-1);
    }

    return OutData;
}