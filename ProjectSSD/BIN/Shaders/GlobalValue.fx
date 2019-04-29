
cbuffer RenBaseBuffer : register(b13) 
{
    float4x4 gWorld;
    float4x4 gInvWorld;
    float4x4 gNormalWorld;
    float4x4 gView;
    float4x4 gProj;
    float4x4 gWV;
    float4x4 gNWV;
    float4x4 gVP;
    float4x4 gWVP;
    float4   gColorFactor;
	float4	 gCamWorldPos;
};

static matrix IdentityMatrix = {{ 1.0f, 0.0f, 0.0f, 0.0f },
                                { 0.0f, 1.0f, 0.0f, 0.0f },
                                { 0.0f, 0.0f, 1.0f, 0.0f },
                                { 0.0f, 0.0f, 0.0f, 1.0f }};

static float4 Zero			=float4(0.f, 0.f, 0.f, 0.f);
static float4 One			=float4(1.f, 1.f, 1.f, 1.f);
static float4 White			=float4(1.f, 1.f, 1.f, 1.f);
static float4 Black			=float4(0.f, 0.f, 0.f, 1.f);
static float4 Red			=float4(1.f, 0.f, 0.f, 1.f);
static float4 Green			=float4(0.f, 1.f, 0.f, 1.f);
static float4 Blue			=float4(0.f, 0.f, 1.f, 1.f);
static float4 Yellow		=float4(1.f, 1.f, 0.f, 1.f);
static float4 Cyan			=float4(0.f, 1.f, 1.f, 1.f);
static float4 Magenta		=float4(1.f, 0.f, 1.f, 1.f);
static float4 Pink			=float4(0.94f, 0.72f, 0.78f, 1.f);
static float4 PastelRed		=float4(1.f, 0.43f, 0.43f, 1.f);
static float4 Mint			=float4(0.81f, 1.f, 0.89f, 1.f);
static float4 LightYello	=float4(1.f, 0.89f, 0.45f, 1.f);
static float4 PastelGreen	=float4(0.71f, 1.f, 0.61f, 1.f);
static float4 Lavender		=float4(0.58f, 0.62f, 0.74f, 1.f);
static float Far            = 50000.f;