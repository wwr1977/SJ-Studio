// 타일맵 인스턴싱용 쉐이더
#define GRAYSCALE 1

cbuffer GlobalInstanceBuffer : register(b0)
{
	matrix g_VP;
    float4 g_TileData;                  //  (x,y,z) : 타일의 크기 w = 타일의 깊이값
}

struct VTX_INPUT 
{
	float3 vPos : POSITION;
	float3 vNormal : NORMAL;
	float2 vTexCoord : TEXCOORD;
    float3 InstancePos : WORLDPOSITION;
    float4 TileUV : SPRITEUV;
    uint   InstaceId : SV_InstanceID;
};

struct VTX_OUTPUT
{
	float4 vPos_Clip : SV_POSITION;
    float4 vPos_World : POSITIONT;
	float3 vNormal : NORMAL;
	float2 vTexCoord : TEXCOORD;

};

struct PIX_OUTPUT
{
	float4 vTexColor : SV_Target;
};

texture2D Image : register(t0);
SamplerState Sampler : register(s0);


VTX_OUTPUT VS_Main(VTX_INPUT _Input)
{
	VTX_OUTPUT OutData = (VTX_OUTPUT)0.f;

    float4x4 WorldMat = float4x4(  g_TileData.x, 0.f, 0.f, 0.f
                                 , 0.f, g_TileData.y, 0.f, 0.f
                                 , 0.f, 0.f, g_TileData.z, 0.f
                                 , _Input.InstancePos.x, _Input.InstancePos.y, g_TileData.w , 1.f);


    OutData.vPos_World = mul(float4(_Input.vPos, 1.f), WorldMat);
    OutData.vPos_Clip = mul(OutData.vPos_World, g_VP);
    OutData.vNormal = _Input.vNormal;

    OutData.vTexCoord.x = _Input.TileUV.x + _Input.TileUV.z * _Input.vTexCoord.x;
    OutData.vTexCoord.y = _Input.TileUV.y + _Input.TileUV.w * _Input.vTexCoord.y;


	return OutData;
}

cbuffer PixShaderBuffer : register(b0)
{
    float4 gColorFactor;
    int4 gOptions;
}


PIX_OUTPUT PS_Main(VTX_OUTPUT _Input)
{
	PIX_OUTPUT OutData = (PIX_OUTPUT)0.f;


    OutData.vTexColor = Image.Sample(Sampler, _Input.vTexCoord) * gColorFactor;
    
    if (OutData.vTexColor.a == 0.f)
        clip(-1);

    if (gOptions.x == GRAYSCALE)
    {
        float Color = (OutData.vTexColor.r + OutData.vTexColor.g + OutData.vTexColor.b) / 3.f;
        OutData.vTexColor.rgb = float3(Color, Color, Color);
    }

   return OutData;
}