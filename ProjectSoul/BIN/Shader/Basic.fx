
// 1번째 상수버퍼 , 입력값 구조체 설정 
// 2번째 버텍스 쉐이더 작성 
// 3번째 픽셀 쉐이더 작성

#define GRAYSCALE 1

cbuffer TransformBuffer :register(b0) 
{
	matrix g_WVP;
    matrix g_World;
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
	float3 vNormal : NORMAL;
	float2 vTexCoord : TEXCOORD;

};

struct VTX_OUTPUT
{
	float4 vPos : SV_POSITION;
	float3 vNormal : NORMAL;
	float2 vTexCoord : TEXCOORD;
    float4 vWorldPos : POSITIONT;
};

struct PIX_OUTPUT
{
	float4 vTexColor : SV_Target;
};

texture2D Image : register(t0);
SamplerState Sampler : register(s0);


VTX_OUTPUT VS_BasicRect(VTX_INPUT  _Input)
{
	VTX_OUTPUT OutData = (VTX_OUTPUT)0.f;
	OutData.vPos = mul(float4(_Input.vPos, 1.f), g_WVP);
    OutData.vNormal = _Input.vNormal;
	//OutData.vTexCoord = _Input.vTexCoord;

    OutData.vTexCoord.x = SpriteUV.x + SpriteUV.z * _Input.vTexCoord.x;
    OutData.vTexCoord.y = SpriteUV.y + SpriteUV.w * _Input.vTexCoord.y;

    // 클립 공간에서의 위치값을 저장
    OutData.vWorldPos = mul(float4(_Input.vPos, 1.f), g_World);

	return OutData;
}

cbuffer PixShaderBuffer : register(b0)
{
    float4 gColorFactor;
    // 일정 너비 or 높이보다 위치값이 낮을경우 해당색상을 클립(픽셀단위 Y소팅으로 사용)
    // x : 클립 너비, y : 큷립 높이  
   float2 gClipPosition;
    //  ix : x축 클립 옵션 , iy : y축 클립 옵션  , (옵션이 0 이면 적용 X)
    int2 gClipOption;
    // RenderOption.x = 랜더링 옵션
    int4 gOptions;
}


PIX_OUTPUT PS_BasicRect(VTX_OUTPUT _Input)
{
	PIX_OUTPUT OutData = (PIX_OUTPUT)0.f;
	OutData.vTexColor = Image.Sample(Sampler, _Input.vTexCoord) * gColorFactor;
    
    if (OutData.vTexColor.a == 0.f)
        clip(-1);
   
    if (gClipOption.x != 0)
    {
        
        if (gClipOption.x < 0 && gClipPosition.x > _Input.vWorldPos.x)
                clip(-1);
        
        if (gClipOption.x > 0 && gClipPosition.x < _Input.vWorldPos.x)
            clip(-1);
    }

    if (gClipOption.y != 0)
    {
        if (gClipOption.y < 0 && gClipPosition.y > _Input.vWorldPos.y)
            clip(-1);
        
        if (gClipOption.y > 0 && gClipPosition.y < _Input.vWorldPos.y)
            clip(-1);

        //if (gClipPosition.y > _Input.vWorldPos.y)
        //    clip(-1);
    }

    if (gOptions.x == GRAYSCALE)
    {
        float Gray = (OutData.vTexColor.r + OutData.vTexColor.g + OutData.vTexColor.b) / 3.f;
        OutData.vTexColor.rgb = float3(Gray, Gray, Gray);
    }

    return OutData;
}