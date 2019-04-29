#define GRAYSCALE 1

cbuffer TransformBuffer :register(b0) 
{
	matrix g_WVP;
}

cbuffer SpriteUVBuffer : register(b1)
{
    // SpriteUV.xy = UV Pos
    // SpriteUV.zw = UV Interval
    float4  SpriteUV;
}

struct VTX_INPUT 
{
	float3 vPos : POSITION;
    float3 vMeshTexCoord : TEXCOORD0;
	float2 vTexCoord : TEXCOORD1;

};

struct VTX_OUTPUT
{
	float4 vPos : SV_POSITION;
    float3 vMeshTexCoord : TEXCOORD0;
    float2 vTexCoord : TEXCOORD1;
};

VTX_OUTPUT VS_Main(VTX_INPUT _Input)
{
    VTX_OUTPUT OutData = (VTX_OUTPUT) 0.f;
    OutData.vPos = mul(float4(_Input.vPos, 1.f), g_WVP);
  
    
    OutData.vMeshTexCoord.x = _Input.vTexCoord.x;
    OutData.vMeshTexCoord.y = _Input.vTexCoord.y;
    
    OutData.vTexCoord.x = SpriteUV.x + SpriteUV.z * _Input.vTexCoord.x;
    OutData.vTexCoord.y = SpriteUV.y + SpriteUV.w * _Input.vTexCoord.y;

    return OutData;
}



struct PIX_OUTPUT
{
	float4 vTexColor : SV_Target;
};

texture2D TileImage : register(t0);
texture2D ObjectImage : register(t1);
SamplerState Sampler : register(s0);



cbuffer PlayerTraceTileBuffer : register(b0)
{
    float4 GObjectUV;
    float4 GTileUV;
    float4 GSpriteUVRatio;
    float4 GRotateAngle;
    int4 GOptions;
}

PIX_OUTPUT PS_Main(VTX_OUTPUT _Input)
{
	PIX_OUTPUT OutData = (PIX_OUTPUT)0.f;
   
    OutData.vTexColor = TileImage.Sample(Sampler, _Input.vTexCoord);
    
    if (OutData.vTexColor.a == 0.f)
        clip(-1);
      
     float Z = GRotateAngle.z;
     float2x2 RotMat = float2x2(cos(Z), -sin(Z), sin(Z), cos(Z));
     
    if (GOptions.x == GRAYSCALE)
    {
        float Color = (OutData.vTexColor.r + OutData.vTexColor.g + OutData.vTexColor.b) / 3.f;
        OutData.vTexColor.rgb = float3(Color, Color, Color);
    }


    if ((GTileUV.x < _Input.vMeshTexCoord.x && GTileUV.z > _Input.vMeshTexCoord.x)
        && (GTileUV.y < _Input.vMeshTexCoord.y && GTileUV.w > _Input.vMeshTexCoord.y))
    {

        float2 TileRatio = float2((_Input.vMeshTexCoord.x - GTileUV.x) / (GTileUV.z - GTileUV.x)
        , (_Input.vMeshTexCoord.y - GTileUV.y) / (GTileUV.w - GTileUV.y));

        float2 UVRatio;
        UVRatio.x = GSpriteUVRatio.x + (GSpriteUVRatio.z - GSpriteUVRatio.x) * TileRatio.x;
        UVRatio.y = GSpriteUVRatio.y + (GSpriteUVRatio.w - GSpriteUVRatio.y) * TileRatio.y;

        float2 SpriteUV;
        SpriteUV.x = GObjectUV.x + UVRatio.x * GObjectUV.z;
        SpriteUV.y = GObjectUV.y + UVRatio.y * GObjectUV.w;
       
        // 오브젝트가 회전한 값 만큼 UV좌표를 (0.5f,0.5f) 기점으로 회전한다
        float4 SpriteColor;
     
        //if (Z != 0.f)
        //{
        //    SpriteUV -= float2(0.5f, 0.5f);
        //    SpriteUV = mul(SpriteUV, RotMat);
        //    SpriteUV += float2(0.5f, 0.5f);
        //}

        SpriteColor = ObjectImage.Sample(Sampler, SpriteUV);

        if (SpriteColor.a != 0.f)
           OutData.vTexColor.rgb = OutData.vTexColor.rgb * 0.3f;
   
    }
    


    return OutData;
}