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
};

struct VTX_OUTPUT
{
	float4 vPos         : SV_POSITION;
	float2 vTexCoord    : TEXCOORD;

	float3 vNormal      : NORMAL;
	float3 vBiNormal    : BINORMAL;
	float3 vTangent     : TANGENT;
	float3 vViewPos     : TEXCOORD1;
};

struct PIX_OUTPUT
{
	float4 DiffuseColor : SV_Target0;
	float4 Position : SV_Target1;
	float4 Normal : SV_Target2;
	float4 Depth : SV_Target3;
};

VTX_OUTPUT VS_Main(VTX_INPUT _Input)
{
	VTX_OUTPUT OutData = (VTX_OUTPUT) 0.f;

	OutData.vPos = mul(float4(_Input.vPos, 1.f), gWVP);
	OutData.vTexCoord = _Input.vTexCoord;

	OutData.vNormal = normalize(mul(_Input.vNormal, (float3x3) gNWV));
	OutData.vBiNormal = normalize(mul(_Input.vBiNormal, (float3x3) gNWV));
	OutData.vTangent = normalize(mul(_Input.vTangent, (float3x3) gNWV));
	OutData.vViewPos = mul(float4(_Input.vPos, 1.f), gWV).xyz;
	return OutData;
}

Texture2D Tex_BaseDiffuse : register(t0);
Texture2D Tex_BaseNormal : register(t1);
Texture2DArray Tex_Floor : register(t9);

SamplerState Sampler : register(s0);

cbuffer TerrainBuffer : register(b0)
{
	int         gFloorCount;
	float       gSizeX;
	float       gSizeZ;

	float       gNone1;
};


PIX_OUTPUT PS_Main(VTX_OUTPUT _Input)
{
	PIX_OUTPUT OutData = (PIX_OUTPUT) 0.f;

	//가장 아래 깔리는 바닥 디퓨즈, 노말
	OutData.DiffuseColor = Tex_BaseDiffuse.Sample(Sampler, _Input.vTexCoord);
	OutData.Normal = float4(GetBumpNormal(Tex_BaseNormal.Sample(Sampler, _Input.vTexCoord),
		_Input.vTangent,
		_Input.vBiNormal,
		_Input.vNormal), 0.f);

	for (int i = 0; i < gFloorCount; ++i)
	{
		float2 SplattingUv = float2(_Input.vTexCoord.x / gSizeX, _Input.vTexCoord.y / gSizeZ);
		float4 Ratio = Tex_Floor.Sample(Sampler, float3(SplattingUv.x, SplattingUv.y, 0.f));

		float4 DownColor = OutData.DiffuseColor;
		float4 UpColor = Tex_Floor.Sample(Sampler, float3(_Input.vTexCoord.x, _Input.vTexCoord.y, 1.f)).bgra;

		//이 위치에 있는 픽셀의 비율만큼 Up의 컬러와 Down의 컬러를 정한다
		OutData.DiffuseColor = (UpColor * Ratio.x) + (DownColor * (1.f - Ratio.x));

		//float4 DownNormal = OutData.Normal;
		//float4 UpNormal = float4(GetBumpNormal(Tex_Floor.Sample(Sampler, float3(_Input.vTexCoord.x, _Input.vTexCoord.y, 2.f)).bgra,
		//                            _Input.vTangent,
		//                            _Input.vBiNormal,
		//                            _Input.vNormal), 0.f);

		////이 위치에 있는 픽셀의 비율만큼 Up의 노말과 Down의 노말을 정한다
		//OutData.Normal = (UpNormal * Ratio.x) + (DownNormal * (1.f - Ratio.x));
		//OutData.Normal = normalize(OutData.Normal);
	}

	OutData.Position = float4(_Input.vViewPos, 1.f);
	OutData.Depth.x = _Input.vViewPos.z;
	OutData.Depth.y = 1.f;
	OutData.Depth.w = 1.f;

	OutData.DiffuseColor.w = 1.f;
	OutData.Normal.w = 1.f;

	return OutData;
}